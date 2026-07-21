#!/usr/bin/env python3
"""Validate Caxecraft catalogs and generate the narrow C rendering adapters.

The JSON files are the human-edited source of truth. Generated Haxe keeps
locale selection and C string-lifetime details out of gameplay and UI code.
"""

from __future__ import annotations

import argparse
import json
import re
from dataclasses import dataclass
from pathlib import Path


CASE = Path(__file__).resolve().parent
UI_SOURCE = CASE / "locales/ui.json"
SCENARIO_SOURCE = CASE / "scenarios/first-playable/messages.json"
UI_OUTPUT = CASE / "src/caxecraft/localization/UiCatalog.hx"
SCENARIO_OUTPUT = CASE / "src/caxecraft/localization/FirstPlayableCatalog.hx"
CATALOG_KEYS = {"schemaVersion", "catalogId", "defaultLocale", "locales", "messages"}
MESSAGE_KEYS = {"id", "symbol", "text"}
LOCALE_PATTERN = re.compile(r"^[a-z]{2}(?:-[A-Z]{2})?$")
MESSAGE_PATTERN = re.compile(r"^[a-z][a-z0-9_]{0,63}$")
SYMBOL_PATTERN = re.compile(r"^[A-Z][A-Za-z0-9]{0,63}$")


class CatalogFailure(ValueError):
    """A catalog cannot be trusted as deterministic player-visible content."""


@dataclass(frozen=True)
class Message:
    message_id: str
    symbol: str
    text: tuple[str, ...]


@dataclass(frozen=True)
class Catalog:
    catalog_id: str
    default_locale: str
    locales: tuple[str, ...]
    messages: tuple[Message, ...]


def reject_duplicate_keys(pairs: list[tuple[str, object]]) -> dict[str, object]:
    value: dict[str, object] = {}
    for key, item in pairs:
        if key in value:
            raise CatalogFailure(f"duplicate JSON key {key!r}")
        value[key] = item
    return value


def decode_document(text: str) -> dict[str, object]:
    try:
        value = json.loads(text, object_pairs_hook=reject_duplicate_keys)
    except json.JSONDecodeError as error:
        raise CatalogFailure(f"invalid JSON: {error.msg} at line {error.lineno}") from error
    if not isinstance(value, dict):
        raise CatalogFailure("catalog root must be an object")
    return value


def validate_text(value: object, *, coordinate: str) -> str:
    if not isinstance(value, str) or not value:
        raise CatalogFailure(f"{coordinate} must be a non-empty string")
    if len(value.encode("utf-8")) > 240:
        raise CatalogFailure(f"{coordinate} exceeds the 240-byte display bound")
    for character in value:
        codepoint = ord(character)
        if codepoint == 0 or codepoint < 32 or 0xD800 <= codepoint <= 0xDFFF:
            raise CatalogFailure(f"{coordinate} contains an unsupported control or Unicode surrogate")
    return value


