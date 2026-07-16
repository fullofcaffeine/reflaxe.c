from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
import unittest
from dataclasses import replace
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
PLAN_PATH = ROOT / "docs/specs/beads-plan.json"
BOOTSTRAP = ROOT / "scripts/beads/bootstrap.py"

from scripts.beads.bootstrap import (
    DependencyState,
    ExistingIssue,
    IssueSpec,
    MaterializeResult,
    epic_spec,
    materialize,
    task_spec,
)
from scripts.beads.hxc_beads_plan import PlanError, load_plan, parse_plan


class MemoryBeadsClient:
    """Typed in-memory proof of materialization semantics, independent of `bd`."""

    def __init__(self) -> None:
        self.initialized = False
        self.issues: dict[str, ExistingIssue] = {}
        self.specs: dict[str, IssueSpec] = {}
        self.next_identifier = 1

    def ensure_initialized(self) -> None:
        self.initialized = True

    def list_issues(self) -> tuple[ExistingIssue, ...]:
        return tuple(self.issues[key] for key in sorted(self.issues))

    def create_issue(self, spec: IssueSpec, parent_id: str | None) -> ExistingIssue:
        issue_id = f"memory-{self.next_identifier:03d}"
        self.next_identifier += 1
        dependencies = (
            (DependencyState(issue_id, parent_id, "parent-child"),)
            if parent_id is not None
            else ()
        )
        issue = ExistingIssue(
            issue_id,
            spec.title,
            spec.issue_type,
            "open",
            spec.external_ref,
            spec.stable_key,
            parent_id,
            dependencies,
        )
        self.issues[issue_id] = issue
        self.specs[spec.stable_key] = spec
        return issue

    def add_blocking_dependencies(
        self, edges: tuple[tuple[str, str], ...]
    ) -> None:
        for issue_id, blocker_id in edges:
            issue = self.issues[issue_id]
            dependency = DependencyState(issue_id, blocker_id, "blocks")
            self.issues[issue_id] = replace(
                issue, dependencies=issue.dependencies + (dependency,)
            )

    def ready_issue_ids(self) -> tuple[str, ...]:
        ready: list[str] = []
        for issue in self.issues.values():
            if issue.status != "open":
                continue
            blockers = (
                dependency.depends_on_id
                for dependency in issue.dependencies
                if dependency.dependency_type == "blocks"
            )
            if all(self.issues[blocker].status == "closed" for blocker in blockers):
                ready.append(issue.issue_id)
        return tuple(sorted(ready))

    def close(self, stable_key: str) -> None:
        issue = self.issue_for_key(stable_key)
        self.issues[issue.issue_id] = replace(issue, status="closed")

    def remove_stable_identity(self, stable_key: str) -> None:
        issue = self.issue_for_key(stable_key)
        self.issues[issue.issue_id] = replace(
            issue, external_ref=None, stable_key=None
        )

    def issue_for_key(self, stable_key: str) -> ExistingIssue:
        matches = [
            issue
            for issue in self.issues.values()
            if issue.stable_key == stable_key
            or issue.external_ref == f"hxc-plan:{stable_key}"
            or issue.title == self.specs[stable_key].title
        ]
        if len(matches) != 1:
            raise AssertionError(f"expected one issue for {stable_key}: {matches}")
        return matches[0]


