from __future__ import annotations

import importlib.util
import io
import json
import math
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
        self.assertEqual(len(canonical), 60)
        self.assertEqual(tuple(self.runner.SHARDS), self.runner.SHARD_ORDER)
        self.assertEqual(
            tuple(self.runner.LOCAL_PARALLEL_ISOLATION), self.runner.SHARD_ORDER
        )
        self.assertEqual(canonical[-1], "snapshots:catalog")
        self.assertNotIn("snapshots:check", canonical)

    def test_required_instruction_links_are_safe_fingerprint_inputs(self) -> None:
        tracked = self.runner.git_bytes(
            ["ls-files", "-z", "--", *self.runner.RELEVANT_UNTRACKED_ROOTS]
        )
        tracked_names = {
            name.decode("utf-8", errors="strict")
            for name in tracked.split(b"\0")
            if name
        }
        self.assertTrue(
            self.runner.is_reviewed_instruction_link(
                "examples/caxecraft/CLAUDE.md", tracked_names
            )
        )
        self.assertFalse(
            self.runner.is_reviewed_instruction_link("AGENTS.md", tracked_names)
        )
        self.assertEqual(len(self.runner.relevant_worktree_digest()), 64)

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

    def test_resource_aware_worker_selection(self) -> None:
        idle = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=1.0,
            load_was_probed=True,
        )
        moderate = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=7.0,
            load_was_probed=True,
        )
        loaded = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=13.0,
            load_was_probed=True,
        )
        unavailable = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=None,
            load_was_probed=True,
        )
        invalid = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=math.nan,
            load_was_probed=True,
        )

        self.assertEqual(idle.jobs, 4)
        self.assertEqual(moderate.jobs, 2)
        self.assertEqual(loaded.jobs, 1)
        self.assertEqual(unavailable.jobs, 2)
        self.assertEqual(unavailable.source, "automatic-fallback")
        self.assertEqual(invalid.jobs, 2)

    def test_explicit_worker_precedence_and_malformed_environment(self) -> None:
        cli = self.runner.resolve_job_selection(
            4, environment={"HXC_TEST_JOBS": "1"}, logical_cpus=2
        )
        environment = self.runner.resolve_job_selection(
            None, environment={"HXC_TEST_JOBS": "3"}, logical_cpus=2
        )
        self.assertEqual((cli.jobs, cli.source), (4, "--jobs"))
        self.assertEqual(
            (environment.jobs, environment.source), (3, "HXC_TEST_JOBS")
        )
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "must be an integer"
        ):
            self.runner.resolve_job_selection(
                None, environment={"HXC_TEST_JOBS": "many"}, logical_cpus=8
            )
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "between 1 and 4"
        ):
            self.runner.resolve_job_selection(
                None, environment={"HXC_TEST_JOBS": "0"}, logical_cpus=8
            )

    def test_arbitrary_inherited_environment_drift_invalidates_evidence(self) -> None:
        baseline = self.runner.environment_digest(
            {"PATH": "/tools", "PYTHONOPTIMIZE": "0", "HXC_TEST_JOBS": "1"}
        )
        changed_semantics = self.runner.environment_digest(
            {"PATH": "/tools", "PYTHONOPTIMIZE": "1", "HXC_TEST_JOBS": "1"}
        )
        changed_scheduling_only = self.runner.environment_digest(
            {"PATH": "/tools", "PYTHONOPTIMIZE": "0", "HXC_TEST_JOBS": "4"}
        )
        self.assertNotEqual(baseline, changed_semantics)
        self.assertEqual(baseline, changed_scheduling_only)

    def sample_inputs(self, shard: str, suffix: str = "") -> dict[str, str]:
        return {
            "stagedTree": "a" * 40 + suffix,
            "worktreeDigest": "b" * 64 + suffix,
            "commandDigest": f"command-{shard}{suffix}",
            "locksDigest": "c" * 64 + suffix,
            "toolsDigest": "d" * 64 + suffix,
            "environmentDigest": "e" * 64 + suffix,
            "runnerDigest": "f" * 64 + suffix,
            "hookDigest": "0" * 64 + suffix,
            "hostDigest": "1" * 64 + suffix,
        }

    def successful_report(self, shard: str) -> dict[str, object]:
        commands: list[dict[str, object]] = []
        for script in self.runner.SHARDS[shard]:
            command: dict[str, object] = {
                "script": script,
                "outcome": "passed",
                "exitCode": 0,
                "durationMs": 1,
            }
            if script == self.runner.CAXECRAFT_SCRIPT:
                command["phaseTiming"] = self.caxecraft_timing_report()
            commands.append(command)
        return {
            "schemaVersion": 1,
            "shard": shard,
            "outcome": "passed",
            "durationMs": len(self.runner.SHARDS[shard]),
            "commands": commands,
        }

    def caxecraft_timing_report(self) -> dict[str, object]:
        return {
            "schemaVersion": 1,
            "suite": "caxecraft-domain",
            "mode": "full",
            "outcome": "passed",
            "durationMs": 12,
            "summary": {
                "haxeRequests": sum(
                    requests
                    for _, requests in self.runner.CAXECRAFT_FULL_PHASES
                )
            },
            "phases": [
                {
                    "name": name,
                    "outcome": "passed",
                    "durationMs": 1,
                    "haxeRequests": requests,
                }
                for name, requests in self.runner.CAXECRAFT_FULL_PHASES
            ],
        }

    def test_caxecraft_phase_timing_is_embedded_and_validated(self) -> None:
        def fake_run(command, **kwargs):
            timing_path = Path(kwargs["env"][self.runner.CAXECRAFT_TIMING_ENV])
            timing_path.write_text(
                json.dumps(self.caxecraft_timing_report()), encoding="utf-8"
            )
            return self.runner.subprocess.CompletedProcess(command, 0)

        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(
                    self.runner,
                    "SHARDS",
                    {"caxecraft": (self.runner.CAXECRAFT_SCRIPT,)},
                ),
                mock.patch.object(self.runner.subprocess, "run", side_effect=fake_run),
            ):
                self.runner.run_shard(
                    "caxecraft",
                    {self.runner.CAXECRAFT_SCRIPT: "ignored"},
                    timing_report=report_path,
                    stream=io.StringIO(),
                )
                payload = json.loads(report_path.read_text(encoding="utf-8"))
                self.runner.validate_successful_shard_report(payload, "caxecraft")

        timing = payload["commands"][0]["phaseTiming"]
        self.assertEqual(timing["summary"]["haxeRequests"], 12)
        self.assertEqual(
            [phase["name"] for phase in timing["phases"]],
            [name for name, _ in self.runner.CAXECRAFT_FULL_PHASES],
        )
        self.assertNotIn(str(ROOT), json.dumps(payload))

    def test_failed_caxecraft_phase_prefix_is_preserved(self) -> None:
        failed_timing = self.caxecraft_timing_report()
        failed_timing["outcome"] = "failed"
        failed_timing["phases"] = failed_timing["phases"][:4]
        failed_timing["phases"][-1]["outcome"] = "failed"
        failed_timing["summary"] = {"haxeRequests": 8}

        def fake_run(command, **kwargs):
            timing_path = Path(kwargs["env"][self.runner.CAXECRAFT_TIMING_ENV])
            timing_path.write_text(json.dumps(failed_timing), encoding="utf-8")
            return self.runner.subprocess.CompletedProcess(command, 7)

        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(
                    self.runner,
                    "SHARDS",
                    {"caxecraft": (self.runner.CAXECRAFT_SCRIPT,)},
                ),
                mock.patch.object(self.runner.subprocess, "run", side_effect=fake_run),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure, "exit 7"
                ):
                    self.runner.run_shard(
                        "caxecraft",
                        {self.runner.CAXECRAFT_SCRIPT: "ignored"},
                        timing_report=report_path,
                        stream=io.StringIO(),
                    )
            payload = json.loads(report_path.read_text(encoding="utf-8"))

        command = payload["commands"][0]
        self.assertEqual(command["outcome"], "failed")
        self.assertEqual(command["phaseTiming"], failed_timing)

    def test_every_evidence_input_drift_invalidates_reuse(self) -> None:
        inputs = self.sample_inputs("contracts")
        record = self.runner.evidence_record("contracts", inputs, now=100)
        valid, _, _ = self.runner.validate_reusable_evidence(
            record, "contracts", inputs, now=101
        )
        self.assertTrue(valid)

        for field in inputs:
            with self.subTest(field=field):
                changed = dict(inputs)
                changed[field] += "changed"
                valid, reason, _ = self.runner.validate_reusable_evidence(
                    record, "contracts", changed, now=101
                )
                self.assertFalse(valid)
                self.assertIn("inputs changed", reason)

    def test_malformed_expired_failed_future_and_symlink_evidence_is_rejected(self) -> None:
        inputs = self.sample_inputs("contracts")
        record = self.runner.evidence_record("contracts", inputs, now=100)

        malformed = dict(record)
        malformed["extra"] = True
        expired = dict(record)
        failed = dict(record)
        failed["outcome"] = "failed"
        future = self.runner.evidence_record("contracts", inputs, now=500)

        self.assertFalse(
            self.runner.validate_reusable_evidence(
                malformed, "contracts", inputs, now=101
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                expired,
                "contracts",
                inputs,
                now=100 + self.runner.EVIDENCE_TTL_SECONDS,
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                failed, "contracts", inputs, now=101
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                future, "contracts", inputs, now=101
            )[0]
        )

        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            target = root / "target.json"
            target.write_text(json.dumps(record), encoding="utf-8")
            link = root / "contracts.json"
            link.symlink_to(target)
            reusable, reason, _ = self.runner.read_reusable_evidence(
                link, "contracts", inputs, now=101
            )
        self.assertFalse(reusable)
        self.assertIn("symlink", reason)

    def test_retry_reuses_three_successes_and_runs_only_failed_shard(self) -> None:
        inputs = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        failed_shard = "lowering-semantics"
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for shard in self.runner.SHARD_ORDER:
                record = self.runner.evidence_record(shard, inputs[shard], now=100)
                if shard == failed_shard:
                    record["outcome"] = "failed"
                self.runner.write_timing_report(root / f"{shard}.json", record)
            reused, rejected = self.runner.classify_reusable_evidence(
                root, inputs, now=101
            )

        self.assertEqual(
            tuple(reused),
            ("contracts", "lowering-objects", "caxecraft"),
        )
        self.assertEqual(tuple(rejected), (failed_shard,))
        self.assertEqual(self.runner.pending_shards(reused), (failed_shard,))

    def test_pending_shards_remain_in_canonical_order(self) -> None:
        self.assertEqual(
            self.runner.pending_shards(("caxecraft", "contracts")),
            ("lowering-objects", "lowering-semantics"),
        )

    def test_success_report_must_cover_every_command_exactly(self) -> None:
        report = self.successful_report("contracts")
        validated = self.runner.validate_successful_shard_report(report, "contracts")
        self.assertIs(validated, report)
        missing = dict(report)
        missing["commands"] = report["commands"][:-1]
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "every expected command"
        ):
            self.runner.validate_successful_shard_report(missing, "contracts")
        failed = self.successful_report("contracts")
        failed["commands"][0]["exitCode"] = 9
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "did not pass exactly"
        ):
            self.runner.validate_successful_shard_report(failed, "contracts")

    def test_evidence_directory_requires_ownership_and_rejects_symlinks(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            owned = root / "owned"
            self.runner.prepare_evidence_directory(owned)
            marker = owned / self.runner.EVIDENCE_MARKER
            self.assertTrue(marker.is_file())

            unowned = root / "unowned"
            unowned.mkdir()
            (unowned / "keep.txt").write_text("user data", encoding="utf-8")
            with self.assertRaisesRegex(
                self.runner.ToolchainShardFailure, "no ownership marker"
            ):
                self.runner.prepare_evidence_directory(unowned)
            self.assertEqual(
                (unowned / "keep.txt").read_text(encoding="utf-8"), "user data"
            )

            target = root / "target"
            target.mkdir()
            link = root / "link"
            link.symlink_to(target, target_is_directory=True)
            with self.assertRaisesRegex(
                self.runner.ToolchainShardFailure, "symlink ancestor"
            ):
                self.runner.prepare_evidence_directory(link / "evidence")

    def test_effective_cpu_count_prefers_process_quota_then_affinity(self) -> None:
        with mock.patch.object(
            self.runner.os, "process_cpu_count", return_value=3, create=True
        ):
            self.assertEqual(self.runner.effective_cpu_count(), 3)
        with (
            mock.patch.object(
                self.runner.os, "process_cpu_count", return_value=None, create=True
            ),
            mock.patch.object(
                self.runner.os, "sched_getaffinity", return_value={0, 1}, create=True
            ),
        ):
            self.assertEqual(self.runner.effective_cpu_count(), 2)

    def test_evidence_tool_resolution_matches_npm_local_precedence(self) -> None:
        self.assertIn("cc", self.runner.EVIDENCE_TOOLS)
        with tempfile.TemporaryDirectory() as temporary:
            isolated_root = Path(temporary)
            expected = isolated_root / "node_modules/.bin/haxe"
            expected.parent.mkdir(parents=True)
            expected.write_text("local shim", encoding="utf-8")
            with (
                mock.patch.object(self.runner, "ROOT", isolated_root),
                mock.patch.object(
                    self.runner.shutil,
                    "which",
                    return_value="/unrelated/global/haxe",
                ),
            ):
                self.assertEqual(
                    self.runner.resolve_evidence_tool("haxe"),
                    str(expected),
                )

    def test_input_drift_during_execution_fails_without_writing_evidence(self) -> None:
        scripts = self.runner.load_scripts()
        initial = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        changed = {
            shard: self.sample_inputs(shard, "changed")
            for shard in self.runner.SHARD_ORDER
        }

        def fake_run_shard(shard, scripts, *, timing_report=None, stream=None):
            self.runner.write_timing_report(timing_report, self.successful_report(shard))

        with tempfile.TemporaryDirectory() as temporary:
            evidence_dir = Path(temporary).resolve() / "evidence"
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(initial, changed),
                ),
                mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure,
                    "inputs changed while the shards were running",
                ):
                    self.runner.run_all_shards(
                        scripts,
                        jobs=1,
                        resume=True,
                        evidence_dir=evidence_dir,
                    )
            self.assertTrue((evidence_dir / self.runner.EVIDENCE_MARKER).is_file())
            self.assertFalse(
                any((evidence_dir / f"{shard}.json").exists() for shard in self.runner.SHARD_ORDER)
            )

    def test_second_run_executes_only_the_previously_failed_shard(self) -> None:
        scripts = self.runner.load_scripts()
        inputs = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        failed_shard = "lowering-semantics"
        attempts: list[tuple[int, str]] = []
        round_number = 1

        def fake_run_shard(shard, scripts, *, timing_report=None, stream=None):
            attempts.append((round_number, shard))
            if round_number == 1 and shard == failed_shard:
                report = self.successful_report(shard)
                report["outcome"] = "failed"
                report["commands"][-1]["outcome"] = "failed"
                report["commands"][-1]["exitCode"] = 7
                self.runner.write_timing_report(timing_report, report)
                raise self.runner.ToolchainShardFailure("intentional shard failure")
            self.runner.write_timing_report(timing_report, self.successful_report(shard))

        with tempfile.TemporaryDirectory() as temporary:
            evidence_dir = Path(temporary).resolve() / "evidence"
            timing_dir = Path(temporary).resolve() / "timings"
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(inputs, inputs),
                ),
                mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure, "intentional shard failure"
                ):
                    self.runner.run_all_shards(
                        scripts,
                        jobs=4,
                        resume=True,
                        evidence_dir=evidence_dir,
                    )

            round_number = 2
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(inputs, inputs),
                ),
                mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                self.runner.run_all_shards(
                    scripts,
                    jobs=4,
                    resume=True,
                    evidence_dir=evidence_dir,
                    timing_dir=timing_dir,
                )
            summary = json.loads(
                (timing_dir / "toolchain-parallel-summary.json").read_text(
                    encoding="utf-8"
                )
            )

        first_round = tuple(shard for attempt, shard in attempts if attempt == 1)
        self.assertEqual(len(first_round), len(self.runner.SHARD_ORDER))
        self.assertEqual(set(first_round), set(self.runner.SHARD_ORDER))
        self.assertEqual(
            tuple(shard for attempt, shard in attempts if attempt == 2),
            (failed_shard,),
        )
        self.assertEqual(summary["schemaVersion"], 2)
        self.assertEqual(
            [report["shard"] for report in summary["executedShards"]],
            [failed_shard],
        )
        self.assertEqual(
            [record["shard"] for record in summary["reusedShards"]],
            ["contracts", "lowering-objects", "caxecraft"],
        )

    def test_hook_resume_is_local_and_uncached_checks_stay_before_it(self) -> None:
        hook = (ROOT / "scripts/hooks/pre-commit").read_text(encoding="utf-8")
        workflow = (ROOT / ".github/workflows/governance.yml").read_text(
            encoding="utf-8"
        )
        resume = "npm run test:toolchain:parallel -- --resume"
        self.assertIn(resume, hook)
        for uncached_check in (
            "Exporting Beads issues",
            "Formatting staged Haxe files",
            "Running local path guard",
            "Checking staged whitespace",
            "Running staged secret scan",
            "npm run test:governance",
        ):
            with self.subTest(uncached_check=uncached_check):
                self.assertLess(hook.index(uncached_check), hook.index(resume))
        self.assertLess(hook.index(resume), hook.index("npm run test:native"))
        self.assertNotIn("--resume", workflow)
        self.assertIn("npm run test:toolchain:shard", workflow)


if __name__ == "__main__":
    unittest.main()
