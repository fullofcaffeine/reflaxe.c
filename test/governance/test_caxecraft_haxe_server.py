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
from contextlib import nullcontext
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


def stop_cookie(pid):
    body = {"schemaVersion": server.COOKIE_SCHEMA_VERSION, "kind": server.COOKIE_KIND,
            "pid": pid, "processStartIdentity": "fixture", "processCommandIdentity": "fixture"}
    return {**body, "sha256": server.canonical_digest(body)}


def assert_process_absent(test: unittest.TestCase, pid: int) -> None:
    """Require positive exit evidence without signaling Windows process groups.

    Windows maps signal zero to CTRL_C_EVENT, unlike the POSIX existence probe.
    A failed or incomplete native PID snapshot must therefore fail this assertion.
    """
    if os.name == "nt":
        test.assertTrue(server.windows_process_absent(pid),
                        f"Windows process {pid} absence could not be confirmed")
    else:
        with test.assertRaises(ProcessLookupError):
            os.kill(pid, 0)


class ProcessAbsenceAssertionTests(unittest.TestCase):
    """Check the observer's platform choice without starting or signaling children."""

    def test_windows_requires_positive_snapshot_without_signaling(self) -> None:
        fake_os = SimpleNamespace(name="nt", kill=Mock(side_effect=AssertionError("must not signal")))
        with patch.dict(assert_process_absent.__globals__, {"os": fake_os}):
            with patch.object(server, "windows_process_absent", return_value=True) as absent:
                assert_process_absent(self, 123)
                absent.assert_called_once_with(123)
            with patch.object(server, "windows_process_absent", return_value=False):
                with self.assertRaises(AssertionError):
                    assert_process_absent(self, 123)
        fake_os.kill.assert_not_called()

    def test_posix_keeps_esrch_requirement(self) -> None:
        fake_os = SimpleNamespace(name="posix", kill=Mock(side_effect=ProcessLookupError()))
        with patch.dict(assert_process_absent.__globals__, {"os": fake_os}):
            assert_process_absent(self, 123)
            fake_os.kill.assert_called_once_with(123, 0)
            fake_os.kill.side_effect = PermissionError("unconfirmed")
            with self.assertRaises(PermissionError):
                assert_process_absent(self, 123)


