from __future__ import annotations

import base64
import contextlib
import io
import tempfile
from types import SimpleNamespace
import importlib.util
import json
import subprocess
import sys
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
CASE = ROOT / "examples/caxecraft"


def load_module(name: str, path: Path):
    spec = importlib.util.spec_from_file_location(name, path)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {path}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


def progress_line(
    state: str,
    *,
    phase: str | None = None,
    profile: str | None = None,
    build_mode: str | None = None,
    status: str | None = None,
) -> str:
    return "HXC_PHASE_PROGRESS\t" + json.dumps(
        {
            "schemaVersion": 1,
            "state": state,
            "phase": phase,
            "profile": profile,
            "buildMode": build_mode,
            "status": status,
        },
        separators=(",", ":"),
    )


class CaxecraftCompilerProgressTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        with mock.patch.object(sys, "path", [str(CASE), *sys.path]):
            cls.progress = load_module(
                "caxecraft_compiler_progress_subject",
                ROOT / "scripts/test/compiler_progress.py",
            )
            cls.haxe_tests = load_module(
                "caxecraft_native_progress_subject",
                CASE / "run_haxe_c_test.py",
            )
            cls.play = load_module(
                "caxecraft_compiler_progress_play_subject",
                CASE / "play.py",
            )

    def test_native_timeout_preserves_partial_stdout_and_stderr(self) -> None:
        for binary in (False, True):
            stdout = "partial native output\n"
            stderr = "native test phase: probe main\n"
            timeout = subprocess.TimeoutExpired(
                ["native-test"], 40,
                output=stdout.encode() if binary else stdout,
                stderr=stderr.encode() if binary else stderr,
            )
            with self.subTest(binary=binary), mock.patch.object(
                self.haxe_tests, "run_bounded_process", side_effect=timeout
            ):
                with self.assertRaises(self.haxe_tests.HaxeCTestFailure) as failure:
                    self.haxe_tests.run(
                        ["native-test"], cwd=ROOT, timeout=40, label="native test"
                    )
            message = str(failure.exception)
            self.assertIn("timed out after 40 seconds", message)
            self.assertIn(stdout.strip(), message)
            self.assertIn(stderr.strip(), message)
            self.assertNotIn("last active compiler phase", message)

    def test_native_success_preserves_the_stdout_comparison_envelope(self) -> None:
        oracle = "0\n11\n12\n13\n14\n15\n16\n17\n"
        expected = subprocess.CompletedProcess(
            ["native-test"], 0, oracle, "native test phase: complete\n"
        )
        with mock.patch.object(
            self.haxe_tests, "run_bounded_process", return_value=expected
        ):
            actual = self.haxe_tests.run(
                ["native-test"], cwd=ROOT, timeout=40, label="native test"
            )
        self.assertIs(actual, expected)
        self.assertEqual(actual.stdout, oracle)
        self.haxe_tests.validate_oracle(
            self.haxe_tests.CASES["runtime-level-loader"], actual.stdout
        )

    def test_nested_progress_names_the_last_active_phase(self) -> None:
        lines = "\n".join(
            (
                progress_line("request-started"),
                progress_line("phase-started", phase="target pipeline"),
                progress_line(
                    "request-configured",
                    profile="portable",
                    build_mode="debug",
                ),
                progress_line("phase-started", phase="semantic lowering"),
                progress_line("phase-started", phase="HxcIR validation"),
                progress_line("phase-completed", phase="HxcIR validation"),
            )
        )
        _, records = self.progress.partition_compiler_progress(lines.encode())
        self.assertEqual(
            self.progress.summarize_compiler_progress(records),
            "last active compiler phase: semantic lowering (portable/debug)",
        )

    def test_capture_region_completion_restores_parent_before_abort(self) -> None:
        lines = [
            progress_line("request-started"),
            progress_line("phase-started", phase="typed input capture"),
            progress_line("phase-started", phase="typed input: declaration normalization"),
        ]
        _, records = self.progress.partition_compiler_progress("\n".join(lines))
        self.assertEqual(self.progress.summarize_compiler_progress(records),
                         "last active compiler phase: typed input: declaration normalization")
        lines.append(progress_line("phase-completed", phase="typed input: declaration normalization"))
        _, records = self.progress.partition_compiler_progress("\n".join(lines))
        self.assertEqual(self.progress.summarize_compiler_progress(records),
                         "last active compiler phase: typed input capture")
        lines.append(progress_line("request-aborted", status="failed"))
        _, records = self.progress.partition_compiler_progress("\n".join(lines))
        self.assertEqual(self.progress.summarize_compiler_progress(records),
                         "compiler request aborted: failed")

    def test_process_detail_strips_records_and_preserves_diagnostic(self) -> None:
        stdout = "\n".join(
            (
                progress_line("request-started"),
                progress_line("phase-started", phase="configuration and contracts"),
                progress_line("request-aborted", status="failed"),
            )
        )
        self.assertEqual(
            self.progress.compiler_process_detail(stdout, "HXC0003: invalid option\n"),
            "compiler progress: compiler request aborted: failed\nHXC0003: invalid option",
        )

    def test_generic_launcher_timeout_adds_partial_compiler_status(self) -> None:
        partial = "\n".join(
            (
                progress_line("request-started"),
                progress_line("phase-started", phase="target pipeline"),
                progress_line(
                    "request-configured",
                    profile="portable",
                    build_mode="debug",
                ),
                progress_line("phase-started", phase="CAST body construction"),
            )
        ).encode()
        timeout = subprocess.TimeoutExpired(
            ["haxe"],
            120,
            output=partial,
            stderr=b"",
        )
        with mock.patch.object(self.play.subprocess, "run", side_effect=timeout):
            with self.assertRaisesRegex(
                self.play.PlayFailure,
                r"timed out after 120 seconds; last active compiler phase: CAST body construction \(portable/debug\)",
            ):
                self.play.run(
                    ["haxe"],
                    cwd=ROOT,
                    timeout=120,
                    label="Caxecraft Haxe-to-C compile",
                )

    def test_non_compiler_timeout_keeps_the_existing_message(self) -> None:
        timeout = subprocess.TimeoutExpired(["clang"], 30)
        with mock.patch.object(self.play.subprocess, "run", side_effect=timeout):
            with self.assertRaisesRegex(
                self.play.PlayFailure,
                r"Command '\['clang'\]' timed out after 30 seconds$",
            ):
                self.play.run(
                    ["clang"],
                    cwd=ROOT,
                    timeout=30,
                    label="native compile",
                )

    def test_compiler_timeout_before_typed_capture_names_the_frontend(self) -> None:
        timeout = subprocess.TimeoutExpired(["haxe"], 120)
        with mock.patch.object(self.play.subprocess, "run", side_effect=timeout):
            with self.assertRaisesRegex(
                self.play.PlayFailure,
                r"no haxe\.c phase marker; request remained in Haxe frontend or server startup$",
            ):
                self.play.run(
                    ["haxe"],
                    cwd=ROOT,
                    timeout=120,
                    label="Caxecraft Haxe-to-C compile",
                )

    def test_compile_requests_lightweight_progress_without_full_profiling(self) -> None:
        captured: list[str] = []

        def fake_run(
            arguments: list[str],
            *,
            cwd: Path,
            timeout: int,
            label: str,
        ) -> subprocess.CompletedProcess[str]:
            del cwd, timeout, label
            captured.extend(arguments)
            return subprocess.CompletedProcess(arguments, 0, "", "")

        with (
            mock.patch.object(self.play, "run", side_effect=fake_run),
            mock.patch.object(self.play, "validate_compiled_haxe", return_value={}),
            mock.patch.object(self.play, "validate_content_platform_output"),
            mock.patch("builtins.print"),
        ):
            self.play.compile_haxe(
                ROOT / "generated",
                layout="split",
                platform_name="macos",
                raylib_configuration="memory-software",
            )
        self.assertIn("reflaxe_c_phase_progress", captured)
        self.assertNotIn("reflaxe_c_phase_timing", captured)



