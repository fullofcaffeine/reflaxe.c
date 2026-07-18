import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "scripts/research/build_cross_target_bundle.py"
SPEC = importlib.util.spec_from_file_location("cross_target_bundle", SCRIPT)
assert SPEC is not None and SPEC.loader is not None
BUNDLE = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = BUNDLE
SPEC.loader.exec_module(BUNDLE)


class CrossTargetBundleTests(unittest.TestCase):
    def test_checked_in_recipe_and_prompt_are_well_formed(self):
        result = BUNDLE.check_recipe(
            ROOT / "docs/specs/cross-target-research-bundle.json"
        )
        self.assertEqual("pass", result["status"])
        self.assertGreaterEqual(result["repositoryCount"], 10)
        self.assertGreaterEqual(result["omissionCount"], 4)

    def test_recipe_is_strict_json(self):
        recipe = json.loads(
            (ROOT / "docs/specs/cross-target-research-bundle.json").read_text(
                encoding="utf-8"
            )
        )
        self.assertEqual(1, recipe["schemaVersion"])
        self.assertFalse(recipe["artifact"]["releaseArtifact"])
        self.assertEqual(
            "git-commit-objects", recipe["policy"]["sourceAuthority"]
        )
        self.assertTrue((ROOT / recipe["tooling"]["gitleaksConfigPath"]).is_file())

    def test_unsafe_relative_paths_are_rejected(self):
        for value in ("../escape", "/absolute", "a/./b", "a\\b"):
            with self.subTest(value=value):
                with self.assertRaises(BUNDLE.BundleError):
                    BUNDLE.validate_relative_path(value, "test path")

    def test_selection_excludes_build_and_credential_paths(self):
        recipe = BUNDLE.load_spec(
            ROOT / "docs/specs/cross-target-research-bundle.json"
        )
        blobs = [
            BUNDLE.TreeBlob("src/Main.hx", "a" * 40, 0o100644),
            BUNDLE.TreeBlob("src/generated/Main.c", "b" * 40, 0o100644),
            BUNDLE.TreeBlob("src/node_modules/pkg/index.js", "c" * 40, 0o100644),
            BUNDLE.TreeBlob("src/.env", "d" * 40, 0o100644),
            BUNDLE.TreeBlob("src/dist-preview/Main.js", "e" * 40, 0o100644),
        ]
        selected, excluded = BUNDLE.select_tree(
            blobs, ["src"], [], recipe["policy"], "fixture"
        )
        self.assertEqual(["src/Main.hx"], [blob.path for blob in selected])
        self.assertEqual(4, excluded)

    def test_secret_and_local_path_scans_fail_closed(self):
        safe = {
            "source.txt": BUNDLE.ArchiveEntry(
                b"ordinary source", 0o644, "fixture", "source.txt"
            )
        }
        BUNDLE.scan_content(safe)
        for content in (
            b"-----BEGIN " + b"PRIVATE KEY-----",
            b"path=" + b"/" + b"Users/alice/private/project",
            b"path=" + b"/" + b"home/alice/private/project",
        ):
            with self.subTest(content=content):
                with self.assertRaises(BUNDLE.BundleError):
                    BUNDLE.scan_content(
                        {
                            "bad.txt": BUNDLE.ArchiveEntry(
                                content, 0o644, "fixture", "bad.txt"
                            )
                        }
                    )

    def test_zip_is_deterministic_and_extracts_identically(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            stage = root / "stage"
            stage.mkdir()
            entries = {
                "PROMPT.md": BUNDLE.ArchiveEntry(
                    b"prompt\n", 0o644, "fixture", "prompt.md"
                ),
                "sources/example/src/Main.hx": BUNDLE.ArchiveEntry(
                    b"class Main {}\n", 0o644, "fixture", "src/Main.hx"
                ),
            }
            BUNDLE.write_stage(stage, entries)
            first = root / "first.zip"
            second = root / "second.zip"
            BUNDLE.build_zip(stage, first)
            BUNDLE.build_zip(stage, second)
            self.assertEqual(first.read_bytes(), second.read_bytes())
            extraction = root / "extracted"
            extraction.mkdir()
            self.assertEqual(2, BUNDLE.verify_zip(first, stage, extraction))


if __name__ == "__main__":
    unittest.main()
