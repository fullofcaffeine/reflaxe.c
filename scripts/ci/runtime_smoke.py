#!/usr/bin/env python3
"""Compile and execute the M0 native seed under strict C/C++ toolchains."""

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
RUNTIME_SOURCE = ROOT / "runtime/hxrt/src/hxc_runtime.c"
RUNTIME_SMOKE = ROOT / "runtime/hxrt/test/runtime_smoke.c"
CPP_HEADER_SMOKE = ROOT / "runtime/hxrt/test/public_header_cpp.cpp"
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


def run_command(command: list[str], *, label: str) -> None:
    print(f"native-smoke: RUN {label}: {shlex.join(command)}")
    result = command_result(command)
    if result.returncode != 0:
        raise NativeSmokeFailure(
            f"{label} failed with exit {result.returncode}\n"
            f"command: {shlex.join(command)}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )


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


def run_toolchain(toolchain: Toolchain, build: Path) -> tuple[str, ...]:
    build.mkdir(parents=True, exist_ok=True)
    family = toolchain.family
    lanes: list[str] = []

    runtime_object = build / "hxc_runtime.o"
    runtime_smoke_object = build / "runtime_smoke.o"
    runtime_executable = build / "runtime_smoke"
    compile_object(
        toolchain.cc,
        C_STRICT_FLAGS,
        RUNTIME_SOURCE,
        runtime_object,
        includes=(RUNTIME_INCLUDE,),
        label=f"{family} hosted runtime",
    )
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
        (runtime_object, runtime_smoke_object),
        runtime_executable,
        label=f"{family} runtime link",
    )
    run_executable(
        runtime_executable,
        "runtime-smoke: OK",
        label=f"{family} runtime execution",
    )
    lanes.append("hosted-runtime-run")

    run_command(
        [
            toolchain.cc,
            *C_STRICT_FLAGS,
            "-ffreestanding",
            "-DHXC_FREESTANDING=1",
            f"-I{RUNTIME_INCLUDE}",
            "-fsyntax-only",
            str(RUNTIME_SOURCE),
        ],
        label=f"{family} freestanding runtime compile-only",
    )
    lanes.append("freestanding-runtime-compile")

    cpp_header_object = build / "public_header_cpp.o"
    cpp_header_executable = build / "public_header_cpp"
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
        (runtime_object, cpp_header_object),
        cpp_header_executable,
        label=f"{family} C++ public-header link",
    )
    run_executable(
        cpp_header_executable,
        "public-header-cpp: OK",
        label=f"{family} C++ public-header execution",
    )
    lanes.append("cxx17-public-header-run")

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
