#!/usr/bin/env python3
"""Compile and run the exact E2.T10 hello Haxe-to-C vertical slice."""

from __future__ import annotations

import argparse
import difflib
import json
import os
import shutil
import subprocess
import sys
import tempfile
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
BUILD_HXML = CASE / "build.hxml"
ORACLE_HXML = CASE / "oracle.hxml"
EXPECTED = CASE / "expected"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
EXPECTED_STDOUT = b"Hello from hxc\n"
EXPECTED_FEATURES = ["runtime-base", "status", "string-literal", "io"]
EXPECTED_RUNTIME_ARTIFACTS = [
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/io.h",
    "runtime/include/hxrt/status.h",
    "runtime/include/hxrt/string_literal.h",
    "runtime/src/io.c",
]
SNAPSHOT_FORMATS = {
    "hello.hxcir": "hxcir",
    "include/hxc/program.h": "header",
    "src/program.c": "c",
    "hxc.runtime-plan.json": "json",
    "hxc.stdlib-report.json": "json",
}
TOOLCHAINS = ("gcc", "clang")
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic-errors",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)


class HelloFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class RenderedProject:
    output: Path
    hxcir: str
    runtime_plan: dict[str, object]
    stdlib_report: dict[str, object]
    manifest: dict[str, object]


@dataclass(frozen=True)
class NativeToolchain:
    family: str
    compiler: str
    version: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def base_environment(locale: str = "C") -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    environment["LC_ALL"] = locale
    return environment


