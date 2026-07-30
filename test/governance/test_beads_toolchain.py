from __future__ import annotations

import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
EXPORTER = ROOT / "scripts/beads/export-passive.sh"
CURRENT_SCANNER = ROOT / "scripts/security/run-beads-gitleaks.sh"
SAFE_PUSH = ROOT / "scripts/beads/push-safe.sh"


class BeadsToolchainTests(unittest.TestCase):
    def write_executable(self, path: Path, source: str) -> Path:
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(source, encoding="utf-8")
        path.chmod(0o755)
        return path

    def environment(self, tools: Path) -> dict[str, str]:
        environment = os.environ.copy()
        environment["PATH"] = f"{tools}:/usr/bin:/bin"
        return environment

    def make_checkout(self, root: Path) -> None:
        (root / "scripts/beads").mkdir(parents=True)
        (root / ".beads").mkdir()
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

    def test_failed_export_preserves_jsonl_and_index(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-export-fail-") as directory:
            root = Path(directory)
            tools = root / "tools"
            self.make_checkout(root)
            self.write_executable(
                tools / "bd",
                "#!/usr/bin/env sh\n"
                'if [ "$3" = "context" ]; then exit 0; fi\n'
                'if [ "$3" = "export" ]; then exit 71; fi\n'
                "exit 91\n",
            )
            before_bytes = (root / ".beads/issues.jsonl").read_bytes()
            before_tree = self.git_tree(root)
            result = subprocess.run(
                ["bash", str(root / "scripts/beads/export-passive.sh")],
                cwd=root,
                check=False,
                capture_output=True,
                text=True,
                env=self.environment(tools),
                timeout=15,
            )
            self.assertNotEqual(result.returncode, 0)
            self.assertEqual((root / ".beads/issues.jsonl").read_bytes(), before_bytes)
            self.assertEqual(self.git_tree(root), before_tree)

    def test_export_is_byte_stable_and_index_stable(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-export-stable-") as directory:
            root = Path(directory)
            tools = root / "tools"
            self.make_checkout(root)
            self.write_executable(
                tools / "bd",
                "#!/usr/bin/env sh\n"
                'if [ "$3" = "context" ]; then exit 0; fi\n'
                'if [ "$3" = "export" ]; then\n'
                "  printf '%s\\n\\n' '{\"_type\":\"issue\",\"id\":\"haxe_c-test\"}'\n"
                "  exit 0\n"
                "fi\n"
                "exit 91\n",
            )
            environment = self.environment(tools)
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

    def make_scan_tools(
        self, root: Path, *, export_status: int = 0
    ) -> tuple[Path, dict[str, str]]:
        tools = root / "tools"
        self.write_executable(
            tools / "bd",
            "#!/usr/bin/env sh\n"
            'if [ "$3" = "export" ]; then\n'
            f"  if [ {export_status} -ne 0 ]; then exit {export_status}; fi\n"
            "  printf '%s\\n' '{\"_type\":\"issue\",\"id\":\"current-record-marker\"}'\n"
            "  exit 0\n"
            "fi\n"
            'if [ "$3" = "dolt" ] && [ "$4" = "push" ]; then\n'
            '  printf "%s\\n" pushed > "$HXC_PUSH_MARKER"\n'
            "  exit 0\n"
            "fi\n"
            "exit 91\n",
        )
        self.write_executable(
            tools / "gitleaks",
            "#!/usr/bin/env sh\n"
            'if [ "$1" != "stdin" ]; then exit 95; fi\n'
            'cat > "$HXC_SCAN_CAPTURE"\n',
        )
        environment = self.environment(tools)
        environment["HXC_SCAN_CAPTURE"] = str(root / "scan-input.json")
        environment["HXC_PUSH_MARKER"] = str(root / "push-marker")
        return tools, environment

    def test_current_export_is_scanned_without_history_reader(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-current-") as directory:
            root = Path(directory)
            _, environment = self.make_scan_tools(root)
            result = subprocess.run(
                ["bash", str(CURRENT_SCANNER)],
                cwd=ROOT,
                env=environment,
                check=False,
                capture_output=True,
                text=True,
                timeout=15,
            )
            captured = (root / "scan-input.json").read_text(encoding="utf-8")

        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn("current-record-marker", captured)
        self.assertIn("[beads-gitleaks] OK", result.stdout)

    def test_export_failure_stops_before_push(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-beads-push-fail-") as directory:
            root = Path(directory)
            _, environment = self.make_scan_tools(root, export_status=73)
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
        self.assertIn("current Beads export could not be read", result.stderr)
        self.assertFalse((root / "push-marker").exists())


if __name__ == "__main__":
    unittest.main()