class BeadsPlanTests(unittest.TestCase):
    def setUp(self) -> None:
        self.plan = load_plan(PLAN_PATH)

    def test_canonical_plan_has_complete_traceable_graph(self) -> None:
        self.assertEqual(len(self.plan.epics), 11)
        self.assertEqual(len(self.plan.tasks), 122)
        self.assertEqual(len(self.plan.blocking_edges()), 274)
        self.assertEqual(len(self.plan.unique_requirement_ids()), 160)
        self.assertEqual(self.plan.initial_ready(), ("E0.T01",))
        self.assertEqual(len(self.plan.topological_order()), 122)

        task = self.plan.task_by_key()["E0.T08"]
        self.assertEqual(
            task.requirement_ids,
            ("HXC-SCAF-007", "HXC-SCAF-008", "HXC-QA-014"),
        )
        self.assertIn("Dry-run works without bd installed.", task.acceptance)

    def test_semantic_validator_rejects_invalid_graphs(self) -> None:
        base = json.loads(PLAN_PATH.read_text(encoding="utf-8"))

        duplicate = json.loads(json.dumps(base))
        duplicate["tasks"][1]["key"] = duplicate["tasks"][0]["key"]
        with self.assertRaisesRegex(PlanError, "duplicate task stable key"):
            parse_plan(duplicate)

        unknown = json.loads(json.dumps(base))
        unknown["tasks"][0]["blockedBy"] = ["E99.T99"]
        with self.assertRaisesRegex(PlanError, "unknown blocker E99.T99"):
            parse_plan(unknown)

        cycle = json.loads(json.dumps(base))
        cycle["tasks"][0]["blockedBy"] = ["E0.T02"]
        with self.assertRaisesRegex(PlanError, "contains a cycle"):
            parse_plan(cycle)

        malformed_requirement = json.loads(json.dumps(base))
        malformed_requirement["tasks"][0]["requirements"] = ["HXC-NOT-TYPED"]
        with self.assertRaisesRegex(PlanError, "contains invalid ID"):
            parse_plan(malformed_requirement)

    def test_preview_is_deterministic_and_does_not_require_bd(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            workspace = Path(temporary)
            environment = os.environ.copy()
            environment["PATH"] = ""
            command = [
                sys.executable,
                str(BOOTSTRAP),
                "--json",
                "--workspace",
                str(workspace),
            ]
            first = subprocess.run(
                command,
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                env=environment,
            )
            second = subprocess.run(
                [*command, "--dry-run"],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                env=environment,
            )
            self.assertEqual(first.returncode, 0, first.stderr)
            self.assertEqual(second.returncode, 0, second.stderr)
            self.assertEqual(first.stdout, second.stdout)
            payload = json.loads(first.stdout)
            self.assertEqual(payload["counts"]["issues"], 133)
            self.assertEqual(payload["counts"]["blockingEdges"], 274)
            self.assertEqual(payload["initialReady"], ["E0.T01"])
            self.assertFalse((workspace / ".beads").exists())
            self.assertFalse((workspace / ".hxc").exists())

    def test_apply_without_bd_fails_before_writing(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            workspace = Path(temporary)
            environment = os.environ.copy()
            environment["PATH"] = ""
            result = subprocess.run(
                [
                    sys.executable,
                    str(BOOTSTRAP),
                    "--apply",
                    "--workspace",
                    str(workspace),
                ],
                cwd=ROOT,
                check=False,
                capture_output=True,
                text=True,
                env=environment,
            )
            self.assertEqual(result.returncode, 1)
            self.assertIn("apply requires an installed bd executable", result.stderr)
            self.assertFalse((workspace / ".beads").exists())
            self.assertFalse((workspace / ".hxc").exists())

    def test_apply_creates_complete_graph_then_is_idempotent(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            state = Path(temporary) / ".hxc/beads-plan-state.json"
            client = MemoryBeadsClient()

            first = materialize(self.plan, client, state)
            self.assertMaterialized(first, created=True)
            self.assertTrue(client.initialized)
            self.assertEqual(len(client.issues), 133)
            self.assertEqual(first.ready_tasks, ("E0.T01",))

            epic = client.specs["E0"]
            self.assertEqual(epic.issue_type, "epic")
            self.assertIn("## Success Criteria", epic.description)
            self.assertIn("Beads plan can dry-run", epic.acceptance)

            task = client.specs["E0.T08"]
            self.assertEqual(task.parent_key, "E0")
            self.assertIn("Requirement IDs: HXC-SCAF-007", task.description)
            self.assertIn("PRD sections: §24", task.description)
            self.assertIn("hxc-plan", task.labels)
            self.assertEqual(
                task.metadata_object()["requirement_ids"],
                "HXC-SCAF-007, HXC-SCAF-008, HXC-QA-014",
            )

            second = materialize(self.plan, client, state)
            self.assertMaterialized(second, created=False)
            self.assertEqual(len(client.issues), 133)
            self.assertEqual(first.issue_ids, second.issue_ids)

    def test_closed_issue_and_exact_title_fallback_are_reused(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            state = Path(temporary) / ".hxc/beads-plan-state.json"
            client = MemoryBeadsClient()
            materialize(self.plan, client, state)

            state.unlink()
            stable_key_result = materialize(self.plan, client, state)
            self.assertEqual(stable_key_result.created_issues, 0)
            self.assertEqual(stable_key_result.reused_issues, 133)

            client.close("E0.T01")
            client.remove_stable_identity("E0.T01")
            state.unlink()

            result = materialize(self.plan, client, state)
            self.assertEqual(result.created_issues, 0)
            self.assertEqual(result.reused_issues, 133)
            self.assertEqual(client.issue_for_key("E0.T01").status, "closed")
            expected_ready = tuple(
                task.stable_key
                for task in self.plan.tasks
                if task.blocked_by == ("E0.T01",)
            )
            self.assertEqual(result.ready_tasks, expected_ready)
            self.assertEqual(len(client.issues), 133)

    def assertMaterialized(
        self, result: MaterializeResult, *, created: bool
    ) -> None:
        if created:
            self.assertEqual(result.created_issues, 133)
            self.assertEqual(result.reused_issues, 0)
            self.assertEqual(result.created_edges, 274)
            self.assertEqual(result.reused_edges, 0)
        else:
            self.assertEqual(result.created_issues, 0)
            self.assertEqual(result.reused_issues, 133)
            self.assertEqual(result.created_edges, 0)
            self.assertEqual(result.reused_edges, 274)


if __name__ == "__main__":
    unittest.main()
