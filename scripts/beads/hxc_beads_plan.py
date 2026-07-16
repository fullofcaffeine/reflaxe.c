#!/usr/bin/env python3
"""Typed model and semantic validator for the reproducible Beads seed graph."""

from __future__ import annotations

import heapq
import json
import re
from collections.abc import Iterable
from dataclasses import dataclass
from pathlib import Path
from typing import cast


EPIC_KEY = re.compile(r"^E(0|[1-9][0-9]*)$")
TASK_KEY = re.compile(r"^E(0|[1-9][0-9]*)\.T([0-9]{2})$")
PLAN_VERSION = re.compile(r"^[0-9]{4}-[0-9]{2}-[0-9]{2}$")
REQUIREMENT_ID = re.compile(r"^HXC-[A-Z]+-[0-9]{3}$")
LABEL = re.compile(r"^[a-z0-9][a-z0-9.-]*$")
ALLOWED_ISSUE_TYPES = frozenset(
    {"task", "feature", "bug", "chore", "decision"}
)
ALLOWED_EFFORTS = frozenset({"small", "medium", "large", "xlarge"})


class PlanError(RuntimeError):
    """Raised when the checked-in bootstrap plan violates its contract."""


@dataclass(frozen=True)
class ProjectContract:
    name: str
    issue_title_prefix: str
    source_of_intent: str
    source_of_execution: str
    bootstrap: str
    rules: tuple[str, ...]


@dataclass(frozen=True)
class Epic:
    stable_key: str
    title: str
    priority: int
    milestone: str
    description: str
    acceptance: tuple[str, ...]
    labels: tuple[str, ...]


@dataclass(frozen=True)
class Task:
    stable_key: str
    epic_key: str
    title: str
    priority: int
    blocked_by: tuple[str, ...]
    prd_references: tuple[str, ...]
    requirement_ids: tuple[str, ...]
    outcome: str
    acceptance: tuple[str, ...]
    labels: tuple[str, ...]
    effort: str
    issue_type: str
    description: str


@dataclass(frozen=True)
class BeadsPlan:
    schema_version: int
    plan_version: str
    project: ProjectContract
    epics: tuple[Epic, ...]
    tasks: tuple[Task, ...]

    def epic_by_key(self) -> dict[str, Epic]:
        return {epic.stable_key: epic for epic in self.epics}

    def task_by_key(self) -> dict[str, Task]:
        return {task.stable_key: task for task in self.tasks}

    def blocking_edges(self) -> tuple[tuple[str, str], ...]:
        return tuple(
            (task.stable_key, blocker)
            for task in self.tasks
            for blocker in task.blocked_by
        )

    def initial_ready(self) -> tuple[str, ...]:
        return tuple(task.stable_key for task in self.tasks if not task.blocked_by)

    def topological_order(self) -> tuple[str, ...]:
        task_by_key = self.task_by_key()
        indegree = {
            task.stable_key: len(task.blocked_by) for task in self.tasks
        }
        dependents: dict[str, list[str]] = {
            stable_key: [] for stable_key in task_by_key
        }
        for task in self.tasks:
            for blocker in task.blocked_by:
                dependents[blocker].append(task.stable_key)

        ready: list[tuple[tuple[int, int, str], str]] = []
        for stable_key, count in indegree.items():
            if count == 0:
                heapq.heappush(ready, (stable_key_order(stable_key), stable_key))

        ordered: list[str] = []
        while ready:
            _, stable_key = heapq.heappop(ready)
            ordered.append(stable_key)
            for dependent in sorted(
                dependents[stable_key], key=stable_key_order
            ):
                indegree[dependent] -= 1
                if indegree[dependent] == 0:
                    heapq.heappush(
                        ready, (stable_key_order(dependent), dependent)
                    )

        if len(ordered) != len(self.tasks):
            remaining = sorted(
                (key for key, count in indegree.items() if count > 0),
                key=stable_key_order,
            )
            raise PlanError(
                "blocking dependency graph contains a cycle involving: "
                + ", ".join(remaining)
            )
        return tuple(ordered)

    def unique_requirement_ids(self) -> tuple[str, ...]:
        return tuple(
            sorted(
                {
                    requirement
                    for task in self.tasks
                    for requirement in task.requirement_ids
                }
            )
        )


