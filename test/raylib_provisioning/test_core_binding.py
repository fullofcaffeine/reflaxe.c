#!/usr/bin/env python3
"""Adversarial tests for the pinned raylib raw-core binding boundary."""

from __future__ import annotations

import copy
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))

from scripts.raylib.core_binding import (
    BindingFailure,
    CANONICAL_TARGET,
    LOCK_PATH,
    RAW_ROOT,
    SELECTION_PATH,
    check_rendered,
    digest_json,
    load_lock,
    load_selection,
    render_files,
    require_selected_node,
    selection_sha256,
    type_fact,
    validate_lock,
)


class RaylibCoreBindingTests(unittest.TestCase):
    def test_repository_lock_and_generated_raw_files_are_current(self) -> None:
        lock = load_lock()
        first = render_files(lock)
        second = render_files(lock)
        self.assertEqual(first, second)
        self.assertEqual(len(first), 17)
        check_rendered(RAW_ROOT, first)

    def test_raw_layer_has_no_unsafe_or_target_foreign_shortcut(self) -> None:
        rendered = "\n".join(render_files(load_lock()).values())
        for forbidden in (
            "cpp.",
            "Dynamic",
            "untyped",
            "__c__",
            "@:native",
            "Reflect.",
            "new ",
        ):
            self.assertNotIn(forbidden, rendered)

    def test_raw_layer_omits_redundant_identity_metadata(self) -> None:
        rendered = "\n".join(render_files(load_lock()).values())
        self.assertNotIn("@:c.name(", rendered)
        self.assertNotIn("@:c.callingConvention(", rendered)
        self.assertIn("extern class Camera3D", rendered)
        self.assertIn("public var position:Vector3", rendered)
        self.assertIn("public static function BeginDrawing():Void", rendered)

    def test_texture_resource_contract_keeps_ownership_visible(self) -> None:
        lock = load_lock()
        contracts = lock["selection"]["resourceContracts"]
        self.assertEqual(len(contracts), 1)
        contract = contracts[0]
        self.assertEqual(contract["resource"], "Texture2D")
        self.assertEqual(contract["load"], "LoadTexture")
        self.assertEqual(contract["validate"], "IsTextureValid")
        self.assertEqual(contract["borrow"], ["DrawBillboardRec", "DrawTexturePro"])
        self.assertEqual(contract["unload"], "UnloadTexture")
        self.assertEqual(
            contract["semanticStatus"],
            "raw-only-until-explicit-cleanup-edges",
        )
        rendered = render_files(lock)["Raylib.hx"]
        self.assertIn("Returns one caller-owned `Texture2D`", rendered)
        self.assertIn("Call `UnloadTexture` exactly once", rendered)
        self.assertIn("Treat every copied value as an alias", rendered)

    def test_lock_rejects_stale_selection_hash(self) -> None:
        lock = copy.deepcopy(load_lock())
        lock["selection"]["sha256"] = "0" * 64
        with self.assertRaisesRegex(BindingFailure, "selection hash is stale"):
            validate_lock(lock)

    def test_selection_hash_normalizes_only_text_line_endings(self) -> None:
        lock = copy.deepcopy(load_lock())
        source = SELECTION_PATH.read_bytes()
        self.assertNotIn(b"\r", source)
        with tempfile.TemporaryDirectory(
            prefix="hxc-raylib-selection-eol-"
        ) as raw_root:
            root = Path(raw_root)
            crlf_path = root / "selection-crlf.json"
            crlf_path.write_bytes(source.replace(b"\n", b"\r\n"))
            self.assertEqual(selection_sha256(), selection_sha256(crlf_path))
            validate_lock(lock, selection_path=crlf_path)

            changed = load_selection()
            changed["omissions"][0]["reason"] += " Semantic drift probe."
            changed_path = root / "selection-changed.json"
            changed_path.write_text(
                json.dumps(changed, ensure_ascii=False, indent=2, sort_keys=True)
                + "\n",
                encoding="utf-8",
                newline="\n",
            )
            with self.assertRaisesRegex(BindingFailure, "selection hash is stale"):
                validate_lock(lock, selection_path=changed_path)

            malformed_path = root / "selection-malformed.json"
            malformed_path.write_bytes(b"\xff")
            with self.assertRaisesRegex(
                BindingFailure, "cannot read raylib core selection"
            ):
                validate_lock(lock, selection_path=malformed_path)

    def test_lock_rejects_wrong_canonical_target(self) -> None:
        lock = copy.deepcopy(load_lock())
        self.assertEqual(CANONICAL_TARGET, "x86_64-unknown-linux-gnu")
        lock["extraction"]["requestedTarget"] = "aarch64-unknown-linux-gnu"
        with self.assertRaisesRegex(BindingFailure, "canonical target drifted"):
            validate_lock(lock)

    def test_lock_rejects_malformed_record_layout(self) -> None:
        lock = copy.deepcopy(load_lock())
        lock["declarations"]["records"][0]["canonicalAbi"]["size"] += 1
        lock["declarationSha256"] = digest_json(lock["declarations"])
        with self.assertRaisesRegex(BindingFailure, "canonical ABI is malformed"):
            validate_lock(lock)

    def test_lock_rejects_stale_declaration_digest(self) -> None:
        lock = copy.deepcopy(load_lock())
        lock["declarationSha256"] = "f" * 64
        with self.assertRaisesRegex(BindingFailure, "declaration digest is stale"):
            validate_lock(lock)

    def test_missing_selected_symbol_fails_closed(self) -> None:
        with self.assertRaisesRegex(BindingFailure, "missing from pinned raylib.h"):
            require_selected_node({}, "DrawCube", "function")

    def test_unsupported_selected_declaration_type_fails_closed(self) -> None:
        with self.assertRaisesRegex(BindingFailure, "unsupported selected C type"):
            type_fact(
                {"qualType": "void (*)(int)"},
                "synthetic callback",
            )

    def test_raw_window_title_rejects_embedded_nul_without_artifact(self) -> None:
        fixture = ROOT / "test/raylib_provisioning/fixtures/nul_title"
        haxe = ROOT / "node_modules/.bin/haxe"
        with tempfile.TemporaryDirectory(prefix="hxc-raylib-nul-") as raw_root:
            output = Path(raw_root) / "generated"
            environment = os.environ.copy()
            environment["HAXE_NO_SERVER"] = "1"
            environment["LC_ALL"] = "C"
            result = subprocess.run(
                [
                    str(haxe),
                    "--cwd",
                    str(fixture),
                    "build.hxml",
                    "-D",
                    "hxc_runtime_diagnostics=off",
                    "-D",
                    "hxc_project_layout=unity",
                    "--custom-target",
                    f"c={output}",
                ],
                cwd=ROOT,
                env=environment,
                check=False,
                capture_output=True,
                text=True,
                timeout=90,
            )
            combined = result.stdout + result.stderr
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("HXC3000", combined)
            self.assertIn("embedded NUL byte", combined)
            self.assertFalse((output / "src/program.c").exists())

    def test_selection_and_lock_schemas_are_closed_documents(self) -> None:
        load_selection()
        for path in (
            ROOT / "docs/specs/raylib-core-selection.schema.json",
            ROOT / "docs/specs/raylib-core-binding-lock.schema.json",
        ):
            schema = json.loads(path.read_text(encoding="utf-8"))
            self.assertEqual(schema["$schema"], "https://json-schema.org/draft/2020-12/schema")
            self.assertFalse(schema["additionalProperties"])
        self.assertTrue(SELECTION_PATH.is_file())
        self.assertTrue(LOCK_PATH.is_file())


if __name__ == "__main__":
    unittest.main()
