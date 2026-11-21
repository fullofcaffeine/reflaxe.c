"""Exercise the virtual-dispatch test's real startup and shutdown boundary."""
from pathlib import Path
import os
import shlex
import socket
import subprocess
import sys
import tempfile
import time
import unittest
from unittest import mock

from test_bounded_process import process_is_active

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
from virtual_dispatch import run as dispatch  # noqa: E402


class DispatchServerCleanupTest(unittest.TestCase):
    def exercise(self, outcome):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            fixture = Path(__file__).parent / "fixtures" / "dispatch_server_tree.py"
            if os.name == "nt":
                launcher = root / "haxe.cmd"
                launcher.write_text(f'@"{sys.executable}" "{fixture}" %*\n')
            else:
                launcher = root / "haxe"
                launcher.write_text("#!/bin/sh\nexec " + shlex.join([sys.executable, str(fixture)]) + ' "$@"\n')
                launcher.chmod(0o700)
            identities = []
            endpoints = []
            original_ready = dispatch.wait_for_server

            def observe_start(server, port):
                original_ready(server, port)
                endpoints.append(port)
                identities.extend(int((root / name).read_text()) for name in ("wrapper", "child", "grandchild"))
                self.assertEqual(len(set(identities)), 3)
                self.assertTrue(all(process_is_active(pid) for pid in identities))
                if outcome == "startup":
                    # The two descendants retain the launcher's standard streams.
                    (root / "exit-wrapper").touch()
                    server.wait(timeout=3)
                    original_ready(server, port)
                    self.fail("early launcher exit did not fail readiness")

            control = subprocess.Popen([sys.executable, "-c", "import time; time.sleep(45)"])
            try:
                with mock.patch.dict(os.environ, {"HXC_DISPATCH_TREE": str(root)}), \
                     mock.patch.object(dispatch, "development_tool", return_value=str(launcher)), \
                     mock.patch.object(dispatch, "wait_for_server", side_effect=observe_start):
                    if outcome == "normal":
                        with dispatch.haxe_server():
                            pass
                    elif outcome == "body":
                        with self.assertRaisesRegex(ValueError, "fixture body"):
                            with dispatch.haxe_server():
                                raise ValueError("fixture body")
                    else:
                        with self.assertRaisesRegex(dispatch.VirtualDispatchFailure, "fixture launcher exited"):
                            with dispatch.haxe_server():
                                self.fail("unexpected ready server")
                end = time.monotonic() + 2
                while any(process_is_active(pid) for pid in identities) and time.monotonic() < end:
                    time.sleep(0.02)
                self.assertTrue(identities)
                self.assertFalse([pid for pid in identities if process_is_active(pid)], "owned processes survived")
                with self.assertRaises(OSError):
                    socket.create_connection(("127.0.0.1", endpoints[0]), timeout=0.2).close()
                self.assertIsNone(control.poll())
                self.assertFalse(list(root.glob("expired-*")), "shutdown waited for fixture expiry")
            finally:
                (root / "stop").touch()
                control.terminate()
                control.wait(timeout=3)
                end = time.monotonic() + 3
                while any(process_is_active(pid) for pid in identities) and time.monotonic() < end:
                    time.sleep(0.02)

    def test_normal_exit(self):
        self.exercise("normal")

    def test_body_error(self):
        self.exercise("body")

    def test_startup_error(self):
        self.exercise("startup")