def stable_key_order(stable_key: str) -> tuple[int, int, str]:
    task_match = TASK_KEY.fullmatch(stable_key)
    if task_match is not None:
        return (int(task_match.group(1)), int(task_match.group(2)), stable_key)
    epic_match = EPIC_KEY.fullmatch(stable_key)
    if epic_match is not None:
        return (int(epic_match.group(1)), -1, stable_key)
    raise PlanError(f"invalid stable key: {stable_key!r}")


def issue_title(prefix: str, stable_key: str, title: str) -> str:
    return f"[{prefix}-{stable_key.replace('.', '-')}] {title}"


def bullet_list(items: tuple[str, ...]) -> str:
    return "\n".join(f"- {item}" for item in items)


def prd_sections(task: Task) -> tuple[str, ...]:
    return tuple(
        reference for reference in task.prd_references if reference.startswith("§")
    )


def epic_description(epic: Epic) -> str:
    return (
        f"Stable plan key: {epic.stable_key}\n"
        f"Milestone: {epic.milestone}\n"
        "Source: docs/specs/beads-plan.json and docs/PRD.md\n\n"
        f"Outcome: {epic.description}\n\n"
        "Scope: Coordinate the acceptance-tested child issues for this "
        "milestone. Parent-child hierarchy is structural and does not imply "
        "blocking.\n\n"
        "Execution: Children may proceed in parallel unless an explicit "
        "blocking edge says otherwise.\n\n"
        "## Success Criteria\n\n"
        f"{bullet_list(epic.acceptance)}"
    )


def task_description(task: Task) -> str:
    dependencies = ", ".join(task.blocked_by) if task.blocked_by else "None"
    sections = ", ".join(prd_sections(task))
    requirements = ", ".join(task.requirement_ids)
    return (
        f"Stable plan key: {task.stable_key}\n"
        f"Requirement IDs: {requirements}\n"
        f"PRD sections: {sections}\n\n"
        f"Context: {task.outcome}\n\n"
        f"Scope: {task.outcome}\n\n"
        "Non-scope: Work outside this outcome and its acceptance criteria. "
        "Create a linked discovered-from issue instead of silently widening "
        "scope.\n\n"
        f"Dependencies: {dependencies}\n\n"
        "Validation: Satisfy every acceptance item with executable evidence "
        "and the applicable PRD §40 definition-of-done gates; record exact "
        "commands and artifacts in the close reason.\n\n"
        "Compatibility: Assess and document portable/metal, runtime-policy, "
        "environment, generated-C, and public-ABI impact where applicable. "
        "Do not harden an unresolved public contract without an accepted "
        "ADR.\n\n"
        "Documentation: Update affected ADRs, documentation, diagnostics, "
        "capability data, and ledgers required by the acceptance criteria "
        "and PRD §40."
    )


def load_plan(path: Path) -> BeadsPlan:
    try:
        raw_text = path.read_text(encoding="utf-8")
    except (OSError, UnicodeError) as error:
        raise PlanError(f"cannot read plan {path}: {error}") from error
    try:
        decoded: object = json.loads(raw_text)
    except json.JSONDecodeError as error:
        raise PlanError(f"plan is not valid JSON: {error}") from error
    return parse_plan(decoded)


def parse_plan(value: object) -> BeadsPlan:
    root = expect_object(value, "plan")
    require_exact_keys(
        root,
        {"schemaVersion", "planVersion", "project", "epics", "tasks"},
        "plan",
    )
    schema_version = expect_int(root["schemaVersion"], "schemaVersion")
    if schema_version != 1:
        raise PlanError("schemaVersion must be 1")
    plan_version = expect_string(root["planVersion"], "planVersion")
    if PLAN_VERSION.fullmatch(plan_version) is None:
        raise PlanError("planVersion must use YYYY-MM-DD")

    project = parse_project(root["project"])
    epics = parse_epics(root["epics"])
    tasks = parse_tasks(root["tasks"])
    plan = BeadsPlan(schema_version, plan_version, project, epics, tasks)
    validate_graph(plan)
    plan.topological_order()
    return plan


