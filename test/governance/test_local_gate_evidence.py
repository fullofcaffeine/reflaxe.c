from __future__ import annotations

import importlib.util
import io
import json
import subprocess
import sys
import tempfile
import threading
import time
import unittest
from contextlib import contextmanager, redirect_stderr, redirect_stdout
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
SCRIPT_DIR = ROOT / "scripts/ci"
RUNNER = SCRIPT_DIR / "run_local_gate.py"


def load_runner():
    sys.path.insert(0, str(SCRIPT_DIR))
    try:
        spec = importlib.util.spec_from_file_location("local_gate_subject", RUNNER)
        if spec is None or spec.loader is None:
            raise RuntimeError(f"cannot load {RUNNER}")
        module = importlib.util.module_from_spec(spec)
        sys.modules[spec.name] = module
        spec.loader.exec_module(module)
        return module
    finally:
        sys.path.remove(str(SCRIPT_DIR))


class LocalGateEvidenceTests(unittest.TestCase):
    """Prove that local reuse accepts only one exact prior passing result."""

    @classmethod
    def setUpClass(cls) -> None:
        cls.runner = load_runner()

    def setUp(self) -> None:
        self.temporary = tempfile.TemporaryDirectory(prefix="hxc-local-gate-test-")
        self.temporary_root = Path(self.temporary.name).resolve()
        self.evidence_dir = self.temporary_root / "evidence"
        self.script = "test:hxc-ir"
        self.command = "python3 test/sample/run.py"
        self.environment = {"PATH": "/tools", "HXC_MODE": "portable"}

    def tearDown(self) -> None:
        self.temporary.cleanup()

    def inputs(self, suffix: str = "") -> dict[str, str]:
        return {
            "stagedTree": "a" * 40 + suffix,
            "worktreeDigest": "b" * 64 + suffix,
            "ownerInputsDigest": "3" * 64 + suffix,
            "commandDigest": "c" * 64 + suffix,
            "locksDigest": "d" * 64 + suffix,
            "toolsDigest": "e" * 64 + suffix,
            "environmentDigest": "f" * 64 + suffix,
            "runnerDigest": "0" * 64 + suffix,
            "hookDigest": "1" * 64 + suffix,
            "hostDigest": "2" * 64 + suffix,
        }

    def record(
        self,
        inputs: dict[str, str] | None = None,
        *,
        now: int = 100,
    ) -> dict[str, object]:
        return self.runner.evidence_record(
            self.script,
            self.inputs() if inputs is None else inputs,
            duration_ms=12,
            now=now,
        )

    def run_with(
        self,
        input_sequence: list[dict[str, str]],
        *,
        hook_mode: bool = False,
        cold: bool = False,
        environment: dict[str, str] | None = None,
        returncode: int = 0,
        now: int = 100,
    ) -> tuple[int, mock.Mock]:
        process = mock.Mock(
            return_value=self.runner.subprocess.CompletedProcess(
                ["npm"], returncode
            )
        )
        with (
            mock.patch.object(
                self.runner,
                "load_scripts",
                return_value={self.script: self.command},
            ),
            mock.patch.object(
                self.runner,
                "collect_inputs",
                side_effect=input_sequence,
            ),
            mock.patch.object(self.runner.subprocess, "run", process),
            redirect_stdout(io.StringIO()),
            redirect_stderr(io.StringIO()),
        ):
            result = self.runner.run_owner(
                self.script,
                hook_mode=hook_mode,
                cold=cold,
                evidence_dir=self.evidence_dir,
                environment=(
                    self.environment if environment is None else environment
                ),
                now=now,
            )
        return result, process

    def test_every_input_family_invalidates_reuse(self) -> None:
        baseline = self.inputs()
        record = self.record(baseline)
        valid, _, _ = self.runner.validate_reusable_evidence(
            record, self.script, baseline, now=101
        )
        self.assertTrue(valid)
        for name in baseline:
            changed = dict(baseline)
            changed[name] += "-changed"
            with self.subTest(input=name):
                reusable, reason, age = self.runner.validate_reusable_evidence(
                    record,
                    self.script,
                    changed,
                    now=101,
                )
                self.assertFalse(reusable)
                self.assertEqual(reason, f"evidence inputs changed: {name}")
                self.assertIsNone(age)

    def test_collect_inputs_maps_every_producer_and_tracks_hook_bytes(self) -> None:
        hook_root = self.temporary_root / "hook-root"
        hook = hook_root / "scripts/hooks/pre-commit"
        hook.parent.mkdir(parents=True)
        hook.write_text("#!/bin/sh\nexit 0\n", encoding="utf-8")
        environment = {"PATH": "/tools", "HXC_MODE": "portable"}
        with (
            mock.patch.object(self.runner, "ROOT", hook_root),
            mock.patch.object(
                self.runner, "staged_tree_identity", return_value="staged"
            ),
            mock.patch.object(
                self.runner, "relevant_worktree_digest", return_value="worktree"
            ),
            mock.patch.object(
                self.runner, "owner_inputs_digest", return_value="owner-inputs"
            ),
            mock.patch.object(self.runner, "digest_files", return_value="locks"),
            mock.patch.object(
                self.runner, "tool_identity_digest", return_value="tools"
            ),
            mock.patch.object(
                self.runner, "environment_digest", return_value="environment"
            ),
            mock.patch.object(
                self.runner, "runner_implementation_digest", return_value="runner"
            ),
            mock.patch.object(
                self.runner, "host_identity_digest", return_value="host"
            ),
        ):
            baseline = self.runner.collect_inputs(
                self.script,
                self.command,
                environment,
            )
            hook.write_text("#!/bin/sh\nexit 1\n", encoding="utf-8")
            hook_changed = self.runner.collect_inputs(
                self.script,
                self.command,
                environment,
            )
        self.assertEqual(
            baseline,
            {
                "stagedTree": "staged",
                "worktreeDigest": "worktree",
                "ownerInputsDigest": "owner-inputs",
                "commandDigest": self.runner.command_definition_digest(
                    self.script,
                    self.command,
                ),
                "locksDigest": "locks",
                "toolsDigest": "tools",
                "environmentDigest": "environment",
                "runnerDigest": "runner",
                "hookDigest": self.runner.sha256_bytes(
                    b"#!/bin/sh\nexit 0\n"
                ),
                "hostDigest": "host",
            },
        )
        self.assertNotEqual(baseline["hookDigest"], hook_changed["hookDigest"])
        self.assertNotEqual(
            self.runner.command_definition_digest(self.script, self.command),
            self.runner.command_definition_digest(
                self.script,
                self.command + " --changed",
            ),
        )

    def test_owner_input_digest_covers_ignored_classpath_files(self) -> None:
        repository = self.temporary_root / "owner-inputs"
        roots = sorted(
            {
                relative
                for owner_roots in self.runner.OWNER_INPUT_ROOTS.values()
                for relative in owner_roots
            }
        )
        for relative in roots:
            (repository / relative).mkdir(parents=True)
        with mock.patch.object(self.runner, "ROOT", repository):
            baseline = self.runner.owner_inputs_digest("test:all-sources")
            ignored_source = repository / "src" / "ignored" / "Shadow.hx"
            ignored_source.parent.mkdir()
            ignored_source.write_text(
                "class Shadow { static var value:Int = 1; }\n",
                encoding="utf-8",
            )
            source_changed = self.runner.owner_inputs_digest("test:all-sources")
            vendor_source = (
                repository / "vendor/reflaxe/src" / "ignored" / "Shadow.hx"
            )
            vendor_source.parent.mkdir()
            vendor_source.write_text(
                "class Shadow { static var value:Int = 2; }\n",
                encoding="utf-8",
            )
            vendor_changed = self.runner.owner_inputs_digest("test:all-sources")
            project_baseline = self.runner.owner_inputs_digest(
                "test:project-emitter"
            )
            ignored_project_runner = (
                repository / "test/project_emitter/ignored_probe.py"
            )
            ignored_project_runner.write_text(
                "raise RuntimeError('must invalidate evidence')\n",
                encoding="utf-8",
            )
            project_changed = self.runner.owner_inputs_digest(
                "test:project-emitter"
            )
        self.assertNotEqual(baseline, source_changed)
        self.assertNotEqual(source_changed, vendor_changed)
        self.assertNotEqual(project_baseline, project_changed)

    def test_owner_environment_prevents_self_generated_python_cache_drift(
        self,
    ) -> None:
        repository = self.temporary_root / "bytecode-inputs"
        roots = sorted(
            {
                relative
                for owner_roots in self.runner.OWNER_INPUT_ROOTS.values()
                for relative in owner_roots
            }
        )
        for relative in roots:
            (repository / relative).mkdir(parents=True)
        imported = repository / "test/all_sources/imported_probe.py"
        imported.write_text("VALUE = 7\n", encoding="utf-8")
        environment = self.runner.normalized_owner_environment(
            {"PATH": "/usr/bin:/bin"}
        )
        with mock.patch.object(self.runner, "ROOT", repository):
            before = self.runner.owner_inputs_digest("test:project-emitter")
            subprocess.run(
                [
                    sys.executable,
                    "-c",
                    (
                        "import importlib.util;"
                        f"p={str(imported)!r};"
                        "s=importlib.util.spec_from_file_location('probe',p);"
                        "m=importlib.util.module_from_spec(s);"
                        "s.loader.exec_module(m)"
                    ),
                ],
                cwd=repository,
                env=environment,
                check=True,
            )
            after = self.runner.owner_inputs_digest("test:project-emitter")
        self.assertEqual(environment["PYTHONDONTWRITEBYTECODE"], "1")
        self.assertEqual(before, after)
        self.assertFalse((imported.parent / "__pycache__").exists())

    def test_direct_and_git_hook_transport_environments_normalize_exactly(self) -> None:
        direct = {
            "PATH": f"{ROOT}/node_modules/.bin:/usr/bin:/bin",
            "HOME": "/workspace/user",
            "HXC_MODE": "portable",
        }
        default_index = self.runner.default_git_index_path(direct)
        self.assertIsNotNone(default_index)
        hook = {
            **direct,
            "BD_GIT_HOOK": "1",
            "GIT_INDEX_FILE": str(default_index),
            "GIT_EDITOR": ":",
            "GIT_AUTHOR_NAME": "Test Author",
            "GIT_AUTHOR_EMAIL": "test@example.invalid",
            "GIT_AUTHOR_DATE": "Thu Jul 30 22:00:00 2026 -0600",
        }
        self.assertEqual(
            self.runner.normalized_owner_environment(direct),
            self.runner.normalized_owner_environment(hook),
        )

        partial_commit = dict(hook)
        partial_commit["GIT_INDEX_FILE"] = str(
            self.temporary_root / "next-index-123.lock"
        )
        self.assertIn(
            "GIT_INDEX_FILE",
            self.runner.normalized_owner_environment(partial_commit),
        )
        changed_semantics = dict(hook)
        changed_semantics["HXC_MODE"] = "metal"
        self.assertNotEqual(
            self.runner.normalized_owner_environment(direct),
            self.runner.normalized_owner_environment(changed_semantics),
        )

    def test_relevant_worktree_digest_covers_untracked_owner_inputs(self) -> None:
        repository = self.temporary_root / "repository"
        repository.mkdir()
        subprocess.run(
            ["git", "init", "--quiet"],
            cwd=repository,
            check=True,
        )
        shard_runner = sys.modules["run_toolchain_shard"]
        with (
            mock.patch.object(shard_runner, "ROOT", repository),
            mock.patch.object(
                shard_runner,
                "RELEVANT_UNTRACKED_ROOTS",
                ("src", "std", "test", "examples", "vendor"),
            ),
        ):
            baseline = shard_runner.relevant_worktree_digest()
            source = repository / "src" / "Unsafe.hx"
            source.parent.mkdir()
            source.write_text(
                "class Unsafe { static var value:Dynamic; }\n",
                encoding="utf-8",
            )
            source_changed = shard_runner.relevant_worktree_digest()
            vendor = repository / "vendor" / "reflaxe" / "src" / "Probe.hx"
            vendor.parent.mkdir(parents=True)
            vendor.write_text(
                "class Probe { static var value:Int = 1; }\n",
                encoding="utf-8",
            )
            vendor_changed = shard_runner.relevant_worktree_digest()
        self.assertNotEqual(baseline, source_changed)
        self.assertNotEqual(source_changed, vendor_changed)

    def test_closed_schema_owner_outcome_key_and_lifetime_fail_closed(self) -> None:
        baseline = self.inputs()
        mutations = (
            ("extra field", lambda value: value.update({"extra": True})),
            ("wrong owner", lambda value: value.update({"owner": "test:other"})),
            ("failure", lambda value: value.update({"outcome": "failed"})),
            ("wrong key", lambda value: value.update({"key": "0" * 64})),
            ("future", lambda value: value.update({"createdAtUnix": 200})),
            ("bad duration", lambda value: value.update({"durationMs": -1})),
        )
        for label, mutate in mutations:
            payload = self.record(baseline)
            mutate(payload)
            with self.subTest(case=label):
                reusable, _, _ = self.runner.validate_reusable_evidence(
                    payload,
                    self.script,
                    baseline,
                    now=101,
                )
                self.assertFalse(reusable)
        expired = self.record(baseline, now=100)
        reusable, reason, _ = self.runner.validate_reusable_evidence(
            expired,
            self.script,
            baseline,
            now=100 + self.runner.LOCAL_EVIDENCE_TTL_SECONDS,
        )
        self.assertFalse(reusable)
        self.assertEqual(reason, "record has expired")

    def test_direct_pass_publishes_and_hook_reuses_without_execution(self) -> None:
        baseline = self.inputs()
        result, direct_process = self.run_with([baseline, baseline])
        self.assertEqual(result, 0)
        self.assertEqual(direct_process.call_count, 1)
        record_path = self.runner.owner_record_path(
            self.evidence_dir, self.script
        )
        self.assertTrue(record_path.is_file())

        result, hook_process = self.run_with(
            [baseline],
            hook_mode=True,
            now=101,
        )
        self.assertEqual(result, 0)
        self.assertEqual(hook_process.call_count, 0)

    def test_failure_timeout_and_input_drift_publish_nothing(self) -> None:
        for returncode in (1, 124):
            with self.subTest(returncode=returncode):
                directory = self.temporary_root / f"failure-{returncode}"
                self.runner.prepare_evidence_directory(directory)
                record_path = self.runner.owner_record_path(directory, self.script)
                self.runner.write_timing_report(
                    record_path,
                    self.record(),
                )
                with mock.patch.object(self, "evidence_dir", directory):
                    result, process = self.run_with(
                        [self.inputs()],
                        returncode=returncode,
                    )
                self.assertEqual(result, returncode)
                self.assertEqual(process.call_count, 1)
                self.assertFalse(record_path.exists())

        drift_directory = self.temporary_root / "drift"
        with mock.patch.object(self, "evidence_dir", drift_directory):
            result, process = self.run_with([self.inputs(), self.inputs("-drift")])
        self.assertEqual(result, 0)
        self.assertEqual(process.call_count, 1)
        self.assertFalse(
            self.runner.owner_record_path(drift_directory, self.script).exists()
        )

    def test_cold_ci_corruption_and_incomplete_records_never_hit(self) -> None:
        baseline = self.inputs()
        result, _ = self.run_with([baseline, baseline])
        self.assertEqual(result, 0)

        result, cold_process = self.run_with(
            [baseline, baseline],
            hook_mode=True,
            cold=True,
            now=101,
        )
        self.assertEqual(result, 0)
        self.assertEqual(cold_process.call_count, 1)
        self.assertFalse(
            self.runner.owner_record_path(
                self.evidence_dir, self.script
            ).exists()
        )

        ci_directory = self.temporary_root / "ci"
        self.runner.prepare_evidence_directory(ci_directory)
        ci_record_path = self.runner.owner_record_path(ci_directory, self.script)
        self.runner.write_timing_report(ci_record_path, self.record())
        with mock.patch.object(self, "evidence_dir", ci_directory):
            result, ci_process = self.run_with(
                [baseline],
                hook_mode=True,
                environment={**self.environment, "CI": "true"},
            )
        self.assertEqual(result, 0)
        self.assertEqual(ci_process.call_count, 1)
        self.assertFalse(ci_record_path.exists())

        broken_cold = self.temporary_root / "broken-cold"
        broken_cold.mkdir()
        (broken_cold / self.runner.EVIDENCE_MARKER).write_text(
            "{not-json",
            encoding="utf-8",
        )
        with mock.patch.object(self, "evidence_dir", broken_cold):
            result, broken_cold_process = self.run_with(
                [baseline],
                hook_mode=True,
                cold=True,
            )
        self.assertEqual(result, 0)
        self.assertEqual(broken_cold_process.call_count, 1)

        for label, payload in (
            ("corrupt", "{not-json"),
            ("incomplete", '{"schemaVersion":1}\n'),
        ):
            directory = self.temporary_root / label
            self.runner.prepare_evidence_directory(directory)
            record_path = self.runner.owner_record_path(directory, self.script)
            record_path.write_text(payload, encoding="utf-8")
            with mock.patch.object(self, "evidence_dir", directory):
                result, process = self.run_with(
                    [baseline, baseline],
                    hook_mode=True,
                )
            self.assertEqual(result, 0)
            self.assertEqual(process.call_count, 1)
            repaired = json.loads(record_path.read_text(encoding="utf-8"))
            self.assertEqual(repaired["outcome"], "passed")

    def test_cold_execution_remains_inside_the_owner_lock(self) -> None:
        events: list[str] = []

        @contextmanager
        def observed_lock(_record_path: Path):
            events.append("lock-enter")
            try:
                yield
            finally:
                events.append("lock-exit")

        def observed_execute(_script: str, _environment: dict[str, str]):
            self.assertEqual(events, ["lock-enter"])
            events.append("execute")
            return 0, 1

        with (
            mock.patch.object(
                self.runner,
                "load_scripts",
                return_value={self.script: self.command},
            ),
            mock.patch.object(
                self.runner, "owner_evidence_lock", observed_lock
            ),
            mock.patch.object(
                self.runner, "execute_package_owner", observed_execute
            ),
            redirect_stdout(io.StringIO()),
            redirect_stderr(io.StringIO()),
        ):
            result = self.runner.run_owner(
                self.script,
                hook_mode=False,
                cold=True,
                evidence_dir=self.evidence_dir,
                environment=self.environment,
            )
        self.assertEqual(result, 0)
        self.assertEqual(events, ["lock-enter", "execute", "lock-exit"])

    def test_atomic_concurrent_writers_never_create_a_false_hit(self) -> None:
        self.runner.prepare_evidence_directory(self.evidence_dir)
        record_path = self.runner.owner_record_path(
            self.evidence_dir, self.script
        )
        first = self.inputs("-first")
        second = self.inputs("-second")
        barrier = threading.Barrier(2)

        def publish(inputs: dict[str, str], now: int) -> None:
            barrier.wait()
            self.runner.write_timing_report(
                record_path,
                self.runner.evidence_record(
                    self.script,
                    inputs,
                    duration_ms=1,
                    now=now,
                ),
            )

        threads = (
            threading.Thread(target=publish, args=(first, 100)),
            threading.Thread(target=publish, args=(second, 101)),
        )
        for thread in threads:
            thread.start()
        for thread in threads:
            thread.join()

        payload = json.loads(record_path.read_text(encoding="utf-8"))
        winning_inputs = payload["inputs"]
        self.assertIn(winning_inputs, (first, second))
        valid, _, _ = self.runner.validate_reusable_evidence(
            payload,
            self.script,
            winning_inputs,
            now=102,
        )
        self.assertTrue(valid)
        losing_inputs = second if winning_inputs == first else first
        invalid, _, _ = self.runner.validate_reusable_evidence(
            payload,
            self.script,
            losing_inputs,
            now=102,
        )
        self.assertFalse(invalid)

    def test_owner_lock_serializes_separate_processes(self) -> None:
        self.runner.prepare_evidence_directory(self.evidence_dir)
        record_path = self.runner.owner_record_path(
            self.evidence_dir, self.script
        )
        entered_path = self.temporary_root / "child-entered"
        child_source = "\n".join(
            (
                "import importlib.util",
                "import sys",
                "from pathlib import Path",
                f"sys.path.insert(0, {str(SCRIPT_DIR)!r})",
                f"spec = importlib.util.spec_from_file_location('child_gate', {str(RUNNER)!r})",
                "module = importlib.util.module_from_spec(spec)",
                "sys.modules[spec.name] = module",
                "spec.loader.exec_module(module)",
                f"with module.owner_evidence_lock(Path({str(record_path)!r})):",
                f"    Path({str(entered_path)!r}).write_text('entered\\n', encoding='utf-8')",
            )
        )
        with self.runner.owner_evidence_lock(record_path):
            child = subprocess.Popen(
                [sys.executable, "-c", child_source],
                cwd=ROOT,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
            )
            try:
                time.sleep(0.2)
                self.assertIsNone(child.poll())
                self.assertFalse(entered_path.exists())
            except BaseException:
                child.kill()
                child.communicate()
                raise
        stdout, stderr = child.communicate(timeout=10)
        self.assertEqual(stdout, "")
        self.assertEqual(stderr, "")
        self.assertEqual(child.returncode, 0)
        self.assertEqual(entered_path.read_text(encoding="utf-8"), "entered\n")

    def test_owned_directory_and_script_policy_reject_unsafe_shapes(self) -> None:
        unowned = self.temporary_root / "unowned"
        unowned.mkdir()
        (unowned / "keep.txt").write_text("user data\n", encoding="utf-8")
        with self.assertRaisesRegex(
            self.runner.LocalGateFailure, "no ownership marker"
        ):
            self.runner.prepare_evidence_directory(unowned)

        target = self.temporary_root / "target"
        target.mkdir()
        link = self.temporary_root / "link"
        link.symlink_to(target, target_is_directory=True)
        with self.assertRaisesRegex(
            self.runner.LocalGateFailure, "symlink ancestor"
        ):
            self.runner.prepare_evidence_directory(link / "evidence")

        self.runner.prepare_evidence_directory(self.evidence_dir)
        record_path = self.runner.owner_record_path(
            self.evidence_dir, self.script
        )
        lock_target = self.temporary_root / "lock-target"
        lock_target.write_text("", encoding="utf-8")
        self.runner.lock_path_for_record(record_path).symlink_to(lock_target)
        with self.assertRaisesRegex(
            self.runner.LocalGateFailure, "lock path is a symlink"
        ):
            with self.runner.owner_evidence_lock(record_path):
                self.fail("a symlink lock path must never be acquired")

        scripts = {
            "test:local-gate": "python3 scripts/ci/run_local_gate.py",
            "profile:slow": "python3 slow.py",
        }
        with self.assertRaisesRegex(self.runner.LocalGateFailure, "itself"):
            self.runner.validate_owner_script("test:local-gate", scripts)
        with self.assertRaisesRegex(
            self.runner.LocalGateFailure, "must be a test"
        ):
            self.runner.validate_owner_script("profile:slow", scripts)
        self.assertFalse(self.runner.script_is_reusable("test:performance-sample"))
        self.assertFalse(self.runner.script_is_reusable("snapshots:catalog"))
        self.assertFalse(self.runner.script_is_reusable("test:governance"))
        self.assertFalse(self.runner.script_is_reusable("test:hello"))
        self.assertTrue(self.runner.script_is_reusable("test:project-emitter"))
        self.assertTrue(self.runner.script_is_reusable("test:hxc-ir"))
        self.assertEqual(
            set(self.runner.OWNER_INPUT_ROOTS),
            set(self.runner.REUSABLE_OWNER_SCRIPTS),
        )


if __name__ == "__main__":
    unittest.main()
