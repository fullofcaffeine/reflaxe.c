from __future__ import annotations

import copy
import json
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "scripts/ci"))
import check_diagnostic_policy as policy  # noqa: E402


class DiagnosticPolicyTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls) -> None:
        cls.catalog = json.loads((ROOT / policy.CATALOG_RELATIVE).read_text(encoding="utf-8"))
        cls.ids = [entry["id"] for entry in cls.catalog["diagnostics"]]

    def test_repository_policy_is_valid(self) -> None:
        self.assertEqual(policy.validate(ROOT), [])

    def test_typed_registry_drift_is_rejected(self) -> None:
        errors = policy.validate_catalog_document(
            self.catalog,
            enum_ids=self.ids[:-1],
            referenced_ids=self.ids,
        )
        self.assertTrue(any("typed Haxe diagnostic registry differs" in error for error in errors))

    def test_range_gap_is_rejected(self) -> None:
        catalog = copy.deepcopy(self.catalog)
        catalog["ranges"][4]["start"] = "HXC4001"
        errors = policy.validate_catalog_document(catalog)
        self.assertTrue(any("ranges must be contiguous" in error for error in errors))

    def test_internal_and_unsupported_kinds_cannot_collapse(self) -> None:
        catalog = copy.deepcopy(self.catalog)
        next(entry for entry in catalog["diagnostics"] if entry["id"] == "HXC9000")["kind"] = "unsupported-source"
        errors = policy.validate_catalog_document(catalog)
        self.assertTrue(any("HXC9000 must remain" in error for error in errors))

    def test_static_initialization_cycle_has_its_own_stable_id(self) -> None:
        definition = next(
            entry for entry in self.catalog["diagnostics"] if entry["id"] == "HXC1002"
        )
        self.assertEqual(definition["name"], "static-initialization-cycle")
        self.assertEqual(definition["phase"], "lowering")
        self.assertEqual(definition["kind"], "unsupported-source")
        self.assertEqual(definition["allowedSeverities"], ["error"])

    def test_disallowed_event_severity_and_missing_source_are_rejected(self) -> None:
        definition = next(entry for entry in self.catalog["diagnostics"] if entry["id"] == "HXC1001")
        event = {
            "schemaVersion": 1,
            "id": definition["id"],
            "severity": "warning",
            "phase": definition["phase"],
            "kind": definition["kind"],
            "profile": "portable",
            "message": "unsupported node",
            "remediation": definition["remediation"],
            "context": "expression",
            "notes": [],
            "relatedLocations": [],
        }
        errors = policy.validate_event(event, self.catalog)
        self.assertTrue(any("severity is not allowed" in error for error in errors))
        self.assertTrue(any("missing=['source']" in error for error in errors))


if __name__ == "__main__":
    unittest.main()
