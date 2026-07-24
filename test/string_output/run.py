#!/usr/bin/env python3
"""Prove literal UTF-8 output lowering, selective hxrt packaging, and failure policy."""

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
FIXTURES = CASE / "fixtures"
POSITIVE = FIXTURES / "positive"
EXPECTED = CASE / "expected"
RUNTIME_CATALOG = ROOT / "runtime/hxrt/features.json"
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
TOOLCHAINS = ("gcc", "clang")
EXPECTED_STDOUT = b"ASCII\n" + "é🙂\n".encode() + b"embedded\x00NUL\nMain.hx:9: traced\n"
EXPECTED_FEATURES = ["runtime-base", "status", "string-literal", "io"]
EXPECTED_RUNTIME_ARTIFACTS = [
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/io.h",
    "runtime/include/hxrt/status.h",
    "runtime/include/hxrt/string_literal.h",
    "runtime/src/io.c",
]
ABI_ASSERTION = (
    "typedef char hxc_runtime_abi_major_must_match["
    "HXC_RUNTIME_ABI_MAJOR == 0U ? 1 : -1];"
)
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


class StringOutputFailure(RuntimeError):
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


def base_environment() -> dict[str, str]:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    return environment


def compile_target(
    fixture: Path,
    output: Path,
    *,
    profile: str = "portable",
    runtime: str | None = None,
    diagnostics: str | None = "off",
    environment: str = "hosted",
    report: bool = False,
    cwd: Path = ROOT,
) -> subprocess.CompletedProcess[str]:
    command = [
        development_tool("haxe"),
        "-cp",
        str(fixture.resolve()),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main",
    ]
    if profile == "metal":
        command.extend(["-D", "reflaxe_c_profile=metal"])
    elif profile != "portable":
        raise StringOutputFailure(f"unknown profile {profile!r}")
    if runtime is not None:
        command.extend(["-D", f"hxc_runtime={runtime}"])
    if diagnostics is not None:
        command.extend(["-D", f"hxc_runtime_diagnostics={diagnostics}"])
    if environment != "hosted":
        command.extend(["-D", f"hxc_environment={environment}"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    command.extend(["-D", "hxc_project_layout=unity", "--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=cwd,
        env=base_environment(),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def load_json(path: Path, label: str) -> dict[str, object]:
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise StringOutputFailure(f"{label} must be a JSON object")
    return value


def extract_hxcir(result: subprocess.CompletedProcess[str], label: str) -> str:
    values = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(values) != 1:
        raise StringOutputFailure(f"{label} emitted an invalid inspection envelope: {result.stdout!r}")
    report = json.loads(values[0])
    if not isinstance(report, dict) or report.get("schemaVersion") != 1:
        raise StringOutputFailure(f"{label} emitted an invalid inspection report")
    hxcir = report.get("hxcir")
    if not isinstance(hxcir, str):
        raise StringOutputFailure(f"{label} omitted its HxcIR dump")
    return hxcir


def validate_hxcir(hxcir: str) -> None:
    required = (
        "hxcir schema=18",
        'string-utf8(bytes=5,value="ASCII")',
        'string-utf8(bytes=6,value="é🙂")',
        'string-utf8(bytes=12,value="embedded\\u0000NUL")',
        'string-utf8(bytes=8,value="fallback")',
        'string-utf8(bytes=17,value="Main.hx:9: traced")',
        "declare-uninitialized",
        'runtime(feature="io",operation="sys-println-literal")',
        'runtime(feature="io",operation="trace-literal")',
        "failure(kind=native-status,target=abort,arguments=[],cleanup=[])",
    )
    for marker in required:
        if marker not in hxcir:
            raise StringOutputFailure(f"literal-output HxcIR omitted {marker!r}")
    if hxcir.count('runtime(feature="io",operation=') != 5:
        raise StringOutputFailure("literal-output HxcIR must retain exactly five output roots")
    if hxcir.count("failure(kind=native-status,target=abort") != 5:
        raise StringOutputFailure("every literal output call must retain its fail-stop edge")
    for forbidden in ("dynamic", 'runtime(feature="object"', 'runtime(feature="gc"', 'runtime(feature="reflection"'):
        if forbidden in hxcir:
            raise StringOutputFailure(f"literal output selected forbidden HxcIR machinery: {forbidden}")


def text_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise StringOutputFailure(f"{label} must be a string array")
    return list(value)


def validate_runtime_plan(plan: dict[str, object], *, profile: str, policy: str) -> None:
    if (
        plan.get("schemaVersion") != 2
        or plan.get("algorithm") != "hxc-runtime-plan-v2"
        or plan.get("status") != "analyzed-runtime-features"
        or plan.get("planPurpose") != "compiler-program"
        or plan.get("profile") != profile
        or plan.get("environment") != "hosted"
        or plan.get("resolvedPolicy") != policy
        or plan.get("noRuntimeProof") is not None
    ):
        raise StringOutputFailure(f"{profile} literal-output runtime-plan policy drifted")
    if text_list(plan.get("features"), "runtime features") != EXPECTED_FEATURES:
        raise StringOutputFailure("literal output did not select the exact dependency-ordered feature closure")
    if text_list(plan.get("artifacts"), "runtime artifacts") != EXPECTED_RUNTIME_ARTIFACTS:
        raise StringOutputFailure("literal output did not package the exact dependency-closed runtime artifacts")
    reasons = plan.get("rootReasons")
    if not isinstance(reasons, list) or len(reasons) != 14:
        raise StringOutputFailure("literal output must retain five output and nine String-storage reasons")
    expected_ids = [
        "runtime.io.sys-println-literal.0",
        "runtime.io.sys-println-literal.1",
        "runtime.io.sys-println-literal.2",
        "runtime.io.sys-println-literal.3",
        "runtime.io.trace-literal.4",
        "runtime.string-literal.static-value.10",
        "runtime.string-literal.static-value.11",
        "runtime.string-literal.static-value.12",
        "runtime.string-literal.static-value.13",
        "runtime.string-literal.static-value.5",
        "runtime.string-literal.static-value.6",
        "runtime.string-literal.static-value.7",
        "runtime.string-literal.static-value.8",
        "runtime.string-literal.static-value.9",
    ]
    expected_operations = ["sys-println-literal"] * 4 + ["trace-literal"] + ["static-value"] * 9
    actual_ids: list[str] = []
    for index, value in enumerate(reasons):
        if not isinstance(value, dict):
            raise StringOutputFailure("runtime root reason must be an object")
        source = value.get("source")
        if not isinstance(source, dict) or source.get("file") != "Main.hx":
            raise StringOutputFailure("runtime root reason lost its stable logical source")
        expected_feature = "io" if index < 5 else "string-literal"
        expected_kind = "hosted-output" if index < 5 else "direct-string-value"
        if (
            value.get("featureId") != expected_feature
            or value.get("kind") != expected_kind
            or value.get("operationId") != expected_operations[index]
        ):
            raise StringOutputFailure("runtime root reason lost its typed output provenance")
        identifier = value.get("id")
        if not isinstance(identifier, str):
            raise StringOutputFailure(f"runtime root reason {index} lost its stable ID")
        actual_ids.append(identifier)
    if actual_ids != expected_ids:
        raise StringOutputFailure("runtime output root IDs are not deterministic")
    selected = plan.get("selectedFeatures")
    if not isinstance(selected, list) or [entry.get("id") for entry in selected if isinstance(entry, dict)] != EXPECTED_FEATURES:
        raise StringOutputFailure("selected runtime feature records are incomplete")
    roots = [entry.get("id") for entry in selected if isinstance(entry, dict) and entry.get("root") is True]
    if roots != ["string-literal", "io"]:
        raise StringOutputFailure("literal values and io must be the only output root features")
    serialized = json.dumps(plan, ensure_ascii=False, sort_keys=True)
    for forbidden in ('"id": "object"', '"id": "gc"', '"id": "dynamic"', '"id": "reflection"', '"id": "exception"'):
        if forbidden in serialized:
            raise StringOutputFailure(f"literal output selected forbidden runtime machinery: {forbidden}")
    if str(ROOT) in serialized or "/Users/" in serialized or "\\\\" in serialized:
        raise StringOutputFailure("literal-output runtime plan leaked a checkout-local path")


def validate_stdlib_report(report: dict[str, object]) -> None:
    if (
        report.get("schemaVersion") != 1
        or report.get("status") != "analyzed-selected-stdlib-use"
        or report.get("profile") not in ("portable", "metal")
        or report.get("modules") != ["String", "Sys", "haxe.Log"]
        or report.get("capabilities") != ["static-value", "sys-println-literal", "trace-literal"]
    ):
        raise StringOutputFailure("literal-output stdlib report overstated or lost its bounded surface")


def validate_manifest(manifest: dict[str, object]) -> None:
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise StringOutputFailure("generated manifest omitted its build plan")
    if manifest.get("compilationStatus") != "lowered-primitive-executable":
        raise StringOutputFailure("literal-output project did not retain its bounded compilation status")
    expected_sources = [
        "runtime/src/io.c",
        "src/program.c",
    ]
    if build.get("sources") != expected_sources or build.get("includeDirectories") != ["include", "runtime/include"]:
        raise StringOutputFailure("generated build plan did not consume the exact selected runtime closure")
    if build.get("runtimeHeaders") != EXPECTED_RUNTIME_ARTIFACTS[:4]:
        raise StringOutputFailure("generated build plan runtime header closure drifted")
    if build.get("publicHeaders") != []:
        raise StringOutputFailure("primitive runtime-using output unexpectedly exposed a public header")


def validate_public_abi_boundary(abi: dict[str, object]) -> None:
    if (
        abi.get("schemaVersion") != 1
        or abi.get("status") != "analyzed-no-public-exports"
        or abi.get("exports") != []
        or abi.get("types") != []
        or abi.get("executableEntryPoint") != "main"
    ):
        raise StringOutputFailure("primitive runtime-using output overstated its public ABI")
    catalog = load_json(RUNTIME_CATALOG, "runtime feature catalog")
    runtime_abi = catalog.get("runtimeAbi")
    if not isinstance(runtime_abi, dict):
        raise StringOutputFailure("runtime feature catalog omitted its ABI contract")
    public_boundary = runtime_abi.get("publicBoundary")
    if not isinstance(public_boundary, dict):
        raise StringOutputFailure("runtime feature catalog omitted its public boundary")
    forbidden = set(text_list(public_boundary.get("forbiddenRuntimeTypes"), "forbidden runtime export types"))
    exposed = set(text_list(abi.get("types"), "generated public ABI types"))
    if exposed & forbidden:
        raise StringOutputFailure("generated public ABI exposed an unstable runtime struct")


def validate_generated_c(output: Path) -> None:
    source = (output / "src/program.c").read_text(encoding="utf-8")
    header = (output / "include/hxc/program.h").read_text(encoding="utf-8")
    markers = (
        '(hxc_string){ (const uint8_t *)"ASCII", 5, true }',
        '(hxc_string){ (const uint8_t *)"fallback", 8, true }',
        '(hxc_string){ (const uint8_t *)"conditional string failure", 26, true }',
        '(hxc_string){ (const uint8_t *)"\\303\\251\\360\\237\\231\\202", 6, true }',
        '(hxc_string){ (const uint8_t *)"embedded\\000NUL", 12, true }',
        '(hxc_string){ (const uint8_t *)"Main.hx:9: traced", 17, true }',
    )
    for marker in markers:
        if marker not in source:
            raise StringOutputFailure(f"generated C omitted literal representation {marker!r}")
    if source.count("hxc_io_println(") != 5 or source.count("abort();") != 5:
        raise StringOutputFailure("generated C lost explicit output status handling")
    if '#include <hxrt/io.h>' not in header or "<stdlib.h>" not in header:
        raise StringOutputFailure("generated private header omitted selected typed dependencies")
    if header.count(ABI_ASSERTION) != 1:
        raise StringOutputFailure("generated private header omitted its one structural runtime ABI major check")
    for forbidden in ("hxc_object", "hxc_gc", "hxc_dynamic", "hxc_reflection"):
        if forbidden in source or forbidden in header:
            raise StringOutputFailure(f"generated literal output retained {forbidden}")


def render_project(
    output: Path,
    *,
    profile: str = "portable",
    cwd: Path = ROOT,
) -> RenderedProject:
    result = compile_target(POSITIVE, output, profile=profile, diagnostics="off", report=True, cwd=cwd)
    if result.returncode != 0 or result.stderr:
        raise StringOutputFailure(
            f"{profile} literal-output compile failed\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    hxcir = extract_hxcir(result, f"{profile} literal-output compile")
    runtime_plan = load_json(output / "hxc.runtime-plan.json", "runtime plan")
    stdlib_report = load_json(output / "hxc.stdlib-report.json", "stdlib report")
    manifest = load_json(output / "hxc.manifest.json", "project manifest")
    abi = load_json(output / "hxc.abi.json", "public ABI report")
    validate_hxcir(hxcir)
    validate_runtime_plan(runtime_plan, profile=profile, policy="minimal" if profile == "metal" else "auto")
    validate_stdlib_report(stdlib_report)
    validate_manifest(manifest)
    validate_public_abi_boundary(abi)
    validate_generated_c(output)
    return RenderedProject(output, hxcir, runtime_plan, stdlib_report, manifest)


def normal_artifacts(output: Path) -> dict[str, bytes]:
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-output-snapshot-") as temporary:
        rendered = render_project(Path(temporary) / "out")
        return {
            "output.hxcir": rendered.hxcir,
            "program.h": (rendered.output / "include/hxc/program.h").read_text(encoding="utf-8"),
            "program.c": (rendered.output / "src/program.c").read_text(encoding="utf-8"),
            "runtime-plan.json": rendered.runtime_plan,
            "stdlib-report.json": rendered.stdlib_report,
        }


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


def validate_snapshots(values: dict[str, object]) -> None:
    for name, actual in values.items():
        path = EXPECTED / name
        if not path.is_file():
            raise StringOutputFailure(
                f"missing {path.relative_to(ROOT)}; use npm run snapshots:update -- --suite string-output"
            )
        expected: object
        if path.suffix == ".json":
            expected = json.loads(path.read_text(encoding="utf-8"))
        else:
            expected = path.read_text(encoding="utf-8")
        if expected != actual:
            raise StringOutputFailure(f"string-output snapshot drifted\n{snapshot_difference(expected, actual, name)}")


def validate_default_diagnostics(root: Path) -> None:
    summary = compile_target(POSITIVE, root / "summary", diagnostics=None)
    if summary.returncode != 0 or summary.stderr or summary.stdout.count("HXC2001:") != 1 or "[INFO]" not in summary.stdout:
        raise StringOutputFailure(f"portable default runtime summary drifted: {summary.stdout!r}")
    metal = compile_target(POSITIVE, root / "metal-warn", profile="metal", diagnostics=None)
    if metal.returncode != 0 or metal.stdout or metal.stderr.count("HXC2001:") != 14 or metal.stderr.count("[WARNING]") != 14:
        raise StringOutputFailure(f"metal default runtime root warnings drifted: {metal.stderr!r}")
    quiet = compile_target(POSITIVE, root / "quiet", diagnostics="off")
    if quiet.returncode != 0 or quiet.stdout or quiet.stderr:
        raise StringOutputFailure("runtime diagnostics=off changed semantics or emitted console noise")


def plausible_output_exists(output: Path) -> bool:
    return any(
        path.exists()
        for path in (
            output / "hxc.manifest.json",
            output / "hxc.runtime-plan.json",
            output / "src/program.c",
            output / "runtime",
        )
    )


def validate_fail_closed(root: Path) -> None:
    cases = (
        ("nonliteral", "TCall(Sys.println(String literal):requires-String-literal)"),
        ("nonstring", "requires-String-literal"),
        ("sys_print", "unavailable-static-target:function.Sys.print"),
        ("trace_custom", "custom-position-info-not-admitted"),
    )
    for name, marker in cases:
        output = root / name
        result = compile_target(FIXTURES / name, output, diagnostics="off")
        if result.returncode == 0 or "HXC1001:" not in result.stderr or marker not in result.stderr:
            raise StringOutputFailure(f"negative literal-output case {name} drifted: {result.stderr!r}")
        if plausible_output_exists(output):
            raise StringOutputFailure(f"negative literal-output case {name} left plausible generated output")
    none_output = root / "runtime-none"
    none = compile_target(POSITIVE, none_output, runtime="none", diagnostics="off")
    blocker_ids = [
        "runtime.io.sys-println-literal.0",
        "runtime.io.sys-println-literal.1",
        "runtime.io.sys-println-literal.2",
        "runtime.io.sys-println-literal.3",
        "runtime.io.trace-literal.4",
        "runtime.string-literal.static-value.10",
        "runtime.string-literal.static-value.11",
        "runtime.string-literal.static-value.12",
        "runtime.string-literal.static-value.13",
        "runtime.string-literal.static-value.5",
        "runtime.string-literal.static-value.6",
        "runtime.string-literal.static-value.7",
        "runtime.string-literal.static-value.8",
        "runtime.string-literal.static-value.9",
    ]
    blocker_positions = [none.stderr.find(identifier) for identifier in blocker_ids]
    expected_sources = ["Main.hx:4:4-4:24", "Main.hx:6:4-6:45", "Main.hx:7:3-7:20", "Main.hx:8:3-8:33", "Main.hx:9:3-9:8"]
    if (
        none.returncode == 0
        or "HXC2000:" not in none.stderr
        or "found 14 deduplicated runtime blocker(s)" not in none.stderr
        or any(position < 0 for position in blocker_positions)
        or blocker_positions != sorted(blocker_positions)
        or any(source not in none.stderr for source in expected_sources)
        or none.stderr.count("kind=hosted-output") != 5
        or none.stderr.count("kind=direct-string-value") != 9
        # Each call owns two different requirements: the hosted output itself
        # and the immutable String value that carries the UTF-8 bytes. Keeping
        # both records makes runtime-policy failures explain both decisions.
        or none.stderr.count("surface=`Sys.println(String literal)`") != 8
        or none.stderr.count("surface=`trace(String literal)`") != 2
        or none.stderr.count("surface=`static-haxe-string-view:String`") != 4
        or none.stderr.count(
            "dependency-chains=[io -> status -> runtime-base; io -> string-literal -> runtime-base]"
        )
        != 5
        or none.stderr.count("dependency-chains=[string-literal -> runtime-base]") != 9
    ):
        raise StringOutputFailure(f"runtime-none output policy did not fail closed: {none.stderr!r}")
    if plausible_output_exists(none_output):
        raise StringOutputFailure("runtime-none failure left plausible generated output")
    free_output = root / "freestanding"
    freestanding = compile_target(POSITIVE, free_output, diagnostics="off", environment="freestanding")
    if freestanding.returncode == 0 or "requires a system platform" not in freestanding.stderr:
        raise StringOutputFailure("freestanding Sys.println did not fail at the typed platform boundary")
    if plausible_output_exists(free_output):
        raise StringOutputFailure("freestanding rejection left plausible generated output")


def run_eval_oracle() -> None:
    result = subprocess.run(
        [development_tool("haxe"), "-cp", ".", "-main", "Main", "--interp"],
        cwd=POSITIVE,
        env=base_environment(),
        check=False,
        capture_output=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != EXPECTED_STDOUT or result.stderr:
        raise StringOutputFailure(
            f"pinned Haxe literal-output oracle drifted: exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
        )


def compiler_identity(executable: str) -> tuple[str, str]:
    result = subprocess.run([executable, "--version"], check=False, capture_output=True, text=True, timeout=30)
    if result.returncode != 0:
        raise StringOutputFailure(f"cannot identify native compiler {executable}")
    output = (result.stdout + result.stderr).strip()
    lowered = output.lower()
    family = "clang" if "clang" in lowered else "gcc" if "gcc" in lowered or "free software foundation" in lowered else "unknown"
    return family, output.splitlines()[0] if output else "unknown version"


def resolve_toolchain(family: str, required: bool) -> NativeToolchain | None:
    executable = shutil.which(family)
    if executable is None:
        if required:
            raise StringOutputFailure(f"required native compiler is missing: {family}")
        print(f"string-output: SKIP optional {family}: missing command")
        return None
    actual, version = compiler_identity(executable)
    if actual != family:
        if required:
            raise StringOutputFailure(f"required {family} identifies as {actual}: {version}")
        print(f"string-output: SKIP optional {family}: command identifies as {actual}")
        return None
    return NativeToolchain(family, executable, version)


def selected_toolchains(selected: str) -> list[NativeToolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    values = [
        toolchain
        for family in families
        if (toolchain := resolve_toolchain(family, required=selected != "auto")) is not None
    ]
    if not values:
        raise StringOutputFailure("no identity-matching strict C11 compiler is available")
    return values


def safe_project_path(output: Path, value: str) -> Path:
    path = Path(value)
    if not value or path.is_absolute() or "\\" in value or any(part in ("", ".", "..") for part in path.parts):
        raise StringOutputFailure(f"generated build plan contains an unsafe path: {value!r}")
    resolved = (output / path).resolve()
    resolved.relative_to(output.resolve())
    if not resolved.is_file():
        raise StringOutputFailure(f"generated build input is missing: {value}")
    return resolved


def safe_project_directory(output: Path, value: str) -> Path:
    path = Path(value)
    if not value or path.is_absolute() or "\\" in value or any(part in ("", ".", "..") for part in path.parts):
        raise StringOutputFailure(f"generated build plan contains an unsafe directory: {value!r}")
    resolved = (output / path).resolve()
    resolved.relative_to(output.resolve())
    if not resolved.is_dir():
        raise StringOutputFailure(f"generated include directory is missing: {value}")
    return resolved


def compile_native(toolchain: NativeToolchain, rendered: RenderedProject, optimization: str, build: Path) -> Path:
    build_plan = rendered.manifest.get("build")
    if not isinstance(build_plan, dict):
        raise StringOutputFailure("native compile lost the generated build plan")
    sources = [safe_project_path(rendered.output, value) for value in text_list(build_plan.get("sources"), "build sources")]
    includes = [
        safe_project_directory(rendered.output, value)
        for value in text_list(build_plan.get("includeDirectories"), "include directories")
    ]
    executable = build / f"{toolchain.family}-{optimization}-{rendered.runtime_plan.get('profile')}"
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        f"-{optimization}",
        *(f"-I{include}" for include in includes),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    result = subprocess.run(command, cwd=ROOT, check=False, capture_output=True, text=True, timeout=90)
    if result.returncode != 0 or result.stdout or result.stderr:
        raise StringOutputFailure(
            f"{toolchain.family} {optimization} generated project compile failed\ncommand={command!r}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return executable


def project_with_runtime_macro(rendered: RenderedProject, output: Path, macro: str, old: str, new: str) -> RenderedProject:
    shutil.copytree(rendered.output, output)
    base_header = output / "runtime/include/hxrt/base.h"
    contents = base_header.read_text(encoding="utf-8")
    before = f"#define {macro} {old}"
    after = f"#define {macro} {new}"
    if contents.count(before) != 1:
        raise StringOutputFailure(f"runtime compatibility fixture could not locate {before!r}")
    base_header.write_text(contents.replace(before, after), encoding="utf-8", newline="\n")
    return RenderedProject(
        output,
        rendered.hxcir,
        rendered.runtime_plan,
        rendered.stdlib_report,
        rendered.manifest,
    )


def reject_incompatible_runtime(toolchain: NativeToolchain, rendered: RenderedProject, build: Path) -> None:
    build_plan = rendered.manifest.get("build")
    if not isinstance(build_plan, dict):
        raise StringOutputFailure("runtime ABI mismatch probe lost the generated build plan")
    sources = [safe_project_path(rendered.output, value) for value in text_list(build_plan.get("sources"), "build sources")]
    includes = [
        safe_project_directory(rendered.output, value)
        for value in text_list(build_plan.get("includeDirectories"), "include directories")
    ]
    executable = build / f"{toolchain.family}-incompatible-runtime"
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        "-O0",
        *(f"-I{include}" for include in includes),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    result = subprocess.run(command, cwd=ROOT, check=False, capture_output=True, text=True, timeout=90)
    if result.returncode == 0 or result.stdout or "hxc_runtime_abi_major_must_match" not in result.stderr:
        raise StringOutputFailure(
            f"{toolchain.family} accepted an incompatible runtime ABI major\ncommand={command!r}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if executable.exists():
        raise StringOutputFailure(f"{toolchain.family} runtime ABI mismatch left a plausible executable")


def run_native(toolchains: list[NativeToolchain], projects: list[RenderedProject], build: Path) -> None:
    for toolchain in toolchains:
        failure_probe: Path | None = None
        for rendered in projects:
            for optimization in ("O0", "O2"):
                executable = compile_native(toolchain, rendered, optimization, build)
                result = subprocess.run([str(executable)], cwd=build, check=False, capture_output=True, timeout=30)
                if result.returncode != 0 or result.stdout != EXPECTED_STDOUT or result.stderr:
                    raise StringOutputFailure(
                        f"{toolchain.family} {optimization} generated output drifted: "
                        f"exit={result.returncode} stdout={result.stdout!r} stderr={result.stderr!r}"
                    )
                if failure_probe is None:
                    failure_probe = executable
        if failure_probe is None:
            raise StringOutputFailure(f"{toolchain.family} produced no output-failure probe")

        compatibility_root = build / f"{toolchain.family}-runtime-compatibility"
        compatibility_root.mkdir()
        compatible = project_with_runtime_macro(projects[0], compatibility_root / "compatible-minor", "HXC_RUNTIME_ABI_MINOR", "8u", "999u")
        compatible_build = compatibility_root / "compatible-build"
        compatible_build.mkdir()
        compatible_executable = compile_native(toolchain, compatible, "O0", compatible_build)
        compatible_result = subprocess.run([str(compatible_executable)], cwd=build, check=False, capture_output=True, timeout=30)
        if compatible_result.returncode != 0 or compatible_result.stdout != EXPECTED_STDOUT or compatible_result.stderr:
            raise StringOutputFailure(f"{toolchain.family} rejected a same-major compatible runtime")
        incompatible = project_with_runtime_macro(projects[0], compatibility_root / "incompatible-major", "HXC_RUNTIME_ABI_MAJOR", "0u", "1u")
        reject_incompatible_runtime(toolchain, incompatible, compatibility_root)

        def close_standard_output() -> None:
            os.close(1)

        failed = subprocess.run(
            [str(failure_probe)],
            cwd=build,
            check=False,
            stderr=subprocess.PIPE,
            preexec_fn=close_standard_output,
            timeout=30,
        )
        if failed.returncode == 0:
            raise StringOutputFailure(f"{toolchain.family} generated program ignored a hosted output failure")


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        run_eval_oracle()
        with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-output-") as temporary:
            root = Path(temporary)
            portable = render_project(root / "portable")
            repeated = render_project(root / "portable-repeated", cwd=POSITIVE)
            if normal_artifacts(portable.output) != normal_artifacts(repeated.output):
                raise StringOutputFailure("literal-output artifacts changed across output roots or caller working directories")
            metal = render_project(root / "metal", profile="metal")
            if (portable.output / "src/program.c").read_bytes() != (metal.output / "src/program.c").read_bytes():
                raise StringOutputFailure("profile selection changed literal-output C semantics")
            validate_snapshots(
                {
                    "output.hxcir": portable.hxcir,
                    "program.h": (portable.output / "include/hxc/program.h").read_text(encoding="utf-8"),
                    "program.c": (portable.output / "src/program.c").read_text(encoding="utf-8"),
                    "runtime-plan.json": portable.runtime_plan,
                    "stdlib-report.json": portable.stdlib_report,
                }
            )
            validate_default_diagnostics(root / "diagnostics")
            validate_fail_closed(root / "negative")
            toolchains = selected_toolchains(args.toolchain)
            build = root / "native"
            build.mkdir()
            run_native(toolchains, [portable, metal], build)
    except (OSError, UnicodeError, ValueError, json.JSONDecodeError, subprocess.TimeoutExpired, StringOutputFailure) as error:
        print(f"string-output: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "string-output: OK: "
        f"{families} exact UTF-8/NUL output, trace formatting, selective runtime closure, diagnostics, and fail-stop I/O passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
