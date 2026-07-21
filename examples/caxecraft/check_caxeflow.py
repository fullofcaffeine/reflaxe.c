#!/usr/bin/env python3
"""Check deterministic CaxeFlow execution under two process locales."""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path

CASE = Path(__file__).resolve().parent
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
    "caxeflow: 10 events, 12 predicates, 18 actions; "
    "stable order/repeat/defer/sequence/budgets; trace=2142133802\n"
)


class CaxeFlowFailure(RuntimeError):
    pass


def run_probe(locale: str) -> str:
    """Run the complete probe with an explicit locale and exact output."""

    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("caxeflow.hxml",), locale=locale)
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
    if (
        result.returncode != 0
        or result.stdout != EXPECTED_TRACE
        or result.stderr
    ):
        raise CaxeFlowFailure(
            f"{locale} CaxeFlow probe changed:\n"
            f"exit:            {result.returncode}\n"
            f"expected stdout: {EXPECTED_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        locale = alternate_locale()
        if locale == "C":
            raise CaxeFlowFailure(
                "no alternate locale is installed for the CaxeFlow reference lane"
            )
        baseline = run_probe("C")
        localized = run_probe(locale)
        if baseline != localized:
            raise CaxeFlowFailure("C and alternate-locale traces did not converge")
    except (
        CaxecraftFailure,
        CaxeFlowFailure,
        OSError,
        subprocess.TimeoutExpired,
        UnicodeError,
    ) as error:
        print(f"caxeflow: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "caxeflow: OK: closed fixed-tick execution, reverse registration, "
        f"C/{locale} locale determinism, and exact runtime budgets"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
