#!/usr/bin/env python3
"""Plan, execute, and aggregate the release-blocking Tier-1 platform matrix."""

from __future__ import annotations

import argparse
import json
import os
import platform
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
MATRIX_PATH = ROOT / "docs/specs/platform-support-matrix.json"
EXPECTED_LANES = (
    "linux-x86_64-gcc",
    "linux-x86_64-clang",
    "linux-aarch64-gcc",
    "linux-aarch64-clang",
    "macos-arm64-apple-clang",
    "macos-x86_64-apple-clang",
    "windows-x86_64-clang-cl",
    "cortex-m3-arm-none-eabi-gcc",
)
HOSTED_EVIDENCE = frozenset(
    ("native-compile", "native-link", "native-run", "c-consumer", "cpp-consumer")
)
METAL_EVIDENCE = frozenset(
    ("cross-compile", "cross-link", "map-symbol-inspection", "emulated-run")
)


class MatrixFailure(RuntimeError):
    """A matrix input, host, execution, or evidence contract failed."""


@dataclass(frozen=True)
class Lane:
    """One release-blocking tuple and the exact evidence that qualifies it."""

    value: dict[str, object]

    @property
    def identifier(self) -> str:
        return required_string(self.value.get("id"), "lane.id")

    @property
    def evidence(self) -> tuple[str, ...]:
        return string_array(self.value.get("evidence"), f"{self.identifier}.evidence")


def required_string(value: object, label: str) -> str:
    if not isinstance(value, str) or not value:
        raise MatrixFailure(f"{label} must be a non-empty string")
    return value


def string_array(value: object, label: str) -> tuple[str, ...]:
    if not isinstance(value, list) or not value:
        raise MatrixFailure(f"{label} must be a non-empty array")
    result: list[str] = []
    for index, item in enumerate(value):
        result.append(required_string(item, f"{label}[{index}]"))
    if len(result) != len(set(result)):
        raise MatrixFailure(f"{label} must not contain duplicates")
    return tuple(result)


def load_matrix(path: Path = MATRIX_PATH) -> tuple[dict[str, object], tuple[Lane, ...]]:
    try:
        raw: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise MatrixFailure(f"cannot read platform matrix: {error}") from error
    if not isinstance(raw, dict):
        raise MatrixFailure("platform matrix must be an object")
    expected_root = {
        "$schema",
        "schemaVersion",
        "matrixId",
        "authority",
        "releaseBlocking",
        "lanes",
    }
    if set(raw) != expected_root:
        raise MatrixFailure("platform matrix root fields do not match schema")
    if raw.get("$schema") != "platform-support-matrix.schema.json":
        raise MatrixFailure("platform matrix schema reference drifted")
    if raw.get("schemaVersion") != 1 or raw.get("matrixId") != "hxc-tier1-v1":
        raise MatrixFailure("platform matrix identity is unsupported")
    if raw.get("releaseBlocking") is not True:
        raise MatrixFailure("Tier-1 platform matrix must be release blocking")
    values = raw.get("lanes")
    if not isinstance(values, list):
        raise MatrixFailure("platform matrix lanes must be an array")
    lanes: list[Lane] = []
    expected_fields = {
        "id", "tier", "runner", "os", "environment", "architecture",
        "toolchain", "profile", "runtimePolicy", "execution", "evidence",
        "artifactKinds",
    }
    for index, value in enumerate(values):
        if not isinstance(value, dict) or set(value) != expected_fields:
            raise MatrixFailure(f"lanes[{index}] fields do not match schema")
        lane = Lane(value)
        for field in expected_fields - {"tier", "evidence", "artifactKinds"}:
            required_string(value.get(field), f"{lane.identifier}.{field}")
        if value.get("tier") != 1:
            raise MatrixFailure(f"{lane.identifier} must remain Tier 1")
        string_array(value.get("artifactKinds"), f"{lane.identifier}.artifactKinds")
        required = METAL_EVIDENCE if value.get("environment") == "freestanding" else HOSTED_EVIDENCE
        missing = sorted(required - set(lane.evidence))
        if missing:
            raise MatrixFailure(f"{lane.identifier} is missing evidence: {', '.join(missing)}")
        if value.get("execution") == "native" and "native-run" not in lane.evidence:
            raise MatrixFailure(f"{lane.identifier} native execution lacks native-run evidence")
        if value.get("execution") == "emulated" and "emulated-run" not in lane.evidence:
            raise MatrixFailure(f"{lane.identifier} emulated execution lacks emulated-run evidence")
        lanes.append(lane)
    identifiers = tuple(lane.identifier for lane in lanes)
    if identifiers != EXPECTED_LANES:
        raise MatrixFailure("Tier-1 lane set or stable order drifted")
    return raw, tuple(lanes)


