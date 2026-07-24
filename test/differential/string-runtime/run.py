#!/usr/bin/env python3
"""Compare the native UTF-8 runtime contract with the pinned Haxe scalar oracle."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[3]
CASE = Path(__file__).resolve().parent
ORACLE_HXML = CASE / "oracle.hxml"
FIXTURE = CASE / "string_runtime.c"
GENERATED = CASE / "generated"
NEGATIVE = CASE / "negative"
INCLUDE = ROOT / "runtime/hxrt/include"
SOURCES = (
    ROOT / "runtime/hxrt/src/abi.c",
    ROOT / "runtime/hxrt/src/status.c",
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/string.c",
    ROOT / "runtime/hxrt/src/string_scalar.c",
)
EXPECTED_TRACE = "1,128512,3,1,2,😀\n"
EXPECTED_GENERATED_STDOUT = "string-owned: OK\n"
LAYOUTS = ("split", "package", "unity")
REPORT_PREFIX = "HXC_STATIC_INITIALIZATION="
EXPECTED_GENERATED_FEATURES = [
    "runtime-base",
    "status",
    "alloc",
    "string-literal",
    "io",
    "string-scalar",
    "string",
]
EXPECTED_GENERATED_ARTIFACTS = [
    "runtime/include/hxrt/allocator.h",
    "runtime/include/hxrt/base.h",
    "runtime/include/hxrt/io.h",
    "runtime/include/hxrt/status.h",
    "runtime/include/hxrt/string.h",
    "runtime/include/hxrt/string_decode.h",
    "runtime/include/hxrt/string_literal.h",
    "runtime/include/hxrt/string_scalar.h",
    "runtime/src/allocator.c",
    "runtime/src/io.c",
    "runtime/src/string.c",
    "runtime/src/string_scalar.c",
]
TOOLCHAINS = ("gcc", "clang")
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic",
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
    "-DHXC_FREESTANDING=1",
)
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fno-sanitize-recover=all",
    "-fsanitize=address,undefined",
)


class StringRuntimeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    compiler: str
    version: str


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def haxe_environment(*, server: bool = False) -> dict[str, str]:
    """Use an isolated process unless a test explicitly owns a warm server."""
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


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
        raise StringRuntimeFailure(f"cannot identify compiler {executable}")
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


def resolve_toolchain(family: str, *, required: bool) -> Toolchain | None:
    compiler = shutil.which(family)
    if compiler is None:
        if required:
            raise StringRuntimeFailure(
                f"required string-runtime compiler is missing: {family}"
            )
        print(f"string-runtime: SKIP optional {family}: missing command")
        return None
    actual, version = compiler_identity(compiler)
    if actual != family:
        if required:
            raise StringRuntimeFailure(
                f"required {family} command identifies as {actual}: {version}"
            )
        print(
            f"string-runtime: SKIP optional {family}: command identifies as {actual}"
        )
        return None
    return Toolchain(family, compiler, version)


def selected_toolchains(selected: str) -> list[Toolchain]:
    families = TOOLCHAINS if selected == "auto" else (selected,)
    toolchains = [
        toolchain
        for family in families
        if (
            toolchain := resolve_toolchain(
                family, required=selected != "auto"
            )
        )
        is not None
    ]
    if not toolchains:
        raise StringRuntimeFailure(
            "no strict C11 compiler is available for the string runtime contract"
        )
    return toolchains


def run_oracle() -> str:
    outputs: list[str] = []
    for label in ("first", "second"):
        result = subprocess.run(
            [development_tool("haxe"), str(ORACLE_HXML)],
            cwd=ROOT,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        if result.returncode != 0 or result.stderr:
            raise StringRuntimeFailure(
                f"{label} Haxe scalar-string oracle failed\n"
                f"exit={result.returncode} stdout={result.stdout!r} "
                f"stderr={result.stderr!r}"
            )
        outputs.append(result.stdout)
    if outputs[0] != outputs[1] or outputs[0] != EXPECTED_TRACE:
        raise StringRuntimeFailure(
            "Haxe scalar-string oracle drifted\n"
            f"first={outputs[0]!r} second={outputs[1]!r}"
        )
    return outputs[0]


def compile_native_fixture(
    toolchain: Toolchain,
    build: Path,
    flags: tuple[str, ...],
    label: str,
    expected_trace: str,
) -> Path:
    executable = build / label
    command = [
        toolchain.compiler,
        *STRICT_FLAGS,
        *flags,
        f"-I{INCLUDE}",
        *(str(source) for source in SOURCES),
        str(FIXTURE),
        "-o",
        str(executable),
    ]
    compiled = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} {label} compile failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\n"
            f"stderr={compiled.stderr!r}"
        )
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if (
        executed.returncode != 0
        or executed.stdout != expected_trace
        or executed.stderr
    ):
        raise StringRuntimeFailure(
            f"{toolchain.family} {label} execution drifted\n"
            f"exit={executed.returncode} stdout={executed.stdout!r} "
            f"stderr={executed.stderr!r}"
        )
    return executable


def inspect_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringRuntimeFailure(f"{family} string runtime requires nm evidence")
    result = subprocess.run(
        [nm, str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise StringRuntimeFailure(f"{family} could not inspect string symbols")
    symbols = result.stdout
    for required in (
        "hxc_string_concat",
        "hxc_string_from_utf8_lossy",
        "hxc_string_borrow_cstring",
    ):
        if required not in symbols:
            raise StringRuntimeFailure(
                f"{family} string link omitted required symbol {required}"
            )
    for forbidden in ("hxc_object", "hxc_gc", "hxc_reflection", "hxc_dynamic"):
        if forbidden in symbols:
            raise StringRuntimeFailure(
                f"{family} string link retained unrelated symbol family {forbidden}"
            )


def run_native(toolchains: list[Toolchain], expected_trace: str) -> None:
    with tempfile.TemporaryDirectory(prefix="reflaxe-c-string-runtime-") as temporary:
        root = Path(temporary)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir(parents=True)
            debug = compile_native_fixture(
                toolchain, build, ("-O0",), "string-runtime-o0", expected_trace
            )
            compile_native_fixture(
                toolchain, build, ("-O2",), "string-runtime-o2", expected_trace
            )
            compile_native_fixture(
                toolchain,
                build,
                SANITIZER_FLAGS,
                "string-runtime-sanitized",
                expected_trace,
            )
            inspect_symbols(debug, toolchain.family)


def run_generated_eval() -> None:
    """Run the ordinary-Haxe fixture twice before involving haxe.c."""
    observations: list[tuple[int, str, str]] = []
    for _ in range(2):
        result = subprocess.run(
            [development_tool("haxe"), "oracle.hxml"],
            cwd=GENERATED,
            env=haxe_environment(),
            check=False,
            capture_output=True,
            text=True,
            timeout=30,
        )
        observations.append((result.returncode, result.stdout, result.stderr))
    expected = (0, EXPECTED_GENERATED_STDOUT, "")
    if observations != [expected, expected]:
        raise StringRuntimeFailure(
            f"ordinary-Haxe managed String Eval oracle drifted: {observations!r}"
        )


def compile_haxe(
    fixture: Path,
    output: Path,
    *,
    layout: str = "split",
    reverse: bool = False,
    report: bool = False,
    defines: tuple[str, ...] = (),
    connect: str | None = None,
) -> subprocess.CompletedProcess[str]:
    """Compile one isolated ordinary-Haxe fixture through the real target."""
    command = [development_tool("haxe")]
    if connect is not None:
        command.extend(["--connect", connect])
    command.extend(
        [
            "-cp",
            str(fixture),
            "-lib",
            "reflaxe.c",
            "-main",
            "Main",
            "-D",
            f"hxc_project_layout={layout}",
            "-D",
            "hxc_runtime_diagnostics=off",
        ]
    )
    if reverse:
        command.extend(["-D", "reflaxe_c_test_reverse_typed_modules"])
    if report:
        command.extend(["-D", "reflaxe_c_static_initialization_report"])
    for define in defines:
        command.extend(["-D", define])
    command.extend(["--custom-target", f"c={output}"])
    return subprocess.run(
        command,
        cwd=ROOT,
        env=haxe_environment(server=connect is not None),
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )


def generated_tree(output: Path) -> dict[str, bytes]:
    """Read normal compiler artifacts while excluding Reflaxe activity metadata."""
    return {
        path.relative_to(output).as_posix(): path.read_bytes()
        for path in sorted(output.rglob("*"))
        if path.is_file() and path.name != "_GeneratedFiles.json"
    }


def extract_hxcir(result: subprocess.CompletedProcess[str]) -> str:
    values = [
        line[len(REPORT_PREFIX) :]
        for line in result.stdout.splitlines()
        if line.startswith(REPORT_PREFIX)
    ]
    if len(values) != 1:
        raise StringRuntimeFailure(
            "managed String compile omitted its one validated HxcIR report"
        )
    report = json.loads(values[0])
    hxcir = report.get("hxcir") if isinstance(report, dict) else None
    if not isinstance(hxcir, str) or not hxcir:
        raise StringRuntimeFailure("managed String HxcIR report is empty")
    return hxcir


def string_list(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise StringRuntimeFailure(f"{label} must be a string array")
    return list(value)


def validate_generated_project(output: Path, hxcir: str) -> None:
    """Check semantic intent, exact runtime closure, and recognizable C calls."""
    for operation in (
        'runtime(feature="string",operation="from-scalar")',
        'runtime(feature="string",operation="concat")',
        'runtime(feature="string-scalar",operation="char-at")',
        'runtime(feature="string-scalar",operation="char-code-at")',
        'runtime(feature="string-scalar",operation="length")',
        'runtime(feature="string-scalar",operation="substring")',
        'implementation=runtime("string")',
    ):
        if operation not in hxcir:
            raise StringRuntimeFailure(
                f"validated HxcIR omitted managed String evidence {operation}"
            )
    if str(ROOT) in hxcir:
        raise StringRuntimeFailure("managed String HxcIR leaked the checkout path")

    plan = json.loads((output / "hxc.runtime-plan.json").read_text(encoding="utf-8"))
    if (
        plan.get("status") != "analyzed-runtime-features"
        or plan.get("planPurpose") != "compiler-program"
        or string_list(plan.get("features"), "runtime features")
        != EXPECTED_GENERATED_FEATURES
        or string_list(plan.get("artifacts"), "runtime artifacts")
        != EXPECTED_GENERATED_ARTIFACTS
    ):
        raise StringRuntimeFailure(
            "ordinary Haxe managed Strings selected the wrong runtime closure"
        )
    reasons = [
        reason
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict) and reason.get("featureId") == "string"
    ]
    operations = {
        reason.get("operationId")
        for reason in reasons
        if reason.get("kind") == "runtime-operation"
    }
    if operations != {"cleanup-release", "concat", "from-scalar", "retain"}:
        raise StringRuntimeFailure(
            f"managed String roots lost typed provenance: {sorted(operations)!r}"
        )
    scalar_operations = {
        reason.get("operationId")
        for reason in plan.get("rootReasons", [])
        if isinstance(reason, dict)
        and reason.get("featureId") == "string-scalar"
        and reason.get("kind") == "runtime-operation"
    }
    if scalar_operations != {"char-at", "char-code-at", "length", "substring"}:
        raise StringRuntimeFailure(
            f"String scalar roots drifted: {sorted(scalar_operations)!r}"
        )

    stdlib = json.loads(
        (output / "hxc.stdlib-report.json").read_text(encoding="utf-8")
    )
    if stdlib.get("modules") != ["String", "Sys", "string"] or stdlib.get(
        "capabilities"
    ) != [
        "char-at",
        "char-code-at",
        "cleanup-release",
        "concat",
        "from-scalar",
        "length",
        "retain",
        "static-value",
        "substring",
        "sys-println-literal",
    ]:
        raise StringRuntimeFailure("managed String standard-library report drifted")

    source_text = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((output / "src").rglob("*.c"))
    )
    for required in (
        "hxc_string_from_scalar(",
        "hxc_string_concat_ref(",
        "hxc_string_retain(",
        "hxc_string_release(",
        "hxc_string_substring(",
    ):
        if required not in source_text:
            raise StringRuntimeFailure(
                f"generated C omitted managed String call {required}"
            )
    for forbidden in ("hxc_object", "hxc_gc", "hxc_dynamic", "__c__("):
        if forbidden in source_text:
            raise StringRuntimeFailure(
                f"managed String fixture acquired unrelated/generated escape {forbidden!r}"
            )


def available_port() -> int:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as candidate:
        candidate.bind(("127.0.0.1", 0))
        return int(candidate.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    deadline = time.monotonic() + 10.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise StringRuntimeFailure(
                f"Haxe server exited before managed String requests: "
                f"{stdout!r} {stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise StringRuntimeFailure(
        "Haxe server did not accept managed String determinism requests"
    )


def render_server_pair(root: Path) -> tuple[Path, Path]:
    port = available_port()
    endpoint = str(port)
    server = subprocess.Popen(
        [development_tool("haxe"), "--wait", endpoint],
        cwd=ROOT,
        env=haxe_environment(server=True),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        outputs = (root / "server-first", root / "server-second")
        for label, output in zip(("first", "second"), outputs):
            result = compile_haxe(GENERATED, output, connect=endpoint)
            if result.returncode != 0 or result.stdout or result.stderr:
                raise StringRuntimeFailure(
                    f"{label} warm-server managed String compile failed: "
                    f"{result.stdout!r} {result.stderr!r}"
                )
        return outputs
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def render_projects(root: Path) -> dict[str, Path]:
    projects: dict[str, Path] = {}
    for layout in LAYOUTS:
        normal = root / f"{layout}-normal"
        reverse = root / f"{layout}-reverse"
        first = compile_haxe(
            GENERATED, normal, layout=layout, report=layout == "split"
        )
        second = compile_haxe(GENERATED, reverse, layout=layout, reverse=True)
        for label, result in (
            (f"{layout}-normal", first),
            (f"{layout}-reverse", second),
        ):
            if result.returncode != 0 or result.stderr:
                raise StringRuntimeFailure(
                    f"{label} managed String compile failed\n"
                    f"stdout={result.stdout!r}\nstderr={result.stderr!r}"
                )
        if generated_tree(normal) != generated_tree(reverse):
            raise StringRuntimeFailure(
                f"{layout} managed String output changed under reversed discovery"
            )
        projects[layout] = normal
        if layout == "split":
            validate_generated_project(normal, extract_hxcir(first))

    server_first, server_second = render_server_pair(root)
    split_tree = generated_tree(projects["split"])
    if (
        generated_tree(server_first) != split_tree
        or generated_tree(server_second) != split_tree
    ):
        raise StringRuntimeFailure(
            "managed String output changed under warm compiler-server reuse"
        )
    return projects


def plausible_output_exists(output: Path) -> bool:
    return output.exists() and any(output.rglob("*"))


def validate_generated_failures(root: Path) -> None:
    unsupported_output = root / "unsupported-method"
    unsupported = compile_haxe(NEGATIVE, unsupported_output)
    expected = "TCall(String.toUpperCase:not-yet-admitted)"
    if (
        unsupported.returncode == 0
        or "HXC1001:" not in unsupported.stderr
        or expected not in unsupported.stderr
    ):
        raise StringRuntimeFailure(
            "unsupported String method did not fail at its intrinsic owner: "
            f"{unsupported.stderr!r}"
        )
    if plausible_output_exists(unsupported_output):
        raise StringRuntimeFailure(
            "unsupported String method left plausible generated output"
        )

    none_output = root / "runtime-none"
    none = compile_haxe(
        GENERATED, none_output, defines=("hxc_runtime=none",)
    )
    if (
        none.returncode == 0
        or "HXC2000:" not in none.stderr
        or "runtime.string.from-scalar." not in none.stderr
        or "string -> alloc -> status -> runtime-base" not in none.stderr
        or "string -> string-scalar -> string-literal -> runtime-base"
        not in none.stderr
    ):
        raise StringRuntimeFailure(
            f"runtime=none did not explain managed String closure: {none.stderr!r}"
        )
    if plausible_output_exists(none_output):
        raise StringRuntimeFailure(
            "runtime=none managed String rejection left plausible output"
        )


def project_build_inputs(project: Path) -> tuple[list[Path], list[Path]]:
    manifest = json.loads(
        (project / "hxc.manifest.json").read_text(encoding="utf-8")
    )
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise StringRuntimeFailure(
            "generated managed String manifest omitted its build plan"
        )
    sources = [
        (project / value).resolve()
        for value in string_list(build.get("sources"), "generated sources")
    ]
    includes = [
        (project / value).resolve()
        for value in string_list(
            build.get("includeDirectories"), "generated include roots"
        )
    ]
    for path in (*sources, *includes):
        path.relative_to(project.resolve())
        if not path.exists():
            raise StringRuntimeFailure(f"generated build input is missing: {path}")
    return sources, includes


def compile_generated_and_run(
    toolchain: Toolchain,
    project: Path,
    executable: Path,
    flags: tuple[str, ...],
) -> None:
    sources, include_roots = project_build_inputs(project)
    command = [
        toolchain.compiler,
        *(flag for flag in STRICT_FLAGS if flag != "-DHXC_FREESTANDING=1"),
        *flags,
        *(f"-I{root}" for root in include_roots),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    compiled = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} generated managed String compile failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\n"
            f"stderr={compiled.stderr!r}"
        )
    environment = os.environ.copy()
    if toolchain.family == "clang" and "-fsanitize=address,undefined" in flags:
        environment["ASAN_OPTIONS"] = "detect_leaks=0"
        environment["UBSAN_OPTIONS"] = "halt_on_error=1"
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if (
        executed.returncode != 0
        or executed.stdout != EXPECTED_GENERATED_STDOUT
        or executed.stderr
    ):
        raise StringRuntimeFailure(
            f"{toolchain.family} generated managed String execution drifted: "
            f"exit={executed.returncode} stdout={executed.stdout!r} "
            f"stderr={executed.stderr!r}"
        )


def inspect_generated_symbols(executable: Path, family: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise StringRuntimeFailure(f"{family} managed String evidence requires nm")
    result = subprocess.run(
        [nm, str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if result.returncode != 0:
        raise StringRuntimeFailure(
            f"{family} could not inspect generated managed String symbols"
        )
    for required in (
        "hxc_string_from_scalar",
        "hxc_string_concat_ref",
        "hxc_string_retain",
        "hxc_string_release",
        "hxc_string_substring",
    ):
        if required not in result.stdout:
            raise StringRuntimeFailure(
                f"{family} executable omitted selected symbol {required}"
            )
    for forbidden in ("hxc_gc", "hxc_object", "hxc_dynamic", "hxc_reflection"):
        if forbidden in result.stdout:
            raise StringRuntimeFailure(
                f"{family} executable retained unrelated symbol {forbidden}"
            )


def compile_cpp_headers(toolchain: Toolchain, project: Path, build: Path) -> None:
    cxx_name = "clang++" if toolchain.family == "clang" else "g++"
    compiler = shutil.which(cxx_name)
    if compiler is None:
        print(
            f"string-runtime: SKIP optional {toolchain.family} C++17 headers: "
            f"missing {cxx_name}"
        )
        return
    _, include_roots = project_build_inputs(project)
    source = build / f"{toolchain.family}-headers.cpp"
    source.write_text(
        '#include "hxc/program.h"\n'
        '#include "hxrt/string.h"\n'
        "int main() {\n"
        "  hxc_string value = HXC_STRING_EMPTY_INITIALIZER;\n"
        "  return value.owner == nullptr ? 0 : 1;\n"
        "}\n",
        encoding="utf-8",
    )
    executable = build / f"{toolchain.family}-headers"
    command = [
        compiler,
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-pedantic-errors",
        *(f"-I{root}" for root in include_roots),
        str(source),
        "-o",
        str(executable),
    ]
    compiled = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if compiled.returncode != 0 or compiled.stdout or compiled.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} C++17 header consumer failed\n"
            f"command={command!r}\nstdout={compiled.stdout!r}\n"
            f"stderr={compiled.stderr!r}"
        )
    executed = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=20,
    )
    if executed.returncode != 0 or executed.stdout or executed.stderr:
        raise StringRuntimeFailure(
            f"{toolchain.family} C++17 header consumer execution drifted"
        )


def run_generated_native(
    toolchains: list[Toolchain], projects: dict[str, Path], root: Path
) -> None:
    for toolchain in toolchains:
        build = root / f"generated-{toolchain.family}"
        build.mkdir()
        for layout, project in projects.items():
            for optimization in ("-O0", "-O2"):
                executable = build / f"{layout}-{optimization[1:].lower()}"
                compile_generated_and_run(
                    toolchain, project, executable, (optimization,)
                )
                inspect_generated_symbols(executable, toolchain.family)
        compile_cpp_headers(toolchain, projects["split"], build)
        if toolchain.family == "clang":
            executable = build / "split-sanitized"
            compile_generated_and_run(
                toolchain, projects["split"], executable, SANITIZER_FLAGS
            )
            inspect_generated_symbols(executable, toolchain.family)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", *TOOLCHAINS), default="auto")
    parser.add_argument(
        "--native-only",
        action="store_true",
        help="use the checked semantic trace without requiring Haxe",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        expected_trace = EXPECTED_TRACE if args.native_only else run_oracle()
        toolchains = selected_toolchains(args.toolchain)
        run_native(toolchains, expected_trace)
        if not args.native_only:
            run_generated_eval()
            with tempfile.TemporaryDirectory(
                prefix="reflaxe-c-generated-string-runtime-"
            ) as temporary:
                root = Path(temporary)
                projects = render_projects(root)
                validate_generated_failures(root)
                run_generated_native(toolchains, projects, root)
    except (
        OSError,
        UnicodeError,
        StringRuntimeFailure,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"string-runtime: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    oracle = (
        "checked native trace"
        if args.native_only
        else "native/Eval parity plus ordinary-Haxe generated projects"
    )
    print(
        "string-runtime: OK: "
        f"{families}; {oracle}; checked/lossy UTF-8, scalar indexing, "
        "owned aliases/fields/containers/returns, split/package/unity "
        "determinism, C11/C++17, sanitizers, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
