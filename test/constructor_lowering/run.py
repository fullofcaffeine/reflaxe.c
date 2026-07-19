#!/usr/bin/env python3
"""Prove bounded constructor ordering, cleanup, elision, and fail-closed escapes."""

from __future__ import annotations

import argparse
import copy
import difflib
import json
import os
import shutil
import signal
import subprocess
import sys
import tempfile
from collections.abc import Iterable
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FIXTURES = Path(__file__).with_name("fixtures")
POSITIVE = FIXTURES / "positive"
ORACLE = FIXTURES / "oracle"
MINIMAL = FIXTURES / "minimal"
FAILURE_RUNTIME = FIXTURES / "failure_runtime"
DEFAULT_RUNTIME = FIXTURES / "default_runtime"
NATIVE = Path(__file__).with_name("native")
EXPECTED = Path(__file__).with_name("expected")
REPORT_PREFIX = "HXC_CONSTRUCTOR_LOWERING="
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
EXPECTED_CONSTRUCTORS = {
    "constructor.BaseRecord": (False, False),
    "constructor.EmptyBase": (True, False),
    "constructor.EmptyLeaf": (True, False),
    "constructor.FailableBase": (False, True),
    "constructor.FailableLeaf": (False, True),
    "constructor.LeafRecord": (False, False),
}
NEGATIVE_CASES = {
    "conditional": "TNew(stack-construction-requires-unconditional-entry-block)",
    "cycle": "TNew(constructor-cycle:CycleA -> CycleB -> CycleA)",
    "escape_alias": "TNew(stack-reference-escape:local-alias:second)",
    "escape_argument": (
        "TNew(stack-reference-escape:static-call-argument:0,"
        "target=function.Main.present)"
    ),
    "escape_return": "TNew(stack-reference-escape:return)",
    "escape_self": "TNew(stack-reference-escape:assignment)",
    "generic": "TVar(box:type):generic-class-reference-requires-bounded-class-specialization:Box",
    "native_layout": "TNew(unsupported-native-layout:NativeRecord)",
}
REQUIRED_NATIVE_COVERAGE = frozenset(
    {
        "constructor-cleanup-failure",
        "constructor-default-field-values",
        "constructor-generated-executable",
        "constructor-runtime-free",
        "field-initialization-order",
        "strict-c11",
        "super-constructor-order",
        "trivial-constructor-elision",
    }
)

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CFixtureFailure,
    CFixtureProject,
    report_json,
    resolve_toolchains,
    run_c_fixture_corpus,
    validate_report,
)


