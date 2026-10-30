#!/usr/bin/env python3
"""Focused lifecycle tests for Caxecraft's worktree-owned Haxe server."""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import Mock, patch


ROOT = Path(__file__).resolve().parents[2]
CASE = ROOT / "examples/caxecraft"
sys.path.insert(0, str(CASE))

import dev_haxe_server as server  # noqa: E402

from dev_haxe_server import (  # noqa: E402
    HaxeInstallation,
    HaxeServerFailure,
    OwnedHaxeServer,
    attached_server,
    pinned_haxe_installation,
    process_command_identity,
    process_start_identity,
    verify_pinned_haxe,
)


class HaxeServerWindowsStopTests(unittest.TestCase):
    """A rejected graceful stop must still reach the guarded force fallback."""

    def exercise_stop(self, *, graceful: bool, still_owned: bool = True):
        alive = True
        commands = []

        def taskkill(command, **options):
            nonlocal alive
            commands.append(command)
            if graceful or "/F" in command:
                alive = False
                return subprocess.CompletedProcess(command, 0)
            return subprocess.CompletedProcess(command, 1)

        windows_os = Mock(wraps=os)
        windows_os.name = "nt"
        with patch.object(server, "os", windows_os), patch.object(
            server, "_owned_cookie", side_effect=[True, still_owned]
        ) as ownership, patch.object(server.subprocess, "run", side_effect=taskkill), patch.object(
            server, "process_start_identity", side_effect=lambda pid: "live" if alive else None
        ), patch.object(server.time, "monotonic", return_value=0), patch.object(
            server.time, "sleep", side_effect=AssertionError("rejected or completed stop must not wait")
        ):
            result = server._terminate_owned({"pid": 123})
        return result, commands, ownership.call_count

    def test_rejected_graceful_stop_revalidates_before_forcing(self) -> None:
        result, commands, checks = self.exercise_stop(graceful=False)
        self.assertTrue(result)
        self.assertEqual(checks, 2)
        self.assertEqual(commands, [["taskkill", "/PID", "123", "/T"],
                                    ["taskkill", "/F", "/PID", "123", "/T"]])

    def test_changed_ownership_prevents_force(self) -> None:
        result, commands, checks = self.exercise_stop(graceful=False, still_owned=False)
        self.assertFalse(result)
        self.assertEqual(checks, 2)
        self.assertEqual(commands, [["taskkill", "/PID", "123", "/T"]])

    def test_completed_graceful_stop_needs_no_force(self) -> None:
        result, commands, checks = self.exercise_stop(graceful=True)
        self.assertTrue(result)
        self.assertEqual(checks, 1)
        self.assertEqual(commands, [["taskkill", "/PID", "123", "/T"]])


