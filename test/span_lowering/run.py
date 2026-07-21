#!/usr/bin/env python3
"""Prove typed fixed-array/span lowering, bounds policy, and zero-runtime C."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import re
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from collections.abc import Iterable, Iterator
from contextlib import contextmanager
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("span_lowering.hxml")
FIXTURE = Path(__file__).with_name("fixtures")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_SPAN_LOWERING="
PROFILES = ("portable", "metal")
BUILD_MODES = ("debug", "release", "minsizerel")
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-pedantic-errors",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
PRODUCTION_FILES = {
    "_GeneratedFiles.json",
    "cmake/CMakeLists.txt",
    "hxc.abi.json",
    "hxc.dispatch.json",
    "hxc.initialization-plan.json",
    "hxc.manifest.json",
    "hxc.runtime-plan.json",
    "hxc.stdlib-report.json",
    "hxc.symbols.json",
    "include/hxc/program.h",
    "meson.build",
    "src/program.c",
}


class SpanLoweringFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class NativeToolchain:
    family: str
    compiler: str
    version: str


@dataclass(frozen=True)
class HaxeInvocation:
    phase: str
    transport: str
    exit_code: int | None
    duration_ms: int

    def to_json(self) -> dict[str, object]:
        return {
            "phase": self.phase,
            "transport": self.transport,
            "exitCode": self.exit_code,
            "durationMs": self.duration_ms,
        }


@dataclass(frozen=True)
class SuitePhase:
    phase: str
    outcome: str
    duration_ms: int

    def to_json(self) -> dict[str, object]:
        return {
            "phase": self.phase,
            "outcome": self.outcome,
            "durationMs": self.duration_ms,
        }


@dataclass(frozen=True)
class NegativeCase:
    main_class: str
    expected_fragment: str
    expected_anchor: str | None = None
    diagnostic_id: str = "HXC1001"


NEGATIVE_CASES = (
    NegativeCase("NonLiteralFixture", "TFunction(return-type):reference-Array-non-null"),
    NegativeCase("ZeroLengthFixture", "TArrayDecl(empty-fixed-array-not-strict-c11)"),
    NegativeCase("LookalikeFixture", "TVar(Span:requires-fixed-array-borrow)"),
    NegativeCase(
        "ZeroConstructionLengthFixture",
        "TCall(c.CArray.zero:length-must-be-positive:0)",
        "ZeroConstructionLengthFixture.hx:6: characters 54-55",
    ),
    NegativeCase(
        "NegativeConstructionLengthFixture",
        "TCall(c.CArray.zero:length-must-be-positive:-1)",
        "NegativeConstructionLengthFixture.hx:6: characters 54-56",
    ),
    NegativeCase(
        "OversizedConstructionFixture",
        "automatic-storage-over-budget:length=65537,element-bytes=1,total-bytes=65537,limit-bytes=65536",
        "OversizedConstructionFixture.hx:6: characters 54-59",
    ),
    NegativeCase(
        "OverflowConstructionFixture",
        "TCall(c.CArray.zero:length-product-overflow:65536*65536)",
        "OverflowConstructionFixture.hx:6: characters 54-67",
    ),
    NegativeCase(
        "NonConstantConstructionFixture",
        "TCall(c.CArray.zero:length-must-be-compile-time-product:TCall)",
        "NonConstantConstructionFixture.hx:10: characters 54-62",
    ),
    NegativeCase(
        "UnsupportedZeroElementFixture",
        "TCall(c.CArray.zero:element-requires-exact-storage-size:bool)",
        "UnsupportedZeroElementFixture.hx:5: characters 50-51",
    ),
    NegativeCase(
        "StaticOutOfBoundsFixture",
        "TArray(index-statically-out-of-bounds:length=16384,index=16384)",
        "StaticOutOfBoundsFixture.hx:7: characters 17-22",
    ),
    NegativeCase(
        "EscapingSpanFixture",
        "TFunction(return-type:borrowed-span-escape)",
        "EscapingSpanFixture.hx:6: lines 6-9",
    ),
    NegativeCase(
        "RecursiveSpanParameterFixture",
        "TCall(recursive-borrowed-span-target-not-admitted:",
        "RecursiveSpanParameterFixture.hx:6: characters 10-25",
    ),
    NegativeCase(
        "StoredSpanFieldFixture",
        "borrowed-span-field-escape",
        "StoredSpanFieldFixture.hx:6: characters 2-34",
    ),
    NegativeCase(
        "StoredSpanGlobalFixture",
        "TField(static:borrowed:abstract `c.Span` is not an admitted primitive",
        "StoredSpanGlobalFixture.hx:9: characters 3-11",
    ),
    NegativeCase(
        "VirtualSpanParameterFixture",
        "borrowed-span-requires-static-function",
        "VirtualSpanParameterFixture.hx:18: lines 18-20",
    ),
    NegativeCase(
        "CallbackSpanParameterFixture",
        "TVar(callback:type):function values await closure/function representation lowering",
        "CallbackSpanParameterFixture.hx:13: characters 3-56",
    ),
    NegativeCase(
        "ExportedSpanParameterFixture",
        "Imported functions must belong to an extern class.",
        "ExportedSpanParameterFixture.hx:7: lines 7-9",
        "HXC3000",
    ),
    NegativeCase(
        "NativeSpanParameterFixture",
        "Pointer and retained-borrow lifetimes are outside this direct by-value slice.",
        "SpanNativeApi.hx:7: characters 2-72",
        "HXC3000",
    ),
)

COLD_NEGATIVE_CASES = frozenset(
    {
        "NonLiteralFixture",
        "ExportedSpanParameterFixture",
    }
)


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(*, server: bool) -> dict[str, str]:
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def elapsed_milliseconds(start_ns: int) -> int:
    return max(0, (time.monotonic_ns() - start_ns + 500_000) // 1_000_000)


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise SpanLoweringFailure(
                "Haxe compiler server exited before accepting span requests\n"
                f"stdout:\n{stdout}\nstderr:\n{stderr}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise SpanLoweringFailure("Haxe compiler server did not accept span requests")


class HaxeHarness:
    """Own one isolated server plus path-free timing evidence for this run."""

    def __init__(self, *, use_server: bool) -> None:
        self.use_server = use_server
        self.started_ns = time.monotonic_ns()
        self.server: subprocess.Popen[str] | None = None
        self.endpoint: str | None = None
        self.server_startup_ms: int | None = None
        self.invocations: list[HaxeInvocation] = []
        self.phases: list[SuitePhase] = []

    def __enter__(self) -> HaxeHarness:
        if not self.use_server:
            return self
        port = available_port()
        self.endpoint = str(port)
        startup = time.monotonic_ns()
        self.server = subprocess.Popen(
            [development_tool("haxe"), "--wait", self.endpoint],
            cwd=ROOT,
            env=haxe_environment(server=True),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
        )
        try:
            wait_for_server(self.server, port)
        except BaseException:
            self.server.terminate()
            try:
                self.server.wait(timeout=5)
            except subprocess.TimeoutExpired:
                self.server.kill()
                self.server.wait(timeout=5)
            raise
        self.server_startup_ms = elapsed_milliseconds(startup)
        return self

    def __exit__(self, _type: object, _value: object, _traceback: object) -> None:
        if self.server is None:
            return
        self.server.terminate()
        try:
            self.server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            self.server.kill()
            self.server.wait(timeout=5)

    @contextmanager
    def phase(self, phase: str) -> Iterator[None]:
        start = time.monotonic_ns()
        outcome = "passed"
        try:
            yield
        except BaseException:
            outcome = "failed"
            raise
        finally:
            self.phases.append(
                SuitePhase(phase, outcome, elapsed_milliseconds(start))
            )

    def run(
        self,
        arguments: list[str],
        *,
        phase: str,
        warm: bool,
        timeout: int = 30,
    ) -> subprocess.CompletedProcess[str]:
        if warm and self.endpoint is None:
            raise SpanLoweringFailure(
                f"warm span request {phase!r} has no compiler server"
            )
        command = [development_tool("haxe")]
        if warm:
            command.extend(["--connect", self.endpoint or ""])
        command.extend(arguments)
        start = time.monotonic_ns()
        try:
            result = subprocess.run(
                command,
                cwd=ROOT,
                env=haxe_environment(server=warm),
                check=False,
                capture_output=True,
                text=True,
                timeout=timeout,
            )
        except (OSError, subprocess.TimeoutExpired):
            self.invocations.append(
                HaxeInvocation(
                    phase,
                    "warm-server" if warm else "cold",
                    None,
                    elapsed_milliseconds(start),
                )
            )
            raise
        self.invocations.append(
            HaxeInvocation(
                phase,
                "warm-server" if warm else "cold",
                result.returncode,
                elapsed_milliseconds(start),
            )
        )
        return result

    def validate_topology(self) -> None:
        cold = sum(value.transport == "cold" for value in self.invocations)
        warm = sum(value.transport == "warm-server" for value in self.invocations)
        if len(self.invocations) != 54 or cold != 8 or warm != 46:
            raise SpanLoweringFailure(
                "span Haxe request topology drifted: "
                f"total={len(self.invocations)}, cold={cold}, warm={warm}; "
                "expected total=54, cold=8, warm=46"
            )

    def timing_report(self, *, outcome: str) -> dict[str, object]:
        cold = sum(value.transport == "cold" for value in self.invocations)
        warm = sum(value.transport == "warm-server" for value in self.invocations)
        return {
            "schemaVersion": 1,
            "suite": "span-lowering",
            "outcome": outcome,
            "durationMs": elapsed_milliseconds(self.started_ns),
            "compilerServer": {
                "started": self.server_startup_ms is not None,
                "startupMs": self.server_startup_ms,
            },
            "summary": {
                "requests": len(self.invocations),
                "coldCompilerLoads": cold,
                "warmServerRequests": warm,
                "compilerLoads": cold + (1 if self.server_startup_ms is not None else 0),
            },
            "phases": [value.to_json() for value in self.phases],
            "invocations": [value.to_json() for value in self.invocations],
        }


def write_timing_report(path: Path, report: dict[str, object]) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    descriptor, temporary_name = tempfile.mkstemp(
        dir=path.parent,
        prefix=f".{path.name}.",
        suffix=".tmp",
        text=True,
    )
    temporary = Path(temporary_name)
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="") as handle:
            json.dump(report, handle, ensure_ascii=False, indent=2)
            handle.write("\n")
            handle.flush()
            os.fsync(handle.fileno())
        os.replace(temporary, path)
    finally:
        if temporary.exists():
            temporary.unlink()


def render_matrix(
    label: str,
    *,
    harness: HaxeHarness,
    warm: bool,
    reverse: bool = False,
) -> tuple[str, dict[tuple[str, str], dict[str, object]]]:
    arguments = [str(HXML)]
    if reverse:
        arguments.extend(["-D", "span_lowering_reverse_input"])
    result = harness.run(
        arguments,
        phase=f"report:{label}",
        warm=warm,
        timeout=30,
    )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise SpanLoweringFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    matrix = json.loads(payload)
    if not isinstance(matrix, dict) or matrix.get("schemaVersion") != 1:
        raise SpanLoweringFailure(f"{label} matrix is not a schema-1 object")
    reports = matrix.get("reports")
    if not isinstance(reports, list) or not all(
        isinstance(report, dict) for report in reports
    ):
        raise SpanLoweringFailure(f"{label} matrix reports are not objects")
    expected = [(profile, build) for profile in PROFILES for build in BUILD_MODES]
    actual: list[tuple[str, str]] = []
    indexed: dict[tuple[str, str], dict[str, object]] = {}
    for report in reports:
        profile = report.get("profile")
        build = report.get("buildMode")
        if not isinstance(profile, str) or not isinstance(build, str):
            raise SpanLoweringFailure(f"{label} matrix report lost its coordinate")
        coordinate = (profile, build)
        if coordinate in indexed:
            raise SpanLoweringFailure(
                f"{label} matrix repeated {profile}/{build}"
            )
        actual.append(coordinate)
        indexed[coordinate] = report
    if actual != expected:
        raise SpanLoweringFailure(
            f"{label} matrix coordinates drifted: {actual!r}"
        )
    return payload, indexed


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise SpanLoweringFailure(f"report field {key!r} must be text")
    return value


def symbol_entries(symbols: dict[str, object]) -> list[dict[str, object]]:
    value = symbols.get("symbols")
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise SpanLoweringFailure("finalized symbol entries are missing")
    return value


def function_names(
    symbols: dict[str, object], *, require_final_class_case: bool = False
) -> dict[str, str]:
    names: dict[str, str] = {}
    for entry in symbol_entries(symbols):
        source = entry.get("sourceSymbol")
        name = entry.get("cName")
        if (
            entry.get("kind") == "method"
            and isinstance(source, str)
            and source.startswith("SpanFixture.")
            and isinstance(name, str)
        ):
            field = source.removeprefix("SpanFixture.").split("(", 1)[0]
            names[field] = name
    expected = {
        "checkedAt",
        "constSum",
        "forwardRead",
        "forwardReplace",
        "linearIndex",
        "main",
        "mutableSum",
        "mutatedGridCell",
        "parameterRoundTrip",
        "readAt",
        "replaceAt",
        "spanBeforeConditionalArgument",
        "zeroedGridCell",
    }
    if require_final_class_case:
        expected.add("finalClassParameterRoundTrip")
    if set(names) != expected:
        raise SpanLoweringFailure(f"span fixture symbol set drifted: {sorted(names)!r}")
    return names


def validate(report: dict[str, object], *, profile: str, build: str) -> None:
    if (
        report.get("schemaVersion") != 2
        or report.get("status")
        != "typed-zero-fixed-arrays-and-span-parameters-runtime-free"
        or report.get("profile") != profile
        or report.get("buildMode") != build
        or report.get("runtimeFeatures") != []
        or report.get("runtimeArtifacts") != []
    ):
        raise SpanLoweringFailure(f"span report contract drifted for {profile}/{build}")
    hxcir = required_text(report, "hxcir")
    if str(ROOT) in hxcir or "\\" in hxcir or "hxrt" in hxcir.lower():
        raise SpanLoweringFailure("HxcIR leaked a host path or runtime dependency")
    for forbidden in ("iterator", "hasNext", 'direct("next")'):
        if forbidden in hxcir:
            raise SpanLoweringFailure(f"HxcIR retained iterator machinery {forbidden!r}")
    expected_counts = {
        "type=fixed-array(length=4,witness=\"Length4\",element=i32)": 3,
        "type=fixed-array(length=4,witness=\"Length4\",element=u8)": 2,
        "type=fixed-array(length=16384,witness=\"GridVolume\",element=u8)": 2,
        " result=- initialize-fixed-array place=": 4,
        "zero-initialize-fixed-array": 3,
        "initialize-span": 9,
        "span-parameter-borrow": 4,
        "bounds-check": 14,
        "static-proof": 3,
        "loop-guarded": 3,
        'index-local="local.3",length=4': 2,
        "checked-abort": 8,
        'operation="hxc.size.add-one.span-index-proven"': 3,
        "static-call-argument-0-initialize": 1,
        "static-call-argument-0-load": 1,
    }
    for marker, expected in expected_counts.items():
        actual = hxcir.count(marker)
        if actual != expected:
            raise SpanLoweringFailure(
                f"HxcIR marker {marker!r} appeared {actual} times, expected {expected}"
            )
    policy = f'policy=checked-abort(profile="{profile}",build="{build}")'
    if hxcir.count(policy) != 8:
        raise SpanLoweringFailure(f"bounds policy lost {profile}/{build} provenance")
    for marker in (
        "terminator branch",
        "span-loop-condition",
        "span-loop-body-index",
        "span-loop-element",
        "span-loop-increment",
    ):
        if marker not in hxcir:
            raise SpanLoweringFailure(f"direct span loop lost {marker!r}")

    functions = report.get("functions")
    if not isinstance(functions, list) or len(functions) != 13:
        raise SpanLoweringFailure("span function inventory drifted")
    fields = [item.get("field") for item in functions if isinstance(item, dict)]
    if fields != [
        "checkedAt",
        "constSum",
        "forwardRead",
        "forwardReplace",
        "linearIndex",
        "main",
        "mutableSum",
        "mutatedGridCell",
        "parameterRoundTrip",
        "readAt",
        "replaceAt",
        "spanBeforeConditionalArgument",
        "zeroedGridCell",
    ]:
        raise SpanLoweringFailure(f"span function order drifted: {fields!r}")
    length_name_count = 0
    for item in functions:
        if not isinstance(item, dict):
            raise SpanLoweringFailure("span function inventory contains a malformed entry")
        names = item.get("spanLengthNames")
        if (
            not isinstance(names, list)
            or not all(isinstance(name, str) for name in names)
            or len(names) != len(set(names))
        ):
            raise SpanLoweringFailure(
                f"span length identifiers are not unique inside {item.get('field')!r}"
            )
        length_name_count += len(names)
    if length_name_count != 18:
        raise SpanLoweringFailure("span length identifier inventory is incomplete")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise SpanLoweringFailure("span report omitted its finalized symbol table")
    function_names(symbols)
    aborts = [
        entry
        for entry in symbol_entries(symbols)
        if entry.get("sourceSymbol") == "c-standard-library.abort"
    ]
    if len(aborts) != 1 or aborts[0].get("cName") != "abort":
        raise SpanLoweringFailure("checked bounds did not register exact C abort authority")


def normalized_semantics(report: dict[str, object]) -> dict[str, object]:
    normalized = copy.deepcopy(report)
    profile = required_text(report, "profile")
    build = required_text(report, "buildMode")
    normalized["profile"] = "<profile>"
    normalized["buildMode"] = "<build>"
    normalized["hxcir"] = required_text(report, "hxcir").replace(
        f'profile="{profile}"', 'profile="<profile>"'
    ).replace(f'build="{build}"', 'build="<build>"')
    return normalized


def custom_target(
    main_class: str,
    output: Path,
    *,
    harness: HaxeHarness,
    warm: bool,
    phase: str,
    profile: str = "portable",
    build: str = "debug",
    runtime: str = "none",
) -> subprocess.CompletedProcess[str]:
    arguments = [
        "-cp",
        str(FIXTURE),
        "-lib",
        "reflaxe.c",
        "-main",
        main_class,
        "-D",
        f"hxc_build={build}",
        "-D",
        f"hxc_runtime={runtime}",
    ]
    if profile == "metal":
        arguments.extend(["-D", "reflaxe_c_profile=metal"])
    arguments.extend(
        ["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"]
    )
    return harness.run(
        arguments,
        phase=phase,
        warm=warm,
        timeout=30,
    )


def generated_files(root: Path) -> set[str]:
    return {
        path.relative_to(root).as_posix()
        for path in root.rglob("*")
        if path.is_file()
    }


def assert_no_output(root: Path, label: str) -> None:
    files = generated_files(root)
    if files:
        raise SpanLoweringFailure(f"{label} emitted files: {sorted(files)!r}")


def validate_project(root: Path, *, profile: str, build: str) -> dict[str, object]:
    actual = generated_files(root)
    if actual != PRODUCTION_FILES:
        raise SpanLoweringFailure(
            f"{profile}/{build} production artifact set drifted: {sorted(actual)!r}"
        )
    header = (root / "include/hxc/program.h").read_text(encoding="utf-8")
    source = (root / "src/program.c").read_text(encoding="utf-8")
    runtime_plan = json.loads((root / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    initialization_plan = json.loads(
        (root / "hxc.initialization-plan.json").read_text(encoding="utf-8")
    )
    manifest = json.loads((root / "hxc.manifest.json").read_text(encoding="utf-8"))
    dispatch = json.loads((root / "hxc.dispatch.json").read_text(encoding="utf-8"))
    symbols = json.loads((root / "hxc.symbols.json").read_text(encoding="utf-8"))
    proof = runtime_plan.get("noRuntimeProof")
    if not isinstance(proof, dict):
        raise SpanLoweringFailure(f"{profile}/{build} omitted its structured no-runtime proof")
    reachability = proof.get("reachability")
    runtime_absence = proof.get("runtimeAbsence")
    expected_helpers = [
        "hxc.i32.add.wrapping",
        "hxc.i32.multiply.wrapping",
        "hxc.u32.to.i32.bits",
    ]
    if (
        initialization_plan.get("schemaVersion") != 1
        or initialization_plan.get("strategy") != "eager-haxe-type-order"
        or initialization_plan.get("runtimeFeatures") != []
        or runtime_plan.get("schemaVersion") != 2
        or runtime_plan.get("algorithm") != "hxc-runtime-plan-v2"
        or runtime_plan.get("profile") != profile
        or runtime_plan.get("resolvedPolicy") != "none"
        or runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("features") != []
        or runtime_plan.get("artifacts") != []
        or proof.get("schemaVersion") != 1
        or proof.get("algorithm") != "hxc-no-runtime-eligibility-v1"
        or proof.get("status") != "eligible"
        or proof.get("scope") != "reachable-whole-program"
        or proof.get("directDecisions") != runtime_plan.get("directDecisions")
        or proof.get("programLocalHelpers") != expected_helpers
        or reachability
        != {
            "modules": 1,
            "typeInstances": 1,
            "functions": 15,
            "blocks": 30,
            "instructions": 184,
            "cleanupActions": 2,
            "runtimeIntents": 0,
        }
        or runtime_absence
        != {
            "features": [],
            "includes": [],
            "sources": [],
            "defines": [],
            "libraries": [],
            "symbols": [],
        }
        or manifest.get("configuration", {}).get("profile") != profile
        or manifest.get("configuration", {}).get("runtimePolicy") != "none"
    ):
        raise SpanLoweringFailure(f"{profile}/{build} lost its runtime-free policy proof")
    manifest_build = manifest.get("build")
    if (
        not isinstance(manifest_build, dict)
        or manifest_build.get("runtimeHeaders") != []
        or manifest_build.get("sources") != ["src/program.c"]
        or manifest_build.get("includeDirectories") != ["include"]
    ):
        raise SpanLoweringFailure(f"{profile}/{build} build plan retained an hxrt input")
    calls = dispatch.get("calls")
    if (
        dispatch.get("schemaVersion") != 1
        or dispatch.get("algorithm") != "hxc-closed-world-virtual-dispatch-v1"
        or dispatch.get("runtimeFeatures") != []
        or dispatch.get("slots") != []
        or dispatch.get("layouts") != []
        or dispatch.get("tables") != []
        or dispatch.get("summary")
        != {
            "indirectCalls": 0,
            "slots": 0,
            "instanceCalls": 1,
            "tables": 0,
            "directCalls": 1,
            "adapters": 0,
            "layouts": 0,
        }
        or not isinstance(calls, list)
        or len(calls) != 1
        or not isinstance(calls[0], dict)
        or calls[0].get("dispatch") != "direct"
        or calls[0].get("reason") != "final-receiver-class"
        or calls[0].get("slotId") is not None
        or calls[0].get("methodFunctionId") != calls[0].get("targetFunctionId")
    ):
        raise SpanLoweringFailure(
            f"{profile}/{build} final-class span call lost direct dispatch proof"
        )
    combined = "\n".join(
        path.read_text(encoding="utf-8")
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h"}
    )
    if "hxrt" in combined.lower():
        raise SpanLoweringFailure(f"{profile}/{build} selected or mentioned hxrt")
    for marker in ("#include <stddef.h>", "#include <stdlib.h>"):
        if marker not in header:
            raise SpanLoweringFailure(f"generated header lost {marker!r}")
    for forbidden in (
        "iterator",
        "hasNext",
        "Array<",
        " + index",
        " + (size_t)",
        "malloc(",
        "calloc(",
        "realloc(",
        "free(",
    ):
        if forbidden in source:
            raise SpanLoweringFailure(f"generated C retained forbidden shape {forbidden!r}")
    if "runtimeTypeOnly" in source or "runtimeTypeOnly" in header:
        raise SpanLoweringFailure("an unreachable String-typed declaration entered generated output")
    if (
        len(re.findall(r"int32_t [A-Za-z0-9_]+\[4\] = \{", source)) != 3
        or len(re.findall(r"uint8_t [A-Za-z0-9_]+\[4\] = \{", source)) != 3
        or len(re.findall(r"uint8_t [A-Za-z0-9_]+\[16384\] = \{ 0 \};", source)) != 2
        or source.count("const int32_t *") != 2
        or len(re.findall(r"(?m)^  int32_t \*[A-Za-z0-9_]+ =", source)) != 1
        or source.count("const uint8_t *") != 9
        or len(re.findall(r"(?m)^  uint8_t \*[A-Za-z0-9_]+ =", source)) != 6
        or source.count(" = sizeof(") != 10
        or source.count("bounds") != 0
        or source.count("abort();") != 11
        or source.count("[(size_t)") != 16
    ):
        raise SpanLoweringFailure(f"{profile}/{build} fixed-array/span C shape drifted")
    if (
        source.count(" < 4)") != 2
        or source.count(" < 16384)") != 1
        or source.count(" + 1;") != 3
    ):
        raise SpanLoweringFailure("span iteration stopped being a direct guarded index loop")
    if source.count(" < 0 || (size_t)") != 10 or source.count(" >= ") != 11:
        raise SpanLoweringFailure("dynamic span access lost its signed/size_t bounds check")
    parameter_signatures = re.findall(
        r"(?m)^uint8_t [^(]+\((?:const )?uint8_t \*[^,]+, size_t [^,]+,",
        source,
    )
    if len(parameter_signatures) != 4 or "struct" in "\n".join(parameter_signatures):
        raise SpanLoweringFailure(
            "span parameters stopped lowering directly to pointer-plus-size_t signatures"
        )
    function_names(symbols, require_final_class_case=True)
    return {"header": header, "source": source, "symbols": symbols}


def production_snapshot(harness: HaxeHarness) -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-span-snapshot-") as temporary:
        output = Path(temporary) / "out"
        result = custom_target(
            "SpanFixture",
            output,
            harness=harness,
            warm=False,
            phase="snapshot:production",
        )
        if result.returncode != 0 or result.stdout or result.stderr:
            raise SpanLoweringFailure(
                "span production snapshot failed\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        project = validate_project(output, profile="portable", build="debug")
        return {
            "program.h": project["header"],
            "program.c": project["source"],
            "program-symbols.json": project["symbols"],
        }


def snapshot_artifacts() -> dict[str, object]:
    with HaxeHarness(use_server=False) as harness:
        _, reports = render_matrix(
            "span snapshot render",
            harness=harness,
            warm=False,
        )
        report = reports[("portable", "debug")]
        validate(report, profile="portable", build="debug")
        artifacts: dict[str, object] = {
            "span.hxcir": required_text(report, "hxcir"),
            "symbols.json": report.get("symbols"),
        }
        artifacts.update(production_snapshot(harness))
        return artifacts


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(artifacts: dict[str, object]) -> None:
    for name, actual in artifacts.items():
        path = EXPECTED / name
        if not path.is_file():
            raise SpanLoweringFailure(f"registered span snapshot {name} is missing")
        if name.endswith(".json"):
            if actual != json.loads(path.read_text(encoding="utf-8")):
                raise SpanLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise SpanLoweringFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise SpanLoweringFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"], capture_output=True, text=True, timeout=10
    )
    combined = (result.stdout + result.stderr).strip()
    first = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first
    if "free software foundation" in lowered or "gcc" in lowered:
        return "gcc", first
    return "unknown", first


def available_compilers(selected: str | None = None) -> list[NativeToolchain]:
    requested = [selected] if selected else ["gcc", "clang"]
    result: list[NativeToolchain] = []
    for family in requested:
        if family is None:
            continue
        executable = shutil.which(family)
        if executable is None:
            if selected:
                raise SpanLoweringFailure(f"required span toolchain missing: {family}")
            print(f"span-lowering: SKIP optional {family}: missing {family}")
            continue
        actual, version = compiler_identity(executable)
        if actual != family:
            message = f"{family}: command identity mismatch ({family}={actual})"
            if selected:
                raise SpanLoweringFailure(f"required span toolchain {message}")
            print(f"span-lowering: SKIP optional {message}")
            continue
        print(
            f"span-lowering: {'REQUIRED' if selected else 'AVAILABLE'} "
            f"{family}: cc={version}"
        )
        result.append(NativeToolchain(family, executable, version))
    if not result:
        raise SpanLoweringFailure("no strict C11 compiler is available for span evidence")
    return result


def run_command(
    command: list[str],
    label: str,
    *,
    timeout: int = 30,
    environment: dict[str, str] | None = None,
) -> None:
    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        timeout=timeout,
        env=environment,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise SpanLoweringFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def harness_source(symbols: dict[str, object]) -> str:
    names = function_names(symbols, require_final_class_case=True)
    return f'''#include "hxc/program.h"

#include <stdint.h>

int main(void)
{{
  if ({names["mutableSum"]}() != INT32_C(34)) return 1;
  if ({names["constSum"]}() != INT32_C(32)) return 2;
  if ({names["checkedAt"]}(INT32_C(2)) != INT32_C(8)) return 3;
  if ({names["linearIndex"]}(INT32_C(31), INT32_C(15), INT32_C(31)) != INT32_C(16383)) return 4;
  if ({names["zeroedGridCell"]}() != UINT8_C(0)) return 5;
  if ({names["mutatedGridCell"]}(UINT8_C(201)) != UINT8_C(201)) return 6;
  if ({names["parameterRoundTrip"]}(UINT8_C(201)) != UINT8_C(201)) return 7;
  if ({names["spanBeforeConditionalArgument"]}(true) != UINT8_C(8)) return 8;
  if ({names["finalClassParameterRoundTrip"]}(UINT8_C(201)) != UINT8_C(201)) return 9;
  return 0;
}}
'''


def check_no_hxrt_symbols(executable: Path, label: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise SpanLoweringFailure("nm is required for the no-hxrt link proof")
    result = subprocess.run([nm, str(executable)], capture_output=True, text=True, timeout=10)
    if result.returncode != 0 or "hxrt" in (result.stdout + result.stderr).lower():
        raise SpanLoweringFailure(f"{label} linked an hxrt symbol or could not be inspected")


def check_native_artifacts(
    artifacts: dict[str, object],
    symbols: dict[str, object],
    selected: str | None = None,
) -> None:
    header = artifacts.get("program.h")
    source = artifacts.get("program.c")
    if not isinstance(header, str) or not isinstance(source, str):
        raise SpanLoweringFailure("native span artifacts are incomplete")
    with tempfile.TemporaryDirectory(prefix="hxc-span-native-") as temporary:
        root = Path(temporary)
        include = root / "include/hxc"
        include.mkdir(parents=True)
        (include / "program.h").write_text(header, encoding="utf-8", newline="\n")
        generated = root / "program.c"
        generated.write_text(source, encoding="utf-8", newline="\n")
        renamed = root / "renamed-entry.h"
        renamed.write_text("int hxc_generated_main(void);\n", encoding="utf-8", newline="\n")
        harness = root / "harness.c"
        harness.write_text(harness_source(symbols), encoding="utf-8", newline="\n")
        native_lanes = (
            ("O0", ["-O0"], []),
            ("O2", ["-O2"], []),
            (
                "asan-ubsan",
                [
                    "-O1",
                    "-g",
                    "-fno-omit-frame-pointer",
                    "-fsanitize=address,undefined",
                ],
                ["-fsanitize=address,undefined"],
            ),
        )
        for toolchain in available_compilers(selected):
            for lane, compile_flags, link_flags in native_lanes:
                generated_object = root / f"generated-{toolchain.family}-{lane}.o"
                harness_object = root / f"harness-{toolchain.family}-{lane}.o"
                executable = root / f"span-{toolchain.family}-{lane}"
                run_command(
                    [
                        toolchain.compiler,
                        *STRICT_FLAGS,
                        *compile_flags,
                        "-I",
                        str(root / "include"),
                        "-Dmain=hxc_generated_main",
                        "-include",
                        str(renamed),
                        "-c",
                        str(generated),
                        "-o",
                        str(generated_object),
                    ],
                    f"{toolchain.family} {lane} generated span compile",
                )
                run_command(
                    [
                        toolchain.compiler,
                        *STRICT_FLAGS,
                        *compile_flags,
                        "-I",
                        str(root / "include"),
                        "-c",
                        str(harness),
                        "-o",
                        str(harness_object),
                    ],
                    f"{toolchain.family} {lane} span harness compile",
                )
                run_command(
                    [
                        toolchain.compiler,
                        *link_flags,
                        str(generated_object),
                        str(harness_object),
                        "-o",
                        str(executable),
                    ],
                    f"{toolchain.family} {lane} span link",
                )
                run_environment = None
                if lane == "asan-ubsan":
                    run_environment = os.environ.copy()
                    run_environment["ASAN_OPTIONS"] = "halt_on_error=1:abort_on_error=1"
                    run_environment["UBSAN_OPTIONS"] = "halt_on_error=1:print_stacktrace=1"
                run_command(
                    [str(executable)],
                    f"{toolchain.family} {lane} span behavior",
                    environment=run_environment,
                )
                check_no_hxrt_symbols(executable, f"{toolchain.family} {lane}")


def compile_failure_fixture(
    main_class: str,
    expected_fragment: str,
    *,
    harness: HaxeHarness,
    warm: bool,
    profile: str,
    expected_anchor: str | None = None,
    diagnostic_id: str = "HXC1001",
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-span-negative-") as temporary:
        output = Path(temporary) / "out"
        result = custom_target(
            main_class,
            output,
            harness=harness,
            warm=warm,
            phase=f"negative:{profile}:{main_class}",
            profile=profile,
        )
        combined = result.stdout + result.stderr
        if (
            result.returncode != 1
            or diagnostic_id not in combined
            or expected_fragment not in combined
            or (expected_anchor is not None and expected_anchor not in combined)
            or f"[profile={profile}]" not in combined
        ):
            raise SpanLoweringFailure(
                f"{profile} {main_class} did not fail closed as expected\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        assert_no_output(output, f"{profile} {main_class} {diagnostic_id}")


def check_configuration_failure(
    *, harness: HaxeHarness, warm: bool, profile: str
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-span-config-") as temporary:
        output = Path(temporary) / "out"
        result = custom_target(
            "SpanFixture",
            output,
            harness=harness,
            warm=warm,
            phase=f"configuration:{profile}:invalid-build",
            profile=profile,
            build="fast",
        )
        combined = result.stdout + result.stderr
        if (
            result.returncode != 1
            or "HXC0003" not in combined
            or "invalid hxc_build `fast`; expected debug, release, or minsizerel" not in combined
            or f"[profile={profile}]" not in combined
        ):
            raise SpanLoweringFailure(
                f"{profile} invalid hxc_build did not fail closed with HXC0003"
            )
        assert_no_output(output, f"{profile} invalid hxc_build")


def check_bounds_matrix(
    toolchain: NativeToolchain,
    *,
    harness: HaxeHarness,
    warm: bool,
    profile: str,
    build: str,
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-span-bounds-") as temporary:
        root = Path(temporary)
        output = root / "out"
        result = custom_target(
            "BoundsMatrixFixture",
            output,
            harness=harness,
            warm=warm,
            phase=f"bounds:{profile}/{build}:matrix",
            profile=profile,
            build=build,
        )
        if result.returncode != 0 or result.stdout or result.stderr:
            raise SpanLoweringFailure(
                f"{profile}/{build} bounds matrix did not compile\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        combined = "\n".join(
            path.read_text(encoding="utf-8")
            for path in output.rglob("*")
            if path.is_file() and path.suffix in {".c", ".h", ".json"}
        )
        if "hxrt" in combined.lower() or combined.count("abort();") != 4:
            raise SpanLoweringFailure(
                f"{profile}/{build} bounds matrix lost its four no-hxrt aborts"
            )
        native_include = root / "include"
        native_include.mkdir()
        (native_include / "span_bounds_harness.h").write_text(
            """#ifndef HXC_SPAN_BOUNDS_HARNESS_H
