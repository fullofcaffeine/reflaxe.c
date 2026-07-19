from __future__ import annotations

import importlib.util
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
RUNNER = ROOT / "scripts/ci/run_toolchain_shard.py"


def load_runner():
    spec = importlib.util.spec_from_file_location("toolchain_shard_subject", RUNNER)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {RUNNER}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


class ToolchainShardTests(unittest.TestCase):
    def setUp(self) -> None:
        self.runner = load_runner()

    def test_actual_partition_and_local_isolation_are_exact(self) -> None:
        scripts = self.runner.load_scripts()
        canonical = self.runner.validate_partition(scripts)
        self.assertEqual(len(canonical), 38)
        self.assertEqual(tuple(self.runner.SHARDS), self.runner.SHARD_ORDER)
        self.assertEqual(
            tuple(self.runner.LOCAL_PARALLEL_ISOLATION), self.runner.SHARD_ORDER
        )
        self.assertEqual(canonical[-1], "snapshots:catalog")
        self.assertNotIn("snapshots:check", canonical)

    def test_timing_report_is_ordered_and_path_free(self) -> None:
        completed = [
            self.runner.subprocess.CompletedProcess([], 0),
            self.runner.subprocess.CompletedProcess([], 0),
        ]
        clock = iter((0, 1_000_000, 11_000_000, 12_000_000, 32_000_000, 35_000_000))
        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(self.runner, "SHARDS", {"sample": ("one", "two")}),
                mock.patch.object(self.runner.subprocess, "run", side_effect=completed),
                mock.patch.object(
                    self.runner.time, "monotonic_ns", side_effect=lambda: next(clock)
                ),
            ):
                self.runner.run_shard(
                    "sample",
                    {"one": "first", "two": "second"},
                    timing_report=report_path,
                    stream=io.StringIO(),
                )
            report = json.loads(report_path.read_text(encoding="utf-8"))

        self.assertEqual(report["schemaVersion"], 1)
        self.assertEqual(report["shard"], "sample")
        self.assertEqual(report["outcome"], "passed")
        self.assertEqual(report["durationMs"], 35)
        self.assertEqual(
            report["commands"],
            [
                {
                    "script": "one",
                    "outcome": "passed",
                    "exitCode": 0,
                    "durationMs": 10,
                },
                {
                    "script": "two",
                    "outcome": "passed",
                    "exitCode": 0,
                    "durationMs": 20,
                },
            ],
        )
        self.assertNotIn(str(ROOT), json.dumps(report))

    def test_failure_still_writes_the_completed_timing_prefix(self) -> None:
        clock = iter((0, 1_000_000, 6_000_000, 8_000_000))
        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(self.runner, "SHARDS", {"sample": ("broken",)}),
                mock.patch.object(
                    self.runner.subprocess,
                    "run",
                    return_value=self.runner.subprocess.CompletedProcess([], 7),
                ),
                mock.patch.object(
                    self.runner.time, "monotonic_ns", side_effect=lambda: next(clock)
                ),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure, "exit 7"
                ):
                    self.runner.run_shard(
                        "sample",
                        {"broken": "false"},
                        timing_report=report_path,
                        stream=io.StringIO(),
                    )
            report = json.loads(report_path.read_text(encoding="utf-8"))

        self.assertEqual(report["outcome"], "failed")
        self.assertEqual(report["commands"][0]["exitCode"], 7)
        self.assertEqual(report["commands"][0]["durationMs"], 5)
        self.assertEqual(report["durationMs"], 8)

    def test_parallel_job_count_is_bounded(self) -> None:
        self.assertEqual(self.runner.requested_jobs(1), 1)
        self.assertEqual(self.runner.requested_jobs(4), 4)
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "between 1 and 4"
        ):
            self.runner.requested_jobs(5)


if __name__ == "__main__":
    unittest.main()