class HaxeSourceFreshnessTests(unittest.TestCase):
    """A live server remembers its latest typed inputs, never a variant's binary."""

    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        root = Path(self.temporary.name)
        self.source = str(root / "Worker.hx")
        self.installation = HaxeInstallation("test", root / "haxe", root, root, None)
        self.owner = OwnedHaxeServer(root / "server", installation=self.installation, compatibility={})
        self.owner.state_root.mkdir()
        self.lease = self.make_lease(10001)
        self.replacement = self.make_lease(10002)
        self.identity = patch.object(server, "_owned_cookie", return_value=True)
        self.identity.start()
        self.addCleanup(self.identity.stop)

    def make_lease(self, pid):
        process = server.OwnedHaxeServerProcess(
            str(pid), pid, f"start-{pid}", f"command-{pid}",
            self.owner.compatibility_sha256, str(self.installation.compiler), f"cookie-{pid}",
        )
        return server.HaxeServerLease(server.HaxeServerConnection(str(pid), self.installation),
                                     True, self.owner.compatibility_sha256, process)

    def test_successful_variants_share_latest_source_not_last_binary(self):
        a, b = {self.source: "a" * 64}, {self.source: "b" * 64}
        self.owner.finish_source_request(self.lease, a)
        lease, changed = self.owner.prepare_source_request(self.lease, b)
        self.assertEqual(lease, self.lease)
        self.assertEqual(changed, (self.source,))
        self.assertFalse(self.owner.source_snapshot_path.exists())
        self.owner.finish_source_request(lease, b)
        _, changed = self.owner.prepare_source_request(lease, a)
        self.assertEqual(changed, (self.source,))

    def test_failed_attempt_leaves_unknown_history_and_requires_restart(self):
        a, b = {self.source: "a" * 64}, {self.source: "b" * 64}
        self.owner.finish_source_request(self.lease, a)
        self.owner.prepare_source_request(self.lease, b)
        with patch.object(self.owner, "restart_after_transport_failure", return_value=self.replacement) as restart:
            lease, changed = self.owner.prepare_source_request(self.lease, a)
        restart.assert_called_once_with(self.lease)
        self.assertEqual((lease, changed), (self.replacement, ()))

    def test_unchanged_success_needs_no_invalidation(self):
        sources = {self.source: "a" * 64}
        self.owner.finish_source_request(self.lease, sources)
        self.assertEqual(self.owner.prepare_source_request(self.lease, sources), (self.lease, ()))

    def test_malformed_foreign_or_changed_path_history_restarts(self):
        sources = {self.source: "a" * 64}
        for fault in ("malformed", "foreign", "removed"):
            with self.subTest(fault=fault):
                self.owner.finish_source_request(self.lease, sources)
                if fault == "malformed":
                    self.owner.source_snapshot_path.write_text("{")
                elif fault == "foreign":
                    self.owner.finish_source_request(self.replacement, sources)
                current = {} if fault == "removed" else sources
                with patch.object(self.owner, "restart_after_transport_failure", return_value=self.replacement) as restart:
                    self.assertEqual(self.owner.prepare_source_request(self.lease, current), (self.replacement, ()))
                restart.assert_called_once_with(self.lease)
                self.assertFalse(self.owner.source_snapshot_path.exists())

    def test_foreign_compatibility_cannot_read_or_publish_frontier(self):
        from dataclasses import replace
        foreign = replace(self.lease, compatibility_sha256="foreign")
        for action in (self.owner.prepare_source_request, self.owner.finish_source_request):
            with self.assertRaises(HaxeServerFailure):
                action(foreign, {self.source: "a" * 64})

    def compile_fixture(self, before, after, *, resolver_error=None, attached=False, compatibility=None):
        import play
        from contextlib import redirect_stdout
        import io
        source_calls = Mock(side_effect=[before, after])
        lease = (server.HaxeServerLease(self.lease.connection, False, None, None)
                 if attached else self.lease)
        commands = []
        def dispatch(command, **kwargs):
            if not attached:
                self.assertFalse(self.owner.source_snapshot_path.exists())
            commands.append(command)
            return subprocess.CompletedProcess(command, 0, "", "")
        with (
            patch.object(play, "haxe_game_source_snapshot", source_calls),
            patch.object(play, "haxe_server_compatibility", return_value=compatibility or {}),
            patch.object(play, "resolve_haxe_arguments", side_effect=resolver_error,
                         return_value=("-cp", str(play.CASE / "src"))),
            patch.object(play, "pinned_haxe_environment", return_value={}),
            patch.object(play.subprocess, "run", side_effect=dispatch),
            patch.object(play, "validate_compiled_haxe", return_value={}),
            patch.object(play, "validate_content_platform_output"),
            redirect_stdout(io.StringIO()),
        ):
            play.compile_haxe(Path(self.temporary.name) / "generated", layout="split",
                              platform_name="macos", raylib_configuration="desktop",
                              server_lease=lease, server_owner=None if attached else self.owner)
        if attached:
            source_calls.assert_not_called()
        return commands

    def test_real_request_boundary_notifies_before_typing_then_publishes(self):
        before, edited = {self.source: "a" * 64}, {self.source: "b" * 64}
        self.owner.finish_source_request(self.lease, before)
        commands = self.compile_fixture(edited, edited)
        self.assertEqual(commands[0][3:5], ["--macro",
            "haxe.macro.CompilationServer.invalidateFiles(" + json.dumps([self.source]) + ")"])
        self.assertEqual(json.loads(self.owner.source_snapshot_path.read_text())["sources"], edited)
        commands = self.compile_fixture(edited, edited)
        self.assertNotIn("--macro", commands[0])

    def test_compile_only_boundary_rejects_changed_inputs_before_publication(self):
        import play
        a, b = {self.source: "a" * 64}, {self.source: "b" * 64}
        self.owner.finish_source_request(self.lease, a)
        with self.assertRaisesRegex(play.PlayFailure, "inputs changed"):
            self.compile_fixture(a, b)
        self.assertFalse(self.owner.source_snapshot_path.exists())

    def test_resolver_failure_leaves_no_trusted_frontier(self):
        sources = {self.source: "a" * 64}
        self.owner.finish_source_request(self.lease, sources)
        with self.assertRaisesRegex(HaxeServerFailure, "resolver failed"):
            self.compile_fixture(sources, sources, resolver_error=HaxeServerFailure("resolver failed"))
        self.assertFalse(self.owner.source_snapshot_path.exists())

    def test_attachment_has_no_frontier_authority(self):
        commands = self.compile_fixture({}, {}, attached=True)
        self.assertNotIn("--macro", commands[0])
        self.assertFalse(self.owner.source_snapshot_path.exists())

    def test_unknown_resolved_root_is_rejected(self):
        import play
        with self.assertRaisesRegex(play.PlayFailure, "no freshness inventory"):
            play.validate_haxe_source_roots(("-cp", "/unreviewed/library"), self.installation)


    def test_infrastructure_change_after_connect_prevents_dispatch(self):
        import play
        sources = {self.source: "a" * 64}
        self.owner.finish_source_request(self.lease, sources)
        with self.assertRaisesRegex(play.PlayFailure, "infrastructure changed"):
            self.compile_fixture(sources, sources, compatibility={"changed": True})

    def test_shared_request_lock_owns_connect_cleanup_and_explicit_stop(self):
        for stage in ("generation-failure", "native-failure", "release-interrupt", "explicit-stop"):
            with self.subTest(stage=stage):
                self.exercise_main_lock(stage)

    def exercise_main_lock(self, stage):
        import play
        import io
        from contextlib import redirect_stdout, redirect_stderr
        held = False
        interrupted = False
        events = []
        class RecordingLock:
            def __init__(inner, path):
                inner.shared = Path(path).name == "hxc-haxe-request.lock"
            def __enter__(inner):
                nonlocal held
                if inner.shared:
                    self.assertFalse(held)
                    held = True
                    events.append("acquire")
                return inner
            def __exit__(inner, *args):
                nonlocal held, interrupted
                if inner.shared:
                    self.assertTrue(held)
                    held = False
                    events.append("release")
                    if stage == "release-interrupt" and not interrupted:
                        interrupted = True
                        raise KeyboardInterrupt()
        owner = Mock()
        owner.state_root = Path(self.temporary.name) / "lock-owner"
        def connect():
            self.assertTrue(held)
            events.append("connect")
            return self.lease
        def stop(*args):
            self.assertTrue(held)
            events.append("stop")
            return True
        def compile_request(generated, **kwargs):
            self.assertTrue(held)
            events.append("compile")
            if stage == "generation-failure":
                raise play.PlayFailure("fixture generation failure")
            return {}
        def native_failure(**kwargs):
            self.assertFalse(held)
            events.append("native")
            raise play.PlayFailure("fixture native failure")
        owner.connect.side_effect = connect
        owner.stop.side_effect = stop
        owner.stop_lease.side_effect = stop
        with (
            patch.dict(os.environ, {"HAXE_NO_SERVER": "0"}),
            patch.object(play, "VariantLock", RecordingLock),
            patch.object(play, "pinned_haxe_installation", return_value=self.installation),
            patch.object(play, "verify_pinned_haxe"),
            patch.object(play, "haxe_server_compatibility", return_value={}),
            patch.object(play, "OwnedHaxeServer", return_value=owner),
            patch.object(play, "play_request_snapshot", return_value={"files": []}),
            patch.object(play, "compile_haxe", side_effect=compile_request),
            patch.object(play, "finalize_transaction", side_effect=lambda root, txn: SimpleNamespace(generated=txn.generated)),
            patch.object(play, "publish_pointer", side_effect=lambda root, generation: generation),
            patch.object(play, "provision_raylib", side_effect=native_failure),
            redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()),
        ):
            args = ["--output-root", str(Path(self.temporary.name) / ("output-" + stage)), "--no-build-cache"]
            if stage == "explicit-stop":
                args.append("--stop-haxe-server")
            status = play.main(args)
        self.assertFalse(held)
        expected = {
            "generation-failure": ["acquire", "connect", "compile", "stop", "release"],
            "native-failure": ["acquire", "connect", "compile", "release", "native", "acquire", "stop", "release"],
            "release-interrupt": ["acquire", "connect", "compile", "release", "acquire", "stop", "release"],
            "explicit-stop": ["acquire", "stop", "release"],
        }
        self.assertEqual(events, expected[stage])
        self.assertEqual(status, 130 if stage == "release-interrupt" else 0 if stage == "explicit-stop" else 1)



