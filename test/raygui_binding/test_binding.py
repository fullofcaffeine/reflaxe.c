#!/usr/bin/env python3
"""Focused checks for the pinned raygui raw binding and typed Haxe facade."""

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

from scripts.raylib.core_binding import BindingFailure  # noqa: E402
from scripts.raygui.core_binding import (  # noqa: E402
    LOCK_PATH,
    RAW_PATH,
    SELECTION_PATH,
    load_lock,
    render_raw,
    validate_lock,
)
from scripts.raygui.provision import (  # noqa: E402
    RayguiProvisionFailure,
    compiler_warning_flags,
    implementation_bytes,
    normalize_archive_headers,
    pinned_source,
)


class RayguiBindingTests(unittest.TestCase):
    def test_repository_lock_and_generated_raw_file_are_current(self) -> None:
        lock = load_lock()
        self.assertEqual(render_raw(lock), render_raw(lock))
        self.assertEqual(RAW_PATH.read_text(encoding="utf-8"), render_raw(lock))

    def test_raw_surface_is_selected_and_contains_no_escape_hatch(self) -> None:
        rendered = render_raw(load_lock())
        self.assertIn("public static function GuiButton", rendered)
        self.assertIn("public static function GuiPanel", rendered)
        for forbidden in ("@:c.name(", "Dynamic", "untyped", "__c__", "@:native"):
            self.assertNotIn(forbidden, rendered)

    def test_one_implementation_owner_is_locked(self) -> None:
        self.assertEqual(
            load_lock()["implementation"],
            {
                "macro": "RAYGUI_IMPLEMENTATION",
                "owner": "raygui-static-library",
                "translationUnits": 1,
                "sourceTemplateSha256": "3c206e58f97342b6b2359be7e28c49620b9045efc9b04fa89fe70102d0b7ce73",
            },
        )

    def test_implementation_template_has_one_content_addressed_owner(self) -> None:
        source = implementation_bytes()
        self.assertEqual(source.count(b"RAYGUI_IMPLEMENTATION"), 1)
        self.assertEqual(source, b'#define RAYGUI_IMPLEMENTATION\n#include "raygui.h"\n')

    def test_offline_provisioning_fails_closed_without_locked_archive(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-raygui-empty-cache-") as raw_root:
            with self.assertRaisesRegex(RayguiProvisionFailure, "archive is absent"):
                pinned_source(Path(raw_root), load_lock(), allow_network=False)

    def test_archive_metadata_normalization_is_deterministic(self) -> None:
        member = b"abc"
        # One minimal ar member with deliberately host-specific time/user IDs.
        header = (
            b"probe.o/        "
            + b"1784745836  "
            + b"501   "
            + b"20    "
            + b"100644  "
            + f"{len(member):<10}".encode("ascii")
            + b"`\n"
        )
        with tempfile.TemporaryDirectory(prefix="hxc-raygui-ar-") as raw_root:
            archive = Path(raw_root) / "libprobe.a"
            archive.write_bytes(b"!<arch>\n" + header + member + b"\n")
            normalize_archive_headers(archive)
            first = archive.read_bytes()
            normalize_archive_headers(archive)
            self.assertEqual(archive.read_bytes(), first)
            self.assertIn(b"0           0     0     ", first)

    def test_warning_exception_uses_only_the_selected_compiler_vocabulary(self) -> None:
        self.assertEqual(compiler_warning_flags("clang version 18.1.0"), ("-Wno-error=shorten-64-to-32",))
        self.assertEqual(compiler_warning_flags("gcc (GCC) 14.2.0"), ())

    def test_lock_rejects_upstream_and_extraction_drift(self) -> None:
        lock = copy.deepcopy(load_lock())
        lock["upstream"]["archive"]["sizeBytes"] += 1
        with self.assertRaisesRegex(BindingFailure, "upstream identity drifted"):
            validate_lock(lock)

        lock = copy.deepcopy(load_lock())
        lock["extraction"]["requestedTarget"] = "aarch64-unknown-linux-gnu"
        with self.assertRaisesRegex(BindingFailure, "extraction contract drifted"):
            validate_lock(lock)

    def test_selection_and_lock_schemas_are_closed_documents(self) -> None:
        self.assertTrue(SELECTION_PATH.is_file())
        self.assertTrue(LOCK_PATH.is_file())
        for path in (
            ROOT / "docs/specs/raygui-core-selection.schema.json",
            ROOT / "docs/specs/raygui-core-binding-lock.schema.json",
        ):
            schema = json.loads(path.read_text(encoding="utf-8"))
            self.assertEqual(schema["$schema"], "https://json-schema.org/draft/2020-12/schema")
            self.assertFalse(schema["additionalProperties"])

    def test_semantic_facade_compiles_to_direct_raygui_calls(self) -> None:
        fixture = ROOT / "test/raygui_binding/fixtures/semantic"
        haxe = ROOT / "node_modules/.bin/haxe"
        with tempfile.TemporaryDirectory(prefix="hxc-raygui-semantic-") as raw_root:
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
            self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
            program = (output / "src/program.c").read_text(encoding="utf-8")
            self.assertIn("GuiButton", program)
            self.assertIn("GuiPanel", program)
            self.assertNotIn("hxrt", program)


if __name__ == "__main__":
    unittest.main()
