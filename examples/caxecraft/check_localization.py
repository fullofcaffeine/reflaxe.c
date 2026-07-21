#!/usr/bin/env python3
"""Check catalog completeness, generated adapters, and locale-blind app code."""

from __future__ import annotations

import copy
import json
import sys
import tempfile
from pathlib import Path

import localization_catalog as catalog
from play import PlayFailure, stage_content_catalogs


CASE = Path(__file__).resolve().parent
APP_SOURCES = (
    CASE / "src/caxecraft/app/Main.hx",
    CASE / "src/caxecraft/app/TitleMenu.hx",
)


class LocalizationCheckFailure(RuntimeError):
    pass


def expect_rejected(document: dict[str, object], label: str) -> None:
    try:
        catalog.validate_document(document, expected_id="caxecraft.ui")
    except catalog.CatalogFailure:
        return
    raise LocalizationCheckFailure(f"malformed catalog was accepted: {label}")


def check_negative_contracts() -> None:
    source = json.loads(catalog.UI_SOURCE.read_text(encoding="utf-8"))

    missing_translation = copy.deepcopy(source)
    del missing_translation["messages"][0]["text"]["es-MX"]
    expect_rejected(missing_translation, "missing translation")

    duplicate_locale = copy.deepcopy(source)
    duplicate_locale["locales"][1] = "en"
    expect_rejected(duplicate_locale, "duplicate locale")

    duplicate_message = copy.deepcopy(source)
    duplicate_message["messages"][1]["id"] = duplicate_message["messages"][0]["id"]
    expect_rejected(duplicate_message, "duplicate message ID")

    control_character = copy.deepcopy(source)
    control_character["messages"][0]["text"]["en"] = "bad\u0000text"
    expect_rejected(control_character, "embedded NUL")

    unknown_key = copy.deepcopy(source)
    unknown_key["fallback"] = "en"
    expect_rejected(unknown_key, "unknown root key")

    try:
        catalog.decode_document('{"schemaVersion":1,"schemaVersion":1}')
    except catalog.CatalogFailure:
        pass
    else:
        raise LocalizationCheckFailure("duplicate JSON object key was accepted")


def check_app_boundary() -> None:
    forbidden = ("GameLanguage", "language ==", "GameLanguage.", 'Raylib.DrawText("')
    for source in APP_SOURCES:
        text = source.read_text(encoding="utf-8")
        found = [fragment for fragment in forbidden if fragment in text]
        if found:
            raise LocalizationCheckFailure(
                f"{source.relative_to(CASE)} still owns locale or prose details: {found}"
            )


def check_generated() -> None:
    rendered = catalog.rendered_catalogs()
    for path, expected in rendered.items():
        if not path.is_file() or path.read_text(encoding="utf-8") != expected:
            raise LocalizationCheckFailure(f"generated adapter is stale: {path.relative_to(CASE)}")
        if expected.count("Raylib.DrawText(") == 0:
            raise LocalizationCheckFailure(f"generated adapter has no direct literal draws: {path.relative_to(CASE)}")


def check_native_package_boundary() -> None:
    with tempfile.TemporaryDirectory(prefix="caxecraft-localization-package-") as temporary:
        destination = Path(temporary)
        stage_content_catalogs(destination)
        for relative in ("locales/ui.json", "scenarios/first-playable/messages.json"):
            source = CASE / relative
            packaged = destination / "content" / relative
            if not packaged.is_file() or packaged.read_bytes() != source.read_bytes():
                raise LocalizationCheckFailure(f"native package lost exact catalog bytes: {relative}")


def main() -> int:
    try:
        check_negative_contracts()
        check_generated()
        check_app_boundary()
        check_native_package_boundary()
    except (OSError, UnicodeError, PlayFailure, catalog.CatalogFailure, LocalizationCheckFailure) as error:
        print(f"caxecraft-localization: ERROR: {error}", file=sys.stderr)
        return 1
    print("caxecraft-localization: OK: complete catalogs, direct C literals, locale-blind app")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
