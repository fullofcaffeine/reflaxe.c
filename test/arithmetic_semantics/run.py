#!/usr/bin/env python3
"""Prove typed UB-safe arithmetic in HxcIR, generated C, sanitizers, and optimized code."""

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
HXML = Path(__file__).with_name("arithmetic_semantics.hxml")
ORACLE_HXML = Path(__file__).with_name("oracle.hxml")
FIXTURE = Path(__file__).with_name("fixtures")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_ARITHMETIC_SEMANTICS="
EXPECTED_ORACLE = (
    "-2147483648,2147483647,-2,-2147483648,2147483648,0,-2147483648,-1,1,"
    "85,95,90,-1,-1,3,0,2147483647,-2147483648,1,18\n"
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
SANITIZER_FLAGS = (
    "-fsanitize=undefined,float-divide-by-zero",
    "-fno-sanitize-recover=all",
)
EXPECTED_HELPERS = [
    "hxc.f64.divide.zero-safe",
    "hxc.f64.modulo",
    "hxc.f64.to.i32.saturating",
    "hxc.i32.add.wrapping",
    "hxc.i32.bit-and",
    "hxc.i32.bit-not",
    "hxc.i32.bit-or",
    "hxc.i32.bit-xor",
    "hxc.i32.modulo.zero-safe",
    "hxc.i32.multiply.wrapping",
    "hxc.i32.negate.wrapping",
    "hxc.i32.shift-left.masked",
    "hxc.i32.shift-right.masked",
    "hxc.i32.subtract.wrapping",
    "hxc.i32.unsigned-shift-right.masked",
    "hxc.u32.modulo.zero-safe",
    "hxc.u32.to.i32.bits",
]


class ArithmeticSemanticsFailure(RuntimeError):
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
        command.extend(["-D", "arithmetic_semantics_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "arithmetic_semantics_profile=metal"])
    elif profile != "portable":
        raise ArithmeticSemanticsFailure(f"unknown arithmetic profile {profile!r}")
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
        raise ArithmeticSemanticsFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise ArithmeticSemanticsFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise ArithmeticSemanticsFailure(f"{label} report is not an object")
    return payload, report


def required_text(report: dict[str, object], key: str) -> str:
    value = report.get(key)
    if not isinstance(value, str):
        raise ArithmeticSemanticsFailure(f"report field {key!r} must be text")
    return value


def source_records(report: dict[str, object]) -> dict[str, str]:
    value = report.get("sources")
    if not isinstance(value, list):
        raise ArithmeticSemanticsFailure("report sources must be an array")
    records: dict[str, str] = {}
    ordered: list[str] = []
    for item in value:
        if (
            not isinstance(item, dict)
            or not isinstance(item.get("path"), str)
            or not isinstance(item.get("content"), str)
        ):
            raise ArithmeticSemanticsFailure(f"invalid source record: {item!r}")
        path = item["path"]
        if path in records or path.startswith("/") or "\\" in path:
            raise ArithmeticSemanticsFailure(f"invalid source path {path!r}")
        records[path] = item["content"]
        ordered.append(path)
    if ordered != sorted(ordered):
        raise ArithmeticSemanticsFailure("source records are not path sorted")
    return records


def symbol_entries(symbols: dict[str, object]) -> list[dict[str, object]]:
    values = symbols.get("symbols")
    if not isinstance(values, list) or not all(isinstance(item, dict) for item in values):
        raise ArithmeticSemanticsFailure("symbol table entries are missing")
    return values


def function_c_name(symbols: dict[str, object], field: str) -> str:
    prefix = f"ArithmeticFixture.{field}"
    matches = [
        item.get("cName")
        for item in symbol_entries(symbols)
        if item.get("kind") == "method"
        and isinstance(item.get("sourceSymbol"), str)
        and (
            item["sourceSymbol"] == prefix
            or item["sourceSymbol"].startswith(prefix + "(")
        )
    ]
    if len(matches) != 1 or not isinstance(matches[0], str):
        raise ArithmeticSemanticsFailure(f"cannot resolve unique C name for {prefix}")
    return matches[0]


def function_body(source: str, name: str) -> str:
    match = re.search(rf"(?:bool|double|int32_t|uint32_t|void) {re.escape(name)}\([^\n]*\)\n\{{", source)
    if match is None:
        raise ArithmeticSemanticsFailure(f"generated definition for {name} is missing")
    start = match.start()
    end = source.find("\n}\n", match.end())
    if end == -1:
        raise ArithmeticSemanticsFailure(f"generated definition for {name} is unterminated")
    return source[start : end + 3]


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "typed-ub-safe-arithmetic-runtime-free"
        or report.get("profile") != profile
        or report.get("runtimeFeatures") != []
    ):
        raise ArithmeticSemanticsFailure("arithmetic schema/status/profile drifted")
    if report.get("helpers") != EXPECTED_HELPERS:
        raise ArithmeticSemanticsFailure("selected primitive helper closure drifted")
    if report.get("buildFacts") != [
        {
            "kind": "link",
            "name": "m",
            "value": None,
            "valueKind": None,
            "ownerModulePaths": ["ArithmeticFixture"],
        }
    ]:
        raise ArithmeticSemanticsFailure("compiler-selected C math build fact drifted")

    hxcir = required_text(report, "hxcir")
    header = required_text(report, "header")
    sources = source_records(report)
    if set(sources) != {"src/program.c"}:
        raise ArithmeticSemanticsFailure(f"unexpected source partition: {sorted(sources)!r}")
    source = sources["src/program.c"]
    for artifact in (hxcir, header, source):
        if str(ROOT) in artifact or "\\" in artifact or "hxrt" in artifact.lower():
            raise ArithmeticSemanticsFailure("artifact leaked a host path or runtime dependency")

    required_ir = (
        ("haxe.i32.add", 'program-local("hxc.i32.add.wrapping")'),
        ("haxe.i32.modulo", 'program-local("hxc.i32.modulo.zero-safe")'),
        ("haxe.i32.shift-left.masked", 'program-local("hxc.i32.shift-left.masked")'),
        ("haxe.i32.shift-right.masked", 'program-local("hxc.i32.shift-right.masked")'),
        ("haxe.i32.unsigned-shift-right.masked", 'program-local("hxc.i32.unsigned-shift-right.masked")'),
        ("haxe.f64.divide", 'program-local("hxc.f64.divide.zero-safe")'),
        ("haxe.f64.modulo", 'program-local("hxc.f64.modulo")'),
        ("haxe.u32.add", "static"),
        ("haxe.u32.shift-left.masked", "static"),
        ("haxe.u32.unsigned-shift-right.masked", "static"),
    )
    for operation, implementation in required_ir:
        marker = rf'operation="{re.escape(operation)}"[^\n]+implementation={re.escape(implementation)}'
        if re.search(marker, hxcir) is None:
            raise ArithmeticSemanticsFailure(
                f"HxcIR lost {operation!r} implementation {implementation!r}"
            )
    if "runtime(" in hxcir:
        raise ArithmeticSemanticsFailure("primitive HxcIR selected a runtime implementation")
    if re.search(
        r'convert[^\n]+kind=numeric-saturating[^\n]+implementation=program-local\("hxc\.f64\.to\.i32\.saturating"\)',
        hxcir,
    ) is None:
        raise ArithmeticSemanticsFailure("HxcIR lost the UB-safe Std.int conversion")

    if header.count("static inline ") != len(EXPECTED_HELPERS):
        raise ArithmeticSemanticsFailure("selected helper definitions are not one static-inline closure")
    for marker in (
        "#include <math.h>",
        "#include <stdint.h>",
        "uint64_t",
        "UINT32_C(2147483648)",
        "INT32_MIN",
        "return NAN;",
        "return fmod(",
    ):
        if marker not in header:
            raise ArithmeticSemanticsFailure(f"helper header lost {marker!r}")

    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
        raise ArithmeticSemanticsFailure("finalized symbol table is missing")
    helper_symbols = [item for item in symbol_entries(symbols) if item.get("kind") == "specialization"]
    if len(helper_symbols) != len(EXPECTED_HELPERS):
        raise ArithmeticSemanticsFailure("not every helper identifier passed through the symbol registry")

    uadd = function_body(source, function_c_name(symbols, "uadd"))
    ushl = function_body(source, function_c_name(symbols, "ushl"))
    ushr = function_body(source, function_c_name(symbols, "ushr"))
    if " + " not in uadd or "(uint64_t)" not in uadd or "(uint32_t)" not in uadd or "primitive" in uadd:
        raise ArithmeticSemanticsFailure("UInt addition stopped lowering to direct unsigned C")
    if " << " not in ushl or "(uint64_t)" not in ushl or "(uint32_t)31" not in ushl or "primitive" in ushl:
        raise ArithmeticSemanticsFailure("UInt left shift lost its direct masked fast path")
    if " >> " not in ushr or "(uint64_t)" not in ushr or "(uint32_t)31" not in ushr or "primitive" in ushr:
        raise ArithmeticSemanticsFailure("UInt right shift lost its direct masked fast path")


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "arithmetic.hxcir": required_text(report, "hxcir"),
        "program.h": required_text(report, "header"),
        "program.c": source_records(report)["src/program.c"],
        "contract.json": {
            "schemaVersion": report.get("schemaVersion"),
            "status": report.get("status"),
            "helpers": report.get("helpers"),
            "buildFacts": report.get("buildFacts"),
            "runtimeFeatures": report.get("runtimeFeatures"),
        },
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
        if not path.is_file():
            raise ArithmeticSemanticsFailure(f"registered snapshot {name} is missing")
        if name.endswith(".json"):
            if actual != json.loads(path.read_text(encoding="utf-8")):
                raise ArithmeticSemanticsFailure(f"{name} semantic snapshot drifted")
        else:
            if not isinstance(actual, str):
                raise ArithmeticSemanticsFailure(f"{name} snapshot is not text")
            expected = path.read_text(encoding="utf-8")
            if actual != expected:
                raise ArithmeticSemanticsFailure(
                    f"{name} drifted:\n" + difference(expected, actual, name)
                )


def check_oracle() -> None:
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
    if result.returncode != 0 or result.stdout != EXPECTED_ORACLE or result.stderr:
        raise ArithmeticSemanticsFailure(
            "Eval arithmetic oracle drifted\n"
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
                raise ArithmeticSemanticsFailure(f"required arithmetic toolchain missing: {family}")
            print(f"arithmetic-semantics: SKIP optional {family}: missing {family}")
            continue
        actual, version = compiler_identity(executable)
        if actual != family:
            if selected:
                raise ArithmeticSemanticsFailure(f"required {family} identity is {actual}")
            print(f"arithmetic-semantics: SKIP optional {family}: identity is {actual}")
            continue
        print(
            f"arithmetic-semantics: {'REQUIRED' if selected else 'AVAILABLE'} "
            f"{family}: cc={version}"
        )
        result.append(NativeToolchain(family, executable, version))
    if not result:
        raise ArithmeticSemanticsFailure("no strict C11 compiler is available")
    return result


def sanitizer_supported(toolchain: NativeToolchain, root: Path) -> bool:
    source = root / f"sanitizer-probe-{toolchain.family}.c"
    executable = root / f"sanitizer-probe-{toolchain.family}"
    source.write_text("int main(void) { return 0; }\n", encoding="utf-8", newline="\n")
    result = subprocess.run(
        [toolchain.compiler, *SANITIZER_FLAGS, str(source), "-o", str(executable)],
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        print(
            f"arithmetic-semantics: SKIP {toolchain.family} UBSan: unsupported "
            f"({(result.stderr or result.stdout).splitlines()[0]})"
        )
        return False
    return True


def harness_source(symbols: dict[str, object]) -> str:
    names = {
        field: function_c_name(symbols, field)
        for field in (
            "iadd", "isub", "imul", "ineg", "idiv", "imod", "ishl", "ishr",
            "iushr", "iand", "ior", "ixor", "inot", "iless", "fadd", "fsub",
            "fmul", "fneg", "fdiv", "fmod", "fint", "fequal", "uadd", "umod", "ushl",
            "ushr", "update",
        )
    }
    return f'''#include "hxc/program.h"

#include <math.h>
#include <stdint.h>

int main(void)
{{
  if ({names["iadd"]}(INT32_MAX, INT32_C(1)) != INT32_MIN) return 1;
  if ({names["isub"]}(INT32_MIN, INT32_C(1)) != INT32_MAX) return 2;
  if ({names["imul"]}(INT32_MAX, INT32_C(2)) != -INT32_C(2)) return 3;
  if ({names["ineg"]}(INT32_MIN) != INT32_MIN) return 4;
  if ({names["idiv"]}(INT32_MIN, -INT32_C(1)) != 2147483648.0) return 5;
  if (!isinf({names["idiv"]}(INT32_C(1), INT32_C(0)))) return 6;
  if (signbit({names["idiv"]}(INT32_C(1), INT32_C(0)))) return 7;
  if (!signbit({names["idiv"]}(-INT32_C(1), INT32_C(0)))) return 8;
  if (!isnan({names["idiv"]}(INT32_C(0), INT32_C(0)))) return 9;
  if ({names["imod"]}(INT32_MIN, -INT32_C(1)) != INT32_C(0)) return 10;
  if ({names["imod"]}(INT32_C(1), INT32_C(0)) != INT32_C(0)) return 11;
  if ({names["imod"]}(-INT32_C(7), INT32_C(3)) != -INT32_C(1)) return 12;
  if ({names["imod"]}(INT32_C(7), -INT32_C(3)) != INT32_C(1)) return 13;
  if ({names["ishl"]}(INT32_C(1), -INT32_C(1)) != INT32_MIN) return 14;
  if ({names["ishl"]}(INT32_C(1), INT32_C(32)) != INT32_C(1)) return 15;
  if ({names["ishr"]}(INT32_MIN, -INT32_C(1)) != -INT32_C(1)) return 16;
  if ({names["iushr"]}(INT32_MIN, -INT32_C(1)) != INT32_C(1)) return 17;
  if ({names["iand"]}(-INT32_C(1), INT32_C(0x55)) != INT32_C(0x55)) return 18;
  if ({names["ior"]}(INT32_C(0x50), INT32_C(0x0f)) != INT32_C(0x5f)) return 19;
  if ({names["ixor"]}(INT32_C(0x55), INT32_C(0x0f)) != INT32_C(0x5a)) return 20;
  if ({names["inot"]}(INT32_C(0)) != -INT32_C(1)) return 21;
  if (!{names["iless"]}(INT32_MIN, INT32_MAX)) return 22;
  if ({names["fadd"]}(1.25, 2.5) != 3.75) return 23;
  if ({names["fsub"]}(1.25, 2.5) != -1.25) return 24;
  if ({names["fmul"]}(1.5, 2.0) != 3.0) return 25;
  if (!signbit({names["fneg"]}(0.0))) return 26;
  if (!isnan({names["fmod"]}(1.0, 0.0))) return 27;
  if ({names["fmod"]}(-7.0, 3.0) != -1.0) return 28;
  if (!signbit({names["fmod"]}(-0.0, 3.0))) return 29;
  if ({names["fint"]}(NAN) != INT32_C(0)) return 30;
  if ({names["fint"]}(INFINITY) != INT32_MAX) return 31;
  if ({names["fint"]}(-INFINITY) != INT32_MIN) return 32;
  if ({names["fint"]}(2147483648.0) != INT32_MAX) return 33;
  if ({names["fint"]}(-2147483649.0) != INT32_MIN) return 34;
  if ({names["fint"]}(-3.75) != -INT32_C(3)) return 35;
  if ({names["fint"]}(-0.0) != INT32_C(0)) return 36;
  if ({names["fequal"]}(NAN, NAN)) return 37;
  if ({names["uadd"]}(UINT32_MAX, UINT32_C(1)) != UINT32_C(0)) return 38;
  if ({names["umod"]}(UINT32_MAX, UINT32_C(0)) != UINT32_C(0)) return 39;
  if ({names["umod"]}(UINT32_MAX, UINT32_C(2147483648)) != UINT32_C(2147483647)) return 40;
  if ({names["ushl"]}(UINT32_C(1), -INT32_C(1)) != UINT32_C(2147483648)) return 41;
  if ({names["ushr"]}(UINT32_C(2147483648), -INT32_C(1)) != UINT32_C(1)) return 42;
  if ({names["update"]}(INT32_C(3)) != INT32_C(18)) return 43;
  return 0;
}}
'''


def run_command(command: list[str], label: str, *, timeout: int = 30) -> None:
    result = subprocess.run(command, capture_output=True, text=True, timeout=timeout)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ArithmeticSemanticsFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def compile_and_run_project(
    root: Path,
    sources: list[Path],
    symbols: dict[str, object],
    toolchain: NativeToolchain,
    optimization: str,
    *,
    sanitizer: bool = False,
) -> None:
    suffix = optimization[1:].lower() + ("-ubsan" if sanitizer else "")
    extra_flags = list(SANITIZER_FLAGS) if sanitizer else []
    renamed_entry_header = root / "renamed_entry.h"
    renamed_entry_header.write_text(
        "int hxc_generated_main(void);\n", encoding="utf-8", newline="\n"
    )
    harness = root / "arithmetic_harness.c"
    harness.write_text(harness_source(symbols), encoding="utf-8", newline="\n")
    objects: list[Path] = []
    for index, source in enumerate(sources):
        target = root / f"source-{index}-{toolchain.family}-{suffix}.o"
        run_command(
            [
                toolchain.compiler,
                *STRICT_FLAGS,
                optimization,
                *extra_flags,
                "-Dmain=hxc_generated_main",
                "-include",
                str(renamed_entry_header),
                "-I",
                str(root / "include"),
                "-c",
                str(source),
                "-o",
                str(target),
            ],
            f"{toolchain.family} {optimization} generated arithmetic compile",
        )
        objects.append(target)
    harness_object = root / f"harness-{toolchain.family}-{suffix}.o"
    run_command(
        [
            toolchain.compiler,
            *STRICT_FLAGS,
            optimization,
            *extra_flags,
            "-I",
            str(root / "include"),
            "-c",
            str(harness),
            "-o",
            str(harness_object),
        ],
        f"{toolchain.family} {optimization} arithmetic harness compile",
    )
    executable = root / f"arithmetic-{toolchain.family}-{suffix}"
    run_command(
        [
            toolchain.compiler,
            *extra_flags,
            *objects,
            str(harness_object),
            "-lm",
            "-o",
            str(executable),
        ],
        f"{toolchain.family} {optimization} arithmetic link",
    )
    run_command(
        [str(executable)],
        f"{toolchain.family} {optimization} arithmetic boundary execution",
        timeout=10,
    )

    if optimization == "-O2" and not sanitizer:
        assembly = root / f"arithmetic-{toolchain.family}-o2.s"
        run_command(
            [
                toolchain.compiler,
                *STRICT_FLAGS,
                "-O2",
                "-Dmain=hxc_generated_main",
                "-include",
                str(renamed_entry_header),
                "-I",
                str(root / "include"),
                "-S",
                str(sources[0]),
                "-o",
                str(assembly),
            ],
            f"{toolchain.family} optimized arithmetic assembly",
        )
        assembly_text = assembly.read_text(encoding="utf-8", errors="strict")
        helper_names = [
            item.get("cName")
            for item in symbol_entries(symbols)
            if item.get("kind") == "specialization"
        ]
        if any(isinstance(name, str) and name in assembly_text for name in helper_names):
            raise ArithmeticSemanticsFailure(
                f"{toolchain.family} -O2 retained an out-of-line primitive helper"
            )


def write_report_project(
    report: dict[str, object], root: Path
) -> tuple[list[Path], dict[str, object]]:
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
        raise ArithmeticSemanticsFailure("report symbols are missing")
    return sources, symbols


def check_native_report(
    report: dict[str, object], selected: str | None = None
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-arithmetic-native-") as temporary:
        root = Path(temporary)
        sources, symbols = write_report_project(report, root)
        for toolchain in available_compilers(selected):
            compile_and_run_project(root, sources, symbols, toolchain, "-O0")
            compile_and_run_project(root, sources, symbols, toolchain, "-O2")
            if sanitizer_supported(toolchain, root):
                compile_and_run_project(
                    root, sources, symbols, toolchain, "-O1", sanitizer=True
                )


def custom_target(
    output: Path, *, profile: str = "portable", runtime: str | None = None
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(FIXTURE),
        "-lib",
        "reflaxe.c",
        "-main",
        "ArithmeticFixture",
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
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
    with tempfile.TemporaryDirectory(prefix="hxc-arithmetic-production-") as temporary:
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
                raise ArithmeticSemanticsFailure(
                    f"{label} production compile failed\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        if generated_tree(portable) != generated_tree(repeated):
            raise ArithmeticSemanticsFailure("repeated arithmetic outputs are not byte-identical")
        for relative in ("include/hxc/program.h", "src/program.c"):
            if (portable / relative).read_bytes() != (metal / relative).read_bytes():
                raise ArithmeticSemanticsFailure(
                    f"portable and metal arithmetic artifact {relative} diverged"
                )
        manifest = json.loads((portable / "hxc.manifest.json").read_text())
        requirements = manifest.get("build", {}).get("requirements")
        if requirements != [
            {
                "kind": "link",
                "name": "m",
                "value": None,
                "valueKind": None,
                "ownerModulePaths": ["ArithmeticFixture"],
            }
        ]:
            raise ArithmeticSemanticsFailure("production manifest lost the selected math link fact")
        runtime_plan = json.loads((portable / "hxc.runtime-plan.json").read_text())
        none_plan = json.loads((no_runtime / "hxc.runtime-plan.json").read_text())
        if (
            runtime_plan.get("status") != "analyzed-runtime-free"
            or runtime_plan.get("features") != []
            or "ub-safe-primitive-operations" not in runtime_plan.get("directDecisions", [])
            or "selected-program-local-helpers" not in runtime_plan.get("directDecisions", [])
            or not runtime_plan.get("noRuntimeProof")
            or none_plan.get("resolvedPolicy") != "none"
        ):
            raise ArithmeticSemanticsFailure("zero-runtime production proof drifted")
        if b"hxrt" in b"\n".join(generated_tree(portable).values()).lower():
            raise ArithmeticSemanticsFailure("primitive arithmetic selected hxrt")
        symbols = json.loads((portable / "hxc.symbols.json").read_text())
        sources = sorted((portable / "src").glob("*.c"))
        for toolchain in available_compilers(selected):
            compile_and_run_project(portable, sources, symbols, toolchain, "-O0")
            compile_and_run_project(portable, sources, symbols, toolchain, "-O2")
            if sanitizer_supported(toolchain, portable):
                compile_and_run_project(
                    portable, sources, symbols, toolchain, "-O1", sanitizer=True
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
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("arithmetic-semantics: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            check_native_report(snapshot_native_report(), args.toolchain)
            print(
                "arithmetic-semantics: OK: required strict-C boundary, UBSan, and optimized native matrix passed"
            )
            return 0
        check_oracle()
        first_payload, first = render("first arithmetic render")
        second_payload, second = render("second arithmetic render")
        reverse_payload, reverse = render("reverse arithmetic render", reverse=True)
        metal_payload, metal = render("metal arithmetic render", profile="metal")
        if first_payload != second_payload or first != second:
            raise ArithmeticSemanticsFailure("repeated arithmetic renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise ArithmeticSemanticsFailure("arithmetic render changed with input order")
        validate(first)
        validate(metal, profile="metal")
        if {**first, "profile": "<profile>"} != {**metal, "profile": "<profile>"}:
            raise ArithmeticSemanticsFailure("portable and metal arithmetic reports diverged")
        check_snapshots(first)
        check_native_report(first, args.toolchain)
        check_production(args.toolchain)
    except (
        ArithmeticSemanticsFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"arithmetic-semantics: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "arithmetic-semantics: OK: typed boundaries, Eval differential, strict C11, UBSan, "
        "runtime-free helpers, and optimized fast paths passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
