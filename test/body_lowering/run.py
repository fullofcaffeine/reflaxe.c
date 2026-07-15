#!/usr/bin/env python3
"""Prove real typed-Haxe body lowering, exact failures, and strict native C."""

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
HXML = Path(__file__).with_name("body_lowering.hxml")
POSITIVE = Path(__file__).with_name("fixtures") / "positive"
UNSUPPORTED = Path(__file__).with_name("fixtures") / "unsupported"
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_BODY_LOWERING="
FIELDS = {
    "booleanValue",
    "directInteger",
    "explicitVoid",
    "floatingValue",
    "implicitVoid",
    "integerValue",
    "unsignedValue",
}
IDENTIFIER = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")


class BodyLoweringFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class BodyToolchain:
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
        command.extend(["-D", "body_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "body_lowering_profile=metal"])
    elif profile != "portable":
        raise BodyLoweringFailure(f"unknown body-lowering profile {profile!r}")
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
        raise BodyLoweringFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise BodyLoweringFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise BodyLoweringFailure(f"{label} report is not a JSON object")
    return payload, report


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status")
        != "typed-haxe-body-lowering-no-calls-or-entry-point-emission"
        or report.get("profile") != profile
    ):
        raise BodyLoweringFailure("body-lowering report schema/status/profile drifted")
    for key in ("hxcir", "cSource", "lineMappedCSource"):
        value = report.get(key)
        if not isinstance(value, str) or not value:
            raise BodyLoweringFailure(f"body-lowering report omitted {key}")
        if str(ROOT) in value or "\\" in value:
            raise BodyLoweringFailure(f"{key} leaked a checkout-local path")
        if "hxrt" in value.lower():
            raise BodyLoweringFailure(f"{key} selected an implicit runtime dependency")
    if report.get("runtimeFeatures") != [] or report.get("runtimeArtifacts") != []:
        raise BodyLoweringFailure("primitive body lowering selected runtime work")

    functions = report.get("functions")
    if not isinstance(functions, list):
        raise BodyLoweringFailure("body-lowering report omitted function records")
    by_field: dict[str, dict[str, object]] = {}
    all_names: set[str] = set()
    for entry in functions:
        if not isinstance(entry, dict) or not isinstance(entry.get("field"), str):
            raise BodyLoweringFailure(f"invalid function record: {entry!r}")
        field = entry["field"]
        c_name = entry.get("cName")
        if (
            field in by_field
            or not isinstance(c_name, str)
            or not IDENTIFIER.fullmatch(c_name)
            or not c_name.startswith("hxc_method_")
        ):
            raise BodyLoweringFailure(f"invalid finalized function name: {entry!r}")
        by_field[field] = entry
        all_names.add(c_name)
        locals_value = entry.get("locals")
        if not isinstance(locals_value, list):
            raise BodyLoweringFailure(f"function {field} omitted local names")
        for local in locals_value:
            if (
                not isinstance(local, dict)
                or not isinstance(local.get("irId"), str)
                or not isinstance(local.get("cName"), str)
                or not IDENTIFIER.fullmatch(local["cName"])
                or not local["cName"].startswith("hxc_local_")
                or local["cName"] in all_names
            ):
                raise BodyLoweringFailure(f"invalid finalized local name: {local!r}")
            all_names.add(local["cName"])
    if set(by_field) != FIELDS:
        raise BodyLoweringFailure(
            f"lowered fields drifted: expected {sorted(FIELDS)!r}, got {sorted(by_field)!r}"
        )
    shadowed = by_field["integerValue"].get("locals")
    if (
        not isinstance(shadowed, list)
        or len(shadowed) != 2
        or shadowed[0].get("cName") == shadowed[1].get("cName")
    ):
        raise BodyLoweringFailure("shadowed Haxe locals did not receive distinct C names")

    hxcir = required_text(report, "hxcir")
    ordered = (
        'instruction "instruction.0.constant"',
        'instruction "instruction.1.initialize"',
        'instruction "instruction.2.load"',
        'instruction "instruction.3.constant"',
        'instruction "instruction.4.initialize"',
        'instruction "instruction.5.load"',
        "terminator return",
    )
    integer_start = hxcir.find('function "function.BodyFixture.integerValue"')
    integer_end = hxcir.find('end function "function.BodyFixture.integerValue"')
    integer_body = hxcir[integer_start:integer_end]
    positions = [integer_body.find(marker) for marker in ordered]
    if integer_start == -1 or integer_end == -1 or -1 in positions or positions != sorted(positions):
        raise BodyLoweringFailure("HxcIR lost source evaluation order for nested shadowing")
    for source_marker in (
        'BodyFixture.hx":3:3-3:22',
        'BodyFixture.hx":6:4-6:23',
        'BodyFixture.hx":7:4-7:16',
    ):
        if source_marker not in hxcir:
            raise BodyLoweringFailure(f"HxcIR lost exact source span {source_marker}")

    c_source = required_text(report, "cSource")
    mapped = required_text(report, "lineMappedCSource")
    if "#line" in c_source or '#line 3 "test/body_lowering/' not in mapped:
        raise BodyLoweringFailure("optional structured #line mode drifted")
    if "#include <stdint.h>" not in c_source or "#include <stdbool.h>" not in c_source:
        raise BodyLoweringFailure("primitive body C omitted its exact standard headers")

    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
        raise BodyLoweringFailure("body lowering omitted the finalized symbol table")
    symbol_entries = symbols.get("symbols")
    if not isinstance(symbol_entries, list):
        raise BodyLoweringFailure("body symbol table omitted symbols")
    shadow_ordinals = sorted(
        entry.get("sourceOrdinal")
        for entry in symbol_entries
        if isinstance(entry, dict)
        and isinstance(entry.get("sourceSymbol"), str)
        and entry["sourceSymbol"].startswith("BodyFixture.integerValue.value#")
    )
    if shadow_ordinals != [0, 1]:
        raise BodyLoweringFailure("shadowed-local symbol provenance lost lexical ordinals")


def expected_text(name: str) -> str:
    return (EXPECTED / name).read_text(encoding="utf-8")


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise BodyLoweringFailure(f"body-lowering report field {key} is not text")
    return value


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
    pairs = {
        "body.hxcir": report.get("hxcir"),
        "body.c": report.get("cSource"),
        "body-lines.c": report.get("lineMappedCSource"),
    }
    for name, actual in pairs.items():
        if not isinstance(actual, str):
            raise BodyLoweringFailure(f"snapshot source {name} is not text")
        expected = expected_text(name)
        if actual != expected:
            raise BodyLoweringFailure(f"{name} drifted:\n" + difference(expected, actual, name))
    expected_symbols = json.loads(expected_text("symbols.json"))
    if report.get("symbols") != expected_symbols:
        raise BodyLoweringFailure(
            "body symbol snapshot drifted\nexpected:\n"
            + json.dumps(expected_symbols, indent=2, sort_keys=True)
            + "\nactual:\n"
            + json.dumps(report.get("symbols"), indent=2, sort_keys=True)
        )


def function_names(report: dict[str, object]) -> dict[str, str]:
    functions = report.get("functions")
    if not isinstance(functions, list):
        raise BodyLoweringFailure("body-lowering native report omitted functions")
    names = {
        entry["field"]: entry["cName"]
        for entry in functions
        if isinstance(entry, dict)
        and isinstance(entry.get("field"), str)
        and isinstance(entry.get("cName"), str)
    }
    if set(names) != FIELDS:
        raise BodyLoweringFailure(
            f"body-lowering native report has fields {sorted(names)!r}, "
            f"expected {sorted(FIELDS)!r}"
        )
    return names


def native_harness(report: dict[str, object]) -> str:
    names = function_names(report)
    return f"""
int main(void)
{{
  {names['explicitVoid']}();
  {names['implicitVoid']}();
  if ({names['integerValue']}() != INT32_C(42)) return 1;
  if ({names['directInteger']}() != INT32_C(23)) return 2;
  if ({names['unsignedValue']}() != UINT32_C(29)) return 3;
  if ({names['floatingValue']}() != 1.5) return 4;
  if ({names['booleanValue']}() != true) return 5;
  return 0;
}}
"""


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
        raise BodyLoweringFailure(
            f"cannot identify body-lowering compiler {executable}: exit {result.returncode}"
        )
    combined = (result.stdout + result.stderr).strip()
    first_line = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first_line
    if "free software foundation" in lowered or "gcc" in lowered:
        return "gcc", first_line
    return "unknown", first_line


def resolve_compiler(family: str, *, required: bool) -> BodyToolchain | None:
    executable = shutil.which(family)
    qualifier = "required" if required else "optional"
    if executable is None:
        if required:
            raise BodyLoweringFailure(
                f"required body-lowering toolchain {family}: missing {family}"
            )
        print(f"body-lowering: SKIP {qualifier} {family}: missing {family}")
        return None
    actual_family, version = compiler_identity(executable)
    if actual_family != family:
        message = f"{family}: command identity mismatch ({family}={actual_family})"
        if required:
            raise BodyLoweringFailure(f"required body-lowering toolchain {message}")
        print(f"body-lowering: SKIP {qualifier} {message}")
        return None
    print(
        f"body-lowering: {'REQUIRED' if required else 'AVAILABLE'} "
        f"{family}: cc={version}"
    )
    return BodyToolchain(family, executable, version)


def available_compilers(selected: str | None = None) -> list[BodyToolchain]:
    requested = [selected] if selected is not None else ["gcc", "clang"]
    result = [
        toolchain
        for family in requested
        if (toolchain := resolve_compiler(family, required=selected is not None))
        is not None
    ]
    if not result:
        raise BodyLoweringFailure(
            "no strict C11 compiler is available for generated-body evidence"
        )
    return result


def check_native(report: dict[str, object], selected: str | None = None) -> None:
    compilers = available_compilers(selected)
    harness = native_harness(report)
    with tempfile.TemporaryDirectory(prefix="hxc-body-lowering-native-") as temporary:
        root = Path(temporary)
        for source_key in ("cSource", "lineMappedCSource"):
            source = report.get(source_key)
            if not isinstance(source, str):
                raise BodyLoweringFailure(
                    f"body-lowering native source {source_key} is not text"
                )
            c_file = root / f"{source_key}.c"
            c_file.write_text(source + harness, encoding="utf-8", newline="\n")
            for toolchain in compilers:
                for optimization in ("-O0", "-O2"):
                    executable = root / f"{source_key}-{toolchain.family}-{optimization[1:]}"
                    compile_result = subprocess.run(
                        [
                            toolchain.compiler,
                            "-std=c11",
                            "-Wall",
                            "-Wextra",
                            "-Werror",
                            "-Wconversion",
                            "-Wsign-conversion",
                            "-pedantic-errors",
                            optimization,
                            str(c_file),
                            "-o",
                            str(executable),
                        ],
                        cwd=ROOT,
                        check=False,
                        capture_output=True,
                        text=True,
                        timeout=30,
                    )
                    if compile_result.returncode != 0 or compile_result.stdout or compile_result.stderr:
                        raise BodyLoweringFailure(
                            f"{toolchain.family} {optimization} rejected {source_key}\n"
                            f"stdout:\n{compile_result.stdout}\nstderr:\n{compile_result.stderr}"
                        )
                    run_result = subprocess.run(
                        [str(executable)],
                        cwd=ROOT,
                        check=False,
                        capture_output=True,
                        text=True,
                        timeout=10,
                    )
                    if run_result.returncode != 0 or run_result.stdout or run_result.stderr:
                        raise BodyLoweringFailure(
                            f"{toolchain.family} {optimization} {source_key} behavior failed: "
                            f"exit={run_result.returncode} stdout={run_result.stdout!r} stderr={run_result.stderr!r}"
                        )


def custom_target(
    fixture: Path, main_class: str, output: Path, *, profile: str = "portable"
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        main_class,
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
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


def assert_no_output(root: Path, label: str) -> None:
    files = [path for path in root.rglob("*") if path.is_file()]
    if files:
        raise BodyLoweringFailure(
            f"{label} emitted files despite its diagnostic: "
            + ", ".join(path.relative_to(root).as_posix() for path in files)
        )


def check_production_boundaries() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-body-lowering-boundary-") as temporary:
        root = Path(temporary)
        for profile in ("portable", "metal"):
            unsupported_output = root / f"unsupported-{profile}"
            unsupported = custom_target(
                UNSUPPORTED, "Main", unsupported_output, profile=profile
            )
            combined = unsupported.stdout + unsupported.stderr
            if (
                unsupported.returncode != 1
                or "HXC1001" not in combined
                or "TUnop(OpIncrement,false)" not in combined
                or "Main.hx:4: characters 3-10" not in combined
                or f"[profile={profile}]" not in combined
            ):
                raise BodyLoweringFailure(
                    f"{profile} unsupported expression lost its exact HXC1001 position\n"
                    f"stdout:\n{unsupported.stdout}\nstderr:\n{unsupported.stderr}"
                )
            assert_no_output(unsupported_output, f"{profile} HXC1001")

        supported_output = root / "supported"
        supported = custom_target(POSITIVE, "BodyFixture", supported_output)
        if supported.returncode != 0 or supported.stdout or supported.stderr:
            raise BodyLoweringFailure(
                "supported body did not emit the E2.T03 runtime-free executable project\n"
                f"stdout:\n{supported.stdout}\nstderr:\n{supported.stderr}"
            )
        required = {
            "_GeneratedFiles.json",
            "hxc.abi.json",
            "hxc.manifest.json",
            "hxc.runtime-plan.json",
            "hxc.stdlib-report.json",
            "hxc.symbols.json",
            "include/hxc/program.h",
            "src/program.c",
        }
        actual = {
            path.relative_to(supported_output).as_posix()
            for path in supported_output.rglob("*")
            if path.is_file()
        }
        if actual != required:
            raise BodyLoweringFailure(
                f"supported production artifact set drifted: {sorted(actual)!r}"
            )
        header = (supported_output / "include/hxc/program.h").read_text(
            encoding="utf-8"
        )
        source = (supported_output / "src/program.c").read_text(encoding="utf-8")
        runtime_plan = json.loads(
            (supported_output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
        )
        abi = json.loads(
            (supported_output / "hxc.abi.json").read_text(encoding="utf-8")
        )
        stdlib = json.loads(
            (supported_output / "hxc.stdlib-report.json").read_text(encoding="utf-8")
        )
        if (
            "void hxc_method_BodyFixture_main(void);" not in header
            or '#include "hxc/program.h"' not in source
            or "int main(void)" not in source
            or "hxc_method_BodyFixture_main();" not in source
            or runtime_plan.get("status") != "analyzed-runtime-free"
            or runtime_plan.get("features") != []
            or not runtime_plan.get("noRuntimeProof")
            or abi.get("status") != "analyzed-no-public-exports"
            or abi.get("executableEntryPoint") != "main"
            or stdlib.get("status") != "analyzed-no-stdlib-use"
        ):
            raise BodyLoweringFailure(
                "supported production project lost its entry/runtime/ABI/stdlib proof"
            )
        combined = "\n".join(
            path.read_text(encoding="utf-8")
            for path in supported_output.rglob("*")
            if path.is_file() and path.name != "_GeneratedFiles.json"
        ).lower()
        if "hxrt" in combined:
            raise BodyLoweringFailure("supported primitive project selected hxrt")

        compiler = available_compilers()[0]
        executable = root / "supported-program"
        compiled = subprocess.run(
            [
                compiler.compiler,
                "-std=c11",
                "-Wall",
                "-Wextra",
                "-Werror",
                "-Wconversion",
                "-Wsign-conversion",
                "-pedantic-errors",
                "-I",
                str(supported_output / "include"),
                str(supported_output / "src/program.c"),
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
            raise BodyLoweringFailure(
                "strict compiler rejected supported production project\n"
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
            raise BodyLoweringFailure(
                "supported production executable did not exit silently with zero"
            )


def snapshot_native_report() -> dict[str, object]:
    symbols = json.loads(expected_text("symbols.json"))
    if not isinstance(symbols, dict) or not isinstance(symbols.get("symbols"), list):
        raise BodyLoweringFailure("body symbol snapshot cannot drive the native matrix")
    functions = []
    for entry in symbols["symbols"]:
        if (
            isinstance(entry, dict)
            and entry.get("kind") == "method"
            and isinstance(entry.get("sourceSymbol"), str)
            and isinstance(entry.get("cName"), str)
        ):
            functions.append(
                {
                    "field": entry["sourceSymbol"].rsplit(".", 1)[-1],
                    "cName": entry["cName"],
                }
            )
    return {
        "cSource": expected_text("body.c"),
        "lineMappedCSource": expected_text("body-lines.c"),
        "functions": functions,
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("body-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            check_native(snapshot_native_report(), args.toolchain)
            print("body-lowering: OK: required generated-body native matrix passed")
            return 0
        first_payload, first = render("first body-lowering render")
        second_payload, second = render("second body-lowering render")
        reverse_payload, reverse = render(
            "reverse-input body-lowering render", reverse=True
        )
        metal_payload, metal = render("metal body-lowering render", profile="metal")
        if first_payload != second_payload or first != second:
            raise BodyLoweringFailure("two body-lowering renders were not byte-identical")
        if first_payload != reverse_payload or first != reverse:
            raise BodyLoweringFailure("body lowering changed with input discovery order")
        validate(first)
        validate(metal, profile="metal")
        portable_without_profile = {**first, "profile": "<profile>"}
        metal_without_profile = {**metal, "profile": "<profile>"}
        if portable_without_profile != metal_without_profile:
            raise BodyLoweringFailure("portable and metal primitive body lowering diverged")
        check_snapshots(first)
        check_native(first, args.toolchain)
        check_production_boundaries()
    except (
        BodyLoweringFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"body-lowering: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "body-lowering: OK: typed constants/locals/blocks/returns, shadow-safe names, "
        "exact HXC1001 spans, optional #line mapping, runtime-free production and "
        "GCC/Clang behavior"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
