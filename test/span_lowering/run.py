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
import subprocess
import sys
import tempfile
from collections.abc import Iterable
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


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def render(
    label: str,
    *,
    reverse: bool = False,
    profile: str = "portable",
    build: str = "debug",
) -> tuple[str, dict[str, object]]:
    if profile not in PROFILES or build not in BUILD_MODES:
        raise SpanLoweringFailure(f"invalid probe matrix coordinate {profile}/{build}")
    command = [development_tool("haxe"), str(HXML)]
    if reverse:
        command.extend(["-D", "span_lowering_reverse_input"])
    if profile == "metal":
        command.extend(["-D", "span_lowering_profile=metal"])
    if build != "debug":
        command.extend(["-D", f"span_lowering_build={build}"])
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise SpanLoweringFailure(
            f"{label} emitted an invalid report\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise SpanLoweringFailure(f"{label} report is not an object")
    return payload, report


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


def function_names(symbols: dict[str, object]) -> dict[str, str]:
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
    if set(names) != {"checkedAt", "constSum", "main", "mutableSum"}:
        raise SpanLoweringFailure(f"span fixture symbol set drifted: {sorted(names)!r}")
    return names


def validate(report: dict[str, object], *, profile: str, build: str) -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "typed-fixed-arrays-and-spans-runtime-free"
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
        "initialize-fixed-array": 3,
        "initialize-span": 3,
        "bounds-check": 5,
        "static-proof": 2,
        "loop-guarded": 2,
        'index-local="local.3",length=4': 2,
        "checked-abort": 1,
        'operation="hxc.size.add-one.span-index-proven"': 2,
    }
    for marker, expected in expected_counts.items():
        actual = hxcir.count(marker)
        if actual != expected:
            raise SpanLoweringFailure(
                f"HxcIR marker {marker!r} appeared {actual} times, expected {expected}"
            )
    policy = f'policy=checked-abort(profile="{profile}",build="{build}")'
    if hxcir.count(policy) != 1:
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
    if not isinstance(functions, list) or len(functions) != 4:
        raise SpanLoweringFailure("span function inventory drifted")
    fields = [item.get("field") for item in functions if isinstance(item, dict)]
    if fields != ["checkedAt", "constSum", "main", "mutableSum"]:
        raise SpanLoweringFailure(f"span function order drifted: {fields!r}")
    length_names = [
        name
        for item in functions
        if isinstance(item, dict)
        for name in item.get("spanLengthNames", [])
        if isinstance(name, str)
    ]
    if len(length_names) != 3 or len(set(length_names)) != 3:
        raise SpanLoweringFailure("span length identifiers are not unique and complete")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
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
    profile: str = "portable",
    build: str = "debug",
    runtime: str = "none",
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
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
        command.extend(["-D", "reflaxe_c_profile=metal"])
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
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
    if (
        initialization_plan.get("schemaVersion") != 1
        or initialization_plan.get("strategy") != "eager-haxe-type-order"
        or initialization_plan.get("runtimeFeatures") != []
        or runtime_plan.get("profile") != profile
        or runtime_plan.get("resolvedPolicy") != "none"
        or runtime_plan.get("status") != "analyzed-runtime-free"
        or runtime_plan.get("features") != []
        or runtime_plan.get("artifacts") != []
        or not runtime_plan.get("noRuntimeProof")
        or manifest.get("configuration", {}).get("profile") != profile
        or manifest.get("configuration", {}).get("runtimePolicy") != "none"
    ):
        raise SpanLoweringFailure(f"{profile}/{build} lost its runtime-free policy proof")
    combined = "\n".join(
        path.read_text(encoding="utf-8")
        for path in root.rglob("*")
        if path.is_file() and path.suffix in {".c", ".h", ".json"}
    )
    if "hxrt" in combined.lower():
        raise SpanLoweringFailure(f"{profile}/{build} selected or mentioned hxrt")
    for marker in ("#include <stddef.h>", "#include <stdlib.h>"):
        if marker not in header:
            raise SpanLoweringFailure(f"generated header lost {marker!r}")
    for forbidden in ("iterator", "hasNext", "Array<", " + index", " + (size_t)"):
        if forbidden in source:
            raise SpanLoweringFailure(f"generated C retained forbidden shape {forbidden!r}")
    if (
        len(re.findall(r"int32_t [A-Za-z0-9_]+\[4\] = \{", source)) != 3
        or source.count("const int32_t *") != 2
        or len(re.findall(r"(?m)^  int32_t \*[A-Za-z0-9_]+ =", source)) != 1
        or source.count(" = sizeof(") != 3
        or source.count("bounds") != 0
        or source.count("abort();") != 1
        or source.count("[(size_t)") != 5
    ):
        raise SpanLoweringFailure(f"{profile}/{build} fixed-array/span C shape drifted")
    if source.count(" < 4)") != 2 or source.count(" + 1;") != 2:
        raise SpanLoweringFailure("span iteration stopped being a direct guarded index loop")
    if source.count(" < 0 || (size_t)") != 1 or source.count(" >= ") != 1:
        raise SpanLoweringFailure("dynamic span access lost its signed/size_t bounds check")
    return {"header": header, "source": source}


