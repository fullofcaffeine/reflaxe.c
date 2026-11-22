"""Check process identity without a shell, including a real Windows child."""

from __future__ import annotations

import ctypes
import os
from pathlib import Path
import subprocess
import sys
import time
import unittest
from unittest.mock import Mock, patch

sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "examples/caxecraft"))
import dev_windows_process as windows


class WindowsProcessBoundaryTests(unittest.TestCase):
    def test_native_handles_are_pointer_sized_and_file_times_are_64_bits(self) -> None:
        windows._kernel32.cache_clear()
        self.addCleanup(windows._kernel32.cache_clear)
        library = Mock()
        with patch.object(ctypes, "WinDLL", return_value=library, create=True):
            bound = windows._kernel32()
        self.assertIs(bound.OpenProcess.restype, ctypes.c_void_p)
        self.assertEqual(bound.OpenProcess.argtypes,
                         [ctypes.c_uint32, ctypes.c_int32, ctypes.c_uint32])
        self.assertEqual(bound.GetProcessTimes.argtypes,
                         [ctypes.c_void_p] + [ctypes.POINTER(windows._FileTime)] * 4)
        self.assertEqual(ctypes.sizeof(windows._FileTime), 8)

    def kernel(self) -> Mock:
        kernel = Mock()
        kernel.OpenProcess.return_value = 0x123456789
        kernel.WaitForSingleObject.return_value = 258
        def times(handle, creation, exit_time, kernel_time, user_time):
            creation._obj.low = 0x89ABCDEF
            creation._obj.high = 0x01234567
            return 1
        def image(handle, flags, buffer, size):
            buffer.value = "C:\\Compiler Space\\haxe.exe"
            size._obj.value = len(buffer.value)
            return 1
        kernel.GetProcessTimes.side_effect = times
        kernel.QueryFullProcessImageNameW.side_effect = image
        return kernel

    def test_absence_requires_complete_valid_pid_snapshot(self) -> None:
        for case, expected in (("absent", True), ("present", False), ("full", False),
                               ("malformed", False), ("failed", False), ("empty", False),
                               ("missing", False)):
            with self.subTest(case=case):
                kernel = self.kernel()
                def enumerate_processes(buffer, capacity, used):
                    buffer[0], buffer[1] = 0, (123 if case == "present" else 456)
                    used._obj.value = {"full": capacity, "malformed": 3, "empty": 0}.get(case, 8)
                    return 0 if case == "failed" else 1
                kernel.K32EnumProcesses.side_effect = enumerate_processes
                if case == "missing":
                    del kernel.K32EnumProcesses
                with patch.object(windows, "_kernel32", return_value=kernel):
                    self.assertEqual(windows.process_absent(123), expected)

    def test_exit_observer_requires_signaled_creation_verified_handle(self) -> None:
        for wait, expected in ((0, True), (258, False), (0xFFFFFFFF, False)):
            with self.subTest(wait=wait):
                kernel = self.kernel()
                kernel.WaitForSingleObject.return_value = wait
                with patch.object(windows, "_kernel32", return_value=kernel):
                    with windows.process_exit_observer(123, "windows-start-ticks:586896761216486895") as exited:
                        self.assertIsNotNone(exited)
                        self.assertEqual(exited(50), expected)
                        kernel.CloseHandle.assert_not_called()
                kernel.CloseHandle.assert_called_once_with(0x123456789)
                kernel.WaitForSingleObject.assert_called_once_with(0x123456789, 50)

    def test_exit_observer_access_identity_and_cleanup_fail_closed(self) -> None:
        for failure in ("access", "identity", "times", "body"):
            with self.subTest(failure=failure):
                kernel = self.kernel()
                if failure == "access":
                    kernel.OpenProcess.return_value = 0
                if failure == "times":
                    kernel.GetProcessTimes.side_effect = OSError("unavailable")
                expected = "wrong" if failure == "identity" else "windows-start-ticks:586896761216486895"
                with patch.object(windows, "_kernel32", return_value=kernel):
                    if failure == "body":
                        with self.assertRaisesRegex(RuntimeError, "primary"):
                            with windows.process_exit_observer(123, expected):
                                raise RuntimeError("primary")
                    else:
                        with windows.process_exit_observer(123, expected) as exited:
                            self.assertIsNone(exited)
                if failure == "access":
                    kernel.CloseHandle.assert_not_called()
                else:
                    kernel.CloseHandle.assert_called_once_with(0x123456789)

    def test_exact_identity_uses_one_read_only_handle_without_a_shell(self) -> None:
        kernel = self.kernel()
        with patch.object(windows, "_kernel32", return_value=kernel), patch.object(
            subprocess, "run", side_effect=AssertionError("identity must not launch a shell")
        ):
            result = windows.process_identity(123)
        self.assertIsNotNone(result)
        self.assertEqual(result.start, "windows-start-ticks:586896761216486895")
        self.assertEqual(result.command, "windows-path:C:\\Compiler Space\\haxe.exe")
        kernel.OpenProcess.assert_called_once_with(0x101000, False, 123)
        kernel.CloseHandle.assert_called_once_with(0x123456789)
        self.assertEqual(ctypes.sizeof(windows._FileTime), 8)

    def test_failed_or_exited_observation_never_returns_an_identity(self) -> None:
        for failure in ("open", "exited", "wait", "times", "image", "empty", "exception"):
            with self.subTest(failure=failure):
                kernel = self.kernel()
                if failure == "open":
                    kernel.OpenProcess.return_value = None
                elif failure in ("exited", "wait"):
                    kernel.WaitForSingleObject.return_value = 0 if failure == "exited" else 0xFFFFFFFF
                elif failure == "times":
                    kernel.GetProcessTimes.side_effect = None
                    kernel.GetProcessTimes.return_value = 0
                elif failure == "image":
                    kernel.QueryFullProcessImageNameW.side_effect = None
                    kernel.QueryFullProcessImageNameW.return_value = 0
                elif failure == "exception":
                    kernel.QueryFullProcessImageNameW.side_effect = OSError("observation failed")
                else:
                    kernel.QueryFullProcessImageNameW.side_effect = lambda *args: 1
                with patch.object(windows, "_kernel32", return_value=kernel):
                    self.assertIsNone(windows.process_identity(123))
                if failure == "open":
                    kernel.CloseHandle.assert_not_called()
                else:
                    kernel.CloseHandle.assert_called_once_with(0x123456789)

    def test_invalid_pid_never_reaches_the_native_boundary(self) -> None:
        with patch.object(windows, "_kernel32") as load:
            for pid in (-1, 0, 2**32):
                self.assertIsNone(windows.process_identity(pid))
            load.assert_not_called()

    def test_server_uses_the_same_native_identity_for_cookie_checks(self) -> None:
        import dev_haxe_server as server
        fake_os = Mock(wraps=os)
        fake_os.name = "nt"
        identity = windows.WindowsProcessIdentity("windows-start-ticks:123", "windows-path:C:\\haxe.exe")
        with patch.object(server, "os", fake_os), patch.object(
            server, "windows_process_identity", return_value=identity
        ), patch.object(subprocess, "run", side_effect=AssertionError("no shell")):
            self.assertEqual(server.process_start_identity(42), identity.start)
            self.assertEqual(server.process_command_identity(42), identity.command)


