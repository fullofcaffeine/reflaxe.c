"""Focused contracts for the path-free Caxecraft incremental edit profiler."""

from __future__ import annotations

import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CAXECRAFT = ROOT / "examples/caxecraft"
sys.path.insert(0, str(CAXECRAFT))
from profile_incremental_edit import (  # noqa: E402
    IncrementalEditProfileFailure,
    artifact_diff,
    hxcir_function_sections,
    hxcir_module_sections,
    replace_source_classpath,
    section_diff,
)


SAMPLE = """hxcir schema=20
dispatch
end dispatch
module "alpha.Main" @"alpha/Main.hx":1:1-3:1
  type "type.alpha" name="Alpha" kind=primitive @synthetic
  function "function.alpha.one" name="one" returns=i32 failure=infallible entry="entry" @synthetic
    block "entry" @synthetic
      terminator return value=none cleanup=[] @synthetic
    end block "entry"
  end function "function.alpha.one"
end module "alpha.Main"
module "beta.Main" @"beta/Main.hx":1:1-3:1
  function "function.beta.two" name="two" returns=i32 failure=infallible entry="entry" @synthetic
    block "entry" @synthetic
      terminator return value=none cleanup=[] @synthetic
    end block "entry"
  end function "function.beta.two"
end module "beta.Main"
"""


class IncrementalEditProfileTest(unittest.TestCase):
    """Keep the profiler strict without running the full playable in governance."""

    def test_function_sections_retain_non_function_semantics(self) -> None:
        functions, skeleton = hxcir_function_sections(SAMPLE)
        self.assertEqual(
            set(functions),
            {"function.alpha.one", "function.beta.two"},
        )
        self.assertIn('type "type.alpha"', skeleton)
        self.assertIn('module "alpha.Main"', skeleton)
        self.assertNotIn('function "function.alpha.one"', skeleton)
        changed = SAMPLE.replace(
            "return value=none", 'return value="value.changed"', 1
        )
        changed_functions, changed_skeleton = hxcir_function_sections(changed)
        difference = section_diff(functions, changed_functions)
        self.assertEqual(difference["changed"], ["function.alpha.one"])
        self.assertEqual(
            difference["unchanged"], ["function.beta.two"]
        )
        self.assertEqual(skeleton, changed_skeleton)

    def test_module_sections_retain_schema_and_dispatch(self) -> None:
        modules, skeleton = hxcir_module_sections(SAMPLE)
        self.assertEqual(set(modules), {"alpha.Main", "beta.Main"})
        self.assertEqual(
            skeleton, "hxcir schema=20\ndispatch\nend dispatch\n"
        )

    def test_unclosed_section_fails_closed(self) -> None:
        malformed = SAMPLE.replace(
            '  end function "function.alpha.one"\n', ""
        )
        with self.assertRaises(IncrementalEditProfileFailure):
            hxcir_function_sections(malformed)

    def test_artifact_diff_covers_added_removed_changed_and_reused(self) -> None:
        difference = artifact_diff(
            {
                "changed.c": b"before",
                "removed.h": b"removed",
                "same.json": b"same",
            },
            {
                "added.h": b"added",
                "changed.c": b"after",
                "same.json": b"same",
            },
        )
        self.assertEqual(difference["added"], ["added.h"])
        self.assertEqual(difference["removed"], ["removed.h"])
        self.assertEqual(difference["changed"], ["changed.c"])
        self.assertEqual(difference["unchanged"], ["same.json"])

    def test_source_classpath_replacement_is_exact(self) -> None:
        source = Path("/temporary/source")
        result = replace_source_classpath(
            ("-cp", "src", "-cp", "/compiler/src", "-main", "Main"),
            source,
        )
        self.assertEqual(result[1], str(source))
        self.assertEqual(result[3], "/compiler/src")
        self.assertIn("reflaxe_c_incremental_input_report", result)
        with self.assertRaises(IncrementalEditProfileFailure):
            replace_source_classpath(("-cp", "/compiler/src"), source)


if __name__ == "__main__":
    unittest.main()