def validate_document(document: dict[str, object], *, expected_id: str) -> Catalog:
    if set(document) != CATALOG_KEYS:
        unknown = sorted(set(document) - CATALOG_KEYS)
        missing = sorted(CATALOG_KEYS - set(document))
        raise CatalogFailure(f"catalog keys differ; missing={missing}, unknown={unknown}")
    if document["schemaVersion"] != 1:
        raise CatalogFailure("only catalog schemaVersion 1 is supported")
    if document["catalogId"] != expected_id:
        raise CatalogFailure(f"catalogId must be {expected_id!r}")
    default_locale = document["defaultLocale"]
    raw_locales = document["locales"]
    raw_messages = document["messages"]
    if not isinstance(default_locale, str):
        raise CatalogFailure("defaultLocale must be a string")
    if not isinstance(raw_locales, list) or not 1 <= len(raw_locales) <= 8:
        raise CatalogFailure("locales must contain between one and eight locale IDs")
    locales: list[str] = []
    for raw_locale in raw_locales:
        if not isinstance(raw_locale, str) or not LOCALE_PATTERN.fullmatch(raw_locale):
            raise CatalogFailure(f"invalid locale ID {raw_locale!r}")
        if raw_locale in locales:
            raise CatalogFailure(f"duplicate locale ID {raw_locale!r}")
        locales.append(raw_locale)
    if default_locale != locales[0]:
        raise CatalogFailure("defaultLocale must be the first locale for deterministic startup")
    if not isinstance(raw_messages, list) or not raw_messages:
        raise CatalogFailure("messages must be a non-empty array")
    messages: list[Message] = []
    seen_ids: set[str] = set()
    seen_symbols: set[str] = set()
    for index, raw_message in enumerate(raw_messages):
        coordinate = f"messages[{index}]"
        if not isinstance(raw_message, dict) or set(raw_message) != MESSAGE_KEYS:
            raise CatalogFailure(f"{coordinate} must contain exactly id, symbol, and text")
        message_id = raw_message["id"]
        symbol = raw_message["symbol"]
        raw_text = raw_message["text"]
        if not isinstance(message_id, str) or not MESSAGE_PATTERN.fullmatch(message_id):
            raise CatalogFailure(f"{coordinate}.id is invalid")
        if not isinstance(symbol, str) or not SYMBOL_PATTERN.fullmatch(symbol):
            raise CatalogFailure(f"{coordinate}.symbol is not a Haxe constructor name")
        if message_id in seen_ids or symbol in seen_symbols:
            raise CatalogFailure(f"duplicate message ID or symbol at {coordinate}")
        if not isinstance(raw_text, dict) or set(raw_text) != set(locales):
            raise CatalogFailure(f"{coordinate}.text must contain exactly {locales}")
        seen_ids.add(message_id)
        seen_symbols.add(symbol)
        messages.append(
            Message(
                message_id,
                symbol,
                tuple(validate_text(raw_text[locale], coordinate=f"{coordinate}.text.{locale}") for locale in locales),
            )
        )
    ids = [message.message_id for message in messages]
    if ids != sorted(ids, key=lambda value: value.encode("utf-8")):
        raise CatalogFailure("messages must be ordered by UTF-8 message ID")
    return Catalog(expected_id, default_locale, tuple(locales), tuple(messages))


def load_catalog(path: Path, *, expected_id: str) -> Catalog:
    try:
        raw = path.read_bytes()
    except OSError as error:
        raise CatalogFailure(f"cannot read {path.relative_to(CASE)}: {error}") from error
    if raw.startswith(b"\xef\xbb\xbf") or b"\r" in raw or not raw.endswith(b"\n"):
        raise CatalogFailure(f"{path.relative_to(CASE)} must use UTF-8 without BOM and canonical LF")
    try:
        text = raw.decode("utf-8")
    except UnicodeDecodeError as error:
        raise CatalogFailure(f"{path.relative_to(CASE)} is not valid UTF-8") from error
    return validate_document(decode_document(text), expected_id=expected_id)


def haxe_string(value: str) -> str:
    escaped = value.replace("\\", "\\\\").replace('"', '\\"')
    return f'"{escaped}"'


def enum_abstract(name: str, symbols: tuple[str, ...]) -> list[str]:
    lines = [f"enum abstract {name}(Int) {{"]
    for index, symbol in enumerate(symbols):
        lines.append(f"\tvar {symbol} = {index};")
    lines.append("}")
    return lines