@unittest.skipUnless(os.name == "nt", "requires the real Windows process APIs")
class WindowsProcessNativeTests(unittest.TestCase):
    def test_child_identity_and_exit_with_no_powershell(self) -> None:
        # A pipe keeps the child alive until this test has observed its identity.
        before = time.time_ns()
        with subprocess.Popen([sys.executable, "-c", "import sys; sys.stdin.read()"],
                              stdin=subprocess.PIPE) as child:
            try:
                with patch.object(subprocess, "run", side_effect=AssertionError("no shell")):
                    first = windows.process_identity(child.pid)
                    second = windows.process_identity(child.pid)
                self.assertIsNotNone(first)
                self.assertEqual(first, second)
                self.assertTrue(os.path.samefile(first.command.removeprefix("windows-path:"), sys.executable))
                ticks = int(first.start.removeprefix("windows-start-ticks:"))
                unix_ns = (ticks - 621355968000000000) * 100
                self.assertGreaterEqual(unix_ns, before - 1_000_000_000)
                self.assertLessEqual(unix_ns, time.time_ns())
                self.assertFalse(windows.process_absent(child.pid))
                with windows.process_exit_observer(child.pid, first.start) as exited:
                    self.assertIsNotNone(exited)
                    self.assertFalse(exited(0))
                    child.stdin.close()
                    child.wait(timeout=5)
                    self.assertTrue(exited(1000))
            finally:
                if not child.stdin.closed:
                    child.stdin.close()
                child.wait(timeout=5)
            self.assertIsNone(windows.process_identity(child.pid))
            self.assertTrue(windows.process_absent(child.pid))


if __name__ == "__main__":
    unittest.main()
