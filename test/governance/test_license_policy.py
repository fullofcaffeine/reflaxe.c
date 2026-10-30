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

    def run_generated_policy(self, package_root: Path) -> subprocess.CompletedProcess[str]:
        return subprocess.run(
            [sys.executable, str(SCRIPT), "--root", str(ROOT), "--quiet", "--generated-package-root", str(package_root)],
            text=True,
            capture_output=True,
            check=False,
        )

    def make_generated_package(self, destination: Path, *, with_runtime: bool) -> None:
        notice = Path("LICENSES/HAXE-C-OUTPUT-SUPPORT-MIT.txt")
        (destination / notice).parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT / notice, destination / notice)
        artifacts: list[dict[str, object]] = []

        def add(path: str, contents: bytes, production: str, **source: str) -> None:
            target = destination / path
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(contents)
            if production == "compiled-program":
                licensing = {
                    "licenseConcluded": "NOASSERTION",
                    "projectCopyrightClaim": False,
                    "rationale": "user-license-not-declared",
                    "noticeFiles": [],
                }
            elif production == "compiler-sidecar":
                licensing = {
                    "licenseConcluded": "MIT",
                    "projectCopyrightClaim": True,
                    "rationale": "compiler-output-support",
                    "noticeFiles": [notice.as_posix()],
                }
            else:
                licensing = {
                    "licenseConcluded": "MIT",
                    "projectCopyrightClaim": True,
                    "rationale": "audited-output-support-copy",
                    "noticeFiles": [notice.as_posix()],
                }
            artifacts.append(
                {
                    "path": path,
                    "kind": "runtime-source" if production == "repository-copy" else "source",
                    "sha256": hashlib.sha256(contents).hexdigest(),
                    "provenance": {"production": production, **source},
                    "licensing": licensing,
                }
            )

        add("src/program.c", b"int main(void) { return 0; }\n", "compiled-program")
        if with_runtime:
            source_path = "runtime/hxrt/src/allocator.c"
            contents = (ROOT / source_path).read_bytes()
            digest = hashlib.sha256(contents).hexdigest()
            add(
                "runtime/src/allocator.c",
                contents,
                "repository-copy",
                sourcePath=source_path,
                sourceSha256=digest,
            )
        notice_bytes = (destination / notice).read_bytes()
        artifacts.append(
            {
                "path": notice.as_posix(),
                "kind": "license-notice",
                "sha256": hashlib.sha256(notice_bytes).hexdigest(),
                "provenance": {"production": "compiler-sidecar"},
                "licensing": {
                    "licenseConcluded": "MIT",
                    "projectCopyrightClaim": True,
                    "rationale": "compiler-output-support",
                    "noticeFiles": [notice.as_posix()],
                },
            }
        )
        manifest = {
            "schemaVersion": 2,
            "licensing": {
                "decision": "haxe_c-od2.5",
                "spdxLicenseListVersion": "3.28.0",
                "licenseConcluded": "MIT",
                "noticeFiles": [notice.as_posix()],
            },
            "artifacts": artifacts,
        }
        (destination / "hxc.manifest.json").write_text(json.dumps(manifest), encoding="utf-8")

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

    def test_runtime_free_generated_package_passes(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.make_generated_package(package_root, with_runtime=False)
            result = self.run_generated_policy(package_root)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_runtime_linked_generated_package_passes(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.make_generated_package(package_root, with_runtime=True)
            result = self.run_generated_policy(package_root)
            self.assertEqual(result.returncode, 0, result.stderr)

    def test_generated_package_missing_output_notice_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.make_generated_package(package_root, with_runtime=False)
            (package_root / "LICENSES/HAXE-C-OUTPUT-SUPPORT-MIT.txt").unlink()
            result = self.run_generated_policy(package_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("generated package is missing required notice", result.stderr)

    def test_generated_package_cannot_claim_mit_for_user_program(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            package_root = Path(temporary)
            self.make_generated_package(package_root, with_runtime=False)
            manifest_path = package_root / "hxc.manifest.json"
            manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
            manifest["artifacts"][0]["licensing"]["licenseConcluded"] = "MIT"
            manifest_path.write_text(json.dumps(manifest), encoding="utf-8")
            result = self.run_generated_policy(package_root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("licensing does not match its validated origin", result.stderr)


if __name__ == "__main__":
    unittest.main()
