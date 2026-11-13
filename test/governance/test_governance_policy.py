from __future__ import annotations

import importlib.util
import shutil
import subprocess
import sys
import tempfile
import unittest
from unittest import mock
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


class GovernanceTriggerPolicyTests(unittest.TestCase):
    """Keep one PR matrix while retaining main pushes and manual runs."""

    @classmethod
    def setUpClass(cls) -> None:
        spec = importlib.util.spec_from_file_location("governance_trigger_policy", ROOT / "scripts/ci/check_ci_policy.py")
        assert spec is not None and spec.loader is not None
        cls.policy = importlib.util.module_from_spec(spec)
        spec.loader.exec_module(cls.policy)

    def test_current_workflow_preserves_required_contract(self) -> None:
        self.assertEqual(self.policy.validate(), [])

    def test_trigger_coverage_and_duplicate_push_regressions_fail(self) -> None:
        workflow = self.policy.WORKFLOW.read_text()
        mutations = {
            "unrestricted feature pushes": workflow.replace("    branches:\n      - main\n", "", 1),
            "extra feature branch": workflow.replace("      - main\n", "      - main\n      - '**'\n", 1),
            "missing PR": workflow.replace("  pull_request:\n", "", 1),
            "missing manual": workflow.replace("  workflow_dispatch:\n", "", 1),
            "wrong default branch": workflow.replace("      - main\n", "      - release\n", 1),
            "filtered PR coverage": workflow.replace("  pull_request:\n", "  pull_request:\n    branches: [main]\n", 1),
        }
        original_read = self.policy.read_text
        for label, mutated in mutations.items():
            with self.subTest(label=label):
                self.assertNotEqual(mutated, workflow)
                def read(path: Path, errors: list[str]) -> str:
                    return mutated if path == self.policy.WORKFLOW else original_read(path, errors)
                with mock.patch.object(self.policy, "read_text", side_effect=read):
                    errors = self.policy.validate()
                self.assertTrue(any("governance workflow is missing required CI contract: on:" in error for error in errors), errors)


if __name__ == "__main__":
    unittest.main()
