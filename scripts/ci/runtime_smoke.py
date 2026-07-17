#!/usr/bin/env python3
"""Compile and execute native seeds and generated goldens under strict toolchains."""

from __future__ import annotations

import argparse
import shlex
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[2]
RUNTIME_INCLUDE = ROOT / "runtime/hxrt/include"
RUNTIME_SOURCES = tuple(sorted((ROOT / "runtime/hxrt/src").glob("*.c")))
RUNTIME_SMOKE = ROOT / "runtime/hxrt/test/runtime_smoke.c"
ALLOCATOR_CONTRACT = ROOT / "runtime/hxrt/test/allocator_contract.c"
ALLOCATOR_ABI = ROOT / "runtime/hxrt/test/allocator_abi.c"
CPP_HEADER_SMOKE = ROOT / "runtime/hxrt/test/public_header_cpp.cpp"
RUNTIME_FEATURE_GRAPH = ROOT / "test/runtime/runtime-feature-graph/run.py"
ARRAY_RUNTIME = ROOT / "test/differential/array-runtime/run.py"
STRING_RUNTIME = ROOT / "test/differential/string-runtime/run.py"
HELLO_EXAMPLE = ROOT / "examples/hello/run.py"
PRIMITIVE_DIFFERENTIAL = ROOT / "test/primitive_differential/run.py"
DECLARATION_PLAN = ROOT / "test/declaration_plan"
DECLARATION_PLAN_INCLUDE = DECLARATION_PLAN / "expected/include"
DECLARATION_PLAN_SUPPORT_INCLUDE = DECLARATION_PLAN / "support/include"
DECLARATION_PLAN_HEADERS = (
    "api.h",
    "detail/state.h",
    "nodes.h",
    "types/value.h",
)
DECLARATION_PLAN_SMOKE = DECLARATION_PLAN / "smoke.c"
PROJECT_EMITTER = ROOT / "test/project_emitter/expected"
PROJECT_EMITTER_INCLUDE = PROJECT_EMITTER / "include"
PROJECT_EMITTER_HEADERS = (
    "hxc/detail/emitter_fixture_internal.h",
    "hxc/emitter_fixture.h",
    "hxc/removed_module.h",
)
PROJECT_EMITTER_SOURCES = (
    PROJECT_EMITTER / "src/emitter_fixture.c",
    PROJECT_EMITTER / "src/hxc_boot.c",
)
C_AST_RUNNER = ROOT / "test/c_ast/run.py"
POINTLIB = ROOT / "test/native/pointlib"
CPP_SHIM = ROOT / "test/native/cpp_shim"