def compile_target(
    output: Path,
    *,
    caller_cwd: Path = ROOT,
    locale: str = "C",
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "--cwd",
        str(CASE),
        BUILD_HXML.name,
        "-D",
        "hxc_runtime_diagnostics=off",
        "-D",
        "reflaxe_c_static_initialization_report",
        "-D",
        "hxc_project_layout=unity",
        "--custom-target",
        f"c={output}",
    ]
    return subprocess.run(
        command,
        cwd=caller_cwd,
        env=base_environment(locale),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def load_json(path: Path, label: str) -> dict[str, object]:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise HelloFailure(f"{label} must be a JSON object")
    return value


def extract_hxcir(result: subprocess.CompletedProcess[str]) -> str:
    values = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(values) != 1:
        raise HelloFailure(
            "hello compile emitted an invalid inspection envelope: "
            f"{result.stdout!r}"
        )
    report = json.loads(values[0])
    if not isinstance(report, dict) or report.get("schemaVersion") != 1:
        raise HelloFailure("hello compile emitted an invalid inspection report")
    hxcir = report.get("hxcir")
    if not isinstance(hxcir, str):
        raise HelloFailure("hello inspection report omitted its HxcIR dump")
    return hxcir


def text_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise HelloFailure(f"{label} must be a string array")
    return list(value)


def validate_hxcir(hxcir: str) -> None:
    required = (
        "hxcir schema=17",
        'string-utf8(bytes=14,value="Hello from hxc")',
        'runtime(feature="io",operation="sys-println-literal")',
        "failure(kind=native-status,target=abort,arguments=[],cleanup=[])",
    )
    for marker in required:
        if marker not in hxcir:
            raise HelloFailure(f"hello HxcIR omitted {marker!r}")
    if hxcir.count('runtime(feature="io",operation="sys-println-literal")') != 1:
        raise HelloFailure("hello HxcIR must contain exactly one output root")
    if hxcir.count("failure(kind=native-status,target=abort") != 1:
        raise HelloFailure("hello output must retain exactly one fail-stop edge")
    for forbidden in (
        "dynamic",
        'runtime(feature="object"',
        'runtime(feature="gc"',
        'runtime(feature="reflection"',
        'runtime(feature="exception"',
    ):
        if forbidden in hxcir:
            raise HelloFailure(f"hello HxcIR selected forbidden machinery: {forbidden}")


def validate_runtime_plan(plan: dict[str, object]) -> None:
    if (
        plan.get("schemaVersion") != 2
        or plan.get("algorithm") != "hxc-runtime-plan-v2"
        or plan.get("status") != "analyzed-runtime-features"
        or plan.get("planPurpose") != "compiler-program"
        or plan.get("profile") != "portable"
        or plan.get("environment") != "hosted"
        or plan.get("resolvedPolicy") != "auto"
        or plan.get("noRuntimeProof") is not None
    ):
        raise HelloFailure("hello runtime-plan policy drifted")
    if text_list(plan.get("features"), "runtime features") != EXPECTED_FEATURES:
        raise HelloFailure("hello did not select the exact runtime feature closure")
    if (
        text_list(plan.get("artifacts"), "runtime artifacts")
        != EXPECTED_RUNTIME_ARTIFACTS
    ):
        raise HelloFailure("hello did not package the exact runtime artifact closure")

    reasons = plan.get("rootReasons")
    if not isinstance(reasons, list) or len(reasons) != 2:
        raise HelloFailure("hello must retain its output and literal-value reasons")
    expected_reasons = (
        (
            "runtime.io.sys-println-literal.0",
            "io",
            "sys-println-literal",
            "hosted-output",
        ),
        (
            "runtime.string-literal.static-value.1",
            "string-literal",
            "static-value",
            "direct-string-value",
        ),
    )
    for reason, expected in zip(reasons, expected_reasons, strict=True):
        if not isinstance(reason, dict):
            raise HelloFailure("hello runtime root reason must be an object")
        source = reason.get("source")
        start = source.get("start") if isinstance(source, dict) else None
        if (
            tuple(reason.get(key) for key in ("id", "featureId", "operationId", "kind"))
            != expected
            or reason.get("surface") != "Sys.println(String literal)"
            or not isinstance(source, dict)
            or source.get("file") != "Main.hx"
            or not isinstance(start, dict)
            or start.get("line") != 3
        ):
            raise HelloFailure("hello runtime root lost its stable source provenance")

    selected = plan.get("selectedFeatures")
    if not isinstance(selected, list) or len(selected) != len(EXPECTED_FEATURES):
        raise HelloFailure("hello selected runtime feature records are incomplete")
    output_reason = "runtime.io.sys-println-literal.0"
    literal_reason = "runtime.string-literal.static-value.1"
    expected_records = (
        ("runtime-base", [], [output_reason, literal_reason], False),
        ("status", ["runtime-base"], [output_reason], False),
        ("string-literal", ["runtime-base"], [output_reason, literal_reason], True),
        ("io", ["status", "string-literal"], [output_reason], True),
    )
    for value, (identifier, dependencies, reason_ids, root) in zip(
        selected, expected_records, strict=True
    ):
        if (
            not isinstance(value, dict)
            or value.get("id") != identifier
            or value.get("dependencies") != dependencies
            or value.get("reasonIds") != reason_ids
            or value.get("root") is not root
        ):
            raise HelloFailure(
                "every selected hello feature must retain its exact source reasons"
            )

    serialized = json.dumps(plan, ensure_ascii=False, sort_keys=True)
    for forbidden in (
        '"id": "alloc"',
        '"id": "string"',
        '"id": "object"',
        '"id": "gc"',
        '"id": "dynamic"',
        '"id": "reflection"',
        '"id": "exception"',
    ):
        if forbidden in serialized:
            raise HelloFailure(f"hello selected forbidden runtime slice: {forbidden}")
    if str(ROOT) in serialized or "/Users/" in serialized or "\\\\" in serialized:
        raise HelloFailure("hello runtime plan leaked a checkout-local path")


def validate_stdlib_report(report: dict[str, object]) -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "analyzed-selected-stdlib-use"
        or report.get("profile") != "portable"
        or report.get("modules") != ["String", "Sys"]
        or report.get("capabilities") != ["static-value", "sys-println-literal"]
    ):
        raise HelloFailure("hello stdlib report overstated or lost its bounded surface")


