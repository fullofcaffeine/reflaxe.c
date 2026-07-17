from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "scripts/ci/check_typed_boundaries.py"
MANIFEST = Path("docs/specs/typed-boundaries.json")
SCHEMA = Path("docs/specs/typed-boundaries.schema.json")
EXCLUDED_PREFIXES = (
    ".beads/",
    ".git/",
    ".hxc/",
    "node_modules/",
    "runtime/third_party/",
    "runtime/vendor/",
    "third_party/",
    "vendor/",
)


class TypedBoundaryTests(unittest.TestCase):
    def run_guard(self, root: Path = ROOT) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, str(SCRIPT), "--root", str(root), "--quiet"],
            text=True,
            capture_output=True,
            check=False,
        )

    def make_root(self, destination: Path) -> None:
        for source in ROOT.rglob("*.hx"):
            relative = source.relative_to(ROOT).as_posix()
            if any(relative.startswith(prefix) for prefix in EXCLUDED_PREFIXES):
                continue
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(source, target)

        manifest = json.loads((ROOT / MANIFEST).read_text(encoding="utf-8"))
        evidence = {
            Path(path)
            for allowance in manifest["allowlist"]
            for path in allowance["testEvidence"]
        }
        for relative in (
            MANIFEST,
            SCHEMA,
            *sorted(evidence, key=lambda path: path.as_posix()),
        ):
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, target)

    def write_source(self, root: Path, relative: str, source: str) -> None:
        path = root / relative
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(source, encoding="utf-8")

    def load_manifest(self, root: Path) -> dict[str, object]:
        value = json.loads((root / MANIFEST).read_text(encoding="utf-8"))
        self.assertIsInstance(value, dict)
        return value

    def write_manifest(self, root: Path, value: dict[str, object]) -> None:
        (root / MANIFEST).write_text(
            json.dumps(value, indent=2, ensure_ascii=False) + "\n",
            encoding="utf-8",
        )

    def test_repository_inventory_passes(self) -> None:
        result = self.run_guard()
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_new_dynamic_semantic_state_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            self.write_source(
                root,
                "src/reflaxe/c/NewSemanticState.hx",
                "package reflaxe.c;\nclass NewSemanticState {\n"
                "  public var value:Dynamic;\n}\n",
            )
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("unapproved typed-boundary construct dynamic-type", result.stderr)

    def test_reflection_any_open_record_and_untyped_are_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            self.write_source(
                root,
                "src/reflaxe/c/UncheckedState.hx",
                "package reflaxe.c;\nclass UncheckedState {\n"
                "  static var erased:Any;\n"
                "  static var fields:haxe.DynamicAccess<Int>;\n"
                "  static function read(value:String):String {\n"
                "    untyped value;\n"
                "    return Reflect.field(value, \"length\");\n"
                "  }\n}\n",
            )
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            for construct in (
                "any-type",
                "open-record",
                "untyped-keyword",
                "reflect-api",
            ):
                self.assertIn(
                    f"unapproved typed-boundary construct {construct}", result.stderr
                )

    def test_raw_and_private_authority_are_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            self.write_source(
                root,
                "test/UncheckedAuthority.hx",
                "@:privateAccess class UncheckedAuthority {\n"
                "  static function run():Void __c__(\"danger\");\n}\n",
            )
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("private-access", result.stderr)
            self.assertIn("raw-code-injection", result.stderr)

    def test_comments_plain_strings_and_regex_literals_are_not_code(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            self.write_source(
                root,
                "test/LexicalNoise.hx",
                "class LexicalNoise {\n"
                "  // Dynamic Any Reflect.field(value, name) untyped cast(value)\n"
                "  static final text = \"Json.parse __c__ DynamicAccess\";\n"
                "  static final pattern = ~/Reflect\\.field|untyped|cast/;\n"
                "}\n",
            )
            result = self.run_guard(root)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_single_quoted_interpolation_is_scanned_as_code(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            self.write_source(
                root,
                "test/InterpolatedReflection.hx",
                "class InterpolatedReflection {\n"
                "  static function render(holder:String):String\n"
                "    return '${Reflect.field(holder, \"length\")}';\n"
                "}\n",
            )
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("reflect-api", result.stderr)

    def test_another_json_ingress_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            self.write_source(
                root,
                "src/reflaxe/c/UncheckedJson.hx",
                "package reflaxe.c;\nimport haxe.Json;\n"
                "class UncheckedJson {\n"
                "  static function decode(text:String) return Json.parse(text);\n"
                "}\n",
            )
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("unapproved typed-boundary construct json-parse", result.stderr)

    def test_removed_boundary_leaves_a_stale_allowance(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            path = root / "test/hxc_ir/fixtures/IRCoverage.hx"
            source = path.read_text(encoding="utf-8")
            self.assertIn("value:Dynamic", source)
            path.write_text(source.replace("value:Dynamic", "value:Int"), encoding="utf-8")
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("stale typed-boundary allowance", result.stderr)

    def test_source_line_drift_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            manifest = self.load_manifest(root)
            allowlist = manifest["allowlist"]
            self.assertIsInstance(allowlist, list)
            first = allowlist[0]
            self.assertIsInstance(first, dict)
            first["sourceLine"] = "final metadata = Json.parse(text);"
            self.write_manifest(root, manifest)
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("sourceLine drifted", result.stderr)

    def test_unknown_manifest_fields_are_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            manifest = self.load_manifest(root)
            manifest["escapeHatch"] = True
            self.write_manifest(root, manifest)
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("unknown fields: escapeHatch", result.stderr)

    def test_test_only_allowance_cannot_excuse_production_source(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_root(root)
            manifest = self.load_manifest(root)
            allowlist = manifest["allowlist"]
            self.assertIsInstance(allowlist, list)
            first = allowlist[0]
            self.assertIsInstance(first, dict)
            first["boundaryKind"] = "negative-test"
            self.write_manifest(root, manifest)
            result = self.run_guard(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("test-only boundary outside test/", result.stderr)


if __name__ == "__main__":
    unittest.main()