class HaxeServerStartupDiagnosticTests(unittest.TestCase):
    """Exercise startup failures without resolving or launching a Haxe compiler."""

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="hxc-server-diagnostic-")
        self.addCleanup(self.temporary.cleanup)
        self.state_root = Path(self.temporary.name)
        self.stderr_path = self.state_root / "server.stderr.log"
        self.owner = OwnedHaxeServer(
            self.state_root,
            installation=HaxeInstallation("fixture", Path(sys.executable),
                                          self.state_root, self.state_root, None),
            compatibility={"fixture": "startup-diagnostic"},
        )

    def failed_start(
        self, output: bytes, *, start_identity: str | None = None,
        command_identity: str | None = None, early_exit: bool = False,
        deadline: bool = False,
    ) -> str:
        """Model detached launch; an optional Python child really exits with stderr."""

        original_popen = subprocess.Popen
        handle = Mock()
        fixture_pid = 123456
        def spawn(command: list[str], **options: object) -> SimpleNamespace:
            nonlocal fixture_pid
            if early_exit:
                child = original_popen(
                    [sys.executable, "-c",
                     "import sys; sys.stderr.buffer.write(" + repr(output) + "); sys.exit(42)"],
                    stdout=options["stdout"], stderr=options["stderr"],
                )
                self.assertEqual(child.wait(timeout=5), 42)
                fixture_pid = child.pid
            else:
                options["stderr"].write(output)
                options["stderr"].flush()
            return SimpleNamespace(pid=fixture_pid, _handle=handle)

        fake_os = Mock(wraps=os)
        fake_os.name = "nt"
        fake_os.SEEK_END = os.SEEK_END
        fake_os.kill = Mock()
        with (
            patch.object(server, "os", fake_os),
            patch.object(server, "_available_endpoint", return_value="127.0.0.1:1"),
            patch.object(server, "pinned_haxe_environment", return_value={}),
            patch.object(server.subprocess, "CREATE_NEW_PROCESS_GROUP", 0, create=True),
            patch.object(server.subprocess, "DETACHED_PROCESS", 0, create=True),
            patch.object(server.subprocess, "Popen", side_effect=spawn),
            patch.object(server, "process_start_identity", return_value=start_identity),
            patch.object(server, "process_command_identity", return_value=command_identity),
            patch.object(server, "_accepting", return_value=False) as accepting,
            patch.object(server.time, "monotonic", side_effect=[0.0, 0.0, 11.0]),
            patch.object(server.time, "sleep"),
        ):
            with self.assertRaises(HaxeServerFailure) as raised:
                self.owner._start()
            if deadline:
                fake_os.kill.assert_called_once_with(fixture_pid, server.signal.SIGTERM)
                accepting.assert_called_once()
            else:
                fake_os.kill.assert_not_called()
                accepting.assert_not_called()
        handle.Close.assert_called_once()
        self.assertFalse(self.owner.cookie_path.exists())
        return str(raised.exception)

    def test_unavailable_identity_does_not_claim_exit_or_ownership(self) -> None:
        for start, command in ((None, None), (None, "command"), ("start", None)):
            with self.subTest(start=start, command=command):
                self.stderr_path.write_bytes(b"prior startup must not be attributed\n")
                message = self.failed_start(b"current startup detail\n",
                                            start_identity=start, command_identity=command)
                self.assertIn("process identity could not be verified", message)
                self.assertNotIn("exited", message)
                self.assertIn("current startup detail", message)
                self.assertNotIn("prior startup", message)

    def test_actual_early_exit_preserves_stderr_without_guessing_from_identity(self) -> None:
        message = self.failed_start(b"fixture startup failure: missing dependency\n", early_exit=True)
        self.assertIn("fixture startup failure: missing dependency", message)
        self.assertIn("process identity could not be verified", message)
        self.assertNotIn("exited", message)

    def test_acceptance_deadline_keeps_error_and_guarded_cleanup(self) -> None:
        message = self.failed_start(b"fixture still starting\n", start_identity="start",
                                    command_identity="command", deadline=True)
        self.assertIn("did not accept connections", message)
        self.assertIn("fixture still starting", message)

    def test_stderr_excerpt_is_bounded_and_original_log_is_preserved(self) -> None:
        previous = b"old startup\n" * 1000
        current = b"omitted start\n" + b"x" * 8192 + b"\xffcurrent tail\n"
        self.stderr_path.write_bytes(previous)
        message = self.failed_start(current)
        self.assertIn("current tail", message)
        self.assertNotIn("old startup", message)
        self.assertNotIn("omitted start", message)
        self.assertLess(len(message), 5000)
        self.assertEqual(self.stderr_path.read_bytes(), previous + current)

    def test_empty_stderr_is_explicit(self) -> None:
        message = self.failed_start(b"")
        self.assertIn("no stderr captured for this startup", message)


    def test_log_read_failure_keeps_primary_startup_reason(self) -> None:
        self.stderr_path.write_bytes(b"fixture detail")
        with patch.object(Path, "open", side_effect=PermissionError("fixture denied")):
            failure = server._startup_failure("primary startup failure", self.stderr_path, 0)
        self.assertIn("primary startup failure", str(failure))
        self.assertIn("startup stderr unavailable", str(failure))

    def test_unknown_offset_or_replaced_log_is_not_attributed_to_startup(self) -> None:
        self.stderr_path.write_bytes(b"unattributed bytes")
        for offset in (None, 100):
            with self.subTest(offset=offset):
                failure = server._startup_failure("primary startup failure", self.stderr_path, offset)
                self.assertIn("primary startup failure", str(failure))
                self.assertIn("startup stderr unavailable", str(failure))
                self.assertNotIn("unattributed bytes", str(failure))
        with patch.object(Path, "is_symlink", return_value=True):
            failure = server._startup_failure("primary startup failure", self.stderr_path, 0)
        self.assertIn("startup stderr unavailable", str(failure))
        self.assertNotIn("unattributed bytes", str(failure))


