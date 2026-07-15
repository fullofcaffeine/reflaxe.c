from __future__ import annotations

import hashlib
import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "scripts/ci/check_license_policy.py"
POLICY_PATH = ROOT / "docs/specs/third-party-provenance.json"


class LicensePolicyTests(unittest.TestCase):
    def run_policy(self, root: Path = ROOT, package_root: Path | None = None) -> subprocess.CompletedProcess[str]:
        command = [sys.executable, str(SCRIPT), "--root", str(root), "--quiet"]
        if package_root is not None:
            command.extend(["--package-root", str(package_root)])
        return subprocess.run(command, text=True, capture_output=True, check=False)

    def copy_release_files(self, destination: Path, *, omit: str | None = None) -> None:
        policy = json.loads(POLICY_PATH.read_text(encoding="utf-8"))
        for entry in policy["release"]["requiredFiles"]:
            relative = Path(entry["path"])
            if relative.as_posix() == omit:
                continue
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, target)

    def make_policy_root(self, destination: Path) -> None:
        self.copy_release_files(destination)

    def test_repository_policy_passes(self) -> None:
        result = self.run_policy()
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_complete_release_tree_passes(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.copy_release_files(package_root)
            result = self.run_policy(package_root=package_root)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_release_tree_missing_notice_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.copy_release_files(package_root, omit="THIRD_PARTY_NOTICES.md")
            result = self.run_policy(package_root=package_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("release package is missing required file: THIRD_PARTY_NOTICES.md", result.stderr)

    def test_release_tree_with_modified_license_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.copy_release_files(package_root)
            with (package_root / "LICENSE").open("a", encoding="utf-8") as handle:
                handle.write("\nmodified\n")
            result = self.run_policy(package_root=package_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("release package file differs from reviewed copy: LICENSE", result.stderr)

    def test_modified_repository_license_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            policy_root = Path(temporary)
            self.make_policy_root(policy_root)
            with (policy_root / "LICENSE").open("a", encoding="utf-8") as handle:
                handle.write("\nmodified\n")
            result = self.run_policy(root=policy_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("project license hash mismatch: LICENSE", result.stderr)

    def test_unlisted_std_override_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            policy_root = Path(temporary)
            self.make_policy_root(policy_root)
            unlisted = policy_root / "std/c/_std/Unlisted.hx"
            unlisted.parent.mkdir(parents=True, exist_ok=True)
            unlisted.write_text("package;\n", encoding="utf-8")
            result = self.run_policy(root=policy_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("derived source is not in the provenance ledger: std/c/_std/Unlisted.hx", result.stderr)

    def test_component_local_license_must_be_packaged(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            policy_root = Path(temporary)
            self.make_policy_root(policy_root)
            extra_license = policy_root / "LICENSES/REFLAXE-MIT.txt"
            extra_license.parent.mkdir(parents=True, exist_ok=True)
            extra_license.write_text("Reflaxe MIT license fixture\n", encoding="utf-8")
            policy_path = policy_root / "docs/specs/third-party-provenance.json"
            policy = json.loads(policy_path.read_text(encoding="utf-8"))
            policy["components"][0]["localLicenseFiles"] = [
                {
                    "path": "LICENSES/REFLAXE-MIT.txt",
                    "sha256": hashlib.sha256(extra_license.read_bytes()).hexdigest(),
                }
            ]
            policy_path.write_text(json.dumps(policy), encoding="utf-8")
            result = self.run_policy(root=policy_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(
                "component local license is absent from release.requiredFiles",
                result.stderr,
            )

    def test_unowned_runtime_vendor_file_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            policy_root = Path(temporary)
            self.make_policy_root(policy_root)
            unowned = policy_root / "runtime/third_party/example.c"
            unowned.parent.mkdir(parents=True, exist_ok=True)
            unowned.write_text("int example(void) { return 0; }\n", encoding="utf-8")
            result = self.run_policy(root=policy_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("unowned third-party file under reserved root", result.stderr)


if __name__ == "__main__":
    unittest.main()
