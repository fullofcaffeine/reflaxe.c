#!/usr/bin/env python3
"""Prove explicit Haxe sequencing in HxcIR, strict C11, and Eval differential execution."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
from collections import Counter
from collections.abc import Iterable
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("evaluation_order.hxml")
ORACLE_HXML = Path(__file__).with_name("oracle.hxml")
INDEX_ORACLE_HXML = ROOT / "test/hxc_ir/oracle.hxml"
FIXTURE = Path(__file__).with_name("fixtures")
FLOW_CARRIER_MAIN = "FlowCarrierFixture"
EXPECTED = Path(__file__).with_name("expected")
MAINTAINABILITY_POLICY = ROOT / "docs/specs/generated-c-maintainability-policy.json"
REPORT_PREFIX = "HXC_EVALUATION_ORDER="
SOURCE_SNAPSHOTS = {"program.c": "src/program.c"}
HXCIR_FUNCTION_SOURCE = re.compile(
    r'^  function "(?P<function_id>[^"]+)" name="[^"]+"[^\n]* '
    r'@"(?P<path>[^"]+)":(?P<start_line>[0-9]+):(?P<start_column>[0-9]+)-'
    r'(?P<end_line>[0-9]+):(?P<end_column>[0-9]+)$',
    re.MULTILINE,
)
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

sys.path.insert(0, str(ROOT / "scripts/test"))
from generated_c_maintainability import (  # noqa: E402
    ArtifactOwner,
    FunctionSourceMapping,
    GeneratedCArtifact,
    GotoAuthority,
    GotoCategory,
    MaintainabilityError,
    OwnerKind,
    SourceSpan,
    SymbolLedgerEntry,
    analyze_generated_c,
    load_corpus_policy,
    validate_report as validate_maintainability_report,
)


class EvaluationOrderFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class NativeToolchain:
    family: str
    compiler: str
    version: str


@dataclass(frozen=True)
class GotoProvenance:
    category: str
    function_id: str
    c_function_name: str
    owner_block_id: str
    target_block_id: str
    target_label: str
    path: str
    occurrence: int


CONTROL_FLOW_ARTIFACT_PATH = "synthetic-control-flow.c"
EXPECTED_CONTROL_FLOW_GOTO_PROVENANCE = (
    GotoProvenance(
        "loop-break-through-switch",
        "synthetic.loop-switch-break",
        "hxc_bounded_control_flow",
        "dispatch",
        "exit",
        "hxc_bounded_label_3",
        CONTROL_FLOW_ARTIFACT_PATH,
        1,
    ),
    GotoProvenance(
        "irreducible-cfg",
        "synthetic.nested-irreducible",
        "hxc_legacy_control_flow",
        "entry",
        "left-entry",
        "hxc_legacy_label_1",
        CONTROL_FLOW_ARTIFACT_PATH,
        1,
    ),
    GotoProvenance(
        "irreducible-cfg",
        "synthetic.nested-irreducible",
        "hxc_legacy_control_flow",
        "entry",
        "right-entry",
        "hxc_legacy_label_2",
        CONTROL_FLOW_ARTIFACT_PATH,
        1,
    ),
    GotoProvenance(
        "irreducible-cfg",
        "synthetic.nested-irreducible",
        "hxc_legacy_control_flow",
        "entry",
        "exit",
        "hxc_legacy_label_3",
        CONTROL_FLOW_ARTIFACT_PATH,
        1,
    ),
    GotoProvenance(
        "irreducible-cfg",
        "synthetic.nested-irreducible",
        "hxc_legacy_control_flow",
        "left-entry",
        "right-entry",
        "hxc_legacy_label_2",
        CONTROL_FLOW_ARTIFACT_PATH,
        2,
    ),
    GotoProvenance(
        "irreducible-cfg",
        "synthetic.nested-irreducible",
        "hxc_legacy_control_flow",
        "right-entry",
        "left-entry",
        "hxc_legacy_label_1",
        CONTROL_FLOW_ARTIFACT_PATH,
        2,
    ),
    GotoProvenance(
        "irreducible-cfg",
        "synthetic.nested-irreducible",
        "hxc_legacy_control_flow",
        "right-entry",
        "entry",
        "hxc_legacy_label_0",
        CONTROL_FLOW_ARTIFACT_PATH,
        1,
    ),
)


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(
    label: str, *, reverse: bool = False, profile: str = "portable"
) -> tuple[str, dict[str, object]]:
    command = [development_tool("haxe"), str(HXML)]
    if reverse:
        command.extend(["-D", "evaluation_order_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "evaluation_order_profile=metal"])
    elif profile != "portable":
        raise EvaluationOrderFailure(f"unknown evaluation-order profile {profile!r}")
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise EvaluationOrderFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise EvaluationOrderFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise EvaluationOrderFailure(f"{label} report is not an object")
    return payload, report


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise EvaluationOrderFailure(f"report field {key!r} must be text")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    value = report.get("sources")
    if not isinstance(value, list):
        raise EvaluationOrderFailure("report sources must be an array")
    records: dict[str, str] = {}
    ordered: list[str] = []
    for item in value:
        if (
            not isinstance(item, dict)
            or not isinstance(item.get("path"), str)
            or not isinstance(item.get("content"), str)
        ):
            raise EvaluationOrderFailure(f"invalid source record: {item!r}")
        path = item["path"]
        if path in records or path.startswith("/") or "\\" in path:
            raise EvaluationOrderFailure(f"invalid source path {path!r}")
        records[path] = item["content"]
        ordered.append(path)
    if ordered != sorted(ordered):
        raise EvaluationOrderFailure("source records are not path sorted")
    return records


def symbol_name(symbols: dict[str, object], source: str) -> str:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise EvaluationOrderFailure("finalized symbol table omitted its entries")
    matches = [
        entry.get("cName")
        for entry in entries
        if isinstance(entry, dict)
        and isinstance(entry.get("sourceSymbol"), str)
        and (
            entry["sourceSymbol"] == source
            or entry["sourceSymbol"].startswith(source + "(")
        )
        and isinstance(entry.get("cName"), str)
    ]
    if len(matches) != 1:
        raise EvaluationOrderFailure(
            f"could not resolve exactly one finalized C name for {source!r}: {matches!r}"
        )
    return matches[0]


def control_flow_goto_provenance(
    report: dict[str, object],
) -> tuple[GotoProvenance, ...]:
    value = report.get("controlFlowGotoProvenance")
    if not isinstance(value, list):
        raise EvaluationOrderFailure("control-flow goto provenance must be an array")
    fields = {
        "category",
        "functionId",
        "cFunctionName",
        "ownerBlockId",
        "targetBlockId",
        "targetLabel",
        "path",
        "occurrence",
    }
    result: list[GotoProvenance] = []
    for item in value:
        if not isinstance(item, dict) or set(item) != fields:
            raise EvaluationOrderFailure(f"invalid control-flow goto provenance: {item!r}")
        text_fields = fields - {"occurrence"}
        if not all(isinstance(item[field], str) for field in text_fields):
            raise EvaluationOrderFailure(
                f"control-flow goto provenance fields must be text: {item!r}"
            )
        occurrence = item["occurrence"]
        if (
            not isinstance(occurrence, int)
            or isinstance(occurrence, bool)
            or occurrence < 1
        ):
            raise EvaluationOrderFailure(
                f"control-flow goto occurrence must be a positive integer: {item!r}"
            )
        category = item["category"]
        if category not in {"loop-break-through-switch", "irreducible-cfg"}:
            raise EvaluationOrderFailure(f"unknown typed goto category {category!r}")
        result.append(
            GotoProvenance(
                category=category,
                function_id=item["functionId"],
                c_function_name=item["cFunctionName"],
                owner_block_id=item["ownerBlockId"],
                target_block_id=item["targetBlockId"],
                target_label=item["targetLabel"],
                path=item["path"],
                occurrence=occurrence,
            )
        )
    provenance = tuple(result)
    if provenance != EXPECTED_CONTROL_FLOW_GOTO_PROVENANCE:
        raise EvaluationOrderFailure(
            "validated control-flow plan goto provenance drifted: "
            f"{provenance!r}"
        )
    category_counts = Counter(item.category for item in provenance)
    if category_counts != Counter(
        {"loop-break-through-switch": 1, "irreducible-cfg": 6}
    ):
        raise EvaluationOrderFailure(
            f"control-flow goto category counts drifted: {category_counts!r}"
        )
    return provenance


def synthetic_function_source(source: str, c_name: str) -> str:
    marker = f"static void {c_name}("
    if source.count(marker) != 1:
        raise EvaluationOrderFailure(
            f"synthetic control-flow C lost unique function {c_name!r}"
        )
    start = source.find(marker)
    end = source.find("\n}\n", start)
    if end < 0:
        raise EvaluationOrderFailure(
            f"synthetic control-flow C lost the end of function {c_name!r}"
        )
    return source[start : end + 3]


def validate_control_flow_emission(
    source: str, provenance: tuple[GotoProvenance, ...]
) -> None:
    bounded_start = source.find("static void hxc_bounded_control_flow")
    legacy_start = source.find("static void hxc_legacy_control_flow")
    main_start = source.find("int main(void)")
    if min(bounded_start, legacy_start, main_start) < 0 or not (
        bounded_start < legacy_start < main_start
    ):
        raise EvaluationOrderFailure(
            "synthetic control-flow C lost its bounded/legacy/main structure"
        )
    bounded = source[bounded_start:legacy_start]
    legacy = source[legacy_start:main_start]
    if bounded.count("goto ") != 1:
        raise EvaluationOrderFailure(
            "bounded structural escape did not emit exactly one goto"
        )
    if (
        bounded.count("goto hxc_bounded_label_3;") != 1
        or bounded.count("hxc_bounded_label_3:") != 1
    ):
        raise EvaluationOrderFailure(
            "bounded structural escape lost its single owned exit label"
        )
    if legacy.count("goto ") != 6:
        raise EvaluationOrderFailure(
            "irreducible CFG fallback did not emit exactly six gotos"
        )
    if (
        "goto hxc_legacy_label_0;" not in legacy
        or legacy.count("hxc_legacy_label_0:") != 1
    ):
        raise EvaluationOrderFailure(
            "irreducible fallback lost the label for its targeted entry block"
        )
    actual_gotos: Counter[tuple[str, str, str, int]] = Counter()
    target_occurrences: Counter[tuple[str, str, str]] = Counter()
    for c_function_name in sorted({item.c_function_name for item in provenance}):
        function_source = synthetic_function_source(source, c_function_name)
        for target_label in re.findall(
            r"(?m)^\s*goto\s+([A-Za-z_][A-Za-z0-9_]*)\s*;$",
            function_source,
        ):
            occurrence_key = (
                CONTROL_FLOW_ARTIFACT_PATH,
                c_function_name,
                target_label,
            )
            target_occurrences[occurrence_key] += 1
            actual_gotos[
                (*occurrence_key, target_occurrences[occurrence_key])
            ] += 1
    expected_gotos = Counter(
        (item.path, item.c_function_name, item.target_label, item.occurrence)
        for item in provenance
    )
    all_emitted_gotos = re.findall(
        r"(?m)^\s*goto\s+[A-Za-z_][A-Za-z0-9_]*\s*;$", source
    )
    if (
        actual_gotos != expected_gotos
        or sum(actual_gotos.values()) != len(all_emitted_gotos)
        or len(all_emitted_gotos) != 7
    ):
        raise EvaluationOrderFailure(
            "emitted gotos are not covered exactly by typed plan provenance: "
            f"actual={actual_gotos!r} expected={expected_gotos!r} "
            f"total={len(all_emitted_gotos)}"
        )
    if str(ROOT) in source or "\\" in source or "hxrt" in source.lower():
        raise EvaluationOrderFailure(
            "synthetic control-flow C leaked a host path or runtime dependency"
        )


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 3
        or report.get("status") != "typed-evaluation-order-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
    ):
        raise EvaluationOrderFailure("evaluation-order schema/status/profile drifted")
    proof = required_text(report, "temporaryElisionProof")
    if "barrier-free single-use private-local loads may remain inline" not in proof:
        raise EvaluationOrderFailure("temporary-elision proof contract drifted")
    value_coalescing_proof = required_text(report, "valueCoalescingProof")
    if value_coalescing_proof != (
        "value-coalescing:single-use-private-local-and-field-and-pure-record-inline;"
        "read-call-lifetime-cleanup-failure-global-pointer-index-multiuse-fanout-cross-block-and-call-result-materialized;"
        "planner-reuse-isolated"
    ):
        raise EvaluationOrderFailure("value-coalescing adversarial proof drifted")
    control_flow_proof = required_text(report, "controlFlowPlanProof")
    if control_flow_proof != (
        "typed-region-plan:reducible-diamond-normal-joins-loop-break-return-converging-escapes-inverted-pre-post-and-bounded-switch-escape-structured;"
        "maximal-and-nested-irreducible-fallback;"
        "overlapping-normal-join-malformed-unreachable-cleanup-and-instruction-failure-region-edge-mapping-and-sequence-order-rejected"
    ):
        raise EvaluationOrderFailure("control-flow plan proof contract drifted")
    goto_provenance = control_flow_goto_provenance(report)
    validate_control_flow_emission(
        required_text(report, "controlFlowEmissionC"), goto_provenance
    )
    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise EvaluationOrderFailure(f"unexpected source partition: {sorted(sources)!r}")
    source = sources["src/program.c"]
    # Concrete C spellings are a naming-registry decision. Tests follow the
    # semantic source identity instead of freezing an obsolete generated prefix.
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise EvaluationOrderFailure("finalized symbol table is missing")
    run_name = symbol_name(symbols, "EvaluationFixture.run")
    set_flag_name = symbol_name(symbols, "EvaluationFixture.setCallFlag")
    consume_pair_name = symbol_name(symbols, "EvaluationFixture.consumePair")
    switch_subject_name = symbol_name(symbols, "EvaluationFixture.switchSubject")
    barrier_global_name = symbol_name(symbols, "EvaluationFixture.barrierValue")
    barrier_read_name = symbol_name(symbols, "EvaluationFixture.readGlobalBeforeCall")
    barrier_overwrite_name = symbol_name(symbols, "EvaluationFixture.overwriteBarrierValue")
    for text in (hxcir, header, source):
        if str(ROOT) in text or "\\" in text or "hxrt" in text.lower():
            raise EvaluationOrderFailure("artifact leaked a host path or runtime dependency")

    run_start = hxcir.find('function "function.EvaluationFixture.run"')
    run_end = hxcir.find('end function "function.EvaluationFixture.run"', run_start)
    run_ir = hxcir[run_start:run_end]
    if min(run_start, run_end) < 0:
        raise EvaluationOrderFailure("run HxcIR function is missing")
    call_markers = (
        'direct("function.EvaluationFixture.setCallFlag")',
        'direct("function.EvaluationFixture.setCallFlag")',
        'direct("function.EvaluationFixture.setCallFlag")',
        'direct("function.EvaluationFixture.consumePair")',
    )
    positions: list[int] = []
    cursor = 0
    for marker in call_markers:
        cursor = run_ir.find(marker, cursor)
        positions.append(cursor)
        cursor += len(marker)
    if -1 in positions or positions != sorted(positions):
        raise EvaluationOrderFailure("call arguments are not sequenced before their consumer")
    for marker in (
        "short-circuit-result-initialize",
        "terminator branch",
        "short-circuit-store",
        "conditional-true-store",
        "conditional-false-store",
        "static-call-argument-0-initialize",
        "static-call-argument-0-load",
        "terminator switch",
        "while-condition",
        "do-condition",
        "switch-case-0",
        "switch-case-1",
        "switch-exit",
        'load place=global("global.EvaluationFixture.callFlag")',
    ):
        if marker not in run_ir:
            raise EvaluationOrderFailure(f"HxcIR lost explicit sequencing marker {marker!r}")
    if len(re.findall(r'^    block "[^"]+\.while-condition"', run_ir, re.MULTILINE)) != 2:
        raise EvaluationOrderFailure("source while and range-for condition blocks drifted")
    if run_ir.count('direct("function.EvaluationFixture.setCallFlag")') != 5:
        raise EvaluationOrderFailure("required sequenced conditional and short-circuit calls drifted")
    if re.search(
        r'instruction "[^"]+\.store" result=- store place=local\(', run_ir
    ) is None:
        raise EvaluationOrderFailure("HxcIR lost explicit local assignment sequencing")
    increment_sequences = re.findall(
        r'instruction "[^"]+\.increment-load" result="([^"]+)"[^\n]+ load [^\n]+\n'
        r'\s+instruction "[^"]+\.increment-one" result="([^"]+)"[^\n]+ constant [^\n]+\n'
        r'\s+instruction "[^"]+\.increment" result="([^"]+)"[^\n]+ binary [^\n]+\n'
        r'\s+instruction "[^"]+\.increment-store" result=- store [^\n]+ value="([^"]+)"[^\n]+\n'
        r'\s+instruction "[^"]+\.initialize" result=- initialize [^\n]+ value="([^"]+)"',
        run_ir,
    )
    if len(increment_sequences) < 2:
        raise EvaluationOrderFailure("source increment load/operate/store sequences drifted")
    postfix_old, _, postfix_new, postfix_store, postfix_result = increment_sequences[0]
    prefix_old, _, prefix_new, prefix_store, prefix_result = increment_sequences[1]
    if (
        postfix_store != postfix_new
        or postfix_result != postfix_old
        or prefix_store != prefix_new
        or prefix_result != prefix_new
        or postfix_old == postfix_new
        or prefix_old == prefix_new
    ):
        raise EvaluationOrderFailure("postfix-old/prefix-new increment semantics drifted")

    semantic_index = (ROOT / "test/hxc_ir/expected/semantic.hxcir").read_text(
        encoding="utf-8"
    )
    indexed_markers = (
        'call dispatch=direct("fn.nextIndex")',
        'address place=index(local("local.array"),"value.index")',
        'load place=dereference("value.slot")',
        'call dispatch=direct("fn.produce")',
        'store place=dereference("value.slot")',
    )
    indexed_positions = [semantic_index.find(marker) for marker in indexed_markers]
    if -1 in indexed_positions or indexed_positions != sorted(indexed_positions):
        raise EvaluationOrderFailure("indexed compound-assignment HxcIR evidence drifted")

    run_c_start = source.find(f"uint32_t {run_name}(void)")
    run_c_end = source.find("\n}\n", run_c_start)
    run_c = source[run_c_start:run_c_end]
    if min(run_c_start, run_c_end) < 0:
        raise EvaluationOrderFailure("generated run definition is missing")
    if any(token in run_c for token in (" && ", " || ", "++", "--", " ? ")):
        raise EvaluationOrderFailure("generated C reintroduced an unsequenced source expression")
    c_calls = [
        run_c.find(f"{set_flag_name}(true)"),
        run_c.find(f"{set_flag_name}(false)"),
        run_c.find(f"{consume_pair_name}("),
    ]
    if -1 in c_calls or c_calls != sorted(c_calls):
        raise EvaluationOrderFailure("generated C did not materialize call arguments left-to-right")
    if run_c.count(f"{set_flag_name}(") != 5:
        raise EvaluationOrderFailure("generated C lost a required sequenced RHS call")
    if "global_load_result" not in run_c:
        raise EvaluationOrderFailure("generated C lost stable loads")

    barrier_ir_start = hxcir.find('function "function.EvaluationFixture.readGlobalBeforeCall"')
    barrier_ir_end = hxcir.find(
        'end function "function.EvaluationFixture.readGlobalBeforeCall"',
        barrier_ir_start,
    )
    barrier_ir = hxcir[barrier_ir_start:barrier_ir_end]
    barrier_ir_load = barrier_ir.find(
        'load place=global("global.EvaluationFixture.barrierValue")'
    )
    barrier_ir_call = barrier_ir.find(
        'direct("function.EvaluationFixture.overwriteBarrierValue")'
    )
    if (
        min(barrier_ir_start, barrier_ir_end, barrier_ir_load, barrier_ir_call) < 0
        or barrier_ir_load >= barrier_ir_call
    ):
        raise EvaluationOrderFailure(
            "source-backed barrier fixture lost its load-before-call HxcIR order"
        )
    barrier_c_start = source.find(f"int32_t {barrier_read_name}(void)")
    barrier_c_end = source.find("\n}\n", barrier_c_start)
    barrier_c = source[barrier_c_start:barrier_c_end]
    materialized_global = re.search(
        rf"int32_t (hxc_tmp_[A-Za-z0-9_]+) = {re.escape(barrier_global_name)};",
        barrier_c,
    )
    barrier_c_call = barrier_c.find(f"{barrier_overwrite_name}(")
    if (
        min(barrier_c_start, barrier_c_end, barrier_c_call) < 0
        or materialized_global is None
        or materialized_global.start() >= barrier_c_call
    ):
        raise EvaluationOrderFailure(
            "generated C delayed an observable global load across a mutating call"
        )
    if "goto " in run_c or re.search(r"(?m)^\s*hxc_(?:temp|tmp)_[A-Za-z0-9_]+:$", run_c):
        raise EvaluationOrderFailure(
            "reducible generated C regressed to blanket labels or gotos"
        )
    if (
        run_c.count("while (1)") < 3
        or "\n    do\n" not in run_c
        or "continue;" not in run_c
        or "break;" not in run_c
    ):
        raise EvaluationOrderFailure(
            "generated C lost structured while/do-while/break/continue lowering"
        )
    if run_c.count("switch (") != 2:
        raise EvaluationOrderFailure("generated C lost its statement/value structural switches")
    for label in (
        "case 1:",
        "case 2:",
        "case 3:",
        "case 18:",
        "case 19:",
        "default:",
    ):
        start = run_c.find(label)
        if start < 0 or "break;" not in run_c[start : start + 1200]:
            raise EvaluationOrderFailure(
                f"generated switch arm {label!r} lost its structural non-fallthrough break"
            )
    arm_labels = re.findall(r'(?m)^\s+(?:case [^:]+|default):$', run_c)
    if len(arm_labels) != 7:
        raise EvaluationOrderFailure("generated C switch arm grouping drifted")
    if run_c.count(f"{switch_subject_name}(") != 1:
        raise EvaluationOrderFailure("switch subject was not evaluated exactly once")

    globals_value = report.get("globals")
    if not isinstance(globals_value, list) or len(globals_value) != 6:
        raise EvaluationOrderFailure("referenced static-field registry drifted")
    functions = report.get("functions")
    if not isinstance(functions, list):
        raise EvaluationOrderFailure("temporary proof function records are missing")
    coalesced_private_loads = 0
    for function in functions:
        if not isinstance(function, dict) or not isinstance(function.get("field"), str):
            raise EvaluationOrderFailure(f"invalid temporary proof function: {function!r}")
        temporary_values = function.get("temporaryValues")
        if not isinstance(temporary_values, list) or not all(
            isinstance(value, str) for value in temporary_values
        ):
            raise EvaluationOrderFailure("temporary value IDs must be typed strings")
        function_id = f'function.EvaluationFixture.{function["field"]}'
        start = hxcir.find(f'function "{function_id}"')
        end = hxcir.find(f'end function "{function_id}"', start)
        if min(start, end) < 0:
            raise EvaluationOrderFailure(f"temporary proof lost {function_id}")
        function_ir = hxcir[start:end]
        load_records = re.findall(
            r'instruction "[^"]+" result="([^"]+)"[^\n]+ load place=([^\n]+)',
            function_ir,
        )
        load_values = {value for value, _ in load_records}
        protected_load_values = {
            value
            for value, place in load_records
            if not (place.startswith('local("') or place.startswith('field(local("'))
        }
        private_load_values = load_values - protected_load_values
        call_values = set(
            re.findall(r'instruction "[^"]+" result="([^"]+)"[^\n]+ call ', function_ir)
        )
        constant_values = set(
            re.findall(r'instruction "[^"]+" result="([^"]+)"[^\n]+ constant ', function_ir)
        )
        total_operator_values = set(
            re.findall(r'instruction "[^"]+" result="([^"]+)"[^\n]+ binary ', function_ir)
        )
        total_conversion_values = set(
            re.findall(r'instruction "[^"]+" result="([^"]+)"[^\n]+ convert ', function_ir)
        )
        materialized = set(temporary_values)
        consumed_calls = {
            value for value in call_values if function_ir.count(f'"{value}"') > 1
        }
        if not protected_load_values <= materialized or not consumed_calls <= materialized:
            raise EvaluationOrderFailure(
                f"{function_id} elided an observable/aliased load or consumed call without proof"
            )
        coalesced_private_loads += len(private_load_values - materialized)
        if (constant_values | total_operator_values | total_conversion_values) & materialized:
            raise EvaluationOrderFailure(
                f"{function_id} stopped applying the pure-value elision proof"
            )
    if coalesced_private_loads == 0:
        raise EvaluationOrderFailure(
            "production integration did not coalesce any proven private-local load"
        )


def hxcir_function_sources(hxcir: str) -> dict[str, SourceSpan]:
    result: dict[str, SourceSpan] = {}
    for match in HXCIR_FUNCTION_SOURCE.finditer(hxcir):
        function_id = match.group("function_id")
        if function_id in result:
            raise EvaluationOrderFailure(
                f"evaluation HxcIR duplicates function identity {function_id!r}"
            )
        result[function_id] = SourceSpan(
            match.group("path"),
            int(match.group("start_line")),
            int(match.group("start_column")),
            int(match.group("end_line")),
            int(match.group("end_column")),
        )
    if not result:
        raise EvaluationOrderFailure("evaluation HxcIR omitted function source spans")
    return result


def maintainability_symbol_ledger(
    symbols: object,
) -> tuple[SymbolLedgerEntry, ...]:
    if not isinstance(symbols, dict) or not isinstance(symbols.get("symbols"), list):
        raise EvaluationOrderFailure(
            "evaluation maintainability input omitted hxc.symbols entries"
        )
    entries = symbols["symbols"]
    result: list[SymbolLedgerEntry] = []
    for entry in entries:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("cName"), str)
            or not isinstance(entry.get("sourceSymbol"), str)
            or not isinstance(entry.get("collisionResolved"), bool)
            or not isinstance(entry.get("escapeReasons"), list)
            or not all(isinstance(reason, str) for reason in entry["escapeReasons"])
        ):
            raise EvaluationOrderFailure(
                f"evaluation maintainability symbol entry is malformed: {entry!r}"
            )
        result.append(
            SymbolLedgerEntry(
                entry["cName"],
                entry["sourceSymbol"],
                entry["collisionResolved"],
                tuple(
                    sorted(
                        entry["escapeReasons"],
                        key=lambda value: value.encode("utf-8"),
                    )
                ),
            )
        )
    return tuple(
        sorted(
            result,
            key=lambda item: (
                item.c_name.encode("utf-8"),
                item.source_symbol.encode("utf-8"),
            ),
        )
    )


def evaluation_program_function_mappings(
    report: dict[str, object],
) -> tuple[FunctionSourceMapping, ...]:
    functions = report.get("functions")
    if not isinstance(functions, list):
        raise EvaluationOrderFailure(
            "evaluation maintainability input omitted function records"
        )
    spans = hxcir_function_sources(required_text(report, "hxcir"))
    mappings: list[FunctionSourceMapping] = []
    seen_ids: set[str] = set()
    for entry in functions:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("field"), str)
            or not isinstance(entry.get("cName"), str)
        ):
            raise EvaluationOrderFailure(
                f"evaluation maintainability function mapping is malformed: {entry!r}"
            )
        function_id = f'function.EvaluationFixture.{entry["field"]}'
        source = spans.get(function_id)
        if function_id in seen_ids or source is None:
            raise EvaluationOrderFailure(
                f"evaluation maintainability cannot map {function_id!r} to one Haxe span"
            )
        seen_ids.add(function_id)
        owner = ArtifactOwner(OwnerKind.SOURCE_MODULE, "EvaluationFixture", source)
        mappings.append(
            FunctionSourceMapping(function_id, entry["cName"], owner, source)
        )
    if seen_ids != set(spans):
        raise EvaluationOrderFailure(
            "evaluation maintainability function records and HxcIR spans differ: "
            f"records={sorted(seen_ids)!r} spans={sorted(spans)!r}"
        )
    mappings.append(
        FunctionSourceMapping(
            "compiler.entry.main",
            "main",
            ArtifactOwner(OwnerKind.COMPILER_ENTRY),
        )
    )
    return tuple(
        sorted(mappings, key=lambda item: item.function_id.encode("utf-8"))
    )


def synthetic_function_mappings(
    provenance: tuple[GotoProvenance, ...],
) -> tuple[FunctionSourceMapping, ...]:
    owner = ArtifactOwner(OwnerKind.SYNTHETIC_FIXTURE)
    identities: dict[str, str] = {}
    for item in provenance:
        existing = identities.get(item.c_function_name)
        if existing is not None and existing != item.function_id:
            raise EvaluationOrderFailure(
                f"synthetic C function {item.c_function_name!r} has conflicting plan identities"
            )
        identities[item.c_function_name] = item.function_id
    if identities != {
        "hxc_bounded_control_flow": "synthetic.loop-switch-break",
        "hxc_legacy_control_flow": "synthetic.nested-irreducible",
    }:
        raise EvaluationOrderFailure(
            f"synthetic control-flow identities drifted: {identities!r}"
        )
    mappings = [
        FunctionSourceMapping(function_id, c_name, owner)
        for c_name, function_id in identities.items()
    ]
    mappings.append(
        FunctionSourceMapping("synthetic.control-flow.main", "main", owner)
    )
    return tuple(
        sorted(mappings, key=lambda item: item.function_id.encode("utf-8"))
    )


def validate_evaluation_maintainability_report(
    report: dict[str, object], *, corpus_id: str, layout: str
) -> None:
    validate_maintainability_report(report)
    summary = report.get("summary")
    if not isinstance(summary, dict):
        raise EvaluationOrderFailure(
            f"{corpus_id} maintainability report omitted its summary"
        )
    if (
        report.get("corpusId") != corpus_id
        or report.get("layout") != layout
        or report.get("status") != "within-reviewed-budgets"
        or summary.get("artifactCount") != 1
        or summary.get("functionIdentityMappedBasisPoints") != 10000
        or summary.get("unauthorizedGotoCount") != 0
    ):
        raise EvaluationOrderFailure(
            f"{corpus_id}/{layout} maintainability policy evidence drifted"
        )
    if corpus_id == "evaluation-order-program":
        if summary.get("gotoCount") != 0:
            raise EvaluationOrderFailure(
                "evaluation program maintainability report admitted a goto"
            )
        return
    expected_gotos = sorted(
        (
            item.path,
            item.function_id,
            item.target_label,
            item.occurrence,
            item.category,
        )
        for item in EXPECTED_CONTROL_FLOW_GOTO_PROVENANCE
    )
    gotos = report.get("gotoOccurrences")
    if not isinstance(gotos, list):
        raise EvaluationOrderFailure(
            "synthetic maintainability report omitted goto occurrences"
        )
    actual_gotos = [
        (
            item.get("path"),
            item.get("functionId"),
            item.get("targetLabel"),
            item.get("occurrence"),
            item.get("category"),
        )
        for item in gotos
        if isinstance(item, dict)
    ]
    if (
        len(actual_gotos) != len(gotos)
        or actual_gotos != expected_gotos
        or summary.get("gotoCount") != 7
        or summary.get("gotoCategoryCounts")
        != [
            {"category": "irreducible-cfg", "occurrences": 6},
            {"category": "loop-break-through-switch", "occurrences": 1},
        ]
    ):
        raise EvaluationOrderFailure(
            "synthetic maintainability report does not exactly consume plan-derived goto authorities"
        )


def evaluation_program_maintainability_report(
    report: dict[str, object],
) -> dict[str, object]:
    source = source_records(report)["src/program.c"].encode("utf-8")
    policy = load_corpus_policy(
        MAINTAINABILITY_POLICY,
        corpus_id="evaluation-order-program",
        layout="canonical",
    )
    maintainability = analyze_generated_c(
        corpus_id="evaluation-order-program",
        layout="canonical",
        artifacts=(
            GeneratedCArtifact(
                "program.c",
                source,
                ArtifactOwner(OwnerKind.AMALGAMATION),
                evaluation_program_function_mappings(report),
            ),
        ),
        symbols=maintainability_symbol_ledger(report.get("symbols")),
        policy=policy,
    )
    validate_evaluation_maintainability_report(
        maintainability,
        corpus_id="evaluation-order-program",
        layout="canonical",
    )
    return maintainability


def evaluation_synthetic_maintainability_report(
    report: dict[str, object],
) -> dict[str, object]:
    provenance = control_flow_goto_provenance(report)
    policy = load_corpus_policy(
        MAINTAINABILITY_POLICY,
        corpus_id="evaluation-order-synthetic",
        layout="synthetic",
    )
    maintainability = analyze_generated_c(
        corpus_id="evaluation-order-synthetic",
        layout="synthetic",
        artifacts=(
            GeneratedCArtifact(
                CONTROL_FLOW_ARTIFACT_PATH,
                required_text(report, "controlFlowEmissionC").encode("utf-8"),
                ArtifactOwner(OwnerKind.SYNTHETIC_FIXTURE),
                synthetic_function_mappings(provenance),
            ),
        ),
        goto_authorities=tuple(
            GotoAuthority(
                item.path,
                item.function_id,
                item.target_label,
                item.occurrence,
                GotoCategory(item.category),
            )
            for item in provenance
        ),
        policy=policy,
    )
    validate_evaluation_maintainability_report(
        maintainability,
        corpus_id="evaluation-order-synthetic",
        layout="synthetic",
    )
    return maintainability


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "evaluation.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "synthetic-control-flow.c": required_text(report, "controlFlowEmissionC"),
        "symbols.json": report.get("symbols"),
        "maintainability-program.json": evaluation_program_maintainability_report(
            report
        ),
        "maintainability-synthetic.json": evaluation_synthetic_maintainability_report(
            report
        ),
    }


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(report: dict[str, object]) -> None:
    for name, actual in snapshot_values(report).items():
        path = EXPECTED / name
        if name.endswith(".json"):
            expected_value = json.loads(path.read_text(encoding="utf-8"))
            if name == "maintainability-program.json":
                if not isinstance(actual, dict) or not isinstance(expected_value, dict):
                    raise EvaluationOrderFailure(
                        "evaluation program maintainability snapshot is not an object"
                    )
                validate_evaluation_maintainability_report(
                    actual,
                    corpus_id="evaluation-order-program",
                    layout="canonical",
                )
                validate_evaluation_maintainability_report(
                    expected_value,
                    corpus_id="evaluation-order-program",
                    layout="canonical",
                )
            elif name == "maintainability-synthetic.json":
                if not isinstance(actual, dict) or not isinstance(expected_value, dict):
                    raise EvaluationOrderFailure(
                        "evaluation synthetic maintainability snapshot is not an object"
                    )
                validate_evaluation_maintainability_report(
                    actual,
                    corpus_id="evaluation-order-synthetic",
                    layout="synthetic",
                )
                validate_evaluation_maintainability_report(
                    expected_value,
                    corpus_id="evaluation-order-synthetic",
                    layout="synthetic",
                )
            if actual != expected_value:
                raise EvaluationOrderFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise EvaluationOrderFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise EvaluationOrderFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def check_oracle() -> str:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(ORACLE_HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != "2\n" or result.stderr:
        raise EvaluationOrderFailure(
            "Eval oracle drifted\n"
            f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
        )
    return result.stdout


def check_indexed_oracle() -> None:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(INDEX_ORACLE_HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if (
        result.returncode != 0
        or result.stdout != "nextIndex,produce:8\n"
        or result.stderr
    ):
        raise EvaluationOrderFailure(
            "indexed compound-assignment Eval oracle drifted\n"
            f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
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
    families = [selected] if selected else ["gcc", "clang"]
    result: list[NativeToolchain] = []
    for family in families:
        executable = shutil.which(family)
        if executable is None:
            if selected:
                raise EvaluationOrderFailure(f"required evaluation-order toolchain missing: {family}")
            print(f"evaluation-order: SKIP optional {family}: missing {family}")
            continue
        actual, version = compiler_identity(executable)
        if actual != family:
            if selected:
                raise EvaluationOrderFailure(f"required {family} identity is {actual}")
            print(f"evaluation-order: SKIP optional {family}: identity is {actual}")
            continue
        print(f"evaluation-order: {'REQUIRED' if selected else 'AVAILABLE'} {family}: cc={version}")
        result.append(NativeToolchain(family, executable, version))
    if not result:
        raise EvaluationOrderFailure("no strict C11 compiler is available")
    return result


def function_c_name(symbols: dict[str, object], source_symbol: str) -> str:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise EvaluationOrderFailure("symbol table entries are missing")
    matches = [
        item.get("cName")
        for item in entries
        if isinstance(item, dict) and item.get("sourceSymbol") == source_symbol
    ]
    if len(matches) != 1 or not isinstance(matches[0], str):
        raise EvaluationOrderFailure(f"cannot resolve unique C name for {source_symbol}")
    return matches[0]


def c_name_with_source_prefix(symbols: dict[str, object], source_prefix: str) -> str:
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise EvaluationOrderFailure("symbol table entries are missing")
    matches = [
        item.get("cName")
        for item in entries
        if isinstance(item, dict)
        and isinstance(item.get("sourceSymbol"), str)
        and item["sourceSymbol"].startswith(source_prefix)
    ]
    if len(matches) != 1 or not isinstance(matches[0], str):
        raise EvaluationOrderFailure(
            f"cannot resolve unique C name with source prefix {source_prefix}"
        )
    return matches[0]


def compile_and_run_project(
    root: Path,
    sources: list[Path],
    symbols: dict[str, object],
    toolchain: NativeToolchain,
    optimization: str,
    *,
    run_source_symbol: str = "EvaluationFixture.run",
    expected_result: int = 2,
) -> None:
    run_name = function_c_name(symbols, run_source_symbol)
    renamed_entry_header = root / "renamed_entry.h"
    renamed_entry_header.write_text(
        "int hxc_generated_main(void);\n", encoding="utf-8", newline="\n"
    )
    harness = root / "evaluation_harness.c"
    harness.write_text(
        "#include <stdint.h>\n\n"
        f"extern uint32_t {run_name}(void);\n\n"
        "int main(void)\n{\n"
        f"  return {run_name}() == UINT32_C({expected_result}) ? 0 : 1;\n"
        "}\n",
        encoding="utf-8",
        newline="\n",
    )
    objects: list[Path] = []
    for index, source in enumerate(sources):
        target = root / f"source-{index}-{toolchain.family}-{optimization[1:]}.o"
        command = [
            toolchain.compiler,
            *STRICT_FLAGS,
            optimization,
            "-Dmain=hxc_generated_main",
            "-include",
            str(renamed_entry_header),
            "-I",
            str(root / "include"),
            "-c",
            str(source),
            "-o",
            str(target),
        ]
        compiled = subprocess.run(command, capture_output=True, text=True, timeout=30)
        if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
            raise EvaluationOrderFailure(
                f"{toolchain.family} {optimization} rejected generated C\n"
                f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
            )
        objects.append(target)
    harness_object = root / f"harness-{toolchain.family}-{optimization[1:]}.o"
    compiled_harness = subprocess.run(
        [
            toolchain.compiler,
            *STRICT_FLAGS,
            optimization,
            "-I",
            str(root / "include"),
            "-c",
            str(harness),
            "-o",
            str(harness_object),
        ],
        capture_output=True,
        text=True,
        timeout=30,
    )
    if compiled_harness.returncode != 0 or compiled_harness.stdout or compiled_harness.stderr:
        raise EvaluationOrderFailure(
            f"{toolchain.family} rejected differential harness\n{compiled_harness.stdout}{compiled_harness.stderr}"
        )
    executable = root / f"evaluation-{toolchain.family}-{optimization[1:]}"
    linked = subprocess.run(
        [toolchain.compiler, *objects, str(harness_object), "-o", str(executable)],
        capture_output=True,
        text=True,
        timeout=30,
    )
    if linked.returncode != 0 or linked.stdout or linked.stderr:
        raise EvaluationOrderFailure(
            f"{toolchain.family} failed to link evaluation fixture\n{linked.stdout}{linked.stderr}"
        )
    ran = subprocess.run(
        [str(executable)], capture_output=True, text=True, timeout=10
    )
    if ran.returncode != 0 or ran.stdout or ran.stderr:
        raise EvaluationOrderFailure(
            f"{toolchain.family} {optimization} differed from Eval: "
            f"exit={ran.returncode} stdout={ran.stdout!r} stderr={ran.stderr!r}"
        )


def write_report_project(report: dict[str, object], root: Path) -> tuple[list[Path], dict[str, object]]:
    header = root / "include/hxc/program.h"
    header.parent.mkdir(parents=True)
    header.write_text(required_text(report, "header"), encoding="utf-8", newline="\n")
    sources: list[Path] = []
    for relative, content in source_records(report).items():
        path = root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8", newline="\n")
        sources.append(path)
    symbols = report.get("symbols")
    if not isinstance(symbols, dict):
        raise EvaluationOrderFailure("report symbols are missing")
    return sources, symbols


def custom_target(
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    layout: str = "unity",
    main: str = "EvaluationFixture",
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(FIXTURE),
        "-lib",
        "reflaxe.c",
        "-main",
        main,
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if layout in ("package", "unity"):
        command.extend(["-D", f"hxc_project_layout={layout}"])
    elif layout != "split":
        raise EvaluationOrderFailure(f"unknown evaluation-order layout {layout!r}")
    command.extend(["--custom-target", f"c={output}"])
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def check_production(selected: str | None = None) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-evaluation-production-") as temporary:
        root = Path(temporary)
        portable = root / "portable"
        repeated = root / "repeated"
        metal = root / "metal"
        no_runtime = root / "none"
        for label, output, profile, runtime in (
            ("portable", portable, "portable", None),
            ("repeat", repeated, "portable", None),
            ("metal", metal, "metal", None),
            ("runtime none", no_runtime, "portable", "none"),
        ):
            result = custom_target(output, profile=profile, runtime=runtime)
            if result.returncode != 0 or result.stdout or result.stderr:
                raise EvaluationOrderFailure(
                    f"{label} production compile failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        if generated_tree(portable) != generated_tree(repeated):
            raise EvaluationOrderFailure("repeated production outputs are not byte-identical")
        if (portable / "src/program.c").read_bytes() != (metal / "src/program.c").read_bytes():
            raise EvaluationOrderFailure("portable and metal sequencing C diverged")
        runtime_plan = json.loads((portable / "hxc.runtime-plan.json").read_text())
        none_plan = json.loads((no_runtime / "hxc.runtime-plan.json").read_text())
        if (
            runtime_plan.get("status") != "analyzed-runtime-free"
            or runtime_plan.get("features") != []
            or not runtime_plan.get("noRuntimeProof")
            or none_plan.get("resolvedPolicy") != "none"
        ):
            raise EvaluationOrderFailure("zero-runtime production proof drifted")
        if b"hxrt" in b"\n".join(generated_tree(portable).values()).lower():
            raise EvaluationOrderFailure("sequenced primitive project selected hxrt")
        symbols = json.loads((portable / "hxc.symbols.json").read_text())
        production_source = (portable / "src/program.c").read_text(encoding="utf-8")
        carrier_name = c_name_with_source_prefix(
            symbols, "EvaluationFixture.run.selectedBySwitch#"
        )
        carrier_declaration = f"uint32_t {carrier_name} = 0;"
        declaration_index = production_source.find(carrier_declaration)
        switch_index = production_source.find("switch (", declaration_index)
        if (
            declaration_index < 0
            or switch_index < 0
            or switch_index - declaration_index > 1000
        ):
            raise EvaluationOrderFailure(
                "production value-switch carrier lost its typed defensive initialization"
            )
        sources = sorted((portable / "src").glob("*.c"))
        for toolchain in available_compilers(selected):
            for optimization in ("-O0", "-O2"):
                compile_and_run_project(portable, sources, symbols, toolchain, optimization)


def generated_c_sources(root: Path) -> list[Path]:
    return sorted(
        root.joinpath("src").rglob("*.c"),
        key=lambda path: path.as_posix().encode(),
    )


def generated_c_function(source: str, c_name: str) -> str:
    pattern = re.compile(
        rf"(?m)^(?:bool|uint32_t) {re.escape(c_name)}\([^;\n]*\)\n\{{"
    )
    match = pattern.search(source)
    if match is None:
        raise EvaluationOrderFailure(
            f"flow-carrier C lost function definition {c_name!r}"
        )
    end = source.find("\n}\n", match.start())
    if end < 0:
        raise EvaluationOrderFailure(
            f"flow-carrier C lost the end of function {c_name!r}"
        )
    return source[match.start() : end + 3]


def check_flow_carrier_production(selected: str | None = None) -> None:
    """Keep Haxe's optimized if-assigned Boolean carriers lazy and initialized."""

    layouts = ("split", "package", "unity")
    with tempfile.TemporaryDirectory(prefix="hxc-flow-carrier-") as temporary:
        root = Path(temporary)
        jobs = [
            (layout, repetition, root / f"{layout}-{repetition}")
            for layout in layouts
            for repetition in ("first", "repeat")
        ]

        def compile_job(
            job: tuple[str, str, Path],
        ) -> tuple[str, str, Path, subprocess.CompletedProcess[str]]:
            layout, repetition, output = job
            return (
                layout,
                repetition,
                output,
                custom_target(output, layout=layout, main=FLOW_CARRIER_MAIN),
            )

        # Every compiler process owns a different output root and has server
        # reuse disabled, so these determinism builds are safe to run together.
        with ThreadPoolExecutor(max_workers=3) as executor:
            results = list(executor.map(compile_job, jobs))
        outputs: dict[tuple[str, str], Path] = {}
        for layout, repetition, output, result in results:
            if result.returncode != 0 or result.stdout or result.stderr:
                raise EvaluationOrderFailure(
                    f"flow-carrier {layout} {repetition} compile failed\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
            outputs[(layout, repetition)] = output

        for layout in layouts:
            first = outputs[(layout, "first")]
            repeat = outputs[(layout, "repeat")]
            if generated_tree(first) != generated_tree(repeat):
                raise EvaluationOrderFailure(
                    f"flow-carrier {layout} output changed across isolated builds"
                )
            runtime_plan = json.loads(
                first.joinpath("hxc.runtime-plan.json").read_text(encoding="utf-8")
            )
            if (
                runtime_plan.get("status") != "analyzed-runtime-free"
                or runtime_plan.get("features") != []
                or not runtime_plan.get("noRuntimeProof")
            ):
                raise EvaluationOrderFailure(
                    f"flow-carrier {layout} selected a runtime feature"
                )

        unity = outputs[("unity", "first")]
        c_sources = generated_c_sources(unity)
        if not c_sources:
            raise EvaluationOrderFailure("flow-carrier unity build emitted no C source")
        source = "\n".join(path.read_text(encoding="utf-8") for path in c_sources)
        lowered_source = source.lower()
        for forbidden in ("goto ", "malloc(", "calloc(", "realloc(", "hxrt"):
            if forbidden in lowered_source:
                raise EvaluationOrderFailure(
                    f"flow-carrier direct C unexpectedly contains {forbidden!r}"
                )
        symbols = json.loads(
            unity.joinpath("hxc.symbols.json").read_text(encoding="utf-8")
        )
        observe_name = symbol_name(symbols, "FlowCarrierFixture.observe")
        for source_symbol in (
            "FlowCarrierFixture.carrierOr",
            "FlowCarrierFixture.carrierAnd",
        ):
            function = generated_c_function(
                source, symbol_name(symbols, source_symbol)
            )
            call_index = function.find(f"{observe_name}(")
            if (
                call_index < 0
                or function.count(f"{observe_name}(") != 1
                or function.find("if (") < 0
                or function.find("if (") >= call_index
                or " && " in function
                or " || " in function
            ):
                raise EvaluationOrderFailure(
                    f"{source_symbol} lost its structural lazy helper call"
                )

        for toolchain in available_compilers(selected):
            for optimization in ("-O0", "-O2"):
                compile_and_run_project(
                    unity,
                    c_sources,
                    symbols,
                    toolchain,
                    optimization,
                    run_source_symbol="FlowCarrierFixture.run",
                    expected_result=2,
                )


def check_native_snapshot(report: dict[str, object], selected: str | None = None) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-evaluation-native-") as temporary:
        root = Path(temporary)
        sources, symbols = write_report_project(report, root)
        for toolchain in available_compilers(selected):
            for optimization in ("-O0", "-O2"):
                compile_and_run_project(root, sources, symbols, toolchain, optimization)


def check_synthetic_control_flow_native(
    report: dict[str, object], selected: str | None = None
) -> None:
    source_text = required_text(report, "controlFlowEmissionC")
    validate_control_flow_emission(
        source_text, control_flow_goto_provenance(report)
    )
    with tempfile.TemporaryDirectory(prefix="hxc-control-flow-native-") as temporary:
        root = Path(temporary)
        source = root / "synthetic-control-flow.c"
        source.write_text(source_text, encoding="utf-8", newline="\n")
        for toolchain in available_compilers(selected):
            for optimization in ("-O0", "-O2"):
                executable = root / (
                    f"synthetic-control-flow-{toolchain.family}-{optimization[1:]}"
                )
                compiled = subprocess.run(
                    [
                        toolchain.compiler,
                        *STRICT_FLAGS,
                        optimization,
                        str(source),
                        "-o",
                        str(executable),
                    ],
                    capture_output=True,
                    text=True,
                    timeout=30,
                )
                if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
                    raise EvaluationOrderFailure(
                        f"{toolchain.family} {optimization} rejected synthetic control-flow C\n"
                        f"stdout:\n{compiled.stdout}stderr:\n{compiled.stderr}"
                    )
                ran = subprocess.run(
                    [str(executable)], capture_output=True, text=True, timeout=10
                )
                if ran.returncode != 0 or ran.stdout or ran.stderr:
                    raise EvaluationOrderFailure(
                        f"{toolchain.family} {optimization} synthetic control-flow execution failed: "
                        f"exit={ran.returncode} stdout={ran.stdout!r} stderr={ran.stderr!r}"
                    )


def snapshot_native_report() -> dict[str, object]:
    return {
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "sources": [
            {
                "path": "src/program.c",
                "content": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
        ],
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
        "controlFlowEmissionC": (EXPECTED / "synthetic-control-flow.c").read_text(
            encoding="utf-8"
        ),
        "controlFlowGotoProvenance": [
            {
                "category": item.category,
                "functionId": item.function_id,
                "cFunctionName": item.c_function_name,
                "ownerBlockId": item.owner_block_id,
                "targetBlockId": item.target_block_id,
                "targetLabel": item.target_label,
                "path": item.path,
                "occurrence": item.occurrence,
            }
            for item in EXPECTED_CONTROL_FLOW_GOTO_PROVENANCE
        ],
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("evaluation-order: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            native_report = snapshot_native_report()
            check_native_snapshot(native_report, args.toolchain)
            check_synthetic_control_flow_native(native_report, args.toolchain)
            print("evaluation-order: OK: required strict-C differential native matrix passed")
            return 0
        check_oracle()
        check_indexed_oracle()
        first_payload, first = render("first evaluation-order render")
        second_payload, second = render("second evaluation-order render")
        reverse_payload, reverse = render("reverse evaluation-order render", reverse=True)
        metal_payload, metal = render("metal evaluation-order render", profile="metal")
        if first_payload != second_payload or first != second:
            raise EvaluationOrderFailure("repeated evaluation-order renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise EvaluationOrderFailure("evaluation-order render changed with input order")
        validate(first)
        validate(metal, profile="metal")
        if {**first, "profile": "<profile>"} != {**metal, "profile": "<profile>"}:
            raise EvaluationOrderFailure("portable and metal sequencing reports diverged")
        check_snapshots(first)
        check_native_snapshot(first, args.toolchain)
        check_synthetic_control_flow_native(first, args.toolchain)
        check_production(args.toolchain)
        check_flow_carrier_production(args.toolchain)
    except (
        EvaluationOrderFailure,
        MaintainabilityError,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"evaluation-order: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "evaluation-order: OK: explicit calls/assignments/lazy values, nested loops/jumps, non-fallthrough switches, "
        "optimized flow carriers, Eval differential, and zero-runtime strict C passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
