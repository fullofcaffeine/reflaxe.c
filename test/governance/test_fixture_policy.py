from __future__ import annotations

import contextlib
import importlib.util
import io
import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
RUNNER = ROOT / "scripts/test/snapshots.py"


def load_runner():
    spec = importlib.util.spec_from_file_location("snapshot_policy_subject", RUNNER)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {RUNNER}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


class FixturePolicyTests(unittest.TestCase):
    def setUp(self) -> None:
        self.runner = load_runner()

    def invoke(self, arguments: list[str]) -> tuple[int, str, str]:
        stdout = io.StringIO()
        stderr = io.StringIO()
        with (
            contextlib.redirect_stdout(stdout),
            contextlib.redirect_stderr(stderr),
        ):
            result = self.runner.main(arguments)
        return result, stdout.getvalue(), stderr.getvalue()

    def test_catalog_and_executable_registry_match(self) -> None:
        catalog = self.runner.read_catalog()
        entries = self.runner.managed_entries(catalog)
        self.assertEqual(tuple(entries), self.runner.registered_suite_ids())
        self.assertEqual(
            tuple(entries),
            (
                "bootstrap",
                "typed-c",
                "typed-ast",
                "c-ast",
                "declaration-plan",
                "symbol-registry",
                "project-emitter",
                "hxc-ir",
                "primitive-semantics",
                "body-lowering",
                "function-lowering",
                "evaluation-order",
                "arithmetic-semantics",
            ),
        )

    def test_update_requires_an_explicit_selector(self) -> None:
        result, stdout, stderr = self.invoke(["--update"])
        self.assertEqual(result, 1)
        self.assertEqual(stdout, "")
        self.assertIn("update mode requires --suite <id> or explicit --all", stderr)

    def test_ci_rejects_update_before_rendering(self) -> None:
        generators = {
            name: mock.Mock(side_effect=AssertionError("must not render in CI update"))
            for name in self.runner.GENERATORS
        }
        with (
            mock.patch.object(self.runner, "GENERATORS", generators),
            mock.patch.dict(os.environ, {"CI": "true"}),
        ):
            result, stdout, stderr = self.invoke(
                ["--update", "--suite", "bootstrap"]
            )
        self.assertEqual(result, 1)
        self.assertEqual(stdout, "")
        self.assertIn("snapshot update mode is forbidden when CI is set", stderr)
        for generator in generators.values():
            generator.assert_not_called()

    def test_targeted_update_prints_diff_then_validates(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            expected = root / "test/snapshot/sample/expected/result.json"
            expected.parent.mkdir(parents=True)
            expected.write_text('{"value": 1}\n', encoding="utf-8")
            catalog = root / "fixture-taxonomy.json"
            catalog.write_text(
                json.dumps(
                    {
                        "snapshotPolicy": {
                            "managedSuites": [
                                {
                                    "id": "sample",
                                    "expectedRoots": [
                                        "test/snapshot/sample/expected"
                                    ],
                                    "formats": ["json"],
                                }
                            ]
                        },
                        "suites": [
                            {
                                "id": "sample",
                                "runner": [sys.executable, "-c", "pass"],
                            }
                        ],
                    }
                ),
                encoding="utf-8",
            )
            generator = lambda: [
                self.runner.Artifact(
                    Path("test/snapshot/sample/expected/result.json"),
                    "json",
                    {"value": 2},
                )
            ]
            with (
                mock.patch.object(self.runner, "ROOT", root),
                mock.patch.object(self.runner, "CATALOG", catalog),
                mock.patch.object(self.runner, "GENERATORS", {"sample": generator}),
                mock.patch.dict(os.environ, {"CI": ""}),
            ):
                result, stdout, stderr = self.invoke(
                    ["--update", "--suite", "sample"]
                )

            self.assertEqual(result, 0, stderr)
            self.assertEqual(stderr, "")
            self.assertLess(
                stdout.index("snapshot-diff: sample"),
                stdout.index("snapshot-validate: sample"),
            )
            self.assertIn('-  "value": 1', stdout)
            self.assertIn('+  "value": 2', stdout)
            self.assertEqual(
                json.loads(expected.read_text(encoding="utf-8")), {"value": 2}
            )

    def test_snapshot_paths_cannot_escape_the_repository(self) -> None:
        with self.assertRaisesRegex(
            self.runner.SnapshotFailure, "snapshot path must be normalized"
        ):
            self.runner.safe_repo_path("../outside.json")


if __name__ == "__main__":
    unittest.main()
