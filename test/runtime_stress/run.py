#!/usr/bin/env python3
"""Run the reproducible cross-feature hxrt stress contract."""

from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
import tempfile
from dataclasses import dataclass
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
BASE_HEADER = INCLUDE / "hxrt/base.h"
CONTRACT = ROOT / "test/runtime_stress/runtime_stress.c"
SOURCES = (
    ROOT / "runtime/hxrt/src/abi.c",
    ROOT / "runtime/hxrt/src/allocator.c",
    ROOT / "runtime/hxrt/src/array.c",
    ROOT / "runtime/hxrt/src/dynamic.c",
    ROOT / "runtime/hxrt/src/exception.c",
    ROOT / "runtime/hxrt/src/gc.c",
    ROOT / "runtime/hxrt/src/object.c",
    ROOT / "runtime/hxrt/src/string.c",
    ROOT / "runtime/hxrt/src/string_scalar.c",
)
DEFAULT_SEED = 0x5EED1234
DEFAULT_LIMIT = 512
MINIMUM_LIMIT = 16
MAXIMUM_LIMIT = 4096
TIMEOUT_SECONDS = 60
REPORT_PATTERN = re.compile(
    r"^runtime-stress: OK "
    r"abi=(?P<abi>\d+\.\d+\.\d+) "
    r"features=alloc,string,array,gc,dynamic,exception "
    r"seed=(?P<seed>\d+) limit=(?P<limit>\d+) "
    r"string-attempts=(?P<string_attempts>\d+) "
    r"array-attempts=(?P<array_attempts>\d+) "
    r"gc-attempts=(?P<gc_attempts>\d+) "
    r"threads=not-applicable\n$"
)
ABI_COMPONENT_PATTERN = re.compile(
    r"^#define HXC_RUNTIME_ABI_(MAJOR|MINOR|PATCH) (\d+)u$", re.MULTILINE
)


class RuntimeStressFailure(RuntimeError):
    """Report one failed phase with the complete reproducibility context."""


@dataclass(frozen=True)
class RunConfig:
    toolchain: CToolchain
    seed: int
    limit: int
    runtime_abi: str

    def label(self, phase: str) -> str:
        return (
            f"phase={phase} compiler={self.toolchain.family} "
            f"version={self.toolchain.version_line!r} seed={self.seed} "
            f"limit={self.limit} abi={self.runtime_abi} "
            "features=alloc,string,array,gc,dynamic,exception"
        )


def runtime_abi_identity() -> str:
    components = {
        name.lower(): value
        for name, value in ABI_COMPONENT_PATTERN.findall(
            BASE_HEADER.read_text(encoding="utf-8")
        )
    }
    if set(components) != {"major", "minor", "patch"}:
        raise RuntimeStressFailure(
            "runtime-stress: cannot read the runtime ABI from hxrt/base.h"
        )
    return ".".join(components[name] for name in ("major", "minor", "patch"))


def run_command(
    command: list[str],
    *,
    config: RunConfig,
    phase: str,
    environment: dict[str, str] | None = None,
    input_text: str | None = None,
) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            command,
            cwd=ROOT,
            check=False,
            capture_output=True,
            text=True,
            input=input_text,
            env=environment,
            timeout=TIMEOUT_SECONDS,
        )
    except subprocess.TimeoutExpired as error:
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} timed out after "
            f"{TIMEOUT_SECONDS}s\ncommand: {' '.join(command)}"
        ) from error
    if result.returncode != 0:
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} exit={result.returncode}\n"
            f"command: {' '.join(command)}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    return result


def compile_contract(
    build: Path,
    *,
    config: RunConfig,
    phase: str,
    extra_flags: tuple[str, ...],
) -> Path:
    executable = build / phase
    command = [
        config.toolchain.compiler,
        *C11_STRICT_FLAGS,
        *extra_flags,
        "-DHXC_STRESS_SEED=" + str(config.seed) + "u",
        "-DHXC_STRESS_LIMIT=" + str(config.limit) + "u",
        f"-I{INCLUDE}",
        *(str(source) for source in SOURCES),
        str(CONTRACT),
        "-o",
        str(executable),
    ]
    run_command(command, config=config, phase=phase + ":compile")
    return executable


def execute_contract(
    executable: Path,
    *,
    config: RunConfig,
    phase: str,
    environment: dict[str, str] | None = None,
) -> str:
    result = run_command(
        [str(executable)],
        config=config,
        phase=phase + ":run",
        environment=environment,
    )
    if result.stderr:
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} unexpected stderr:\n"
            + result.stderr
        )
    match = REPORT_PATTERN.fullmatch(result.stdout)
    if match is None:
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} malformed report:\n"
            + result.stdout
        )
    if int(match.group("seed")) != config.seed or int(
        match.group("limit")
    ) != config.limit:
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} report changed its inputs"
        )
    if match.group("abi") != config.runtime_abi:
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} report changed its ABI"
        )
    if any(
        int(match.group(name)) <= 0
        for name in ("string_attempts", "array_attempts", "gc_attempts")
    ):
        raise RuntimeStressFailure(
            f"runtime-stress: FAIL {config.label(phase)} omitted a fault-site class"
        )
    return result.stdout


