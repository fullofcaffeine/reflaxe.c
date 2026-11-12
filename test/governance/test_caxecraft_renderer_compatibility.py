"""Keep unsupported GNU forwarding declarations outside renderer coverage.

These native controls exercise the ordinary extraction and graph owners. A
successful parser compatibility step must never authorize the affected wrapper
as a measured call, marker owner, or reviewed foreign leaf.
"""
from pathlib import Path
import json
import shutil
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path[:0] = [str(ROOT), str(ROOT / "examples/caxecraft")]
import renderer_call_coverage as coverage

FORWARD = """
int sink(const char *, ...) __attribute__((format(printf, 1, 2)));
extern __inline __attribute__((__always_inline__, __gnu_inline__))
int forward(const char *format, ...) {
  return sink(format, __builtin_va_arg_pack());
}
"""
RETAINED_EVIDENCE: Path | None = None
MARKERS = "void begin(void); void end(void);\n"


class ForwardingContracts(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="renderer-forwarding-")
        self.addCleanup(self.temporary.cleanup)
        self.root = (RETAINED_EVIDENCE / self._testMethodName if RETAINED_EVIDENCE
                     else Path(self.temporary.name))
        self.root.mkdir(parents=True, exist_ok=True)
        compiler = shutil.which("clang")
        self.assertIsNotNone(compiler, "native coverage controls require Clang")
        self.compiler = Path(compiler)

    def extract(self, name, source, *, system_forwarder=True):
        if system_forwarder and FORWARD in source:
            # Match the real GCC-header boundary. Clang omits source-edit fixits
            # inside system headers; ordinary-source fixits remain unsupported.
            (self.root / "forward.h").write_text("#pragma GCC system_header\n" + FORWARD)
            source = source.replace(FORWARD, '#include "forward.h"\n')
        path = self.root / (name + ".c")
        path.write_text(source)
        return coverage.extract_unit(path, compiler=self.compiler,
                                     flags=("-std=c11",), cwd=self.root,
                                     evidence=self.root / name)

    def prove(self, units, leaves=()):
        return coverage.prove_interval(units, begin="begin", end="end",
                                       forbidden={"allocate"}, reviewed_leaves=set(leaves))

    def test_unused_forwarder_extracts_without_becoming_a_leaf(self):
        unit = self.extract("forward", FORWARD + MARKERS +
                            "void measured(void) { begin(); end(); }\n")
        self.assertEqual(self.prove({"forward": unit})["functions"], [])
        record = json.loads(unit.receipt.read_text())
        self.assertEqual(record["status"], "success")
        self.assertEqual([item["returncode"] for item in record["commands"]], [0] * 4)
        self.assertTrue(all(item["timeout"] == 20 for item in record["commands"]))
        self.assertIn(b"__builtin_va_arg_pack", (self.root / "forward/preprocess.stdout").read_bytes())

    def test_cross_unit_call_cannot_use_a_reviewed_leaf_escape(self):
        declaration = self.extract("forward", FORWARD)
        caller = self.extract("caller", MARKERS + "int forward(const char *, ...);\n"
                              "void measured(void) { begin(); forward(\"%s\", \"x\"); end(); }\n")
        for units in ({"caller": caller, "forward": declaration},
                      {"forward": declaration, "caller": caller}):
            with self.assertRaisesRegex(coverage.CoverageFailure, "unproved declaration"):
                self.prove(units, leaves={"forward", "sink"})
            with self.assertRaises(coverage.CoverageFailure):
                coverage.prove_interval(units, begin="forward", end="end",
                                        forbidden=set(), reviewed_leaves={"forward", "sink"})

    def test_allocator_association_extracts_but_stays_excluded(self):
        declarations = ("extern int dispose(void *);\n"
                        "extern void *make(void) __attribute__((malloc)) "
                        "__attribute__((malloc(dispose, 1)));\n")
        unused = self.extract("unused", declarations + MARKERS +
                              "void measured(void) { begin(); end(); }\n")
        self.assertEqual(self.prove({"unused": unused})["functions"], [])
        ledger = json.loads((self.root / "unused/diagnostic-decisions.json").read_text())
        self.assertEqual([entry["certificate"]["kind"] for entry in ledger["decisions"]], ["malloc"])
        used = self.extract("used", declarations + MARKERS +
                            "void measured(void) { begin(); (void)make(); end(); }\n")
        with self.assertRaisesRegex(coverage.CoverageFailure, "unproved declaration"):
            self.prove({"used": used}, leaves={"make"})

    def test_invalid_deallocator_index_cannot_receive_a_certificate(self):
        source = ("extern int dispose(void *);\n"
                  "extern void *make(void) __attribute__((malloc(dispose, 0)));\n")
        with self.assertRaises(coverage.CoverageFailure):
            self.extract("invalid", source)

    def test_unmodified_format_warning_is_not_a_forwarding_certificate(self):
        source = "int sink(const char *, ...) __attribute__((format(printf, 1, 2)));\n"
        source += "int ordinary(const char *format) { return sink(format); }\n"
        with self.assertRaises(coverage.CoverageFailure):
            self.extract("ordinary", source)

    def test_source_file_fixit_note_remains_fatal(self):
        with self.assertRaisesRegex(coverage.CoverageFailure, "fixit"):
            self.extract("source", FORWARD, system_forwarder=False)

    def test_extra_statement_cannot_be_erased_by_compatibility(self):
        source = "void allocate(void);\n" + FORWARD.replace(
            "return sink", "allocate(); return sink")
        with self.assertRaises(coverage.CoverageFailure):
            self.extract("effect", source)


if __name__ == "__main__":
    unittest.main()
