#!/usr/bin/env python3
"""Check Caxecraft's target-neutral friendly and hostile actor rules."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
GAMEPLAY = CASE / "src/caxecraft/gameplay"
sys.path.insert(0, str(CASE))

from run import (  # noqa: E402
    CaxecraftFailure,
    alternate_locale,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)

EXPECTED = "caxecraft-gameplay: Nia gift, Mossling combat/drop, berry recovery, and bounded player health passed\n"
FORBIDDEN = tuple(
    re.compile(pattern)
    for pattern in (r"#if\b", r"\bDynamic\b", r"\bAny\b", r"\bReflect\b", r"\buntyped\b", r"\b__c__\b", r"\bc\.", r"\braylib\.")
)


class GameplayFailure(RuntimeError):
    pass


def check_boundary() -> None:
    sources = sorted(GAMEPLAY.glob("*.hx"), key=lambda path: path.name.encode("utf-8"))
    if not sources:
        raise GameplayFailure("gameplay source inventory is empty")
    for source in sources:
        text = source.read_text(encoding="utf-8")
        for pattern in FORBIDDEN:
            if pattern.search(text):
                raise GameplayFailure(
                    f"{source.relative_to(ROOT)} crosses the target-neutral gameplay boundary: {pattern.pattern}"
                )


def run_probe(locale: str) -> str:
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("gameplay.hxml",), locale=locale)
    result = subprocess.run(
        [str(installation.compiler), *arguments],
        cwd=CASE,
        env=pinned_haxe_environment(locale, installation),
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        timeout=30,
    )
    if result.returncode != 0 or result.stdout != EXPECTED or result.stderr:
        raise GameplayFailure(
            f"{locale} gameplay probe changed:\nexit: {result.returncode}\nstdout: {result.stdout!r}\nstderr: {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        check_boundary()
        locale = alternate_locale()
        if locale == "C":
            raise GameplayFailure("no alternate locale is installed for the gameplay lane")
        if run_probe("C") != run_probe(locale):
            raise GameplayFailure("C and alternate-locale gameplay traces did not converge")
    except (CaxecraftFailure, GameplayFailure, OSError, subprocess.TimeoutExpired, UnicodeError) as error:
        print(f"caxecraft-gameplay: ERROR: {error}", file=sys.stderr)
        return 1
    print(f"caxecraft-gameplay: OK: target-neutral actor, combat, drop, recovery, and health state under C/{locale}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
