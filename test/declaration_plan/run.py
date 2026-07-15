#!/usr/bin/env python3
"""Verify deterministic declaration planning, diagnostics, and header goldens."""

from __future__ import annotations

import difflib
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("declaration_plan.hxml")
EXPECTED_REPORT = Path(__file__).with_name("expected") / "plan.json"
EXPECTED_INCLUDE = Path(__file__).with_name("expected") / "include"
REPORT_PREFIX = "HXC_DECLARATION_PLAN="


class DeclarationPlanFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(label: str) -> tuple[str, dict[str, object]]:
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
        raise DeclarationPlanFailure(
            f"{label} failed with {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    lines = [line for line in result.stdout.splitlines() if line.startswith(REPORT_PREFIX)]
    if len(lines) != 1 or result.stderr:
        raise DeclarationPlanFailure(
            f"{label} emitted an invalid report envelope\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    payload = lines[0][len(REPORT_PREFIX) :]
    parsed = json.loads(payload)
    if not isinstance(parsed, dict):
        raise DeclarationPlanFailure(f"{label} report must be a JSON object")
    return payload, parsed


def difference(expected: str, actual: str, expected_name: str, actual_name: str) -> str:
    return "".join(
        difflib.unified_diff(
            expected.splitlines(keepends=True),
            actual.splitlines(keepends=True),
            fromfile=expected_name,
            tofile=actual_name,
        )
    )


def check_report(report: dict[str, object]) -> None:
    expected = json.loads(EXPECTED_REPORT.read_text(encoding="utf-8"))
    actual = {"plan": report.get("plan"), "diagnostics": report.get("diagnostics")}
    if actual != expected:
        raise DeclarationPlanFailure(
            "declaration plan snapshot drifted\nexpected:\n"
            + json.dumps(expected, indent=2, sort_keys=True)
            + "\nactual:\n"
            + json.dumps(actual, indent=2, sort_keys=True)
        )

    serialized = json.dumps(actual, sort_keys=True).lower()
    if "hxrt" in serialized or "hxc_runtime" in serialized:
        raise DeclarationPlanFailure("declaration planning selected a hidden runtime dependency")


def check_headers(report: dict[str, object]) -> None:
    raw_files = report.get("files")
    if not isinstance(raw_files, list):
        raise DeclarationPlanFailure("declaration plan report omitted its header files")

    actual_files: dict[str, str] = {}
    for item in raw_files:
        if not isinstance(item, dict) or not isinstance(item.get("path"), str) or not isinstance(item.get("content"), str):
            raise DeclarationPlanFailure(f"invalid header report entry: {item!r}")
        path = item["path"]
        if path in actual_files:
            raise DeclarationPlanFailure(f"duplicate planned header path: {path}")
        actual_files[path] = item["content"]

    expected_paths = sorted(
        path.relative_to(EXPECTED_INCLUDE).as_posix()
        for path in EXPECTED_INCLUDE.rglob("*.h")
    )
    if sorted(actual_files) != expected_paths:
        raise DeclarationPlanFailure(
            f"planned header set drifted: expected {expected_paths!r}, got {sorted(actual_files)!r}"
        )

    for relative in expected_paths:
        expected = (EXPECTED_INCLUDE / relative).read_text(encoding="utf-8")
        actual = actual_files[relative]
        if actual != expected:
            raise DeclarationPlanFailure(
                f"planned header {relative} drifted:\n"
                + difference(expected, actual, f"expected/include/{relative}", f"actual/include/{relative}")
            )
        lowered = actual.lower()
        if "hxrt" in lowered or "hxc_runtime" in lowered:
            raise DeclarationPlanFailure(f"planned header {relative} unexpectedly selected hxrt")


def main() -> int:
    if shutil.which(development_tool("haxe")) is None:
        print("declaration-plan: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    try:
        first_payload, first = render("first declaration plan render")
        second_payload, _ = render("second declaration plan render")
        if first_payload != second_payload:
            raise DeclarationPlanFailure("two declaration plan renders were not byte-identical")
        check_report(first)
        check_headers(first)
    except (DeclarationPlanFailure, OSError, UnicodeError, json.JSONDecodeError, subprocess.TimeoutExpired) as error:
        print(f"declaration-plan: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "declaration-plan: OK: stable include/forward/complete placement, actionable failures, "
        "portable guards, and runtime-free header goldens"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
