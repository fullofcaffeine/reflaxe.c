from __future__ import annotations

import json
import os
import shlex
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
RESOLVER = ROOT / "scripts/beads/resolve-reviewed.py"
DOLT_RESOLVER = ROOT / "scripts/beads/resolve-reviewed-dolt.py"
EXPORTER = ROOT / "scripts/beads/export-passive.sh"
HISTORY_SCANNER = ROOT / "scripts/security/run-beads-gitleaks.sh"
SAFE_PUSH = ROOT / "scripts/beads/push-safe.sh"


class BeadsToolchainTests(unittest.TestCase):
    def write_bd(self, directory: Path, version: str, *, marker: Path | None = None) -> Path:
        directory.mkdir(parents=True, exist_ok=True)
        executable = directory / "bd"
        marker_command = ""
        if marker is not None:
            marker_command = f"printf '%s\\n' \"$*\" >> {str(marker)!r}\n"
        executable.write_text(
            "#!/usr/bin/env sh\n"
            'if [ "$1" = "--version" ]; then\n'
            f"  printf '%s\\n' {version!r}\n"
            "  exit 0\n"
            "fi\n"
            + marker_command
            + 'if [ "$3" = "context" ]; then exit 0; fi\n'
            + 'if [ "$3" = "export" ]; then\n'
            + "  printf '%s\\n\\n' '{\"_type\":\"issue\",\"id\":\"haxe_c-test\"}'\n"
            + "  exit 0\n"
            + "fi\n"
            + "exit 91\n",
            encoding="utf-8",
        )
        executable.chmod(0o755)
        return executable

    def environment(self, path: str) -> dict[str, str]:
        environment = os.environ.copy()
        environment.pop("HXC_BD_BIN", None)
        environment["PATH"] = path
        return environment

    def test_resolver_skips_incompatible_path_first_client(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-resolver-") as directory:
            root = Path(directory)
            self.write_bd(root / "bad", "bd version 1.0.4 (ce242a879)")
            reviewed = self.write_bd(
                root / "good", "bd version 1.1.0 (8e4e59d39)"
            )
            result = subprocess.run(
                ["python3", str(RESOLVER)],
                check=False,
                capture_output=True,
                text=True,
                env=self.environment(f"{root / 'bad'}:{root / 'good'}:/usr/bin:/bin"),
                timeout=15,
            )
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout.strip(), str(reviewed.resolve()))

    def test_explicit_incompatible_client_fails_with_recovery_help(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-explicit-") as directory:
            root = Path(directory)
            incompatible = self.write_bd(
                root / "bad", "bd version 1.0.4 (ce242a879)"
            )
            environment = self.environment("/usr/bin:/bin")
            environment["HXC_BD_BIN"] = str(incompatible)
            result = subprocess.run(
                ["python3", str(RESOLVER)],
                check=False,
                capture_output=True,
                text=True,
                env=environment,
                timeout=15,
            )
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("requires bd version 1.1.0 (8e4e59d39)", result.stderr)
        self.assertIn("bd bootstrap", result.stderr)
        self.assertIn("do not migrate a second clone", result.stderr)

    def make_checkout(self, root: Path) -> None:
        (root / "scripts/beads").mkdir(parents=True)
        (root / ".beads").mkdir()
        shutil.copy2(RESOLVER, root / "scripts/beads/resolve-reviewed.py")
        shutil.copy2(EXPORTER, root / "scripts/beads/export-passive.sh")
        subprocess.run(["git", "init", "-q"], cwd=root, check=True, timeout=15)
        subprocess.run(
            ["git", "config", "user.email", "beads-test@example.invalid"],
            cwd=root,
            check=True,
            timeout=15,
        )
        subprocess.run(
            ["git", "config", "user.name", "Beads Test"],
            cwd=root,
            check=True,
            timeout=15,
        )
        (root / ".beads/issues.jsonl").write_text(
            '{"_type":"issue","id":"original"}\n', encoding="utf-8"
        )
        subprocess.run(
            ["git", "add", ".beads/issues.jsonl"], cwd=root, check=True, timeout=15
        )

    def git_tree(self, root: Path) -> str:
        return subprocess.run(
            ["git", "write-tree"],
            cwd=root,
            check=True,
            capture_output=True,
            text=True,
            timeout=15,
        ).stdout.strip()

    def test_incompatible_export_cannot_change_jsonl_or_index(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-export-reject-") as directory:
            root = Path(directory)
            self.make_checkout(root)
            marker = root / "unexpected-operation"
            self.write_bd(
                root / "bad",
                "bd version 1.0.4 (ce242a879)",
                marker=marker,
            )
            before_bytes = (root / ".beads/issues.jsonl").read_bytes()
            before_tree = self.git_tree(root)
            result = subprocess.run(
                ["bash", str(root / "scripts/beads/export-passive.sh")],
                cwd=root,
                check=False,
                capture_output=True,
                text=True,
                env=self.environment(f"{root / 'bad'}:/usr/bin:/bin"),
                timeout=15,
            )
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual((root / ".beads/issues.jsonl").read_bytes(), before_bytes)
            self.assertEqual(self.git_tree(root), before_tree)
            self.assertFalse(marker.exists())

    def test_reviewed_export_is_byte_stable_and_index_stable(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-export-stable-") as directory:
            root = Path(directory)
            self.make_checkout(root)
            self.write_bd(root / "good", "bd version 1.1.0 (8e4e59d39)")
            environment = self.environment(f"{root / 'good'}:/usr/bin:/bin")
            command = ["bash", str(root / "scripts/beads/export-passive.sh")]
            subprocess.run(
                command, cwd=root, env=environment, check=True, timeout=15
            )
            first_bytes = (root / ".beads/issues.jsonl").read_bytes()
            first_tree = self.git_tree(root)
            subprocess.run(
                command, cwd=root, env=environment, check=True, timeout=15
            )
            self.assertEqual((root / ".beads/issues.jsonl").read_bytes(), first_bytes)
            self.assertEqual(self.git_tree(root), first_tree)
            self.assertEqual(first_bytes.count(b"\n"), 1)


class BeadsHistoryRecoveryTests(unittest.TestCase):
    def write_executable(self, path: Path, source: str) -> Path:
        path.write_text(source, encoding="utf-8")
        path.chmod(0o755)
        return path

    def make_tools(
        self,
        root: Path,
        *,
        module_version: str = "v0.40.5-0.20260605230755-1bf533220ab0",
    ) -> tuple[Path, Path, dict[str, str]]:
        tools = root / "tools"
        beads_dir = root / "beads"
        database_dir = beads_dir / "embeddeddolt" / "test_beads"
        tools.mkdir()
        database_dir.mkdir(parents=True)

        context = json.dumps(
            {
                "beads_dir": str(beads_dir),
                "database": "test_beads",
                "dolt_mode": "embedded",
            },
            separators=(",", ":"),
        )
        bd = self.write_executable(
            tools / "bd",
            "#!/usr/bin/env sh\n"
            'if [ "$1" = "--version" ]; then\n'
            "  printf '%s\\n' 'bd version 1.1.0 (8e4e59d39)'\n"
            "  exit 0\n"
            "fi\n"
            'if [ "$3" = "context" ]; then\n'
            f"  printf '%s\\n' {shlex.quote(context)}\n"
            "  exit 0\n"
            "fi\n"
            'if [ "$3" = "export" ]; then\n'
            '  printf "export\\n" >> "$HXC_EXPORT_COUNTER"\n'
            "  printf '%s\\n' '{\"_type\":\"issue\",\"id\":\"current-record-marker\"}'\n"
            "  exit 0\n"
            "fi\n"
            'if [ "$3" = "history" ]; then\n'
            "  printf '%s\\n' '{\"error\":\"legacy NULL history\"}'\n"
            "  exit 71\n"
            "fi\n"
            'if [ "$3" = "dolt" ] && [ "$4" = "push" ]; then\n'
            '  printf "%s\\n" pushed > "$HXC_PUSH_MARKER"\n'
            "  exit 0\n"
            "fi\n"
            "exit 91\n",
        )
        dolt = self.write_executable(
            tools / "dolt",
            "#!/usr/bin/env sh\n"
            'if [ "$1" = "version" ]; then\n'
            "  printf '%s\\n' 'dolt version 2.1.4'\n"
            "  exit 0\n"
            "fi\n"
            'if [ "$1" = "-C" ]; then shift 2; fi\n'
            'if [ "$1" != "sql" ]; then exit 92; fi\n'
            'query="$3"\n'
            'case "$query" in\n'
            "  *information_schema.tables*)\n"
            "    printf '%s\\n' '{\"rows\":[{\"TABLE_NAME\":\"comments\"},{\"TABLE_NAME\":\"issues\"}]}'\n"
            "    ;;\n"
            "  *dolt_log*)\n"
            "    printf '%s\\n' '{\"rows\":[{\"message\":\"commit-log-marker\"}]}'\n"
            "    ;;\n"
            "  *dolt_diff_comments*)\n"
            '    if [ "${HXC_FAIL_DOLT_TABLE:-}" = "comments" ]; then exit 73; fi\n'
            "    printf '%s\\n' '{\"rows\":[{\"from_body\":null,\"to_body\":\"comment-history-marker\"}]}'\n"
            "    ;;\n"
            "  *dolt_diff_issues*)\n"
            "    printf '%s\\n' '{\"rows\":[{\"from_description\":null,\"to_description\":\"issue-history-marker\"}]}'\n"
            "    ;;\n"
            "  *) exit 93 ;;\n"
            "esac\n",
        )
        self.write_executable(
            tools / "go",
            "#!/usr/bin/env sh\n"
            'if [ "$1" = "version" ] && [ "$2" = "-m" ]; then\n'
            f"  printf '\\tpath\\t%s\\n' 'github.com/dolthub/dolt/go/cmd/dolt'\n"
            f"  printf '\\tmod\\t%s\\t%s\\t%s\\n' 'github.com/dolthub/dolt/go' {shlex.quote(module_version)} 'h1:oPg5f5bYFy5x7Ws2qtVG7wiva96cIh9SFg7nrC4n7QA='\n"
            "  exit 0\n"
            "fi\n"
            "exit 94\n",
        )
        self.write_executable(
            tools / "gitleaks",
            "#!/usr/bin/env sh\n"
            'if [ "$1" != "stdin" ]; then exit 95; fi\n'
            'cat > "$HXC_SCAN_CAPTURE"\n',
        )

        environment = os.environ.copy()
        environment["PATH"] = f"{tools}:{environment['PATH']}"
        environment["HXC_BD_BIN"] = str(bd)
        environment["HXC_DOLT_BIN"] = str(dolt)
        environment["HXC_EXPORT_COUNTER"] = str(root / "export-counter")
        environment["HXC_SCAN_CAPTURE"] = str(root / "scan-input.json")
        environment["HXC_PUSH_MARKER"] = str(root / "push-marker")
        return bd, dolt, environment

    def test_recovery_reader_requires_exact_go_module_provenance(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-dolt-provenance-") as directory:
            root = Path(directory)
            _, _, environment = self.make_tools(
                root, module_version="v0.40.5-0.20260715172757-a6690826d767"
            )
            result = subprocess.run(
                ["python3", str(DOLT_RESOLVER)],
                cwd=ROOT,
                env=environment,
                check=False,
                capture_output=True,
                text=True,
                timeout=15,
            )
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("unexpected Go build provenance", result.stderr)

    def test_recovery_scans_current_log_and_every_changed_base_table(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-dolt-history-") as directory:
            root = Path(directory)
            _, dolt, environment = self.make_tools(root)
            result = subprocess.run(
                ["bash", str(HISTORY_SCANNER)],
                cwd=ROOT,
                env=environment,
                check=False,
                capture_output=True,
                text=True,
                timeout=15,
            )
            captured = (root / "scan-input.json").read_text(encoding="utf-8")
            export_calls = (root / "export-counter").read_text(encoding="utf-8")
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(export_calls, "export\n")
        self.assertIn("current-record-marker", captured)
        self.assertIn("commit-log-marker", captured)
        self.assertIn("comment-history-marker", captured)
        self.assertIn("issue-history-marker", captured)
        self.assertIn(str(dolt), result.stdout)
        self.assertIn("[beads-gitleaks] OK", result.stdout)

    def test_history_query_failure_stops_before_dolt_push(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-dolt-fail-closed-") as directory:
            root = Path(directory)
            _, _, environment = self.make_tools(root)
            environment["HXC_FAIL_DOLT_TABLE"] = "comments"
            result = subprocess.run(
                ["bash", str(SAFE_PUSH)],
                cwd=ROOT,
                env=environment,
                check=False,
                capture_output=True,
                text=True,
                timeout=15,
            )
        self.assertNotEqual(result.returncode, 0)
        self.assertIn("could not be read completely", result.stderr)
        self.assertFalse((root / "push-marker").exists())


if __name__ == "__main__":
    unittest.main()