#define HXC_SPAN_BOUNDS_HARNESS_H

#include <stdint.h>

int32_t span_bounds_case(void);
void span_bounds_mark(int32_t actual);

#endif
""",
            encoding="utf-8",
            newline="\n",
        )
        selector = root / "selector.c"
        selector.write_text(
            """#include "span_bounds_harness.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef HXC_SPAN_BOUNDS_CASE
#error "HXC_SPAN_BOUNDS_CASE is required"
#endif

int32_t span_bounds_case(void)
{
  return (int32_t)HXC_SPAN_BOUNDS_CASE;
}

void span_bounds_mark(int32_t actual)
{
  if (actual != (int32_t)HXC_SPAN_BOUNDS_CASE) _Exit(100);
  if (fputc('0' + actual, stderr) == EOF) _Exit(101);
  if (fflush(stderr) != 0) _Exit(102);
}
""",
            encoding="utf-8",
            newline="\n",
        )
        generated_object = root / "bounds-generated.o"
        run_command(
            [
                toolchain.compiler,
                *STRICT_FLAGS,
                "-O2",
                "-I",
                str(output / "include"),
                "-I",
                str(native_include),
                "-c",
                str(output / "src/program.c"),
                "-o",
                str(generated_object),
            ],
            f"{profile}/{build} bounds matrix strict compile",
        )
        cases = (
            "local-upper",
            "local-negative",
            "parameter-upper",
            "parameter-negative",
        )
        for case_index, case_name in enumerate(cases):
            selector_object = root / f"bounds-selector-{case_index}.o"
            executable = root / f"bounds-{case_index}"
            run_command(
                [
                    toolchain.compiler,
                    *STRICT_FLAGS,
                    "-O2",
                    "-I",
                    str(native_include),
                    f"-DHXC_SPAN_BOUNDS_CASE={case_index}",
                    "-c",
                    str(selector),
                    "-o",
                    str(selector_object),
                ],
                f"{profile}/{build} {case_name} selector compile",
            )
            run_command(
                [
                    toolchain.compiler,
                    str(generated_object),
                    str(selector_object),
                    "-o",
                    str(executable),
                ],
                f"{profile}/{build} {case_name} link",
            )
            ran = subprocess.run(
                [str(executable)], capture_output=True, text=True, timeout=10
            )
            if (
                ran.returncode == 0
                or ran.returncode in {100, 101, 102}
                or ran.stdout
                or ran.stderr != str(case_index)
            ):
                raise SpanLoweringFailure(
                    f"{profile}/{build} {case_name} did not reach its own fail-stop path"
                )
            check_no_hxrt_symbols(
                executable, f"{profile}/{build} {case_name}"
            )


def check_production(
    harness: HaxeHarness, selected: str | None = None
) -> dict[str, object]:
    canonical: dict[str, object] | None = None
    canonical_source: str | None = None
    canonical_header: str | None = None
    with tempfile.TemporaryDirectory(prefix="hxc-span-production-") as temporary:
        root = Path(temporary)
        for profile in PROFILES:
            for build in BUILD_MODES:
                output = root / f"{profile}-{build}"
                cold_canonical = profile == "portable" and build == "debug"
                result = custom_target(
                    "SpanFixture",
                    output,
                    harness=harness,
                    warm=not cold_canonical,
                    phase=f"production:{profile}/{build}",
                    profile=profile,
                    build=build,
                )
                if result.returncode != 0 or result.stdout or result.stderr:
                    raise SpanLoweringFailure(
                        f"{profile}/{build} production compile failed\n"
                        f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                    )
                project = validate_project(output, profile=profile, build=build)
                if canonical is None:
                    canonical = {
                        "program.h": project["header"],
                        "program.c": project["source"],
                        "program-symbols.json": project["symbols"],
                    }
                    canonical_header = project["header"]
                    canonical_source = project["source"]
                elif project["header"] != canonical_header or project["source"] != canonical_source:
                    raise SpanLoweringFailure("profile/build matrix changed generated C bytes")
        repeat = root / "repeat"
        result = custom_target(
            "SpanFixture",
            repeat,
            harness=harness,
            warm=True,
            phase="production:portable/debug:repeat-after-warm-failures",
        )
        if result.returncode != 0 or result.stdout or result.stderr:
            raise SpanLoweringFailure("repeated production render failed")
        repeated = validate_project(repeat, profile="portable", build="debug")
        if repeated["header"] != canonical_header or repeated["source"] != canonical_source:
            raise SpanLoweringFailure("repeated production render was not byte deterministic")

    if canonical is None:
        raise SpanLoweringFailure("production matrix produced no canonical project")
    toolchain = available_compilers(selected)[0]
    for profile in PROFILES:
        for build in BUILD_MODES:
            check_bounds_matrix(
                toolchain,
                harness=harness,
                warm=not (profile == "portable" and build == "debug"),
                profile=profile,
                build=build,
            )
    return canonical


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    parser.add_argument(
        "--timing-report",
        type=Path,
        help="write a path-free JSON report for Haxe requests and suite phases",
    )
    return parser.parse_args(list(arguments))


def check_full_suite(args: argparse.Namespace, harness: HaxeHarness) -> None:
    baseline: dict[str, object] | None = None
    canonical_report: dict[str, object] | None = None
    with harness.phase("typed-report-matrix"):
        first_payload, first_reports = render_matrix(
            "first span matrix render",
            harness=harness,
            warm=False,
        )
        second_payload, second_reports = render_matrix(
            "second span matrix render",
            harness=harness,
            warm=False,
        )
        reverse_payload, reverse_reports = render_matrix(
            "reverse span matrix render",
            harness=harness,
            warm=False,
            reverse=True,
        )
        if first_payload != second_payload or first_payload != reverse_payload:
            raise SpanLoweringFailure(
                "span report matrix changed across cold repeat/input order"
            )
        for profile in PROFILES:
            for build in BUILD_MODES:
                coordinate = (profile, build)
                first = first_reports[coordinate]
                second = second_reports[coordinate]
                reverse = reverse_reports[coordinate]
                validate(first, profile=profile, build=build)
                validate(second, profile=profile, build=build)
                validate(reverse, profile=profile, build=build)
                normalized = normalized_semantics(first)
                if baseline is None:
                    baseline = normalized
                    canonical_report = first
                elif normalized != baseline:
                    raise SpanLoweringFailure(
                        "profile/build bounds policy changed semantics beyond its provenance"
                    )

    if canonical_report is None:
        raise SpanLoweringFailure("span macro matrix produced no canonical report")

    with harness.phase("negative-diagnostics"):
        for profile in PROFILES:
            for case in NEGATIVE_CASES:
                compile_failure_fixture(
                    case.main_class,
                    case.expected_fragment,
                    harness=harness,
                    warm=not (
                        profile == "portable"
                        and case.main_class in COLD_NEGATIVE_CASES
                    ),
                    profile=profile,
                    expected_anchor=case.expected_anchor,
                    diagnostic_id=case.diagnostic_id,
                )

    with harness.phase("configuration-diagnostics"):
        for profile in PROFILES:
            check_configuration_failure(
                harness=harness,
                warm=profile != "portable",
                profile=profile,
            )

    with harness.phase("production-profile-build-and-bounds"):
        production = check_production(harness, args.toolchain)

    with harness.phase("snapshot-comparison"):
        snapshots: dict[str, object] = {
            "span.hxcir": required_text(canonical_report, "hxcir"),
            "symbols.json": canonical_report.get("symbols"),
            **production,
        }
        check_snapshots(snapshots)

    symbols = production.get("program-symbols.json")
    if not isinstance(symbols, dict):
        raise SpanLoweringFailure("canonical span symbols are missing")
    with harness.phase("native-toolchain-matrix"):
        check_native_artifacts(production, symbols, args.toolchain)


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if args.native_only and args.timing_report is not None:
        print(
            "span-lowering: ERROR: --timing-report requires the full suite",
            file=sys.stderr,
        )
        return 1
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("span-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    if args.native_only:
        try:
            artifacts = {
                "program.h": (EXPECTED / "program.h").read_text(encoding="utf-8"),
                "program.c": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
            symbols = json.loads(
                (EXPECTED / "program-symbols.json").read_text(encoding="utf-8")
            )
            check_native_artifacts(artifacts, symbols, args.toolchain)
            print("span-lowering: OK: required strict-C native matrix passed")
            return 0
        except (
            OSError,
            subprocess.TimeoutExpired,
            SpanLoweringFailure,
            json.JSONDecodeError,
        ) as error:
            print(f"span-lowering: ERROR: {error}", file=sys.stderr)
            return 1

    harness = HaxeHarness(use_server=True)
    try:
        with harness:
            check_full_suite(args, harness)
            harness.validate_topology()
        report = harness.timing_report(outcome="passed")
        if args.timing_report is not None:
            write_timing_report(args.timing_report, report)
        summary = report["summary"]
        if not isinstance(summary, dict):
            raise SpanLoweringFailure("span timing summary is not an object")
        print(
            "span-lowering: TIMING: "
            f"requests={summary['requests']}, "
            f"cold-compiler-loads={summary['coldCompilerLoads']}, "
            f"warm-server-requests={summary['warmServerRequests']}, "
            f"compiler-loads={summary['compilerLoads']}, "
            f"total={report['durationMs']}ms"
        )
        print(
            "span-lowering: OK: literal/zero fixed arrays, exact-width spans and "
            "span parameters, bounds/storage matrix, strict C11, and zero-hxrt links passed"
        )
        return 0
    except (
        OSError,
        subprocess.TimeoutExpired,
        SpanLoweringFailure,
        json.JSONDecodeError,
    ) as error:
        if args.timing_report is not None:
            try:
                write_timing_report(
                    args.timing_report,
                    harness.timing_report(outcome="failed"),
                )
            except OSError as report_error:
                print(
                    f"span-lowering: ERROR: could not write failed timing report: {report_error}",
                    file=sys.stderr,
                )
        print(f"span-lowering: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
