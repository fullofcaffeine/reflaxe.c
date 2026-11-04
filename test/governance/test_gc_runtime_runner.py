"""Protect the GC runner's isolated cold-render scheduling contract."""

from __future__ import annotations

import importlib.util
import sys
import threading
import unittest
from pathlib import Path
from unittest.mock import patch


ROOT = Path(__file__).resolve().parents[2]
RUNNER = ROOT / "test/runtime/gc/run.py"


def load_runner():
    """Load the script as a module without invoking its command-line entry point."""
    specification = importlib.util.spec_from_file_location(
        "gc_runtime_runner_for_test", RUNNER
    )
    if specification is None or specification.loader is None:
        raise AssertionError("could not load the GC runtime runner")
    module = importlib.util.module_from_spec(specification)
    sys.modules[specification.name] = module
    specification.loader.exec_module(module)
    return module


class GcRuntimeRunnerTests(unittest.TestCase):
    """Keep independent determinism samples concurrent and deterministically ordered."""

    def test_cold_renders_overlap_and_keep_label_order(self) -> None:
        runner = load_runner()
        both_started = threading.Barrier(2)

        def fake_render(label: str):
            both_started.wait(timeout=1)
            return label, []

        with patch.object(runner, "render_generated_root_frame", side_effect=fake_render):
            first, second = runner.render_generated_root_frames()

        self.assertEqual(first, ("first generated root-frame render", []))
        self.assertEqual(second, ("second generated root-frame render", []))


if __name__ == "__main__":
    unittest.main()
