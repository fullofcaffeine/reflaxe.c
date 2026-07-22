#!/usr/bin/env python3
"""Run the Haxe-owned CAXEMAP level generator and check or write its bytes."""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path

from run import (
    CaxecraftFailure,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)


CASE = Path(__file__).resolve().parent
OUTPUT = CASE / "src/caxecraft/content/FirstPlayableLevel.hx"


class LevelAdapterFailure(RuntimeError):
    """The Haxe-owned generator could not prove one current typed adapter."""


def render() -> str:
    """Ask Haxe to parse, validate, resolve, and render the complete adapter."""

    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    result = subprocess.run(
        [str(installation.compiler), *resolve_haxe_arguments(("level-adapter.hxml",), locale="C")],
        cwd=CASE,
        env=pinned_haxe_environment("C", installation),
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        timeout=30,
    )
    if result.returncode != 0 or result.stderr:
        raise LevelAdapterFailure(
            "Haxe level generator failed\n"
            f"exit: {result.returncode}\nstdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    if not result.stdout.endswith("\n") or "function loadTerrain" not in result.stdout:
        raise LevelAdapterFailure("Haxe level generator emitted an invalid result envelope")
    return result.stdout


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail when the checked-in adapter is stale")
    arguments = parser.parse_args()
    try:
        rendered = render()
        if arguments.check:
            if not OUTPUT.is_file() or OUTPUT.read_text(encoding="utf-8") != rendered:
                raise LevelAdapterFailure(
                    "generated adapter is stale; run python3 examples/caxecraft/level_adapter.py"
                )
        else:
            OUTPUT.parent.mkdir(parents=True, exist_ok=True)
            OUTPUT.write_text(rendered, encoding="utf-8", newline="\n")
    except (CaxecraftFailure, LevelAdapterFailure, OSError, subprocess.TimeoutExpired, UnicodeError) as error:
        print(f"caxecraft-level-adapter: ERROR: {error}", file=sys.stderr)
        return 1
    action = "current" if arguments.check else "generated"
    print(f"caxecraft-level-adapter: OK: Haxe-validated first-playable adapter {action}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
