from __future__ import annotations

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
                CASE / "compiler_progress.py",
            )
            cls.play = load_module(
                "caxecraft_compiler_progress_play_subject",
                CASE / "play.py",
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


if __name__ == "__main__":
    unittest.main()
