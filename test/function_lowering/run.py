#!/usr/bin/env python3
"""Prove typed static functions, direct calls, recursive prototypes, and production entry emission."""

from __future__ import annotations

import argparse
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
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    CFixtureFailure,
    CFixtureProject,
    run_c_fixture_corpus,
    validate_report,
)


HXML = Path(__file__).with_name("function_lowering.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
SPLIT_RECURSIVE = FIXTURES / "split_recursive"
MODULE_FIELDS = FIXTURES / "module_fields"
MODULE_FIELDS_UNSUPPORTED = FIXTURES / "module_fields_unsupported"
DEFAULT_ARGUMENT = FIXTURES / "default"
OPTIONAL_ARGUMENT = FIXTURES / "optional"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_FUNCTION_LOWERING="
SOURCE_SNAPSHOTS = {
    "nonreturn_0000.c": "src/nonreturn_0000.c",
    "nonreturn_0001.c": "src/nonreturn_0001.c",
    "nonreturn_0002.c": "src/nonreturn_0002.c",
    "program.c": "src/program.c",
}
MODULE_FIELD_SNAPSHOTS = {
    "module-fields-inventory.json": "inventory",
    "module-fields-project.json": "project",
    "module-fields-symbols.json": "symbols",
    "module-fields.h": "header",
    "module-fields.c": "source",
    "module-fields-main.c": "mainSource",
}
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


class FunctionLoweringFailure(RuntimeError):
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
        command.extend(["-D", "function_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "function_lowering_profile=metal"])
    elif profile != "portable":
        raise FunctionLoweringFailure(f"unknown function profile {profile!r}")
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
        raise FunctionLoweringFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise FunctionLoweringFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise FunctionLoweringFailure(f"{label} report is not a JSON object")
    return payload, report


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise FunctionLoweringFailure(f"function report field {key} is not text")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    value = report.get("sources")
    if not isinstance(value, list):
        raise FunctionLoweringFailure("function report field sources is not an array")
    result: dict[str, str] = {}
    ordered_paths: list[str] = []
    for entry in value:
        if (
            not isinstance(entry, dict)
            or not isinstance(entry.get("path"), str)
            or not isinstance(entry.get("content"), str)
        ):
            raise FunctionLoweringFailure(f"invalid function source record: {entry!r}")
        path = entry["path"]
        if (
            path in result
            or path.startswith("/")
            or "\\" in path
            or any(part in ("", ".", "..") for part in path.split("/"))
        ):
            raise FunctionLoweringFailure(f"invalid or duplicate function source path {path!r}")
        result[path] = entry["content"]
        ordered_paths.append(path)
    if ordered_paths != sorted(ordered_paths):
        raise FunctionLoweringFailure("function source records are not path-sorted")
    return result


def required_source(report: dict[str, object], path: str) -> str:
    value = source_records(report).get(path)
    if value is None:
        raise FunctionLoweringFailure(f"function report omitted source {path}")
    return value


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 2
        or report.get("status") != "typed-static-functions-direct-calls-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
        or report.get("runtimeArtifacts") != []
    ):
        raise FunctionLoweringFailure("function-lowering schema/status/profile drifted")
    for key in ("hxcir", "header"):
        value = required_text(report, key)
        if str(ROOT) in value or "\\" in value or "hxrt" in value.lower():
            raise FunctionLoweringFailure(f"{key} leaked a host path or runtime dependency")
    sources = source_records(report)
    if set(sources) != set(SOURCE_SNAPSHOTS.values()):
        raise FunctionLoweringFailure(
            f"function source partition drifted: {sorted(sources)!r}"
        )
    for path, value in sources.items():
        if str(ROOT) in value or "\\" in value or "hxrt" in value.lower():
            raise FunctionLoweringFailure(
                f"source {path} leaked a host path or runtime dependency"
            )
        if not value.startswith('#include "hxc/program.h"\n'):
            raise FunctionLoweringFailure(
                f"source {path} omitted the private prototype header"
            )

    hxcir = required_text(report, "hxcir")
    conversion = hxcir.find('instruction "instruction.0.convert"')
    converted_call = hxcir.find(
        'call dispatch=direct("function.FunctionFixture.asFloat")'
    )
    if conversion == -1 or converted_call == -1 or conversion > converted_call:
        raise FunctionLoweringFailure(
            "implicit argument conversion was not ordered before its direct call in HxcIR"
        )
    ordered_start = hxcir.find(
        'function "function.FunctionFixture.ordered"'
    )
    ordered_end = hxcir.find(
        'end function "function.FunctionFixture.ordered"', ordered_start
    )
    ordered_ir = hxcir[ordered_start:ordered_end]
    ordered_calls = [
        ordered_ir.find(
            f'call dispatch=direct("function.FunctionFixture.{target}")'
        )
        for target in ("passthrough", "chain", "first")
    ]
    if (
        ordered_start == -1
        or ordered_end == -1
        or any(index == -1 for index in ordered_calls)
        or ordered_calls != sorted(ordered_calls)
    ):
        raise FunctionLoweringFailure(
            "two-argument nested calls lost left-to-right HxcIR evaluation order"
        )
    for marker in (
        'direct("function.FunctionFixture.recursive")',
        'direct("function.FunctionFixture.mutualLeft")',
        'direct("function.FunctionFixture.mutualRight")',
    ):
        if marker not in hxcir:
            raise FunctionLoweringFailure(f"recursive call graph omitted {marker}")

    header = required_text(report, "header")
    program_source = sources["src/program.c"]
    mutual_left_source = sources["src/nonreturn_0000.c"]
    mutual_right_source = sources["src/nonreturn_0001.c"]
    recursive_source = sources["src/nonreturn_0002.c"]
    if (
        not header.startswith("#ifndef HXC_PROGRAM_H_INCLUDED\n")
        or "_Noreturn void hxc_FunctionFixture_recursive(" not in header
        or "_Noreturn void hxc_FunctionFixture_mutualLeft(" not in header
        or "_Noreturn void hxc_FunctionFixture_mutualRight(" not in header
        or "recursiveStep" in header
        or "int main(void)" not in program_source
        or "hxc_FunctionFixture_main();" not in program_source
        or "return 0;" not in program_source
    ):
        raise FunctionLoweringFailure("prototype plan or executable entry shape drifted")
    cycle_sources = {
        "mutualLeft": mutual_left_source,
        "mutualRight": mutual_right_source,
        "recursive": recursive_source,
    }
    for field, source in cycle_sources.items():
        definition_start = source.find(
            f"_Noreturn void hxc_FunctionFixture_{field}("
        )
        definition_end = source.find("\n}\n", definition_start)
        definition = source[definition_start:definition_end]
        if (
            definition_start == -1
            or definition_end == -1
            or "\n  return" in definition
        ):
            raise FunctionLoweringFailure(
                f"closed recursive cycle {field} lost its no-return C proof"
            )
        other_definitions = [
            other
            for other in cycle_sources
            if other != field
            and f"_Noreturn void hxc_FunctionFixture_{other}(" in source
        ]
        if other_definitions:
            raise FunctionLoweringFailure(
                f"closed recursive definitions shared one translation unit: "
                f"{field}, {other_definitions!r}"
            )
    if (
        "hxc_FunctionFixture_mutualRight(" not in mutual_left_source
        or "hxc_FunctionFixture_mutualLeft(" not in mutual_right_source
        or "while (1)" not in recursive_source
        or "continue;" not in recursive_source
        or "hxc_tmp_tail_argument" not in recursive_source
        or recursive_source.count("hxc_FunctionFixture_recursive(") != 1
    ):
        raise FunctionLoweringFailure(
            "direct/mutual recursion did not retain calls or typed self-tail lowering"
        )
    tail_steps = [
        recursive_source.find(marker)
        for marker in (
            "hxc_tmp_tail_argument_n0 = hxc_right;",
            "hxc_tmp_tail_argument_n1 = hxc_left;",
            "hxc_left = hxc_tmp_tail_argument_n0;",
            "hxc_right = hxc_tmp_tail_argument_n1;",
        )
    ]
    if any(index == -1 for index in tail_steps) or tail_steps != sorted(tail_steps):
        raise FunctionLoweringFailure(
            "self-tail parameter swap was not materialized before ordered assignment"
        )
    left_prototype = header.find("hxc_FunctionFixture_mutualLeft")
    right_prototype = header.find("hxc_FunctionFixture_mutualRight")
    left_definition = mutual_left_source.find("hxc_FunctionFixture_mutualLeft")
    right_definition = mutual_right_source.find("hxc_FunctionFixture_mutualRight")
    if min(left_prototype, right_prototype, left_definition, right_definition) < 0:
        raise FunctionLoweringFailure("recursive prototype/definition evidence is incomplete")
    ordered_source_start = program_source.find("int32_t hxc_FunctionFixture_ordered(")
    ordered_source_end = program_source.find("\n}\n", ordered_source_start)
    ordered_source = program_source[ordered_source_start:ordered_source_end]
    ordered_c_calls = [
        ordered_source.find(f"hxc_FunctionFixture_{target}(")
        for target in ("passthrough", "chain", "first")
    ]
    if (
        ordered_source_start == -1
        or ordered_source_end == -1
        or any(index == -1 for index in ordered_c_calls)
        or ordered_c_calls != sorted(ordered_c_calls)
    ):
        raise FunctionLoweringFailure(
            "generated C did not materialize nested arguments before the two-argument call"
        )

    functions = report.get("functions")
    if not isinstance(functions, list) or len(functions) != 14:
        raise FunctionLoweringFailure("function report omitted admitted functions")
    by_field = {
        entry.get("field"): entry
        for entry in functions
        if isinstance(entry, dict) and isinstance(entry.get("field"), str)
    }
    if (
        len(by_field) != 14
        or by_field.get("main", {}).get("parameters") != []
        or len(by_field.get("first", {}).get("parameters", [])) != 2
        or len(by_field.get("apply", {}).get("parameters", [])) != 2
        or len(by_field.get("recursive", {}).get("parameters", [])) != 2
    ):
        raise FunctionLoweringFailure("function parameter records drifted")
    if not by_field.get("convert", {}).get("temporaries"):
        raise FunctionLoweringFailure("direct call result was not materialized as a C temporary")

    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v2":
        raise FunctionLoweringFailure("function lowering omitted its finalized symbol table")
    entries = symbols.get("symbols")
    if not isinstance(entries, list):
        raise FunctionLoweringFailure("function symbol table omitted records")
    main_symbols = [
        entry
        for entry in entries
        if isinstance(entry, dict)
        and entry.get("requestedName") == "main"
        and entry.get("cName") == "main"
    ]
    if len(main_symbols) != 1:
        raise FunctionLoweringFailure("compiler-owned exact main was not registered once")


def expected_text(name: str) -> str:
    return (EXPECTED / name).read_text(encoding="utf-8")


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    values: dict[str, object] = {
        "functions.hxcir": required_text(report, "hxcir"),
        "functions.h": required_text(report, "header"),
        "symbols.json": report.get("symbols"),
    }
    for snapshot_name, source_path in SOURCE_SNAPSHOTS.items():
        values[snapshot_name] = required_source(report, source_path)
    return values


def check_snapshots(report: dict[str, object]) -> None:
    for name, actual in snapshot_values(report).items():
        expected_path = EXPECTED / name
        if name.endswith(".json"):
            expected = json.loads(expected_path.read_text(encoding="utf-8"))
            if actual != expected:
                raise FunctionLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise FunctionLoweringFailure(f"{name} snapshot source is not text")
            expected = expected_path.read_text(encoding="utf-8")
            if actual != expected:
                raise FunctionLoweringFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        raise FunctionLoweringFailure(f"cannot identify compiler {executable}")
    combined = (result.stdout + result.stderr).strip()
    first_line = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first_line
    if "free software foundation" in lowered or "gcc" in lowered:
        return "gcc", first_line
    return "unknown", first_line


def resolve_compiler(family: str, *, required: bool) -> NativeToolchain | None:
    executable = shutil.which(family)
    if executable is None:
        if required:
            raise FunctionLoweringFailure(f"required function toolchain missing: {family}")
        print(f"function-lowering: SKIP optional {family}: missing {family}")
        return None
    actual, version = compiler_identity(executable)
    if actual != family:
        if required:
            raise FunctionLoweringFailure(
                f"required {family} command identity is {actual}"
            )
        print(
            f"function-lowering: SKIP optional {family}: command identity is {actual}"
        )
        return None
    print(
        f"function-lowering: {'REQUIRED' if required else 'AVAILABLE'} "
        f"{family}: cc={version}"
    )
    return NativeToolchain(family, executable, version)


def available_compilers(selected: str | None = None) -> list[NativeToolchain]:
    requested = [selected] if selected is not None else ["gcc", "clang"]
    result = [
        toolchain
        for family in requested
        if (toolchain := resolve_compiler(family, required=selected is not None))
        is not None
    ]
    if not result:
        raise FunctionLoweringFailure("no strict C11 compiler is available")
    return result


def write_native_project(report: dict[str, object], root: Path) -> list[Path]:
    header = root / "include/hxc/program.h"
    header.parent.mkdir(parents=True)
    header.write_text(required_text(report, "header"), encoding="utf-8", newline="\n")
    paths: list[Path] = []
    for relative, content in source_records(report).items():
        path = root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8", newline="\n")
        paths.append(path)
    return paths


def check_native(
    report: dict[str, object], selected: str | None = None
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-function-native-") as temporary:
        root = Path(temporary)
        sources = write_native_project(report, root)
        header = root / "include/hxc/program.h"
        for toolchain in available_compilers(selected):
            header_result = subprocess.run(
                [
                    toolchain.compiler,
                    *STRICT_FLAGS,
                    "-I",
                    str(root / "include"),
                    "-x",
                    "c",
                    "-fsyntax-only",
                    str(header),
                ],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            if header_result.returncode != 0 or header_result.stdout or header_result.stderr:
                raise FunctionLoweringFailure(
                    f"{toolchain.family} rejected the independent prototype header\n"
                    f"stdout:\n{header_result.stdout}\nstderr:\n{header_result.stderr}"
                )
            for optimization in ("-O0", "-O2"):
                executable = root / f"program-{toolchain.family}-{optimization[1:]}"
                compiled = subprocess.run(
                    [
                        toolchain.compiler,
                        *STRICT_FLAGS,
                        optimization,
                        "-I",
                        str(root / "include"),
                        *(str(source) for source in sources),
                        "-o",
                        str(executable),
                    ],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    text=True,
                    timeout=30,
                )
                if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
                    raise FunctionLoweringFailure(
                        f"{toolchain.family} {optimization} rejected function C\n"
                        f"stdout:\n{compiled.stdout}\nstderr:\n{compiled.stderr}"
                    )
                ran = subprocess.run(
                    [str(executable)],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    text=True,
                    timeout=10,
                )
                if ran.returncode != 0 or ran.stdout or ran.stderr:
                    raise FunctionLoweringFailure(
                        f"{toolchain.family} {optimization} executable failed: "
                        f"exit={ran.returncode} stdout={ran.stdout!r} stderr={ran.stderr!r}"
                    )


def custom_target(
    fixture: Path,
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    environment: str | None = None,
    layout: str = "unity",
    main: str | None = None,
    defines: tuple[str, ...] = (),
    connect: str | None = None,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        main if main is not None else ("Main" if fixture != POSITIVE else "FunctionFixture"),
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if environment is not None:
        command.extend(["-D", f"hxc_environment={environment}"])
    if layout in ("package", "unity"):
        command.extend(["-D", f"hxc_project_layout={layout}"])
    elif layout != "split":
        raise FunctionLoweringFailure(f"unknown function project layout {layout!r}")
    for define in defines:
        command.extend(["-D", define])
    command.extend(["--custom-target", f"c={output}"])
    if connect is not None:
        command[1:1] = ["--connect", connect]
    environment = os.environ.copy()
    if connect is None:
        environment["HAXE_NO_SERVER"] = "1"
    else:
        environment.pop("HAXE_NO_SERVER", None)
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


def planned_runtime_sources(output: Path) -> tuple[Path, ...]:
    """Resolve only the repository runtime sources selected by the compiler plan."""
    plan_path = output / "hxc.runtime-plan.json"
    plan = json.loads(plan_path.read_text(encoding="utf-8"))
    details = plan.get("artifactDetails")
    if not isinstance(details, list):
        raise FunctionLoweringFailure("runtime plan omitted artifactDetails")
    sources: set[Path] = set()
    root = ROOT.resolve()
    for index, detail in enumerate(details):
        if not isinstance(detail, dict):
            raise FunctionLoweringFailure(
                f"runtime artifactDetails[{index}] is not an object"
            )
        if detail.get("kind") != "runtime-source":
            continue
        source_path = detail.get("sourcePath")
        if not isinstance(source_path, str) or not source_path:
            raise FunctionLoweringFailure(
                f"runtime artifactDetails[{index}] omitted sourcePath"
            )
        relative = Path(source_path)
        if relative.is_absolute() or ".." in relative.parts:
            raise FunctionLoweringFailure(
                f"runtime source path is not repository-relative: {source_path!r}"
            )
        source = (ROOT / relative).resolve()
        try:
            source.relative_to(root)
        except ValueError as error:
            raise FunctionLoweringFailure(
                f"runtime source escapes the repository: {source_path!r}"
            ) from error
        if not source.is_file():
            raise FunctionLoweringFailure(
                f"planned runtime source does not exist: {source_path!r}"
            )
        sources.add(source)
    return tuple(sorted(sources))


def check_production() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-function-production-") as temporary:
        root = Path(temporary)
        first = root / "first"
        repeated = root / "repeated"
        metal = root / "metal"
        none = root / "none"
        freestanding = root / "freestanding"
        for label, output, profile, runtime in (
            ("portable", first, "portable", None),
            ("portable repeat", repeated, "portable", None),
            ("metal", metal, "metal", None),
            ("runtime none", none, "portable", "none"),
        ):
            result = custom_target(POSITIVE, output, profile=profile, runtime=runtime)
            if result.returncode != 0 or result.stdout or result.stderr:
                raise FunctionLoweringFailure(
                    f"{label} production compile failed\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        unsupported_environment = custom_target(
            POSITIVE, freestanding, environment="freestanding"
        )
        environment_text = (
            unsupported_environment.stdout + unsupported_environment.stderr
        )
        if (
            unsupported_environment.returncode != 1
            or "HXC1000" not in environment_text
            or "requires the hosted environment" not in environment_text
            or list(freestanding.rglob("*"))
        ):
            raise FunctionLoweringFailure(
                "freestanding executable entry did not fail closed without output\n"
                f"stdout:\n{unsupported_environment.stdout}\n"
                f"stderr:\n{unsupported_environment.stderr}"
            )
        if generated_tree(first) != generated_tree(repeated):
            raise FunctionLoweringFailure("two production roots were not byte-identical")
        if (first / "src/program.c").read_bytes() != (metal / "src/program.c").read_bytes():
            raise FunctionLoweringFailure("portable and metal changed primitive generated C")
        if (first / "include/hxc/program.h").read_bytes() != (
            metal / "include/hxc/program.h"
        ).read_bytes():
            raise FunctionLoweringFailure("portable and metal changed primitive prototypes")

        runtime_plan = json.loads((first / "hxc.runtime-plan.json").read_text())
        metal_plan = json.loads((metal / "hxc.runtime-plan.json").read_text())
        none_plan = json.loads((none / "hxc.runtime-plan.json").read_text())
        if (
            runtime_plan.get("status") != "analyzed-runtime-free"
            or runtime_plan.get("resolvedPolicy") != "auto"
            or runtime_plan.get("features") != []
            or not runtime_plan.get("noRuntimeProof")
            or metal_plan.get("resolvedPolicy") != "minimal"
            or none_plan.get("resolvedPolicy") != "none"
        ):
            raise FunctionLoweringFailure("resolved no-runtime policy evidence drifted")
        abi = json.loads((first / "hxc.abi.json").read_text())
        stdlib = json.loads((first / "hxc.stdlib-report.json").read_text())
        if (
            abi.get("status") != "analyzed-no-public-exports"
            or abi.get("executableEntryPoint") != "main"
            or stdlib.get("status") != "analyzed-no-stdlib-use"
        ):
            raise FunctionLoweringFailure("ABI/stdlib analysis status drifted")
        combined = b"\n".join(generated_tree(first).values()).lower()
        if b"hxrt" in combined:
            raise FunctionLoweringFailure("production primitive project selected hxrt")

        compiler = available_compilers()[0]
        executable = root / "production-program"
        production_sources = sorted((first / "src").glob("*.c"))
        if not production_sources:
            raise FunctionLoweringFailure("production project emitted no C sources")
        compiled = subprocess.run(
            [
                compiler.compiler,
                *STRICT_FLAGS,
                "-I",
                str(first / "include"),
                *(str(source) for source in production_sources),
                "-o",
                str(executable),
            ],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
            raise FunctionLoweringFailure(
                f"production generated C failed strict compile\n{compiled.stdout}{compiled.stderr}"
            )
        ran = subprocess.run(
            [str(executable)],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
        if ran.returncode != 0 or ran.stdout or ran.stderr:
            raise FunctionLoweringFailure("production executable did not exit silently with 0")


def module_field_snapshot_values(
    output: Path, inventory: dict[str, object]
) -> dict[str, object]:
    modules = inventory.get("modules")
    declarations = [
        declaration
        for module in modules
        if isinstance(module, dict) and isinstance(module.get("declarations"), list)
        for declaration in module["declarations"]
    ] if isinstance(modules, list) else []
    module_declarations = [
        declaration
        for declaration in declarations
        if isinstance(declaration, dict)
        and declaration.get("classKind") == "module-fields"
        and declaration.get("ownerModulePath") == "ModuleFunctions"
    ] if isinstance(declarations, list) else []
    if len(module_declarations) != 1:
        raise FunctionLoweringFailure(
            "typed inventory did not contain exactly one module-fields declaration"
        )
    manifest = json.loads((output / "hxc.manifest.json").read_text(encoding="utf-8"))
    symbols_document = json.loads((output / "hxc.symbols.json").read_text(encoding="utf-8"))
    symbols = symbols_document.get("symbols")
    module_symbols = [
        symbol
        for symbol in symbols
        if isinstance(symbol, dict)
        and isinstance(symbol.get("readableName"), list)
        and "ModuleFunctions" in symbol["readableName"]
    ] if isinstance(symbols, list) else []
    return {
        "inventory": module_declarations[0],
        "project": {
            "projectName": manifest.get("projectName"),
            "configuration": manifest.get("configuration"),
            "build": manifest.get("build"),
        },
        "symbols": module_symbols,
        "header": (output / "include/hxc/modules/ModuleFunctions.h").read_text(encoding="utf-8"),
        "source": (output / "src/modules/ModuleFunctions.c").read_text(encoding="utf-8"),
        "mainSource": (output / "src/hxc/main.c").read_text(encoding="utf-8"),
    }


def check_module_field_snapshots(values: dict[str, object]) -> None:
    for snapshot_name, value_name in MODULE_FIELD_SNAPSHOTS.items():
        actual = values[value_name]
        expected_path = EXPECTED / snapshot_name
        if snapshot_name.endswith(".json"):
            expected = json.loads(expected_path.read_text(encoding="utf-8"))
            if actual != expected:
                raise FunctionLoweringFailure(
                    f"{snapshot_name} semantic snapshot drifted"
                )
        else:
            if not isinstance(actual, str):
                raise FunctionLoweringFailure(
                    f"{snapshot_name} snapshot source is not text"
                )
            expected = expected_path.read_text(encoding="utf-8")
            if actual != expected:
                raise FunctionLoweringFailure(
                    f"{snapshot_name} drifted:\n"
                    + difference(expected, actual, snapshot_name)
                )


def check_module_fields() -> None:
    """Prove Haxe module functions without leaking Haxe's hidden static class."""
    with tempfile.TemporaryDirectory(prefix="hxc-module-fields-") as temporary:
        root = Path(temporary)
        output = root / "generated"
        compiled = custom_target(
            MODULE_FIELDS,
            output,
            layout="split",
            main="ModuleFunctions",
            defines=("reflaxe_c_typed_ast_report",),
        )
        inventory_lines = [
            line
            for line in compiled.stdout.splitlines()
            if line.startswith("HXC_TYPED_AST_INVENTORY=")
        ]
        if compiled.returncode != 0 or compiled.stderr or len(inventory_lines) != 1:
            raise FunctionLoweringFailure(
                "module-field production compile failed or omitted its typed inventory\n"
                f"stdout:\n{compiled.stdout}stderr:\n{compiled.stderr}"
            )
        inventory = json.loads(inventory_lines[0].split("=", 1)[1])
        snapshots = module_field_snapshot_values(output, inventory)
        typed_declaration = snapshots["inventory"]
        if not isinstance(typed_declaration, dict) or typed_declaration.get("ownerModulePath") != "ModuleFunctions":
            raise FunctionLoweringFailure(
                "typed inventory did not retain the module-fields source identity"
            )
        check_module_field_snapshots(snapshots)

        expected_paths = {
            "include/hxc/modules/ModuleFunctions.h",
            "src/hxc/main.c",
            "src/modules/ModuleFunctions.c",
        }
        manifest = json.loads((output / "hxc.manifest.json").read_text(encoding="utf-8"))
        build = manifest.get("build")
        visible_paths = set(build.get("sources", [])) | set(build.get("privateHeaders", [])) if isinstance(build, dict) else set()
        if manifest.get("projectName") != "ModuleFunctions" or not expected_paths.issubset(visible_paths):
            raise FunctionLoweringFailure(
                "module fields did not retain the source module's split-project ownership"
            )

        header = snapshots["header"]
        source = snapshots["source"]
        if not isinstance(header, str) or not isinstance(source, str):
            raise FunctionLoweringFailure("module-field snapshots lost generated C text")
        for spelling in (
            "hxc_ModuleFunctions_base",
            "hxc_ModuleFunctions_answer",
            "hxc_ModuleFunctions_doubled",
            "hxc_ModuleFunctions_main",
            "hxc_ModuleFunctions_static_field_base",
        ):
            if spelling not in header or spelling not in source:
                raise FunctionLoweringFailure(
                    f"module-field C omitted readable source name {spelling}"
                )
        visible_c = "".join(
            path.read_text(encoding="utf-8")
            for path in sorted(output.rglob("*"))
            if path.is_file() and path.suffix in (".c", ".h")
        )
        if "ModuleFunctions_Fields" in visible_c:
            raise FunctionLoweringFailure(
                "generated C leaked Haxe's hidden module-fields container"
            )

        oracle = subprocess.run(
            [development_tool("haxe"), "-cp", str(MODULE_FIELDS), "-main", "ModuleFunctions", "--interp"],
            cwd=ROOT,
            env={**os.environ, "HAXE_NO_SERVER": "1"},
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
        if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
            raise FunctionLoweringFailure("module-field Eval oracle did not exit silently")

        toolchain = available_compilers()[0]
        executable = root / "module-fields"
        native = subprocess.run(
            [
                toolchain.compiler,
                *STRICT_FLAGS,
                "-O2",
                "-I",
                str(output / "include"),
                *(str(path) for path in sorted((output / "src").rglob("*.c"))),
                "-o",
                str(executable),
            ],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if native.returncode != 0 or native.stdout or native.stderr:
            raise FunctionLoweringFailure(
                f"module-field generated C failed strict native compilation\n{native.stdout}{native.stderr}"
            )
        ran = subprocess.run(
            [str(executable)],
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            timeout=10,
        )
        if ran.returncode != 0 or ran.stdout or ran.stderr:
            raise FunctionLoweringFailure("module-field native executable did not match Eval")

        unsupported_output = root / "unsupported"
        unsupported = custom_target(
            MODULE_FIELDS_UNSUPPORTED,
            unsupported_output,
            main="UnsupportedModuleFunction",
        )
        unsupported_text = (unsupported.stdout + unsupported.stderr).replace("\\", "/")
        if (
            unsupported.returncode != 1
            or "HXC1001" not in unsupported_text
            or "TFunction(rest-argument:values)" not in unsupported_text
            or "module_fields_unsupported/UnsupportedModuleFunction.hx:2:" not in unsupported_text
            or list(unsupported_output.rglob("*"))
        ):
            raise FunctionLoweringFailure(
                "unsupported module function did not fail closed at its source declaration\n"
                f"stdout:\n{unsupported.stdout}stderr:\n{unsupported.stderr}"
            )


def check_recursive_partition(selected: str | None, layout: str) -> None:
    if layout == "split":
        expected_sources = (
            "src/hxc/main.c",
            "src/modules/RecursiveFixture.c",
            "src/modules/RecursiveFixture.nonreturn_0000.c",
        )
        expected_headers = (
            "include/hxc/detail/program_types.h",
            "include/hxc/modules/RecursiveFixture.h",
            "include/hxc/program.h",
        )
    elif layout == "package":
        expected_sources = (
            "src/hxc/main.c",
            "src/packages/package.c",
            "src/packages/package.nonreturn_0000.c",
        )
        expected_headers = (
            "include/hxc/detail/program_types.h",
            "include/hxc/packages/package.h",
            "include/hxc/program.h",
        )
    else:
        raise FunctionLoweringFailure(f"unsupported recursive partition layout {layout!r}")
    coverage = frozenset(
        {
            "closed-recursive-cycle",
            f"{layout}-nonreturn-partition",
            f"{layout}-project-layout",
            "strict-c11",
        }
    )
    with tempfile.TemporaryDirectory(prefix=f"hxc-function-{layout}-recursive-") as temporary:
        root = Path(temporary)
        output = root / "output"
        compiled = custom_target(
            SPLIT_RECURSIVE,
            output,
            layout=layout,
            main="RecursiveFixture",
        )
        if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
            raise FunctionLoweringFailure(
                f"{layout} recursive production compile failed\n"
                f"stdout:\n{compiled.stdout}stderr:\n{compiled.stderr}"
            )
        manifest = json.loads((output / "hxc.manifest.json").read_text())
        configuration = manifest.get("configuration")
        build = manifest.get("build")
        if (
            not isinstance(configuration, dict)
            or configuration.get("projectLayout") != layout
            or not isinstance(build, dict)
            or build.get("sources") != list(expected_sources)
            or build.get("privateHeaders") != list(expected_headers)
        ):
            raise FunctionLoweringFailure(
                f"{layout} recursive manifest lost its exact source/header partition"
            )
        recursive_source = (output / expected_sources[2]).read_text(encoding="utf-8")
        module_source = (output / expected_sources[1]).read_text(encoding="utf-8")
        if (
            "_Noreturn void hxc_RecursiveFixture_recursive(" not in recursive_source
            or "while (1)" not in recursive_source
            or "continue;" not in recursive_source
            or "hxc_RecursiveFixture_recursive(1, 2);" not in module_source
            or "_Noreturn void hxc_RecursiveFixture_recursive(" in module_source
        ):
            raise FunctionLoweringFailure(
                f"{layout} recursive function lost its isolated non-returning definition"
            )

        fixture = root / "fixture"
        shutil.copytree(output, fixture / "generated")
        (fixture / "native").mkdir(parents=True)
        for name in ("recursive_split_entry.h", "recursive_split_harness.c"):
            shutil.copy2(NATIVE / name, fixture / "native" / name)
        project = CFixtureProject(
            f"function-{layout}-recursive",
            (
                *(f"generated/{path}" for path in expected_sources),
                "native/recursive_split_harness.c",
            ),
            (
                *(f"generated/{path}" for path in expected_headers),
                "native/recursive_split_entry.h",
            ),
            ("generated/include", "native"),
            "",
            tuple(sorted(coverage)),
        )
        for optimization in ("-O0", "-O2"):
            report = run_c_fixture_corpus(
                suite=f"function-{layout}-recursive-{optimization[1:].lower()}",
                projects=(project,),
                fixture_root=fixture,
                build_root=root / f"build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=selected or "auto",
                strict_flags=(
                    *STRICT_FLAGS,
                    optimization,
                    "-Dmain=hxc_recursive_generated_main",
                    "-include",
                    "recursive_split_entry.h",
                ),
                timeout_seconds=60,
            )
            validate_report(report, required_coverage=coverage)
            serialized = json.dumps(report, ensure_ascii=False, sort_keys=True)
            if any(str(path) in serialized for path in (ROOT, root, fixture)):
                raise FunctionLoweringFailure(
                    f"{layout} recursive native report leaked an absolute path"
                )


def check_nonreturn_partitions(selected: str | None) -> None:
    for layout in ("split", "package"):
        check_recursive_partition(selected, layout)


def check_direct_argument_defaults(selected: str | None) -> None:
    """Prove omission is completed before HxcIR/C emission, never at runtime."""
    toolchain = available_compilers(selected)[0]
    with tempfile.TemporaryDirectory(prefix="hxc-function-defaults-") as temporary:
        root = Path(temporary)
        for name, fixture in (
            ("default", DEFAULT_ARGUMENT),
            ("optional", OPTIONAL_ARGUMENT),
        ):
            oracle = subprocess.run(
                [development_tool("haxe"), "-cp", str(fixture), "-main", "Main", "--interp"],
                cwd=ROOT,
                env={**os.environ, "HAXE_NO_SERVER": "1"},
                check=False,
                capture_output=True,
                text=True,
                timeout=10,
            )
            if oracle.returncode != 0 or oracle.stdout or oracle.stderr:
                raise FunctionLoweringFailure(
                    f"{name} argument Eval oracle did not exit silently\n"
                    f"stdout:\n{oracle.stdout}stderr:\n{oracle.stderr}"
                )

            outputs: dict[str, Path] = {}
            for layout in ("unity", "split", "package"):
                output = root / f"{name}-{layout}"
                compiled = custom_target(fixture, output, layout=layout)
                compile_messages = compiled.stdout + compiled.stderr
                expected_runtime_notice = name == "optional" and "HXC2001" in compile_messages and "[ERROR]" not in compile_messages
                if compiled.returncode != 0 or (compile_messages and not expected_runtime_notice):
                    raise FunctionLoweringFailure(
                        f"{name} argument {layout} compile failed\n"
                        f"stdout:\n{compiled.stdout}stderr:\n{compiled.stderr}"
                    )
                outputs[layout] = output

            repeated = root / f"{name}-unity-repeat"
            repeated_compile = custom_target(fixture, repeated, layout="unity")
            repeated_messages = repeated_compile.stdout + repeated_compile.stderr
            expected_repeated_notice = name == "optional" and "HXC2001" in repeated_messages and "[ERROR]" not in repeated_messages
            if repeated_compile.returncode != 0 or (repeated_messages and not expected_repeated_notice):
                raise FunctionLoweringFailure(f"repeated {name} argument compile failed")
            if generated_tree(outputs["unity"]) != generated_tree(repeated):
                raise FunctionLoweringFailure(
                    f"repeated {name} argument output was not byte-identical"
                )

            generated_c = "\n".join(
                path.read_text(encoding="utf-8")
                for path in sorted(outputs["split"].rglob("*.c"))
            )
            generated_headers = "\n".join(
                path.read_text(encoding="utf-8")
                for path in sorted((outputs["split"] / "include").rglob("*.h"))
            )
            if name == "default":
                if (
                    "hxc_Main_defaultValue(7)" not in generated_c
                    or "hxc_Main_defaultValue(9)" not in generated_c
                    or "hxc_DefaultSupport_genericIdentity(8, 3)" not in generated_c
                ):
                    raise FunctionLoweringFailure(
                        "default argument was not materialized as a full-arity typed C call"
                    )
            elif (
                generated_c.count("hxc_Main_optionalValue((struct hxc_optional_Main_Choice){ .hxc_has_value = false })") < 2
                or ".hxc_has_value = true" not in generated_c
                or ".hxc_value = false" not in generated_c
                or ".hxc_value = 0" not in generated_c
                or ".hxc_value = 0.0" not in generated_c
                or "bool hxc_value;" not in generated_headers
                or "int32_t hxc_value;" not in generated_headers
                or "uint32_t hxc_value;" not in generated_headers
                or "double hxc_value;" not in generated_headers
                or 'hxc_Main_acceptText((hxc_string){ (const uint8_t *)"haxe", 4, true, NULL })' not in generated_c
            ):
                raise FunctionLoweringFailure(
                    "optional primitive/enum omission, false/zero, or presence lost its distinct tagged C value"
                )

            runtime_sources = planned_runtime_sources(outputs["split"])
            for optimization in ("-O0", "-O2"):
                executable = root / f"{name}-{toolchain.family}-{optimization[1:]}"
                native = subprocess.run(
                    [
                        toolchain.compiler,
                        *STRICT_FLAGS,
                        optimization,
                        "-I",
                        str(outputs["split"] / "include"),
                        "-I",
                        str(ROOT / "runtime/hxrt/include"),
                        *(str(path) for path in sorted((outputs["split"] / "src").rglob("*.c"))),
                        *(str(path) for path in runtime_sources),
                        "-o",
                        str(executable),
                    ],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    text=True,
                    timeout=30,
                )
                if native.returncode != 0 or native.stdout or native.stderr:
                    raise FunctionLoweringFailure(
                        f"{name} {optimization} generated C failed strict compilation\n"
                        f"stdout:\n{native.stdout}stderr:\n{native.stderr}"
                    )
                ran = subprocess.run(
                    [str(executable)],
                    cwd=ROOT,
                    check=False,
                    capture_output=True,
                    text=True,
                    timeout=10,
                )
                if ran.returncode != 0 or ran.stdout or ran.stderr:
                    raise FunctionLoweringFailure(
                        f"{name} {optimization} native behavior differed from Eval"
                    )

            sanitized = root / f"{name}-{toolchain.family}-sanitized"
            sanitizer_flags = (
                "-O1",
                "-fno-omit-frame-pointer",
                "-fsanitize=address,undefined",
            )
            sanitizer_compile = subprocess.run(
                [
                    toolchain.compiler,
                    *STRICT_FLAGS,
                    *sanitizer_flags,
                    "-I",
                    str(outputs["split"] / "include"),
                    "-I",
                    str(ROOT / "runtime/hxrt/include"),
                    *(str(path) for path in sorted((outputs["split"] / "src").rglob("*.c"))),
                    *(str(path) for path in runtime_sources),
                    "-o",
                    str(sanitized),
                ],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                timeout=30,
            )
            if sanitizer_compile.returncode != 0 or sanitizer_compile.stdout or sanitizer_compile.stderr:
                raise FunctionLoweringFailure(
                    f"{name} generated C failed the address/undefined-behavior sanitizer build\n"
                    f"stdout:\n{sanitizer_compile.stdout}stderr:\n{sanitizer_compile.stderr}"
                )
            sanitized_run = subprocess.run(
                [str(sanitized)],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                timeout=10,
            )
            if sanitized_run.returncode != 0 or sanitized_run.stdout or sanitized_run.stderr:
                raise FunctionLoweringFailure(
                    f"{name} generated C failed under address/undefined-behavior sanitizers"
                )


def wait_for_compiler_server(server: subprocess.Popen[str], endpoint: tuple[str, int]) -> None:
    """Wait for the owned local server without hiding an early process failure."""
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise FunctionLoweringFailure(
                "Haxe compiler server exited before accepting default-argument requests\n"
                f"stdout:\n{stdout}stderr:\n{stderr}"
            )
        try:
            with socket.create_connection(endpoint, timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise FunctionLoweringFailure("Haxe compiler server did not accept connections within 10 seconds")


def check_default_argument_server_determinism() -> None:
    """Prove repeated warm requests retain declaration defaults and clean state."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as reservation:
        reservation.bind(("127.0.0.1", 0))
        port = reservation.getsockname()[1]
    endpoint = str(port)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", endpoint],
        cwd=ROOT,
        env={key: value for key, value in os.environ.items() if key != "HAXE_NO_SERVER"},
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_compiler_server(server, ("127.0.0.1", port))
        with tempfile.TemporaryDirectory(prefix="hxc-function-defaults-server-") as temporary:
            root = Path(temporary)
            for name, fixture in (
                ("default", DEFAULT_ARGUMENT),
                ("optional", OPTIONAL_ARGUMENT),
            ):
                first = root / f"{name}-first"
                second = root / f"{name}-second"
                for label, output in (("first", first), ("second", second)):
                    compiled = custom_target(fixture, output, connect=endpoint)
                    messages = compiled.stdout + compiled.stderr
                    expected_notice = name == "optional" and "HXC2001" in messages and "[ERROR]" not in messages
                    if compiled.returncode != 0 or (messages and not expected_notice):
                        raise FunctionLoweringFailure(
                            f"{name} {label} compiler-server request failed\n"
                            f"stdout:\n{compiled.stdout}stderr:\n{compiled.stderr}"
                        )
                if generated_tree(first) != generated_tree(second):
                    raise FunctionLoweringFailure(
                        f"{name} optional/default output changed on a warm compiler server"
                    )
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def check_argument_diagnostics() -> None:
    expected = {"rest": "TFunction(rest-argument:values)"}
    with tempfile.TemporaryDirectory(prefix="hxc-function-negative-") as temporary:
        root = Path(temporary)
        for kind, marker in expected.items():
            output = root / kind
            result = custom_target(FIXTURES / kind, output)
            combined = result.stdout + result.stderr
            if (
                result.returncode != 1
                or "HXC1001" not in combined
                or marker not in combined
                or f"fixtures/{kind}/Main.hx:2:" not in combined.replace("\\", "/")
                or list(output.rglob("*"))
            ):
                raise FunctionLoweringFailure(
                    f"{kind} argument diagnostic was not exact and output-free\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )


def snapshot_native_report() -> dict[str, object]:
    return {
        "header": expected_text("functions.h"),
        "sources": [
            {
                "path": source_path,
                "content": expected_text(snapshot_name),
            }
            for snapshot_name, source_path in SOURCE_SNAPSHOTS.items()
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
        print("function-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            check_native(snapshot_native_report(), args.toolchain)
            print("function-lowering: OK: required static-function native matrix passed")
            return 0
        first_payload, first = render("first function render")
        second_payload, second = render("second function render")
        reverse_payload, reverse = render("reverse function render", reverse=True)
        metal_payload, metal = render("metal function render", profile="metal")
        if first_payload != second_payload or first != second:
            raise FunctionLoweringFailure("two function renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise FunctionLoweringFailure("function render changed with discovery order")
        validate(first)
        validate(metal, profile="metal")
        portable_normalized = {**first, "profile": "<profile>"}
        metal_normalized = {**metal, "profile": "<profile>"}
        if portable_normalized != metal_normalized:
            raise FunctionLoweringFailure("portable and metal function lowering diverged")
        check_snapshots(first)
        check_native(first, args.toolchain)
        check_production()
        check_module_fields()
        check_nonreturn_partitions(args.toolchain)
        check_direct_argument_defaults(args.toolchain)
        check_default_argument_server_determinism()
        check_argument_diagnostics()
    except (
        CFixtureFailure,
        FunctionLoweringFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"function-lowering: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "function-lowering: OK: typed parameters/calls/conversions, recursive private "
        "prototypes/unity+split+package source partitions, readable module-level functions, direct optional/default completion, exact rest diagnostics, strict int main(void), "
        "and zero-runtime production artifacts passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
