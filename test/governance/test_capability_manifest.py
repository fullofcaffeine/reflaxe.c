from __future__ import annotations

import json
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "scripts/ci/check_capability_manifest.py"
MANIFEST = "docs/specs/bootstrap-inventory.json"


def json_text_object(text: str, label: str) -> dict[str, object]:
    raw: object = json.loads(text)
    if not isinstance(raw, dict):
        raise AssertionError(f"{label} did not contain an object")
    result: dict[str, object] = {}
    for key, value in raw.items():
        if not isinstance(key, str):
            raise AssertionError(f"{label} contained a non-string key")
        result[key] = value
    return result


def json_object(path: Path) -> dict[str, object]:
    return json_text_object(path.read_text(encoding="utf-8"), str(path))


def object_list(document: dict[str, object], key: str) -> list[dict[str, object]]:
    raw = document.get(key)
    if not isinstance(raw, list):
        raise AssertionError(f"{key} was not a list")
    result: list[dict[str, object]] = []
    for index, value in enumerate(raw):
        if not isinstance(value, dict):
            raise AssertionError(f"{key}[{index}] was not an object")
        item: dict[str, object] = {}
        for item_key, item_value in value.items():
            if not isinstance(item_key, str):
                raise AssertionError(f"{key}[{index}] had a non-string key")
            item[item_key] = item_value
        result.append(item)
    return result


