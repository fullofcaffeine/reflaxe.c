from __future__ import annotations

import importlib.util
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "scripts/ci/check_agent_instruction_links.py"
SPEC = importlib.util.spec_from_file_location("agent_instruction_links", SCRIPT)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError("cannot load the agent-instruction link checker")
MODULE = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(MODULE)


class AgentInstructionLinkTests(unittest.TestCase):
    def test_repository_links_pass(self) -> None:
        result = subprocess.run(
            [sys.executable, str(SCRIPT), "--root", str(ROOT), "--quiet"],
            text=True,
            capture_output=True,
            check=False,
        )
        self.assertEqual(result.returncode, 0, result.stderr)

    def make_root(self) -> tuple[tempfile.TemporaryDirectory[str], Path, dict[str, str]]:
        temporary = tempfile.TemporaryDirectory()
        root = Path(temporary.name)
        (root / "AGENTS.md").write_text("canonical\n", encoding="utf-8")
        (root / "CLAUDE.md").symlink_to("AGENTS.md")
        return temporary, root, {"AGENTS.md": "100644", "CLAUDE.md": "120000"}

    def assert_rejected(self, target: str, expected: str) -> None:
        temporary, root, tracked = self.make_root()
        with temporary:
            (root / "CLAUDE.md").unlink()
            (root / "CLAUDE.md").symlink_to(target)
            errors = MODULE.validate_instruction_link(root, "CLAUDE.md", tracked)
            self.assertTrue(any(expected in error for error in errors), errors)

    def test_valid_relative_link_passes(self) -> None:
        temporary, root, tracked = self.make_root()
        with temporary:
            self.assertEqual(
                MODULE.validate_instruction_link(root, "CLAUDE.md", tracked), []
            )

    def test_copied_file_is_rejected(self) -> None:
        temporary, root, tracked = self.make_root()
        with temporary:
            (root / "CLAUDE.md").unlink()
            (root / "CLAUDE.md").write_text("copy\n", encoding="utf-8")
            tracked["CLAUDE.md"] = "100644"
            errors = MODULE.validate_instruction_link(root, "CLAUDE.md", tracked)
            self.assertTrue(any("symbolic link" in error for error in errors), errors)

    def test_absolute_escaping_and_wrong_links_are_rejected(self) -> None:
        for target in ("/tmp/AGENTS.md", "../AGENTS.md", "OTHER.md"):
            with self.subTest(target=target):
                self.assert_rejected(target, "same-directory AGENTS.md")

    def test_broken_link_is_rejected(self) -> None:
        temporary, root, tracked = self.make_root()
        with temporary:
            (root / "AGENTS.md").unlink()
            errors = MODULE.validate_instruction_link(root, "CLAUDE.md", tracked)
            self.assertTrue(any("missing" in error for error in errors), errors)

    def test_untracked_target_is_rejected(self) -> None:
        temporary, root, tracked = self.make_root()
        with temporary:
            del tracked["AGENTS.md"]
            errors = MODULE.validate_instruction_link(root, "CLAUDE.md", tracked)
            self.assertTrue(any("tracked" in error for error in errors), errors)


if __name__ == "__main__":
    unittest.main()