class CaxecraftHaxeServerTests(unittest.TestCase):
    """Prove exact reuse, replacement, attachment, and process cleanup."""

    @classmethod
    def setUpClass(cls) -> None:
        cls.installation = pinned_haxe_installation()
        verify_pinned_haxe(cls.installation)

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="hxc-haxe-server-")
        self.state_root = Path(self.temporary.name) / "server"
        self.owners: list[OwnedHaxeServer] = []

    def tearDown(self) -> None:
        for owner in reversed(self.owners):
            owner.stop()
        self.temporary.cleanup()

    def owner(self, identity: str) -> OwnedHaxeServer:
        value = OwnedHaxeServer(
            self.state_root,
            installation=self.installation,
            compatibility={"schemaVersion": 1, "identity": identity},
        )
        self.owners.append(value)
        return value

    def test_auto_reuses_one_exact_compatible_process(self) -> None:
        owner = self.owner("same")
        first = owner.connect()
        second = owner.connect()
        cookie = json.loads(owner.cookie_path.read_text(encoding="utf-8"))

        self.assertTrue(first.owned)
        self.assertEqual(first.connection.endpoint, second.connection.endpoint)
        self.assertEqual(first.compatibility_sha256, second.compatibility_sha256)
        self.assertIsNotNone(process_start_identity(int(cookie["pid"])))
        self.assertEqual(
            cookie["processCommandIdentity"],
            process_command_identity(int(cookie["pid"])),
        )

    def test_infrastructure_change_replaces_and_reaps_old_process(self) -> None:
        first_owner = self.owner("before")
        first_owner.connect()
        first_cookie = json.loads(first_owner.cookie_path.read_text(encoding="utf-8"))
        first_pid = int(first_cookie["pid"])

        second_owner = self.owner("after")
        second_owner.connect()
        second_cookie = json.loads(second_owner.cookie_path.read_text(encoding="utf-8"))

        self.assertNotEqual(first_pid, int(second_cookie["pid"]))
        self.assertIsNone(process_start_identity(first_pid))

    def test_lease_cleanup_survives_a_replaced_cookie(self) -> None:
        owner = self.owner("lease-cleanup")
        first = owner.connect()
        self.assertIsNotNone(first.process)
        assert first.process is not None

        # Reproduce the timeout race: the first client still owns a live
        # process, but a later lifecycle operation publishes a new cookie.
        owner.cookie_path.unlink()
        second = owner.connect()
        self.assertIsNotNone(second.process)
        assert second.process is not None
        self.assertNotEqual(first.process.pid, second.process.pid)

        self.assertTrue(owner.stop_lease(first))
        self.assertIsNone(process_start_identity(first.process.pid))
        self.assertIsNotNone(process_start_identity(second.process.pid))
        current = json.loads(owner.cookie_path.read_text(encoding="utf-8"))
        self.assertEqual(current["pid"], second.process.pid)

    def test_explicit_attachment_never_claims_process_ownership(self) -> None:
        owner = self.owner("attachment-host")
        automatic = owner.connect()
        borrowed = attached_server(
            automatic.connection.endpoint, self.installation
        )

        self.assertFalse(borrowed.owned)
        self.assertIsNone(borrowed.compatibility_sha256)
        self.assertTrue(owner.stop())

    def test_stale_foreign_pid_identity_is_not_killed(self) -> None:
        owner = self.owner("safe-replacement")
        foreign_identity = process_start_identity(1)
        self.state_root.mkdir(parents=True)
        body = {
            "schemaVersion": 1,
            "kind": "caxecraft-owned-haxe-server",
            "endpoint": "127.0.0.1:1",
            "pid": 1,
            "processStartIdentity": "deliberately-wrong",
            "processCommandIdentity": "also-wrong",
            "compatibilitySha256": "wrong",
            "compiler": "foreign",
        }
        from dev_build_state import canonical_digest

        owner.cookie_path.write_text(
            json.dumps({**body, "sha256": canonical_digest(body)}),
            encoding="utf-8",
        )

        lease = owner.connect()
        self.assertTrue(lease.owned)
        self.assertEqual(process_start_identity(1), foreign_identity)

    def test_separate_state_roots_never_share_mutable_server_state(self) -> None:
        first = self.owner("shared-infrastructure")
        second = OwnedHaxeServer(
            Path(self.temporary.name) / "other-worktree-server",
            installation=self.installation,
            compatibility={"schemaVersion": 1, "identity": "shared-infrastructure"},
        )
        self.owners.append(second)

        first_lease = first.connect()
        second_lease = second.connect()

        self.assertNotEqual(
            first_lease.compatibility_sha256,
            second_lease.compatibility_sha256,
        )
        self.assertNotEqual(
            first_lease.connection.endpoint,
            second_lease.connection.endpoint,
        )


if __name__ == "__main__":
    unittest.main()
