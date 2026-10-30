#!/usr/bin/env python3
"""Prove the hosted POSIX synchronization slice under strict C and TSan."""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
import tempfile
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.c_fixture_harness import (  # noqa: E402
    C11_STRICT_FLAGS,
    CToolchain,
    resolve_toolchains,
)


INCLUDE = ROOT / "runtime/hxrt/include"
THREAD_SOURCES = (ROOT / "runtime/hxrt/src/thread.c",)
THREAD_CONTRACT = ROOT / "runtime/hxrt/test/thread_contract.c"
THREAD_GC_SOURCES = (
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/gc.c",
    ROOT / "runtime/hxrt/src/object.c",
    ROOT / "runtime/hxrt/src/thread.c",
    ROOT / "runtime/hxrt/src/thread_gc.c",
)
THREAD_GC_CONTRACT = ROOT / "runtime/hxrt/test/thread_gc_contract.c"
TIMEOUT_SECONDS = 30


class ThreadRuntimeFailure(RuntimeError):
    """Report the compiler and phase for one failed thread contract."""


def run(command: list[str], *, phase: str, toolchain: CToolchain, environment: dict[str, str] | None = None) -> subprocess.CompletedProcess[str]:
    result = subprocess.run(
        command,
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=TIMEOUT_SECONDS,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise ThreadRuntimeFailure(
            f"thread-runtime: FAIL compiler={toolchain.family} phase={phase} "
            f"exit={result.returncode}\ncommand: {' '.join(command)}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    return result


def compile_contract(
    build: Path,
    toolchain: CToolchain,
    phase: str,
    flags: tuple[str, ...],
    sources: tuple[Path, ...],
    contract: Path,
) -> Path:
    executable = build / phase
    run(
        [
            toolchain.compiler,
            *C11_STRICT_FLAGS,
            *flags,
            "-pthread",
            f"-I{INCLUDE}",
            *(str(source) for source in sources),
            str(contract),
            "-o",
            str(executable),
        ],
        phase=phase + ":compile",
        toolchain=toolchain,
    )
    return executable


def sanitizer_available(build: Path, toolchain: CToolchain) -> bool:
    executable = build / "tsan-probe"
    result = subprocess.run(
        [toolchain.compiler, "-std=c11", "-pthread", "-fsanitize=thread", "-x", "c", "-", "-o", str(executable)],
        cwd=ROOT,
        input="#include <pthread.h>\nint main(void) { return 0; }\n",
        check=False,
        capture_output=True,
        text=True,
        timeout=TIMEOUT_SECONDS,
    )
    if result.returncode != 0:
        return False
    probe = subprocess.run(
        [str(executable)],
        cwd=ROOT,
        env={**os.environ, "TSAN_OPTIONS": "halt_on_error=1"},
        check=False,
        capture_output=True,
        text=True,
        timeout=TIMEOUT_SECONDS,
    )
    return probe.returncode == 0 and not probe.stdout and not probe.stderr


def run_toolchain(build: Path, toolchain: CToolchain) -> str:
    build.mkdir(parents=True, exist_ok=True)
    for optimization in ("O0", "O2"):
        for name, sources, contract in (
            ("sync", THREAD_SOURCES, THREAD_CONTRACT),
            ("gc", THREAD_GC_SOURCES, THREAD_GC_CONTRACT),
        ):
            phase = "strict-" + optimization.lower() + "-" + name
            executable = compile_contract(build, toolchain, phase, ("-" + optimization,), sources, contract)
            run([str(executable)], phase=phase + ":run", toolchain=toolchain)

    if not sanitizer_available(build, toolchain):
        print(f"thread-runtime: {toolchain.family} TSan unavailable at compile/run probe")
        return "unavailable"

    for name, sources, contract in (
        ("sync", THREAD_SOURCES, THREAD_CONTRACT),
        ("gc", THREAD_GC_SOURCES, THREAD_GC_CONTRACT),
    ):
        phase = "tsan-" + name
        executable = compile_contract(
            build,
            toolchain,
            phase,
            ("-O1", "-g", "-fno-omit-frame-pointer", "-fsanitize=thread"),
            sources,
            contract,
        )
        run(
            [str(executable)],
            phase=phase + ":run",
            toolchain=toolchain,
            environment={**os.environ, "TSAN_OPTIONS": "halt_on_error=1"},
        )
    print(f"thread-runtime: {toolchain.family} TSan available and clean")
    return "clean"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "clang", "gcc"), default="auto")
    arguments = parser.parse_args()
    try:
        toolchains = resolve_toolchains(arguments.toolchain, repository_root=ROOT)
        outcomes: list[str] = []
        with tempfile.TemporaryDirectory(prefix="hxc-thread-runtime-") as temporary:
            root = Path(temporary)
            for toolchain in toolchains:
                outcome = run_toolchain(root / toolchain.family, toolchain)
                outcomes.append(f"{toolchain.family}:{outcome}")
        print("thread-runtime: OK strict=O0,O2 tsan=" + ",".join(outcomes))
        return 0
    except (RuntimeError, subprocess.SubprocessError) as error:
        print(str(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
