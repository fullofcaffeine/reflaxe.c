"""Keep partial compiler diagnostics visible when the all-sources owner times out."""

from __future__ import annotations

import contextlib
import importlib.util
import io
import json
import subprocess
import unittest
import tempfile
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]


class AllSourcesTimeoutOutputTests(unittest.TestCase):
    def load_owner(self):
        spec = importlib.util.spec_from_file_location(
            "all_sources_log_subject", ROOT / "test/all_sources/run.py"
        )
        assert spec is not None and spec.loader is not None
        owner = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(owner)
        return owner

    def successful_results(self, owner):
        return [
            subprocess.CompletedProcess([], 0, "\n".join(owner.MACRO_BRANCH_MARKERS) + "\nall-sources: OK\n", "first stderr\n"),
            subprocess.CompletedProcess([], 0, "hxc-sources: OK\n", "second stderr\n"),
            subprocess.CompletedProcess([], 1, "Typing macro c.Init.init\n", f"{owner.LOWERING_DIAGNOSTIC_ID} {owner.LOWERING_DETAIL} {owner.LOWERING_SOURCE}\n"),
        ]

    def test_optional_log_preserves_successful_children_without_console_noise(self):
        owner = self.load_owner()
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary) / "compiler.jsonl"
            results = self.successful_results(owner)
            stdout, stderr = io.StringIO(), io.StringIO()
            with (
                mock.patch.object(owner.shutil, "which", return_value="synthetic-haxe"),
                mock.patch.object(owner, "owned_haxe_sources", return_value=[]),
                mock.patch.object(owner, "run_bounded_process", side_effect=results) as run,
                contextlib.redirect_stdout(stdout), contextlib.redirect_stderr(stderr),
            ):
                code = owner.main(["--compiler-log", str(destination)])
            self.assertEqual(code, 0)
            self.assertEqual(stderr.getvalue(), "")
            self.assertNotIn("Typing", stdout.getvalue())
            records = [json.loads(line) for line in destination.read_text().splitlines()]
            self.assertEqual(len(records), 3)
            for record, result, call in zip(records, results, run.call_args_list):
                self.assertEqual(record["command"], call.args[0])
                self.assertEqual(record["label"], call.kwargs["phase"])
                self.assertEqual(record["returncode"], result.returncode)
                self.assertFalse(record["timedOut"])
                self.assertEqual(record["stdout"], result.stdout)
                self.assertEqual(record["stderr"], result.stderr)
                self.assertEqual(call.kwargs["timeout"], 30)
            # A second invocation cannot overwrite evidence or start a compiler.
            original = destination.read_bytes()
            with mock.patch.object(owner.shutil, "which", return_value="synthetic-haxe"), mock.patch.object(owner, "run_bounded_process") as run, contextlib.redirect_stderr(io.StringIO()):
                self.assertEqual(owner.main(["--compiler-log", str(destination)]), 1)
            run.assert_not_called()
            self.assertEqual(destination.read_bytes(), original)

    def test_timeout_log_keeps_prior_success_and_partial_streams_once(self):
        owner = self.load_owner()
        timeout = subprocess.TimeoutExpired(["synthetic-haxe"], 30, output=b"Parsed Last.hx\n", stderr=b"partial failure\n")
        with tempfile.TemporaryDirectory() as temporary:
            destination = Path(temporary) / "compiler.jsonl"
            with (
                mock.patch.object(owner.shutil, "which", return_value="synthetic-haxe"),
                mock.patch.object(owner, "owned_haxe_sources", return_value=[]),
                mock.patch.object(owner, "run_bounded_process", side_effect=[*self.successful_results(owner)[:2], timeout]) as run,
                contextlib.redirect_stderr(io.StringIO()) as stderr,
            ):
                self.assertEqual(owner.main(["--compiler-log", str(destination)]), 1)
            records = [json.loads(line) for line in destination.read_text().splitlines()]
            self.assertEqual(len(records), 3)
            self.assertEqual([r["returncode"] for r in records], [0, 0, None])
            self.assertEqual(records[-1]["command"], run.call_args.args[0])
            self.assertTrue(records[-1]["timedOut"])
            self.assertEqual(records[-1]["stdout"], "Parsed Last.hx\n")
            self.assertEqual(records[-1]["stderr"], "partial failure\n")
            self.assertIn("partial failure", stderr.getvalue())

    def test_log_failure_never_hides_compiler_failure_or_reports_success(self):
        owner = self.load_owner()
        log = mock.Mock()
        log.write.side_effect = OSError("full evidence volume")
        timeout = subprocess.TimeoutExpired(["synthetic-haxe"], 30, output="partial", stderr="error")
        for result, exception in (
            (subprocess.CompletedProcess([], 0, "ok", ""), OSError),
            (subprocess.CompletedProcess([], 17, "partial", "error"), owner.AllSourcesFailure),
            (timeout, subprocess.TimeoutExpired),
        ):
            with self.subTest(result=result), mock.patch.object(owner, "run_bounded_process", side_effect=([result] if not isinstance(result, Exception) else result)), contextlib.redirect_stderr(io.StringIO()):
                with self.assertRaises(exception) as failure:
                    owner.run_haxe(owner.HXML, [], expected_code=0, label="probe", compiler_log=log)
                if result is timeout:
                    self.assertIs(failure.exception, timeout)

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
