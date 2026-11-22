from __future__ import annotations

import importlib.util
import io
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]


def load_module(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


class CaxecraftTimingTests(unittest.TestCase):
    def test_compiler_profile_timeout_keeps_partial_diagnostics(self) -> None:
        profiler = load_module(
            "caxecraft_profile_partial_timeout_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        for output, error_output, expected in (
            (
                b"HXC_PHASE_TIMING\ttyped input capture\t123\n",
                b"C printing\npartial UTF-8: \xe2",
                ("typed input capture\t123", "C printing", "partial UTF-8: \ufffd"),
            ),
            ("partial stdout", "partial stderr", ("partial stdout", "partial stderr")),
            (None, None, ()),
        ):
            with self.subTest(output=output), tempfile.TemporaryDirectory() as directory:
                report_path = Path(directory) / "profile.json"
                timeout = subprocess.TimeoutExpired(
                    ["haxe"], 120, output=output, stderr=error_output
                )
                diagnostics = io.StringIO()
                with mock.patch.object(profiler, "profile", side_effect=timeout), mock.patch.object(
                    sys, "stderr", diagnostics
                ):
                    status = profiler.main(["--output", str(report_path)])
                self.assertEqual(status, 1)
                self.assertIn("timed out after 120 seconds", diagnostics.getvalue())
                for fragment in expected:
                    self.assertIn(fragment, diagnostics.getvalue())
                self.assertFalse(report_path.exists())

    def test_compiler_profile_timeout_override_is_bounded_and_explicit(self) -> None:
        profiler = load_module(
            "caxecraft_profile_timeout_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )

        self.assertIsNone(profiler.parse_args([]).timeout_seconds)
        self.assertEqual(
            profiler.parse_args(["--timeout-seconds", "600"]).timeout_seconds,
            600,
        )
        for timeout in (0, 601):
            with self.assertRaisesRegex(
                profiler.CompilerProfileFailure,
                "timeout must be from 1 through 600",
            ):
                profiler.profile(1, timeout_seconds=timeout)

    def test_secondary_locale_pilot_has_a_measured_bounded_budget(self) -> None:
        with mock.patch.object(
            sys, "path", [str(ROOT / "examples/caxecraft"), *sys.path]
        ):
            play = load_module(
                "caxecraft_pilot_timeout_subject",
                ROOT / "examples/caxecraft/play.py",
            )

        self.assertEqual(play.pilot_timeout_seconds("secondary-locale"), 35)
        self.assertEqual(play.pilot_timeout_seconds("editor-shell"), 90)
        self.assertEqual(play.pilot_timeout_seconds("adventure-journey"), 35)
        self.assertEqual(play.pilot_timeout_seconds("launch-smoke"), 15)

    def test_representation_owner_details_are_required(self) -> None:
        profiler = load_module("representation_details_subject", ROOT / "examples/caxecraft/profile_compiler.py")
        expected = ("dynamic discovery", "managed discovery", "managed completion",
                    "shared-body discovery", "canonical export", "replay revision")
        for suffix in expected:
            detail = "HxcIR representation " + suffix
            self.assertIn(detail, profiler.DETAIL_PHASES)
            self.assertNotIn(detail, profiler.PHASES)
            remaining = "\n".join(f"HXC_DETAIL_TIMING\t{name}\t1"
                                  for name in profiler.DETAIL_PHASES if name != detail)
            with self.assertRaisesRegex(profiler.CompilerProfileFailure, "omitted"):
                profiler.parse_detail_records(remaining)
        with self.assertRaises(profiler.CompilerProfileFailure):
            profiler.parse_detail_records("HXC_DETAIL_TIMING\tHxcIR representation unknown\t1")

    def test_compiler_profile_parses_closed_phase_and_haxe_timer_records(self) -> None:
        profiler = load_module(
            "caxecraft_compiler_profile_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        phase_lines = "\n".join(
            f"HXC_PHASE_TIMING\t{phase}\t{index + 1}"
            for index, phase in enumerate(profiler.PHASES)
        )
        self.assertEqual(
            profiler.parse_phase_records(phase_lines),
            {phase: index + 1 for index, phase in enumerate(profiler.PHASES)},
        )
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure, "unknown or repeated"
        ):
            profiler.parse_phase_records(
                phase_lines + "\nHXC_PHASE_TIMING\ttarget pipeline\t1"
            )

        detail_lines = "\n".join(
            f"HXC_DETAIL_TIMING\t{detail}\t{index + 1}"
            for index, detail in enumerate(profiler.DETAIL_PHASES)
        )
        self.assertEqual(
            profiler.parse_detail_records(detail_lines),
            {
                detail: index + 1
                for index, detail in enumerate(profiler.DETAIL_PHASES)
            },
        )
        self.assertEqual(
            profiler.parse_detail_records(detail_lines + "\n" + detail_lines),
            {
                detail: (index + 1) * 2
                for index, detail in enumerate(profiler.DETAIL_PHASES)
            },
        )
        hit_only_lines = "\n".join(
            f"HXC_DETAIL_TIMING\t{detail}\t{index + 1}"
            for index, detail in enumerate(profiler.DETAIL_PHASES)
            if detail not in profiler.CONTROL_FLOW_PLAN_COMPUTATION_DETAILS
        )
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure, "compiler detail report omitted"
        ):
            profiler.parse_detail_records(hit_only_lines)
        hit_only_records = profiler.parse_detail_records(
            hit_only_lines,
            allowed_missing=profiler.CONTROL_FLOW_PLAN_COMPUTATION_DETAILS,
        )
        for detail in profiler.CONTROL_FLOW_PLAN_COMPUTATION_DETAILS:
            self.assertEqual(hit_only_records[detail], 0)

        table = "\n".join(
            (
                "name                 | time(s) |   % |  p% |     # | info",
                "--------------------------------------------------------",
                "macro                |   0.100 |  90 |  90 |  1000 | ",
                "  hxc target pipeline |   0.050 |  45 |  50 |     1 | ",
                "--------------------------------------------------------",
                "total                |   0.120 | 100 | 100 | 12000 | ",
            )
        )
        rows = profiler.parse_haxe_timer_rows(
            table, profiler.TimerClock(2.0, "test", None, None)
        )
        self.assertEqual(
            [(row.path, row.depth, row.adjusted_duration_ms) for row in rows],
            [
                ("macro", 0, 200.0),
                ("macro/hxc target pipeline", 1, 100.0),
                ("total", 0, 240.0),
            ],
        )

    def test_compiler_profile_validates_nested_structured_records(self) -> None:
        profiler = load_module(
            "caxecraft_structured_profile_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )

        def record(value: dict[str, object]) -> str:
            return "HXC_PROFILE\t" + json.dumps(value, separators=(",", ":"))

        child = {
            "schemaVersion": profiler.PROFILE_SCHEMA_VERSION,
            "recordKind": "span",
            "requestId": "request-1",
            "spanId": 2,
            "parentSpanId": 1,
            "category": "phase",
            "name": "runtime planning",
            "subject": None,
            "work": None,
            "status": "ok",
            "startOffsetMicroseconds": 20.0,
            "inclusiveWallMicroseconds": 60.0,
            "exclusiveWallMicroseconds": 60.0,
            "inclusiveCpuMicroseconds": 60.0,
            "exclusiveCpuMicroseconds": 60.0,
            "allocatedBytesDelta": 12.0,
            "residentBytesAtEnd": 1024.0,
        }
        parent = {
            "schemaVersion": profiler.PROFILE_SCHEMA_VERSION,
            "recordKind": "span",
            "requestId": "request-1",
            "spanId": 1,
            "parentSpanId": None,
            "category": "phase",
            "name": "target pipeline",
            "subject": None,
            "work": None,
            "status": "ok",
            "startOffsetMicroseconds": 0.0,
            "inclusiveWallMicroseconds": 100.0,
            "exclusiveWallMicroseconds": 40.0,
            "inclusiveCpuMicroseconds": 80.0,
            "exclusiveCpuMicroseconds": 20.0,
            "allocatedBytesDelta": 20.0,
            "residentBytesAtEnd": 1024.0,
        }
        counter = {
            "schemaVersion": profiler.PROFILE_SCHEMA_VERSION,
            "recordKind": "counter",
            "requestId": "request-1",
            "name": "runtime.requirements",
            "value": 3,
        }
        request = {
            "schemaVersion": profiler.PROFILE_SCHEMA_VERSION,
            "recordKind": "request",
            "requestId": "request-1",
            "status": "ok",
            "profile": "portable",
            "buildMode": "debug",
            "haxeVersion": "5.0.0-preview.1",
            "wallMicroseconds": 120.0,
            "cpuMicroseconds": 100.0,
            "allocatedBytesDelta": 32.0,
            "maximumObservedResidentBytes": 1024.0,
            "spanCount": 2,
            "counterCount": 1,
        }
        stream = "\n".join(
            (record(child), record(parent), record(counter), record(request))
        )
        parsed = profiler.parse_profile_records(stream, expected_status="ok")
        self.assertEqual(parsed.request.span_count, 2)
        self.assertEqual(parsed.counters, (("runtime.requirements", 3.0),))
        wall, cpu = profiler.exclusive_accounting(150, parsed)
        self.assertEqual(wall["runtime planning"], 60.0)
        self.assertEqual(wall["target pipeline"], 40.0)
        self.assertEqual(wall["profile request unspanned remainder"], 20.0)
        self.assertEqual(wall["external process/frontend remainder"], 30.0)
        self.assertEqual(cpu["profile request unspanned remainder"], 20.0)
        work_child = dict(child)
        work_child.update(
            {
                "category": "detail",
                "name": "body control-flow planning",
                "subject": "demo.Main.main",
                "work": {
                    "kind": "normal-join-search-v1",
                    "controlFlow": {
                        "blockCount": 4,
                        "normalJoinSearches": 2,
                        "normalJoinCandidateProofs": 2,
                        "normalJoinDistanceSearches": 4,
                        "normalJoinDistanceBlockVisits": 12,
                        "completionSetSearches": 2,
                        "completionSetInitialBlockScans": 12,
                        "completionSetWorklistDequeues": 3,
                        "abruptCompletionSetSearches": 1,
                        "abruptCompletionSetInitialBlockScans": 4,
                        "abruptCompletionSetWorklistDequeues": 1,
                        "forwardReachabilitySearches": 4,
                        "forwardReachabilityBlockVisits": 12,
                        "prefixDisjointSearches": 2,
                        "prefixDisjointBlockVisits": 8,
                    },
                    "typedBody": None,
                    "functionBuild": None,
                    "printer": None,
                },
            }
        )
        work_profile = profiler.parse_profile_records(
            "\n".join(
                (
                    record(work_child),
                    record(parent),
                    record(counter),
                    record(request),
                )
            ),
            expected_status="ok",
        )
        self.assertEqual(work_profile.spans[0].subject, "demo.Main.main")
        self.assertEqual(work_profile.spans[0].work.block_count, 4)
        typed_body_child = dict(child)
        typed_body_child.update(
            {
                "category": "detail",
                "name": "HxcIR typed-body lowering",
                "subject": "method.demo.Main.run",
                "work": {
                    "kind": "typed-body-lowering-v1",
                    "controlFlow": None,
                    "typedBody": {
                        "expressionNodeCount": 12,
                        "statementLoweringCalls": 5,
                        "valueLoweringCalls": 7,
                        "bodyValueTypeRequests": 4,
                        "directPrimitiveFastPaths": 3,
                        "stringTypeClassifications": 1,
                        "stringTypeCpuMicroseconds": 10.5,
                        "recordTypeClassifications": 1,
                        "recordTypeCpuMicroseconds": 20.5,
                        "collectionTypeClassifications": 1,
                        "collectionTypeCpuMicroseconds": 30.5,
                        "nominalTypeClassifications": 1,
                        "nominalTypeCpuMicroseconds": 40.5,
                        "callableOptionalTypeClassifications": 1,
                        "callableOptionalTypeCpuMicroseconds": 50.5,
                        "otherTypeClassifications": 1,
                        "otherTypeCpuMicroseconds": 60.5,
                        "specializationRequests": 6,
                        "specializationCpuMicroseconds": 65.5,
                        "coercionRequests": 3,
                        "coercionTypeComparisonCpuMicroseconds": 66.5,
                        "sourceSpanRequests": 11,
                        "sourceSpanComputations": 8,
                        "sourceSpanCacheHits": 3,
                        "sourceSpanCpuMicroseconds": 70.5,
                        "producedBlockCount": 2,
                        "producedInstructionCount": 9,
                    },
                    "functionBuild": None,
                    "printer": None,
                },
            }
        )
        typed_body_profile = profiler.parse_profile_records(
            "\n".join(
                (
                    record(typed_body_child),
                    record(parent),
                    record(counter),
                    record(request),
                )
            ),
            expected_status="ok",
        )
        self.assertEqual(
            typed_body_profile.spans[0].work.expression_node_count, 12
        )
        self.assertEqual(
            typed_body_profile.spans[0].work.direct_primitive_fast_paths, 3
        )
        self.assertEqual(
            typed_body_profile.spans[0].work.source_span_computations, 8
        )
        malformed_source_span_work = json.loads(json.dumps(typed_body_child))
        malformed_source_span_work["work"]["typedBody"]["sourceSpanCacheHits"] = 2
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure,
            "must account for every request",
        ):
            profiler.parse_profile_records(
                "\n".join(
                    (
                        record(malformed_source_span_work),
                        record(parent),
                        record(counter),
                        record(request),
                    )
                )
            )

        function_build_child = dict(child)
        function_build_child.update(
            {
                "category": "detail",
                "name": "HxcIR function build",
                "subject": "method.demo.Main.run",
                "work": {
                    "kind": "hxcir-function-build-contributions-v1",
                    "controlFlow": None,
                    "typedBody": None,
                    "functionBuild": {
                        "builtLocals": 2,
                        "builtBlocks": 3,
                        "builtInstructions": 8,
                        "builtCleanupRegions": 1,
                        "builtRuntimeRequirements": 2,
                        "builtLocalNameRequests": 2,
                        "builtSpanLengthNameRequests": 0,
                        "builtTemporaryNameRequests": 3,
                        "builtTailArgumentNameRequests": 1,
                        "builtLabelNameRequests": 2,
                        "addedAggregates": 1,
                        "addedEnums": 0,
                        "addedClasses": 0,
                        "addedInterfaces": 0,
                        "addedArrays": 1,
                        "addedIntMaps": 0,
                        "addedStringMaps": 0,
                        "addedTypedMaps": 0,
                        "addedBytes": 0,
                        "addedOptionals": 0,
                        "addedImportTypes": 1,
                        "addedImportFunctions": 0,
                        "addedImportConstants": 0,
                        "addedImportOwners": 1,
                        "addedEnumConstructorAdapters": 0,
                        "addedFunctionLiterals": 0,
                        "addedStaticFunctionAdapters": 0,
                        "addedSymbolRequests": 5,
                    },
                    "printer": None,
                },
            }
        )
        function_build_profile = profiler.parse_profile_records(
            "\n".join(
                (
                    record(function_build_child),
                    record(parent),
                    record(counter),
                    record(request),
                )
            ),
            expected_status="ok",
        )
        self.assertEqual(
            function_build_profile.spans[0].work.built_instructions, 8
        )
        contribution_summary = profiler.function_build_contribution_summary(
            function_build_profile
        )
        self.assertEqual(contribution_summary["functionCount"], 1)
        self.assertEqual(
            contribution_summary["semanticSharedContributionFunctions"], 1
        )
        self.assertEqual(
            contribution_summary["zeroSemanticSharedContributionFunctions"], 0
        )
        self.assertEqual(
            contribution_summary["totals"]["addedSymbolRequests"], 5
        )
        self.assertEqual(
            contribution_summary["contributors"],
            [
                {
                    "functionId": "method.demo.Main.run",
                    "added": {
                        "addedAggregates": 1,
                        "addedArrays": 1,
                        "addedImportOwners": 1,
                        "addedImportTypes": 1,
                        "addedSymbolRequests": 5,
                    },
                }
            ],
        )
        zero_function_build_child = json.loads(
            json.dumps(function_build_child)
        )
        for field in profiler.FUNCTION_BUILD_WORK_FIELDS:
            if field.startswith("added"):
                zero_function_build_child["work"]["functionBuild"][field] = 0
        zero_function_build_profile = profiler.parse_profile_records(
            "\n".join(
                (
                    record(zero_function_build_child),
                    record(parent),
                    record(counter),
                    record(request),
                )
            ),
            expected_status="ok",
        )
        zero_contribution_summary = (
            profiler.function_build_contribution_summary(
                zero_function_build_profile
            )
        )
        self.assertEqual(
            zero_contribution_summary[
                "zeroSemanticSharedContributionFunctions"
            ],
            1,
        )
        self.assertEqual(
            zero_contribution_summary["allZeroSharedContributionFunctions"],
            1,
        )
        self.assertEqual(zero_contribution_summary["contributors"], [])

        printer_child = dict(child)
        printer_child.update(
            {
                "category": "detail",
                "name": "C translation-unit printing",
                "subject": "src/modules/demo/Main.c",
                "work": {
                    "kind": "c-printer-v1",
                    "controlFlow": None,
                    "typedBody": None,
                    "functionBuild": None,
                    "printer": {
                        "declarationCount": 3,
                        "statementCount": 9,
                        "expressionCount": 14,
                        "outputBytes": 512,
                        "indentationRequests": 11,
                        "indentationUnitCopies": 17,
                        "tokenJoinCalls": 8,
                        "tokenJoinInputs": 21,
                        "tokenJoinOutputs": 15,
                        "uniquenessCheckCalls": 6,
                        "uniquenessCheckInputs": 4,
                        "utf8EncodingCalls": 2,
                        "utf8InputCodeUnits": 18,
                    },
                },
            }
        )
        printer_profile = profiler.parse_profile_records(
            "\n".join(
                (
                    record(printer_child),
                    record(parent),
                    record(counter),
                    record(request),
                )
            ),
            expected_status="ok",
        )
        self.assertEqual(printer_profile.spans[0].work.output_bytes, 512)
        self.assertEqual(printer_profile.spans[0].work.token_join_inputs, 21)

        mixed_work_child = json.loads(json.dumps(typed_body_child))
        mixed_work_child["work"]["controlFlow"] = work_child["work"]["controlFlow"]
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure,
            "typed-body span work requires only a typedBody payload",
        ):
            profiler.parse_profile_records(
                "\n".join(
                    (
                        record(mixed_work_child),
                        record(parent),
                        record(counter),
                        record(request),
                    )
                )
            )

        unknown_work_child = json.loads(json.dumps(typed_body_child))
        unknown_work_child["work"] = {
            "kind": "future-unregistered-work-v1",
            "controlFlow": None,
            "typedBody": None,
            "functionBuild": None,
            "printer": None,
        }
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure, "unknown kind"
        ):
            profiler.parse_profile_records(
                "\n".join(
                    (
                        record(unknown_work_child),
                        record(parent),
                        record(counter),
                        record(request),
                    )
                )
            )

        self.assertEqual(
            profiler.distribution([1.0, 2.0, 3.0, 4.0, 5.0]),
            {
                "samples": 5,
                "minimum": 1.0,
                "median": 3.0,
                "medianAbsoluteDeviation": 1.0,
                "p95": 4.8,
                "maximum": 5.0,
            },
        )
        self.assertEqual(
            profiler.host_condition((5.5, 6.0), 12), "representative"
        )
        self.assertEqual(profiler.host_condition((5.5, 6.1), 12), "contended")

        missing_parent = dict(child)
        missing_parent["parentSpanId"] = 99
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure, "missing parent"
        ):
            profiler.parse_profile_records(
                "\n".join(
                    (
                        record(missing_parent),
                        record(parent),
                        record(counter),
                        record(request),
                    )
                )
            )

        inconsistent = dict(parent)
        inconsistent["exclusiveWallMicroseconds"] = 30.0
        with self.assertRaisesRegex(
            profiler.CompilerProfileFailure, "inconsistent exclusive wall"
        ):
            profiler.parse_profile_records(
                "\n".join(
                    (
                        record(child),
                        record(inconsistent),
                        record(counter),
                        record(request),
                    )
                )
            )

    def test_compiler_profile_preserves_the_active_failed_span(self) -> None:
        profiler = load_module(
            "caxecraft_failed_profile_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        span = {
            "schemaVersion": profiler.PROFILE_SCHEMA_VERSION,
            "recordKind": "span",
            "requestId": "request-1",
            "spanId": 1,
            "parentSpanId": None,
            "category": "phase",
            "name": "configuration and contracts",
            "subject": None,
            "work": None,
            "status": "failed",
            "startOffsetMicroseconds": 0,
            "inclusiveWallMicroseconds": 10,
            "exclusiveWallMicroseconds": 10,
            "inclusiveCpuMicroseconds": 8,
            "exclusiveCpuMicroseconds": 8,
            "allocatedBytesDelta": None,
            "residentBytesAtEnd": None,
        }
        request = {
            "schemaVersion": profiler.PROFILE_SCHEMA_VERSION,
            "recordKind": "request",
            "requestId": "request-1",
            "status": "failed",
            "profile": "portable",
            "buildMode": "unresolved",
            "haxeVersion": "5.0.0-preview.1",
            "wallMicroseconds": 12,
            "cpuMicroseconds": 9,
            "allocatedBytesDelta": None,
            "maximumObservedResidentBytes": None,
            "spanCount": 1,
            "counterCount": 0,
        }
        stream = "\n".join(
            "HXC_PROFILE\t" + json.dumps(value, separators=(",", ":"))
            for value in (span, request)
        )
        parsed = profiler.parse_profile_records(stream, expected_status="failed")
        self.assertEqual(parsed.request.status, "failed")
        self.assertEqual(parsed.spans[0].status, "failed")

    def test_compiler_profile_accounting_does_not_double_count_parents(self) -> None:
        profiler = load_module(
            "caxecraft_compiler_accounting_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        phases = {phase: 100 for phase in profiler.PHASES}
        phases["semantic lowering"] = 500
        phases["target pipeline"] = 1200
        accounting = profiler.accounting_durations(1700, phases)

        self.assertEqual(tuple(accounting), profiler.ACCOUNTING_PHASES)
        self.assertEqual(accounting["semantic lowering remainder"], 100)
        self.assertEqual(accounting["target pipeline remainder"], 0)
        self.assertEqual(accounting["host/frontend/setup remainder"], 300)
        self.assertEqual(sum(accounting.values()), 1700)

    def test_compiler_profile_uses_the_reviewed_haxe_source_revision(self) -> None:
        profiler = load_module(
            "caxecraft_compiler_pin_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        lock = json.loads(
            (ROOT / "docs/specs/toolchain-lock.json").read_text(encoding="utf-8")
        )
        self.assertEqual(
            profiler.PINNED_HAXE_SOURCE_REVISION,
            lock["haxe"]["sourceRevision"],
        )

    def test_compiler_profile_matches_ordinary_play_on_each_platform(self) -> None:
        profiler = load_module(
            "caxecraft_playable_profile_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        play = load_module(
            "caxecraft_playable_arguments_subject",
            ROOT / "examples/caxecraft/play.py",
        )
        output = Path("generated-playable-profile")
        for host, target in (("darwin", "macos"), ("linux", "linux"), ("win32", "windows")):
            with (
                self.subTest(platform=target),
                mock.patch.object(profiler.sys, "platform", host),
                mock.patch.object(
                    profiler, "resolve_haxe_arguments",
                    side_effect=lambda arguments, *, locale: tuple(arguments),
                ),
                mock.patch.object(play, "run") as run_process,
                mock.patch.object(play, "validate_compiled_haxe", return_value={}),
                mock.patch.object(play, "validate_content_platform_output"),
                mock.patch("builtins.print"),
            ):
                arguments = list(profiler.workload_arguments(output, "playable"))
                play.compile_haxe(
                    output, layout="split", platform_name=target,
                    raylib_configuration="desktop",
                )
                # Timing instrumentation is the only permitted difference from
                # the ordinary build, including its real content adapter.
                timing_define = arguments.index("reflaxe_c_phase_timing")
                del arguments[timing_define - 1 : timing_define + 1]
                arguments.remove("--times")
                self.assertEqual(tuple(arguments), tuple(run_process.call_args.args[0][3:]))

    def test_compiler_profile_can_match_the_editor_shell_request(self) -> None:
        profiler = load_module(
            "caxecraft_editor_profile_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        output = Path("generated-editor-profile")

        with (
            mock.patch.object(profiler.sys, "platform", "darwin"),
            mock.patch.object(
                profiler,
                "resolve_haxe_arguments",
                side_effect=lambda arguments, *, locale: tuple(arguments),
            ),
        ):
            arguments = profiler.workload_arguments(output, "editor-shell")

        self.assertEqual(
            arguments,
            (
                "play.hxml",
                "-D",
                "hxc_runtime_diagnostics=off",
                "-D",
                "raylib_platform_macos",
                "-D",
                "raylib_configuration_memory",
                "-D",
                "hxc_runtime_report=summary",
                "-D",
                "reflaxe_c_phase_progress",
                "-D",
                "caxecraft_posix_hosted",
                "-D",
                "caxecraft_posix_darwin",
                "-D",
                "caxecraft_pilot",
                "-D",
                "caxecraft_pilot_editor_shell",
                "-D",
                "reflaxe_c_phase_timing",
                "--times",
                "--custom-target",
                f"c={output}",
            ),
        )

        play = load_module(
            "caxecraft_editor_play_arguments_subject",
            ROOT / "examples/caxecraft/play.py",
        )
        with (
            mock.patch.object(play, "run") as run_process,
            mock.patch.object(play, "validate_compiled_haxe", return_value={}),
            mock.patch.object(play, "validate_content_platform_output"),
            mock.patch("builtins.print"),
        ):
            play.compile_haxe(
                output,
                layout="split",
                platform_name="macos",
                raylib_configuration="memory-software",
                pilot="editor-shell",
            )

        play_arguments = tuple(run_process.call_args.args[0][3:])
        profiled_play_arguments = list(arguments)
        timing_define = profiled_play_arguments.index("reflaxe_c_phase_timing")
        del profiled_play_arguments[timing_define - 1 : timing_define + 1]
        profiled_play_arguments.remove("--times")
        self.assertEqual(tuple(profiled_play_arguments), play_arguments)

    def test_compiler_profile_can_match_the_runtime_level_loader_request(
        self,
    ) -> None:
        profiler = load_module(
            "caxecraft_runtime_level_loader_profile_subject",
            ROOT / "examples/caxecraft/profile_compiler.py",
        )
        output = Path("generated-runtime-level-loader-profile")

        with (
            mock.patch.object(profiler.sys, "platform", "darwin"),
            mock.patch.object(
                profiler,
                "resolve_haxe_arguments",
                side_effect=lambda arguments, *, locale: tuple(arguments),
            ),
        ):
            arguments = profiler.workload_arguments(output, "runtime-level-loader")

        self.assertIn("runtime-level-loader", profiler.PROFILE_WORKLOADS)
        self.assertEqual(
            arguments,
            (
                "runtime-level-loader-c.hxml",
                "-D",
                "hxc_runtime_diagnostics=off",
                "-D",
                "caxecraft_posix_hosted",
                "-D",
                "caxecraft_posix_darwin",
                "-D",
                "hxc_project_layout=split",
                "-D",
                "reflaxe_c_phase_timing",
                "--times",
                "--custom-target",
                f"c={output}",
            ),
        )

    def test_pinned_haxe_resolution_uses_the_haxerc_version(self) -> None:
        caxecraft = load_module(
            "caxecraft_pin_subject", ROOT / "examples/caxecraft/run.py"
        )
        with tempfile.TemporaryDirectory(prefix="hxc-haxe-pin-test-") as temporary:
            root = Path(temporary)
            scope = root / "scope"
            haxe_root = root / "haxe-root"
            installation = haxe_root / "versions" / "5.0.0-preview.1"
            scope.mkdir()
            (scope / ".haxerc").write_text(
                '{"version":"5.0.0-preview.1","resolveLibs":"scoped"}\n',
                encoding="utf-8",
            )
            installation.mkdir(parents=True)
            (installation / "std").mkdir()
            compiler = installation / ("haxe.exe" if os.name == "nt" else "haxe")
            compiler.write_text("test compiler\n", encoding="utf-8")
            compiler.chmod(0o755)

            observed = caxecraft.pinned_haxe_installation(
                scope_root=scope,
                environment={"HAXE_ROOT": str(haxe_root)},
            )

            self.assertEqual(observed.version, "5.0.0-preview.1")
            self.assertEqual(observed.compiler, compiler.resolve())
            self.assertEqual(
                observed.standard_library, (installation / "std").resolve()
            )

            (scope / ".haxerc").write_text(
                '{"version":"../4.3.7","resolveLibs":"scoped"}\n',
                encoding="utf-8",
            )
            with self.assertRaisesRegex(
                caxecraft.CaxecraftFailure, "unsafe or missing"
            ):
                caxecraft.pinned_haxe_installation(
                    scope_root=scope,
                    environment={"HAXE_ROOT": str(haxe_root)},
                )

    def test_server_context_uses_loopback_and_reaps_on_interrupt(self) -> None:
        caxecraft = load_module(
            "caxecraft_server_subject", ROOT / "examples/caxecraft/run.py"
        )
        installation = caxecraft.HaxeInstallation(
            "5.0.0-preview.1",
            Path("/test/haxe-5/haxe"),
            Path("/test/haxe-5/std"),
            Path("/test/haxelib"),
            None,
        )
        server = mock.Mock()
        server.poll.return_value = None
        server.wait.side_effect = [
            subprocess.TimeoutExpired(cmd="haxe server", timeout=5),
            0,
        ]
        with (
            mock.patch.object(
                caxecraft,
                "pinned_haxe_installation",
                return_value=installation,
            ),
            mock.patch.object(caxecraft, "verify_pinned_haxe"),
            mock.patch.object(caxecraft, "available_port", return_value=48123),
            mock.patch.object(caxecraft, "wait_for_server") as wait_for_server,
            mock.patch.object(
                caxecraft.subprocess, "Popen", return_value=server
            ) as popen,
        ):
            with self.assertRaises(KeyboardInterrupt):
                with caxecraft.haxe_compilation_server() as connection:
                    self.assertEqual(connection.endpoint, "127.0.0.1:48123")
                    self.assertIs(connection.installation, installation)
                    raise KeyboardInterrupt()

        command = popen.call_args.args[0]
        self.assertEqual(
            command,
            [
                "/test/haxe-5/haxe",
                "--server-listen",
                "127.0.0.1:48123",
            ],
        )
        self.assertNotIn("HAXE_NO_SERVER", popen.call_args.kwargs["env"])
        wait_for_server.assert_called_once_with(server, 48123)
        server.terminate.assert_called_once_with()
        server.kill.assert_called_once_with()

    def test_native_server_client_receives_only_resolved_arguments(self) -> None:
        caxecraft = load_module(
            "caxecraft_client_subject", ROOT / "examples/caxecraft/run.py"
        )
        installation = caxecraft.HaxeInstallation(
            "5.0.0-preview.1",
            Path("/test/haxe-5/haxe"),
            Path("/test/haxe-5/std"),
            Path("/test/haxelib"),
            None,
        )
        connection = caxecraft.HaxeServerConnection(
            "127.0.0.1:48123", installation
        )
        completed = subprocess.CompletedProcess([], 0, "ok", "")
        with (
            mock.patch.object(
                caxecraft,
                "resolve_haxe_arguments",
                return_value=("-cp", "src", "--custom-target", "c=/test/out"),
            ) as resolve,
            mock.patch.object(
                caxecraft, "run_bounded_process", return_value=completed
            ) as run,
        ):
            observed = caxecraft.compile_target(
                Path("/test/out"), connect=connection
            )

        self.assertIs(observed, completed)
        resolve.assert_called_once()
        command = run.call_args.args[0]
        self.assertEqual(
            command,
            [
                "/test/haxe-5/haxe",
                "--connect",
                "127.0.0.1:48123",
                "-cp",
                "src",
                "--custom-target",
                "c=/test/out",
            ],
        )
        self.assertNotIn("--haxe-version", command)
        self.assertEqual(run.call_args.kwargs["cwd"], caxecraft.CASE)
        self.assertEqual(
            run.call_args.kwargs["env"]["HAXE_VERSION"], "5.0.0-preview.1"
        )

    def test_full_recorder_matches_the_toolchain_phase_contract(self) -> None:
        caxecraft = load_module(
            "caxecraft_timing_subject", ROOT / "examples/caxecraft/run.py"
        )
        toolchain = load_module(
            "toolchain_timing_subject", ROOT / "scripts/ci/run_toolchain_shard.py"
        )
        recorder = caxecraft.TimingRecorder()
        for name, requests in toolchain.CAXECRAFT_FULL_PHASES:
            with recorder.phase(name, haxe_requests=requests):
                pass

        report = recorder.report(mode="full", outcome="passed")
        self.assertIs(
            toolchain.validate_caxecraft_timing_report(report),
            report,
        )
        self.assertEqual(report["summary"], {"haxeRequests": 12})

    def test_failed_phase_is_retained_in_a_failure_prefix(self) -> None:
        caxecraft = load_module(
            "caxecraft_failure_timing_subject", ROOT / "examples/caxecraft/run.py"
        )
        recorder = caxecraft.TimingRecorder()
        with self.assertRaisesRegex(RuntimeError, "expected"):
            with recorder.phase("backend-render", haxe_requests=3):
                raise RuntimeError("expected")

        report = recorder.report(mode="full", outcome="failed")
        self.assertEqual(report["outcome"], "failed")
        self.assertEqual(report["summary"], {"haxeRequests": 3})
        self.assertEqual(report["phases"][0]["outcome"], "failed")

    def test_public_toolchain_schema_keeps_the_ordered_phase_contract(self) -> None:
        toolchain = load_module(
            "toolchain_schema_subject", ROOT / "scripts/ci/run_toolchain_shard.py"
        )
        schema = json.loads(
            (ROOT / "docs/specs/toolchain-timing.schema.json").read_text(
                encoding="utf-8"
            )
        )
        timing = schema["$defs"]["caxecraftTiming"]
        self.assertEqual(
            schema["$defs"]["parallelReport"]["properties"]["schemaVersion"][
                "const"
            ],
            toolchain.PARALLEL_REPORT_SCHEMA_VERSION,
        )
        references = timing["properties"]["phases"]["prefixItems"]
        observed = []
        for reference in references:
            definition_name = reference["$ref"].removeprefix("#/$defs/")
            definition = schema["$defs"][definition_name]
            observed.append(
                (
                    definition["properties"]["name"]["const"],
                    definition["properties"]["haxeRequests"]["const"],
                )
            )
        self.assertEqual(tuple(observed), toolchain.CAXECRAFT_FULL_PHASES)
        self.assertFalse(timing["properties"]["phases"]["items"])


class FocusedCasePhaseReceiptTests(unittest.TestCase):
    def setUp(self) -> None:
        self.subject = load_module("focused_case_phase_subject", ROOT / "examples/caxecraft/run_haxe_c_test.py")
        self.temporary = tempfile.TemporaryDirectory(prefix="case-phase-tests-")
        self.addCleanup(self.temporary.cleanup)
        self.directory = Path(self.temporary.name).resolve()
        self.command = ["/synthetic/clang", *self.subject.STRICT_FLAGS,
                        *self.subject.SANITIZER_FLAGS, "/private/input.c", "-o", "/private/output"]

    def enable(self):
        collector = self.subject.TestPhaseReceipts(str(self.directory / "receipts"), "campaign-runtime", "split")
        self.subject._phase_receipts = collector
        return collector

    def invoke(self):
        return self.subject.run(self.command, cwd=ROOT, timeout=60,
                                label="sanitized strict native build", environment={"EXAMPLE": "fixed"})

    def test_disabled_preserves_dispatch_without_observer_work(self) -> None:
        completed = subprocess.CompletedProcess(self.command, 0, "exact", "")
        with mock.patch.object(self.subject, "run_bounded_process", return_value=completed) as run, \
             mock.patch.object(self.subject.time, "monotonic", side_effect=AssertionError("unexpected clock")):
            self.assertIs(self.invoke(), completed)
        self.assertEqual(run.call_args.args, (self.command,))
        self.assertEqual(run.call_args.kwargs, dict(cwd=ROOT,
            env={**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C", "EXAMPLE": "fixed"},
            check=False, capture_output=True, text=True, encoding="utf-8", timeout=60, compiler_progress=False))
        self.assertEqual(list(self.directory.iterdir()), [])

    def test_success_records_phase_and_keeps_command_private(self) -> None:
        collector = self.enable()
        completed = subprocess.CompletedProcess(self.command, 0, "ok", "")
        with mock.patch.object(self.subject, "run_bounded_process", return_value=completed) as run, \
             mock.patch.object(self.subject.time, "monotonic", side_effect=[10.0, 12.5]):
            self.assertIs(self.invoke(), completed)
        self.assertEqual(run.call_args.args, (self.command,))
        self.assertEqual(run.call_args.kwargs["timeout"], 60)
        report = json.loads(collector.path.read_text())
        self.assertEqual(report["phases"], [dict(sequence=1, label="sanitized strict native build",
            timeoutSeconds=60, cSourceCount=1, outcome="passed", elapsedMs=2500.0, exitCode=0)])
        self.assertNotIn("/private", collector.path.read_text())
        self.assertNotIn("EXAMPLE", collector.path.read_text())

    def test_failure_receipts_preserve_primary_error(self) -> None:
        for outcome, error in [("timeout", subprocess.TimeoutExpired(self.command, 60, output="partial", stderr="details")),
                               ("spawn-error", OSError("cannot start")),
                               ("interrupted", KeyboardInterrupt())]:
            with self.subTest(outcome=outcome):
                self.subject._phase_receipts = None
                folder = self.directory / outcome
                collector = self.subject.TestPhaseReceipts(str(folder), "campaign-runtime", "split")
                self.subject._phase_receipts = collector
                with mock.patch.object(self.subject, "run_bounded_process", side_effect=error), \
                     mock.patch.object(self.subject.time, "monotonic", side_effect=[1.0, 2.0]):
                    with self.assertRaises(BaseException) as caught:
                        self.invoke()
                if isinstance(error, KeyboardInterrupt):
                    self.assertIs(caught.exception, error)
                else:
                    self.assertIs(caught.exception.__cause__, error)
                    self.assertIn("could not run", str(caught.exception))
                if outcome == "timeout":
                    self.assertIn("partial", str(caught.exception))
                    self.assertIn("details", str(caught.exception))
                self.assertEqual(json.loads(collector.path.read_text())["phases"][0]["outcome"], outcome)

    def test_nonzero_keeps_diagnostics_and_exit(self) -> None:
        collector = self.enable()
        completed = subprocess.CompletedProcess(self.command, 7, "out", "err")
        with mock.patch.object(self.subject, "run_bounded_process", return_value=completed):
            with self.assertRaises(self.subject.HaxeCTestFailure) as caught:
                self.invoke()
        self.assertEqual(str(caught.exception), "sanitized strict native build failed with exit 7\nstdout:\noutstderr:\nerr")
        phase = json.loads(collector.path.read_text())["phases"][0]
        self.assertEqual((phase["outcome"], phase["exitCode"]), ("nonzero", 7))

    def test_terminal_receipt_failure_keeps_failed_command_and_running_record(self) -> None:
        collector = self.enable()
        original = collector.write
        def write():
            if collector.phases[0].outcome == "running":
                return original()
            raise OSError("report unavailable")
        with mock.patch.object(collector, "write", side_effect=write), \
             mock.patch.object(self.subject, "run_bounded_process", side_effect=subprocess.TimeoutExpired(self.command, 60)), \
             mock.patch("sys.stderr", new_callable=io.StringIO) as stderr:
            with self.assertRaises(self.subject.HaxeCTestFailure):
                self.invoke()
        self.assertIn("primary failure retained", stderr.getvalue())
        self.assertEqual(json.loads(collector.path.read_text())["phases"][0]["outcome"], "running")

    def test_success_cannot_hide_requested_evidence_failure(self) -> None:
        collector = self.enable()
        original = collector.write
        def write():
            if collector.phases[0].outcome == "running":
                return original()
            raise OSError("report unavailable")
        with mock.patch.object(collector, "write", side_effect=write), \
             mock.patch.object(self.subject, "run_bounded_process", return_value=subprocess.CompletedProcess(self.command, 0, "", "")):
            with self.assertRaisesRegex(OSError, "report unavailable"):
                self.invoke()

    def test_paths_duplicates_and_labels_fail_before_dispatch(self) -> None:
        collector = self.enable()
        with self.assertRaises(FileExistsError):
            self.subject.TestPhaseReceipts(str(collector.path.parent), "campaign-runtime", "split")
        with self.assertRaises(self.subject.HaxeCTestFailure):
            self.subject.TestPhaseReceipts(str(ROOT / "phase-receipts"), "campaign-runtime", "split")
        link = self.directory / "link"
        link.symlink_to(collector.path.parent, target_is_directory=True)
        with self.assertRaises(self.subject.HaxeCTestFailure):
            self.subject.TestPhaseReceipts(str(link), "campaign-runtime", "split")
        with mock.patch.object(self.subject, "run_bounded_process") as run:
            with self.assertRaises(self.subject.HaxeCTestFailure):
                self.subject.run(self.command, cwd=ROOT, timeout=60, label="/private/source.c")
            run.assert_not_called()

    def test_main_enables_ordered_receipts_and_clears_owner(self) -> None:
        directory = self.directory / "main"
        arguments = mock.Mock(case="campaign-runtime", layout="split", cc=None)
        def execute(*unused):
            self.invoke()
            self.subject.run(["/synthetic/result"], cwd=ROOT, timeout=30, label="campaign-runtime sanitized native test")
            return True
        with mock.patch.dict(os.environ, {"HXC_CAXECRAFT_PHASE_REPORT_DIR": str(directory)}), \
             mock.patch.object(self.subject, "parse_args", return_value=arguments), \
             mock.patch.object(self.subject, "execute", side_effect=execute), \
             mock.patch.object(self.subject, "run_bounded_process", return_value=subprocess.CompletedProcess([], 0, "", "")), \
             mock.patch("sys.stdout", new_callable=io.StringIO):
            self.assertEqual(self.subject.main(), 0)
        report = json.loads((directory / "campaign-runtime-split.json").read_text())
        self.assertEqual([p["sequence"] for p in report["phases"]], [1, 2])
        self.assertEqual([p["timeoutSeconds"] for p in report["phases"]], [60, 30])
        self.assertIsNone(self.subject._phase_receipts)

    def test_start_receipt_failure_prevents_dispatch(self) -> None:
        collector = self.enable()
        with mock.patch.object(collector, "write", side_effect=OSError("cannot save start")), \
             mock.patch.object(self.subject, "run_bounded_process") as run:
            with self.assertRaisesRegex(OSError, "cannot save start"):
                self.invoke()
            run.assert_not_called()

    def test_phase_files_cannot_satisfy_required_aggregate_upload(self) -> None:
        workflow = (ROOT / ".github/workflows/governance.yml").read_text()
        aggregate = workflow.split("      - name: Preserve toolchain suite timings\n", 1)[1].split("      - name:", 1)[0]
        self.assertIn("if-no-files-found: error", aggregate)
        path_line = next(line.strip() for line in aggregate.splitlines() if line.strip().startswith("path:"))
        self.assertEqual(path_line, "path: ${{ runner.temp }}/toolchain-${{ matrix.shard }}-timing.json")
        selected = path_line.removeprefix("path: ").replace("${{ runner.temp }}", str(self.directory)).replace("${{ matrix.shard }}", "caxecraft")
        phases = self.directory / "toolchain-caxecraft-phases"
        phases.mkdir()
        (phases / "campaign-runtime-split.json").write_text("{}")
        self.assertFalse(Path(selected).exists())
        self.assertNotIn("-phases/", aggregate)

    def test_ci_retains_existing_and_phase_timing_artifacts(self) -> None:
        workflow = (ROOT / ".github/workflows/governance.yml").read_text()
        self.assertIn("HXC_CAXECRAFT_PHASE_REPORT_DIR: ${{ runner.temp }}/toolchain-${{ matrix.shard }}-phases", workflow)
        self.assertIn("toolchain-${{ matrix.shard }}-timing.json", workflow)
        self.assertIn("toolchain-${{ matrix.shard }}-phases/", workflow)
        self.assertIn("actions/upload-artifact@ea165f8d65b6e75b540449e92b4886f43607fa02", workflow)


if __name__ == "__main__":
    unittest.main()
