from __future__ import annotations

import importlib.util
import json
import sys
import unittest
from pathlib import Path


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
