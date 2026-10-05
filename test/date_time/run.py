#!/usr/bin/env python3
"""Prove portable Date arithmetic and the hosted clock/timezone boundary."""

from __future__ import annotations

import json
import os
import platform
import shutil
import socket
import subprocess
import sys
import tempfile
import time
from contextlib import contextmanager
from pathlib import Path
from typing import Iterator


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402


CASE = Path(__file__).resolve().parent
FIXTURES = CASE / "fixtures"
RUNTIME_INCLUDE = ROOT / "runtime/hxrt/include"
RUNTIME_SOURCE = ROOT / "runtime/hxrt/src/date_time.c"
RUNTIME_CONTRACT = ROOT / "runtime/hxrt/test/date_time_contract.c"
CENTRAL_TZ = "CST6CDT,M3.2.0/2,M11.1.0/2"
TIMEZONES = ("UTC0", "IST-5:30", CENTRAL_TZ)
STRICT_C_FLAGS = (
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
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fno-sanitize-recover=all",
    "-fsanitize=address,undefined",
)


class DateTimeFailure(RuntimeError):
    """Report one focused Date/time acceptance failure."""


def development_tool(name: str) -> str:
    """Use the repository-pinned tool shim when it exists."""
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def background_command(command: list[str]) -> list[str]:
    """Lower one expensive process root while an interactive editor shares the host."""
    nice = shutil.which("nice")
    taskpolicy = shutil.which("taskpolicy")
    if platform.system() == "Darwin" and taskpolicy is not None and nice is not None:
        return [taskpolicy, "-b", nice, "-n", "10", *command]
    if nice is not None:
        return [nice, "-n", "10", *command]
    return command


def haxe_environment(*, server: bool) -> dict[str, str]:
    """Keep compiler-server reuse explicit and local to this runner."""
    environment = os.environ.copy()
    if server:
        environment.pop("HAXE_NO_SERVER", None)
    else:
        environment["HAXE_NO_SERVER"] = "1"
    return environment


def available_port() -> int:
    """Reserve an ephemeral loopback port for the task-owned Haxe server."""
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as probe:
        probe.bind(("127.0.0.1", 0))
        return int(probe.getsockname()[1])


def wait_for_server(server: subprocess.Popen[str], port: int) -> None:
    """Wait until the owned server accepts requests or fails visibly."""
    deadline = time.monotonic() + 20.0
    while time.monotonic() < deadline:
        if server.poll() is not None:
            stdout, stderr = server.communicate()
            raise DateTimeFailure(
                f"Haxe server stopped during startup: {stdout!r} {stderr!r}"
            )
        try:
            with socket.create_connection(("127.0.0.1", port), timeout=0.2):
                return
        except OSError:
            time.sleep(0.05)
    raise DateTimeFailure("Haxe server did not accept date-time fixture requests")


@contextmanager
def haxe_server() -> Iterator[str]:
    """Own one warm compiler server and stop its complete root at runner exit."""
    port = available_port()
    endpoint = str(port)
    server = subprocess.Popen(
        background_command([development_tool("haxe"), "--wait", endpoint]),
        cwd=ROOT,
        env=haxe_environment(server=True),
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )
    try:
        wait_for_server(server, port)
        yield endpoint
    finally:
        server.terminate()
        try:
            server.wait(timeout=5)
        except subprocess.TimeoutExpired:
            server.kill()
            server.wait(timeout=5)


def compile_haxe(
    fixture: Path,
    output: Path,
    endpoint: str,
    *,
    defines: tuple[str, ...] = (),
) -> subprocess.CompletedProcess[str]:
    """Compile one ordinary Haxe fixture through the real C target."""
    command = [
        development_tool("haxe"),
        "--connect",
        endpoint,
        "-cp",
        str(fixture),
        "-lib",
        "reflaxe.c",
        "-main",
        "Main",
        "-D",
        "hxc_project_layout=split",
        "-D",
        "hxc_runtime_report=full",
        "-D",
        "hxc_runtime_diagnostics=off",
    ]
    for define in defines:
        command.extend(("-D", define))
    command.extend(("--custom-target", f"c={output}"))
    return run_bounded_process(
        command,
        cwd=ROOT,
        env=haxe_environment(server=True),
        check=False,
        capture_output=True,
        text=True,
        timeout=300,
        phase=f"date-time Haxe compile ({fixture.name})",
    )


