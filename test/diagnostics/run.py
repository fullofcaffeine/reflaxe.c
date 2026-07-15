#!/usr/bin/env python3
"""Exercise the typed registry and deterministic structured diagnostic record."""

from __future__ import annotations

import json
import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("diagnostics.hxml")
DEFINITIONS_PREFIX = "HXC_DIAGNOSTIC_DEFINITIONS="
EVENTS_PREFIX = "HXC_DIAGNOSTIC_EVENTS="
HUMAN_PREFIX = "HXC_DIAGNOSTIC_HUMAN="
SENTINEL = "diagnostics-haxe: OK"

sys.path.insert(0, str(ROOT / "scripts/ci"))
import check_diagnostic_policy as policy  # noqa: E402


class DiagnosticFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def run_haxe() -> str:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(HXML)],
        cwd=ROOT,
        env=environment,
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise DiagnosticFailure(
            f"typed diagnostic probe failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result.stdout


def prefixed_json(lines: list[str], prefix: str) -> object:
    matches = [line.removeprefix(prefix) for line in lines if line.startswith(prefix)]
    if len(matches) != 1:
        raise DiagnosticFailure(f"expected exactly one {prefix} record, found {len(matches)}")
    try:
        return json.loads(matches[0])
    except json.JSONDecodeError as error:
        raise DiagnosticFailure(f"invalid JSON after {prefix}: {error}") from error


def main() -> int:
    if shutil.which(development_tool("haxe")) is None:
        print("diagnostics: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        policy_errors = policy.validate(ROOT)
        if policy_errors:
            raise DiagnosticFailure("diagnostic policy failed:\n" + "\n".join(f"- {error}" for error in policy_errors))

        first = run_haxe()
        second = run_haxe()
        if first.encode("utf-8") != second.encode("utf-8"):
            raise DiagnosticFailure("two typed diagnostic renders were not byte-identical")
        lines = first.splitlines()
        if lines[-1:] != [SENTINEL]:
            raise DiagnosticFailure(f"typed diagnostic probe missed its sentinel\n{first}")

        catalog = json.loads((ROOT / policy.CATALOG_RELATIVE).read_text(encoding="utf-8"))
        definitions = prefixed_json(lines, DEFINITIONS_PREFIX)
        if definitions != catalog["diagnostics"]:
            raise DiagnosticFailure("typed Haxe definitions differ from docs/specs/diagnostics.json")

        events = prefixed_json(lines, EVENTS_PREFIX)
        if not isinstance(events, list) or len(events) != 3:
            raise DiagnosticFailure("typed probe must emit exactly three structured records")
        event_errors: list[str] = []
        for index, event in enumerate(events):
            if not isinstance(event, dict):
                event_errors.append(f"event {index} is not an object")
                continue
            event_errors.extend(f"event {index}: {error}" for error in policy.validate_event(event, catalog))
        if event_errors:
            raise DiagnosticFailure("structured event validation failed:\n" + "\n".join(f"- {error}" for error in event_errors))

        ids = [event["id"] for event in events]
        if ids != ["HXC1001", "HXC9000", "HXC2001"]:
            raise DiagnosticFailure(f"typed diagnostic IDs drifted: {ids}")
        if events[0]["kind"] != "unsupported-source" or events[1]["kind"] != "internal-compiler-failure":
            raise DiagnosticFailure("unsupported-source and internal failures are not structurally distinct")
        if events[2]["severity"] != "warning":
            raise DiagnosticFailure("HXC2001 policy escalation was not retained in structured output")

        human = [line.removeprefix(HUMAN_PREFIX) for line in lines if line.startswith(HUMAN_PREFIX)]
        if len(human) != 1 or "HXC1001" not in human[0] or "[profile=portable]" not in human[0] or "Remediation:" not in human[0]:
            raise DiagnosticFailure("human output lost its stable ID, profile, or remediation contract")
    except (DiagnosticFailure, OSError, subprocess.TimeoutExpired) as error:
        print(f"diagnostics: ERROR: {error}", file=sys.stderr)
        return 1

    print("diagnostics: OK: 12 typed IDs, deterministic schema-1 records, severity/source guards, and registry drift checks passed")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