def sanitizer_supported(
    build: Path,
    *,
    config: RunConfig,
    name: str,
    flags: tuple[str, ...],
) -> bool:
    executable = build / (name + "-probe")
    command = [
        config.toolchain.compiler,
        *C11_STRICT_FLAGS,
        *flags,
        "-x",
        "c",
        "-",
        "-o",
        str(executable),
    ]
    result = subprocess.run(
        command,
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        input="int main(void) { return 0; }\n",
        timeout=TIMEOUT_SECONDS,
    )
    if result.returncode != 0:
        return False
    run_command(
        [str(executable)],
        config=config,
        phase=name + ":probe",
    )
    return True


def run_toolchain(build: Path, config: RunConfig) -> tuple[str, ...]:
    build.mkdir(parents=True, exist_ok=True)
    reports: list[str] = []
    for optimization in ("O0", "O2"):
        executable = compile_contract(
            build,
            config=config,
            phase="strict-" + optimization.lower(),
            extra_flags=("-" + optimization,),
        )
        reports.append(
            execute_contract(
                executable,
                config=config,
                phase="strict-" + optimization.lower(),
            )
        )

    address_undefined_flags = (
        "-O1",
        "-g",
        "-fno-omit-frame-pointer",
        "-fno-sanitize-recover=all",
        "-fsanitize=address,undefined",
    )
    leak_flags = (
        "-O1",
        "-g",
        "-fno-omit-frame-pointer",
        "-fsanitize=leak",
    )
    leak_available = sanitizer_supported(
        build,
        config=config,
        name="leak",
        flags=leak_flags,
    )
    if not sanitizer_supported(
        build,
        config=config,
        name="address-undefined",
        flags=address_undefined_flags,
    ):
        raise RuntimeStressFailure(
            "runtime-stress: FAIL "
            + config.label("address-undefined")
            + " compiler lacks the required platform sanitizer lane"
        )
    sanitizer_environment = dict(os.environ)
    sanitizer_environment["ASAN_OPTIONS"] = (
        "detect_leaks=" + ("1" if leak_available else "0") + ":halt_on_error=1"
    )
    sanitizer_environment["UBSAN_OPTIONS"] = "halt_on_error=1:print_stacktrace=1"
    executable = compile_contract(
        build,
        config=config,
        phase="address-undefined",
        extra_flags=address_undefined_flags,
    )
    reports.append(
        execute_contract(
            executable,
            config=config,
            phase="address-undefined",
            environment=sanitizer_environment,
        )
    )

    if leak_available:
        leak_environment = dict(os.environ)
        leak_environment["LSAN_OPTIONS"] = "exitcode=23:report_objects=1"
        executable = compile_contract(
            build,
            config=config,
            phase="leak",
            extra_flags=leak_flags,
        )
        reports.append(
            execute_contract(
                executable,
                config=config,
                phase="leak",
                environment=leak_environment,
            )
        )
        print(f"runtime-stress: {config.toolchain.family} LSan available and clean")
    else:
        print(
            f"runtime-stress: {config.toolchain.family} LSan unavailable; "
            "ASan+UBSan remain enabled without a platform leak detector"
        )

    first = reports[0]
    if any(report != first for report in reports[1:]):
        raise RuntimeStressFailure(
            "runtime-stress: FAIL "
            + config.label("determinism")
            + " strict and sanitizer reports differ"
        )
    return tuple(reports)


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--toolchain", choices=("auto", "clang", "gcc"), default="auto")
    parser.add_argument("--seed", type=lambda value: int(value, 0), default=DEFAULT_SEED)
    parser.add_argument("--limit", type=int, default=DEFAULT_LIMIT)
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    if arguments.seed < 0 or arguments.seed > 0xFFFFFFFF:
        print("runtime-stress: seed must fit uint32", file=sys.stderr)
        return 2
    if arguments.limit < MINIMUM_LIMIT or arguments.limit > MAXIMUM_LIMIT:
        print(
            f"runtime-stress: limit must be {MINIMUM_LIMIT}..{MAXIMUM_LIMIT}",
            file=sys.stderr,
        )
        return 2
    try:
        toolchains = resolve_toolchains(arguments.toolchain, repository_root=ROOT)
        runtime_abi = runtime_abi_identity()
        completed: list[str] = []
        with tempfile.TemporaryDirectory(prefix="hxc-runtime-stress-") as temporary:
            build_root = Path(temporary)
            for toolchain in toolchains:
                config = RunConfig(
                    toolchain,
                    arguments.seed,
                    arguments.limit,
                    runtime_abi,
                )
                run_toolchain(build_root / toolchain.family, config)
                completed.append(toolchain.family)
        print(
            "runtime-stress: OK toolchains="
            + ",".join(completed)
            + f" seed={arguments.seed} limit={arguments.limit} "
            "lanes=O0,O2,ASan+UBSan,LSan-when-available"
        )
        return 0
    except (RuntimeStressFailure, RuntimeError, subprocess.SubprocessError) as error:
        print(str(error), file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