def require_compile_success(
    label: str, result: subprocess.CompletedProcess[str]
) -> None:
    """Reject warnings and diagnostics from a positive target compile."""
    if result.returncode != 0 or result.stdout or result.stderr:
        raise DateTimeFailure(
            f"{label} compile failed: exit={result.returncode} "
            f"stdout={result.stdout!r} stderr={result.stderr!r}"
        )


def plausible_output_exists(output: Path) -> bool:
    """Detect a negative compile that left target artifacts behind."""
    return output.exists() and any(output.rglob("*"))


def render_projects(root: Path, endpoint: str) -> dict[str, Path]:
    """Render positive projects and prove unavailable surfaces fail before C."""
    projects: dict[str, Path] = {}
    for name in ("pure", "utc", "local", "oracle"):
        output = root / name
        result = compile_haxe(FIXTURES / name, output, endpoint)
        require_compile_success(name, result)
        projects[name] = output

    for environment in ("freestanding", "wasi"):
        output = root / f"unsupported-{environment}"
        result = compile_haxe(
            FIXTURES / "unsupported",
            output,
            endpoint,
            defines=(f"hxc_environment={environment}",),
        )
        if (
            result.returncode == 0
            or "HXC1000" not in result.stderr
            or "direct executable entry emission currently requires the hosted environment"
            not in result.stderr
            or f"`{environment}` remains fail-closed" not in result.stderr
            or plausible_output_exists(output)
        ):
            raise DateTimeFailure(
                f"{environment} did not fail closed before hosted services: "
                f"{result.stderr!r}"
            )

    output = root / "from-string"
    result = compile_haxe(FIXTURES / "from_string", output, endpoint)
    if (
        result.returncode == 0
        or "HXC1001" not in result.stderr
        or "TCall(Date.fromString:not-yet-admitted)" not in result.stderr
        or plausible_output_exists(output)
    ):
        raise DateTimeFailure(
            f"Date.fromString gap lost its source failure: {result.stderr!r}"
        )
    return projects


def load_json(path: Path) -> dict[str, object]:
    """Read one generated report as a text-keyed JSON object."""
    value = json.loads(path.read_text(encoding="utf-8"))
    if not isinstance(value, dict):
        raise DateTimeFailure(f"generated report is not an object: {path}")
    return value


def selected_features(project: Path) -> dict[str, dict[str, object]]:
    """Index one runtime plan's selected feature records by stable ID."""
    plan = load_json(project / "hxc.runtime-plan.json")
    raw_features = plan.get("selectedFeatures")
    if not isinstance(raw_features, list):
        raise DateTimeFailure("runtime plan omitted selectedFeatures")
    result: dict[str, dict[str, object]] = {}
    for raw_feature in raw_features:
        if not isinstance(raw_feature, dict) or not isinstance(
            raw_feature.get("id"), str
        ):
            raise DateTimeFailure("runtime plan has a malformed selected feature")
        result[str(raw_feature["id"])] = raw_feature
    return result


