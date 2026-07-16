#!/usr/bin/env python3
"""Validate the Beads bootstrap schema and dependency graph without `bd`."""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.beads.hxc_beads_plan import BeadsPlan, PlanError, load_plan


DEFAULT_PLAN = ROOT / "docs/specs/beads-plan.json"
DEFAULT_SCHEMA = ROOT / "docs/specs/beads-plan.schema.json"
JSON_SCHEMA_DIALECT = "https://json-schema.org/draft/2020-12/schema"


def validate_schema_document(path: Path) -> None:
    try:
        decoded: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise PlanError(f"cannot read plan schema {path}: {error}") from error
    if not isinstance(decoded, dict):
        raise PlanError("plan schema must contain a JSON object")
    if decoded.get("$schema") != JSON_SCHEMA_DIALECT:
        raise PlanError("plan schema must use JSON Schema 2020-12")
    if decoded.get("$id") != "https://reflaxe-c.dev/schemas/beads-plan.schema.json":
        raise PlanError("plan schema has an unexpected canonical ID")


def summary(plan: BeadsPlan) -> dict[str, object]:
    return {
        "schemaVersion": plan.schema_version,
        "planVersion": plan.plan_version,
        "project": plan.project.name,
        "counts": {
            "epics": len(plan.epics),
            "tasks": len(plan.tasks),
            "blockingEdges": len(plan.blocking_edges()),
            "coveredRequirementIds": len(plan.unique_requirement_ids()),
        },
        "initialReady": list(plan.initial_ready()),
        "topologicalOrder": list(plan.topological_order()),
    }


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(
        description="Validate the canonical Beads bootstrap without changing Beads."
    )
    result.add_argument("--plan", type=Path, default=DEFAULT_PLAN)
    result.add_argument("--schema", type=Path, default=DEFAULT_SCHEMA)
    result.add_argument("--json", action="store_true", dest="json_output")
    return result


def main(arguments: list[str] | None = None) -> int:
    options = parser().parse_args(arguments)
    try:
        validate_schema_document(options.schema.resolve())
        plan = load_plan(options.plan.resolve())
    except PlanError as error:
        print(f"beads-plan validation failed: {error}", file=sys.stderr)
        return 1

    payload = summary(plan)
    if options.json_output:
        print(json.dumps(payload, indent=2, ensure_ascii=False, sort_keys=True))
    else:
        counts = payload["counts"]
        assert isinstance(counts, dict)
        print(
            "beads-plan: valid "
            f"({counts['epics']} epics, {counts['tasks']} tasks, "
            f"{counts['blockingEdges']} blocking edges, "
            f"{counts['coveredRequirementIds']} requirement IDs)"
        )
        print("initial-ready: " + ", ".join(plan.initial_ready()))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
