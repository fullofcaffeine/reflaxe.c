#!/usr/bin/env python3
"""Focused crash-safety tests for immutable Caxecraft generated projects."""

from __future__ import annotations

import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CASE = ROOT / "examples/caxecraft"
sys.path.insert(0, str(CASE))

from dev_generation import (  # noqa: E402
    GenerationFailure,
    VariantLock,
    begin_transaction,
    current_generation,
    finalize_transaction,
    incomplete_transactions,
    publish_pointer,
)


class CaxecraftGenerationTests(unittest.TestCase):
    """Prove atomic selection, corruption rejection, and transaction isolation."""

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="hxc-caxecraft-generation-")
        self.variant = Path(self.temporary.name) / "variant"
        self.variant.mkdir()

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def build(self, source: str):
        transaction = begin_transaction(self.variant)
        (transaction.generated / "main.c").write_text(source, encoding="utf-8")
        return transaction, finalize_transaction(self.variant, transaction)

    def test_unpublished_generation_does_not_replace_current(self) -> None:
        with VariantLock(self.variant / "build.lock"):
            _, first = self.build("int first(void) { return 1; }\n")
            first = publish_pointer(self.variant, first)
            _, second = self.build("int second(void) { return 2; }\n")

        self.assertEqual(current_generation(self.variant).generation_id, first.generation_id)
        self.assertNotEqual(second.generation_id, first.generation_id)

    def test_atomic_pointer_selects_complete_generations_monotonically(self) -> None:
        with VariantLock(self.variant / "build.lock"):
            _, first = self.build("int first(void) { return 1; }\n")
            first = publish_pointer(self.variant, first)
            _, second = self.build("int second(void) { return 2; }\n")
            second = publish_pointer(self.variant, second)

        selected = current_generation(self.variant)
        self.assertEqual(first.sequence, 1)
        self.assertEqual(second.sequence, 2)
        self.assertEqual(selected.generation_id, second.generation_id)
        self.assertEqual(
            (selected.generated / "main.c").read_text(encoding="utf-8"),
            "int second(void) { return 2; }\n",
        )

    def test_abandoned_transaction_is_never_selected(self) -> None:
        abandoned = begin_transaction(self.variant)
        (abandoned.generated / "partial.c").write_text("partial", encoding="utf-8")

        self.assertEqual(list(incomplete_transactions(self.variant)), [abandoned.root])
        with self.assertRaisesRegex(GenerationFailure, "no generated project"):
            current_generation(self.variant)

    def test_mutated_generation_and_pointer_are_rejected(self) -> None:
        with VariantLock(self.variant / "build.lock"):
            _, generation = self.build("int main(void) { return 0; }\n")
            generation = publish_pointer(self.variant, generation)
        (generation.generated / "main.c").write_text("corrupt\n", encoding="utf-8")
        with self.assertRaisesRegex(GenerationFailure, "missing or corrupt"):
            current_generation(self.variant)

        pointer = self.variant / "hxc-play-current-generation.json"
        value = json.loads(pointer.read_text(encoding="utf-8"))
        value["sequence"] = 99
        pointer.write_text(json.dumps(value), encoding="utf-8")
        with self.assertRaisesRegex(GenerationFailure, "pointer digest"):
            current_generation(self.variant)

    def test_identical_content_reuses_one_generation(self) -> None:
        with VariantLock(self.variant / "build.lock"):
            _, first = self.build("int same(void) { return 1; }\n")
            first = publish_pointer(self.variant, first)
            _, second = self.build("int same(void) { return 1; }\n")
            second = publish_pointer(self.variant, second)

        self.assertEqual(first.generation_id, second.generation_id)
        generations = [
            path for path in (self.variant / "generations").iterdir() if path.is_dir()
        ]
        self.assertEqual(generations, [first.root])
        self.assertEqual(second.sequence, 2)


if __name__ == "__main__":
    unittest.main()
