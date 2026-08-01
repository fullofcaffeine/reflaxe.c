from __future__ import annotations

import copy
import importlib.util
import json
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CHECKER = ROOT / "scripts/ci/check_fixture_policy.py"


def load_checker():
    spec = importlib.util.spec_from_file_location("hxc_fixture_policy_subject", CHECKER)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {CHECKER}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


def read_json(path: str):
    return json.loads((ROOT / path).read_text(encoding="utf-8"))


class TestSurfaceScorecardTests(unittest.TestCase):
    def setUp(self) -> None:
        self.checker = load_checker()
        self.scorecards = read_json("docs/specs/test-surface-scorecards.json")
        self.capabilities = read_json("docs/specs/bootstrap-inventory.json")
        self.scripts = read_json("package.json")["scripts"]
        beads_plan = read_json("docs/specs/beads-plan.json")
        self.stable_owner_ids = {
            entry["key"]
            for section in (beads_plan["epics"], beads_plan["tasks"])
            for entry in section
        }

    def validate_scorecards(self, scorecards) -> list[str]:
        errors: list[str] = []
        self.checker.validate_surface_scorecards(
            scorecards,
            self.capabilities,
            self.scripts,
            self.stable_owner_ids,
            errors,
        )
        return errors

    def test_current_scorecards_reference_real_independent_evidence(self) -> None:
        self.assertEqual(self.validate_scorecards(self.scorecards), [])

    def test_missing_surface_cannot_be_hidden_by_other_green_scorecards(self) -> None:
        mutated = copy.deepcopy(self.scorecards)
        mutated["surfaces"] = mutated["surfaces"][:-1]
        self.assertTrue(
            any("must be exactly" in error for error in self.validate_scorecards(mutated))
        )

    def test_unknown_capability_and_missing_evidence_fail_closed(self) -> None:
        mutated = copy.deepcopy(self.scorecards)
        surface = mutated["surfaces"][0]
        surface["capabilityIds"].append("invented-green-capability")
        surface["evidence"][0]["path"] = "test/does-not-exist/case.json"
        errors = self.validate_scorecards(mutated)
        self.assertTrue(any("unknown capabilities" in error for error in errors))
        self.assertTrue(any("evidence is missing" in error for error in errors))

    def test_required_claim_boundary_cannot_be_omitted(self) -> None:
        mutated = copy.deepcopy(self.scorecards)
        del mutated["surfaces"][0]["residualRisks"]
        self.assertTrue(
            any("omitted required fields" in error for error in self.validate_scorecards(mutated))
        )

    def test_schema_authority_and_commands_fail_closed(self) -> None:
        mutated = copy.deepcopy(self.scorecards)
        mutated["inventedAuthority"] = "green"
        surface = mutated["surfaces"][0]
        surface["archetype"] = "invented"
        surface["releaseCommand"] = 7
        surface["fullBackstopCommand"] = "npm run invented-green"
        surface["ownerBeads"].append("E99.T99")
        errors = self.validate_scorecards(mutated)
        for expected in (
            "undeclared root fields",
            "must use archetype",
            "releaseCommand must be a string or null",
            "canonical full backstop command",
            "unknown stable Beads owners",
        ):
            self.assertTrue(any(expected in error for error in errors), errors)

    def test_policy_only_owner_cannot_enter_a_product_scorecard(self) -> None:
        for owner_field in ("focusedOwners", "systemOwners"):
            with self.subTest(owner_field=owner_field):
                mutated = copy.deepcopy(self.scorecards)
                mutated["surfaces"][0][owner_field].append("test:fixture-policy")
                self.assertTrue(
                    any(
                        "contains policy-only owners" in error
                        for error in self.validate_scorecards(mutated)
                    )
                )

    def test_example_cannot_omit_oracle_provenance(self) -> None:
        case = read_json("examples/hello/case.json")
        del case["oracleProvenance"]
        errors: list[str] = []
        self.checker.validate_case(
            ROOT / "examples/hello/case.json",
            case,
            expected_role="example",
            errors=errors,
        )
        self.assertTrue(any("must declare oracleProvenance" in error for error in errors))

    def test_example_rejects_duplicate_surfaces_and_extra_fields(self) -> None:
        case = read_json("examples/hello/case.json")
        case["productSurfaces"].append(case["productSurfaces"][0])
        case["example"]["inventedClaim"] = "green"
        errors: list[str] = []
        self.checker.validate_case(
            ROOT / "examples/hello/case.json",
            case,
            expected_role="example",
            errors=errors,
        )
        self.assertTrue(any("productSurfaces must be unique" in error for error in errors))
        self.assertTrue(any("example fields must match" in error for error in errors))

    def test_example_surfaces_must_match_its_scorecard_owner(self) -> None:
        case = read_json("examples/hello/case.json")
        case["productSurfaces"].append("c-abi-native-ffi")
        errors: list[str] = []
        self.checker.validate_case(
            ROOT / "examples/hello/case.json",
            case,
            expected_role="example",
            errors=errors,
        )
        self.assertTrue(any("must exactly match scorecard ownership" in error for error in errors))

    def test_runner_expectation_and_artifact_schema_fail_closed(self) -> None:
        case = read_json("examples/hello/case.json")
        case["runner"]["invented"] = True
        case["runner"]["timeoutSeconds"] = 0
        case["expected"]["invented"] = "green"
        case["expected"]["exitCode"] = True
        case["expected"]["artifacts"][0]["invented"] = "green"
        errors: list[str] = []
        self.checker.validate_case(
            ROOT / "examples/hello/case.json",
            case,
            expected_role="example",
            errors=errors,
        )
        for expected in (
            "runner has undeclared fields",
            "timeoutSeconds must be an integer from 1 to 3600",
            "expected must declare an integer exitCode",
            "expected has undeclared fields",
            "artifact",
        ):
            self.assertTrue(any(expected in error for error in errors), errors)


if __name__ == "__main__":
    unittest.main()
