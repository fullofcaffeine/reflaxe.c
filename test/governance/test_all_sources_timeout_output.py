"""Keep partial compiler diagnostics visible when the all-sources owner times out."""

from __future__ import annotations

import contextlib
import importlib.util
import io
import subprocess
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]


class AllSourcesTimeoutOutputTests(unittest.TestCase):
    def test_timeout_retains_labeled_streams_and_original_failure(self) -> None:
        spec = importlib.util.spec_from_file_location(
            "all_sources_timeout_subject", ROOT / "test/all_sources/run.py"
        )
        assert spec is not None and spec.loader is not None
        owner = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(owner)
        for stdout, stderr, expected_stdout, expected_stderr in (
            ("Parsed First.hx\n", "typing diagnostic\n", "Parsed First.hx\n", "typing diagnostic\n"),
            (b"Parsed Second.hx\n", b"partial \xff diagnostic\n", "Parsed Second.hx\n", "partial \ufffd diagnostic\n"),
            ("", "", "", ""),
            (None, None, "", ""),
        ):
            with self.subTest(stdout=stdout, stderr=stderr):
                timeout = subprocess.TimeoutExpired(
                    ["synthetic-haxe"], 30, output=stdout, stderr=stderr
                )
                captured = io.StringIO()
                with (
                    mock.patch.object(owner.shutil, "which", return_value="synthetic-haxe"),
                    mock.patch.object(owner, "run_bounded_process", side_effect=timeout) as run,
                    contextlib.redirect_stderr(captured),
                ):
                    result = owner.main()
                self.assertEqual(result, 1)
                run.assert_called_once()
                self.assertEqual(run.call_args.kwargs["timeout"], 30)
                self.assertEqual(run.call_args.kwargs["phase"], "compiler-source Eval graph")
                self.assertTrue(run.call_args.kwargs["capture_output"])
                self.assertTrue(run.call_args.kwargs["text"])
                self.assertEqual(run.call_args.args[0][1:], [str(owner.HXML), "--interp", "-v"])
                self.assertEqual(
                    captured.getvalue(),
                    f"all-sources: ERROR: {timeout}\n"
                    f"partial stdout:\n{expected_stdout}\n"
                    f"partial stderr:\n{expected_stderr}\n",
                )


if __name__ == "__main__":
    unittest.main()