def validate_manifest(manifest: dict[str, object]) -> None:
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise HelloFailure("hello manifest omitted its typed build plan")
    if manifest.get("compilationStatus") != "lowered-primitive-executable":
        raise HelloFailure("hello manifest lost its bounded compilation status")
    if (
        build.get("sources") != ["runtime/src/io.c", "src/program.c"]
        or build.get("includeDirectories") != ["include", "runtime/include"]
        or build.get("runtimeHeaders") != EXPECTED_RUNTIME_ARTIFACTS[:4]
    ):
        raise HelloFailure("hello manifest did not consume the exact runtime closure")


def validate_generated_text(source: str, header: str) -> None:
    marker = '(hxc_string){ (const uint8_t *)"Hello from hxc", 14, true }'
    if marker not in source:
        raise HelloFailure("generated hello C omitted its byte-counted String literal")
    if source.count("hxc_io_println(") != 1 or source.count("abort();") != 1:
        raise HelloFailure("generated hello C lost its one checked output call")
    if "#include <hxrt/io.h>" not in header or "#include <stdlib.h>" not in header:
        raise HelloFailure("generated hello header omitted selected typed dependencies")
    for forbidden in (
        "hxc_alloc",
        "hxc_object",
        "hxc_gc",
        "hxc_dynamic",
        "hxc_reflection",
        "hxc_exception",
    ):
        if forbidden in source or forbidden in header:
            raise HelloFailure(f"generated hello C retained forbidden symbol {forbidden}")


