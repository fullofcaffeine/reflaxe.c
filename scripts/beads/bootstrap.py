#!/usr/bin/env python3
"""Preview or idempotently materialize the canonical Beads seed graph."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass, replace
from pathlib import Path
from typing import Protocol, cast


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.beads.hxc_beads_plan import (
    BeadsPlan,
    Epic,
    PlanError,
    Task,
    bullet_list,
    epic_description,
    issue_title,
    load_plan,
    prd_sections,
    stable_key_order,
    task_description,
)


DEFAULT_PLAN = ROOT / "docs/specs/beads-plan.json"
DEFAULT_STATE = Path(".hxc/beads-plan-state.json")


class BeadsCommandError(RuntimeError):
    """Raised when the external Beads CLI fails or returns malformed data."""


@dataclass(frozen=True)
class DependencyState:
    issue_id: str
    depends_on_id: str
    dependency_type: str


@dataclass(frozen=True)
class ExistingIssue:
    issue_id: str
    title: str
    issue_type: str
    status: str
    external_ref: str | None
    stable_key: str | None
    parent_id: str | None
    dependencies: tuple[DependencyState, ...]


@dataclass(frozen=True)
class IssueSpec:
    stable_key: str
    title: str
    issue_type: str
    priority: int
    labels: tuple[str, ...]
    description: str
    acceptance: str
    external_ref: str
    metadata: tuple[tuple[str, str], ...]
    parent_key: str | None

    def metadata_object(self) -> dict[str, str]:
        return dict(self.metadata)


@dataclass(frozen=True)
class MaterializeResult:
    created_issues: int
    reused_issues: int
    created_edges: int
    reused_edges: int
    issue_ids: tuple[tuple[str, str], ...]
    ready_tasks: tuple[str, ...]

    def payload(self, plan: BeadsPlan) -> dict[str, object]:
        return {
            "mode": "apply",
            "planVersion": plan.plan_version,
            "counts": {
                "createdIssues": self.created_issues,
                "reusedIssues": self.reused_issues,
                "createdBlockingEdges": self.created_edges,
                "reusedBlockingEdges": self.reused_edges,
            },
            "issueIds": dict(self.issue_ids),
            "readyTasks": list(self.ready_tasks),
        }


class BeadsClient(Protocol):
    def ensure_initialized(self) -> None: ...

    def list_issues(self) -> tuple[ExistingIssue, ...]: ...

    def create_issue(self, spec: IssueSpec, parent_id: str | None) -> ExistingIssue: ...

    def add_blocking_dependencies(
        self, edges: tuple[tuple[str, str], ...]
    ) -> None: ...

    def ready_issue_ids(self) -> tuple[str, ...]: ...


class SubprocessBeadsClient:
    """Narrow argument-array adapter around an installed `bd` executable."""

    def __init__(self, executable: str, workspace: Path) -> None:
        self.executable = executable
        self.workspace = workspace

    def ensure_initialized(self) -> None:
        if not self.workspace.is_dir():
            raise BeadsCommandError(
                f"workspace does not exist or is not a directory: {self.workspace}"
            )
        if (self.workspace / ".beads").is_dir():
            return
        self._run(
            (
                "init",
                "--quiet",
                "--non-interactive",
                "--skip-agents",
                "--skip-hooks",
            )
        )

    def list_issues(self) -> tuple[ExistingIssue, ...]:
        raw = self._run_json(("list", "--all", "--limit", "0", "--json"))
        if not isinstance(raw, list):
            raise BeadsCommandError("bd list --json must return an array")
        return tuple(
            parse_existing_issue(entry, f"bd list[{index}]")
            for index, entry in enumerate(cast(list[object], raw))
        )

    def create_issue(self, spec: IssueSpec, parent_id: str | None) -> ExistingIssue:
        arguments = [
            "create",
            "--title",
            spec.title,
            "--type",
            spec.issue_type,
            "--priority",
            str(spec.priority),
            "--labels",
            ",".join(spec.labels),
            "--description",
            spec.description,
            "--acceptance",
            spec.acceptance,
            "--external-ref",
            spec.external_ref,
            "--metadata",
            json.dumps(
                spec.metadata_object(),
                ensure_ascii=False,
                sort_keys=True,
                separators=(",", ":"),
            ),
        ]
        if parent_id is not None:
            arguments.extend(("--parent", parent_id, "--no-inherit-labels"))
        arguments.append("--json")
        raw = self._run_json(tuple(arguments))
        issue = parse_existing_issue(raw, f"bd create {spec.stable_key}")
        if parent_id is not None and issue.parent_id is None:
            issue = replace(
                issue,
                parent_id=parent_id,
                dependencies=issue.dependencies
                + (DependencyState(issue.issue_id, parent_id, "parent-child"),),
            )
        return issue

    def add_blocking_dependencies(
        self, edges: tuple[tuple[str, str], ...]
    ) -> None:
        if not edges:
            return
        records = "".join(
            json.dumps(
                {"from": issue_id, "to": blocker_id, "type": "blocks"},
                sort_keys=True,
                separators=(",", ":"),
            )
            + "\n"
            for issue_id, blocker_id in edges
        )
        self._run(("dep", "add", "--file", "-", "--json"), input_text=records)

    def ready_issue_ids(self) -> tuple[str, ...]:
        raw = self._run_json(("list", "--ready", "--limit", "0", "--json"))
        if not isinstance(raw, list):
            raise BeadsCommandError("bd list --ready --json must return an array")
        issue_ids: list[str] = []
        for index, entry in enumerate(cast(list[object], raw)):
            issue = expect_object(entry, f"bd ready[{index}]")
            issue_ids.append(expect_string(issue.get("id"), f"bd ready[{index}].id"))
        return tuple(issue_ids)

    def _run_json(self, arguments: tuple[str, ...]) -> object:
        output = self._run(arguments)
        try:
            return json.loads(output)
        except json.JSONDecodeError as error:
            raise BeadsCommandError(
                f"bd returned malformed JSON for {' '.join(arguments[:2])}: {error}"
            ) from error

    def _run(self, arguments: tuple[str, ...], *, input_text: str | None = None) -> str:
        environment = os.environ.copy()
        environment["BD_NON_INTERACTIVE"] = "1"
        if arguments and arguments[0] == "init":
            command = (self.executable, *arguments)
            working_directory: Path | None = self.workspace
        else:
            command = (
                self.executable,
                "-C",
                str(self.workspace),
                *arguments,
            )
            working_directory = None
        try:
            completed = subprocess.run(
                command,
                check=False,
                capture_output=True,
                text=True,
                env=environment,
                cwd=working_directory,
                input=input_text,
            )
        except OSError as error:
            raise BeadsCommandError(
                f"cannot execute bd command {arguments[0]!r}: {error}"
            ) from error
        if completed.returncode != 0:
            detail = completed.stderr.strip() or completed.stdout.strip()
            raise BeadsCommandError(
                f"bd command failed ({completed.returncode}): "
                f"{' '.join(arguments[:3])}: {detail}"
            )
        return completed.stdout


def epic_spec(plan: BeadsPlan, epic: Epic) -> IssueSpec:
    return IssueSpec(
        epic.stable_key,
        issue_title(plan.project.issue_title_prefix, epic.stable_key, epic.title),
        "epic",
        epic.priority,
        epic.labels,
        epic_description(epic),
        bullet_list(epic.acceptance),
        f"hxc-plan:{epic.stable_key}",
        (
            ("source", "docs/specs/beads-plan.json"),
            ("milestone", epic.milestone),
            ("stable_key", epic.stable_key),
            ("plan_version", plan.plan_version),
        ),
        None,
    )


def task_spec(plan: BeadsPlan, task: Task) -> IssueSpec:
    return IssueSpec(
        task.stable_key,
        issue_title(plan.project.issue_title_prefix, task.stable_key, task.title),
        task.issue_type,
        task.priority,
        task.labels,
        task_description(task),
        bullet_list(task.acceptance),
        f"hxc-plan:{task.stable_key}",
        (
            ("source", "docs/specs/beads-plan.json"),
            ("epic_key", task.epic_key),
            ("stable_key", task.stable_key),
            ("plan_version", plan.plan_version),
            ("prd_sections", ", ".join(prd_sections(task))),
            ("requirement_ids", ", ".join(task.requirement_ids)),
            ("effort", task.effort),
        ),
        task.epic_key,
    )


def all_specs(plan: BeadsPlan) -> tuple[IssueSpec, ...]:
    return tuple(epic_spec(plan, epic) for epic in plan.epics) + tuple(
        task_spec(plan, task) for task in plan.tasks
    )


def preview_payload(plan: BeadsPlan) -> dict[str, object]:
    return {
        "mode": "preview",
        "planVersion": plan.plan_version,
        "counts": {
            "epics": len(plan.epics),
            "tasks": len(plan.tasks),
            "issues": len(plan.epics) + len(plan.tasks),
            "blockingEdges": len(plan.blocking_edges()),
            "coveredRequirementIds": len(plan.unique_requirement_ids()),
        },
        "initialReady": list(plan.initial_ready()),
        "issueOrder": [spec.stable_key for spec in all_specs(plan)],
        "blockingEdges": [
            {"child": child, "blocker": blocker}
            for child, blocker in plan.blocking_edges()
        ],
    }


def materialize(
    plan: BeadsPlan, client: BeadsClient, state_path: Path
) -> MaterializeResult:
    client.ensure_initialized()
    live_issues = list(client.list_issues())
    by_id = {issue.issue_id: issue for issue in live_issues}
    if len(by_id) != len(live_issues):
        raise PlanError("bd returned duplicate issue IDs")

    by_key: dict[str, ExistingIssue] = {}
    by_title: dict[str, list[ExistingIssue]] = {}
    for issue in live_issues:
        if issue.stable_key is not None:
            if issue.stable_key in by_key:
                raise PlanError(
                    f"multiple live issues claim stable key {issue.stable_key}"
                )
            by_key[issue.stable_key] = issue
        by_title.setdefault(issue.title, []).append(issue)

    state = read_state(state_path)
    issue_for_key: dict[str, ExistingIssue] = {}
    created = 0
    reused = 0

    for spec in all_specs(plan):
        parent_id = (
            issue_for_key[spec.parent_key].issue_id
            if spec.parent_key is not None
            else None
        )
        issue = resolve_existing(spec, state, by_id, by_key, by_title)
        if issue is None:
            issue = client.create_issue(spec, parent_id)
            created += 1
            live_issues.append(issue)
            by_id[issue.issue_id] = issue
            by_key[spec.stable_key] = issue
            by_title.setdefault(issue.title, []).append(issue)
        else:
            reused += 1
        validate_existing(spec, issue, parent_id)
        issue_for_key[spec.stable_key] = issue

    existing_edges = {
        (
            dependency.issue_id,
            dependency.depends_on_id,
            dependency.dependency_type,
        )
        for issue in live_issues
        for dependency in issue.dependencies
    }
    missing_edges: list[tuple[str, str]] = []
    reused_edges = 0
    for child_key, blocker_key in plan.blocking_edges():
        child_id = issue_for_key[child_key].issue_id
        blocker_id = issue_for_key[blocker_key].issue_id
        edge = (child_id, blocker_id, "blocks")
        if edge in existing_edges:
            reused_edges += 1
            continue
        missing_edges.append((child_id, blocker_id))
        existing_edges.add(edge)
    client.add_blocking_dependencies(tuple(missing_edges))
    created_edges = len(missing_edges)

    expected_ready = expected_ready_keys(plan, issue_for_key)
    seed_task_ids = {
        issue_for_key[task.stable_key].issue_id: task.stable_key
        for task in plan.tasks
    }
    actual_ready = tuple(
        sorted(
            (
                seed_task_ids[issue_id]
                for issue_id in client.ready_issue_ids()
                if issue_id in seed_task_ids
            ),
            key=stable_key_order,
        )
    )
    if actual_ready != expected_ready:
        raise PlanError(
            "bd ready disagrees with the canonical graph: "
            f"expected {list(expected_ready)}, got {list(actual_ready)}"
        )

    mapping = tuple(
        (key, issue_for_key[key].issue_id)
        for key in sorted(issue_for_key, key=stable_key_order)
    )
    write_state(state_path, plan.plan_version, mapping)
    return MaterializeResult(
        created,
        reused,
        created_edges,
        reused_edges,
        mapping,
        actual_ready,
    )


def expected_ready_keys(
    plan: BeadsPlan, issue_for_key: dict[str, ExistingIssue]
) -> tuple[str, ...]:
    ready: list[str] = []
    for task in plan.tasks:
        issue = issue_for_key[task.stable_key]
        if issue.status != "open":
            continue
        if all(
            issue_for_key[blocker].status == "closed"
            for blocker in task.blocked_by
        ):
            ready.append(task.stable_key)
    return tuple(sorted(ready, key=stable_key_order))


def resolve_existing(
    spec: IssueSpec,
    state: dict[str, str],
    by_id: dict[str, ExistingIssue],
    by_key: dict[str, ExistingIssue],
    by_title: dict[str, list[ExistingIssue]],
) -> ExistingIssue | None:
    state_id = state.get(spec.stable_key)
    if state_id is not None and state_id in by_id:
        return by_id[state_id]
    if spec.stable_key in by_key:
        return by_key[spec.stable_key]
    title_matches = by_title.get(spec.title, [])
    if len(title_matches) > 1:
        raise PlanError(
            f"multiple live issues match fallback title for {spec.stable_key}"
        )
    return title_matches[0] if title_matches else None


def validate_existing(
    spec: IssueSpec, issue: ExistingIssue, parent_id: str | None
) -> None:
    if issue.title != spec.title:
        raise PlanError(
            f"stable key {spec.stable_key} maps to unexpected title {issue.title!r}"
        )
    if issue.issue_type != spec.issue_type:
        raise PlanError(
            f"stable key {spec.stable_key} maps to type {issue.issue_type!r}, "
            f"expected {spec.issue_type!r}"
        )
    if issue.external_ref not in (None, spec.external_ref):
        raise PlanError(
            f"stable key {spec.stable_key} has conflicting external reference"
        )
    if issue.stable_key not in (None, spec.stable_key):
        raise PlanError(f"stable key metadata conflicts for {spec.stable_key}")
    if issue.parent_id != parent_id:
        raise PlanError(
            f"stable key {spec.stable_key} has parent {issue.parent_id!r}, "
            f"expected {parent_id!r}"
        )


def read_state(path: Path) -> dict[str, str]:
    if not path.exists():
        return {}
    try:
        decoded: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise PlanError(f"cannot read bootstrap state {path}: {error}") from error
    root = expect_object(decoded, "bootstrap state")
    if root.get("schemaVersion") != 1:
        raise PlanError("bootstrap state schemaVersion must be 1")
    issues = expect_object(root.get("issues"), "bootstrap state.issues")
    mapping: dict[str, str] = {}
    for stable_key, issue_id_value in issues.items():
        stable_key_order(stable_key)
        mapping[stable_key] = expect_string(
            issue_id_value, f"bootstrap state.issues.{stable_key}"
        )
    return mapping


def write_state(
    path: Path, plan_version: str, mapping: tuple[tuple[str, str], ...]
) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    payload = {
        "schemaVersion": 1,
        "planVersion": plan_version,
        "issues": dict(mapping),
    }
    rendered = json.dumps(payload, indent=2, ensure_ascii=False) + "\n"
    descriptor, temporary_name = tempfile.mkstemp(
        prefix=f".{path.name}.", suffix=".tmp", dir=path.parent
    )
    try:
        with os.fdopen(descriptor, "w", encoding="utf-8", newline="\n") as output:
            output.write(rendered)
            output.flush()
            os.fsync(output.fileno())
        os.replace(temporary_name, path)
    finally:
        if os.path.exists(temporary_name):
            os.unlink(temporary_name)


def parse_existing_issue(value: object, label: str) -> ExistingIssue:
    raw = expect_object(value, label)
    issue_id = expect_string(raw.get("id"), f"{label}.id")
    title = expect_string(raw.get("title"), f"{label}.title")
    issue_type = expect_string(raw.get("issue_type"), f"{label}.issue_type")
    status = expect_string(raw.get("status"), f"{label}.status")
    external_ref = optional_string(raw.get("external_ref"), f"{label}.external_ref")
    metadata_raw = raw.get("metadata", {})
    metadata = expect_object(metadata_raw, f"{label}.metadata")
    metadata_key = optional_string(
        metadata.get("stable_key"), f"{label}.metadata.stable_key"
    )
    external_key: str | None = None
    if external_ref is not None and external_ref.startswith("hxc-plan:"):
        external_key = external_ref.removeprefix("hxc-plan:")
    if (
        external_key is not None
        and metadata_key is not None
        and external_key != metadata_key
    ):
        raise BeadsCommandError(f"{label} has conflicting stable-key authorities")
    stable_key = metadata_key or external_key
    parent_id = optional_string(raw.get("parent"), f"{label}.parent")

    dependencies_value = raw.get("dependencies", [])
    if not isinstance(dependencies_value, list):
        raise BeadsCommandError(f"{label}.dependencies must be an array")
    dependencies: list[DependencyState] = []
    for index, entry in enumerate(cast(list[object], dependencies_value)):
        dependency = expect_object(entry, f"{label}.dependencies[{index}]")
        dependencies.append(
            DependencyState(
                expect_string(
                    dependency.get("issue_id"),
                    f"{label}.dependencies[{index}].issue_id",
                ),
                expect_string(
                    dependency.get("depends_on_id"),
                    f"{label}.dependencies[{index}].depends_on_id",
                ),
                expect_string(
                    dependency.get("type"),
                    f"{label}.dependencies[{index}].type",
                ),
            )
        )
    return ExistingIssue(
        issue_id,
        title,
        issue_type,
        status,
        external_ref,
        stable_key,
        parent_id,
        tuple(dependencies),
    )


def expect_object(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise BeadsCommandError(f"{label} must be an object")
    return cast(dict[str, object], value)


def expect_string(value: object, label: str) -> str:
    if not isinstance(value, str) or not value:
        raise BeadsCommandError(f"{label} must be a non-empty string")
    return value


def optional_string(value: object, label: str) -> str | None:
    if value is None:
        return None
    return expect_string(value, label)


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description=(
            "Preview the canonical Beads graph without bd, or apply it "
            "idempotently when bd is installed."
        )
    )
    mode = result.add_mutually_exclusive_group()
    mode.add_argument("--apply", action="store_true")
    mode.add_argument("--dry-run", action="store_true")
    result.add_argument("--json", action="store_true", dest="json_output")
    result.add_argument("--plan", type=Path, default=DEFAULT_PLAN)
    result.add_argument("--workspace", type=Path, default=ROOT)
    result.add_argument(
        "--bd-command",
        help="Explicit bd executable path/name for apply-mode testing or wrappers.",
    )
    return result


def main(arguments: list[str] | None = None) -> int:
    options = parser().parse_args(arguments)
    try:
        plan = load_plan(options.plan.resolve())
        if not options.apply:
            payload = preview_payload(plan)
        else:
            executable = options.bd_command or shutil.which("bd")
            if executable is None:
                raise BeadsCommandError(
                    "apply requires an installed bd executable; preview mode does not"
                )
            workspace = options.workspace.resolve()
            state_path = workspace / DEFAULT_STATE
            result = materialize(
                plan,
                SubprocessBeadsClient(executable, workspace),
                state_path,
            )
            payload = result.payload(plan)
    except (PlanError, BeadsCommandError) as error:
        print(f"beads bootstrap failed: {error}", file=sys.stderr)
        return 1

    if options.json_output:
        print(json.dumps(payload, indent=2, ensure_ascii=False, sort_keys=True))
    elif payload["mode"] == "preview":
        counts = cast(dict[str, int], payload["counts"])
        print(
            "beads bootstrap preview: "
            f"{counts['epics']} epics, {counts['tasks']} tasks, "
            f"{counts['blockingEdges']} blocking edges"
        )
        print("initial-ready: " + ", ".join(plan.initial_ready()))
        print("no Beads state changed; pass --apply to materialize")
    else:
        counts = cast(dict[str, int], payload["counts"])
        print(
            "beads bootstrap apply: "
            f"created {counts['createdIssues']} issues and "
            f"{counts['createdBlockingEdges']} blocking edges; reused "
            f"{counts['reusedIssues']} issues and "
            f"{counts['reusedBlockingEdges']} blocking edges"
        )
        ready = cast(list[str], payload["readyTasks"])
        print("ready: " + (", ".join(ready) if ready else "none"))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
