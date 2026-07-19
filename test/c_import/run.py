#!/usr/bin/env python3
"""Prove the reusable, runtime-free direct C-import vertical slice."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
FIXTURES = CASE / "fixtures"
POSITIVE = FIXTURES / "positive"
NATIVE = ROOT / "test/native/pointlib"
ABI_PROBE = CASE / "native/abi_probe.c"
EXPECTED = CASE / "expected"
PRODUCTION_FILES = {
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
NEGATIVE_CASES = {
    "callback_return": (
        "HXC3000",
        "CallbackApi.hx:4",
        "Callbacks require the later typed function-pointer and context-lifetime contract.",
    ),
    "embedded_nul": (
        "HXC3000",
        "Main.hx:3",
        "contains an embedded NUL byte",
    ),
    "inferred_keyword": (
        "HXC5002",
        "InferredKeywordApi.hx:3",
        "C identifier `goto` is reserved",
    ),
    "nonliteral_cstring": (
        "HXC3000",
        "Main.hx:3",
        "requires a direct String literal so its borrowed lifetime is static",
    ),
    "pointer_return": (
        "HXC3000",
        "PointerApi.hx:4",
        "Pointer and retained-borrow lifetimes are outside this direct by-value slice.",
    ),
    "preprocessor_define": (
        "HXC3000",
        "DefineApi.hx:",
        "Preprocessor definitions require an exact configuration/ABI identity",
    ),
    "variadic": (
        "HXC3000",
        "VariadicApi.hx:4",
        "Variadic functions are outside the admitted direct slice.",
    ),
}
TYPING_NEGATIVE_CASES = {
    "implicit_float32": (
        "Main.hx:3",
        "Float should be c.Float32",
    ),
}
REQUIRED_COVERAGE = frozenset(
    {
        "allocation-free-cstring",
        "compiled-abi-layout",
        "compiled-constant-values",
        "direct-native-calls",
        "float32-abi",
        "float32-conversions",
        "generated-haxe-program",
        "header-owned-structs",
        "runtime-free",
        "strict-c11",
    }
)

if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CFixtureFailure,
    CFixtureProject,
    report_json,
    run_c_fixture_corpus,
    validate_report,
)


class CImportFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    artifacts: dict[str, bytes]
    ownership: dict[str, object]
    header: str
    source: str
    manifest: dict[str, object]
    runtime_plan: dict[str, object]
    symbols: dict[str, object]


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(locale: str = "C") -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    environment["LC_ALL"] = locale
    return environment


def compile_fixture(
    fixture: Path,
    output: Path,
    *,
    reverse: bool = False,
    locale: str = "C",
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "--cwd",
        str(fixture),
        "build.hxml",
        "-D",
        "hxc_runtime_diagnostics=off",
    ]
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    command.extend(["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(locale),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def generated_files(root: Path) -> set[str]:
    if not root.exists():
        return set()
    return {
        path.relative_to(root).as_posix()
        for path in root.rglob("*")
        if path.is_file()
    }


def read_json(path: Path, label: str) -> dict[str, object]:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise CImportFailure(f"{label} is not a JSON object")
    return value


def read_project(output: Path) -> RenderedProject:
    files = generated_files(output)
    if files != PRODUCTION_FILES | {"_GeneratedFiles.json"}:
        raise CImportFailure(f"C-import artifact set drifted: {sorted(files)!r}")
    artifacts = {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }
    ownership = read_json(output / "_GeneratedFiles.json", "ownership metadata")
    if ownership.get("version") != 1 or ownership.get("filesGenerated") != sorted(
        PRODUCTION_FILES, key=lambda value: value.encode("utf-8")
    ):
        raise CImportFailure("Reflaxe ownership metadata lost its complete path set")
    return RenderedProject(
        artifacts,
        ownership,
        (output / "include/hxc/program.h").read_text(encoding="utf-8"),
        (output / "src/program.c").read_text(encoding="utf-8"),
        read_json(output / "hxc.manifest.json", "compiler manifest"),
        read_json(output / "hxc.runtime-plan.json", "runtime plan"),
        read_json(output / "hxc.symbols.json", "symbol table"),
    )


def read_native_snapshot() -> RenderedProject:
    header_path = EXPECTED / "program.h"
    source_path = EXPECTED / "program.c"
    build_path = EXPECTED / "build.json"
    runtime_path = EXPECTED / "runtime-plan.json"
    for path in (header_path, source_path, build_path, runtime_path):
        if not path.is_file():
            raise CImportFailure(
                "C-import native snapshots are missing; run "
                "`npm run snapshots:update -- --suite c-import`"
            )
    header = header_path.read_text(encoding="utf-8")
    source = source_path.read_text(encoding="utf-8")
    return RenderedProject(
        {"program.h": header.encode("utf-8"), "program.c": source.encode("utf-8")},
        {},
        header,
        source,
        {"build": read_json(build_path, "native build snapshot")},
        read_json(runtime_path, "native runtime-plan snapshot"),
        {},
    )


def render(label: str, output: Path, *, reverse: bool = False, locale: str = "C") -> RenderedProject:
    result = compile_fixture(POSITIVE, output, reverse=reverse, locale=locale)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise CImportFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    return read_project(output)


def alternate_locale() -> str:
    result = subprocess.run(
        ["locale", "-a"], check=False, capture_output=True, text=True, timeout=10
    )
    available = {line.strip() for line in result.stdout.splitlines()}
    for candidate in ("C.UTF-8", "C.utf8", "en_US.UTF-8", "en_US.utf8"):
        if candidate in available:
            return candidate
    return "C"


def first_byte_difference(left: bytes, right: bytes) -> int:
    limit = min(len(left), len(right))
    for index in range(limit):
        if left[index] != right[index]:
            return index
    return limit


def assert_artifacts_equal(
    expected: dict[str, bytes], actual: dict[str, bytes], label: str
) -> None:
    if set(expected) != set(actual):
        raise CImportFailure(f"{label} artifact paths differed")
    for path in sorted(expected, key=lambda value: value.encode("utf-8")):
        if expected[path] != actual[path]:
            offset = first_byte_difference(expected[path], actual[path])
            raise CImportFailure(
                f"{label} first differed in {path!r} at byte offset {offset}"
            )


def require_dict(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise CImportFailure(f"{label} is not an object")
    return value


def require_list(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise CImportFailure(f"{label} is not an array")
    return value


def validate_positive(project: RenderedProject) -> None:
    header = project.header
    source = project.source
    if header.count('#include "pointlib.h"') != 1:
        raise CImportFailure("generated header did not include pointlib.h exactly once")
    for spelling in (
        "#include <float.h>",
        "#include <limits.h>",
        "CHAR_BIT == 8",
        "sizeof(float) * CHAR_BIT == 32",
        "FLT_MANT_DIG == 24",
        "FLT_HAS_SUBNORM == 1",
    ):
        if spelling not in header:
            raise CImportFailure(
                f"generated Float32 carrier omitted target guard {spelling!r}"
            )
    for spelling in (
        "pointlib_point_make(",
        "pointlib_point_translate(",
        "pointlib_point_alias_identity(",
        "pointlib_point_dot(",
        "pointlib_point_component(",
        "pointlib_point_verify(",
        "pointlib_float_point_make(",
        "pointlib_float_point_scale(",
        "pointlib_float_point_dot(",
        "pointlib_float32_verify(",
        "struct pointlib_point",
        "struct pointlib_float_point",
        "pointlib_coord",
        "pointlib_point_alias",
        "pointlib_axis",
        "POINTLIB_AXIS_Y",
        '"c-import-',
        "\\303\\251",
    ):
        if spelling not in source:
            raise CImportFailure(f"generated C omitted direct import spelling {spelling!r}")
    forbidden = (
        "malloc(",
        "calloc(",
        "realloc(",
        "free(",
        "hxrt",
        "__c__",
        "Reflect",
        "typedef struct pointlib_point",
        "struct pointlib_point\n{",
        "raylib",
        "unused_pointlib",
        "UnusedPointlib",
    )
    for spelling in forbidden:
        if spelling.lower() in (header + source).lower():
            raise CImportFailure(f"generated import path leaked forbidden spelling {spelling!r}")
    for spelling in (
        ".x = POINTLIB_COORD_ONE;",
        " = &hxc_local_Main_main_left_n0.x;",
        " = *hxc_temp_Main_main_importedzx2Dfieldzx2Daddress",
        "pointlib_axis hxc_local_Main_main_axis",
        "float hxc_local_Main_main_floatDot",
        "struct pointlib_point hxc_local_Main_main_pointAlias",
        "double hxc_local_Main_main_widened",
    ):
        if spelling not in source:
            raise CImportFailure(
                f"generated C omitted structural imported-field/enum evidence {spelling!r}"
            )

    manifest = project.manifest
    if (
        manifest.get("schemaVersion") != 1
        or manifest.get("compilationStatus") != "lowered-direct-value-executable"
    ):
        raise CImportFailure("compiler manifest lost its bounded direct-value status")
    build = require_dict(manifest.get("build"), "neutral build plan")
    if build.get("requiredHeaders") != [
        {
            "path": "pointlib.h",
            "ownerModulePaths": [
                "Axis",
                "Coord",
                "FloatPoint",
                "Point",
                "PointAlias",
                "PointLib",
            ],
            "kind": "local",
        }
    ] or build.get("libraries") != [
        {"ownerModulePaths": ["PointLib"], "name": "pointlib"}
    ]:
        raise CImportFailure("header/library facts were not deduplicated with provenance")
    if build.get("pkgConfigPackages") != [] or build.get("frameworks") != []:
        raise CImportFailure("unused package/framework facts leaked into the build plan")
    cmake = project.artifacts["cmake/CMakeLists.txt"].decode("utf-8")
    meson = project.artifacts["meson.build"].decode("utf-8")
    if cmake.count("[[pointlib]]") != 1 or meson.count("find_library('pointlib'") != 1:
        raise CImportFailure("build adapters did not derive the pointlib link exactly once")

    runtime = project.runtime_plan
    if (
        runtime.get("schemaVersion") != 2
        or runtime.get("status") != "analyzed-runtime-free"
        or runtime.get("features") != []
        or runtime.get("artifacts") != []
        or runtime.get("symbols") != []
        or runtime.get("libraries") != []
        or "typed-header-owned-c-imports"
        not in require_list(runtime.get("directDecisions"), "direct decisions")
    ):
        raise CImportFailure("C imports did not retain a complete runtime-free proof")
    proof = require_dict(runtime.get("noRuntimeProof"), "no-runtime proof")
    absence = require_dict(proof.get("runtimeAbsence"), "runtime absence proof")
    if any(absence.get(key) != [] for key in ("sources", "features", "symbols", "libraries", "includes", "defines")):
        raise CImportFailure("no-runtime proof contains an hxrt dependency")

    symbols = require_list(project.symbols.get("symbols"), "symbol table")
    external = {
        entry.get("cName")
        for entry in symbols
        if isinstance(entry, dict) and entry.get("visibility") == "external"
    }
    for spelling in (
        "pointlib_axis",
        "pointlib_coord",
        "pointlib_point",
        "pointlib_point_alias",
        "pointlib_build_fact_probe",
        "pointlib_point_make",
        "pointlib_point_translate",
        "pointlib_point_alias_identity",
        "pointlib_point_dot",
        "pointlib_point_component",
        "pointlib_point_verify",
        "pointlib_float_point",
        "pointlib_float_point_make",
        "pointlib_float_point_scale",
        "pointlib_float_point_dot",
        "pointlib_float32_verify",
    ):
        if spelling not in external:
            raise CImportFailure(f"symbol table omitted exact external name {spelling!r}")


def check_determinism(root: Path) -> RenderedProject:
    first = render("first C-import render", root / "first")
    repeated = render("repeated C-import render", root / "repeated")
    reversed_project = render(
        "reverse-input/locale C-import render",
        root / "reversed",
        reverse=True,
        locale=alternate_locale(),
    )
    assert_artifacts_equal(first.artifacts, repeated.artifacts, "repeated cold render")
    assert_artifacts_equal(
        first.artifacts,
        reversed_project.artifacts,
        "reverse-input/locale render",
    )
    validate_positive(first)
    return first


def check_reached_build_facts(root: Path) -> None:
    fixture = FIXTURES / "build_facts"
    result = compile_fixture(fixture, root)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise CImportFailure(
            f"reached build-fact render failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    project = read_project(root)
    build = require_dict(project.manifest.get("build"), "reached build-fact plan")
    owner = ["BuildFactApi"]
    if build.get("requiredHeaders") != [
        {"path": "pointlib.h", "ownerModulePaths": owner, "kind": "local"}
    ]:
        raise CImportFailure("reached include fact lost its exact source owner")
    for key, name in (
        ("libraries", "pointlib"),
        ("pkgConfigPackages", "pointlib-fixture"),
        ("frameworks", "PointlibFixture"),
    ):
        if build.get(key) != [{"ownerModulePaths": owner, "name": name}]:
            raise CImportFailure(f"reached {key} fact lost its exact source owner")
    runtime = project.runtime_plan
    if (
        runtime.get("status") != "analyzed-runtime-free"
        or runtime.get("features") != []
        or runtime.get("artifacts") != []
        or runtime.get("symbols") != []
        or "typed-header-owned-c-imports"
        not in require_list(runtime.get("directDecisions"), "build-fact decisions")
    ):
        raise CImportFailure("reached build facts did not remain runtime-free")
    if "hxrt" in (project.header + project.source).lower():
        raise CImportFailure("build-only import facts selected hxrt output")
    for unexpected_float32_fact in (
        "#include <float.h>",
        "#include <limits.h>",
        "FLT_MANT_DIG",
        "FLT_HAS_SUBNORM",
    ):
        if unexpected_float32_fact in project.header + project.source:
            raise CImportFailure(
                "a project without c.Float32 reachability emitted the conditional "
                f"binary32 fact {unexpected_float32_fact!r}"
            )


def check_negative_cases(root: Path) -> None:
    for case_name in sorted(NEGATIVE_CASES, key=lambda value: value.encode("utf-8")):
        diagnostic_id, source_marker, detail = NEGATIVE_CASES[case_name]
        output = root / case_name
        result = compile_fixture(FIXTURES / case_name, output)
        combined = result.stdout + result.stderr
        if (
            result.returncode == 0
            or diagnostic_id not in combined
            or source_marker not in combined
            or detail not in combined
            or generated_files(output)
        ):
            raise CImportFailure(
                f"{case_name} did not fail closed at its Haxe ABI span\n"
                f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
            )
    for case_name in sorted(TYPING_NEGATIVE_CASES, key=lambda value: value.encode("utf-8")):
        source_marker, detail = TYPING_NEGATIVE_CASES[case_name]
        output = root / case_name
        result = compile_fixture(FIXTURES / case_name, output)
        combined = result.stdout + result.stderr
        if (
            result.returncode == 0
            or source_marker not in combined
            or detail not in combined
            or generated_files(output)
        ):
            raise CImportFailure(
                f"{case_name} did not reject an implicit lossy Float32 conversion during Haxe typing\n"
                f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
            )


def write_native_fixture(project: RenderedProject, root: Path) -> None:
    generated_header = root / "generated/include/hxc/program.h"
    generated_source = root / "generated/src/program.c"
    native_header = root / "native/include/pointlib.h"
    native_source = root / "native/src/pointlib.c"
    native_probe = root / "native/abi_probe.c"
    for path in (
        generated_header,
        generated_source,
        native_header,
        native_source,
        native_probe,
    ):
        path.parent.mkdir(parents=True, exist_ok=True)
    generated_header.write_text(project.header, encoding="utf-8", newline="\n")
    generated_source.write_text(project.source, encoding="utf-8", newline="\n")
    shutil.copyfile(NATIVE / "include/pointlib.h", native_header)
    shutil.copyfile(NATIVE / "src/pointlib.c", native_source)
    shutil.copyfile(ABI_PROBE, native_probe)


def check_native(
    project: RenderedProject, root: Path, *, requested_toolchain: str
) -> None:
    fixture_root = root / "fixture"
    write_native_fixture(project, fixture_root)
    projects = (
        CFixtureProject(
            "abi-probe",
            ("native/abi_probe.c", "native/src/pointlib.c"),
            ("native/include/pointlib.h",),
            ("native/include",),
            "pointlib-abi: OK\n",
            ("compiled-abi-layout", "compiled-constant-values", "float32-abi", "strict-c11"),
        ),
        CFixtureProject(
            "generated-program",
            ("generated/src/program.c", "native/src/pointlib.c"),
            (
                "generated/include/hxc/program.h",
                "native/include/pointlib.h",
            ),
            ("generated/include", "native/include"),
            "",
            (
                "allocation-free-cstring",
                "direct-native-calls",
                "float32-conversions",
                "generated-haxe-program",
                "header-owned-structs",
                "runtime-free",
            ),
        ),
    )
    for optimization in ("-O0", "-O2"):
        report = run_c_fixture_corpus(
            suite=f"c-import-{optimization[1:].lower()}",
            projects=projects,
            fixture_root=fixture_root,
            build_root=root / f"build-{optimization[1:].lower()}",
            repository_root=ROOT,
            requested_toolchain=requested_toolchain,
            strict_flags=(*C11_STRICT_FLAGS, optimization),
        )
        validate_report(report, required_coverage=REQUIRED_COVERAGE)
        encoded = report_json(report, compact=True)
        for forbidden in (str(ROOT), str(fixture_root), str(root)):
            if forbidden in encoded:
                raise CImportFailure(
                    f"native invocation report leaked absolute path {forbidden}"
                )


def snapshot_values(project: RenderedProject) -> dict[str, object]:
    return {
        "program.h": project.header,
        "program.c": project.source,
        "build.json": require_dict(project.manifest.get("build"), "build snapshot"),
        "runtime-plan.json": project.runtime_plan,
    }


def render_snapshot() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-c-import-snapshot-") as temporary:
        return snapshot_values(check_determinism(Path(temporary)))


def difference(expected: str, actual: str, name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def check_snapshots(project: RenderedProject) -> None:
    for name, actual in snapshot_values(project).items():
        path = EXPECTED / name
        if not path.is_file():
            raise CImportFailure(
                "C-import snapshots are missing; run "
                "`npm run snapshots:update -- --suite c-import`"
            )
        if name.endswith(".json"):
            if json.loads(path.read_text(encoding="utf-8")) != actual:
                raise CImportFailure(f"{name} semantic snapshot drifted")
        else:
            expected = path.read_text(encoding="utf-8")
            if expected != actual:
                raise CImportFailure(
                    f"{name} drifted:\n" + difference(expected, str(actual), name)
                )


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "gcc", "clang"), default="auto")
    parser.add_argument("--native-only", action="store_true")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    args = parse_args(argv)
    try:
        with tempfile.TemporaryDirectory(prefix="hxc-c-import-") as temporary:
            root = Path(temporary)
            if args.native_only:
                project = read_native_snapshot()
            else:
                project = check_determinism(root / "renders")
                check_snapshots(project)
                check_reached_build_facts(root / "build-facts")
                check_negative_cases(root / "negative")
            check_native(project, root / "native", requested_toolchain=args.toolchain)
    except (
        CFixtureFailure,
        CImportFailure,
        OSError,
        UnicodeError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"c-import: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "c-import: OK: direct scalar/enum/typedef/by-value struct calls, compiled ABI "
        "probes, explicit Float32 narrowing/widening, static CString borrowing, deterministic build facts, runtime absence, "
        "and fail-closed ABI edges passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