class PlayActiveLeaseFailureTests(unittest.TestCase):
    """Exercise the real launcher retry and transaction cleanup with host edges mocked."""

    def exercise_retry_failure(self, failure, *, uncertain_stop=False, original_stop=False, publication_failure=False):
        import play
        from contextlib import redirect_stdout, redirect_stderr
        import io

        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            installation = HaxeInstallation("test", root / "haxe", root, root, None)
            owner = OwnedHaxeServer(root / "server", installation=installation,
                                    compatibility={"test": "active-lease"})
            def lease(pid):
                process = server.OwnedHaxeServerProcess(
                    f"127.0.0.1:{pid}", pid, f"start-{pid}", f"command-{pid}",
                    owner.compatibility_sha256, str(installation.compiler), f"cookie-{pid}",
                )
                return server.HaxeServerLease(
                    server.HaxeServerConnection(process.endpoint, installation), True,
                    owner.compatibility_sha256, process,
                )
            original, replacement = lease(10001), lease(10002)
            alive = {10001, 10002}
            events = []
            discard = play.discard_transaction
            compile_haxe = play.compile_haxe
            def compile_request(*args, **kwargs):
                if publication_failure:
                    publish = kwargs["publish_server_lease"]
                    def interrupted(lease):
                        if publication_failure == "after":
                            publish(lease)
                        raise KeyboardInterrupt()
                    kwargs["publish_server_lease"] = interrupted
                return compile_haxe(*args, **kwargs)
            def terminate(cookie):
                pid = cookie["pid"]
                events.append(("stop", pid))
                if original_stop and pid == 10001:
                    return False
                if uncertain_stop and pid == 10002:
                    if uncertain_stop == "false":
                        return False
                    raise OSError("termination denied")
                if pid not in alive:
                    return True  # Positive absence now permits already-dead recovery.
                alive.remove(pid)
                return True
            def dispose(transaction):
                events.append(("discard", tuple(sorted(alive))))
                discard(transaction)
            with (
                patch.dict(os.environ, {"HAXE_NO_SERVER": "0"}),
                patch.object(play, "pinned_haxe_installation", return_value=installation),
                patch.object(play, "verify_pinned_haxe"),
                patch.object(play, "haxe_server_compatibility", return_value={k: v for k, v in owner.compatibility.items() if k != "canonicalServerStateRoot"}),
                patch.object(play, "haxe_game_source_snapshot", return_value={}),
                patch.object(play, "OwnedHaxeServer", return_value=owner),
                patch.object(owner, "connect", side_effect=[original, replacement]) as connection,
                # These cases isolate retry/disposal; source freshness has its own controls.
                patch.object(owner, "prepare_source_request", side_effect=lambda lease, sources: (lease, ())),
                patch.object(server, "_terminate_owned", side_effect=terminate),
                patch.object(play, "resolve_haxe_arguments", return_value=("fixture.hxml",)),
                patch.object(play, "pinned_haxe_environment", return_value={}),
                patch.object(play, "transport_failed", return_value=True),
                patch.object(play.subprocess, "run", side_effect=[
                    subprocess.CompletedProcess([], 1, "", "connection reset"), failure,
                ]),
                patch.object(play, "discard_transaction", side_effect=dispose),
                patch.object(play, "compile_haxe", side_effect=compile_request),
                redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()),
            ):
                status = play.main(["--compile-only", "--output-root", str(root / "output")])
            if original_stop:
                connection.assert_called_once_with()
            return status, events, alive

    def test_retried_timeout_stops_replacement_before_discard(self):
        status, events, alive = self.exercise_retry_failure(
            subprocess.TimeoutExpired("connected Haxe retry", 120)
        )
        self.assertEqual(status, 1)
        self.assertEqual(events, [("stop", 10001), ("stop", 10002), ("discard", ())])
        self.assertEqual(alive, set())

    def test_retried_interrupt_stops_replacement_before_discard(self):
        status, events, alive = self.exercise_retry_failure(KeyboardInterrupt())
        self.assertEqual(status, 130)
        self.assertEqual(events, [("stop", 10001), ("stop", 10002), ("discard", ())])
        self.assertEqual(alive, set())


    def test_uncertain_stop_retains_failed_generation(self):
        status, events, alive = self.exercise_retry_failure(
            subprocess.TimeoutExpired("connected Haxe retry", 120), uncertain_stop=True
        )
        self.assertEqual(status, 1)
        self.assertEqual(events, [("stop", 10001), ("stop", 10002)])
        self.assertEqual(alive, {10002})

    def test_unconfirmed_original_stop_prevents_retry_and_discard(self):
        status, events, alive = self.exercise_retry_failure(
            subprocess.TimeoutExpired("must not retry", 120), original_stop=True
        )
        self.assertEqual(status, 1)
        self.assertEqual(events, [("stop", 10001), ("stop", 10001)])
        self.assertEqual(alive, {10001, 10002})

    def test_publication_interrupt_and_failed_stop_retain_transaction(self):
        for publication in ("before", "after"):
            for uncertain in (True, "false"):
                with self.subTest(publication=publication, uncertain=uncertain):
                    status, events, alive = self.exercise_retry_failure(
                        AssertionError("retry must not run"), uncertain_stop=uncertain,
                        publication_failure=publication,
                    )
                    self.assertEqual(status, 130)
                    self.assertEqual(events, [("stop", 10001), ("stop", 10002)])
                    self.assertEqual(alive, {10002})

    def test_publication_interrupt_discards_only_after_replacement_exit(self):
        for publication in ("before", "after"):
            with self.subTest(publication=publication):
                status, events, alive = self.exercise_retry_failure(
                    AssertionError("retry must not run"), publication_failure=publication,
                )
                self.assertEqual(status, 130)
                self.assertEqual(events, [("stop", 10001), ("stop", 10002), ("discard", ())])
                self.assertEqual(alive, set())

    def test_attached_timeout_retains_output_without_stopping_owner(self):
        import play
        import io
        from contextlib import redirect_stdout, redirect_stderr
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            installation = HaxeInstallation("test", root / "haxe", root, root, None)
            lease = server.HaxeServerLease(
                server.HaxeServerConnection("127.0.0.1:12345", installation), False, None, None,
            )
            with (
                patch.dict(os.environ, {"HAXE_NO_SERVER": "0"}),
                patch.object(play, "pinned_haxe_installation", return_value=installation),
                patch.object(play, "verify_pinned_haxe"),
                patch.object(play, "attached_server", return_value=lease),
                patch.object(play, "OwnedHaxeServer") as owner,
                patch.object(play, "haxe_game_source_snapshot", return_value={}),
            patch.object(play, "haxe_server_compatibility", return_value={}),
            patch.object(play, "resolve_haxe_arguments", return_value=("unused.hxml",)),
                patch.object(play, "pinned_haxe_environment", return_value={}),
                patch.object(play.subprocess, "run", side_effect=subprocess.TimeoutExpired("attached", 120)),
                patch.object(play, "discard_transaction") as discard,
                redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()) as errors,
            ):
                status = play.main(["--compile-only", "--haxe-server", "attach",
                                    "--haxe-server-endpoint", lease.connection.endpoint,
                                    "--output-root", str(root / "output")])
            self.assertEqual(status, 1)
            owner.assert_not_called()
            discard.assert_not_called()
            self.assertIn("retained failed generation", errors.getvalue())

    def test_publication_interrupt_stops_replacement_before_propagating(self):
        import play
        from contextlib import redirect_stdout
        import io
        installation = HaxeInstallation("test", Path("haxe"), Path("std"), Path("libs"), None)
        first = server.HaxeServerLease(server.HaxeServerConnection("127.0.0.1:1", installation), True, "same", None)
        replacement = server.HaxeServerLease(server.HaxeServerConnection("127.0.0.1:2", installation), True, "same", None)
        owner = Mock()
        owner.state_root = Path("unused-server")
        owner.compatibility = {"canonicalServerStateRoot": str(owner.state_root.resolve())}
        owner.prepare_source_request.side_effect = lambda lease, sources: (lease, ())
        owner.restart_after_transport_failure.return_value = replacement
        with (
            patch.object(play, "haxe_game_source_snapshot", return_value={}),
            patch.object(play, "haxe_server_compatibility", return_value={}),
            patch.object(play, "resolve_haxe_arguments", return_value=("unused.hxml",)),
            patch.object(play, "pinned_haxe_environment", return_value={}),
            patch.object(play.subprocess, "run", return_value=subprocess.CompletedProcess([], 1, "", "reset")) as request,
            patch.object(play, "transport_failed", return_value=True),
            redirect_stdout(io.StringIO()),
        ):
            with self.assertRaises(play._LeasePublicationFailure) as caught:
                play.compile_haxe(Path("unused"), layout="split", platform_name="macos",
                                  raylib_configuration="desktop", server_lease=first,
                                  server_owner=owner, publish_server_lease=Mock(side_effect=KeyboardInterrupt()))
        self.assertIs(caught.exception.lease, replacement)
        self.assertIsInstance(caught.exception.failure, KeyboardInterrupt)
        owner.stop_lease.assert_not_called()
        self.assertEqual(request.call_count, 1)


