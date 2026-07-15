#!/usr/bin/env python3
"""Render the structural C AST twice and compare it with the checked-in golden."""

from __future__ import annotations

import difflib
import os
import shutil
import subprocess
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
HXML = Path(__file__).with_name("c_ast.hxml")
EXPECTED = Path(__file__).with_name("expected") / "declarators.c"


class CASTGoldenFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(label: str) -> str:
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
        raise CASTGoldenFailure(
            f"{label} returned {result.returncode}\n"
            f"stdout:\n{result.stdout}\nstderr:\n{result.stderr}"
        )
    return result.stdout


def main() -> int:
    if shutil.which(development_tool("haxe")) is None:
        print("c-ast-golden: ERROR: pinned Haxe executable is unavailable", file=sys.stderr)
        return 1
    if not EXPECTED.is_file():
        print(f"c-ast-golden: ERROR: missing golden: {EXPECTED}", file=sys.stderr)
        return 1

    try:
        first = render("first structural AST render")
        second = render("second structural AST render")
        if first != second:
            raise CASTGoldenFailure("two renders in one checkout were not byte-identical")
        if "hxrt" in first or "hxc_runtime" in first:
            raise CASTGoldenFailure("declarator-only golden unexpectedly selected runtime code")
        expected = EXPECTED.read_text(encoding="utf-8")
        if first != expected:
            difference = "".join(
                difflib.unified_diff(
                    expected.splitlines(keepends=True),
                    first.splitlines(keepends=True),
                    fromfile="expected/declarators.c",
                    tofile="actual/declarators.c",
                )
            )
            raise CASTGoldenFailure("structural printer output drifted:\n" + difference)
    except (CASTGoldenFailure, OSError, UnicodeError, subprocess.TimeoutExpired) as error:
        print(f"c-ast-golden: ERROR: {error}", file=sys.stderr)
        return 1

    print("c-ast-golden: OK: deterministic structural C11 declarator corpus matched")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
