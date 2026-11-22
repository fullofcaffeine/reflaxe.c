#!/usr/bin/env python3
"""Focused contract tests for Tier-1 platform planning and aggregation."""

from __future__ import annotations

import copy
import importlib.util
import json
import sys
import tempfile
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
MODULE_PATH = ROOT / "scripts/ci/platform_matrix.py"
SPEC = importlib.util.spec_from_file_location("hxc_platform_matrix", MODULE_PATH)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError("cannot load platform matrix module")
platform_matrix = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = platform_matrix
SPEC.loader.exec_module(platform_matrix)


class PlatformMatrixTests(unittest.TestCase):
    """Reject matrix omissions and incomplete or mixed-revision evidence."""

    def setUp(self) -> None:
        self.value = json.loads(platform_matrix.MATRIX_PATH.read_text(encoding="utf-8"))

    def load_mutation(self, value: object) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-platform-matrix-test-") as temporary:
            path = Path(temporary) / "matrix.json"
            path.write_text(json.dumps(value), encoding="utf-8")
            platform_matrix.load_matrix(path)

    def test_authoritative_matrix_is_complete(self) -> None:
        matrix, lanes = platform_matrix.load_matrix()
        self.assertEqual(matrix["matrixId"], "hxc-tier1-v1")
        self.assertEqual(
            tuple(lane.identifier for lane in lanes), platform_matrix.EXPECTED_LANES
        )

    def test_required_lane_cannot_disappear(self) -> None:
        changed = copy.deepcopy(self.value)
        changed["lanes"].pop()
        with self.assertRaisesRegex(platform_matrix.MatrixFailure, "lane set"):
            self.load_mutation(changed)

    def test_native_and_emulated_evidence_are_distinct(self) -> None:
        changed = copy.deepcopy(self.value)
        changed["lanes"][-1]["evidence"].remove("emulated-run")
        with self.assertRaisesRegex(platform_matrix.MatrixFailure, "emulated-run"):
            self.load_mutation(changed)

    def test_unknown_combination_fails_during_planning(self) -> None:
        with self.assertRaisesRegex(platform_matrix.MatrixFailure, "unsupported"):
            platform_matrix.lane_by_id("linux-riscv64-pcc")

    def write_reports(self, root: Path, *, revision: str = "abc123") -> None:
        matrix, lanes = platform_matrix.load_matrix()
        for lane in lanes:
            path = root / lane.identifier / f"{lane.identifier}.json"
            path.parent.mkdir(parents=True)
            path.write_text(
                json.dumps(
                    {
                        "schemaVersion": 1,
                        "matrixId": matrix["matrixId"],
                        "lane": lane.value,
                        "sourceRevision": revision,
                        "status": "passed",
                        "observedEvidence": list(lane.evidence),
                        "observation": {
                            "runnerOs": "test-os",
                            "runnerArchitecture": "test-architecture",
                            "toolchainVersion": "test-toolchain 1.0",
                            "execution": lane.value["execution"],
                        },
                    }
                ),
                encoding="utf-8",
            )

    def test_aggregate_requires_every_lane(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-platform-reports-") as temporary:
            root = Path(temporary)
            self.write_reports(root)
            missing = root / platform_matrix.EXPECTED_LANES[-1]
            for path in missing.iterdir():
                path.unlink()
            missing.rmdir()
            with self.assertRaisesRegex(platform_matrix.MatrixFailure, "exactly one"):
                platform_matrix.aggregate(root, root / "aggregate.json")

    def test_aggregate_rejects_mixed_revisions(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-platform-reports-") as temporary:
            root = Path(temporary)
            self.write_reports(root)
            identifier = platform_matrix.EXPECTED_LANES[-1]
            path = root / identifier / f"{identifier}.json"
            value = json.loads(path.read_text(encoding="utf-8"))
            value["sourceRevision"] = "different"
            path.write_text(json.dumps(value), encoding="utf-8")
            with self.assertRaisesRegex(platform_matrix.MatrixFailure, "different"):
                platform_matrix.aggregate(root, root / "aggregate.json")

    def test_aggregate_archives_normalized_matrix_metadata(self) -> None:
        with tempfile.TemporaryDirectory(prefix="hxc-platform-reports-") as temporary:
            root = Path(temporary)
            self.write_reports(root)
            output = root / "release" / "platform-matrix.json"
            platform_matrix.aggregate(root, output)
            value = json.loads(output.read_text(encoding="utf-8"))
            self.assertEqual(value["status"], "passed")
            self.assertEqual(value["sourceRevision"], "abc123")
            self.assertEqual(len(value["lanes"]), len(platform_matrix.EXPECTED_LANES))


if __name__ == "__main__":
    unittest.main()
