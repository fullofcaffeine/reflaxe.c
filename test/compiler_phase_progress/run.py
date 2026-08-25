#!/usr/bin/env python3
"""Verify deterministic, lightweight compiler progress records."""

from __future__ import annotations

import json
import os
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.test.bounded_process import run as run_bounded_process  # noqa: E402

HXML = Path(__file__).with_name("compiler_phase_progress.hxml")
PREFIX = "HXC_PHASE_PROGRESS\t"


class CompilerPhaseProgressFailure(RuntimeError):
    """Report one focused progress-contract failure."""


def record(
    state: str,
    *,
    phase: str | None = None,
    profile: str | None = None,
    build_mode: str | None = None,
    status: str | None = None,
) -> dict[str, object]:
    """Build one expected schema-1 marker with every bounded field present."""

    return {
        "schemaVersion": 1,
        "state": state,
        "phase": phase,
        "profile": profile,
        "buildMode": build_mode,
        "status": status,
    }


def main() -> int:
    """Compile the macro probe and require its exact ordered record stream."""

    haxe = ROOT / "node_modules/.bin/haxe"
    command = [str(haxe) if haxe.is_file() else "haxe", str(HXML)]
    result = run_bounded_process(
        command,
        phase="compiler phase-progress probe",
        cwd=ROOT,
        env={**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C"},
        check=False,
        capture_output=True,
        text=True,
        timeout=30,
    )
    if result.returncode != 0:
        raise CompilerPhaseProgressFailure(
            f"progress probe failed with {result.returncode}\nstdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    if result.stderr:
        raise CompilerPhaseProgressFailure(f"progress probe wrote unexpected stderr:\n{result.stderr}")
    lines = result.stdout.splitlines()
    if any(not line.startswith(PREFIX) for line in lines):
        raise CompilerPhaseProgressFailure(f"progress probe wrote an unknown stdout record:\n{result.stdout}")
    actual = [json.loads(line[len(PREFIX) :]) for line in lines]
    expected = [
        record("request-started"),
        record("phase-started", phase="typed input capture"),
        record("phase-completed", phase="typed input capture"),
        record("phase-started", phase="target pipeline"),
        record("request-configured", profile="portable", build_mode="debug"),
        record("phase-started", phase="configuration and contracts"),
        record("phase-completed", phase="configuration and contracts"),
        record("phase-completed", phase="target pipeline"),
        record("request-completed", status="ok"),
        record("request-started"),
        record("phase-started", phase="target pipeline"),
        record("request-aborted", status="expected-failure"),
    ]
    if actual != expected:
        raise CompilerPhaseProgressFailure(
            "progress records changed\n"
            + "expected:\n"
            + json.dumps(expected, indent=2, sort_keys=True)
            + "\nactual:\n"
            + json.dumps(actual, indent=2, sort_keys=True)
        )
    print("compiler-phase-progress: PASS")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except (CompilerPhaseProgressFailure, OSError, subprocess.TimeoutExpired, json.JSONDecodeError) as error:
        print(f"compiler-phase-progress: ERROR: {error}", file=sys.stderr)
        raise SystemExit(1)
