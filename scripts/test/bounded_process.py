#!/usr/bin/env python3
"""Run a bounded command without leaving its child processes active.

Python's ``subprocess.run`` stops only the process that it started when a
timeout expires. A command wrapper can leave its compiler child active. This
module starts each bounded command in a separate operating-system process
group and stops that complete group after a timeout.

Successful commands return the same ``CompletedProcess`` data as
``subprocess.run``. POSIX hosts use a new session as the process group.
Windows hosts use a new process group and ``taskkill /T /F``.
"""

from __future__ import annotations

import math
import os
import re
import signal
import subprocess
import time
from collections.abc import Sequence
from typing import Any

from scripts.test.compiler_progress import compiler_timeout_suffix


TIMEOUT_SCALE_ENV = "HXC_TEST_TIMEOUT_SCALE"
TIMEOUT_SCALE_PATTERN = re.compile(r"(?:[1-9]|[12][0-9]|30)")


class ProcessTreeTimeout(subprocess.TimeoutExpired):
    """Report which bounded phase stopped and how long it was allowed to run."""

    def __init__(
        self,
        command: Sequence[str | os.PathLike[str]] | str | os.PathLike[str],
        timeout: float,
        *,
        phase: str,
        output: str | bytes | None,
        stderr: str | bytes | None,
        detail_suffix: str = "",
    ) -> None:
        super().__init__(command, timeout, output=output, stderr=stderr)
        self.phase = phase
        self.detail_suffix = detail_suffix

    def __str__(self) -> str:
        return (
            f"{self.phase} timed out after {self.timeout} seconds: {self.cmd!r}"
            f"{self.detail_suffix}"
        )


def effective_timeout(timeout: float) -> float:
    """Apply one explicit bounded slowdown factor to a test deadline.

    The default remains exact. A busy local host can select an integer from 1
    through 30. Invalid values stop before the child process starts, so a typo
    cannot silently remove or shorten the bound.
    """

    raw_scale = os.environ.get(TIMEOUT_SCALE_ENV)
    if raw_scale is None:
        return timeout
    if TIMEOUT_SCALE_PATTERN.fullmatch(raw_scale) is None:
        raise ValueError(f"{TIMEOUT_SCALE_ENV} must be an integer from 1 through 30")
    return timeout * int(raw_scale)


def _windows_creation_flags(existing: int, new_process_group: int) -> int:
    """Keep caller flags and add the Windows process-group ownership flag."""

    return existing | new_process_group


def _terminate_windows_tree(process: subprocess.Popen[Any], *, timeout: float = 10) -> str:
    """Stop one Windows process and all descendants that still belong to it."""

    try:
        result = subprocess.run(
            ["taskkill", "/PID", str(process.pid), "/T", "/F"],
            check=False,
            capture_output=True,
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired):
        result = None
    if (result is None or result.returncode != 0) and process.poll() is None:
        process.kill()
    return "" if result is not None and result.returncode == 0 else "Windows tree termination was not confirmed"


def _terminate_process_tree(process: subprocess.Popen[Any], *, timeout: float = 10) -> str:
    """Stop the process group created for one bounded command."""

    if os.name == "nt":
        return _terminate_windows_tree(process, timeout=timeout)
    try:
        os.killpg(process.pid, signal.SIGKILL)
    except ProcessLookupError:
        pass
    return ""


def _stop_and_drain(
    process: subprocess.Popen[Any], failure: subprocess.TimeoutExpired,
    *, cleanup_timeout: float,
) -> tuple[str | bytes | None, str | bytes | None, str]:
    """Keep timeout cleanup finite even if an escaped child holds a pipe open.

    The original deadline has already failed. This separate allowance only
    stops owned processes, drains output, and reaps the immediate child.
    Detached descendants are outside the process-group contract; report a
    failed drain instead of waiting indefinitely for their output pipes.
    """

    end = time.monotonic() + cleanup_timeout
    problems: list[str] = []
    try:
        problem = _terminate_process_tree(process, timeout=cleanup_timeout)
        if problem:
            problems.append(problem)
    except OSError as error:
        problems.append(f"tree termination failed: {type(error).__name__}")
    stdout, stderr = failure.output, failure.stderr
    try:
        stdout, stderr = process.communicate(timeout=max(0, end - time.monotonic()))
    except (subprocess.TimeoutExpired, OSError, UnicodeError) as error:
        if isinstance(error, subprocess.TimeoutExpired):
            # communicate retains accumulated output; do not concatenate it twice.
            stdout = error.output if error.output is not None else stdout
            stderr = error.stderr if error.stderr is not None else stderr
            problems.append("output drain exceeded the cleanup allowance")
        else:
            # Text decoding can fail after the child exits. Keep the original
            # timeout's raw bytes instead of replacing it with a decode error.
            problems.append(f"output drain failed: {type(error).__name__}")
        try:
            process.wait(timeout=max(0, end - time.monotonic()))
        except subprocess.TimeoutExpired:
            problems.append("immediate child was not reaped")
    return stdout, stderr, "; ".join(problems)