class CompilerEvidenceTests(unittest.TestCase):
    """Observe real request boundaries with subprocesses and server ownership mocked."""

    @classmethod
    def setUpClass(cls) -> None:
        with mock.patch.object(sys, "path", [str(CASE), *sys.path]):
            cls.play = load_module("compiler_evidence_play_subject", CASE / "play.py")

    def compile(self, *, log=None, cache=False, connected=False, results=None,
                validation_error=None, owned=False):
        installation = SimpleNamespace(compiler=Path("/fixture/haxe"))
        process = SimpleNamespace(pid=123, start_identity="start-one", command_identity="command-one", cookie_sha256="cookie-one")
        replacement_process = SimpleNamespace(pid=456, start_identity="start-two", command_identity="command-two", cookie_sha256="cookie-two")
        lease = SimpleNamespace(connection=SimpleNamespace(installation=installation, endpoint="127.0.0.1:6001"),
                                owned=owned, process=process if owned else None, compatibility_sha256="compat" if owned else None)
        replacement = SimpleNamespace(connection=SimpleNamespace(installation=installation, endpoint="127.0.0.1:6002"),
                                      owned=True, process=replacement_process, compatibility_sha256="compat")
        owner = mock.Mock()
        owner.state_root = Path("/fixture/server")
        owner.compatibility = {"canonicalServerStateRoot": str(owner.state_root.resolve())}
        owner.prepare_source_request.side_effect = [(lease, ()), (replacement, ())]
        owner.restart_after_transport_failure.return_value = replacement
        results = results if results is not None else [subprocess.CompletedProcess([], 0, "compiler stdout\n", "compiler stderr\n")]
        with (
            mock.patch.object(self.play, "development_tool", return_value="/fixture/shim"),
            mock.patch.object(self.play, "resolve_haxe_arguments", side_effect=lambda arguments, **kw: tuple(arguments)),
            mock.patch.object(self.play, "pinned_haxe_environment", return_value={}),
            mock.patch.object(self.play, "haxe_game_source_snapshot", return_value={}),
            mock.patch.object(self.play, "haxe_server_compatibility", return_value={}),
            mock.patch.object(self.play, "validate_haxe_source_roots"),
            mock.patch.object(self.play.subprocess, "run", side_effect=results) as run,
            mock.patch.object(self.play, "transport_failed", side_effect=lambda result, lease: result.returncode == 17),
            mock.patch.object(self.play, "validate_compiled_haxe", return_value={}, side_effect=validation_error),
            mock.patch.object(self.play, "validate_content_platform_output"),
            contextlib.redirect_stdout(io.StringIO()) as console,
        ):
            self.last_run, self.last_owner, self.last_console = run, owner, console
            self.play.compile_haxe(
                Path("/fixture/generated"), layout="split", platform_name="macos",
                raylib_configuration="desktop", server_lease=lease if connected else None,
                server_owner=owner if owned else None, compiler_log=log,
                compiler_cache_report=cache,
            )
        return run.call_args_list

    def test_log_only_keeps_default_argv_and_quiet_success(self):
        calls = self.compile()
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "compile.jsonl"
            with path.open("x") as log:
                logged_calls = self.compile(log=log)
            row = json.loads(path.read_text())
        self.assertEqual(calls[0].args, logged_calls[0].args)
        self.assertNotIn("compiler stdout", self.last_console.getvalue())
        self.assertEqual(row["command"], logged_calls[0].args[0])
        self.assertEqual(row["cwd"], str(ROOT))
        self.assertEqual(row["stdout"], "compiler stdout\n")
        self.assertEqual(row["stderr"], "compiler stderr\n")
        self.assertEqual(row["stdoutKind"], "text")
        self.assertEqual(row["returncode"], 0)
        self.assertEqual(row["configuredTimeoutSeconds"], 120)
        self.assertEqual(row["attempt"], 1)
        self.assertIsNone(row["server"])
        self.assertNotIn("environment", row)

    def test_cache_reports_and_each_retry_keep_exact_request_identity(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "compile.jsonl"
            with path.open("x") as log:
                calls = self.compile(log=log, cache=True, connected=True, owned=True, results=[
                    subprocess.CompletedProcess([], 17, "transport", "failed"),
                    subprocess.CompletedProcess([], 0, "cache records", ""),
                ])
            rows = [json.loads(line) for line in path.read_text().splitlines()]
        self.assertEqual([row["attempt"] for row in rows], [1, 2])
        self.assertEqual([row["returncode"] for row in rows], [17, 0])
        self.assertEqual([row["server"]["endpoint"] for row in rows], ["127.0.0.1:6001", "127.0.0.1:6002"])
        self.assertTrue(all(row["server"]["owned"] for row in rows))
        self.assertEqual([row["server"]["process"] for row in rows], [
            {"pid": 123, "startIdentity": "start-one", "commandIdentity": "command-one", "cookieSha256": "cookie-one"},
            {"pid": 456, "startIdentity": "start-two", "commandIdentity": "command-two", "cookieSha256": "cookie-two"},
        ])
        self.assertEqual(rows[0]["stdout"], "transport")
        self.assertEqual(rows[1]["stdout"], "cache records")
        for row, call in zip(rows, calls):
            self.assertEqual(row["command"], call.args[0])
            self.assertEqual(call.kwargs["timeout"], 120)
            for define in ("reflaxe_c_body_function_replay_cache_report", "reflaxe_c_control_flow_plan_cache_report"):
                self.assertEqual(row["command"].count(define), 1)
            self.assertNotIn("reflaxe_c_phase_timing", row["command"])
        self.last_owner.finish_source_request.assert_called_once()

    def test_timeout_preserves_raw_bytes_once_for_both_transports(self):
        for connected in (False, True):
            with self.subTest(connected=connected), tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / "compile.jsonl"
                timeout = subprocess.TimeoutExpired(["haxe"], 120, output=b"partial \xff", stderr=None)
                with path.open("x") as log, self.assertRaisesRegex(self.play.PlayFailure, "timed out"):
                    self.compile(log=log, connected=connected, results=timeout)
                rows = [json.loads(line) for line in path.read_text().splitlines()]
                self.assertEqual(len(rows), 1)
                row = rows[0]
                self.assertTrue(row["timedOut"])
                self.assertIsNone(row["returncode"])
                self.assertEqual(row["stdoutKind"], "bytes")
                self.assertEqual(base64.b64decode(row["stdoutBase64"]), b"partial \xff")
                self.assertEqual(row["stderrKind"], "none")
                self.assertIsNone(row["stderrBase64"])
                if connected:
                    self.assertFalse(row["server"]["owned"])
                self.last_run.assert_called_once()

    def test_log_failure_keeps_nonzero_or_timeout_as_primary(self):
        for connected in (False, True):
            for result, detail in ((subprocess.CompletedProcess([], 17, "original stdout", "original stderr"), "exit 17"),
                                   (subprocess.TimeoutExpired(["haxe"], 120, output=b"original partial"), "timed out"),
                                   (subprocess.CompletedProcess([], 0, "", ""), "retain compiler log")):
                for operation in ("write", "flush"):
                    with self.subTest(connected=connected, detail=detail, operation=operation), contextlib.redirect_stderr(io.StringIO()):
                        log = mock.Mock()
                        getattr(log, operation).side_effect = OSError("full evidence volume")
                        with self.assertRaisesRegex(self.play.PlayFailure, detail):
                            self.compile(log=log, connected=connected, results=result if isinstance(result, Exception) else [result])
                        self.last_owner.finish_source_request.assert_not_called()

    def test_close_failure_precedes_owned_source_history_completion(self):
        log = mock.Mock()
        log.close.side_effect = OSError("close failed")
        with self.assertRaisesRegex(OSError, "close failed"):
            self.compile(log=log, connected=True, owned=True)
        self.last_owner.finish_source_request.assert_not_called()

    def test_validation_failure_keeps_record_without_source_history(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "compile.jsonl"
            with path.open("x") as log, self.assertRaisesRegex(self.play.PlayFailure, "invalid project"):
                self.compile(log=log, connected=True, owned=True, validation_error=self.play.PlayFailure("invalid project"))
            self.assertEqual(json.loads(path.read_text())["returncode"], 0)
        self.last_owner.finish_source_request.assert_not_called()

    def test_cache_report_requires_log_before_launcher_work(self):
        with mock.patch.object(self.play, "run_play") as run, contextlib.redirect_stderr(io.StringIO()) as stderr:
            self.assertEqual(self.play.main(["--compile-only", "--compiler-cache-report"]), 1)
        run.assert_not_called()
        self.assertIn("requires --compiler-log", stderr.getvalue())

    def test_reservation_rejects_existing_or_output_owned_log(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            existing = root / "existing.jsonl"
            existing.write_text("keep")
            for path in (existing, root / "output" / "compile.jsonl"):
                with self.subTest(path=path), mock.patch.object(self.play, "run_play") as run, contextlib.redirect_stderr(io.StringIO()):
                    self.assertEqual(self.play.main(["--compile-only", "--output-root", str(root / "output"), "--compiler-log", str(path)]), 1)
                run.assert_not_called()
            self.assertEqual(existing.read_text(), "keep")
            self.assertFalse((root / "output").exists())

    def test_cache_flags_on_cold_request_and_missing_sink_rejection(self):
        log = io.StringIO()
        calls = self.compile(log=log, cache=True)
        self.assertTrue(log.closed)
        defines = calls[0].args[0]
        self.assertEqual(defines.count("reflaxe_c_body_function_replay_cache_report"), 1)
        self.assertEqual(defines.count("reflaxe_c_control_flow_plan_cache_report"), 1)
        self.assertNotIn("reflaxe_c_phase_timing", defines)
        with self.assertRaisesRegex(self.play.PlayFailure, "requires --compiler-log"):
            self.compile(cache=True)
        self.last_run.assert_not_called()

    def test_cli_reserves_log_before_delegation_and_no_compile_leaves_it_empty(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "compile.jsonl"
            def no_compile(args, *, compiler_log):
                self.assertTrue(path.is_file())
                self.assertFalse(compiler_log.closed)
                self.assertTrue(args.compiler_cache_report)
                return 0
            with mock.patch.object(self.play, "run_play", side_effect=no_compile):
                self.assertEqual(self.play.main(["--build-only", "--compiler-log", str(path), "--compiler-cache-report"]), 0)
            self.assertEqual(path.read_bytes(), b"")
        with mock.patch.object(self.play, "run_play") as run, contextlib.redirect_stderr(io.StringIO()):
            self.assertEqual(self.play.main(["--check-snapshots", "--compiler-log", "/fixture/snapshots.jsonl"]), 1)
        run.assert_not_called()

    def test_no_compile_and_close_errors_do_not_invent_fresh_evidence(self):
        for code in (0, 1, 130):
            log = mock.Mock(closed=False)
            log.close.side_effect = OSError("close failed")
            with self.subTest(code=code), mock.patch.object(Path, "open", return_value=log), mock.patch.object(self.play, "run_play", return_value=code), contextlib.redirect_stderr(io.StringIO()):
                result = self.play.main(["--build-only", "--compiler-log", "/fixture/evidence.jsonl"])
            self.assertEqual(result, 1 if code == 0 else code)
            log.write.assert_not_called()


if __name__ == "__main__":
    unittest.main()