def inspect_generated_projects(projects: dict[str, Path]) -> None:
    """Prove Date identity and clock-kind selection remain visible in generated C."""
    catalog = load_json(ROOT / "runtime/hxrt/features.json")
    catalog_features = catalog.get("features")
    if not isinstance(catalog_features, list):
        raise DateTimeFailure("runtime feature catalog omitted its feature records")
    date_catalog = [
        feature
        for feature in catalog_features
        if isinstance(feature, dict) and feature.get("id") == "date-time"
    ]
    if len(date_catalog) != 1 or date_catalog[0].get("environments") != ["hosted"]:
        raise DateTimeFailure("date-time catalog availability is not hosted-only")

    for name in ("pure", "utc"):
        if "date-time" in selected_features(projects[name]):
            raise DateTimeFailure(f"{name} selected hosted date-time services")

    local = projects["local"]
    date_time = selected_features(local).get("date-time")
    expected_symbols = {
        "hxc_date_time_wall_milliseconds",
        "hxc_date_time_monotonic_seconds",
        "hxc_date_time_local_to_milliseconds",
        "hxc_date_time_timezone_offset",
    }
    if date_time is None or set(date_time.get("symbols", [])) != expected_symbols:
        raise DateTimeFailure("local Date plan lost its exact hosted symbol closure")

    date_header = (local / "include/hxc/modules/Date.h").read_text(
        encoding="utf-8"
    )
    if (
        "struct hxc_Date" not in date_header
        or "double hxc_milliseconds;" not in date_header
    ):
        raise DateTimeFailure("generated Date lost nominal Float millisecond storage")

    generated_c = "\n".join(
        path.read_text(encoding="utf-8")
        for path in sorted((local / "src").rglob("*.c"))
    )
    for symbol in expected_symbols:
        if f"{symbol}(" not in generated_c:
            raise DateTimeFailure(f"generated local Date omitted {symbol}")
    if generated_c.count("hxc_gc_allocate(") < 2:
        raise DateTimeFailure(
            "Date.fromTime no longer creates independently rooted objects"
        )

    report = (local / "hxc.runtime-plan.json").read_text(encoding="utf-8")
    for operation in (
        "wall-milliseconds",
        "monotonic-seconds",
        "local-to-milliseconds",
        "timezone-offset",
    ):
        if operation not in report:
            raise DateTimeFailure(f"runtime plan does not explain {operation}")


def string_list(value: object, label: str) -> list[str]:
    """Validate one generated manifest string array."""
    if not isinstance(value, list) or not all(
        isinstance(item, str) for item in value
    ):
        raise DateTimeFailure(f"{label} must be a string array")
    return list(value)


def project_build_inputs(project: Path) -> tuple[list[Path], list[Path]]:
    """Resolve compiler-owned sources and include roots from the manifest."""
    manifest = load_json(project / "hxc.manifest.json")
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise DateTimeFailure("generated manifest omitted its build plan")
    sources = [
        (project / value).resolve()
        for value in string_list(build.get("sources"), "manifest sources")
    ]
    includes = [
        (project / value).resolve()
        for value in string_list(
            build.get("includeDirectories"), "manifest include directories"
        )
    ]
    if not all(path.exists() for path in (*sources, *includes)):
        raise DateTimeFailure("generated manifest names a missing build input")
    return sources, includes