class HaxeServerStopEvidenceTests(unittest.TestCase):
    """An unavailable identity lookup is not proof that a failed signal worked."""

    def test_oversized_signed_cookie_never_reaches_process_api(self):
        with patch.object(server.os, "kill") as kill:
            self.assertFalse(server._terminate_owned(stop_cookie(1 << 100)))
        kill.assert_not_called()

    @unittest.skipIf(os.name == "nt", "POSIX signal evidence")
    def test_missing_identity_and_denied_signal_are_not_success(self):
        with patch.object(server, "_owned_cookie", return_value=True), patch.object(
            server, "process_start_identity", return_value=None
        ), patch.object(server.os, "kill", side_effect=PermissionError("denied")):
            self.assertFalse(server._terminate_owned(stop_cookie(12345)))

    @unittest.skipIf(os.name == "nt", "POSIX positive absence")
    def test_kernel_confirmed_absence_is_success(self):
        with patch.object(server, "_owned_cookie", return_value=True), patch.object(
            server.os, "kill", side_effect=ProcessLookupError("absent")
        ):
            self.assertTrue(server._terminate_owned(stop_cookie(12345)))


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
        with patch.object(server, "windows_process_absent", return_value=False), patch.object(server, "process_exit_observer", return_value=nullcontext(lambda ms: not alive)), patch.object(server, "os", windows_os), patch.object(
            server, "_owned_cookie", side_effect=[True, still_owned]
        ) as ownership, patch.object(server.subprocess, "run", side_effect=taskkill), patch.object(
            server, "process_start_identity", side_effect=lambda pid: "live" if alive else None
        ), patch.object(server.time, "monotonic", return_value=0), patch.object(
            server.time, "sleep", side_effect=AssertionError("rejected or completed stop must not wait")
        ):
            result = server._terminate_owned(stop_cookie(123))
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

    def test_launcher_retry_reaps_replacement_before_disposal(self) -> None:
        self.exercise_launcher_retry(original_died=False)

    def test_launcher_retries_after_original_already_exited(self) -> None:
        self.exercise_launcher_retry(original_died=True)

    def exercise_launcher_retry(self, *, original_died: bool) -> None:
        import play
        import io
        from contextlib import redirect_stdout, redirect_stderr

        owner = self.owner("launcher-retry")
        unrelated = OwnedHaxeServer(
            Path(self.temporary.name) / "unrelated", installation=self.installation,
            compatibility={"test": "unrelated"},
        )
        self.owners.append(unrelated)
        foreign = unrelated.connect()
        output = play.prepare_output_root(Path(self.temporary.name) / "output")
        (output / "variants").mkdir()
        variant = play.prepare_output_root(
            output / "variants" / f"{play.host_platform()}-split-interactive"
        )
        previous = play.begin_transaction(variant)
        (previous.generated / "main.c").write_text("last-good-output\n")
        selected = play.publish_pointer(variant, play.finalize_transaction(variant, previous))
        pointer = variant / "hxc-play-current-generation.json"
        previous_pointer = pointer.read_bytes()
        leases = []
        connect = owner.connect
        def connected():
            lease = connect()
            leases.append(lease)
            return lease
        real_run = subprocess.run
        requests = 0
        def transport(command, **kwargs):
            nonlocal requests
            if "--connect" not in command:
                return real_run(command, **kwargs)
            requests += 1
            if requests == 1:
                if original_died:
                    self.assertTrue(owner.stop_lease(leases[-1]))
                return subprocess.CompletedProcess(command, 1, "", "connection reset")
            raise subprocess.TimeoutExpired(command, kwargs["timeout"])
        discard = play.discard_transaction
        disposed = []
        def dispose(transaction):
            self.assertEqual(len(leases), 2)
            for lease in leases:
                assert_process_absent(self, lease.process.pid)
            self.assertIsNotNone(process_start_identity(foreign.process.pid))
            disposed.append(transaction.root)
            discard(transaction)
        with (
            patch.dict(os.environ, {"HAXE_NO_SERVER": "0"}),
            patch.object(play, "pinned_haxe_installation", return_value=self.installation),
            patch.object(play, "verify_pinned_haxe"),
            patch.object(play, "haxe_server_compatibility", return_value={k: v for k, v in owner.compatibility.items() if k != "canonicalServerStateRoot"}),
            patch.object(play, "haxe_game_source_snapshot", return_value={}),
            patch.object(play, "OwnedHaxeServer", return_value=owner),
            patch.object(owner, "connect", side_effect=connected),
            patch.object(play, "resolve_haxe_arguments", return_value=("unused.hxml",)),
            patch.object(play, "transport_failed", return_value=True),
            patch.object(play.subprocess, "run", side_effect=transport),
            patch.object(play, "discard_transaction", side_effect=dispose),
            redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()) as diagnostics,
        ):
            status = play.main(["--compile-only", "--output-root", str(output)])
        self.assertEqual(status, 1)
        self.assertEqual(requests, 2, diagnostics.getvalue())
        self.assertEqual(len(disposed), 1, diagnostics.getvalue())
        self.assertFalse(disposed[0].exists())
        self.assertEqual(pointer.read_bytes(), previous_pointer)
        self.assertEqual(play.current_generation(variant).generation_id, selected.generation_id)
        self.assertEqual((selected.generated / "main.c").read_text(), "last-good-output\n")

    def test_already_dead_original_allows_one_replacement(self) -> None:
        owner = self.owner("already-dead-retry")
        original = owner.connect()
        self.assertTrue(owner.stop_lease(original))
        with patch.object(owner, "connect", wraps=owner.connect) as connect:
            replacement = owner.restart_after_transport_failure(original)
        connect.assert_called_once_with()
        self.assertNotEqual(original.process.pid, replacement.process.pid)
        self.assertIsNotNone(process_start_identity(replacement.process.pid))

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
