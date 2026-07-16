from __future__ import annotations

import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "scripts/ci/check_governance_policy.py"
POLICY_FILES = (
    "CONTRIBUTING.md",
    "SECURITY.md",
    "AGENTS.md",
    "README.md",
    "docs/PRD.md",
)


class GovernancePolicyTests(unittest.TestCase):
    def run_policy(self, root: Path = ROOT) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, str(SCRIPT), "--root", str(root), "--quiet"],
            text=True,
            capture_output=True,
            check=False,
        )

    def make_policy_root(self, destination: Path) -> None:
        for relative in POLICY_FILES:
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, target)

        linked_files = (
            "docs/architecture.md",
            "docs/specs/third-party-provenance.json",
            "docs/specs/typed-boundaries.json",
            "THIRD_PARTY_NOTICES.md",
        )
        for relative in linked_files:
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, target)

    def replace(self, root: Path, relative: str, old: str, new: str) -> None:
        path = root / relative
        text = path.read_text(encoding="utf-8")
        self.assertIn(old, text)
        path.write_text(text.replace(old, new), encoding="utf-8")

    def test_repository_policy_passes(self) -> None:
        result = self.run_policy()
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_private_contact_is_required(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            self.replace(
                root,
                "SECURITY.md",
                "boss@fullofcaffeine.com",
                "missing@example.invalid",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("mailto:boss@fullofcaffeine.com", result.stderr)

    def test_supported_version_policy_is_required(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            self.replace(
                root,
                "SECURITY.md",
                "There is no published or supported release yet.",
                "Version policy pending.",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("no published or supported release", result.stderr)

    def test_agents_and_contributing_workflow_drift_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            self.replace(
                root,
                "AGENTS.md",
                "bd list --ready --type task --json",
                "bd list --status open",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("AGENTS.md lost shared workflow text", result.stderr)

    def test_release_signer_responsibility_is_required(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            self.replace(
                root,
                "CONTRIBUTING.md",
                "owner-authorized signer",
                "unspecified signer",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("owner-authorized signer", result.stderr)

    def test_broken_local_governance_link_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            self.replace(
                root,
                "CONTRIBUTING.md",
                "docs/architecture.md",
                "docs/missing-architecture.md",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("links to a missing path", result.stderr)

    def test_verified_github_advisory_path_is_required(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            self.replace(
                root,
                "SECURITY.md",
                "https://github.com/fullofcaffeine/reflaxe.c/security/advisories/new",
                "https://github.com/fullofcaffeine/reflaxe.c/security",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(
                "must name the verified GitHub private reporting path",
                result.stderr,
            )


if __name__ == "__main__":
    unittest.main()
