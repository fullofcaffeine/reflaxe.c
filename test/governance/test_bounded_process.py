#!/usr/bin/env python3
"""Prove that bounded test commands stop their complete process trees."""

from __future__ import annotations

import ast
import os
import subprocess
import sys
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test import bounded_process  # noqa: E402


CANONICAL_EXAMPLE_RUNNERS = (
    ROOT / "examples/caxecraft/check_pilot.py",
    ROOT / "examples/caxecraft/run.py",
    ROOT / "examples/caxecraft/run_haxe_c_test.py",
    ROOT / "examples/hello/run.py",
)


CHILD_PROGRAM = """
import os
import subprocess
import sys
import time

pid_path = sys.argv[1]
time.sleep(float(sys.argv[2]))
grandchild = subprocess.Popen([sys.executable, "-c", "import time; time.sleep(60)"])
with open(pid_path, "a", encoding="utf-8") as output:
    output.write(f"{os.getpid()}\\n{grandchild.pid}\\n")
    output.flush()
time.sleep(60)
"""

ROOT_PROGRAM = """
import os
import subprocess
import sys
import time

pid_path = sys.argv[1]
child_program = sys.argv[2]
child = subprocess.Popen([sys.executable, "-c", child_program, pid_path, sys.argv[3]])
with open(pid_path, "a", encoding="utf-8") as output:
    output.write(f"{os.getpid()}\\n")
    output.flush()
time.sleep(60)
"""