def run(
    command: Sequence[str | os.PathLike[str]] | str | os.PathLike[str],
    *,
    phase: str = "bounded command",
    timeout: float,
    input: str | bytes | None = None,
    capture_output: bool = False,
    check: bool = False,
    compiler_progress: bool = False,
    honor_timeout_scale: bool = True,
    cleanup_timeout: float = 5,
    **popen_arguments: Any,
) -> subprocess.CompletedProcess[Any]:
    """Run one command and stop its complete process group after ``timeout``.

    A caller that requests haxe.c's lightweight progress define can set
    ``compiler_progress``. Timeout text then names the last active compiler
    phase, or states that the request never left the Haxe frontend.

    Transport contracts can set ``honor_timeout_scale=False`` to keep their
    exact deadline without changing the child environment. After expiration,
    cleanup has its own fixed allowance and can never turn failure into success.
    """

    if not math.isfinite(cleanup_timeout) or cleanup_timeout <= 0:
        raise ValueError("cleanup_timeout must be finite and positive")
    deadline = effective_timeout(timeout) if honor_timeout_scale else timeout

    if input is not None:
        if "stdin" in popen_arguments:
            raise ValueError("stdin and input arguments cannot both be used")
        popen_arguments["stdin"] = subprocess.PIPE
    if capture_output:
        if "stdout" in popen_arguments or "stderr" in popen_arguments:
            raise ValueError(
                "stdout and stderr arguments cannot be used with capture_output"
            )
        popen_arguments["stdout"] = subprocess.PIPE
        popen_arguments["stderr"] = subprocess.PIPE

    if os.name == "nt":
        existing_flags = int(popen_arguments.pop("creationflags", 0))
        new_process_group = subprocess.CREATE_NEW_PROCESS_GROUP  # type: ignore[attr-defined]
        popen_arguments["creationflags"] = _windows_creation_flags(
            existing_flags, new_process_group
        )
    else:
        popen_arguments["start_new_session"] = True

    process = subprocess.Popen(command, **popen_arguments)
    drained = False
    try:
        try:
            stdout, stderr = process.communicate(input, timeout=deadline)
            drained = True
        except subprocess.TimeoutExpired as failure:
            stdout, stderr, cleanup_problem = _stop_and_drain(
                process, failure, cleanup_timeout=cleanup_timeout,
            )
            drained = not cleanup_problem
            detail_suffix = (
                compiler_timeout_suffix(
                    stdout,
                    stderr,
                    missing_status=(
                        "no haxe.c phase marker; request remained in Haxe frontend or server startup"
                    ),
                )
                if compiler_progress
                else ""
            )
            if cleanup_problem:
                detail_suffix += f"; cleanup incomplete: {cleanup_problem}"
            raise ProcessTreeTimeout(
                command,
                deadline,
                phase=phase,
                output=stdout,
                stderr=stderr,
                detail_suffix=detail_suffix,
            ) from None
        except BaseException as failure:
            # An interrupted caller still owns the subprocess it started.
            _, _, cleanup_problem = _stop_and_drain(
                process, subprocess.TimeoutExpired(command, deadline),
                cleanup_timeout=cleanup_timeout,
            )
            drained = not cleanup_problem
            if cleanup_problem:
                failure.add_note(f"process cleanup incomplete: {cleanup_problem}")
            raise

        return_code = process.poll()
        if check and return_code:
            raise subprocess.CalledProcessError(
                return_code, command, output=stdout, stderr=stderr
            )
    finally:
        # A Popen context manager would wait without a deadline here. On POSIX,
        # communicate has no reader threads and closing these streams is safe.
        # Windows reader threads may still own a stream after a failed drain;
        # retain it for that reader rather than block on its stream lock.
        if drained or os.name != "nt":
            for stream in (process.stdin, process.stdout, process.stderr):
                if stream is not None:
                    stream.close()
    return subprocess.CompletedProcess(command, return_code, stdout, stderr)