CXX_STRICT_FLAGS = (
    "-std=c++17",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
CXX_COMMANDS = {"gcc": "g++", "clang": "clang++"}


class ConstructorLoweringFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def custom_target(
    fixture: Path,
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    reverse: bool = False,
    report: bool = False,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main",
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    elif profile != "portable":
        raise ConstructorLoweringFailure(f"unknown profile {profile!r}")
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_constructor_lowering_report"])
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
    if not root.exists():
        return set()
    return {
        path.relative_to(root).as_posix()
        for path in root.rglob("*")
        if path.is_file()
    }


def generated_tree(root: Path) -> dict[str, bytes]:
    return {
        path.relative_to(root).as_posix(): path.read_bytes()
        for path in sorted(root.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def required_text(value: object, label: str) -> str:
    if not isinstance(value, str):
        raise ConstructorLoweringFailure(f"{label} must be text")
    return value


def required_objects(value: object, label: str) -> list[dict[str, object]]:
    if not isinstance(value, list) or not all(isinstance(item, dict) for item in value):
        raise ConstructorLoweringFailure(f"{label} must be an object array")
    return value


def extract_report(
    result: subprocess.CompletedProcess[str], output: Path, label: str
) -> tuple[str, dict[str, object]]:
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if result.returncode != 0 or result.stderr or len(lines) != 1:
        raise ConstructorLoweringFailure(
            f"{label} emitted an invalid constructor report\n"
            f"exit={result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    other_lines = [line for line in result.stdout.splitlines() if not line.startswith(REPORT_PREFIX)]
    if other_lines:
        raise ConstructorLoweringFailure(f"{label} emitted unexpected stdout: {other_lines!r}")
    payload = lines[0][len(REPORT_PREFIX) :]
    report = json.loads(payload)
    if not isinstance(report, dict):
        raise ConstructorLoweringFailure(f"{label} report is not an object")
    if generated_files(output) != PRODUCTION_FILES:
        raise ConstructorLoweringFailure(
            f"{label} artifact set drifted: {sorted(generated_files(output))!r}"
        )
    report["header"] = (output / "include/hxc/program.h").read_text(encoding="utf-8")
    report["source"] = (output / "src/program.c").read_text(encoding="utf-8")
    report["symbols"] = json.loads(
        (output / "hxc.symbols.json").read_text(encoding="utf-8")
    )
    report["runtimePlan"] = json.loads(
        (output / "hxc.runtime-plan.json").read_text(encoding="utf-8")
    )
    report["manifest"] = json.loads(
        (output / "hxc.manifest.json").read_text(encoding="utf-8")
    )
    report["abi"] = json.loads((output / "hxc.abi.json").read_text(encoding="utf-8"))
    return payload, report


def render(
    label: str,
    *,
    reverse: bool = False,
    profile: str = "portable",
    runtime: str | None = None,
) -> tuple[str, dict[str, object]]:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-render-") as temporary:
        output = Path(temporary) / "generated"
        result = custom_target(
            POSITIVE,
            output,
            profile=profile,
            runtime=runtime,
            reverse=reverse,
            report=True,
        )
        return extract_report(result, output, label)


def function_section(hxcir: str, identifier: str) -> str:
    start_marker = f'  function "{identifier}"'
    end_marker = f'  end function "{identifier}"'
    start = hxcir.find(start_marker)
    end = hxcir.find(end_marker, start)
    if start == -1 or end == -1:
        raise ConstructorLoweringFailure(f"HxcIR omitted function {identifier}")
    return hxcir[start : end + len(end_marker)]


def ordered(section: str, markers: tuple[str, ...], label: str) -> None:
    offset = -1
    for marker in markers:
        next_offset = section.find(marker, offset + 1)
        if next_offset == -1:
            raise ConstructorLoweringFailure(f"{label} omitted {marker!r}")
        if next_offset <= offset:
            raise ConstructorLoweringFailure(f"{label} reordered {marker!r}")
        offset = next_offset


def constructor_map(report: dict[str, object]) -> dict[str, dict[str, object]]:
    records = required_objects(report.get("constructors"), "constructors")
    identifiers = [record.get("id") for record in records]
    if not all(isinstance(identifier, str) for identifier in identifiers):
        raise ConstructorLoweringFailure("constructor records omitted stable IDs")
    if identifiers != sorted(identifiers, key=lambda value: value.encode("utf-8")):
        raise ConstructorLoweringFailure("constructor records are not UTF-8 ordered")
    result = {str(record["id"]): record for record in records}
    if set(result) != set(EXPECTED_CONSTRUCTORS):
        raise ConstructorLoweringFailure(
            f"reachable constructor set drifted: {sorted(result)!r}"
        )
    return result


def validate_runtime_plan(report: dict[str, object], *, profile: str) -> None:
    plan = report.get("runtimePlan")
    manifest = report.get("manifest")
    abi = report.get("abi")
    if not isinstance(plan, dict) or not isinstance(manifest, dict) or not isinstance(abi, dict):
        raise ConstructorLoweringFailure("production report omitted plan, manifest, or ABI")
    proof = plan.get("noRuntimeProof")
    reachability = proof.get("reachability") if isinstance(proof, dict) else None
    if (
        plan.get("schemaVersion") != 2
        or plan.get("status") != "analyzed-runtime-free"
        or plan.get("profile") != profile
        or plan.get("features") != []
        or plan.get("artifacts") != []
        or "bounded-stack-construction" not in plan.get("directDecisions", [])
        or not isinstance(proof, dict)
        or proof.get("status") != "eligible"
        or "compiler-proven stack construction" not in str(proof.get("semanticProof"))
        or not isinstance(reachability, dict)
        or reachability.get("cleanupActions", 0) < 8
        or manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or manifest.get("configuration", {}).get("profile") != profile
        or abi.get("status") != "analyzed-no-public-exports"
        or abi.get("exports") != []
        or abi.get("types") != []
    ):
        raise ConstructorLoweringFailure("constructor runtime-free production contract drifted")


def validate(report: dict[str, object], *, profile: str = "portable") -> None:
    if report.get("schemaVersion") != 1 or report.get("profile") != profile:
        raise ConstructorLoweringFailure(f"constructor report contract drifted for {profile}")
    constructors = constructor_map(report)
    for identifier, (elided, can_fail) in EXPECTED_CONSTRUCTORS.items():
        record = constructors[identifier]
        c_name = record.get("cName")
        if (
            record.get("elided") is not elided
            or record.get("canFail") is not can_fail
            or not isinstance(record.get("haxePath"), str)
            or not isinstance(record.get("instanceId"), str)
            or (elided and c_name is not None)
            or (not elided and (not isinstance(c_name, str) or not c_name))
        ):
            raise ConstructorLoweringFailure(f"constructor record drifted: {identifier}")
        if isinstance(c_name, str) and ("::" in c_name or c_name.startswith("new ")):
            raise ConstructorLoweringFailure("constructor escaped ordinary C function naming")

    hxcir = required_text(report.get("hxcir"), "HxcIR")
    header = required_text(report.get("header"), "header")
    source = required_text(report.get("source"), "source")
    symbols = report.get("symbols")
    if not isinstance(symbols, dict) or symbols.get("algorithm") != "hxc-c-symbol-v1":
        raise ConstructorLoweringFailure("constructor report omitted finalized symbols")
    for label, text in (("HxcIR", hxcir), ("header", header), ("source", source)):
        if str(ROOT) in text or "\\" in text or "hxrt" in text.lower():
            raise ConstructorLoweringFailure(f"{label} leaked a host path or runtime")
    if not hxcir.startswith("hxcir schema=10\n"):
        raise ConstructorLoweringFailure("constructor lowering did not use schema-10 HxcIR")

    leaf = function_section(hxcir, "constructor.LeafRecord")
    ordered(
        leaf,
        (
            "constant value=int(3)",
            'store place=field(dereference("parameter.self"),"secondLeafField")',
            "constant value=int(2)",
            'store place=field(dereference("parameter.self"),"firstLeafField")',
            "constant value=int(6)",
            'dispatch=direct("constructor.BaseRecord")',
            "constant value=int(7)",
        ),
        "derived constructor",
    )
    base = function_section(hxcir, "constructor.BaseRecord")
    ordered(
        base,
        (
            "constant value=int(4)",
            'store place=field(dereference("parameter.self"),"baseField")',
            "constant value=int(5)",
            'store place=field(dereference("parameter.self"),"defaulted")',
        ),
        "base constructor",
    )
    failable_base = function_section(hxcir, "constructor.FailableBase")
    failable_leaf = function_section(hxcir, "constructor.FailableLeaf")
    main = function_section(hxcir, "function.Main.main")
    if (
        "failure=status(exception)" not in failable_base
        or "failure=status(exception)" not in failable_leaf
        or 'edge=failure(kind=exception,target=propagate' not in failable_base
        or 'cleanup=["cleanup.construction"."construction.0.initialized"]'
        not in failable_base
        or 'dispatch=direct("constructor.FailableBase")' not in failable_leaf
        or "target=propagate" not in failable_leaf
    ):
        raise ConstructorLoweringFailure("constructor status/cleanup propagation drifted")
    ordered(
        main,
        (
            "constant value=int(1)",
            'dispatch=direct("function.ConstructorTrace.mark")',
            "class-default-initialize",
            'dispatch=direct("constructor.LeafRecord")',
            "constructor-complete",
        ),
        "construction call site",
    )
    if (
        'transition=uninitialized->initializing' not in main
        or 'transition=initializing->initialized reason="constructor completed"' not in main
        or 'transition=initializing->destroyed' not in main
        or 'transition=initialized->destroyed' not in main
        or 'target=abort' not in main
        or '"construction.2.partial"' not in main
        or '"construction.1.initialized"' not in main
        or '"construction.0.initialized"' not in main
    ):
        raise ConstructorLoweringFailure("partial-construction cleanup order drifted")

    emitted_names = {
        identifier: record.get("cName") for identifier, record in constructors.items()
    }
    for identifier in ("constructor.EmptyBase", "constructor.EmptyLeaf"):
        if emitted_names[identifier] is not None:
            raise ConstructorLoweringFailure(f"{identifier} was not elided")
    for identifier in (
        "constructor.BaseRecord",
        "constructor.FailableBase",
        "constructor.FailableLeaf",
        "constructor.LeafRecord",
    ):
        name = emitted_names[identifier]
        if not isinstance(name, str) or f"{name}(" not in header or f"{name}(" not in source:
            raise ConstructorLoweringFailure(f"emitted constructor missing: {identifier}")
    if (
        " = { 0 };" not in source
        or "if (!" not in source
        or "abort();" not in source
        or "bool " not in header
        or "int main(void)" not in source
    ):
        raise ConstructorLoweringFailure("structural constructor C emission drifted")
    validate_runtime_plan(report, profile=profile)


def normalized_profile(report: dict[str, object]) -> dict[str, object]:
    value = copy.deepcopy(report)
    value.pop("runtimePlan", None)
    value.pop("manifest", None)
    value.pop("abi", None)
    value["profile"] = "<profile>"
    hxcir = value.get("hxcir")
    if isinstance(hxcir, str):
        value["hxcir"] = hxcir.replace(
            'profile="portable"', 'profile="<profile>"'
        ).replace('profile="metal"', 'profile="<profile>"')
    return value


def snapshot_values(report: dict[str, object]) -> dict[str, object]:
    return {
        "constructors.hxcir": required_text(report.get("hxcir"), "HxcIR"),
        "constructors.json": required_objects(report.get("constructors"), "constructors"),
        "program.h": required_text(report.get("header"), "header"),
        "program.c": required_text(report.get("source"), "source"),
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
            raise ConstructorLoweringFailure(
                "constructor snapshots are missing; run "
                "npm run snapshots:update -- --suite constructor-lowering"
            )
        if name.endswith(".json"):
            expected = json.loads(path.read_text(encoding="utf-8"))
            if actual != expected:
                raise ConstructorLoweringFailure(f"{name} semantic snapshot drifted")
        else:
            actual_text = required_text(actual, name)
            expected_text = path.read_text(encoding="utf-8")
            if actual_text != expected_text:
                raise ConstructorLoweringFailure(
                    f"{name} drifted:\n" + difference(expected_text, actual_text, name)
                )


def require_silent_success(
    command: list[str], *, label: str, cwd: Path = ROOT
) -> None:
    result = subprocess.run(
        command,
        cwd=cwd,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            f"{label} failed\nexit={result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def write_generated_fixture(report: dict[str, object], destination: Path) -> None:
    header = destination / "include/hxc/program.h"
    source = destination / "src/program.c"
    header.parent.mkdir(parents=True, exist_ok=True)
    source.parent.mkdir(parents=True, exist_ok=True)
    header.write_text(required_text(report.get("header"), "header"), encoding="utf-8", newline="\n")
    source.write_text(required_text(report.get("source"), "source"), encoding="utf-8", newline="\n")


def compile_failure_fixture(destination: Path) -> None:
    result = custom_target(FAILURE_RUNTIME, destination)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            "failure-runtime fixture did not compile cleanly\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def compile_default_fixture(destination: Path) -> None:
    result = custom_target(DEFAULT_RUNTIME, destination)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            "default-field runtime fixture did not compile cleanly\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    plan = json.loads((destination / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        "bounded-stack-construction" not in plan.get("directDecisions", [])
        or plan.get("features") != []
    ):
        raise ConstructorLoweringFailure(
            "default-field runtime fixture lost its direct runtime-free plan"
        )


def check_cpp_header(
    fixture_root: Path, build_root: Path, *, requested_toolchain: str
) -> None:
    for toolchain in resolve_toolchains(requested_toolchain, repository_root=ROOT):
        command_name = CXX_COMMANDS[toolchain.family]
        compiler = shutil.which(command_name)
        if compiler is None:
            if requested_toolchain != "auto":
                raise ConstructorLoweringFailure(
                    f"required C++ companion {command_name!r} is unavailable"
                )
            print(
                f"constructor-lowering: SKIP optional {toolchain.family} C++17 header consumer"
            )
            continue
        for optimization in ("-O0", "-O2"):
            output = build_root / toolchain.family / optimization[1:].lower()
            output.mkdir(parents=True, exist_ok=True)
            require_silent_success(
                [
                    compiler,
                    *CXX_STRICT_FLAGS,
                    optimization,
                    f"-I{fixture_root / 'positive/include'}",
                    "-c",
                    str(NATIVE / "constructor_header_cpp.cpp"),
                    "-o",
                    str(output / "constructor_header_cpp.o"),
                ],
                label=f"{toolchain.family} {optimization} constructor C++17 header",
            )


def check_native(
    report: dict[str, object], *, requested_toolchain: str = "auto"
) -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-native-") as temporary:
        root = Path(temporary)
        fixture_root = root / "fixture"
        write_generated_fixture(report, fixture_root / "positive")
        compile_failure_fixture(fixture_root / "failure")
        compile_default_fixture(fixture_root / "defaults")
        projects = (
            CFixtureProject(
                "constructor-default-fields",
                ("defaults/src/program.c",),
                ("defaults/include/hxc/program.h",),
                ("defaults/include",),
                "",
                ("constructor-default-field-values", "strict-c11"),
            ),
            CFixtureProject(
                "constructor-failure",
                ("failure/src/program.c",),
                ("failure/include/hxc/program.h",),
                ("failure/include",),
                "",
                ("constructor-cleanup-failure", "strict-c11"),
                expected_exit=-signal.SIGABRT,
            ),
            CFixtureProject(
                "constructor-positive",
                ("positive/src/program.c",),
                ("positive/include/hxc/program.h",),
                ("positive/include",),
                "",
                (
                    "constructor-generated-executable",
                    "constructor-runtime-free",
                    "field-initialization-order",
                    "super-constructor-order",
                    "trivial-constructor-elision",
                ),
            ),
        )
        for optimization in ("-O0", "-O2"):
            native_report = run_c_fixture_corpus(
                suite=f"constructor-lowering-{optimization[1:].lower()}",
                projects=projects,
                fixture_root=fixture_root,
                build_root=root / f"c-build-{optimization[1:].lower()}",
                repository_root=ROOT,
                requested_toolchain=requested_toolchain,
                strict_flags=(*C11_STRICT_FLAGS, optimization),
            )
            validate_report(native_report, required_coverage=REQUIRED_NATIVE_COVERAGE)
            encoded = report_json(native_report, compact=True)
            for forbidden in (str(ROOT), str(fixture_root), str(root)):
                if forbidden in encoded:
                    raise ConstructorLoweringFailure(
                        f"native report leaked absolute path {forbidden}"
                    )
        check_cpp_header(
            fixture_root, root / "cxx-build", requested_toolchain=requested_toolchain
        )


def check_eval_oracle() -> None:
    result = subprocess.run(
        [development_tool("haxe"), "-cp", str(ORACLE), "-main", "Main", "--interp"],
        cwd=ROOT,
        env=haxe_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ConstructorLoweringFailure(
            "pinned Haxe constructor oracle failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def check_minimal_example() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-minimal-") as temporary:
        output = Path(temporary) / "generated"
        result = custom_target(MINIMAL, output)
        if result.returncode != 0 or result.stdout or result.stderr:
            raise ConstructorLoweringFailure(
                "minimal constructor example did not compile cleanly\n"
                f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
            )
        source = (output / "src/program.c").read_text(encoding="utf-8")
        plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
        if (
            "constructor_Item" not in source
            or " = { 0 };" not in source
            or "bounded-stack-construction" not in plan.get("directDecisions", [])
            or plan.get("features") != []
        ):
            raise ConstructorLoweringFailure("minimal constructor example lost direct runtime-free lowering")


def check_negative_cases() -> None:
    with tempfile.TemporaryDirectory(prefix="hxc-constructor-negative-") as temporary:
        root = Path(temporary)
        for directory, detail in NEGATIVE_CASES.items():
            output = root / directory
            result = custom_target(FIXTURES / directory, output)
            combined = result.stdout + result.stderr
            if (
                result.returncode == 0
                or "HXC1001" not in combined
                or detail not in combined
                or generated_files(output)
            ):
                raise ConstructorLoweringFailure(
                    f"{directory} did not fail closed with exact HXC1001 and no output\n"
                    f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
                )
        cycle = custom_target(FIXTURES / "cycle", root / "cycle-repeat")
        if "reachable function and constructor graph discovery" not in cycle.stderr:
            raise ConstructorLoweringFailure("constructor-cycle diagnostic lost graph context")


def snapshot_report() -> dict[str, object]:
    return {
        "schemaVersion": 1,
        "profile": "portable",
        "constructors": json.loads(
            (EXPECTED / "constructors.json").read_text(encoding="utf-8")
        ),
        "hxcir": (EXPECTED / "constructors.hxcir").read_text(encoding="utf-8"),
        "header": (EXPECTED / "program.h").read_text(encoding="utf-8"),
        "source": (EXPECTED / "program.c").read_text(encoding="utf-8"),
        "symbols": json.loads((EXPECTED / "symbols.json").read_text(encoding="utf-8")),
    }


def parse_args(arguments: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(list(arguments))


def main(arguments: Iterable[str] = ()) -> int:
    args = parse_args(arguments)
    if not args.native_only and shutil.which(development_tool("haxe")) is None:
        print("constructor-lowering: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        if args.native_only:
            report = snapshot_report()
            check_native(report, requested_toolchain=args.toolchain)
            print("constructor-lowering: OK: required constructor native matrix passed")
            return 0

        first_payload, first = render("first constructor render")
        second_payload, second = render("second constructor render")
        reverse_payload, reverse = render("reverse-input constructor render", reverse=True)
        _, metal = render("metal constructor render", profile="metal")
        _, runtime_none = render("runtime-none constructor render", runtime="none")
        if first_payload != second_payload or first != second:
            raise ConstructorLoweringFailure("two constructor renders differed")
        if first_payload != reverse_payload or first != reverse:
            raise ConstructorLoweringFailure("constructor output changed with typed-module order")
        validate(first)
        validate(metal, profile="metal")
        validate(runtime_none)
        if normalized_profile(first) != normalized_profile(metal):
            raise ConstructorLoweringFailure("portable and metal constructor output diverged")
        for key in ("constructors", "hxcir", "header", "source", "symbols"):
            if first.get(key) != runtime_none.get(key):
                raise ConstructorLoweringFailure(
                    f"runtime-none changed constructor compiler artifact {key}"
                )
        check_snapshots(first)
        check_eval_oracle()
        check_minimal_example()
        check_native(first, requested_toolchain=args.toolchain)
        check_negative_cases()
    except (
        ConstructorLoweringFailure,
        CFixtureFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"constructor-lowering: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "constructor-lowering: OK: pinned super/field/body order, default storage, "
        "status cleanup, trivial elision, runtime-free strict C11/C++17 consumers, "
        "determinism, and fail-closed escape/cycle/native-layout/generic edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