def production_snapshot() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-span-snapshot-") as temporary:
        output = Path(temporary) / "out"
        result = custom_target("SpanFixture", output)
        if result.returncode != 0 or result.stdout or result.stderr:
            raise SpanLoweringFailure(
                "span production snapshot failed\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        project = validate_project(output, profile="portable", build="debug")
        return {"program.h": project["header"], "program.c": project["source"]}


def snapshot_artifacts() -> dict[str, object]:
    _, report = render("span snapshot render")
    validate(report, profile="portable", build="debug")
    artifacts: dict[str, object] = {
        "span.hxcir": required_text(report, "hxcir"),
        "symbols.json": report.get("symbols"),
    }
    artifacts.update(production_snapshot())
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


def run_command(command: list[str], label: str, *, timeout: int = 30) -> None:
    result = subprocess.run(command, capture_output=True, text=True, timeout=timeout)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise SpanLoweringFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def harness_source(symbols: dict[str, object]) -> str:
    names = function_names(symbols)
    return f'''#include "hxc/program.h"

#include <stdint.h>

int main(void)
{{
  if ({names["mutableSum"]}() != INT32_C(34)) return 1;
  if ({names["constSum"]}() != INT32_C(32)) return 2;
  if ({names["checkedAt"]}(INT32_C(2)) != INT32_C(8)) return 3;
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
        for toolchain in available_compilers(selected):
            for optimization in ("-O0", "-O2"):
                generated_object = root / f"generated-{toolchain.family}-{optimization[1:]}.o"
                harness_object = root / f"harness-{toolchain.family}-{optimization[1:]}.o"
                executable = root / f"span-{toolchain.family}-{optimization[1:]}"
                run_command(
                    [
                        toolchain.compiler,
                        *STRICT_FLAGS,
                        optimization,
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
                    f"{toolchain.family} {optimization} generated span compile",
                )
                run_command(
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
                    f"{toolchain.family} {optimization} span harness compile",
                )
                run_command(
                    [toolchain.compiler, str(generated_object), str(harness_object), "-o", str(executable)],
                    f"{toolchain.family} {optimization} span link",
                )
                run_command([str(executable)], f"{toolchain.family} {optimization} span behavior")
                check_no_hxrt_symbols(executable, f"{toolchain.family} {optimization}")


def compile_failure_fixture(
    main_class: str, expected_fragment: str, *, profile: str
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-span-negative-") as temporary:
        output = Path(temporary) / "out"
        result = custom_target(main_class, output, profile=profile)
        combined = result.stdout + result.stderr
        if (
            result.returncode != 1
            or "HXC1001" not in combined
            or expected_fragment not in combined
            or f"[profile={profile}]" not in combined
        ):
            raise SpanLoweringFailure(
                f"{profile} {main_class} did not fail closed as expected\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        assert_no_output(output, f"{profile} {main_class} HXC1001")


def check_configuration_failure(*, profile: str) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-span-config-") as temporary:
        output = Path(temporary) / "out"
        result = custom_target("SpanFixture", output, profile=profile, build="fast")
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


def check_abort_fixture(
    main_class: str,
    toolchain: NativeToolchain,
    *,
    profile: str,
    build: str,
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-span-bounds-") as temporary:
        root = Path(temporary)
        output = root / "out"
        result = custom_target(main_class, output, profile=profile, build=build)
        if result.returncode != 0 or result.stdout or result.stderr:
            raise SpanLoweringFailure(f"{profile}/{build} {main_class} did not compile")
        combined = "\n".join(
            path.read_text(encoding="utf-8")
            for path in output.rglob("*")
            if path.is_file() and path.suffix in {".c", ".h", ".json"}
        )
        if "hxrt" in combined.lower() or combined.count("abort();") != 1:
            raise SpanLoweringFailure(f"{profile}/{build} {main_class} lost its no-hxrt abort")
        executable = root / "bounds"
        run_command(
            [
                toolchain.compiler,
                *STRICT_FLAGS,
                "-O2",
                "-I",
                str(output / "include"),
                str(output / "src/program.c"),
                "-o",
                str(executable),
            ],
            f"{profile}/{build} {main_class} strict compile",
        )
        ran = subprocess.run([str(executable)], capture_output=True, text=True, timeout=10)
        if ran.returncode == 0:
            raise SpanLoweringFailure(f"{profile}/{build} {main_class} did not fail stop")
        check_no_hxrt_symbols(executable, f"{profile}/{build} {main_class}")


def check_production(selected: str | None = None) -> dict[str, object]:
    canonical: dict[str, object] | None = None
    canonical_source: str | None = None
    canonical_header: str | None = None
    with tempfile.TemporaryDirectory(prefix="hxc-span-production-") as temporary:
        root = Path(temporary)
        for profile in PROFILES:
            for build in BUILD_MODES:
                output = root / f"{profile}-{build}"
                result = custom_target("SpanFixture", output, profile=profile, build=build)
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
                    }
                    canonical_header = project["header"]
                    canonical_source = project["source"]
                elif project["header"] != canonical_header or project["source"] != canonical_source:
                    raise SpanLoweringFailure("profile/build matrix changed generated C bytes")
        repeat = root / "repeat"
        result = custom_target("SpanFixture", repeat)
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
            check_abort_fixture("UpperBoundsFixture", toolchain, profile=profile, build=build)
            check_abort_fixture("NegativeBoundsFixture", toolchain, profile=profile, build=build)
    return canonical


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("gcc", "clang"))
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("span-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            artifacts = {
                "program.h": (EXPECTED / "program.h").read_text(encoding="utf-8"),
                "program.c": (EXPECTED / "program.c").read_text(encoding="utf-8"),
            }
            symbols = json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8"))
            check_native_artifacts(artifacts, symbols, args.toolchain)
            print("span-lowering: OK: required strict-C native matrix passed")
            return 0

        baseline: dict[str, object] | None = None
        canonical_report: dict[str, object] | None = None
        for profile in PROFILES:
            for build in BUILD_MODES:
                first_payload, first = render(
                    f"first {profile}/{build} span render", profile=profile, build=build
                )
                second_payload, second = render(
                    f"second {profile}/{build} span render", profile=profile, build=build
                )
                reverse_payload, reverse = render(
                    f"reverse {profile}/{build} span render",
                    reverse=True,
                    profile=profile,
                    build=build,
                )
                if first_payload != second_payload or first_payload != reverse_payload:
                    raise SpanLoweringFailure(
                        f"{profile}/{build} span report changed across repeat/input order"
                    )
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
        for profile in PROFILES:
            compile_failure_fixture(
                "NonLiteralFixture", "TFunction(return-type):reference-Array-non-null", profile=profile
            )
            compile_failure_fixture(
                "ZeroLengthFixture", "TArrayDecl(empty-fixed-array-not-strict-c11)", profile=profile
            )
            compile_failure_fixture(
                "LookalikeFixture",
                "TVar(Span:requires-fixed-array-borrow)",
                profile=profile,
            )
        for profile in PROFILES:
            check_configuration_failure(profile=profile)
        production = check_production(args.toolchain)
        snapshots: dict[str, object] = {
            "span.hxcir": required_text(canonical_report, "hxcir"),
            "symbols.json": canonical_report.get("symbols"),
            **production,
        }
        check_snapshots(snapshots)
        symbols = canonical_report.get("symbols")
        if not isinstance(symbols, dict):
            raise SpanLoweringFailure("canonical span symbols are missing")
        check_native_artifacts(production, symbols, args.toolchain)
        print(
            "span-lowering: OK: fixed arrays, direct spans, bounds matrix, "
            "strict C11, and zero-hxrt links passed"
        )
        return 0
    except (OSError, subprocess.TimeoutExpired, SpanLoweringFailure, json.JSONDecodeError) as error:
        print(f"span-lowering: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
