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

import os
import signal
import subprocess
from collections.abc import Sequence
from typing import Any


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
    ) -> None:
        super().__init__(command, timeout, output=output, stderr=stderr)
        self.phase = phase

    def __str__(self) -> str:
        return f"{self.phase} timed out after {self.timeout} seconds: {self.cmd!r}"


def _windows_creation_flags(existing: int, new_process_group: int) -> int:
    """Keep caller flags and add the Windows process-group ownership flag."""

    return existing | new_process_group


def _terminate_windows_tree(process: subprocess.Popen[Any]) -> None:
    """Stop one Windows process and all descendants that still belong to it."""

    try:
        result = subprocess.run(
            ["taskkill", "/PID", str(process.pid), "/T", "/F"],
            check=False,
            capture_output=True,
            timeout=10,
        )
    except (OSError, subprocess.TimeoutExpired):
        result = None
    if (result is None or result.returncode != 0) and process.poll() is None:
        process.kill()


def _terminate_process_tree(process: subprocess.Popen[Any]) -> None:
    """Stop the process group created for one bounded command."""

    if os.name == "nt":
        _terminate_windows_tree(process)
        return
    try:
        os.killpg(process.pid, signal.SIGKILL)
    except ProcessLookupError:
        pass


def run(
    command: Sequence[str | os.PathLike[str]] | str | os.PathLike[str],
    *,
    phase: str = "bounded command",
    timeout: float,
    input: str | bytes | None = None,
    capture_output: bool = False,
    check: bool = False,
    **popen_arguments: Any,
) -> subprocess.CompletedProcess[Any]:
    """Run one command and stop its complete process group after ``timeout``."""

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

    with subprocess.Popen(command, **popen_arguments) as process:
        try:
            stdout, stderr = process.communicate(input, timeout=timeout)
        except subprocess.TimeoutExpired:
            _terminate_process_tree(process)
            stdout, stderr = process.communicate()
            raise ProcessTreeTimeout(
                command,
                timeout,
                phase=phase,
                output=stdout,
                stderr=stderr,
            ) from None

        return_code = process.poll()
        if check and return_code:
            raise subprocess.CalledProcessError(
                return_code, command, output=stdout, stderr=stderr
            )
    return subprocess.CompletedProcess(command, return_code, stdout, stderr)
