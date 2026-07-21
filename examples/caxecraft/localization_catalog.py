#!/usr/bin/env python3
"""Validate Caxecraft text sources and generate narrow C rendering adapters.

Reusable interface copy comes from one JSON catalog. Authored Adventure copy
comes from the same CaxeMap that owns its world and story references. Generated
Haxe keeps locale selection and C string-lifetime details out of gameplay code.
"""

from __future__ import annotations

import argparse
import json
import re
from dataclasses import dataclass
from pathlib import Path


CASE = Path(__file__).resolve().parent
UI_SOURCE = CASE / "locales/ui.json"
SCENARIO_SOURCE = CASE / "scenarios/first-playable/map.caxemap"
UI_OUTPUT = CASE / "src/caxecraft/localization/UiCatalog.hx"
SCENARIO_OUTPUT = CASE / "src/caxecraft/localization/FirstPlayableCatalog.hx"
CATALOG_KEYS = {"schemaVersion", "catalogId", "defaultLocale", "locales", "messages"}
MESSAGE_KEYS = {"id", "symbol", "text"}
LOCALE_PATTERN = re.compile(r"^[a-z]{2}(?:-[A-Z]{2})?$")
SCENARIO_LOCALE_PATTERN = re.compile(r"^[a-z]{2}(?:-[a-z]{2})?$")
MESSAGE_PATTERN = re.compile(r"^[a-z][a-z0-9]*(?:[._-][a-z][a-z0-9]*)*$")
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


def decode_caxemap_quoted(value: str, *, line: int) -> str:
    """Decode the quoted-token subset defined by CAXEMAP 1."""

    if not value.startswith('"'):
        raise CatalogFailure(f"line {line}: message text must be quoted")
    output: list[str] = []
    index = 1
    while index < len(value):
        character = value[index]
        if character == '"':
            if index != len(value) - 1:
                raise CatalogFailure(f"line {line}: text has trailing tokens")
            return validate_text("".join(output), coordinate=f"line {line} message text")
        if ord(character) < 32 or ord(character) == 127:
            raise CatalogFailure(f"line {line}: text contains a control character")
        if character != "\\":
            output.append(character)
            index += 1
            continue
        index += 1
        if index >= len(value):
            raise CatalogFailure(f"line {line}: unfinished escape")
        escape = value[index]
        simple = {'"': '"', "\\": "\\", "n": "\n", "r": "\r", "t": "\t"}
        if escape in simple:
            output.append(simple[escape])
            index += 1
            continue
        if escape != "u" or index + 1 >= len(value) or value[index + 1] != "{":
            raise CatalogFailure(f"line {line}: invalid escape")
        close = value.find("}", index + 2)
        digits = value[index + 2 : close] if close != -1 else ""
        if close == -1 or not 1 <= len(digits) <= 6 or any(character not in "0123456789ABCDEF" for character in digits):
            raise CatalogFailure(f"line {line}: invalid Unicode escape")
        scalar = int(digits, 16)
        if scalar == 0 or scalar > 0x10FFFF or 0xD800 <= scalar <= 0xDFFF:
            raise CatalogFailure(f"line {line}: invalid Unicode scalar")
        output.append(chr(scalar))
        index = close + 1
    raise CatalogFailure(f"line {line}: unterminated quoted text")


def scenario_symbol(message_id: str) -> str:
    symbol = "".join(part[0].upper() + part[1:] for part in re.split(r"[._-]", message_id))
    if not SYMBOL_PATTERN.fullmatch(symbol):
        raise CatalogFailure(f"message ID {message_id!r} cannot form a bounded Haxe symbol")
    return symbol


