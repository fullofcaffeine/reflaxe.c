from __future__ import annotations

import contextlib
import importlib.util
import io
import sys
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
RUNNER = ROOT / "scripts/ci/runtime_smoke.py"


def load_runner():
    spec = importlib.util.spec_from_file_location("runtime_smoke_policy_subject", RUNNER)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {RUNNER}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


class RuntimeSmokePolicyTests(unittest.TestCase):
    def setUp(self) -> None:
        self.runner = load_runner()

    def invoke_without_tools(self, arguments: list[str]) -> tuple[int, str, str]:
        stdout = io.StringIO()
        stderr = io.StringIO()
        with (
            mock.patch.object(self.runner.shutil, "which", return_value=None),
            contextlib.redirect_stdout(stdout),
            contextlib.redirect_stderr(stderr),
        ):
            result = self.runner.main(arguments)
        return result, stdout.getvalue(), stderr.getvalue()

    def test_missing_explicit_toolchain_is_required_failure(self) -> None:
        result, stdout, stderr = self.invoke_without_tools(["--toolchain", "gcc"])
        self.assertEqual(result, 1)
        self.assertNotIn("native-smoke: OK", stdout + stderr)
        self.assertIn("required toolchain gcc: missing gcc, g++", stderr)

    def test_auto_mode_reports_optional_skips_but_cannot_pass_without_a_lane(self) -> None:
        result, stdout, stderr = self.invoke_without_tools([])
        self.assertEqual(result, 1)
        self.assertIn("SKIP optional gcc: missing gcc, g++", stdout)
        self.assertIn("SKIP optional clang: missing clang, clang++", stdout)
        self.assertIn("a native gate cannot be reported as passed", stderr)
        self.assertNotIn("native-smoke: OK", stdout + stderr)


if __name__ == "__main__":
    unittest.main()
