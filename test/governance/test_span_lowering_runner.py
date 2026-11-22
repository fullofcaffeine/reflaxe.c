"""Protect the distinction between typed iterator inventory and execution."""

from __future__ import annotations

import importlib.util
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
RUNNER_PATH = ROOT / "test/span_lowering/run.py"
SPEC = importlib.util.spec_from_file_location("span_lowering_runner", RUNNER_PATH)
if SPEC is None or SPEC.loader is None:
    raise RuntimeError("could not load the span-lowering runner")
RUNNER = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = RUNNER
SPEC.loader.exec_module(RUNNER)


class SpanLoweringRunnerTest(unittest.TestCase):
    """Keep type declarations while executable iterator shapes fail closed."""

    def test_typed_iterator_inventory_is_not_execution(self) -> None:
        RUNNER.assert_no_retained_iterator_execution(
            "\n".join(
                (
                    '  type "type.haxe-iterator.abc" name="Iterator<int32_t>" kind=reference @"Main.hx":1:1-1:2',
                    '  instance "instance.haxe-iterator.abc" declaration="type.haxe-iterator.abc" arguments=[i32] representation=managed("iterator") @"Main.hx":1:1-1:2',
                    '  instruction "instruction.0" span-loop-element',
                )
            )
        )

    def test_executable_iterator_shapes_remain_rejected(self) -> None:
        invalid_shapes = (
            '  function "function.Iterator.hasNext"',
            '  instruction call dispatch=direct("next")',
            '  instruction call dispatch=direct("c.ConstSpan.iterator")',
            '  local "local.0" type=iterator-object',
        )
        for invalid in invalid_shapes:
            with self.subTest(invalid=invalid):
                with self.assertRaisesRegex(
                    RUNNER.SpanLoweringFailure, "retained iterator machinery"
                ):
                    RUNNER.assert_no_retained_iterator_execution(invalid)


if __name__ == "__main__":
    unittest.main()
