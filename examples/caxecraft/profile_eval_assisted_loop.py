#!/usr/bin/env python3
"""Compare fast Caxecraft edit loops without inventing a second game runtime.

The portable probe measures one shared Haxe rule under Eval and JavaScript.
The optional native probe keeps one haxe.c-built Caxecraft process alive and
measures bounded Piloscript requests through its existing agent-session path.
The report keeps compile feedback separate from native frame feedback because
neither measurement proves that the complete game runs under Eval.
"""

from __future__ import annotations

import argparse
import json
import math
import os
import selectors
import shutil
import signal
import statistics
import subprocess
import sys
import tempfile
import time
from collections.abc import Sequence
from dataclasses import dataclass
from pathlib import Path


CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
sys.path.insert(0, str(CASE))

from run import (  # noqa: E402
    CaxecraftFailure,
    haxe_compilation_server,
    pinned_haxe_environment,
    pinned_haxe_installation,
    verify_pinned_haxe,
)


SCHEMA_VERSION = 1
FIXTURE = CASE / "test/cross_target_adapter"
SHARED_RULE = Path("shared/caxecraft/adapterprobe/AdapterArchitectureProbe.hx")
EVAL_MAIN = "caxecraft.adapterprobe.AdapterArchitectureEvalMain"
JAVASCRIPT_MAIN = "caxecraft.adapterprobe.AdapterArchitectureProbe"
EXPECTED_RESULT = "0\n"
RESPONSE_PREFIX = "CAXECRAFT_AGENT_RESPONSE="


class EvalAssistedProfileFailure(RuntimeError):
    """The experiment could not produce complete, comparable evidence."""


@dataclass(frozen=True)
class CommandResult:
    """One successful command and its elapsed wall time."""

    elapsed_ms: float
    stdout: str


def elapsed_ms(started_ns: int) -> float:
    """Convert one monotonic interval to milliseconds."""

    return (time.monotonic_ns() - started_ns) / 1_000_000.0