def load_scenario_catalog(path: Path, *, expected_id: str) -> Catalog:
    """Extract one complete embedded catalog from a canonical CaxeMap."""

    try:
        raw = path.read_bytes()
    except OSError as error:
        raise CatalogFailure(f"cannot read {path.relative_to(CASE)}: {error}") from error
    if raw.startswith(b"\xef\xbb\xbf") or b"\r" in raw or not raw.endswith(b"\n"):
        raise CatalogFailure(f"{path.relative_to(CASE)} must use UTF-8 without BOM and canonical LF")
    try:
        lines = raw.decode("utf-8").splitlines()
    except UnicodeDecodeError as error:
        raise CatalogFailure(f"{path.relative_to(CASE)} is not valid UTF-8") from error
    if not lines or lines[0] != "CAXEMAP 1":
        raise CatalogFailure("scenario catalog source must start with CAXEMAP 1")

    map_id: str | None = None
    default_locale: str | None = None
    locale_order: list[str] = []
    locale_messages: dict[str, list[tuple[str, str]]] = {}
    active_locale: str | None = None
    for line_number, line in enumerate(lines[1:], start=2):
        if active_locale is not None:
            if line == "end locale":
                active_locale = None
                continue
            match = re.fullmatch(r'  message ([a-z][a-z0-9]*(?:[._-][a-z][a-z0-9]*)*) (".*")', line)
            if match is None:
                raise CatalogFailure(f"line {line_number}: locale blocks contain only canonical message records")
            message_id, quoted = match.groups()
            if any(existing == message_id for existing, _ in locale_messages[active_locale]):
                raise CatalogFailure(f"line {line_number}: duplicate message ID {message_id!r}")
            locale_messages[active_locale].append((message_id, decode_caxemap_quoted(quoted, line=line_number)))
            continue
        if line.startswith("map "):
            if map_id is not None:
                raise CatalogFailure(f"line {line_number}: duplicate map record")
            map_id = line[4:]
        elif line.startswith("default-locale "):
            if default_locale is not None:
                raise CatalogFailure(f"line {line_number}: duplicate default locale")
            default_locale = line[len("default-locale ") :]
        elif line.startswith("locale "):
            locale = line[len("locale ") :]
            if not SCENARIO_LOCALE_PATTERN.fullmatch(locale):
                raise CatalogFailure(f"line {line_number}: invalid scenario locale {locale!r}")
            if locale in locale_messages:
                raise CatalogFailure(f"line {line_number}: duplicate scenario locale {locale!r}")
            locale_order.append(locale)
            locale_messages[locale] = []
            active_locale = locale
    if active_locale is not None:
        raise CatalogFailure(f"locale {active_locale!r} is missing end locale")
    if map_id != expected_id:
        raise CatalogFailure(f"map ID must be {expected_id!r}")
    if default_locale is None or default_locale not in locale_messages:
        raise CatalogFailure("default locale must name one embedded locale")
    if not 1 <= len(locale_order) <= 8 or locale_order[0] != default_locale:
        raise CatalogFailure("default locale must be the first of one to eight embedded locales")
    if locale_order[1:] != sorted(locale_order[1:], key=lambda value: value.encode("utf-8")):
        raise CatalogFailure("non-default locales must be ordered by UTF-8 locale ID")

    base_ids = [message_id for message_id, _ in locale_messages[default_locale]]
    if not base_ids or base_ids != sorted(base_ids, key=lambda value: value.encode("utf-8")):
        raise CatalogFailure("default-locale messages must be non-empty and ordered by UTF-8 ID")
    symbols = [scenario_symbol(message_id) for message_id in base_ids]
    if len(set(symbols)) != len(symbols):
        raise CatalogFailure("scenario message IDs produce duplicate Haxe symbols")
    by_locale: dict[str, dict[str, str]] = {}
    for locale in locale_order:
        entries = locale_messages[locale]
        ids = [message_id for message_id, _ in entries]
        if ids != base_ids:
            raise CatalogFailure(f"locale {locale!r} must translate exactly the default message IDs in the same order")
        by_locale[locale] = dict(entries)
    messages = tuple(
        Message(message_id, symbol, tuple(by_locale[locale][message_id] for locale in locale_order))
        for message_id, symbol in zip(base_ids, symbols, strict=True)
    )
    return Catalog(expected_id, default_locale, tuple(locale_order), messages)


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
    source_name = "scenarios/first-playable/map.caxemap" if scenario else "locales/ui.json"
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
            f" * The {'embedded CaxeMap catalog' if scenario else 'JSON catalog'} is the editable source of truth. Each branch keeps a",
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
    scenario = load_scenario_catalog(SCENARIO_SOURCE, expected_id="adventure.first-playable")
    if tuple(locale.lower() for locale in ui.locales) != scenario.locales or ui.default_locale.lower() != scenario.default_locale:
        raise CatalogFailure("the built-in UI and first-playable catalogs must use the same locale order and default")
    return {
        UI_OUTPUT: render_catalog(ui, scenario=False),
        SCENARIO_OUTPUT: render_catalog(scenario, scenario=True),
    }


def expected_draw_call_count() -> int:
    """Return the direct raylib literal calls required by the source catalogs."""

    ui = load_catalog(UI_SOURCE, expected_id="caxecraft.ui")
    scenario = load_scenario_catalog(SCENARIO_SOURCE, expected_id="adventure.first-playable")
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