def process_is_active(pid: int, *, timeout: float = 3) -> bool:
    """Return true only while a process can still consume host resources."""

    if os.name == "nt":
        result = subprocess.run(
            ["tasklist", "/FI", f"PID eq {pid}", "/FO", "CSV", "/NH"],
            check=False,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
        return f'"{pid}"' in result.stdout
    result = subprocess.run(
        ["ps", "-o", "stat=", "-p", str(pid)],
        check=False,
        capture_output=True,
        text=True,
        timeout=timeout,
    )
    state = result.stdout.strip()
    return result.returncode == 0 and bool(state) and not state.startswith("Z")


def run_ready_tree(command: list[str], pid_path: Path, *, setup_timeout: float = 15) -> None:
    """Start the cleanup clock only after this real process tree is ready.

    Python startup can exceed the cleanup deadline on a busy host. The test's
    Popen adapter waits separately for all three live identities, then hands
    the real process to the unchanged production timeout and termination path.
    Other subprocesses, including OS activity probes and Windows taskkill,
    pass directly through to the original constructor.
    """

    original_popen = subprocess.Popen

    def start(*args, **kwargs):
        process = original_popen(*args, **kwargs)
        if args[0] != command:
            return process
        try:
            deadline = time.monotonic() + setup_timeout
            timeout_message = "nested fixture setup timed out before three live PIDs"
            while True:
                if process.poll() is not None:
                    raise AssertionError("nested fixture exited before readiness")
                if time.monotonic() >= deadline:
                    raise AssertionError(timeout_message)
                text = pid_path.read_text(encoding="utf-8") if pid_path.exists() else ""
                try:
                    pids = (
                        {int(line) for line in text.splitlines()}
                        if text.endswith("\n")
                        else set()
                    )
                except ValueError as failure:
                    raise AssertionError("nested fixture recorded malformed PIDs") from failure
                if len(pids) == 3 and process.pid in pids:
                    ready = True
                    for pid in pids:
                        remaining = deadline - time.monotonic()
                        if remaining <= 0:
                            raise AssertionError(timeout_message)
                        if not process_is_active(pid, timeout=remaining):
                            ready = False
                            break
                    if time.monotonic() >= deadline:
                        raise AssertionError(timeout_message)
                    if ready:
                        return process
                time.sleep(0.01)
        except BaseException as failure:
            # Popen's context manager has not acquired this process yet.
            # Clean up even when readiness fails, preserving its diagnostic.
            try:
                bounded_process._terminate_process_tree(process)
                process.communicate(timeout=5)
            except BaseException as cleanup_failure:
                raise failure from cleanup_failure
            raise

    with mock.patch.object(bounded_process.subprocess, "Popen", side_effect=start):
        bounded_process.run(
            command,
            phase="nested compiler fixture",
            timeout=3,
            check=False,
            capture_output=True,
            text=True,
        )


def direct_bounded_subprocess_lines(path: Path) -> tuple[int, ...]:
    """Find direct ``subprocess.run`` calls that still own a timeout."""

    source = path.read_text(encoding="utf-8")
    tree = ast.parse(source, filename=str(path))
    return tuple(
        node.lineno
        for node in ast.walk(tree)
        if isinstance(node, ast.Call)
        and isinstance(node.func, ast.Attribute)
        and isinstance(node.func.value, ast.Name)
        and node.func.value.id == "subprocess"
        and node.func.attr == "run"
        and any(keyword.arg == "timeout" for keyword in node.keywords)
    )


class BoundedProcessTest(unittest.TestCase):
    """Protect successful results, timeout diagnostics, and platform cleanup."""

    def test_timeout_scale_is_bounded_and_opt_in(self) -> None:
        with mock.patch.dict(os.environ, {}, clear=True):
            self.assertEqual(bounded_process.effective_timeout(30), 30)
        with mock.patch.dict(os.environ, {"HXC_TEST_TIMEOUT_SCALE": "4"}, clear=True):
            self.assertEqual(bounded_process.effective_timeout(30), 120)
        with mock.patch.dict(os.environ, {"HXC_TEST_TIMEOUT_SCALE": "30"}, clear=True):
            self.assertEqual(bounded_process.effective_timeout(30), 900)

        for invalid in ("", "0", "-1", "1.5", "31", "slow"):
            with self.subTest(invalid=invalid):
                with mock.patch.dict(
                    os.environ, {"HXC_TEST_TIMEOUT_SCALE": invalid}, clear=True
                ):
                    with self.assertRaisesRegex(ValueError, "integer from 1 through 30"):
                        bounded_process.effective_timeout(30)

    def test_success_and_nonzero_results_match_subprocess_run(self) -> None:
        command = [
            sys.executable,
            "-c",
            "import sys; print('out'); print('err', file=sys.stderr); sys.exit(7)",
        ]
        expected = subprocess.run(command, check=False, capture_output=True, text=True)
        actual = bounded_process.run(
            command,
            phase="result compatibility fixture",
            timeout=5,
            check=False,
            capture_output=True,
            text=True,
        )
        self.assertEqual(actual.args, expected.args)
        self.assertEqual(actual.returncode, expected.returncode)
        self.assertEqual(actual.stdout, expected.stdout)
        self.assertEqual(actual.stderr, expected.stderr)
        with self.assertRaises(subprocess.CalledProcessError) as caught:
            bounded_process.run(
                command,
                phase="checked exit fixture",
                timeout=5,
                check=True,
                capture_output=True,
                text=True,
            )
        self.assertEqual(caught.exception.returncode, expected.returncode)
        self.assertEqual(caught.exception.stdout, expected.stdout)
        self.assertEqual(caught.exception.stderr, expected.stderr)

    def test_repeated_timeouts_leave_no_active_descendants(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            for attempt in range(3):
                pid_path = Path(temporary) / f"attempt-{attempt}.pids"
                command = [
                    sys.executable,
                    "-c",
                    ROOT_PROGRAM,
                    str(pid_path),
                    CHILD_PROGRAM,
                    "4" if attempt == 0 else "0",
                ]
                with self.assertRaises(bounded_process.ProcessTreeTimeout) as caught:
                    run_ready_tree(command, pid_path)
                self.assertIn("nested compiler fixture", str(caught.exception))
                self.assertIn("3 seconds", str(caught.exception))

                pids = {
                    int(line)
                    for line in pid_path.read_text(encoding="utf-8").splitlines()
                }
                self.assertEqual(len(pids), 3)
                deadline = time.monotonic() + 3
                while any(process_is_active(pid) for pid in pids):
                    if time.monotonic() >= deadline:
                        self.fail(f"timeout attempt {attempt} left active PIDs {pids}")
                    time.sleep(0.05)

    def test_fixture_readiness_failure_cleans_up_its_process(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            pid_path = Path(temporary) / "not-ready.pids"
            command = [sys.executable, "-c", "import time; time.sleep(60)"]
            with mock.patch.object(
                bounded_process,
                "_terminate_process_tree",
                wraps=bounded_process._terminate_process_tree,
            ) as terminate:
                with self.assertRaisesRegex(
                    AssertionError, "setup timed out before three live PIDs"
                ):
                    run_ready_tree(command, pid_path, setup_timeout=0.1)
            terminate.assert_called_once()
            process = terminate.call_args.args[0]
            self.assertIsNotNone(process.returncode)
            self.assertFalse(process_is_active(process.pid))

    def test_malformed_fixture_readiness_cleans_up_descendants(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            pid_path = Path(temporary) / "malformed.pids"
            child_program = CHILD_PROGRAM.replace(
                "output.flush()", 'output.write("invalid\\n"); output.flush()'
            )
            command = [sys.executable, "-c", ROOT_PROGRAM, str(pid_path), child_program, "0"]
            with mock.patch.object(
                bounded_process,
                "_terminate_process_tree",
                wraps=bounded_process._terminate_process_tree,
            ) as terminate:
                with self.assertRaisesRegex(AssertionError, "recorded malformed PIDs"):
                    run_ready_tree(command, pid_path)
            terminate.assert_called_once()
            process = terminate.call_args.args[0]
            pids = {process.pid} | {
                int(line)
                for line in pid_path.read_text(encoding="utf-8").splitlines()
                if line != "invalid"
            }
            self.assertEqual(len(pids), 3)
            self.assertIsNotNone(process.returncode)
            self.assertFalse(any(process_is_active(pid) for pid in pids))

    def test_compiler_timeout_reports_phase_or_frontend(self) -> None:
        progress = (
            'HXC_PHASE_PROGRESS\t{"schemaVersion":1,"state":"phase-started",'
            '"phase":"semantic lowering","profile":"portable",'
            '"buildMode":"debug","status":null}'
        )
        with self.assertRaisesRegex(
            bounded_process.ProcessTreeTimeout,
            r"last active compiler phase: semantic lowering \(portable/debug\)$",
        ):
            bounded_process.run(
                [
                    sys.executable,
                    "-c",
                    "import time; print(%r, flush=True); time.sleep(60)" % progress,
                ],
                phase="compiler progress fixture",
                timeout=1,
                check=False,
                capture_output=True,
                text=True,
                compiler_progress=True,
            )

        with self.assertRaisesRegex(
            bounded_process.ProcessTreeTimeout,
            r"no haxe\.c phase marker; request remained in Haxe frontend or server startup$",
        ):
            bounded_process.run(
                [sys.executable, "-c", "import time; time.sleep(60)"],
                phase="compiler frontend fixture",
                timeout=1,
                check=False,
                capture_output=True,
                text=True,
                compiler_progress=True,
            )

    def test_haxe_focused_runners_use_the_process_tree_owner(self) -> None:
        runners = [
            path
            for path in (ROOT / "test").rglob("*.py")
            if "governance" not in path.parts
            and path.name != "bounded_process.py"
            and "haxe" in path.read_text(encoding="utf-8").lower()
        ]
        runners.extend(CANONICAL_EXAMPLE_RUNNERS)
        violations = {}
        for path in runners:
            lines = direct_bounded_subprocess_lines(path)
            if lines:
                violations[path.relative_to(ROOT).as_posix()] = lines
        self.assertEqual(violations, {})

    def test_cold_custom_target_lanes_request_phase_progress(self) -> None:
        for path in (
            ROOT / "examples/caxecraft/run_haxe_c_test.py",
            ROOT / "test/all_sources/run.py",
        ):
            with self.subTest(path=path.relative_to(ROOT).as_posix()):
                source = path.read_text(encoding="utf-8")
                self.assertIn('"reflaxe_c_phase_progress"', source)
                self.assertIn("compiler_progress=True", source)

    def test_windows_termination_targets_the_complete_tree(self) -> None:
        process = mock.Mock()
        process.pid = 4182
        process.poll.return_value = None
        taskkill = subprocess.CompletedProcess([], 0, b"", b"")
        with mock.patch.object(
            bounded_process.subprocess, "run", return_value=taskkill
        ) as run_taskkill:
            bounded_process._terminate_windows_tree(process)
        run_taskkill.assert_called_once_with(
            ["taskkill", "/PID", "4182", "/T", "/F"],
            check=False,
            capture_output=True,
            timeout=10,
        )
        process.kill.assert_not_called()

    def test_windows_process_group_flag_keeps_existing_flags(self) -> None:
        self.assertEqual(
            bounded_process._windows_creation_flags(0x00000008, 0x00000200),
            0x00000208,
        )

    def test_windows_termination_falls_back_when_taskkill_is_missing(self) -> None:
        process = mock.Mock()
        process.pid = 4182
        process.poll.return_value = None
        with mock.patch.object(
            bounded_process.subprocess, "run", side_effect=FileNotFoundError
        ):
            bounded_process._terminate_windows_tree(process)
        process.kill.assert_called_once_with()


if __name__ == "__main__":
    unittest.main()
