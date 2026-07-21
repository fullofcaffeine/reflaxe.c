from __future__ import annotations

import os
import shutil
import subprocess
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
RESOLVER = ROOT / "scripts/beads/resolve-reviewed.py"
EXPORTER = ROOT / "scripts/beads/export-passive.sh"


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


if __name__ == "__main__":
    unittest.main()
