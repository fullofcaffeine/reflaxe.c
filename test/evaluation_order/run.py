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
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("evaluation_order.hxml")
ORACLE_HXML = Path(__file__).with_name("oracle.hxml")
INDEX_ORACLE_HXML = ROOT / "test/hxc_ir/oracle.hxml"
FIXTURE = Path(__file__).with_name("fixtures")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_EVALUATION_ORDER="
SOURCE_SNAPSHOTS = {"program.c": "src/program.c"}
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


class EvaluationOrderFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class NativeToolchain:
    family: str
    compiler: str
    version: str


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


def validate_control_flow_emission(source: str) -> None:
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
    if (
        "goto hxc_legacy_label_0;" not in legacy
        or legacy.count("hxc_legacy_label_0:") != 1
    ):
        raise EvaluationOrderFailure(
            "irreducible fallback lost the label for its targeted entry block"
        )
    if str(ROOT) in source or "\\" in source or "hxrt" in source.lower():
        raise EvaluationOrderFailure(
            "synthetic control-flow C leaked a host path or runtime dependency"
        )


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "typed-evaluation-order-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
    ):
        raise EvaluationOrderFailure("evaluation-order schema/status/profile drifted")
    proof = required_text(report, "temporaryElisionProof")
    if "every load and consumed call is materialized" not in proof:
        raise EvaluationOrderFailure("temporary-elision proof contract drifted")
    control_flow_proof = required_text(report, "controlFlowPlanProof")
    if control_flow_proof != (
        "typed-region-plan:reducible-diamond-normal-joins-loop-break-return-converging-escapes-inverted-pre-post-and-bounded-switch-escape-structured;"
        "maximal-and-nested-irreducible-fallback;"
        "overlapping-normal-join-malformed-unreachable-cleanup-and-instruction-failure-region-edge-mapping-and-sequence-order-rejected"
    ):
        raise EvaluationOrderFailure("control-flow plan proof contract drifted")
    validate_control_flow_emission(required_text(report, "controlFlowEmissionC"))
    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise EvaluationOrderFailure(f"unexpected source partition: {sorted(sources)!r}")
    source = sources["src/program.c"]
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
    if run_ir.count('direct("function.EvaluationFixture.setCallFlag")') != 4:
        raise EvaluationOrderFailure("required short-circuit RHS calls drifted")
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

    run_c_start = source.find("uint32_t hxc_method_EvaluationFixture_run(void)")
    run_c_end = source.find("\n}\n", run_c_start)
    run_c = source[run_c_start:run_c_end]
    if min(run_c_start, run_c_end) < 0:
        raise EvaluationOrderFailure("generated run definition is missing")
    if any(token in run_c for token in (" && ", " || ", "++", "--", " ? ")):
        raise EvaluationOrderFailure("generated C reintroduced an unsequenced source expression")
    c_calls = [
        run_c.find("hxc_method_EvaluationFixture_setCallFlag(true)"),
        run_c.find("hxc_method_EvaluationFixture_setCallFlag(false)"),
        run_c.find("hxc_method_EvaluationFixture_consumePair("),
    ]
    if -1 in c_calls or c_calls != sorted(c_calls):
        raise EvaluationOrderFailure("generated C did not materialize call arguments left-to-right")
    if run_c.count("hxc_method_EvaluationFixture_setCallFlag(") != 4:
        raise EvaluationOrderFailure("generated C lost a required short-circuit RHS call")
    if "globalzx2Dloadzx2Dresult" not in run_c:
        raise EvaluationOrderFailure("generated C lost stable loads")
    if "goto " in run_c or re.search(r"(?m)^\s*hxc_temp_[A-Za-z0-9_]+:$", run_c):
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
    if run_c.count("hxc_method_EvaluationFixture_switchSubject(") != 1:
        raise EvaluationOrderFailure("switch subject was not evaluated exactly once")

    globals_value = report.get("globals")
    if not isinstance(globals_value, list) or len(globals_value) != 5:
        raise EvaluationOrderFailure("referenced static-field registry drifted")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
        raise EvaluationOrderFailure("finalized symbol table is missing")

    functions = report.get("functions")
    if not isinstance(functions, list):
        raise EvaluationOrderFailure("temporary proof function records are missing")
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
        load_values = set(
            re.findall(r'instruction "[^"]+" result="([^"]+)"[^\n]+ load ', function_ir)
        )
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
        if not load_values <= materialized or not consumed_calls <= materialized:
            raise EvaluationOrderFailure(
                f"{function_id} elided a load or consumed call without proof"
            )
        if (constant_values | total_operator_values | total_conversion_values) & materialized:
            raise EvaluationOrderFailure(
                f"{function_id} stopped applying the pure-value elision proof"
            )


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "evaluation.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "synthetic-control-flow.c": required_text(report, "controlFlowEmissionC"),
        "symbols.json": report.get("symbols"),
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
            if actual != json.loads(path.read_text(encoding="utf-8")):
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
) -> None:
    run_name = function_c_name(symbols, "EvaluationFixture.run")
    renamed_entry_header = root / "renamed_entry.h"
    renamed_entry_header.write_text(
        "int hxc_generated_main(void);\n", encoding="utf-8", newline="\n"
    )
    harness = root / "evaluation_harness.c"
    harness.write_text(
        '#include "hxc/program.h"\n\n'
        "int main(void)\n{\n"
        f"  return {run_name}() == UINT32_C(2) ? 0 : 1;\n"
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


def custom_target(output: Path, *, profile: str = "portable", runtime: str | None = None) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(FIXTURE),
        "-lib",
        "reflaxe.c",
        "-main",
        "EvaluationFixture",
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    command.extend(["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"])
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
    validate_control_flow_emission(source_text)
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
    except (
        EvaluationOrderFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"evaluation-order: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "evaluation-order: OK: explicit calls/assignments/lazy values, nested loops/jumps, non-fallthrough switches, "
        "Eval differential, and zero-runtime strict C passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
