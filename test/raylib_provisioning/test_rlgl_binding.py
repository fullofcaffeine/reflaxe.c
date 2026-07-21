#!/usr/bin/env python3
"""Adversarial tests for Caxecraft's precise pinned rlgl slice."""

from __future__ import annotations

import copy
import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raylib.rlgl_binding import (  # noqa: E402
    BindingFailure,
    LOCK_PATH,
    PROBE_PATH,
    RAW_PATH,
    SELECTION_PATH,
    load_lock,
    load_selection,
    render_abi_probe,
    render_raw,
    validate_lock,
)


class RaylibRlglBindingTests(unittest.TestCase):
    def test_repository_lock_and_generated_files_are_current(self) -> None:
        lock = load_lock()
        self.assertEqual(render_raw(lock), render_raw(lock))
        self.assertEqual(render_abi_probe(lock), render_abi_probe(lock))
        self.assertEqual(RAW_PATH.read_text(encoding="utf-8"), render_raw(lock))
        self.assertEqual(PROBE_PATH.read_text(encoding="utf-8"), render_abi_probe(lock))

    def test_slice_is_exact_and_omits_identity_metadata(self) -> None:
        selection = load_selection()
        self.assertEqual(selection["constant"], {"name": "RL_QUADS", "value": 7})
        self.assertEqual(len(selection["functions"]), 7)
        rendered = render_raw(load_lock())
        for forbidden in ("Dynamic", "untyped", "__c__", "@:native", "@:c.name(", "new "):
            self.assertNotIn(forbidden, rendered)
        self.assertIn('@:c.include("rlgl.h", c.IncludeKind.System)', rendered)

    def test_stale_selection_hash_fails_closed(self) -> None:
        lock = copy.deepcopy(load_lock())
        lock["selection"]["sha256"] = "0" * 64
        with self.assertRaisesRegex(BindingFailure, "selection hash is stale"):
            validate_lock(lock)

    def test_selection_and_lock_schemas_are_closed_documents(self) -> None:
        for path in (
            ROOT / "docs/specs/raylib-rlgl-selection.schema.json",
            ROOT / "docs/specs/raylib-rlgl-binding-lock.schema.json",
        ):
            schema = json.loads(path.read_text(encoding="utf-8"))
            self.assertEqual(schema["$schema"], "https://json-schema.org/draft/2020-12/schema")
            self.assertFalse(schema["additionalProperties"])
        self.assertTrue(SELECTION_PATH.is_file())
        self.assertTrue(LOCK_PATH.is_file())


if __name__ == "__main__":
    unittest.main()