C_STRICT_FLAGS = (
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

TOOLCHAIN_COMMANDS = {
    "gcc": ("gcc", "g++"),
    "clang": ("clang", "clang++"),
}


class NativeSmokeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    cc: str
    cxx: str
    cc_version: str
    cxx_version: str


def command_result(command: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def compiler_version(executable: str) -> tuple[str, str]:
    result = command_result([executable, "--version"])
    if result.returncode != 0:
        raise NativeSmokeFailure(
            f"cannot identify compiler {executable}: exit {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    combined = (result.stdout + result.stderr).strip()
    first_line = combined.splitlines()[0] if combined else "unknown version"
    lowered = combined.lower()
    if "clang" in lowered:
        return "clang", first_line
    if "free software foundation" in lowered or "gcc" in lowered or "g++" in lowered:
        return "gcc", first_line
    return "unknown", first_line


def resolve_toolchain(family: str, *, required: bool) -> Toolchain | None:
    cc_name, cxx_name = TOOLCHAIN_COMMANDS[family]
    cc = shutil.which(cc_name)
    cxx = shutil.which(cxx_name)
    missing = [name for name, value in ((cc_name, cc), (cxx_name, cxx)) if value is None]
    qualifier = "required" if required else "optional"
    if missing:
        message = f"{family}: missing {', '.join(missing)}"
        if required:
            raise NativeSmokeFailure(f"required toolchain {message}")
        print(f"native-smoke: SKIP {qualifier} {message}")
        return None

    assert cc is not None
    assert cxx is not None
    cc_family, cc_version = compiler_version(cc)
    cxx_family, cxx_version = compiler_version(cxx)
    if cc_family != family or cxx_family != family:
        message = (
            f"{family}: command identity mismatch "
            f"({cc_name}={cc_family}, {cxx_name}={cxx_family})"
        )
        if required:
            raise NativeSmokeFailure(f"required toolchain {message}")
        print(f"native-smoke: SKIP {qualifier} {message}")
        return None

    print(
        f"native-smoke: {'REQUIRED' if required else 'AVAILABLE'} {family}: "
        f"cc={cc_version}; cxx={cxx_version}"
    )
    return Toolchain(family, cc, cxx, cc_version, cxx_version)


def selected_toolchains(requested: str) -> list[Toolchain]:
    if requested != "auto":
        toolchain = resolve_toolchain(requested, required=True)
        assert toolchain is not None
        return [toolchain]

    resolved = [
        toolchain
        for family in TOOLCHAIN_COMMANDS
        if (toolchain := resolve_toolchain(family, required=False)) is not None
    ]
    if not resolved:
        raise NativeSmokeFailure(
            "no complete GCC or Clang C11/C++17 toolchain is available; "
            "a native gate cannot be reported as passed"
        )
    return resolved


def run_command(command: list[str], *, label: str, echo_output: bool = False) -> None:
    print(f"native-smoke: RUN {label}: {shlex.join(command)}")
    result = command_result(command)
    if result.returncode != 0:
        raise NativeSmokeFailure(
            f"{label} failed with exit {result.returncode}\n"
            f"command: {shlex.join(command)}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if echo_output and result.stdout:
        print(result.stdout, end="" if result.stdout.endswith("\n") else "\n")


def compile_object(
    compiler: str,
    flags: tuple[str, ...],
    source: Path,
    output: Path,
    *,
    includes: tuple[Path, ...],
    label: str,
) -> None:
    command = [compiler, *flags]
    command.extend(f"-I{include}" for include in includes)
    command.extend(("-c", str(source), "-o", str(output)))
    run_command(command, label=label)


def link_executable(
    linker: str, objects: tuple[Path, ...], output: Path, *, label: str
) -> None:
    run_command(
        [linker, *(str(path) for path in objects), "-o", str(output)],
        label=label,
    )


def run_executable(executable: Path, sentinel: str, *, label: str) -> None:
    print(f"native-smoke: RUN {label}: {executable}")
    result = command_result([str(executable)])
    if result.returncode != 0 or result.stdout.strip() != sentinel or result.stderr:
        raise NativeSmokeFailure(
            f"{label} produced unexpected results\n"
            f"exit: {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def run_silent_executable(executable: Path, *, label: str) -> None:
    print(f"native-smoke: RUN {label}: {executable}")
    result = command_result([str(executable)])
    if result.returncode != 0 or result.stdout or result.stderr:
        raise NativeSmokeFailure(
            f"{label} produced unexpected results\n"
            f"exit: {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


def require_no_allocator_libc_symbols(object_path: Path, *, label: str) -> None:
    nm = shutil.which("nm")
    if nm is None:
        raise NativeSmokeFailure(f"{label} requires nm for undefined-symbol evidence")
    result = command_result([nm, "-u", str(object_path)])
    if result.returncode != 0:
        raise NativeSmokeFailure(
            f"{label} could not inspect undefined symbols\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    forbidden = {
        "aligned_alloc",
        "calloc",
        "free",
        "malloc",
        "posix_memalign",
        "realloc",
    }
    found: set[str] = set()
    for line in result.stdout.splitlines():
        parts = line.split()
        if not parts:
            continue
        symbol = parts[-1].lstrip("_").split("@", 1)[0]
        if symbol in forbidden:
            found.add(symbol)
    if found:
        raise NativeSmokeFailure(
            f"{label} retained forbidden libc symbols: {', '.join(sorted(found))}"
        )


def run_toolchain(toolchain: Toolchain, build: Path) -> tuple[str, ...]:
    build.mkdir(parents=True, exist_ok=True)
    family = toolchain.family
    lanes: list[str] = []

    run_command(
        [
            sys.executable,
            str(RUNTIME_FEATURE_GRAPH),
            "--toolchain",
            family,
            "--native-only",
        ],
        label=f"{family} selective runtime feature packaging",
    )
    lanes.append("runtime-feature-selective-packaging")

    run_command(
        [
            sys.executable,
            str(ARRAY_RUNTIME),
            "--toolchain",
            family,
            "--native-only",
        ],
        label=f"{family} typed array runtime contract",
    )
    lanes.append("array-runtime-contract")

    run_command(
        [
            sys.executable,
            str(STRING_RUNTIME),
            "--toolchain",
            family,
            "--native-only",
        ],
        label=f"{family} UTF-8 scalar string runtime contract",
    )
    lanes.append("string-runtime-contract")

    run_command(
        [
            sys.executable,
            str(HELLO_EXAMPLE),
            "--toolchain",
            family,
            "--native-only",
        ],
        label=f"{family} generated hello example",
    )
    lanes.append("generated-hello-example-run")

    run_command(
        [
            sys.executable,
            str(PRIMITIVE_DIFFERENTIAL),
            "--toolchain",
            family,
            "--native-only",
        ],
        label=f"{family} seeded primitive differential and sanitizer corpus",
        echo_output=True,
    )
    lanes.append("primitive-differential-sanitizer-run")

    run_command(
        [
            sys.executable,
            str(C_AST_RUNNER),
            "--native-only",
            "--toolchain",
            family,
        ],
        label=f"{family} reusable direct C AST fixture corpus",
        echo_output=True,
    )
    lanes.extend(
        (
            "structural-c-ast-golden-run",
            "expression-precedence-golden-run",
            "c-ast-attribute-run",
            "c-ast-header-source-run",
        )
    )

    declaration_includes = (
        DECLARATION_PLAN_INCLUDE,
        DECLARATION_PLAN_SUPPORT_INCLUDE,
    )
    for index, header in enumerate(DECLARATION_PLAN_HEADERS):
        header_probe = build / f"declaration_header_{index}.c"
        header_probe.write_text(
            f'#include "{header}"\n#include "{header}"\n', encoding="utf-8"
        )
        compile_object(
            toolchain.cc,
            C_STRICT_FLAGS,
            header_probe,
            build / f"declaration_header_{index}.o",
            includes=declaration_includes,
            label=f"{family} independent declaration header {header}",
        )
    lanes.append("declaration-header-independent-compile")

    declaration_smoke_object = build / "declaration_plan_smoke.o"
    declaration_smoke_executable = build / "declaration_plan_smoke"
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        DECLARATION_PLAN_SMOKE,
        declaration_smoke_object,
        includes=declaration_includes,
        label=f"{family} declaration-plan header consumer",
    )
    link_executable(
        toolchain.cc,
        (declaration_smoke_object,),
        declaration_smoke_executable,
        label=f"{family} declaration-plan header link",
    )
    run_executable(
        declaration_smoke_executable,
        "declaration-plan-headers: OK",
        label=f"{family} declaration-plan header execution",
    )
    lanes.append("declaration-plan-header-run")

    for index, header in enumerate(PROJECT_EMITTER_HEADERS):
        header_probe = build / f"project_emitter_header_{index}.c"
        header_probe.write_text(
            f'#include "{header}"\n#include "{header}"\n', encoding="utf-8"
        )
        compile_object(
            toolchain.cc,
            C_STRICT_FLAGS,
            header_probe,
            build / f"project_emitter_header_{index}.o",
            includes=(PROJECT_EMITTER_INCLUDE,),
            label=f"{family} independent project-emitter header {header}",
        )
    lanes.append("project-emitter-header-independent-compile")

    project_objects: list[Path] = []
    for index, source in enumerate(PROJECT_EMITTER_SOURCES):
        project_object = build / f"project_emitter_{index}.o"
        compile_object(
            toolchain.cc,
            C_STRICT_FLAGS,
            source,
            project_object,
            includes=(PROJECT_EMITTER_INCLUDE,),
            label=f"{family} project-emitter source {source.name}",
        )
        project_objects.append(project_object)
    project_executable = build / "project_emitter"
    link_executable(
        toolchain.cc,
        tuple(project_objects),
        project_executable,
        label=f"{family} project-emitter link",
    )
    run_executable(
        project_executable,
        "project-emitter: OK",
        label=f"{family} project-emitter execution",
    )
    lanes.append("project-emitter-structural-run")

    runtime_objects: list[Path] = []
    runtime_smoke_object = build / "runtime_smoke.o"
    runtime_executable = build / "runtime_smoke"
    if not RUNTIME_SOURCES:
        raise NativeSmokeFailure("runtime source feature slices are missing")
    for index, source in enumerate(RUNTIME_SOURCES):
        runtime_object = build / f"hxc_runtime_{index}.o"
        compile_object(
            toolchain.cc,
            C_STRICT_FLAGS,
            source,
            runtime_object,
            includes=(RUNTIME_INCLUDE,),
            label=f"{family} hosted runtime {source.name}",
        )
        runtime_objects.append(runtime_object)
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        RUNTIME_SMOKE,
        runtime_smoke_object,
        includes=(RUNTIME_INCLUDE,),
        label=f"{family} runtime fixture",
    )
    link_executable(
        toolchain.cc,
        (*runtime_objects, runtime_smoke_object),
        runtime_executable,
        label=f"{family} runtime link",
    )
    run_executable(
        runtime_executable,
        "runtime-smoke: OK",
        label=f"{family} runtime execution",
    )
    lanes.append("hosted-runtime-run")

    allocator_sanitizer_executable = build / "allocator_sanitizer"
    run_command(
        [
            toolchain.cc,
            *C_STRICT_FLAGS,
            "-O1",
            "-g",
            "-fno-omit-frame-pointer",
            "-fno-sanitize-recover=all",
            "-fsanitize=address,undefined",
            f"-I{RUNTIME_INCLUDE}",
            *(str(source) for source in RUNTIME_SOURCES),
            str(RUNTIME_SMOKE),
            "-o",
            str(allocator_sanitizer_executable),
        ],
        label=f"{family} hosted allocator address/undefined sanitizer build",
    )
    run_executable(
        allocator_sanitizer_executable,
        "runtime-smoke: OK",
        label=f"{family} hosted allocator address/undefined sanitizer execution",
    )
    lanes.append("allocator-address-undefined-sanitizer-run")

    run_command(
        [
            toolchain.cc,
            *C_STRICT_FLAGS,
            "-ffreestanding",
            "-DHXC_FREESTANDING=1",
            f"-I{RUNTIME_INCLUDE}",
            "-fsyntax-only",
            *(str(source) for source in RUNTIME_SOURCES),
        ],
        label=f"{family} freestanding runtime compile-only",
    )
    lanes.append("freestanding-runtime-compile")

    freestanding_flags = (
        *C_STRICT_FLAGS,
        "-ffreestanding",
        "-DHXC_FREESTANDING=1",
    )
    freestanding_allocator_object = build / "freestanding_allocator.o"
    allocator_contract_object = build / "allocator_contract.o"
    allocator_contract_executable = build / "allocator_contract"
    compile_object(
        toolchain.cc,
        freestanding_flags,
        ROOT / "runtime/hxrt/src/allocator.c",
        freestanding_allocator_object,
        includes=(RUNTIME_INCLUDE,),
        label=f"{family} freestanding allocator object",
    )
    require_no_allocator_libc_symbols(
        freestanding_allocator_object,
        label=f"{family} freestanding allocator",
    )
    compile_object(
        toolchain.cc,
        freestanding_flags,
        ALLOCATOR_CONTRACT,
        allocator_contract_object,
        includes=(RUNTIME_INCLUDE,),
        label=f"{family} custom allocator contract fixture",
    )
    link_executable(
        toolchain.cc,
        (freestanding_allocator_object, allocator_contract_object),
        allocator_contract_executable,
        label=f"{family} custom allocator contract link",
    )
    run_silent_executable(
        allocator_contract_executable,
        label=f"{family} custom allocator contract execution",
    )
    lanes.append("freestanding-custom-allocator-run")

    allocator_abi_object = build / "allocator_abi.o"
    cpp_header_object = build / "public_header_cpp.o"
    cpp_header_executable = build / "public_header_cpp"
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        ALLOCATOR_ABI,
        allocator_abi_object,
        includes=(RUNTIME_INCLUDE,),
        label=f"{family} C allocator ABI layout producer",
    )
    compile_object(
        toolchain.cxx,
        CXX_STRICT_FLAGS,
        CPP_HEADER_SMOKE,
        cpp_header_object,
        includes=(RUNTIME_INCLUDE,),
        label=f"{family} C++ public-header consumer",
    )
    link_executable(
        toolchain.cxx,
        (*runtime_objects, allocator_abi_object, cpp_header_object),
        cpp_header_executable,
        label=f"{family} C++ public-header link",
    )
    run_executable(
        cpp_header_executable,
        "public-header-cpp: OK",
        label=f"{family} C++ public-header execution",
    )
    lanes.append("c-cxx17-allocator-layout-run")

    point_include = POINTLIB / "include"
    point_object = build / "pointlib.o"
    point_smoke_object = build / "pointlib_smoke.o"
    point_executable = build / "pointlib_smoke"
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        POINTLIB / "src/pointlib.c",
        point_object,
        includes=(point_include,),
        label=f"{family} pointlib fixture",
    )
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        POINTLIB / "smoke.c",
        point_smoke_object,
        includes=(point_include,),
        label=f"{family} pointlib consumer",
    )
    link_executable(
        toolchain.cc,
        (point_object, point_smoke_object),
        point_executable,
        label=f"{family} pointlib link",
    )
    run_executable(
        point_executable,
        "pointlib-smoke: OK",
        label=f"{family} pointlib execution",
    )
    lanes.append("native-c-library-run")

    shim_include = CPP_SHIM / "include"
    shim_object = build / "counter_shim.o"
    shim_smoke_object = build / "counter_shim_smoke.o"
    shim_executable = build / "counter_shim_smoke"
    compile_object(
        toolchain.cxx,
        CXX_STRICT_FLAGS,
        CPP_SHIM / "src/counter_shim.cpp",
        shim_object,
        includes=(shim_include,),
        label=f"{family} C++ shim fixture",
    )
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        CPP_SHIM / "smoke.c",
        shim_smoke_object,
        includes=(shim_include,),
        label=f"{family} C consumer of C++ shim",
    )
    link_executable(
        toolchain.cxx,
        (shim_object, shim_smoke_object),
        shim_executable,
        label=f"{family} C++ shim link",
    )
    run_executable(
        shim_executable,
        "cpp-shim-smoke: OK",
        label=f"{family} C++ shim execution",
    )
    lanes.append("native-cxx-shim-run")

    return tuple(lanes)


def parse_args(argv: Iterable[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--toolchain",
        choices=("auto", *TOOLCHAIN_COMMANDS),
        default="auto",
        help="auto-runs available pairs; an explicit family is required and may not skip",
    )
    return parser.parse_args(list(argv))


def main(argv: Iterable[str] = ()) -> int:
    args = parse_args(argv)
    try:
        toolchains = selected_toolchains(args.toolchain)
        with tempfile.TemporaryDirectory(prefix="reflaxe-c-native-smoke-") as temporary:
            build_root = Path(temporary)
            completed = {
                toolchain.family: run_toolchain(
                    toolchain, build_root / toolchain.family
                )
                for toolchain in toolchains
            }
    except (NativeSmokeFailure, subprocess.TimeoutExpired) as error:
        print(f"native-smoke: ERROR: {error}", file=sys.stderr)
        return 1

    summary = "; ".join(
        f"{family}=[{', '.join(lanes)}]" for family, lanes in completed.items()
    )
    print(f"native-smoke: OK: {summary}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
