#!/usr/bin/env python3
"""Build and run the private Dynamic carrier under strict native lanes."""

from __future__ import annotations

import argparse
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[3]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402

INCLUDE = ROOT / "runtime/hxrt/include"
SOURCE = ROOT / "runtime/hxrt/src/dynamic.c"
CONTRACT = ROOT / "runtime/hxrt/test/dynamic_contract.c"
CPP_HEADER = ROOT / "runtime/hxrt/test/dynamic_header_cpp.cpp"
EXPECTED_CONTRACT = "dynamic-runtime-contract: OK\n"
EXPECTED_CPP = "dynamic-header-cpp: OK\n"

C_FLAGS = (
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
CXX_FLAGS = (
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


class DynamicRuntimeFailure(RuntimeError):
    pass


@dataclass(frozen=True)
class Toolchain:
    family: str
    cc: str
    cxx: str


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
    return run_bounded_process(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )


def compiler_family(command: str) -> str:
    result = run([command, "--version"])
    if result.returncode != 0:
        return "unknown"
    version = (result.stdout + result.stderr).lower()
    if "clang" in version:
        return "clang"
    if "free software foundation" in version or "gcc" in version or "g++" in version:
        return "gcc"
    return "unknown"


def resolve(requested: str) -> list[Toolchain]:
    names = (("gcc", "gcc", "g++"), ("clang", "clang", "clang++"))
    found: list[Toolchain] = []
    for family, cc_name, cxx_name in names:
        if requested != "auto" and requested != family:
            continue
        cc = shutil.which(cc_name)
        cxx = shutil.which(cxx_name)
        if (
            cc is None
            or cxx is None
            or compiler_family(cc) != family
            or compiler_family(cxx) != family
        ):
            if requested == family:
                raise DynamicRuntimeFailure(
                    f"requested {family} C/C++ toolchain is unavailable or misidentified"
                )
            print(
                f"dynamic-runtime: SKIP optional {family}: unavailable or command identity mismatch"
            )
            continue
        found.append(Toolchain(family, cc, cxx))
    if not found:
        raise DynamicRuntimeFailure(
            "no identified GCC or Clang C/C++ toolchain is available"
        )
    return found


def require_success(
    result: subprocess.CompletedProcess[str], label: str
) -> None:
    if result.returncode != 0:
        raise DynamicRuntimeFailure(
            f"{label} failed with exit {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )


def run_contract(
    toolchain: Toolchain,
    build: Path,
    *,
    optimization: str,
    sanitizer: bool,
) -> None:
    suffix = f"{optimization}{'-san' if sanitizer else ''}"
    executable = build / f"dynamic-contract-{suffix}"
    command = [
        toolchain.cc,
        *C_FLAGS,
        f"-{optimization}",
        f"-I{INCLUDE}",
    ]
    if sanitizer:
        command.extend(
            (
                "-g",
                "-fno-omit-frame-pointer",
                "-fno-sanitize-recover=all",
                "-fsanitize=address,undefined",
            )
        )
    command.extend((str(SOURCE), str(CONTRACT), "-o", str(executable)))
    require_success(run(command), f"{toolchain.family} {suffix} Dynamic build")
    result = run([str(executable)])
    require_success(result, f"{toolchain.family} {suffix} Dynamic run")
    if result.stdout != EXPECTED_CONTRACT or result.stderr:
        raise DynamicRuntimeFailure(
            f"{toolchain.family} {suffix} Dynamic output drifted\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )


def run_freestanding_and_symbol_check(toolchain: Toolchain, build: Path) -> None:
    freestanding = run(
        [
            toolchain.cc,
            *C_FLAGS,
            "-ffreestanding",
            "-DHXC_FREESTANDING=1",
            f"-I{INCLUDE}",
            "-fsyntax-only",
            str(SOURCE),
        ]
    )
    require_success(freestanding, f"{toolchain.family} freestanding Dynamic syntax")

    object_path = build / "dynamic.o"
    require_success(
        run(
            [
                toolchain.cc,
                *C_FLAGS,
                "-O2",
                f"-I{INCLUDE}",
                "-c",
                str(SOURCE),
                "-o",
                str(object_path),
            ]
        ),
        f"{toolchain.family} Dynamic object",
    )
    nm = shutil.which("nm")
    if nm is None:
        raise DynamicRuntimeFailure("Dynamic dependency evidence requires nm")
    symbols = run([nm, "-u", str(object_path)])
    require_success(symbols, f"{toolchain.family} Dynamic undefined-symbol scan")
    forbidden = (
        "alloc",
        "array",
        "calloc",
        "free",
        "gc",
        "malloc",
        "object",
        "realloc",
    )
    observed = symbols.stdout.lower()
    if any(token in observed for token in forbidden):
        raise DynamicRuntimeFailure(
            f"{toolchain.family} scalar Dynamic retained allocation/object/collector symbols:\n"
            f"{symbols.stdout}"
        )


def run_cpp_header(toolchain: Toolchain, build: Path) -> None:
    dynamic_object = build / "dynamic-cpp-link.o"
    require_success(
        run(
            [
                toolchain.cc,
                *C_FLAGS,
                "-O2",
                f"-I{INCLUDE}",
                "-c",
                str(SOURCE),
                "-o",
                str(dynamic_object),
            ]
        ),
        f"{toolchain.family} C Dynamic object for C++",
    )
    executable = build / "dynamic-header-cpp"
    require_success(
        run(
            [
                toolchain.cxx,
                *CXX_FLAGS,
                f"-I{INCLUDE}",
                str(CPP_HEADER),
                str(dynamic_object),
                "-o",
                str(executable),
            ]
        ),
        f"{toolchain.family} C++17 Dynamic header",
    )
    result = run([str(executable)])
    require_success(result, f"{toolchain.family} C++17 Dynamic header run")
    if result.stdout != EXPECTED_CPP or result.stderr:
        raise DynamicRuntimeFailure(
            f"{toolchain.family} C++17 Dynamic header output drifted\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--toolchain", choices=("auto", "gcc", "clang"), default="auto"
    )
    arguments = parser.parse_args()
    toolchains = resolve(arguments.toolchain)
    with tempfile.TemporaryDirectory(prefix="haxe-c-dynamic-runtime-") as temporary:
        root = Path(temporary)
        for toolchain in toolchains:
            build = root / toolchain.family
            build.mkdir(parents=True)
            run_contract(toolchain, build, optimization="O0", sanitizer=False)
            run_contract(toolchain, build, optimization="O2", sanitizer=False)
            run_contract(toolchain, build, optimization="O1", sanitizer=True)
            run_freestanding_and_symbol_check(toolchain, build)
            run_cpp_header(toolchain, build)
    families = ", ".join(toolchain.family for toolchain in toolchains)
    print(
        "dynamic-runtime: OK: tagged scalars, exact managed wrappers, invalid-tag "
        f"rejection, freestanding C11, C++17, and ASan/UBSan passed ({families})"
    )
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except DynamicRuntimeFailure as error:
        print(f"dynamic-runtime: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