def lane_by_id(identifier: str) -> tuple[dict[str, object], Lane]:
    matrix, lanes = load_matrix()
    for lane in lanes:
        if lane.identifier == identifier:
            return matrix, lane
    raise MatrixFailure(f"unsupported platform lane: {identifier}")


def run(command: list[str], *, cwd: Path = ROOT, timeout: int = 180) -> str:
    print(f"platform-matrix: RUN {' '.join(command)}")
    try:
        result = subprocess.run(
            command,
            cwd=cwd,
            check=False,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise MatrixFailure(f"command could not run: {error}") from error
    if result.returncode != 0:
        raise MatrixFailure(
            f"command failed with exit {result.returncode}: {' '.join(command)}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if result.stdout:
        print(result.stdout, end="")
    if result.stderr:
        print(result.stderr, end="", file=sys.stderr)
    return result.stdout + result.stderr


def normalized_host() -> tuple[str, str]:
    system = platform.system().lower()
    os_name = {"darwin": "macos"}.get(system, system)
    machine = platform.machine().lower()
    architecture = {"amd64": "x86_64", "arm64": "arm64"}.get(machine, machine)
    return os_name, architecture


def require_native_host(lane: Lane) -> None:
    actual_os, actual_arch = normalized_host()
    expected_os = required_string(lane.value.get("os"), f"{lane.identifier}.os")
    expected_arch = required_string(
        lane.value.get("architecture"), f"{lane.identifier}.architecture"
    )
    equivalent_arches = {("macos", "arm64", "aarch64"), ("linux", "aarch64", "arm64")}
    architecture_matches = actual_arch == expected_arch or (
        actual_os,
        actual_arch,
        expected_arch,
    ) in equivalent_arches
    if actual_os != expected_os or not architecture_matches:
        raise MatrixFailure(
            f"{lane.identifier} requires native {expected_os}/{expected_arch}; "
            f"runner is {actual_os}/{actual_arch}"
        )


def require_tool(name: str) -> str:
    resolved = shutil.which(name)
    if resolved is None:
        raise MatrixFailure(f"required tool is unavailable: {name}")
    return resolved


def execute_unix(lane: Lane) -> None:
    require_native_host(lane)
    family = required_string(lane.value.get("toolchain"), f"{lane.identifier}.toolchain")
    run([sys.executable, "examples/hello/run.py", "--native-only", "--toolchain", family])
    compiler = require_tool(family)
    cxx = require_tool("g++" if family == "gcc" else "clang++")
    expected = ROOT / "examples/hello/expected"
    runtime_include = ROOT / "runtime/hxrt/include"
    consumer = ROOT / "test/platform_matrix/fixtures/hosted/hello_consumer.cpp"
    with tempfile.TemporaryDirectory(prefix="hxc-platform-hosted-") as temporary:
        build = Path(temporary)
        run([
            cxx, "-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic-errors",
            f"-I{expected / 'include'}", f"-I{runtime_include}", "-c", str(consumer),
            "-o", str(build / "hello-consumer.o"),
        ], cwd=build)
        if "sanitizer" in lane.evidence:
            executable = build / "hello-sanitized"
            run([
                compiler, "-std=c11", "-Wall", "-Wextra", "-Werror",
                "-pedantic-errors", "-O1", "-g", "-fno-omit-frame-pointer",
                "-fno-sanitize-recover=all", "-fsanitize=address,undefined",
                f"-I{expected / 'include'}", f"-I{runtime_include}",
                str(expected / "src/program.c"), str(ROOT / "runtime/hxrt/src/io.c"),
                "-o", str(executable),
            ], cwd=build)
            output = run([str(executable)], cwd=build)
            if output != "Hello from hxc\n":
                raise MatrixFailure("sanitized generated hello emitted unexpected output")


def execute_windows(lane: Lane) -> None:
    require_native_host(lane)
    clang_cl = require_tool("clang-cl")
    library_tool = require_tool("llvm-lib")
    fixtures = ROOT / "test/platform_matrix/fixtures/windows"
    expected = ROOT / "examples/hello/expected"
    runtime_include = ROOT / "runtime/hxrt/include"
    with tempfile.TemporaryDirectory(prefix="hxc-platform-windows-") as temporary:
        build = Path(temporary)
        hello = build / "hello.exe"
        run([
            clang_cl, "/nologo", "/std:c11", "/W4", "/WX",
            f"/I{expected / 'include'}", f"/I{runtime_include}",
            str(expected / "src/program.c"), str(ROOT / "runtime/hxrt/src/io.c"),
            f"/Fe{hello}",
        ], cwd=build)
        if run([str(hello)], cwd=build) != "Hello from hxc\n":
            raise MatrixFailure("clang-cl generated hello emitted unexpected output")
        library_object = build / "library.obj"
        c_object = build / "consumer-c.obj"
        cpp_object = build / "consumer-cpp.obj"
        run([clang_cl, "/nologo", "/std:c11", "/W4", "/WX", "/c", str(fixtures / "library.c"), f"/Fo{library_object}"], cwd=build)
        run([clang_cl, "/nologo", "/std:c11", "/W4", "/WX", "/c", str(fixtures / "consumer.c"), f"/Fo{c_object}"], cwd=build)
        run([clang_cl, "/nologo", "/std:c++17", "/W4", "/WX", "/EHsc", "/c", str(fixtures / "consumer.cpp"), f"/Fo{cpp_object}"], cwd=build)
        static_library = build / "platform-matrix.lib"
        run([library_tool, f"/out:{static_library}", str(library_object)], cwd=build)
        for language, consumer in (("c", c_object), ("cpp", cpp_object)):
            executable = build / f"consumer-{language}.exe"
            run([clang_cl, "/nologo", str(consumer), str(static_library), f"/Fe{executable}"], cwd=build)
            run([str(executable)], cwd=build)
        dll_object = build / "library-dll.obj"
        dll = build / "platform-matrix.dll"
        run([clang_cl, "/nologo", "/std:c11", "/W4", "/WX", "/DHXC_PLATFORM_MATRIX_BUILD_DLL", "/c", str(fixtures / "library.c"), f"/Fo{dll_object}"], cwd=build)
        run([clang_cl, "/nologo", "/LD", str(dll_object), f"/Fe{dll}"], cwd=build)
        if not dll.is_file() or not static_library.is_file():
            raise MatrixFailure("clang-cl did not produce both required library artifacts")


def execute_metal() -> None:
    compiler = require_tool("arm-none-eabi-gcc")
    nm = require_tool("arm-none-eabi-nm")
    qemu = require_tool("qemu-system-arm")
    fixtures = ROOT / "test/platform_matrix/fixtures/cortex_m3"
    with tempfile.TemporaryDirectory(prefix="hxc-platform-cortex-m3-") as temporary:
        build = Path(temporary)
        executable = build / "smoke.elf"
        map_file = build / "smoke.map"
        run([
            compiler, "-mcpu=cortex-m3", "-mthumb", "-ffreestanding", "-fno-builtin",
            "-nostdlib", "-Wall", "-Wextra", "-Werror", "-Wl,--gc-sections",
            f"-Wl,-Map={map_file}", "-T", str(fixtures / "link.ld"),
            str(fixtures / "smoke.c"), "-o", str(executable),
        ], cwd=build)
        symbols = run([nm, "--defined-only", str(executable)], cwd=build)
        if "reset_handler" not in symbols or not map_file.is_file():
            raise MatrixFailure("Cortex-M image lost reset symbol or linker map")
        output = run([
            qemu, "-M", "lm3s6965evb", "-nographic", "-semihosting-config",
            "enable=on,target=native", "-kernel", str(executable),
        ], cwd=build, timeout=30)
        if "hxc-cortex-m3: OK" not in output:
            raise MatrixFailure("Cortex-M emulator did not emit its smoke marker")


def source_revision() -> str:
    revision = os.environ.get("GITHUB_SHA")
    if revision:
        return revision
    return run(["git", "rev-parse", "HEAD"]).strip()


def toolchain_version(lane: Lane) -> str:
    toolchain = required_string(
        lane.value.get("toolchain"), f"{lane.identifier}.toolchain"
    )
    executable = require_tool(toolchain)
    output = run([executable, "--version"], timeout=30)
    first_line = next((line.strip() for line in output.splitlines() if line.strip()), "")
    if not first_line:
        raise MatrixFailure(f"{lane.identifier} toolchain emitted no version identity")
    return first_line


def write_report(matrix: dict[str, object], lane: Lane, path: Path) -> None:
    runner_os, runner_architecture = normalized_host()
    report = {
        "schemaVersion": 1,
        "matrixId": matrix["matrixId"],
        "lane": lane.value,
        "sourceRevision": source_revision(),
        "status": "passed",
        "observedEvidence": list(lane.evidence),
        "observation": {
            "runnerOs": runner_os,
            "runnerArchitecture": runner_architecture,
            "toolchainVersion": toolchain_version(lane),
            "execution": lane.value["execution"],
        },
    }
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n", encoding="utf-8")


def execute(identifier: str, report_path: Path) -> None:
    matrix, lane = lane_by_id(identifier)
    toolchain = lane.value.get("toolchain")
    if toolchain in ("gcc", "clang"):
        execute_unix(lane)
    elif toolchain == "clang-cl":
        execute_windows(lane)
    elif toolchain == "arm-none-eabi-gcc":
        execute_metal()
    else:
        raise MatrixFailure(f"{lane.identifier} has unsupported toolchain {toolchain!r}")
    write_report(matrix, lane, report_path)
    print(f"platform-matrix: OK {lane.identifier}")


def github_plan() -> dict[str, list[dict[str, object]]]:
    _, lanes = load_matrix()
    return {"include": [lane.value for lane in lanes]}


def aggregate(report_root: Path, output: Path) -> None:
    matrix, lanes = load_matrix()
    reports: list[dict[str, object]] = []
    revision: str | None = None
    for lane in lanes:
        candidates = list(report_root.rglob(f"{lane.identifier}.json"))
        if len(candidates) != 1:
            raise MatrixFailure(
                f"{lane.identifier} requires exactly one report; found {len(candidates)}"
            )
        value: object = json.loads(candidates[0].read_text(encoding="utf-8"))
        if not isinstance(value, dict):
            raise MatrixFailure(f"{lane.identifier} report must be an object")
        if value.get("status") != "passed" or value.get("lane") != lane.value:
            raise MatrixFailure(f"{lane.identifier} report does not prove the planned lane")
        if value.get("observedEvidence") != list(lane.evidence):
            raise MatrixFailure(f"{lane.identifier} report evidence is incomplete")
        observation = value.get("observation")
        if not isinstance(observation, dict):
            raise MatrixFailure(f"{lane.identifier} report lacks runner/toolchain metadata")
        for field in ("runnerOs", "runnerArchitecture", "toolchainVersion"):
            required_string(observation.get(field), f"{lane.identifier}.{field}")
        if observation.get("execution") != lane.value["execution"]:
            raise MatrixFailure(f"{lane.identifier} report execution mode drifted")
        lane_revision = required_string(value.get("sourceRevision"), "sourceRevision")
        if revision is None:
            revision = lane_revision
        elif revision != lane_revision:
            raise MatrixFailure("platform reports refer to different source revisions")
        reports.append(value)
    result = {
        "schemaVersion": 1,
        "matrixId": matrix["matrixId"],
        "sourceRevision": revision,
        "status": "passed",
        "lanes": reports,
    }
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    print(f"platform-matrix: OK aggregated {len(reports)} release-blocking lanes")


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    subparsers = parser.add_subparsers(dest="command", required=True)
    subparsers.add_parser("check")
    plan = subparsers.add_parser("plan")
    plan.add_argument("--github-output", type=Path)
    execute_parser = subparsers.add_parser("execute")
    execute_parser.add_argument("--lane", required=True)
    execute_parser.add_argument("--report", type=Path, required=True)
    aggregate_parser = subparsers.add_parser("aggregate")
    aggregate_parser.add_argument("--reports", type=Path, required=True)
    aggregate_parser.add_argument("--output", type=Path, required=True)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    try:
        if args.command == "check":
            _, lanes = load_matrix()
            print(f"platform-matrix: OK {len(lanes)} release-blocking Tier-1 lanes")
        elif args.command == "plan":
            encoded = json.dumps(github_plan(), separators=(",", ":"))
            if args.github_output is None:
                print(encoded)
            else:
                with args.github_output.open("a", encoding="utf-8") as output:
                    output.write(f"matrix={encoded}\n")
        elif args.command == "execute":
            execute(args.lane, args.report)
        elif args.command == "aggregate":
            aggregate(args.reports, args.output)
        return 0
    except (MatrixFailure, OSError, UnicodeError, json.JSONDecodeError) as error:
        print(f"platform-matrix: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
