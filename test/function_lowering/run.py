#!/usr/bin/env python3
"""Prove typed static functions, direct calls, recursive prototypes, and production entry emission."""

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
HXML = Path(__file__).with_name("function_lowering.hxml")
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_FUNCTION_LOWERING="
SNAPSHOTS = {
    "functions.hxcir": "hxcir",
    "functions.h": "header",
    "functions.c": "source",
    "symbols.json": "symbols",
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


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "typed-static-functions-direct-calls-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
        or report.get("runtimeArtifacts") != []
    ):
        raise FunctionLoweringFailure("function-lowering schema/status/profile drifted")
    for key in ("hxcir", "header", "source"):
        value = required_text(report, key)
        if str(ROOT) in value or "\\" in value or "hxrt" in value.lower():
            raise FunctionLoweringFailure(f"{key} leaked a host path or runtime dependency")

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
        'direct("function.FunctionFixture.recursiveStep")',
        'direct("function.FunctionFixture.recursive")',
        'direct("function.FunctionFixture.mutualLeft")',
        'direct("function.FunctionFixture.mutualRight")',
    ):
        if marker not in hxcir:
            raise FunctionLoweringFailure(f"recursive call graph omitted {marker}")

    header = required_text(report, "header")
    source = required_text(report, "source")
    if (
        not header.startswith("#ifndef HXC_GENERATED_PATH_")
        or "_Noreturn void hxc_method_FunctionFixture_recursive(" not in header
        or "_Noreturn void hxc_method_FunctionFixture_recursiveStep(" not in header
        or "_Noreturn void hxc_method_FunctionFixture_mutualLeft(" not in header
        or "_Noreturn void hxc_method_FunctionFixture_mutualRight(" not in header
        or '#include "hxc/program.h"' not in source
        or "int main(void)" not in source
        or "hxc_method_FunctionFixture_main();" not in source
        or "return 0;" not in source
    ):
        raise FunctionLoweringFailure("prototype plan or executable entry shape drifted")
    for field in ("recursive", "recursiveStep", "mutualLeft", "mutualRight"):
        definition_start = source.find(
            f"_Noreturn void hxc_method_FunctionFixture_{field}("
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
    left_prototype = header.find("hxc_method_FunctionFixture_mutualLeft")
    right_prototype = header.find("hxc_method_FunctionFixture_mutualRight")
    left_definition = source.find("hxc_method_FunctionFixture_mutualLeft")
    right_definition = source.find("hxc_method_FunctionFixture_mutualRight")
    if min(left_prototype, right_prototype, left_definition, right_definition) < 0:
        raise FunctionLoweringFailure("recursive prototype/definition evidence is incomplete")
    ordered_source_start = source.find("int32_t hxc_method_FunctionFixture_ordered(")
    ordered_source_end = source.find("\n}\n", ordered_source_start)
    ordered_source = source[ordered_source_start:ordered_source_end]
    ordered_c_calls = [
        ordered_source.find(f"hxc_method_FunctionFixture_{target}(")
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
    if not isinstance(functions, list) or len(functions) != 12:
        raise FunctionLoweringFailure("function report omitted admitted functions")
    by_field = {
        entry.get("field"): entry
        for entry in functions
        if isinstance(entry, dict) and isinstance(entry.get("field"), str)
    }
    if (
        len(by_field) != 12
        or by_field.get("main", {}).get("parameters") != []
        or len(by_field.get("first", {}).get("parameters", [])) != 2
    ):
        raise FunctionLoweringFailure("function parameter records drifted")
    if not by_field.get("convert", {}).get("temporaries"):
        raise FunctionLoweringFailure("direct call result was not materialized as a C temporary")

    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
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
    return {
        "functions.hxcir": required_text(report, "hxcir"),
        "functions.h": required_text(report, "header"),
        "functions.c": required_text(report, "source"),
        "symbols.json": report.get("symbols"),
    }


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


def write_native_project(report: dict[str, object], root: Path) -> None:
    header = root / "include/hxc/program.h"
    source = root / "src/program.c"
    header.parent.mkdir(parents=True)
    source.parent.mkdir(parents=True)
    header.write_text(required_text(report, "header"), encoding="utf-8", newline="\n")
    source.write_text(required_text(report, "source"), encoding="utf-8", newline="\n")


def check_native(
    report: dict[str, object], selected: str | None = None
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-function-native-") as temporary:
        root = Path(temporary)
        write_native_project(report, root)
        header = root / "include/hxc/program.h"
        source = root / "src/program.c"
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
                        str(source),
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
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main" if fixture != POSITIVE else "FunctionFixture",
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if environment is not None:
        command.extend(["-D", f"hxc_environment={environment}"])
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
        compiled = subprocess.run(
            [
                compiler.compiler,
                *STRICT_FLAGS,
                "-I",
                str(first / "include"),
                str(first / "src/program.c"),
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


def check_argument_diagnostics() -> None:
    expected = {
        "default": "TFunction(default-argument:value)",
        "optional": "TFunction(optional-argument:value)",
        "rest": "TFunction(rest-argument:values)",
    }
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
        "source": expected_text("functions.c"),
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
        check_argument_diagnostics()
    except (
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
        "prototypes, exact argument diagnostics, strict int main(void), and zero-runtime "
        "production artifacts passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