class CapabilityManifestTests(unittest.TestCase):
    def run_policy(
        self, root: Path = ROOT, *arguments: str
    ) -> subprocess.CompletedProcess[str]:
        command = [sys.executable, str(SCRIPT), "--root", str(root)]
        if arguments:
            command.extend(arguments)
        else:
            command.append("--quiet")
        return subprocess.run(
            command,
            text=True,
            capture_output=True,
            check=False,
        )

    def make_policy_root(self, destination: Path) -> None:
        copied = (
            "AGENTS.md",
            "README.md",
            "docs/PRD.md",
            "docs/specs/beads-plan.json",
            MANIFEST,
            "docs/specs/bootstrap-inventory.schema.json",
        )
        for relative in copied:
            target = destination / relative
            target.parent.mkdir(parents=True, exist_ok=True)
            shutil.copyfile(ROOT / relative, target)

        document = json_object(ROOT / MANIFEST)
        paths: set[str] = set()
        for capability in object_list(document, "capabilities"):
            evidence = capability.get("evidence")
            if not isinstance(evidence, list):
                raise AssertionError("capability evidence was not a list")
            for value in evidence:
                if not isinstance(value, str):
                    raise AssertionError("capability evidence was not a string")
                paths.add(value)
        for ledger in object_list(document, "ledgers"):
            value = ledger.get("path")
            if not isinstance(value, str):
                raise AssertionError("ledger path was not a string")
            paths.add(value)
        for relative in paths:
            target = destination / relative
            if not target.exists():
                target.parent.mkdir(parents=True, exist_ok=True)
                target.write_text("test evidence\n", encoding="utf-8")

    def write_manifest(self, root: Path, document: dict[str, object]) -> None:
        (root / MANIFEST).write_text(
            json.dumps(document, ensure_ascii=False, indent=2) + "\n",
            encoding="utf-8",
        )

    def test_repository_manifest_passes(self) -> None:
        result = self.run_policy()
        self.assertEqual(result.returncode, 0, result.stderr)

    def test_doctor_contract_is_derived_and_explicitly_unavailable(self) -> None:
        result = self.run_policy(ROOT, "--print-doctor-contract")
        self.assertEqual(result.returncode, 0, result.stderr)
        contract = json_text_object(result.stdout, "doctor contract")
        self.assertEqual(contract["command"], "hxc doctor")
        self.assertEqual(contract["status"], "unsupported")
        self.assertIs(contract["implemented"], False)
        self.assertEqual(contract["ownerBeads"], "E8.T06")
        self.assertIs(contract["manifestConsumerRequired"], True)

    def test_unknown_manifest_field_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            document = json_object(root / MANIFEST)
            document["futureGuess"] = True
            self.write_manifest(root, document)
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("manifest has unknown fields: futureGuess", result.stderr)

    def test_schema_and_checker_statuses_cannot_drift(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            schema_path = root / "docs/specs/bootstrap-inventory.schema.json"
            schema = json_object(schema_path)
            definitions = schema.get("$defs")
            self.assertIsInstance(definitions, dict)
            capability = definitions.get("capability")
            self.assertIsInstance(capability, dict)
            properties = capability.get("properties")
            self.assertIsInstance(properties, dict)
            status = properties.get("status")
            self.assertIsInstance(status, dict)
            status["enum"] = [
                "implemented",
                "scaffold-only",
                "preview",
                "unsupported",
            ]
            schema_path.write_text(
                json.dumps(schema, ensure_ascii=False, indent=2) + "\n",
                encoding="utf-8",
            )
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("status enum drifted from the checker", result.stderr)

    def test_invalid_status_disposition_pair_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            document = json_object(root / MANIFEST)
            capabilities = object_list(document, "capabilities")
            target = next(item for item in capabilities if item.get("id") == "bindgen")
            target["status"] = "implemented"
            document["capabilities"] = capabilities
            self.write_manifest(root, document)
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(
                "status implemented cannot use disposition not-exposed",
                result.stderr,
            )

    def test_unknown_capability_area_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            document = json_object(root / MANIFEST)
            capabilities = object_list(document, "capabilities")
            capabilities[0]["area"] = "wishful-thinking"
            document["capabilities"] = capabilities
            self.write_manifest(root, document)
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(
                "capabilities[0].area is invalid: wishful-thinking",
                result.stderr,
            )

    def test_missing_evidence_file_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            missing = "test/governance/test_capability_manifest.py"
            (root / missing).unlink()
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(f"does not name a real repository file: {missing}", result.stderr)

    def test_readme_drift_is_rejected_and_writer_repairs_it(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            readme = root / "README.md"
            text = readme.read_text(encoding="utf-8")
            old = "The checked-in Beads plan validates and previews deterministically"
            self.assertIn(old, text)
            readme.write_text(text.replace(old, "The plan probably works"), encoding="utf-8")
            rejected = self.run_policy(root)
            self.assertNotEqual(rejected.returncode, 0)
            self.assertIn("README.md capability block drifted", rejected.stderr)
            repaired = self.run_policy(root, "--write-readme")
            self.assertEqual(repaired.returncode, 0, repaired.stderr)

    def test_premature_product_claim_is_rejected(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            readme = root / "README.md"
            with readme.open("a", encoding="utf-8") as output:
                output.write("\nThis compiler is production-ready.\n")
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("README.md contains premature claim", result.stderr)

    def test_absent_doctor_contract_rejects_cli_source(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            cli = root / "src/Run.hx"
            cli.parent.mkdir(parents=True, exist_ok=True)
            cli.write_text("class Run {}\n", encoding="utf-8")
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(
                "src/Run.hx exists while the manifest still says hxc doctor is absent",
                result.stderr,
            )

    def test_m0_claim_gate_cannot_be_enabled(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            document = json_object(root / MANIFEST)
            gates = object_list(document, "claimGates")
            gates[0]["claimed"] = True
            document["claimGates"] = gates
            self.write_manifest(root, document)
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn(
                "claim gate cannot be claimed at M0: full-standard-library",
                result.stderr,
            )

    def test_capability_order_is_deterministic(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            self.make_policy_root(root)
            document = json_object(root / MANIFEST)
            capabilities = object_list(document, "capabilities")
            capabilities[0], capabilities[1] = capabilities[1], capabilities[0]
            document["capabilities"] = capabilities
            self.write_manifest(root, document)
            result = self.run_policy(root)
            self.assertNotEqual(result.returncode, 0)
            self.assertIn("capabilities must be sorted by stable ID", result.stderr)


if __name__ == "__main__":
    unittest.main()
