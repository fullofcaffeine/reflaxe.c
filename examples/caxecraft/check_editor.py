#!/usr/bin/env python3
"""Check renderer-independent CaxeMap editing under two process locales."""

from __future__ import annotations

import re
import subprocess
import sys
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
EDITOR = CASE / "src/caxecraft/editor"
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
    "caxemap-editor: 17 command round trips, 1038 canonical bytes; "
    "bounded history/test-play/recovery; trace=530444322\n"
)
FORBIDDEN_EDITOR_TEXT = (
    re.compile(r"#if\b"),
    re.compile(r"\bDynamic\b"),
    re.compile(r"\bAny\b"),
    re.compile(r"\bReflect\b"),
    re.compile(r"\buntyped\b"),
    re.compile(r"\b__c__\b"),
    # These broad spellings also catch fully qualified types, `using`
    # declarations, and metadata such as `@:c.name`, not only imports.
    re.compile(r"\bc\."),
    re.compile(r"\braylib\."),
)


class EditorFailure(RuntimeError):
    pass


def check_target_neutral_boundary() -> None:
    """Keep UI and target adapters out of the reusable editor semantics."""

    sources = sorted(EDITOR.glob("*.hx"), key=lambda path: path.name.encode("utf-8"))
    if not sources:
        raise EditorFailure("editor source inventory is empty")
    for source in sources:
        text = source.read_text(encoding="utf-8")
        for pattern in FORBIDDEN_EDITOR_TEXT:
            if pattern.search(text):
                raise EditorFailure(
                    f"{source.relative_to(ROOT)} crosses the target-neutral editor "
                    f"boundary: {pattern.pattern}"
                )


def run_probe(locale: str) -> str:
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("editor.hxml",), locale=locale)
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
        raise EditorFailure(
            f"{locale} editor probe changed:\n"
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
            raise EditorFailure("no alternate locale is installed for the editor lane")
        baseline = run_probe("C")
        localized = run_probe(locale)
        if baseline != localized:
            raise EditorFailure("C and alternate-locale editor traces did not converge")
    except (
        CaxecraftFailure,
        EditorFailure,
        OSError,
        subprocess.TimeoutExpired,
        UnicodeError,
    ) as error:
        print(f"caxemap-editor: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "caxemap-editor: OK: public commands, exact undo/redo, validation, "
        f"bounded history, and reversible test play agree under C/{locale}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