def compiler_family(executable: str) -> str:
    """Classify a native compiler without trusting its command name."""
    result = run_bounded_process(
        [executable, "--version"],
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    output = (result.stdout + result.stderr).lower()
    if "clang" in output:
        return "clang"
    if "gcc" in output or "free software foundation" in output:
        return "gcc"
    return "unknown"


def native_compilers() -> list[tuple[str, str]]:
    """Return each available, identity-distinct strict C compiler family."""
    found: dict[str, str] = {}
    for name in ("clang", "gcc", "cc"):
        executable = shutil.which(name)
        if executable is None:
            continue
        family = compiler_family(executable)
        if family in ("clang", "gcc") and family not in found:
            found[family] = executable
    if not found:
        raise DateTimeFailure("date-time evidence requires Clang or GCC")
    return sorted(found.items())


def compile_generated(
    compiler: str,
    project: Path,
    executable: Path,
    *,
    sanitizer: bool = False,
) -> None:
    """Compile one generated project as strict C11."""
    sources, includes = project_build_inputs(project)
    command = [
        compiler,
        *STRICT_C_FLAGS,
        *(SANITIZER_FLAGS if sanitizer else ("-O2",)),
        *(f"-I{include}" for include in includes),
        *(str(source) for source in sources),
        "-o",
        str(executable),
    ]
    result = run_bounded_process(
        background_command(command),
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=180,
        phase="date-time strict generated C compile",
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise DateTimeFailure(
            f"strict generated C compile failed: {result.stdout!r} {result.stderr!r}"
        )


def run_executable(
    executable: Path, *, timezone: str | None = None, sanitizer: bool = False
) -> str:
    """Run one native probe with deterministic locale and optional timezone."""
    environment = os.environ.copy()
    environment["LC_ALL"] = "C"
    if timezone is not None:
        environment["TZ"] = timezone
    if sanitizer:
        environment["ASAN_OPTIONS"] = "detect_leaks=0"
        environment["UBSAN_OPTIONS"] = "halt_on_error=1"
    result = run_bounded_process(
        [str(executable)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise DateTimeFailure(
            f"native date-time probe failed: exit={result.returncode} "
            f"stdout={result.stdout!r} stderr={result.stderr!r}"
        )
    return result.stdout


def compile_runtime_contract(
    family: str, compiler: str, build: Path, *, sanitizer: bool
) -> None:
    """Compile and run the status/out ABI against controlled host timezone data."""
    suffix = "sanitizer" if sanitizer else "strict"
    executable = build / f"runtime-{family}-{suffix}"
    command = [
        compiler,
        *STRICT_C_FLAGS,
        *(SANITIZER_FLAGS if sanitizer else ("-O2",)),
        f"-I{RUNTIME_INCLUDE}",
        str(RUNTIME_SOURCE),
        str(RUNTIME_CONTRACT),
        "-o",
        str(executable),
    ]
    result = run_bounded_process(
        background_command(command),
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=90,
        phase="date-time runtime contract compile",
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise DateTimeFailure(
            f"{family} runtime contract compile failed: "
            f"{result.stdout!r} {result.stderr!r}"
        )
    if run_executable(
        executable, timezone=CENTRAL_TZ, sanitizer=sanitizer
    ) != "date-time-runtime-ok\n":
        raise DateTimeFailure(f"{family} runtime contract output drifted")


def compile_cpp_header(family: str, project: Path, build: Path) -> None:
    """Prove the generated and runtime Date headers remain valid C++17."""
    compiler_name = "clang++" if family == "clang" else "g++"
    compiler = shutil.which(compiler_name)
    if compiler is None:
        print(
            f"date-time: SKIP optional {family} C++17 headers: "
            f"missing {compiler_name}"
        )
        return
    _, includes = project_build_inputs(project)
    executable = build / f"headers-{family}"
    source = (
        '#include "hxc/program.h"\n'
        '#include "hxrt/date_time.h"\n'
        "int main() { return 0; }\n"
    )
    command = [
        compiler,
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-Werror",
        "-pedantic-errors",
        *(f"-I{include}" for include in includes),
        "-x",
        "c++",
        "-",
        "-o",
        str(executable),
    ]
    result = run_bounded_process(
        background_command(command),
        cwd=ROOT,
        input=source,
        check=False,
        capture_output=True,
        text=True,
        timeout=60,
    )
    if result.returncode != 0 or result.stdout or result.stderr:
        raise DateTimeFailure(
            f"{family} C++17 header check failed: {result.stdout!r} {result.stderr!r}"
        )
    run_executable(executable)


def run_eval(timezone: str) -> str:
    """Run the pinned Eval implementation in a fresh controlled process."""
    environment = haxe_environment(server=False)
    environment["LC_ALL"] = "C"
    environment["TZ"] = timezone
    result = run_bounded_process(
        [development_tool("haxe"), "build.hxml"],
        cwd=FIXTURES / "oracle",
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise DateTimeFailure(
            f"Eval Date oracle failed for {timezone}: {result.stderr!r}"
        )
    return result.stdout


def compare_eval_and_native(native: str, timezone: str) -> None:
    """Compare fixed instants exactly and validate host-selected repeated times."""
    evaluated = run_eval(timezone)
    if timezone != CENTRAL_TZ:
        if native != evaluated:
            raise DateTimeFailure(f"native Date diverged from Eval in {timezone}")
        return

    eval_bug = (
        "spring-before|time=1.710057599e+12|utc=2024-3-10T7:59:59|"
        "local=2024-03-10 01:59:59|offset=300"
    )
    corrected = eval_bug.removesuffix("300") + "360"
    if eval_bug not in evaluated:
        raise DateTimeFailure("pinned Eval's documented pre-DST offset changed")
    if corrected not in native:
        raise DateTimeFailure(
            "native Date lost the civil-time-consistent pre-DST offset"
        )
    evaluated = evaluated.replace(eval_bug, corrected)

    # Linux Eval reports standard time for the first (daylight) occurrence.
    fall_first = (
        "fall-first|time=1.7306154e+12|utc=2024-11-3T6:30:0|"
        "local=2024-11-03 01:30:00|offset="
    )
    if fall_first + "360" in evaluated:
        evaluated = evaluated.replace(fall_first + "360", fall_first + "300")
    if fall_first + "300" not in evaluated or fall_first + "300" not in native:
        raise DateTimeFailure("first fall-back instant lost its daylight offset")

    # mktime may select either occurrence of a repeated local time. Check the
    # timestamp and offset together; never erase the offset from comparisons.
    daylight = "fall-overlap|time=1.7306154e+12|local=2024-11-03 01:30:00|offset=300"
    standard = "fall-overlap|time=1.730619e+12|local=2024-11-03 01:30:00|offset=360"
    eval_overlap_bug = daylight.removesuffix("300") + "360"
    native_lines = native.splitlines(keepends=True)
    eval_lines = evaluated.splitlines(keepends=True)
    native_overlap = [line for line in native_lines if line.startswith("fall-overlap|")]
    eval_overlap = [line for line in eval_lines if line.startswith("fall-overlap|")]
    if len(native_overlap) != 1 or native_overlap[0] not in (daylight + "\n", standard + "\n"):
        raise DateTimeFailure("native repeated local time has an invalid timestamp/offset pair")
    if len(eval_overlap) != 1 or eval_overlap[0] not in (
        daylight + "\n", standard + "\n", eval_overlap_bug + "\n"
    ):
        raise DateTimeFailure("pinned Eval repeated local time changed")
    native_lines.remove(native_overlap[0])
    eval_lines.remove(eval_overlap[0])
    if native_lines != eval_lines:
        raise DateTimeFailure("native Date has an unreviewed Central-time divergence")


def run_generated_native(
    projects: dict[str, Path], compilers: list[tuple[str, str]], build: Path
) -> None:
    """Run every generated path under strict native compilation and one sanitizer."""
    expected = {
        "pure": "date-time-pure-ok\n",
        "utc": "date-time-utc-ok\n",
        "local": "date-time-local-ok\n",
    }
    for family, compiler in compilers:
        family_build = build / family
        family_build.mkdir()
        for name in ("pure", "utc", "local", "oracle"):
            executable = family_build / name
            compile_generated(compiler, projects[name], executable)
            if name == "oracle":
                for timezone in TIMEZONES:
                    compare_eval_and_native(
                        run_executable(executable, timezone=timezone), timezone
                    )
            else:
                timezone = CENTRAL_TZ if name == "local" else "UTC0"
                if run_executable(executable, timezone=timezone) != expected[name]:
                    raise DateTimeFailure(f"{family} {name} output drifted")

        compile_runtime_contract(family, compiler, family_build, sanitizer=False)
        compile_cpp_header(family, projects["local"], family_build)

        if family == "clang":
            executable = family_build / "local-sanitizer"
            compile_generated(
                compiler, projects["local"], executable, sanitizer=True
            )
            if run_executable(
                executable, timezone=CENTRAL_TZ, sanitizer=True
            ) != expected["local"]:
                raise DateTimeFailure("sanitized generated local Date output drifted")
            compile_runtime_contract(
                family, compiler, family_build, sanitizer=True
            )


def main() -> int:
    """Run the complete focused Date/time acceptance lane."""
    compilers = native_compilers()
    with tempfile.TemporaryDirectory(prefix="hxc-date-time-") as directory:
        root = Path(directory)
        (root / "projects").mkdir()
        with haxe_server() as endpoint:
            projects = render_projects(root / "projects", endpoint)
        inspect_generated_projects(projects)
        build = root / "native"
        build.mkdir()
        run_generated_native(projects, compilers, build)
    print("date-time: OK")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except DateTimeFailure as error:
        print(f"date-time: FAIL: {error}", file=sys.stderr)
        raise SystemExit(1) from error
