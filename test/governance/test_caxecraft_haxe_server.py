#!/usr/bin/env python3
"""Focused lifecycle tests for Caxecraft's worktree-owned Haxe server."""

from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = ROOT / "examples/caxecraft"
sys.path.insert(0, str(CASE))

from dev_haxe_server import (  # noqa: E402
    OwnedHaxeServer,
    attached_server,
    pinned_haxe_installation,
    process_command_identity,
    process_start_identity,
    verify_pinned_haxe,
)


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
