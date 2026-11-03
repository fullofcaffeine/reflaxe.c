"""Host provenance checks and tiny actual-Clang interval coverage controls.

Normal governance discovery runs both classes. For host-only review without a
compiler process: python3 -m unittest ...CoverageHostTests (using this module's
normal governance import path). Every Clang extraction retains a terminal
receipt in its temporary owner; explicit --evidence-dir retains those files.
"""
from __future__ import annotations

import json
from pathlib import Path
import shutil
import shlex
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(ROOT / "examples/caxecraft"))
import renderer_call_coverage as coverage
from renderer_call_coverage import CoverageFailure

PREAMBLE = "void observe_begin(void); void observe_end(void); void allocate(void);\n"
RETAINED_EVIDENCE: Path | None = None


class ClosureContracts(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory(prefix="caxecraft-coverage-")
        self.addCleanup(self.temporary.cleanup)
        self.root = (RETAINED_EVIDENCE / self._testMethodName if RETAINED_EVIDENCE
                     else Path(self.temporary.name))
        self.root.mkdir(parents=True, exist_ok=True)
        compiler = shutil.which("clang")
        if compiler is None:
            self.fail("actual C coverage controls require Clang")
        self.compiler = Path(compiler)

    def test_system_include_next_uses_native_preprocessing_warning_policy(self):
        first, second = self.root / "first", self.root / "second"
        first.mkdir()
        second.mkdir()
        (first / "wrapper.h").write_text('#include_next <wrapper.h>\n')
        (second / "wrapper.h").write_text('typedef int NativeValue;\n')
        source = self.root / "main.c"
        source.write_text(PREAMBLE + '#include <wrapper.h>\n'
                          'void run(void) { observe_begin(); NativeValue x = 0; (void)x; observe_end(); }\n')
        flags = ('-std=c11', '-Werror', '-pedantic', '-isystem', str(first), '-isystem', str(second))
        # The ordinary native syntax check accepts its own system-header policy.
        ordinary = coverage.run_bounded_process(
            [str(self.compiler), *flags, '-fsyntax-only', str(source)],
            cwd=self.root, capture_output=True, timeout=20)
        (self.root / "native.stdout").write_bytes(ordinary.stdout)
        (self.root / "native.stderr").write_bytes(ordinary.stderr)
        self.assertEqual(ordinary.returncode, 0, ordinary.stderr.decode(errors="replace"))
        # Reproduce the former extraction-only failure without changing acceptance.
        extra_warnings = coverage.run_bounded_process(
            [str(self.compiler), *flags, '-Wsystem-headers', '-Werror=vla', '-E', str(source)],
            cwd=self.root, capture_output=True, timeout=20)
        (self.root / "old-preprocess.stderr").write_bytes(extra_warnings.stderr)
        self.assertNotEqual(extra_warnings.returncode, 0)
        self.assertIn(b'-Wgnu-include-next', extra_warnings.stderr)
        unit = coverage.extract_unit(source, compiler=self.compiler, flags=flags,
                                     cwd=self.root, evidence=self.root / "extraction")
        self.assertEqual(self.proof({'main': unit})['functions'], [])
        receipt = json.loads(unit.receipt.read_text())
        self.assertEqual([item['outcome'] for item in receipt['commands']], ['success'] * 4)

    def test_unknown_semantic_pragma_fails_proof_diagnostics(self):
        source = self.root / "main.c"
        source.write_text(PREAMBLE + '#pragma unreviewed_semantic_effect\n'
                          'void run(void) { observe_begin(); observe_end(); }\n')
        with self.assertRaisesRegex(CoverageFailure, "unclassified coverage diagnostic"):
            coverage.extract_unit(source, compiler=self.compiler, flags=('-std=c11',),
                                  cwd=self.root, evidence=self.root / 'extraction')
        receipt = json.loads((self.root / 'extraction/receipt.json').read_text())
        self.assertEqual([item['outcome'] for item in receipt['commands']], ['success'] * 4)

    def test_nested_header_access_context_notes_are_retained(self):
        (self.root / 'outer.h').write_text('#include "inner.h"\n')
        (self.root / 'inner.h').write_text('void helper(const char *p) __attribute__((access(read_only,1)));\n')
        source = self.root / 'main.c'
        source.write_text(PREAMBLE + '#include "outer.h"\n'
                          'void helper(const char *p) {(void)p;}\n'
                          'void run(void) {observe_begin(); helper("x"); observe_end();}\n')
        unit = coverage.extract_unit(source, compiler=self.compiler, flags=('-std=c11',),
                                     cwd=self.root, evidence=self.root / 'extraction')
        self.proof({'main': unit})
        decisions = json.loads((self.root / 'extraction/diagnostic-decisions.json').read_text())['decisions']
        self.assertEqual(len(decisions), 1)
        self.assertEqual(len(decisions[0]['contextNotes']), 2)

    def test_access_metadata_extracts_but_does_not_authorize_effects(self):
        declarations = 'void helper(const char *p) __attribute__((__access__(__read_only__, 1)));\n'
        for name, body in (("plain", "(void)p;"), ("allocation", "(void)p; allocate();"),
                           ("objectsize", "(void)__builtin_dynamic_object_size(p, 0);")):
            units = self.program(**{name: declarations + 'void helper(const char *p) { ' + body + ' }\n'
                                    'void run(void) { observe_begin(); helper("x"); observe_end(); }\n'})
            record = json.loads(units[name].receipt.read_text())
            self.assertEqual(record['status'], 'success')
            ledger = json.loads((self.root / name / 'diagnostic-decisions.json').read_text())
            self.assertEqual(len(ledger['decisions']), 1)
            if name == "plain":
                self.assertIn(("", "helper"), self.proof(units)['functions'])
            else:
                with self.assertRaises(CoverageFailure): self.proof(units)

    def test_serialized_ranges_use_physical_bytes_under_repeated_line_markers(self):
        units = self.program(main='# 100 "repeated.h" 3\n/* café */\n'
            'void first(const char *p) __attribute__((__access__(\n __read_only__,1)));\n'
            '# 100 "repeated.h" 3\n'
            'void second(char *a, const char *b) __attribute__((__access__(__write_only__,1), __access__(__read_only__,2)));\n'
            '# 100 "repeated.h" 3\n'
            'void third(const char *p) __attribute__((__access__(__read_only__,1)));\n')
        self.assertEqual(json.loads(units['main'].receipt.read_text())['status'], 'success')
        data = (self.root / 'main/diagnostic-neutral.i').read_bytes()
        expected = [index for index in range(len(data)) if data.startswith(b'__access__', index)]
        ledger = json.loads((self.root / 'main/diagnostic-decisions.json').read_text())
        self.assertEqual([item['start'] for item in ledger['decisions']], expected)
        self.assertEqual(len(expected), 4)
        for item in ledger['decisions']:
            self.assertEqual(data[item['lastToken']:item['lastToken'] + 1], b')')

    def test_corrupt_serialized_file_fails_reader(self):
        malformed = self.root / 'corrupt.dia'
        malformed.write_bytes(b'incomplete serialized diagnostics')
        reader = Path(coverage.renderer_diagnostics.__file__)
        result = coverage.run_bounded_process(
            [sys.executable, str(reader), str(self.compiler),
             str(coverage.matching_library(self.compiler)), str(malformed)],
            cwd=self.root, capture_output=True, timeout=20)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn(b'invalid serialized diagnostics', result.stderr)

    def test_unreviewed_ignored_attribute_fails_extraction(self):
        with self.assertRaisesRegex(CoverageFailure, "unreviewed"):
            self.program(main='void helper(void) __attribute__((unreviewed_effect));\n'
                         'void run(void) { observe_begin(); helper(); observe_end(); }\n')
        receipt = json.loads((self.root / 'main/receipt.json').read_text())
        self.assertEqual(receipt['commands'][-1]['outcome'], 'success')
        self.assertEqual(receipt['status'], 'failed')

    def test_native_cast_quality_does_not_replace_proof_diagnostics(self):
        header = self.root / 'native.h'
        header.write_text('#pragma GCC system_header\n'
                          'static inline void *pointer(const void *p) {\n'
                          '#pragma GCC diagnostic push\n#pragma GCC diagnostic ignored "-Wcast-qual"\n'
                          'return (void *)p;\n#pragma GCC diagnostic pop\n}\n')
        source = self.root / 'cast.c'
        source.write_text(PREAMBLE + '#include "native.h"\n'
                          'void run(const void *p) { observe_begin(); (void)pointer(p); observe_end(); }\n')
        flags = ('-std=c11', '-Werror', '-Wcast-qual', '-pedantic')
        native = coverage.run_bounded_process([str(self.compiler), *flags, '-fsyntax-only', str(source)],
                                             cwd=self.root, capture_output=True, timeout=20)
        self.assertEqual(native.returncode, 0, native.stderr.decode(errors='replace'))
        unit = coverage.extract_unit(source, compiler=self.compiler, flags=flags,
                                     cwd=self.root, evidence=self.root / 'cast')
        self.assertTrue(self.proof({'cast': unit}))

    def test_native_preprocessor_branch_is_preserved_for_clang_parser(self):
        source = self.root / 'native-branch.c'
        source.write_text(PREAMBLE + '''
void run(void) {
  observe_begin();
#ifdef COVERAGE_NATIVE_BRANCH
  allocate();
#endif
  observe_end();
}
''')
        # A real driver wrapper supplies a native-only predefined macro. The
        # parser must not preprocess again and discard that selected branch.
        driver = self.root / 'native-driver'
        driver.write_text('#!/bin/sh\nexec ' + shlex.quote(str(self.compiler))
                          + ' -DCOVERAGE_NATIVE_BRANCH "$@"\n')
        driver.chmod(0o755)
        native = coverage.extract_unit(source, compiler=self.compiler,
            preprocessor=driver, flags=('-std=c11',), cwd=self.root,
            evidence=self.root / 'native-extraction')
        with self.assertRaisesRegex(CoverageFailure, 'prohibited call'):
            self.proof({'native': native})
        ordinary = coverage.extract_unit(source, compiler=self.compiler,
            flags=('-std=c11',), cwd=self.root, evidence=self.root / 'parser-extraction')
        self.assertEqual(self.proof({'ordinary': ordinary})['functions'], [])

    def program(self, **sources):
        units = {}
        for name, source in sources.items():
            path = self.root / (name + ".c")
            path.write_text(PREAMBLE + source)
            units[name] = coverage.extract_unit(
                path, compiler=self.compiler, flags=("-std=c11", "-ffreestanding", "-Werror"),
                cwd=self.root, evidence=self.root / name,
            )
        return units

    def proof(self, units):
        return coverage.prove_interval(units, begin="observe_begin", end="observe_end",
                                       forbidden={"allocate"}, reviewed_leaves={"clock_read"})

    def test_startup_allocation_is_outside_interval(self):
        program = self.program(main="void run(void) { allocate(); observe_begin(); observe_end(); }")
        self.assertEqual(self.proof(program)["functions"], [])

    def test_transitive_cross_unit_allocation_is_rejected(self):
        program = self.program(main="void helper(void); void run(void) { observe_begin(); helper(); observe_end(); }",
                               other="void helper(void) { allocate(); }")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof(program)

    def test_unexecuted_branch_is_still_checked(self):
        program = self.program(main="void run(void) { observe_begin(); if (0) allocate(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof(program)

    def test_argument_helper_is_checked(self):
        program = self.program(main="int getter(void) { allocate(); return 0; } void draw(int x) { (void)x; } void run(void) { observe_begin(); draw(getter()); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof(program)

    def test_macro_expansion_is_checked(self):
        program = self.program(main="#define WORK() allocate()\nvoid run(void) { observe_begin(); WORK(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof(program)

    def test_unknown_foreign_call_is_rejected(self):
        program = self.program(main="void mystery(void); void run(void) { observe_begin(); mystery(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "unreviewed external"):
            self.proof(program)

    def test_indirect_call_is_rejected(self):
        program = self.program(main="void run(void (*callback)(void)) { observe_begin(); callback(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "indirect call"):
            self.proof(program)

    def test_bookkeeping_and_reviewed_leaf_are_reported(self):
        program = self.program(main="int roots; void clock_read(void); static void push(void) { roots++; } void run(void) { observe_begin(); push(); clock_read(); observe_end(); }")
        proof = self.proof(program)
        self.assertEqual(proof["functions"], [("", "clock_read"), ("main", "push")])
        self.assertEqual(proof["foreignLeaves"], ["clock_read"])

    def test_same_named_static_functions_do_not_alias(self):
        program = self.program(main="static void helper(void) {} void other(void); void run(void) { observe_begin(); helper(); other(); observe_end(); }",
                               other="static void helper(void) { allocate(); } void other(void) { helper(); }")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof(program)

    def test_recursion_terminates_analysis(self):
        program = self.program(main="void recur(int n) { if (n) recur(n-1); } void run(void) { observe_begin(); recur(3); observe_end(); }")
        self.assertEqual(self.proof(program)["functions"], [("", "recur")])

    def test_cleanup_attribute_is_not_silently_omitted(self):
        program = self.program(main="void cleanup(int *x) { (void)x; allocate(); } void helper(void) { int x __attribute__((cleanup(cleanup))) = 0; } void run(void) { observe_begin(); helper(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "implicit operation"):
            self.proof(program)

    def test_conditional_end_is_rejected(self):
        program = self.program(main="void run(int n) { observe_begin(); if (n) observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "one complete interval"):
            self.proof(program)

    def test_duplicate_interval_is_rejected(self):
        program = self.program(main="void run(void) { observe_begin(); observe_end(); observe_begin(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "one complete interval"):
            self.proof(program)

    def test_missing_end_is_rejected(self):
        program = self.program(main="void run(void) { observe_begin(); }")
        with self.assertRaisesRegex(CoverageFailure, "one complete interval"):
            self.proof(program)

    def test_jump_out_and_back_cannot_hide_allocation(self):
        program = self.program(main="void run(void) { observe_begin(); goto work; back:; observe_end(); return; work: allocate(); goto back; }")
        with self.assertRaises(CoverageFailure):
            self.proof(program)

    def test_cleanup_declared_before_interval_is_rejected(self):
        program = self.program(main="void clean(int *x) { (void)x; allocate(); } void run(void) { int x __attribute__((cleanup(clean))) = 0; observe_begin(); return; observe_end(); }")
        with self.assertRaises(CoverageFailure):
            self.proof(program)

    def test_linker_name_cannot_disguise_forbidden_call(self):
        program = self.program(main='void clock_read(void) __asm__("allocate"); void run(void) { observe_begin(); clock_read(); observe_end(); }')
        with self.assertRaises(CoverageFailure):
            self.proof(program)

    def test_thread_local_access_needs_native_proof(self):
        program = self.program(main="extern _Thread_local int value; void run(void) { observe_begin(); int x = value; (void)x; observe_end(); }")
        with self.assertRaises(CoverageFailure):
            self.proof(program)

    def test_forbidden_call_overrides_reviewed_leaf(self):
        units = self.program(main="void run(void) { observe_begin(); allocate(); observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            coverage.prove_interval(units, begin="observe_begin", end="observe_end",
                                    forbidden={"allocate"}, reviewed_leaves={"allocate"})

    def test_atomic_read_and_write_are_not_assumed_call_free(self):
        units = self.program(main="extern _Atomic int value; void run(void) { observe_begin(); int x = value; value = x; observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "atomic"):
            self.proof(units)

    def test_atomic_typedef_is_not_assumed_call_free(self):
        units = self.program(main="typedef _Atomic(int) Counter; extern Counter value; void run(void) { observe_begin(); value = 1; observe_end(); }")
        with self.assertRaisesRegex(CoverageFailure, "atomic"):
            self.proof(units)

    def test_vla_type_effects_fail_extraction(self):
        forms = {
            "declaration": "void run(void) { int a[extent()]; (void)a; }",
            "sizeof": "void run(void) { (void)sizeof(int[extent()]); }",
            "typedef": "void run(void) { typedef int Row[extent()]; }",
            "parameter": "void run(int a[extent()]) { (void)a; }",
        }
        for name, source in forms.items():
            with self.subTest(name=name), self.assertRaises(CoverageFailure):
                self.program(**{name: "int extent(void) { allocate(); return 1; } " + source})
            diagnostics = "".join(p.read_text() for p in (self.root / name).glob("*.stderr"))
            self.assertIn("-Wvla", diagnostics)

    def test_transitive_header_call_is_part_of_the_graph(self):
        (self.root / "inner.h").write_text("static void helper(void) { allocate(); }\n")
        (self.root / "outer.h").write_text('#include "inner.h"\n')
        units = self.program(main='#include "outer.h"\nvoid run(void) { observe_begin(); helper(); observe_end(); }')
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof(units)


    def test_suppressed_vla_forms_fail_source_admission(self):
        forms = {
            "decl_suppressed": "void run(void) { int a[extent()]; (void)a; }",
            "sizeof_suppressed": "void run(void) { (void)sizeof(int[extent()]); }",
            "typedef_suppressed": "void run(void) { typedef int Row[extent()]; }",
            "parameter_suppressed": "void run(int a[extent()]) { (void)a; }",
        }
        for name, form in forms.items():
            source = ('#pragma clang diagnostic ignored "-Wvla"\n'
                      'int extent(void) { allocate(); return 1; } ' + form)
            with self.subTest(name=name), self.assertRaises(CoverageFailure):
                self.program(**{name: source})
            receipt = json.loads((self.root / name / "receipt.json").read_text())
            self.assertEqual(receipt["status"], "failed")
            self.assertEqual(receipt["commands"][-1]["outcome"], "failed")
            self.assertGreater(receipt["removedDiagnosticDirectives"], 0)
            self.assertIn("-Wvla", (self.root / name / "ast.stderr").read_text())


    def test_macro_pragma_suppression_cannot_hide_parameter_bound(self):
        source = ('#define QUIET _Pragma(' + json.dumps('GCC diagnostic ignored "-Wvla"') + ')\n'
                  'QUIET\nint extent(void) { allocate(); return 1; } '
                  'void run(int a[extent()]) { (void)a; }')
        with self.assertRaises(CoverageFailure):
            self.program(main=source)
        receipt = json.loads((self.root / 'main/receipt.json').read_text())
        self.assertGreater(receipt['removedDiagnosticDirectives'], 0)
        self.assertIn('-Wvla', (self.root / 'main/ast.stderr').read_text())

    def test_semantic_pack_and_source_line_markers_survive(self):
        source = ('#pragma clang diagnostic ignored "-Wvla"\n'
                  '#pragma pack(push, 1)\n'
                  'struct Packed { char c; int i; };\n'
                  '#pragma pack(pop)\n'
                  '_Static_assert(sizeof(struct Packed) == sizeof(int) + 1, "pack survived");\n'
                  '_Static_assert(__builtin_LINE() == 7, "source line survived");\n'
                  'void run(void) { observe_begin(); observe_end(); }')
        units = self.program(main=source)
        self.assertEqual(self.proof(units)['functions'], [])
        raw = (self.root / 'main/preprocess.stdout').read_text()
        neutral = (self.root / 'main/diagnostic-neutral.i').read_text()
        self.assertIn('#pragma pack(push, 1)', neutral)
        self.assertIn('#pragma pack(pop)', neutral)
        self.assertEqual([line for line in raw.splitlines() if line.startswith('# ')],
                         [line for line in neutral.splitlines() if line.startswith('# ')])


    def require_final_vla_rejection(self, source: str, flags: tuple[str, ...]) -> None:
        path = self.root / 'main.c'
        path.write_text(PREAMBLE + source)
        evidence = self.root / 'main'
        with self.assertRaises(CoverageFailure):
            coverage.extract_unit(path, compiler=self.compiler, flags=flags,
                                  cwd=self.root, evidence=evidence)
        receipt = json.loads((evidence / 'receipt.json').read_text())
        self.assertEqual([command['outcome'] for command in receipt['commands']],
                         ['success', 'success', 'failed'])
        self.assertIn('-Wvla', (evidence / 'ast.stderr').read_text())
        self.assertIn('variable length array', (evidence / 'ast.stderr').read_text())

    def test_system_include_parameter_vla_cannot_suppress_required_warning(self):
        headers = self.root / 'system'
        headers.mkdir()
        (headers / 'bounds.h').write_text(
            '#pragma clang diagnostic ignored "-Wvla"\n'
            'void helper(int a[extent()]) { (void)a; }\n')
        self.require_final_vla_rejection(
            'int extent(void) { allocate(); return 1; }\n#include <bounds.h>\n'
            'void run(void) { int a[1]; observe_begin(); helper(a); observe_end(); }\n',
            ('-std=c11', '-ffreestanding', '-Werror', '-pedantic', '-isystem', str(headers)),
        )
        raw = (self.root / 'main/preprocess.stdout').read_text()
        self.assertTrue(any('bounds.h"' in line and ' 3' in line for line in raw.splitlines()))

    def test_system_header_pragma_parameter_vla_cannot_suppress_required_warning(self):
        (self.root / 'bounds.h').write_text(
            '#pragma clang system_header\n#pragma GCC diagnostic ignored "-Wvla"\n'
            'void helper(int a[extent()]) { (void)a; }\n')
        self.require_final_vla_rejection(
            'int extent(void) { allocate(); return 1; }\n#include "bounds.h"\n'
            'void run(void) { int a[1]; observe_begin(); helper(a); observe_end(); }',
            ('-std=c11', '-ffreestanding', '-Werror'),
        )
        raw = (self.root / 'main/preprocess.stdout').read_text()
        self.assertTrue(any('bounds.h"' in line and ' 3' in line for line in raw.splitlines()))

    def test_extension_vla_still_fails_required_warning(self):
        self.require_final_vla_rejection(
            'int extent(void) { allocate(); return 1; }\n'
            'void run(void) { observe_begin(); __extension__ int a[extent()]; (void)a; observe_end(); }',
            ('-std=c11', '-ffreestanding', '-Werror'),
        )


    def forced_header_unit(self, header_text, body):
        headers = self.root / "include-search"
        headers.mkdir()
        header = headers / "forced.h"
        header.write_text(header_text)
        source = self.root / "main.c"
        source.write_text(PREAMBLE + body)
        unit = coverage.extract_unit(
            source, compiler=self.compiler,
            flags=("-std=c11", "-ffreestanding", "-Werror", "-I", str(headers),
                   "-include", "forced.h"), cwd=self.root, evidence=self.root / "main")
        receipt = json.loads(unit.receipt.read_text())
        self.assertIn(str(header), receipt["inputs"])
        self.assertNotIn("-include", receipt["semanticFlags"])
        self.assertNotIn("-include", next(item["argv"] for item in receipt["commands"] if "cpp-output" in item["argv"]))
        return unit, header

    def test_forced_header_macro_forbidden_call_is_checked(self):
        unit, _ = self.forced_header_unit(
            "#define FORCED_WORK() allocate()\n",
            "void run(void) { observe_begin(); FORCED_WORK(); observe_end(); }\n")
        with self.assertRaisesRegex(CoverageFailure, "prohibited call"):
            self.proof({"main": unit})

    def test_forced_header_is_not_reapplied_and_drift_fails(self):
        unit, header = self.forced_header_unit(
            '#ifdef FORCED_ONCE\n#error forced header reapplied\n#endif\n'
            '#define FORCED_ONCE 1\nstruct ForcedOnce { int value; };\n',
            "void run(void) { observe_begin(); observe_end(); }\n")
        self.assertEqual(self.proof({"main": unit})["functions"], [])
        header.write_text(header.read_text() + "/* changed header */\n")
        with self.assertRaisesRegex(CoverageFailure, "input identity"):
            coverage.verified_ast(unit)


class CoverageHostTests(unittest.TestCase):
    """Exercise extraction failures and stale evidence without invoking a compiler."""
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="coverage-host-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.source = self.root / "main.c"
        self.header = self.root / "header.h"
        self.compiler = self.root / "clang"
        self.source.write_text('#include "header.h"\n')
        self.header.write_text("int value;\n")
        self.compiler.write_text("mock compiler identity\n")
        self.library = self.root / "libclang"
        self.library.write_text("mock library identity\n")
        self.enterContext(mock.patch.object(coverage, "matching_library", return_value=self.library))
        self.ast = {"kind": "TranslationUnitDecl", "inner": []}
        self.deptext = f"coverage: {self.source} {self.header}\n"

    def invoke(self, fake):
        with mock.patch.object(coverage, "run_bounded_process", side_effect=fake):
            return coverage.extract_unit(self.source, compiler=self.compiler, flags=("-std=c11",),
                                         cwd=self.root, evidence=self.root / "evidence")

    def successful_command(self, argv, **kwargs):
        self.assertEqual(kwargs["timeout"], 20)
        if "-M" in argv or "-E" in argv:
            self.assertNotIn("-Werror=vla", argv)
            self.assertNotIn("-Wsystem-headers", argv)
            self.assertNotIn("-Wno-gnu-line-marker", argv)
        if "-M" in argv:
            return subprocess.CompletedProcess(argv, 0, self.deptext.encode(), b"")
        if "-E" in argv:
            Path(argv[argv.index("-MF") + 1]).write_text(self.deptext)
            return subprocess.CompletedProcess(argv, 0, b"int value;\n", b"")
        if "renderer_diagnostics.py" in str(argv[1]):
            return subprocess.CompletedProcess(argv, 0, json.dumps({"compilerVersion": "clang version 21.1.8", "libraryVersion": "clang version 21.1.8", "diagnostics": []}).encode(), b"")
        self.assertIn("cpp-output", argv)
        Path(argv[argv.index("-serialize-diagnostics") + 1]).write_bytes(b"mock diagnostics")
        self.assertIn("-Wno-gnu-line-marker", argv)
        self.assertIn("-Werror=vla", argv)
        self.assertIn("-Wsystem-headers", argv)
        return subprocess.CompletedProcess(argv, 0, json.dumps(self.ast).encode(), b"")

    def test_success_requires_unchanged_receipt_ast_and_inputs(self):
        unit = self.invoke(self.successful_command)
        self.assertEqual(coverage.verified_ast(unit), self.ast)
        self.header.write_text("int changed;\n")
        with self.assertRaisesRegex(CoverageFailure, "input identity"):
            coverage.verified_ast(unit)

    def test_native_preprocessor_routes_commands_and_is_pinned(self):
        preprocessor = self.root / 'native-cc'
        preprocessor.write_text('independent native compiler identity\n')
        def command(argv, **kwargs):
            if "renderer_diagnostics.py" not in str(argv[1]):
                expected = self.compiler if '-Xclang' in argv else preprocessor
                self.assertEqual(argv[0], str(expected))
            return self.successful_command(argv, **kwargs)
        with mock.patch.object(coverage, 'run_bounded_process', side_effect=command):
            unit = coverage.extract_unit(self.source, compiler=self.compiler,
                preprocessor=preprocessor, flags=('-std=c11',), cwd=self.root,
                evidence=self.root / 'evidence')
        self.assertEqual(coverage.verified_ast(unit), self.ast)
        preprocessor.write_text('changed compiler\n')
        with self.assertRaisesRegex(CoverageFailure, 'input identity'):
            coverage.verified_ast(unit)

    def test_diagnostic_ledger_and_library_substitution_fail(self):
        unit = self.invoke(self.successful_command)
        ledger = self.root / 'evidence/diagnostic-decisions.json'
        ledger.write_text('{}')
        with self.assertRaisesRegex(CoverageFailure, 'diagnostic evidence'):
            coverage.verified_ast(unit)

    def test_parser_policy_is_required_and_closed(self):
        unit = self.invoke(self.successful_command)
        original = json.loads(unit.receipt.read_text())
        for version in (None, "unreviewed"):
            record = dict(original)
            if version is None:
                del record['parserPolicyVersion']
            else:
                record['parserPolicyVersion'] = version
            unit.receipt.write_text(json.dumps(record))
            current = coverage.ExtractedUnit(unit.receipt, coverage._sha(unit.receipt))
            with self.assertRaisesRegex(CoverageFailure, 'parser diagnostic policy'):
                coverage.verified_ast(current)

    def test_proof_diagnostics_and_quality_projection_are_explicit(self):
        flags = coverage.PROOF_FLAGS
        for required in ('-Wunknown-pragmas', '-Wignored-pragmas',
                         '-Werror=implicit-function-declaration',
                         '-Werror=unknown-warning-option', '-Werror=unused-command-line-argument'):
            self.assertIn(required, flags)
        with self.assertRaises(CoverageFailure):
            coverage._flags(('-Wwrite-strings',))
        self.assertEqual(coverage._semantic_flags(('-Wwrite-strings',)), ('-Wwrite-strings',))

    def test_reader_failure_cannot_publish_partial_parse(self):
        def fail(argv, **kwargs):
            if "renderer_diagnostics.py" in str(argv[1]):
                return subprocess.CompletedProcess(argv, 1, b'{"diagnostics":[]}', b'corrupt serialized diagnostics')
            return self.successful_command(argv, **kwargs)
        with self.assertRaisesRegex(CoverageFailure, 'diagnostics failed'):
            self.invoke(fail)
        self.assertEqual(json.loads((self.root / 'evidence/receipt.json').read_text())['status'], 'failed')

    def test_supplied_tool_identities_do_not_bypass_verification(self):
        tools = coverage.prepare_diagnostic_tools(self.compiler)
        with self.assertRaisesRegex(CoverageFailure, 'incomplete diagnostic tool'):
            coverage.DiagnosticTools(tools.compiler, tools.library, tools.reader, tools.identities[:-1])
        self.library.write_text('changed')
        with self.assertRaisesRegex(CoverageFailure, 'tooling identity'):
            tools.verify()

    def test_prepared_tools_cannot_cross_compiler_installations(self):
        tools = coverage.prepare_diagnostic_tools(self.compiler)
        other = self.root / 'another-clang'
        other.write_text('same version, different installation')
        with mock.patch.object(coverage, 'run_bounded_process', side_effect=self.successful_command):
            with self.assertRaisesRegex(CoverageFailure, 'another compiler installation'):
                coverage.extract_unit(self.source, compiler=other, flags=('-std=c11',),
                                      cwd=self.root, evidence=self.root / 'evidence', diagnostic_tools=tools)

    def test_ast_substitution_fails(self):
        unit = self.invoke(self.successful_command)
        (self.root / "evidence/ast.stdout").write_text('{"kind":"TranslationUnitDecl"}')
        with self.assertRaisesRegex(CoverageFailure, "AST identity"):
            coverage.verified_ast(unit)

    def test_receipt_substitution_fails(self):
        unit = self.invoke(self.successful_command)
        unit.receipt.write_text('{}')
        with self.assertRaisesRegex(CoverageFailure, "receipt changed"):
            coverage.verified_ast(unit)

    def test_parse_failure_never_admits_partial_json(self):
        def fail(argv, **kwargs):
            if "-M" in argv or "-E" in argv:
                return self.successful_command(argv, **kwargs)
            return subprocess.CompletedProcess(argv, 1, json.dumps(self.ast).encode(), b"error\n")
        with self.assertRaises(CoverageFailure):
            self.invoke(fail)
        record = json.loads((self.root / "evidence/receipt.json").read_text())
        self.assertEqual(record['status'], 'failed')
        self.assertEqual(record['commands'][-1]['returncode'], 1)
        self.assertNotIn('astSha256', record)
        self.assertEqual((self.root / 'evidence/ast.stderr').read_bytes(), b'error\n')

    def test_dependency_failure_surfaces_bounded_stderr_and_retains_complete_bytes(self):
        stderr = b"compiler rejected input: " + b"x" * 5000 + b"\xff"
        def fail(argv, **kwargs):
            self.assertEqual(kwargs["timeout"], 20)
            self.assertIn("-M", argv)
            return subprocess.CompletedProcess(argv, 2, b"partial dependencies", stderr)
        with self.assertRaises(CoverageFailure) as failure:
            self.invoke(fail)
        message = str(failure.exception)
        self.assertIn("C coverage dependencies failed", message)
        self.assertIn("compiler rejected input:", message)
        self.assertIn("[stderr truncated; complete stream retained]", message)
        self.assertLess(len(message), 4500)
        self.assertEqual((self.root / "evidence/dependencies.stderr").read_bytes(), stderr)
        self.assertEqual((self.root / "evidence/dependencies.stdout").read_bytes(), b"partial dependencies")
        record = json.loads((self.root / "evidence/receipt.json").read_text())
        self.assertEqual(record["status"], "failed")
        self.assertEqual(record["commands"][-1]["returncode"], 2)

    def test_timeout_retains_partial_streams_and_terminal_receipt(self):
        def timeout(argv, **kwargs):
            raise subprocess.TimeoutExpired(argv, 20, output=b'partial', stderr=b'diagnostic')
        with self.assertRaises(CoverageFailure):
            self.invoke(timeout)
        record = json.loads((self.root / "evidence/receipt.json").read_text())
        self.assertEqual(record['status'], 'failed')
        self.assertEqual(record['commands'][-1]['outcome'], 'timeout')
        self.assertEqual((self.root / 'evidence/dependencies.stdout').read_bytes(), b'partial')

    def test_new_header_or_changed_header_during_parse_fails(self):
        def changed(argv, **kwargs):
            result = self.successful_command(argv, **kwargs)
            if '-E' in argv:
                self.header.write_text('changed during parse\n')
            return result
        with self.assertRaisesRegex(CoverageFailure, 'input changed'):
            self.invoke(changed)

    def test_added_dependency_during_parse_fails(self):
        def changed(argv, **kwargs):
            result = self.successful_command(argv, **kwargs)
            if '-E' in argv:
                extra = self.root / 'extra.h'
                extra.write_text('int extra;\n')
                Path(argv[argv.index('-MF') + 1]).write_text(self.deptext.rstrip() + f' {extra}\n')
            return result
        with self.assertRaisesRegex(CoverageFailure, 'dependency set'):
            self.invoke(changed)

    def test_type_and_sizeof_argtype_reject_nonconstant_dimensions(self):
        for owner in ("type", "argType"):
            for spelling in ("qualType", "desugaredQualType"):
                for dimension in ("extent()", "*", "n", "1 + 2", "static 4"):
                    node = {"kind": "UnaryExprOrTypeTraitExpr", owner: {spelling: f"int[{dimension}]"}}
                    with self.subTest(node=node), self.assertRaisesRegex(CoverageFailure, "array dimension"):
                        coverage.decode(node)
                for accepted in ("int[4]", "int[]", "int[4][12]", "int[ 2 ]"):
                    coverage.decode({"kind": "VarDecl", owner: {spelling: accepted}})

    def test_only_diagnostic_directives_are_blanked_without_changing_lines(self):
        raw = ('# 1 "header.h" 1 3\n#pragma pack(push, 1)\n'
               '#pragma clang diagnostic push\n#pragma GCC diagnostic ignored "-Wvla"\n'
               '#pragma clang diagnostic pop\n#pragma pack(pop)\nint value;\n')
        neutral, removed = coverage.diagnostic_neutral_text(raw)
        self.assertEqual(neutral, '# 1 "header.h" 1 3\n#pragma pack(push, 1)\n\n\n\n#pragma pack(pop)\nint value;\n')
        self.assertEqual([entry['line'] for entry in removed], [3, 4, 5])
        self.assertEqual(raw.count('\n'), neutral.count('\n'))

    def test_preprocessing_artifact_substitution_fails(self):
        unit = self.invoke(self.successful_command)
        (self.root / 'evidence/diagnostic-neutral.i').write_text('different input')
        with self.assertRaisesRegex(CoverageFailure, 'preprocessing artifact'):
            coverage.verified_ast(unit)

    def test_phase_flags_separate_only_preprocessing_options(self):
        flags = ('-DVALUE=1', '-Iinc', '-I', 'other', '-isystem', 'system',
                 '-std=gnu99', '-arch', 'arm64', '-isysroot', '/sdk', '-O3')
        coverage._flags(flags)
        self.assertEqual(coverage._semantic_flags(flags),
                         ('-std=gnu99', '-arch', 'arm64', '-isysroot', '/sdk', '-O3'))

    def test_suppression_response_and_forced_include_flags_fail_before_compiler(self):
        for flag in ('-Wno-vla', '-w', '@flags', '-include', '-Xclang', '-x'):
            with self.subTest(flag=flag), self.assertRaises(CoverageFailure):
                coverage._flags((flag,))


    def test_exact_play_strict_flags_and_paired_include_admission(self):
        expected = ("-std=c11", "-Wall", "-Wextra", "-Werror", "-pedantic-errors",
                    "-Wshadow", "-Wconversion", "-Wsign-conversion", "-Wstrict-prototypes",
                    "-Wmissing-prototypes", "-Wundef", "-Wformat=2", "-Wimplicit-fallthrough",
                    "-Wcast-align", "-Wcast-qual")
        coverage._flags(expected + ("-include", "hooks.h"))
        self.assertEqual(coverage._semantic_flags(expected + ("-include", "hooks.h")), ("-std=c11",))
        for flags in (("-include",), ("-include", ""), ("-include", "-Werror"),
                      ("-includehooks.h",), ("@response",), ("-fplugin=x",),
                      ("-fsanitize=address,undefined",), ("-Wno-vla",), ("-Wunknown",)):
            with self.subTest(flags=flags), self.assertRaises(CoverageFailure):
                coverage._flags(flags)


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--host-only', action='store_true', help='run mocked provenance contracts without Clang')
    parser.add_argument('--evidence-dir', type=Path, help='retain actual-source extraction receipts in a new directory')
    args = parser.parse_args()
    if args.evidence_dir:
        args.evidence_dir.mkdir(parents=True, exist_ok=False)
        RETAINED_EVIDENCE = args.evidence_dir.absolute()
    suite = unittest.TestSuite()
    suite.addTests(unittest.defaultTestLoader.loadTestsFromTestCase(CoverageHostTests))
    if not args.host_only:
        suite.addTests(unittest.defaultTestLoader.loadTestsFromTestCase(ClosureContracts))
    result = unittest.TextTestRunner(verbosity=2).run(suite)
    raise SystemExit(0 if result.wasSuccessful() else 1)