def parse_project(value: object) -> ProjectContract:
    raw = expect_object(value, "project")
    require_exact_keys(
        raw,
        {
            "name",
            "issueTitlePrefix",
            "sourceOfIntent",
            "sourceOfExecution",
            "bootstrap",
            "rules",
        },
        "project",
    )
    prefix = expect_string(raw["issueTitlePrefix"], "project.issueTitlePrefix")
    if re.fullmatch(r"[A-Z][A-Z0-9]*", prefix) is None:
        raise PlanError("project.issueTitlePrefix must be an uppercase identifier")
    return ProjectContract(
        expect_string(raw["name"], "project.name"),
        prefix,
        expect_string(raw["sourceOfIntent"], "project.sourceOfIntent"),
        expect_string(raw["sourceOfExecution"], "project.sourceOfExecution"),
        expect_string(raw["bootstrap"], "project.bootstrap"),
        expect_string_tuple(raw["rules"], "project.rules", nonempty=True),
    )


def parse_epics(value: object) -> tuple[Epic, ...]:
    entries = expect_array(value, "epics")
    if not entries:
        raise PlanError("epics must not be empty")
    epics: list[Epic] = []
    for index, entry in enumerate(entries):
        label = f"epics[{index}]"
        raw = expect_object(entry, label)
        require_exact_keys(
            raw,
            {
                "key",
                "title",
                "priority",
                "milestone",
                "description",
                "acceptance",
                "labels",
            },
            label,
        )
        stable_key = expect_string(raw["key"], f"{label}.key")
        if EPIC_KEY.fullmatch(stable_key) is None:
            raise PlanError(f"{label}.key is not an epic stable key")
        priority = expect_priority(raw["priority"], f"{label}.priority")
        milestone = expect_string(raw["milestone"], f"{label}.milestone")
        epic_match = EPIC_KEY.fullmatch(stable_key)
        if epic_match is None:
            raise PlanError(f"{label}.key is not an epic stable key")
        if milestone != f"M{epic_match.group(1)}":
            raise PlanError(f"{label}.milestone must correspond to its epic key")
        labels = expect_labels(raw["labels"], f"{label}.labels")
        for required in (
            "hxc-plan",
            f"epic-{stable_key.lower()}",
            f"milestone-{milestone.lower()}",
        ):
            if required not in labels:
                raise PlanError(f"{label}.labels is missing {required!r}")
        epics.append(
            Epic(
                stable_key,
                expect_string(raw["title"], f"{label}.title"),
                priority,
                milestone,
                expect_string(raw["description"], f"{label}.description"),
                expect_string_tuple(
                    raw["acceptance"], f"{label}.acceptance", nonempty=True
                ),
                labels,
            )
        )
    require_unique((epic.stable_key for epic in epics), "epic stable key")
    require_unique((epic.title for epic in epics), "epic title")
    expected_order = sorted(epics, key=lambda epic: stable_key_order(epic.stable_key))
    if epics != expected_order:
        raise PlanError("epics must be stored in canonical stable-key order")
    return tuple(epics)


def parse_tasks(value: object) -> tuple[Task, ...]:
    entries = expect_array(value, "tasks")
    if not entries:
        raise PlanError("tasks must not be empty")
    tasks: list[Task] = []
    for index, entry in enumerate(entries):
        label = f"tasks[{index}]"
        raw = expect_object(entry, label)
        require_exact_keys(
            raw,
            {
                "key",
                "epic",
                "title",
                "priority",
                "blockedBy",
                "prd",
                "requirements",
                "outcome",
                "acceptance",
                "labels",
                "effort",
                "type",
                "description",
            },
            label,
        )
        stable_key = expect_string(raw["key"], f"{label}.key")
        key_match = TASK_KEY.fullmatch(stable_key)
        if key_match is None:
            raise PlanError(f"{label}.key is not a task stable key")
        epic_key = expect_string(raw["epic"], f"{label}.epic")
        if epic_key != f"E{key_match.group(1)}":
            raise PlanError(f"{label}.epic does not own {stable_key}")
        effort = expect_string(raw["effort"], f"{label}.effort")
        if effort not in ALLOWED_EFFORTS:
            raise PlanError(f"{label}.effort is not recognized")
        issue_type = expect_string(raw["type"], f"{label}.type")
        if issue_type not in ALLOWED_ISSUE_TYPES:
            raise PlanError(f"{label}.type is not a supported Beads issue type")
        labels = expect_labels(raw["labels"], f"{label}.labels")
        for required in (
            "hxc-plan",
            f"epic-{epic_key.lower()}",
            f"effort-{effort}",
        ):
            if required not in labels:
                raise PlanError(f"{label}.labels is missing {required!r}")
        requirements = expect_string_tuple(
            raw["requirements"], f"{label}.requirements", nonempty=True
        )
        for requirement in requirements:
            if REQUIREMENT_ID.fullmatch(requirement) is None:
                raise PlanError(
                    f"{label}.requirements contains invalid ID {requirement!r}"
                )
        tasks.append(
            Task(
                stable_key,
                epic_key,
                expect_string(raw["title"], f"{label}.title"),
                expect_priority(raw["priority"], f"{label}.priority"),
                expect_stable_key_tuple(raw["blockedBy"], f"{label}.blockedBy"),
                expect_string_tuple(raw["prd"], f"{label}.prd", nonempty=True),
                requirements,
                expect_string(raw["outcome"], f"{label}.outcome"),
                expect_string_tuple(
                    raw["acceptance"], f"{label}.acceptance", nonempty=True
                ),
                labels,
                effort,
                issue_type,
                expect_string(raw["description"], f"{label}.description"),
            )
        )
    require_unique((task.stable_key for task in tasks), "task stable key")
    require_unique((task.title for task in tasks), "task title")
    expected_order = sorted(tasks, key=lambda task: stable_key_order(task.stable_key))
    if tasks != expected_order:
        raise PlanError("tasks must be stored in canonical stable-key order")
    return tuple(tasks)


