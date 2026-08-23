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
child = subprocess.Popen([sys.executable, "-c", child_program, pid_path])
with open(pid_path, "a", encoding="utf-8") as output:
    output.write(f"{os.getpid()}\\n")
    output.flush()
time.sleep(60)
"""


def process_is_active(pid: int) -> bool:
    """Return true only while a process can still consume host resources."""

    if os.name == "nt":
        result = subprocess.run(
            ["tasklist", "/FI", f"PID eq {pid}", "/FO", "CSV", "/NH"],
            check=False,
            capture_output=True,
            text=True,
        )
        return f'"{pid}"' in result.stdout
    result = subprocess.run(
        ["ps", "-o", "stat=", "-p", str(pid)],
        check=False,
        capture_output=True,
        text=True,
    )
    state = result.stdout.strip()
    return result.returncode == 0 and bool(state) and not state.startswith("Z")


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
                ]
                with self.assertRaises(bounded_process.ProcessTreeTimeout) as caught:
                    bounded_process.run(
                        command,
                        phase="nested compiler fixture",
                        timeout=3,
                        check=False,
                        capture_output=True,
                        text=True,
                    )
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
