#!/usr/bin/env python3
"""Keep Caxecraft UI text in one authoritative data catalog."""

from __future__ import annotations

import json
import re
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CAXECRAFT = ROOT / "examples" / "caxecraft"
SOURCE_ROOT = CAXECRAFT / "src"
UI_TYPES = SOURCE_ROOT / "caxecraft" / "localization" / "UiTypes.hx"
UI_CATALOG = CAXECRAFT / "locales" / "ui.json"
MESSAGE_DECLARATION = re.compile(r'^\s*var ([A-Z][A-Za-z0-9_]*) = "([^"]+)";', re.MULTILINE)


class CaxecraftLocalizationTests(unittest.TestCase):
    """Prove that code refers to catalog data without mirroring its membership."""

    def setUp(self) -> None:
        self.type_source = UI_TYPES.read_text(encoding="utf-8")
        self.catalog = json.loads(UI_CATALOG.read_text(encoding="utf-8"))
        self.declarations = MESSAGE_DECLARATION.findall(self.type_source)
        self.catalog_messages = self.catalog["messages"]

    def test_typed_keys_exist_in_the_authoritative_catalog(self) -> None:
        catalog_ids = [message["id"] for message in self.catalog_messages]
        code_ids = [message_id for _, message_id in self.declarations]

        self.assertEqual(len(catalog_ids), len(set(catalog_ids)))
        self.assertEqual(len(code_ids), len(set(code_ids)))
        self.assertEqual(set(code_ids) - set(catalog_ids), set())

    def test_each_typed_key_has_a_real_production_call_site(self) -> None:
        production_source = "\n".join(
            path.read_text(encoding="utf-8")
            for path in sorted(SOURCE_ROOT.rglob("*.hx"))
            if path != UI_TYPES
        )

        unused = [
            name
            for name, _ in self.declarations
            if re.search(rf"\bUiMessage\.{re.escape(name)}\b", production_source) is None
        ]
        self.assertEqual(unused, [])

    def test_catalog_does_not_duplicate_code_owned_symbols(self) -> None:
        messages_with_symbols = [
            message["id"] for message in self.catalog_messages if "symbol" in message
        ]
        self.assertEqual(messages_with_symbols, [])


if __name__ == "__main__":
    unittest.main()