def run_command(
    arguments: Sequence[str],
    *,
    cwd: Path,
    environment: dict[str, str],
    timeout_seconds: float,
    expected_stdout: str | None = None,
) -> CommandResult:
    """Run one bounded child and require its declared observable result."""

    started_ns = time.monotonic_ns()
    try:
        result = subprocess.run(
            list(arguments),
            cwd=cwd,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            timeout=timeout_seconds,
        )
    except subprocess.TimeoutExpired as error:
        raise EvalAssistedProfileFailure(
            f"command exceeded {timeout_seconds:g} seconds: {list(arguments)!r}"
        ) from error
    duration = elapsed_ms(started_ns)
    if result.returncode != 0:
        raise EvalAssistedProfileFailure(
            f"command failed with exit {result.returncode}: {list(arguments)!r}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if expected_stdout is not None and result.stdout != expected_stdout:
        raise EvalAssistedProfileFailure(
            f"command emitted {result.stdout!r}, expected {expected_stdout!r}: "
            f"{list(arguments)!r}"
        )
    return CommandResult(duration, result.stdout)


def haxe_arguments(source_root: Path) -> list[str]:
    """Return the shared portable class paths and one composition root."""

    return [
        "-cp",
        str(CASE / "src"),
        "-cp",
        str(source_root / "shared"),
        "-cp",
        str(source_root / "portable"),
        "-main",
        EVAL_MAIN,
    ]


def mutate_shared_rule(source_root: Path, edited: bool) -> None:
    """Switch one shared gameplay input and its independent expected result."""

    path = source_root / SHARED_RULE
    source = path.read_text(encoding="utf-8")
    before = ("final value = 513;", "final expected = 10;")
    after = ("final value = 514;", "final expected = 17;")
    old, new = (before, after) if edited else (after, before)
    if source.count(old[0]) != 1 or source.count(old[1]) != 1:
        raise EvalAssistedProfileFailure("portable edit anchors drifted")
    path.write_text(source.replace(old[0], new[0]).replace(old[1], new[1]), encoding="utf-8")


def run_eval(
    compiler: Path,
    source_root: Path,
    environment: dict[str, str],
    endpoint: str | None,
) -> CommandResult:
    """Compile and execute the shared rule through the Haxe Eval target."""

    arguments = [str(compiler)]
    if endpoint is not None:
        arguments.extend(("--connect", endpoint))
    arguments.extend(haxe_arguments(source_root))
    arguments.append("--interp")
    return run_command(
        arguments,
        cwd=CASE,
        environment=environment,
        timeout_seconds=30.0,
        expected_stdout=EXPECTED_RESULT,
    )


def run_javascript(
    compiler: Path,
    source_root: Path,
    output: Path,
    environment: dict[str, str],
    endpoint: str | None,
) -> CommandResult:
    """Compile the same shared rule to a non-executable JavaScript artifact."""

    arguments = [str(compiler)]
    if endpoint is not None:
        arguments.extend(("--connect", endpoint))
    arguments.extend(haxe_arguments(source_root))
    main_index = arguments.index(EVAL_MAIN)
    arguments[main_index] = JAVASCRIPT_MAIN
    arguments.extend(("--js", str(output)))
    started_ns = time.monotonic_ns()
    run_command(
        arguments,
        cwd=CASE,
        environment=environment,
        timeout_seconds=30.0,
    )
    if not output.is_file() or output.stat().st_size == 0:
        raise EvalAssistedProfileFailure("JavaScript target emitted no artifact")
    return CommandResult(elapsed_ms(started_ns), "")


def distribution(values: Sequence[float]) -> dict[str, float]:
    """Summarize a small sample without hiding its slowest observation."""

    if not values:
        raise EvalAssistedProfileFailure("a timing distribution cannot be empty")
    ordered = sorted(values)
    p95_index = max(0, math.ceil(len(ordered) * 0.95) - 1)
    median = statistics.median(ordered)
    deviations = [abs(value - median) for value in ordered]
    return {
        "medianMs": round(median, 3),
        "medianAbsoluteDeviationMs": round(statistics.median(deviations), 3),
        "p95Ms": round(ordered[p95_index], 3),
        "maximumMs": round(ordered[-1], 3),
    }


def haxe_code_line_count(root: Path) -> int:
    """Count nonblank Haxe lines after removing line and block comments."""

    count = 0
    in_block_comment = False
    for path in sorted(root.rglob("*.hx")):
        for source_line in path.read_text(encoding="utf-8").splitlines():
            index = 0
            code: list[str] = []
            while index < len(source_line):
                if in_block_comment:
                    end = source_line.find("*/", index)
                    if end < 0:
                        index = len(source_line)
                    else:
                        in_block_comment = False
                        index = end + 2
                elif source_line.startswith("//", index):
                    break
                elif source_line.startswith("/*", index):
                    in_block_comment = True
                    index += 2
                else:
                    code.append(source_line[index])
                    index += 1
            if "".join(code).strip():
                count += 1
    if in_block_comment:
        raise EvalAssistedProfileFailure(f"unterminated Haxe comment under {root}")
    return count


def portable_source_sharing() -> dict[str, object]:
    """Report the narrow probe's shared and target-owned Haxe line counts."""

    shared = haxe_code_line_count(FIXTURE / "shared")
    target = haxe_code_line_count(FIXTURE / "portable")
    total = shared + target
    if total == 0:
        raise EvalAssistedProfileFailure("portable probe has no executable Haxe lines")
    return {
        "metric": "nonblank authored Haxe lines after comment removal",
        "sharedLines": shared,
        "portableAdapterLines": target,
        "sharedPercentage": round(shared * 100.0 / total, 3),
        "scope": (
            "cross-target adapter fixture roots only; imported Caxecraft domain "
            "and the complete playable are excluded"
        ),
    }


def portable_profile(samples: int) -> dict[str, object]:
    """Measure cold, unchanged-server, and edited-server portable requests."""

    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    environment = pinned_haxe_environment("C", installation)
    measurements: dict[str, list[float]] = {
        "evalCold": [],
        "evalServerPrime": [],
        "evalWarmUnchanged": [],
        "evalWarmEdited": [],
        "javascriptCold": [],
        "javascriptServerPrime": [],
        "javascriptWarmUnchanged": [],
        "javascriptWarmEdited": [],
    }
    with haxe_compilation_server() as server:
        for sample in range(samples):
            with tempfile.TemporaryDirectory(prefix="caxecraft-eval-loop-") as temporary:
                source_root = Path(temporary) / "source"
                shutil.copytree(FIXTURE, source_root)
                measurements["evalCold"].append(
                    run_eval(installation.compiler, source_root, environment, None).elapsed_ms
                )
                measurements["evalServerPrime"].append(
                    run_eval(
                        installation.compiler,
                        source_root,
                        environment,
                        server.endpoint,
                    ).elapsed_ms
                )
                measurements["evalWarmUnchanged"].append(
                    run_eval(
                        installation.compiler,
                        source_root,
                        environment,
                        server.endpoint,
                    ).elapsed_ms
                )
                mutate_shared_rule(source_root, True)
                measurements["evalWarmEdited"].append(
                    run_eval(
                        installation.compiler,
                        source_root,
                        environment,
                        server.endpoint,
                    ).elapsed_ms
                )

                cold_javascript = Path(temporary) / "cold.js"
                measurements["javascriptCold"].append(
                    run_javascript(
                        installation.compiler,
                        source_root,
                        cold_javascript,
                        environment,
                        None,
                    ).elapsed_ms
                )
                warm_javascript = Path(temporary) / "warm.js"
                measurements["javascriptServerPrime"].append(
                    run_javascript(
                        installation.compiler,
                        source_root,
                        warm_javascript,
                        environment,
                        server.endpoint,
                    ).elapsed_ms
                )
                repeated_javascript = Path(temporary) / "repeated.js"
                measurements["javascriptWarmUnchanged"].append(
                    run_javascript(
                        installation.compiler,
                        source_root,
                        repeated_javascript,
                        environment,
                        server.endpoint,
                    ).elapsed_ms
                )
                mutate_shared_rule(source_root, False)
                edited_javascript = Path(temporary) / f"edited-{sample}.js"
                measurements["javascriptWarmEdited"].append(
                    run_javascript(
                        installation.compiler,
                        source_root,
                        edited_javascript,
                        environment,
                        server.endpoint,
                    ).elapsed_ms
                )
    return {
        "sampleCount": samples,
        "compiler": str(installation.compiler),
        "compilerVersion": installation.version,
        "sharedBehavior": "AdapterArchitectureProbe.selfCheck",
        "javascriptBoundary": "compile-to-artifact only; no JavaScript presentation adapter",
        "sourceSharing": portable_source_sharing(),
        "distributions": {
            name: distribution(values) for name, values in measurements.items()
        },
    }


def read_response(
    process: subprocess.Popen[str],
    selector: selectors.BaseSelector,
    timeout_seconds: float,
) -> dict[str, object]:
    """Read one bounded response from the existing native agent session."""

    deadline = time.monotonic() + timeout_seconds
    recent_lines: list[str] = []
    while True:
        remaining = deadline - time.monotonic()
        if remaining <= 0:
            raise EvalAssistedProfileFailure("native agent-session response timed out")
        if not selector.select(remaining):
            raise EvalAssistedProfileFailure("native agent-session response timed out")
        if process.stdout is None:
            raise EvalAssistedProfileFailure("native agent-session output pipe closed")
        line = process.stdout.readline()
        if line == "":
            raise EvalAssistedProfileFailure(
                f"native agent session exited early with status {process.poll()}\n"
                + "\n".join(recent_lines[-20:])
            )
        text = line.rstrip("\r\n")
        recent_lines.append(text)
        if not text.startswith(RESPONSE_PREFIX):
            continue
        try:
            response = json.loads(text[len(RESPONSE_PREFIX) :])
        except json.JSONDecodeError as error:
            raise EvalAssistedProfileFailure("native agent session emitted invalid JSON") from error
        if not isinstance(response, dict) or response.get("schemaVersion") != 1:
            raise EvalAssistedProfileFailure("native agent session emitted an unknown schema")
        if response.get("type") == "error":
            raise EvalAssistedProfileFailure(
                f"native agent session rejected a profile request: {response.get('message')}"
            )
        return response


def stop_owned_process(process: subprocess.Popen[str]) -> None:
    """Stop only the process group created by this profiler."""

    if process.stdin is not None:
        process.stdin.close()
    try:
        process.wait(timeout=5)
        return
    except subprocess.TimeoutExpired:
        pass
    os.killpg(process.pid, signal.SIGTERM)
    try:
        process.wait(timeout=5)
    except subprocess.TimeoutExpired:
        os.killpg(process.pid, signal.SIGKILL)
        process.wait(timeout=5)


def repository_tool_environment() -> dict[str, str]:
    """Expose the primary worktree's reviewed tools to a linked worktree."""

    result = subprocess.run(
        ("git", "rev-parse", "--git-common-dir"),
        cwd=ROOT,
        check=False,
        capture_output=True,
        text=True,
        timeout=10,
    )
    if result.returncode != 0:
        raise EvalAssistedProfileFailure("Git could not locate the common repository")
    common = Path(result.stdout.strip())
    if not common.is_absolute():
        common = (ROOT / common).resolve()
    primary_root = common.parent
    tool_directory = primary_root / "node_modules/.bin"
    haxe_shim = tool_directory / ("haxe.cmd" if os.name == "nt" else "haxe")
    if not haxe_shim.is_file():
        raise EvalAssistedProfileFailure(
            f"the primary worktree has no reviewed Haxe shim: {haxe_shim}"
        )
    environment = os.environ.copy()
    environment["PATH"] = str(tool_directory) + os.pathsep + environment.get("PATH", "")
    return environment


def git_source_state(repository_root: Path) -> dict[str, object]:
    """Record the exact checkout used for a native-session observation."""

    revision = run_command(
        ("git", "rev-parse", "HEAD"),
        cwd=repository_root,
        environment=os.environ.copy(),
        timeout_seconds=10.0,
    ).stdout.strip()
    status = run_command(
        ("git", "status", "--short"),
        cwd=repository_root,
        environment=os.environ.copy(),
        timeout_seconds=10.0,
    ).stdout
    return {"revision": revision, "dirty": bool(status)}


def native_session_profile(samples: int, case_root: Path) -> dict[str, object]:
    """Measure semantic request-to-screenshot feedback in one native process."""

    if case_root.is_symlink() or not (case_root / "play.py").is_file():
        raise EvalAssistedProfileFailure(
            "--native-session-root must name a real Caxecraft source directory"
        )
    command = [
        sys.executable,
        str(case_root / "play.py"),
        "--agent-session",
        "--raylib-configuration",
        "memory-software",
    ]
    started_ns = time.monotonic_ns()
    process = subprocess.Popen(
        command,
        cwd=case_root.parents[1],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        encoding="utf-8",
        errors="strict",
        bufsize=1,
        start_new_session=True,
        env=repository_tool_environment(),
    )
    selector = selectors.DefaultSelector()
    if process.stdout is None or process.stdin is None:
        stop_owned_process(process)
        raise EvalAssistedProfileFailure("native agent session did not expose both pipes")
    selector.register(process.stdout, selectors.EVENT_READ)
    durations: list[float] = []
    sequences: list[int] = []
    try:
        initial = read_response(process, selector, 180.0)
        startup_ms = elapsed_ms(started_ns)
        for index in range(samples):
            source = (
                "PILOSCRIPT 1\n"
                f"name agent-session-profile-{index + 1}\n"
                "frames 2\n"
                "action 0 idle\n"
                "end\n"
            )
            envelope = json.dumps({"piloscript": source}, separators=(",", ":"))
            request_started_ns = time.monotonic_ns()
            process.stdin.write(envelope + "\n")
            process.stdin.flush()
            response = read_response(process, selector, 20.0)
            durations.append(elapsed_ms(request_started_ns))
            sequence = response.get("sequence")
            if not isinstance(sequence, int):
                raise EvalAssistedProfileFailure("native response omitted its sequence")
            sequences.append(sequence)
            screenshot = response.get("screenshot")
            if screenshot != "caxecraft-agent-session.png":
                raise EvalAssistedProfileFailure("native response omitted its review frame")
        if sequences != sorted(sequences) or len(set(sequences)) != len(sequences):
            raise EvalAssistedProfileFailure("native response sequence did not increase")
        return {
            "sampleCount": samples,
            "source": {
                "checkout": git_source_state(case_root.parents[1]),
                "buildReceiptValidated": True,
            },
            "startupThroughFirstFrameMs": round(startup_ms, 3),
            "requestToScreenshot": distribution(durations),
            "framesPerRequest": 2,
            "perFrameLowerBoundMedianMs": round(statistics.median(durations) / 2.0, 3),
            "transport": "bounded Piloscript batch plus JSON observation",
            "renderer": "memory-software",
            "initialSequence": initial.get("sequence"),
            "finalSequence": sequences[-1],
        }
    finally:
        selector.close()
        stop_owned_process(process)


def host_report(operator_declared_representative: bool) -> dict[str, object]:
    """Record whether local contention permits a performance claim."""

    logical_cpus = os.cpu_count() or 1
    load = os.getloadavg() if hasattr(os, "getloadavg") else (0.0, 0.0, 0.0)
    return {
        "logicalCpuCount": logical_cpus,
        "loadAverage1m": round(load[0], 3),
        "loadAverage5m": round(load[1], 3),
        "loadAverage15m": round(load[2], 3),
        "loadBelowLogicalCpuCount": load[0] <= logical_cpus,
        "operatorDeclaredRepresentative": operator_declared_representative,
        "representativeForLatency": operator_declared_representative
        and load[0] <= logical_cpus,
    }


def self_test() -> None:
    """Exercise deterministic report math without starting a compiler."""

    summary = distribution((4.0, 1.0, 3.0, 2.0, 100.0))
    expected = {
        "medianMs": 3.0,
        "medianAbsoluteDeviationMs": 1.0,
        "p95Ms": 100.0,
        "maximumMs": 100.0,
    }
    if summary != expected:
        raise EvalAssistedProfileFailure(f"distribution self-test drifted: {summary!r}")


def main() -> int:
    """Run the selected bounded experiment and print one JSON report."""

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--samples", type=int, default=5)
    parser.add_argument("--native-session", action="store_true")
    parser.add_argument(
        "--native-session-root",
        type=Path,
        default=CASE,
        help="Caxecraft directory whose existing build cache supplies the native session",
    )
    parser.add_argument(
        "--representative-host",
        action="store_true",
        help="record that the operator inspected the host and permits latency claims",
    )
    parser.add_argument("--self-test", action="store_true")
    arguments = parser.parse_args()
    try:
        if arguments.samples < 1 or arguments.samples > 20:
            raise EvalAssistedProfileFailure("--samples must be from 1 through 20")
        if arguments.self_test:
            self_test()
            print("caxecraft-eval-assisted-loop: self-test OK")
            return 0
        report: dict[str, object] = {
            "schemaVersion": SCHEMA_VERSION,
            "host": host_report(arguments.representative_host),
            "portable": portable_profile(arguments.samples),
            "nativeSession": native_session_profile(
                arguments.samples,
                arguments.native_session_root.resolve(),
            )
            if arguments.native_session
            else None,
            "claimBoundary": (
                "Portable timings prove shared-rule feedback only. Native-session timings "
                "prove content/control feedback through the haxe.c-built game. Neither path "
                "proves that the complete game runs under Eval or JavaScript."
            ),
        }
        print(json.dumps(report, indent=2, sort_keys=True))
    except (
        CaxecraftFailure,
        EvalAssistedProfileFailure,
        OSError,
        subprocess.SubprocessError,
    ) as error:
        print(f"caxecraft-eval-assisted-loop: ERROR: {error}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
