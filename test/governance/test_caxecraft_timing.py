from __future__ import annotations

import importlib.util
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]


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


class CaxecraftTimingTests(unittest.TestCase):
    def test_pinned_haxe_resolution_uses_the_haxerc_version(self) -> None:
        caxecraft = load_module(
            "caxecraft_pin_subject", ROOT / "examples/caxecraft/run.py"
        )
        with tempfile.TemporaryDirectory(prefix="hxc-haxe-pin-test-") as temporary:
            root = Path(temporary)
            scope = root / "scope"
            haxe_root = root / "haxe-root"
            installation = haxe_root / "versions" / "5.0.0-preview.1"
            scope.mkdir()
            (scope / ".haxerc").write_text(
                '{"version":"5.0.0-preview.1","resolveLibs":"scoped"}\n',
                encoding="utf-8",
            )
            installation.mkdir(parents=True)
            (installation / "std").mkdir()
            compiler = installation / ("haxe.exe" if os.name == "nt" else "haxe")
            compiler.write_text("test compiler\n", encoding="utf-8")
            compiler.chmod(0o755)

            observed = caxecraft.pinned_haxe_installation(
                scope_root=scope,
                environment={"HAXE_ROOT": str(haxe_root)},
            )

            self.assertEqual(observed.version, "5.0.0-preview.1")
            self.assertEqual(observed.compiler, compiler.resolve())
            self.assertEqual(
                observed.standard_library, (installation / "std").resolve()
            )

            (scope / ".haxerc").write_text(
                '{"version":"../4.3.7","resolveLibs":"scoped"}\n',
                encoding="utf-8",
            )
            with self.assertRaisesRegex(
                caxecraft.CaxecraftFailure, "unsafe or missing"
            ):
                caxecraft.pinned_haxe_installation(
                    scope_root=scope,
                    environment={"HAXE_ROOT": str(haxe_root)},
                )

    def test_server_context_uses_loopback_and_reaps_on_interrupt(self) -> None:
        caxecraft = load_module(
            "caxecraft_server_subject", ROOT / "examples/caxecraft/run.py"
        )
        installation = caxecraft.HaxeInstallation(
            "5.0.0-preview.1",
            Path("/test/haxe-5/haxe"),
            Path("/test/haxe-5/std"),
            Path("/test/haxelib"),
            None,
        )
        server = mock.Mock()
        server.poll.return_value = None
        server.wait.side_effect = [
            subprocess.TimeoutExpired(cmd="haxe server", timeout=5),
            0,
        ]
        with (
            mock.patch.object(
                caxecraft,
                "pinned_haxe_installation",
                return_value=installation,
            ),
            mock.patch.object(caxecraft, "verify_pinned_haxe"),
            mock.patch.object(caxecraft, "available_port", return_value=48123),
            mock.patch.object(caxecraft, "wait_for_server") as wait_for_server,
            mock.patch.object(
                caxecraft.subprocess, "Popen", return_value=server
            ) as popen,
        ):
            with self.assertRaises(KeyboardInterrupt):
                with caxecraft.haxe_compilation_server() as connection:
                    self.assertEqual(connection.endpoint, "127.0.0.1:48123")
                    self.assertIs(connection.installation, installation)
                    raise KeyboardInterrupt()

        command = popen.call_args.args[0]
        self.assertEqual(
            command,
            [
                "/test/haxe-5/haxe",
                "--server-listen",
                "127.0.0.1:48123",
            ],
        )
        self.assertNotIn("HAXE_NO_SERVER", popen.call_args.kwargs["env"])
        wait_for_server.assert_called_once_with(server, 48123)
        server.terminate.assert_called_once_with()
        server.kill.assert_called_once_with()

    def test_native_server_client_receives_only_resolved_arguments(self) -> None:
        caxecraft = load_module(
            "caxecraft_client_subject", ROOT / "examples/caxecraft/run.py"
        )
        installation = caxecraft.HaxeInstallation(
            "5.0.0-preview.1",
            Path("/test/haxe-5/haxe"),
            Path("/test/haxe-5/std"),
            Path("/test/haxelib"),
            None,
        )
        connection = caxecraft.HaxeServerConnection(
            "127.0.0.1:48123", installation
        )
        completed = subprocess.CompletedProcess([], 0, "ok", "")
        with (
            mock.patch.object(
                caxecraft,
                "resolve_haxe_arguments",
                return_value=("-cp", "src", "--custom-target", "c=/test/out"),
            ) as resolve,
            mock.patch.object(
                caxecraft.subprocess, "run", return_value=completed
            ) as run,
        ):
            observed = caxecraft.compile_target(
                Path("/test/out"), connect=connection
            )

        self.assertIs(observed, completed)
        resolve.assert_called_once()
        command = run.call_args.args[0]
        self.assertEqual(
            command,
            [
                "/test/haxe-5/haxe",
                "--connect",
                "127.0.0.1:48123",
                "-cp",
                "src",
                "--custom-target",
                "c=/test/out",
            ],
        )
        self.assertNotIn("--haxe-version", command)
        self.assertEqual(run.call_args.kwargs["cwd"], caxecraft.CASE)
        self.assertEqual(
            run.call_args.kwargs["env"]["HAXE_VERSION"], "5.0.0-preview.1"
        )

    def test_full_recorder_matches_the_toolchain_phase_contract(self) -> None:
        caxecraft = load_module(
            "caxecraft_timing_subject", ROOT / "examples/caxecraft/run.py"
        )
        toolchain = load_module(
            "toolchain_timing_subject", ROOT / "scripts/ci/run_toolchain_shard.py"
        )
        recorder = caxecraft.TimingRecorder()
        for name, requests in toolchain.CAXECRAFT_FULL_PHASES:
            with recorder.phase(name, haxe_requests=requests):
                pass

        report = recorder.report(mode="full", outcome="passed")
        self.assertIs(
            toolchain.validate_caxecraft_timing_report(report),
            report,
        )
        self.assertEqual(report["summary"], {"haxeRequests": 12})

    def test_failed_phase_is_retained_in_a_failure_prefix(self) -> None:
        caxecraft = load_module(
            "caxecraft_failure_timing_subject", ROOT / "examples/caxecraft/run.py"
        )
        recorder = caxecraft.TimingRecorder()
        with self.assertRaisesRegex(RuntimeError, "expected"):
            with recorder.phase("backend-render", haxe_requests=3):
                raise RuntimeError("expected")

        report = recorder.report(mode="full", outcome="failed")
        self.assertEqual(report["outcome"], "failed")
        self.assertEqual(report["summary"], {"haxeRequests": 3})
        self.assertEqual(report["phases"][0]["outcome"], "failed")

    def test_public_toolchain_schema_keeps_the_ordered_phase_contract(self) -> None:
        toolchain = load_module(
            "toolchain_schema_subject", ROOT / "scripts/ci/run_toolchain_shard.py"
        )
        schema = json.loads(
            (ROOT / "docs/specs/toolchain-timing.schema.json").read_text(
                encoding="utf-8"
            )
        )
        timing = schema["$defs"]["caxecraftTiming"]
        references = timing["properties"]["phases"]["prefixItems"]
        observed = []
        for reference in references:
            definition_name = reference["$ref"].removeprefix("#/$defs/")
            definition = schema["$defs"][definition_name]
            observed.append(
                (
                    definition["properties"]["name"]["const"],
                    definition["properties"]["haxeRequests"]["const"],
                )
            )
        self.assertEqual(tuple(observed), toolchain.CAXECRAFT_FULL_PHASES)
        self.assertFalse(timing["properties"]["phases"]["items"])


if __name__ == "__main__":
    unittest.main()