def validate_graph(plan: BeadsPlan) -> None:
    epic_keys = set(plan.epic_by_key())
    task_keys = set(plan.task_by_key())
    for task in plan.tasks:
        if task.epic_key not in epic_keys:
            raise PlanError(
                f"{task.stable_key} references unknown epic {task.epic_key}"
            )
        for blocker in task.blocked_by:
            if blocker == task.stable_key:
                raise PlanError(f"{task.stable_key} cannot block itself")
            if blocker not in task_keys:
                raise PlanError(
                    f"{task.stable_key} references unknown blocker {blocker}"
                )


def expect_object(value: object, label: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise PlanError(f"{label} must be an object")
    return cast(dict[str, object], value)


def expect_array(value: object, label: str) -> list[object]:
    if not isinstance(value, list):
        raise PlanError(f"{label} must be an array")
    return cast(list[object], value)


def expect_string(value: object, label: str) -> str:
    if not isinstance(value, str) or not value.strip():
        raise PlanError(f"{label} must be a non-empty string")
    return value


def expect_int(value: object, label: str) -> int:
    if isinstance(value, bool) or not isinstance(value, int):
        raise PlanError(f"{label} must be an integer")
    return value


def expect_priority(value: object, label: str) -> int:
    priority = expect_int(value, label)
    if priority < 0 or priority > 4:
        raise PlanError(f"{label} must be in the range 0..4")
    return priority


def expect_string_tuple(
    value: object, label: str, *, nonempty: bool
) -> tuple[str, ...]:
    entries = expect_array(value, label)
    if nonempty and not entries:
        raise PlanError(f"{label} must not be empty")
    strings = tuple(
        expect_string(entry, f"{label}[{index}]")
        for index, entry in enumerate(entries)
    )
    require_unique(strings, label)
    return strings


def expect_labels(value: object, label: str) -> tuple[str, ...]:
    labels = expect_string_tuple(value, label, nonempty=True)
    for item in labels:
        if LABEL.fullmatch(item) is None:
            raise PlanError(f"{label} contains invalid label {item!r}")
    return labels


def expect_stable_key_tuple(value: object, label: str) -> tuple[str, ...]:
    keys = expect_string_tuple(value, label, nonempty=False)
    for stable_key in keys:
        if TASK_KEY.fullmatch(stable_key) is None:
            raise PlanError(f"{label} contains invalid task key {stable_key!r}")
    return keys


def require_exact_keys(
    value: dict[str, object], expected: set[str], label: str
) -> None:
    actual = set(value)
    missing = sorted(expected - actual)
    extra = sorted(actual - expected)
    if missing or extra:
        details: list[str] = []
        if missing:
            details.append("missing " + ", ".join(missing))
        if extra:
            details.append("unexpected " + ", ".join(extra))
        raise PlanError(f"{label} has invalid fields: {'; '.join(details)}")


def require_unique(values: Iterable[str], label: str) -> None:
    seen: set[str] = set()
    for value in values:
        if value in seen:
            raise PlanError(f"duplicate {label}: {value}")
        seen.add(value)
