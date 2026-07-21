#!/usr/bin/env python3
"""Check the target-neutral named Caxecraft game-pilot foundation."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
PILOT = CASE / "src/caxecraft/pilot"
sys.path.insert(0, str(CASE))

from run import (  # noqa: E402
    CaxecraftFailure,
    alternate_locale,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)

EXPECTED_TRACE = (
    "caxecraft-pilot: 6 named scripts, 35 deterministic frames, 8 checkpoints; "
    "bounded quit and shared input interface\n"
)
FORBIDDEN_PILOT_TEXT = (
    re.compile(r"#if\b"),
    re.compile(r"\bDynamic\b"),
    re.compile(r"\bAny\b"),
    re.compile(r"\bReflect\b"),
    re.compile(r"\buntyped\b"),
    re.compile(r"\b__c__\b"),
    re.compile(r"\bArray\b"),
    re.compile(r"\bc\."),
    re.compile(r"\braylib\."),
)


class PilotFailure(RuntimeError):
    pass


def check_target_neutral_boundary() -> None:
    sources = sorted(PILOT.glob("*.hx"), key=lambda path: path.name.encode("utf-8"))
    if not sources:
        raise PilotFailure("pilot source inventory is empty")
    for source in sources:
        text = source.read_text(encoding="utf-8")
        for pattern in FORBIDDEN_PILOT_TEXT:
            if pattern.search(text):
                raise PilotFailure(
                    f"{source.relative_to(ROOT)} crosses the target-neutral pilot "
                    f"boundary: {pattern.pattern}"
                )


def run_probe(locale: str) -> str:
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("pilot.hxml",), locale=locale)
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
    if result.returncode != 0 or result.stdout != EXPECTED_TRACE or result.stderr:
        raise PilotFailure(
            f"{locale} pilot probe changed:\n"
            f"exit:            {result.returncode}\n"
            f"expected stdout: {EXPECTED_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        check_target_neutral_boundary()
        locale = alternate_locale()
        if locale == "C":
            raise PilotFailure("no alternate locale is installed for the pilot lane")
        baseline = run_probe("C")
        localized = run_probe(locale)
        if baseline != localized:
            raise PilotFailure("C and alternate-locale pilot traces did not converge")
    except (
        CaxecraftFailure,
        PilotFailure,
        OSError,
        subprocess.TimeoutExpired,
        UnicodeError,
    ) as error:
        print(f"caxecraft-pilot: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "caxecraft-pilot: OK: named semantic input, exact checkpoints, "
        f"bounded quit, and C/{locale} determinism"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
