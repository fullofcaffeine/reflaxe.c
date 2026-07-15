#!/usr/bin/env python3
"""Render every structural C AST corpus twice and compare checked-in goldens."""

from __future__ import annotations

import difflib
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]

@dataclass(frozen=True)
class GoldenCase:
    label: str
    hxml: Path
    expected: Path


CASES = (
    GoldenCase(
        "declarator",
        Path(__file__).with_name("c_ast.hxml"),
        Path(__file__).with_name("expected") / "declarators.c",
    ),
    GoldenCase(
        "expression/statement",
        Path(__file__).with_name("expression.hxml"),
        Path(__file__).with_name("expected") / "expressions.c",
    ),
)


class CASTGoldenFailure(RuntimeError):
    pass


def development_tool(name: str) -> str:
    local = ROOT / "node_modules/.bin" / name
    return str(local) if local.is_file() else name


def render(case: GoldenCase, label: str) -> str:
    environment = os.environ.copy()
    environment["HAXE_NO_SERVER"] = "1"
    result = subprocess.run(
        [development_tool("haxe"), str(case.hxml)],
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
    missing = [case.expected for case in CASES if not case.expected.is_file()]
    if missing:
        print(
            "c-ast-golden: ERROR: missing goldens: "
            + ", ".join(str(path) for path in missing),
            file=sys.stderr,
        )
        return 1

    try:
        for case in CASES:
            first = render(case, f"first {case.label} AST render")
            second = render(case, f"second {case.label} AST render")
            if first != second:
                raise CASTGoldenFailure(
                    f"two {case.label} renders were not byte-identical"
                )
            if "hxrt" in first or "hxc_runtime" in first:
                raise CASTGoldenFailure(
                    f"{case.label} golden unexpectedly selected runtime code"
                )
            expected = case.expected.read_text(encoding="utf-8")
            if first != expected:
                difference = "".join(
                    difflib.unified_diff(
                        expected.splitlines(keepends=True),
                        first.splitlines(keepends=True),
                        fromfile=f"expected/{case.expected.name}",
                        tofile=f"actual/{case.expected.name}",
                    )
                )
                raise CASTGoldenFailure(
                    f"{case.label} printer output drifted:\n" + difference
                )
    except (CASTGoldenFailure, OSError, UnicodeError, subprocess.TimeoutExpired) as error:
        print(f"c-ast-golden: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "c-ast-golden: OK: deterministic declarator and expression/statement "
        "C11 corpora matched without runtime selection"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
