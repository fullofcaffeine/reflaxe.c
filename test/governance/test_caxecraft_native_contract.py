"""Host-only admission checks; source receipt validation keeps its existing owner."""
from __future__ import annotations
import copy
from dataclasses import FrozenInstanceError
import hashlib
import json
from pathlib import Path
import shlex
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / "examples/caxecraft"))
import renderer_native_contract as contract


class NativeTerrainContractTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="native-terrain-contract-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.build = self.root / "build"
        self.build.mkdir()
        self.effective = self.root / "effective"
        self.library = self.root / "raylib.a"
        self.library.write_bytes(b"trusted synthetic library")
        self.lock = json.loads((ROOT / "docs/specs/raylib-provisioning-lock.json").read_text())
        self.report = {"target": {"platform": "macos", "architecture": "arm64",
            "compiler": {"family": "apple-clang", "versionLine": "Apple clang fixture", "target": "arm64-apple-darwin24.0"}},
            "outputs": {}}
        self.entries = []
        # These fixtures exercise recipe admission, not Raylib's build process.
        # Existing provisioning tests own source-tree and library receipt checks.
        for name in ("raudio.c", "rcore.c", "rmodels.c", "rshapes.c", "rtext.c", "rtextures.c"):
            source = str(self.effective / "src" / name)
            argv = ["/usr/bin/clang", *contract.REVIEWED_DEFINES,
                    "-I" + str(self.effective / "src"), *contract.ORDINARY_FLAGS,
                    "-arch", "arm64", "-isysroot", "/trusted/SDK.sdk",
                    "-o", f"CMakeFiles/raylib.dir/{name}.o", "-c", source]
            self.entries.append({"directory": str(self.build / "raylib"), "file": source,
                                 "output": f"raylib/CMakeFiles/raylib.dir/{name}.o", "command": shlex.join(argv)})
        self.source_check = mock.patch.object(contract.provision, "verify_source")
        self.source_check.start(); self.addCleanup(self.source_check.stop)
        self.receipt_check = mock.patch.object(contract.provision, "verify_source_build_receipt",
                                              return_value=contract.provision.TreeIdentity("a" * 64, 6, 100))
        self.receipt = self.receipt_check.start(); self.addCleanup(self.receipt_check.stop)

    def write_recipe(self):
        path = self.build / "compile_commands.json"
        path.write_text(json.dumps(self.entries))
        self.report["outputs"]["compileCommandsSha256"] = hashlib.sha256(path.read_bytes()).hexdigest()

    def verify(self, **overrides):
        self.write_recipe()
        options = dict(lock=self.lock, report=self.report, authority="pinned-source", platform_name="macos",
                       configuration="memory-software", source_root=self.root / "upstream",
                       effective_source_root=self.effective, build_root=self.build, library_file=self.library)
        return contract.verify_native_contract(**{**options, **overrides})

    def test_reviewed_contract_is_immutable_and_reports_bounded_identity(self):
        result = self.verify()
        self.assertEqual(result.leaves, frozenset(("GetTime", "rlSetTexture", "rlBegin", "rlEnd",
            "rlColor4ub", "rlNormal3f", "rlTexCoord2f", "rlVertex3f")))
        self.assertEqual(result.identity_report()["librarySha256"], hashlib.sha256(self.library.read_bytes()).hexdigest())
        self.assertEqual(result.identity_report()["sourceTreeSha256"], "a" * 64)
        result.identity_report()["leaves"] = []
        self.assertEqual(len(result.identity_report()["leaves"]), 8)
        with self.assertRaises(FrozenInstanceError):
            result.identity_json = "{}"
        self.receipt.assert_called_once()

    def test_linux_target_has_no_darwin_operands(self):
        self.report["target"] = {"architecture": "x86_64", "compiler": {
            "family": "gcc", "target": "x86_64-linux-gnu", "versionLine": "gcc fixture"}}
        for entry in self.entries:
            argv = shlex.split(entry["command"])
            index = argv.index("-arch")
            del argv[index:index + 4]
            entry["command"] = shlex.join(argv)
        self.assertEqual(self.verify(platform_name="linux").identity_report()["platform"], "linux")

    def test_changed_defines_fail_even_with_updated_recipe_receipt(self):
        self.entries[0]["command"] = self.entries[0]["command"].replace("-DPLATFORM_MEMORY", "-DPLATFORM_DESKTOP")
        with self.assertRaisesRegex(contract.NativeContractFailure, "defines"):
            self.verify()

    def test_wrong_target_configuration_and_lock_fail(self):
        with self.assertRaises(contract.NativeContractFailure):
            self.verify(configuration="desktop")
        self.report["target"]["compiler"]["target"] = "aarch64-linux-gnu"
        with self.assertRaisesRegex(contract.NativeContractFailure, "target"):
            self.verify()
        changed = copy.deepcopy(self.lock); changed["schemaVersion"] = 999
        with self.assertRaisesRegex(contract.NativeContractFailure, "lock changed"):
            self.verify(lock=changed)

    def test_extra_missing_and_duplicate_units_fail(self):
        original = copy.deepcopy(self.entries)
        for entries in (original[:-1], original + [original[0]], original[:-1] + [original[0]]):
            self.entries = entries
            with self.subTest(count=len(entries)), self.assertRaises(contract.NativeContractFailure):
                self.verify()

    def test_include_root_substitution_fails(self):
        self.entries[0]["command"] = self.entries[0]["command"].replace(str(self.effective / "src"), "/unowned", 1)
        with self.assertRaisesRegex(contract.NativeContractFailure, "owned paths"):
            self.verify()

    def test_unknown_effectful_and_preprocessor_flags_fail(self):
        original = self.entries[0]["command"]
        for flag in ("-DNEW=1", "-UPLATFORM_MEMORY", "-include other.h", "@response",
                     "-fplugin=plugin", "-fsanitize=address", "-pg", "-Wno-error", "-unknown"):
            self.entries[0]["command"] = original + " " + flag
            with self.subTest(flag=flag), self.assertRaises(contract.NativeContractFailure):
                self.verify()

    def test_source_receipt_failure_is_not_bypassed(self):
        self.receipt.side_effect = contract.provision.ProvisionFailure("source drift")
        with self.assertRaisesRegex(contract.NativeContractFailure, "source drift"):
            self.verify()

    def test_stale_recipe_bytes_fail_before_admission(self):
        self.write_recipe()
        with mock.patch.object(self, "write_recipe"):
            (self.build / "compile_commands.json").write_text("[]")
            with self.assertRaisesRegex(contract.NativeContractFailure, "identity mismatch"):
                self.verify()


if __name__ == "__main__":
    unittest.main()