def render_project(
    output: Path,
    *,
    caller_cwd: Path = ROOT,
    locale: str = "C",
) -> RenderedProject:
    result = compile_target(output, caller_cwd=caller_cwd, locale=locale)
    if result.returncode != 0 or result.stderr:
        raise HelloFailure(
            "hello C-target compile failed\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    hxcir = extract_hxcir(result)
    runtime_plan = load_json(output / "hxc.runtime-plan.json", "runtime plan")
    stdlib_report = load_json(output / "hxc.stdlib-report.json", "stdlib report")
    manifest = load_json(output / "hxc.manifest.json", "project manifest")
    source = (output / "src/program.c").read_text(encoding="utf-8")
    header = (output / "include/hxc/program.h").read_text(encoding="utf-8")
    validate_hxcir(hxcir)
    validate_runtime_plan(runtime_plan)
    validate_stdlib_report(stdlib_report)
    validate_manifest(manifest)
    validate_generated_text(source, header)
    return RenderedProject(output, hxcir, runtime_plan, stdlib_report, manifest)


def normal_artifacts(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def values_from_rendered(rendered: RenderedProject) -> dict[str, object]:
    return {
        "hello.hxcir": rendered.hxcir,
        "include/hxc/program.h": (
            rendered.output / "include/hxc/program.h"
        ).read_text(encoding="utf-8"),
        "src/program.c": (rendered.output / "src/program.c").read_text(
            encoding="utf-8"
        ),
        "hxc.runtime-plan.json": rendered.runtime_plan,
        "hxc.stdlib-report.json": rendered.stdlib_report,
    }


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-hello-snapshot-") as temporary:
        return values_from_rendered(render_project(Path(temporary) / "out"))


def expected_values() -> dict[str, object]:
    values: dict[str, object] = {}
    for name, format_name in SNAPSHOT_FORMATS.items():
        path = EXPECTED / name
        if not path.is_file():
            raise HelloFailure(
                f"missing {path.relative_to(ROOT)}; run "
                "npm run snapshots:update -- --suite hello"
            )
        values[name] = (
            load_json(path, name)
            if format_name == "json"
            else path.read_text(encoding="utf-8")
        )
    return values


def snapshot_difference(expected: object, actual: object, name: str) -> str:
    if isinstance(expected, dict) and isinstance(actual, dict):
        before = json.dumps(expected, ensure_ascii=False, indent=2, sort_keys=True) + "\n"
        after = json.dumps(actual, ensure_ascii=False, indent=2, sort_keys=True) + "\n"
    else:
        before = str(expected)
        after = str(actual)
    return "".join(
        difflib.unified_diff(
            before.splitlines(keepends=True),
            after.splitlines(keepends=True),
            fromfile=f"expected/{name}",
            tofile=f"actual/{name}",
        )
    )


def validate_snapshots(actual: dict[str, object]) -> None:
    expected = expected_values()
    for name in SNAPSHOT_FORMATS:
        if expected[name] != actual[name]:
            raise HelloFailure(
                "hello generated baseline drifted\n"
                + snapshot_difference(expected[name], actual[name], name)
            )


def validate_expected_baseline(values: dict[str, object]) -> None:
    hxcir = values["hello.hxcir"]
    source = values["src/program.c"]
    header = values["include/hxc/program.h"]
    runtime_plan = values["hxc.runtime-plan.json"]
    stdlib_report = values["hxc.stdlib-report.json"]
    if not isinstance(hxcir, str) or not isinstance(source, str) or not isinstance(header, str):
        raise HelloFailure("hello text baseline has an invalid type")
    if not isinstance(runtime_plan, dict) or not isinstance(stdlib_report, dict):
        raise HelloFailure("hello JSON baseline has an invalid type")
    validate_hxcir(hxcir)
    validate_runtime_plan(runtime_plan)
    validate_stdlib_report(stdlib_report)
    validate_generated_text(source, header)


def run_eval_oracle() -> None:
    result = subprocess.run(
        [
            development_tool("haxe"),
            "--cwd",
            str(CASE),
            ORACLE_HXML.name,
        ],
        cwd=ROOT,
        env=base_environment(),
        check=False,
        capture_output=True,
        timeout=60,
    )
    if result.returncode != 0 or result.stdout != EXPECTED_STDOUT or result.stderr:
        raise HelloFailure(
            "Haxe Eval hello oracle drifted: "
            f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
        )


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run(
        [executable, "--version"],
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise HelloFailure(f"cannot identify native compiler {executable}")
    output = (result.stdout + result.stderr).strip()
    lowered = output.lower()
    family = (
        "clang"
        if "clang" in lowered
        else "gcc"
        if "gcc" in lowered or "free software foundation" in lowered
        else "unknown"
    )
    return family, output.splitlines()[0] if output else "unknown version"


def resolve_toolchain(family: str, required: bool) -> NativeToolchain | None:
    executable = shutil.which(family)
    if executable is None:
        if required:
            raise HelloFailure(f"required native compiler is missing: {family}")
        print(f"hello-example: SKIP optional {family}: missing command")
        return None
    actual, version = compiler_identity(executable)
    if actual != family:
        if required:
            raise HelloFailure(
                f"required {family} identifies as {actual}: {version}"
            )
        print(
            f"hello-example: SKIP optional {family}: command identifies as {actual}"
        )
        return None
    return NativeToolchain(family, executable, version)


def selected_toolchains(selected: str) -> list[NativeToolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    values = [
        toolchain
        for family in families
        if (toolchain := resolve_toolchain(family, required=selected != "auto"))
        is not None
    ]
    if not values:
        raise HelloFailure("no identity-matching strict C11 compiler is available")
    return values


def safe_project_path(output: Path, value: str) -> Path:
    path = Path(value)
    if (
        not value
        or path.is_absolute()
        or "\\" in value
        or any(part in ("", ".", "..") for part in path.parts)
    ):
        raise HelloFailure(f"generated build plan contains an unsafe path: {value!r}")
    resolved = (output / path).resolve()
    resolved.relative_to(output.resolve())
    if not resolved.is_file():
        raise HelloFailure(f"generated build input is missing: {value}")
    return resolved


def safe_project_directory(output: Path, value: str) -> Path:
    path = Path(value)
    if (
        not value
        or path.is_absolute()
        or "\\" in value
        or any(part in ("", ".", "..") for part in path.parts)
    ):
        raise HelloFailure(
            f"generated build plan contains an unsafe directory: {value!r}"
        )
    resolved = (output / path).resolve()
    resolved.relative_to(output.resolve())
    if not resolved.is_dir():
        raise HelloFailure(f"generated include directory is missing: {value}")
    return resolved


def compile_command(
    toolchain: NativeToolchain,
    optimization: str,
    sources: list[Path],
    includes: list[Path],
    executable: Path,
) -> None:
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        f"-{optimization}",
        *(f"-I{include}" for include in includes),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    result = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise HelloFailure(
            f"{toolchain.family} {optimization} hello compile failed\n"
            f"command={command!r}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def run_executable(executable: Path, label: str) -> None:
    result = subprocess.run(
        [str(executable)],
        cwd=executable.parent,
        check=False,
        capture_output=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != EXPECTED_STDOUT or result.stderr:
        raise HelloFailure(
            f"{label} stdout drifted: exit={result.returncode} "
            f"stdout={result.stdout!r} stderr={result.stderr!r}"
        )


def build_generated(
    toolchain: NativeToolchain,
    rendered: RenderedProject,
    optimization: str,
    build: Path,
) -> None:
    build_plan = rendered.manifest.get("build")
    if not isinstance(build_plan, dict):
        raise HelloFailure("generated hello manifest lost its build plan")
    sources = [
        safe_project_path(rendered.output, value)
        for value in text_list(build_plan.get("sources"), "build sources")
    ]
    includes = [
        safe_project_directory(rendered.output, value)
        for value in text_list(
            build_plan.get("includeDirectories"), "include directories"
        )
    ]
    executable = build / f"{toolchain.family}-{optimization}-generated"
    compile_command(toolchain, optimization, sources, includes, executable)
    run_executable(executable, f"{toolchain.family} {optimization} generated hello")


def build_expected(
    toolchain: NativeToolchain,
    optimization: str,
    build: Path,
) -> None:
    sources = [
        EXPECTED / "src/program.c",
        ROOT / "runtime/hxrt/src/io.c",
    ]
    includes = [
        EXPECTED / "include",
        ROOT / "runtime/hxrt/include",
    ]
    for path in (*sources, *includes):
        if not path.exists():
            raise HelloFailure(f"native hello baseline input is missing: {path}")
    executable = build / f"{toolchain.family}-{optimization}-baseline"
    compile_command(toolchain, optimization, sources, includes, executable)
    run_executable(executable, f"{toolchain.family} {optimization} baseline hello")


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="compile the checked-in generated baseline without requiring Haxe",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    toolchains: list[NativeToolchain] = []
    try:
        toolchains = selected_toolchains(args.toolchain)
        baseline = expected_values()
        validate_expected_baseline(baseline)
        with tempfile.TemporaryDirectory(prefix="reflaxe-c-hello-") as temporary:
            root = Path(temporary)
            native = root / "native"
            native.mkdir()
            if args.native_only:
                for toolchain in toolchains:
                    for optimization in ("O0", "O2"):
                        build_expected(toolchain, optimization, native)
            else:
                run_eval_oracle()
                caller = root / "unrelated-caller"
                caller.mkdir()
                first = render_project(root / "first", caller_cwd=ROOT, locale="C")
                repeated = render_project(
                    root / "repeated", caller_cwd=caller, locale="C.UTF-8"
                )
                if normal_artifacts(first.output) != normal_artifacts(repeated.output):
                    raise HelloFailure(
                        "hello artifacts changed across output roots, caller working "
                        "directories, or locales"
                    )
                validate_snapshots(values_from_rendered(first))
                for toolchain in toolchains:
                    for optimization in ("O0", "O2"):
                        build_generated(toolchain, first, optimization, native)
    except (
        OSError,
        UnicodeError,
        ValueError,
        json.JSONDecodeError,
        subprocess.TimeoutExpired,
        HelloFailure,
    ) as error:
        print(f"hello-example: ERROR: {error}", file=sys.stderr)
        return 1

    families = ", ".join(toolchain.family for toolchain in toolchains)
    mode = "checked-in baseline" if args.native_only else "Eval/C differential"
    print(
        "hello-example: OK: "
        f"{families} {mode}, exact Hello from hxc\\n stdout, deterministic "
        "four-feature plan, and warning-clean strict C11 O0/O2 builds passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
