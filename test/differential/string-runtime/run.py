#!/usr/bin/env python3
"""Compare the native UTF-8 runtime contract with the pinned Haxe scalar oracle."""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable


ROOT = Path(__file__).resolve().parents[3]
CASE = Path(__file__).resolve().parent
ORACLE_HXML = CASE / "oracle.hxml"
FIXTURE = CASE / "string_runtime.c"
INCLUDE = ROOT / "runtime/hxrt/include"
SOURCES = (
    ROOT / "runtime/hxrt/src/abi.c",
    ROOT / "runtime/hxrt/src/status.c",
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/string.c",
    ROOT / "runtime/hxrt/src/string_scalar.c",
)
EXPECTED_TRACE = "1,128512,3,1,2,😀\n"
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
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    outputs: list[str] = []
    for label in ("first", "second"):
        result = subprocess.run(
            [development_tool("haxe"), str(ORACLE_HXML)],
            cwd=ROOT,
            env=environment,
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


def compile_and_run(
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
            debug = compile_and_run(
                toolchain, build, ("-O0",), "string-runtime-o0", expected_trace
            )
            compile_and_run(
                toolchain, build, ("-O2",), "string-runtime-o2", expected_trace
            )
            compile_and_run(
                toolchain,
                build,
                SANITIZER_FLAGS,
                "string-runtime-sanitized",
                expected_trace,
            )
            inspect_symbols(debug, toolchain.family)


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
    except (
        OSError,
        UnicodeError,
        StringRuntimeFailure,
        subprocess.TimeoutExpired,
    ) as error:
        print(f"string-runtime: ERROR: {error}", file=sys.stderr)
        return 1
    families = ", ".join(toolchain.family for toolchain in toolchains)
    oracle = "checked scalar trace" if args.native_only else "pinned Haxe Eval oracle"
    print(
        "string-runtime: OK: "
        f"{families}; {oracle}; checked/lossy UTF-8, scalar indexing, "
        "allocation, CString lifetime, sanitizers, and selective symbols passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