def render_catalog(catalog: Catalog, *, scenario: bool) -> str:
    class_name = "FirstPlayableCatalog" if scenario else "UiCatalog"
    message_type = "ScenarioMessage" if scenario else "UiMessage"
    source_name = "scenarios/first-playable/messages.json" if scenario else "locales/ui.json"
    lines = [
        "package caxecraft.localization;",
        "",
        "#if c",
    ]
    if scenario:
        lines.append("import caxecraft.localization.UiCatalog.LocaleCursor;")
    lines.extend(["import raylib.Color;", "import raylib.Raylib;", ""])
    if not scenario:
        lines.extend(enum_abstract("LocaleCursor", tuple(f"Locale{index}" for index in range(len(catalog.locales)))))
        lines.append("")
    lines.extend(enum_abstract(message_type, tuple(message.symbol for message in catalog.messages)))
    lines.extend(
        [
            "",
            "/**",
            f" * C rendering adapter generated from `{source_name}`.",
            " *",
            " * The JSON catalog is the editable source of truth. Each branch keeps a",
            " * direct string literal at the raylib call so haxe.c can prove static C",
            " * lifetime. Gameplay and UI code choose only typed message IDs.",
            " */",
            f"final class {class_name} {{",
        ]
    )
    if not scenario:
        lines.extend(
            [
                "\tpublic static inline function defaultLocale():LocaleCursor",
                "\t\treturn LocaleCursor.Locale0;",
                "",
                "\tpublic static function nextLocale(locale:LocaleCursor):LocaleCursor",
                "\t\treturn switch (locale) {",
            ]
        )
        for index in range(len(catalog.locales)):
            next_index = (index + 1) % len(catalog.locales)
            lines.append(f"\t\t\tcase Locale{index}: Locale{next_index};")
        # Enum abstracts have a primitive representation, so a foreign or
        # corrupted value is theoretically possible. Recover to the reviewed
        # default instead of letting an invalid value escape this adapter.
        lines.append("\t\t\tcase _: Locale0;")
        lines.extend(["\t\t}", ""])
    lines.extend(
        [
            f"\tpublic static function draw(locale:LocaleCursor, message:{message_type}, x:Int, y:Int, fontSize:Int, color:Color):Void {{",
            "\t\tswitch (locale) {",
        ]
    )
    for index in range(len(catalog.locales)):
        lines.extend(
            [
                f"\t\t\tcase Locale{index}:",
                f"\t\t\t\tdrawLocale{index}(message, x, y, fontSize, color);",
            ]
        )
    lines.extend(["\t\t\tcase _:", "\t\t\t\tdrawLocale0(message, x, y, fontSize, color);"])
    lines.extend(["\t\t}", "\t}", ""])
    for locale_index, locale in enumerate(catalog.locales):
        lines.extend(
            [
                f"\t/** Direct C literals for the validated `{locale}` catalog. */",
                f"\tstatic function drawLocale{locale_index}(message:{message_type}, x:Int, y:Int, fontSize:Int, color:Color):Void {{",
                "\t\tswitch (message) {",
            ]
        )
        for message in catalog.messages:
            lines.extend(
                [
                    f"\t\t\tcase {message.symbol}:",
                    f"\t\t\t\tRaylib.DrawText({haxe_string(message.text[locale_index])}, x, y, fontSize, color);",
                ]
            )
        lines.extend(["\t\t}", "\t}", ""])
    lines.pop()
    lines.extend(["}", "#end", ""])
    return "\n".join(lines)


def rendered_catalogs() -> dict[Path, str]:
    ui = load_catalog(UI_SOURCE, expected_id="caxecraft.ui")
    scenario = load_catalog(SCENARIO_SOURCE, expected_id="caxecraft.scenario.first-playable")
    if ui.locales != scenario.locales or ui.default_locale != scenario.default_locale:
        raise CatalogFailure("the built-in UI and first-playable catalogs must use the same locale order and default")
    return {
        UI_OUTPUT: render_catalog(ui, scenario=False),
        SCENARIO_OUTPUT: render_catalog(scenario, scenario=True),
    }


def expected_draw_call_count() -> int:
    """Return the direct raylib literal calls required by the source catalogs."""

    ui = load_catalog(UI_SOURCE, expected_id="caxecraft.ui")
    scenario = load_catalog(SCENARIO_SOURCE, expected_id="caxecraft.scenario.first-playable")
    return len(ui.locales) * len(ui.messages) + len(scenario.locales) * len(scenario.messages)


def write_catalogs() -> None:
    for path, text in rendered_catalogs().items():
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(text, encoding="utf-8", newline="\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if generated Haxe is stale")
    arguments = parser.parse_args()
    try:
        rendered = rendered_catalogs()
        if arguments.check:
            stale = [path.relative_to(CASE) for path, text in rendered.items() if not path.is_file() or path.read_text(encoding="utf-8") != text]
            if stale:
                raise CatalogFailure(f"generated localization adapters are stale: {stale}")
        else:
            write_catalogs()
    except (OSError, UnicodeError, CatalogFailure) as error:
        print(f"caxecraft-localization: ERROR: {error}")
        return 1
    action = "current" if arguments.check else "generated"
    print(f"caxecraft-localization: OK: catalogs validated and adapters {action}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
