"""Pure contracts for the parser-only Float128 type and diagnostic origins.

These controls never start a compiler or load libclang. The retained native
failure and separately reviewed parser experiment own the real-tool evidence.
"""
from copy import deepcopy
from pathlib import Path
import json
import subprocess
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path[:0] = [str(ROOT), str(ROOT / "examples/caxecraft")]
import renderer_compatibility as compatibility
import renderer_diagnostics as diagnostics
import renderer_call_coverage as coverage
from dev_native_cache import NativeBuildPins

HEADER = ROOT / "examples/caxecraft/renderer_float128.h"
MAIN = Path("/proof/main.i")
VERSION = "clang version 21.1.8"


class Float128Origins(unittest.TestCase):
    def source(self, name="authored.h"):
        return ('# 1 "' + name + '"\nextern void f(char *p) __attribute__((access(read_only, 1)));\n').encode()

    def diagnostic(self, source, name="authored.h"):
        start = source.index(b"access(")
        last = source.index(b")", start)
        point = lambda offset: {"file": name, "offset": offset, "line": 1, "column": 1}
        return {"compilerVersion": VERSION, "libraryVersion": VERSION, "diagnostics": [
            {"severity": 2, "option": "-Wunknown-attributes", "category": 0, "fixits": 0,
             "offset": point(start), "ranges": [[point(start), point(last)]], "children": []}]}

    def test_main_origin_converts_without_changing_occurrence_admission(self):
        source = self.source()
        origins = diagnostics.main_buffer_origins(source, MAIN, HEADER)
        value = self.diagnostic(source)
        converted = diagnostics.main_buffer_diagnostics(value, origins, len(source))
        entry = converted["diagnostics"][0]
        self.assertIsInstance(entry["offset"], int)
        old = {key: item for key, item in entry.items() if key not in ("category", "fixits")}
        decision = diagnostics.admit_diagnostics(dict(converted, diagnostics=[old]), source)
        self.assertEqual(decision[0]["attribute"], "access(read_only,1)")
        self.assertIsInstance(value["diagnostics"][0]["offset"], dict)

    def test_foreign_primary_either_endpoint_and_recursive_note_reject(self):
        source = self.source()
        origins = diagnostics.main_buffer_origins(source, MAIN, HEADER)
        for foreign in (str(HEADER), "<built-in>", "<command-line>", "unknown.h", None):
            for place in ("primary", "first", "last", "note", "nested-note"):
                with self.subTest(foreign=foreign, place=place):
                    value = self.diagnostic(source)
                    entry = value["diagnostics"][0]
                    point = {"file": foreign, "offset": 9, "line": 1, "column": 1}
                    if place == "primary": entry["offset"] = point
                    elif place == "first": entry["ranges"][0][0] = point
                    elif place == "last": entry["ranges"][0][1] = point
                    else:
                        note = {"severity": 1, "option": "", "category": 0, "fixits": 0,
                                "location": point, "ranges": [], "children": []}
                        if place == "nested-note":
                            parent = deepcopy(note)
                            parent["location"]["file"] = "authored.h"
                            parent["children"] = [note]
                            note = parent
                        entry["children"] = [note]
                    with self.assertRaisesRegex(ValueError, "origin"):
                        diagnostics.main_buffer_diagnostics(value, origins, len(source))

    def test_main_context_keeps_all_fields_for_existing_stricter_rules(self):
        source = self.source()
        value = self.diagnostic(source)
        value["diagnostics"][0]["children"] = [{
            "severity": 1, "option": "", "category": 0, "fixits": 1,
            "location": {"file": "authored.h", "offset": 9, "line": 1, "column": 1},
            "ranges": [], "children": []}]
        converted = diagnostics.main_buffer_diagnostics(
            value, diagnostics.main_buffer_origins(source, MAIN, HEADER), len(source))
        self.assertEqual(converted["diagnostics"][0]["children"][0]["fixits"], 1)
        old = {k: v for k, v in converted["diagnostics"][0].items() if k not in ("category", "fixits")}
        with self.assertRaisesRegex(ValueError, "context"):
            diagnostics.admit_diagnostics(dict(converted, diagnostics=[old]), source)

    def test_colliding_escaped_and_unsupported_markers_reject(self):
        for name in (str(HEADER), str(HEADER.parent / "sub/../renderer_float128.h"),
                     "renderer_float128.h", "RENDERER_FLOAT128.H", r"escaped\056h", "é.h"):
            with self.subTest(name=name), self.assertRaises(ValueError):
                diagnostics.main_buffer_origins(self.source(name), MAIN, HEADER)
        for prefix in (b'#include "other.h"\n', b'#pragma clang module import other\n',
                       b'#line 1 "other.h"\n', b'# 1 "other.h" 5\n',
                       b'%:include "other.h"\n', b'_Pragma("once")\n'):
            with self.subTest(prefix=prefix), self.assertRaises(ValueError):
                diagnostics.main_buffer_origins(prefix + self.source(), MAIN, HEADER)

    def test_pseudo_markers_do_not_authorize_pseudo_diagnostics(self):
        source = b'# 0 "<built-in>"\n# 0 "<command-line>"\n' + self.source()
        origins = diagnostics.main_buffer_origins(source, MAIN, HEADER)
        self.assertNotIn("<built-in>", origins)
        self.assertIn("authored.h", origins)


class Float128TypeContract(unittest.TestCase):
    source = b"extern int classify(_Float128 value);\n"

    def tree(self):
        offset = self.source.index(b"_Float128")
        return {"kind": "TranslationUnitDecl", "inner": [
            {"kind": "TypedefDecl", "id": "alias", "name": "_Float128",
             "loc": {"file": str(HEADER), "offset": 19},
             "range": {"begin": {"offset": 0}, "end": {"offset": 19, "tokLen": 9}},
             "type": {"qualType": "__float128"},
             "inner": [{"kind": "BuiltinType", "type": {"qualType": "__float128"}}]},
            {"kind": "FunctionDecl", "id": "function", "name": "classify", "storageClass": "extern",
             "inner": [{"kind": "ParmVarDecl", "id": "parameter", "name": "value",
                        "type": {"qualType": "_Float128", "desugaredQualType": "__float128", "typeAliasDeclId": "alias"},
                        "range": {"begin": {"offset": offset}, "end": {"offset": offset + 10, "tokLen": 5}}}]}]}

    def test_exact_parameter_type_binds_original_offset_and_alias(self):
        result = compatibility.certify_float128(self.source, self.tree(), HEADER)
        self.assertEqual(result["parameterOffsets"], [self.source.index(b"_Float128")])
        self.assertEqual(result["canonicalType"], "__float128")

    def test_conflicting_type_body_wrong_alias_or_missing_occurrence_reject(self):
        for mutation in ("type", "body", "alias", "offset", "duplicate", "header"):
            ast = self.tree()
            alias, function = ast["inner"]
            param = function["inner"][0]
            if mutation == "type": alias["type"]["qualType"] = "double"
            elif mutation == "body": function["inner"].append({"kind": "CompoundStmt"})
            elif mutation == "alias": param["type"]["typeAliasDeclId"] = "other"
            elif mutation == "offset": param["range"]["begin"]["offset"] += 1
            elif mutation == "duplicate": ast["inner"].append(deepcopy(alias))
            elif mutation == "header": alias["loc"]["file"] = str(MAIN)
            with self.subTest(mutation=mutation), self.assertRaises(ValueError):
                compatibility.certify_float128(self.source, ast, HEADER)
        with self.assertRaises(ValueError):
            compatibility.certify_float128(self.source + b"_Float128 global;", self.tree(), HEADER)

    def test_header_is_one_directive_free_typedef(self):
        compatibility.require_float128_header(HEADER.read_bytes())
        for value in (b"typedef double _Float128;\n", b"#pragma once\n" + HEADER.read_bytes(),
                      HEADER.read_bytes() + b"extern int other;", b"/*comment*/" + HEADER.read_bytes()):
            with self.subTest(value=value), self.assertRaises(ValueError):
                compatibility.require_float128_header(value)

    def test_token_inventory_and_target_profile_are_closed(self):
        self.assertEqual(compatibility.float128_offsets(b'/* _Float128 */ "_Float128"'), ())
        self.assertEqual(compatibility.float128_offsets(self.source), (self.source.index(b"_Float128"),))
        self.assertEqual(compatibility.float128_target("x86_64-linux-gnu", "gcc (GCC) 15.2", ("-std=c11", "-O2")), "x86_64-linux-gnu")
        for target, version, flags in (("aarch64-linux-gnu", "gcc (GCC) 15.2", ("-std=c11",)),
                ("x86_64-linux-gnux32", "gcc (GCC) 15.2", ("-std=c11",)),
                ("x86_64-apple-darwin", "gcc (GCC) 15.2", ("-std=c11",)),
                ("x86_64-linux-gnu", VERSION, ("-std=c11",)),
                ("x86_64-linux-gnu", "gcc (GCC) 15.2", ("-std=gnu99",))):
            with self.subTest(target=target, version=version, flags=flags), self.assertRaises(ValueError):
                compatibility.float128_target(target, version, flags)


class Float128Extraction(unittest.TestCase):
    """Mock process bytes, retaining the real preparation/receipt/graph owners."""

    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="float128-pure-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        self.source = self.root / "input.c"
        self.source.write_bytes(Float128TypeContract.source)
        self.compiler = self.root / "clang"
        self.native = self.root / "gcc"
        self.library = self.root / "libclang"
        for path in (self.compiler, self.native, self.library):
            path.write_bytes(b"mock tool bytes")
        self.enterContext(mock.patch.object(coverage, "matching_library", return_value=self.library))
        self.ast = Float128TypeContract().tree()
        self.process_calls = []
        self.native_pins = NativeBuildPins(
            sources=(("input.c", self.source),), flags=("-std=c11",), compiler=self.native,
            objects=(), pins=(), include_roots=(), include_inventory_sha256="mock", environment=(),
            source_dependencies=(), compiler_target="x86_64-linux-gnu", compiler_version="gcc (GCC) 15.2")

    def command(self, argv, **kwargs):
        self.assertEqual(kwargs["timeout"], 20)
        self.process_calls.append(argv)
        if "-M" in argv:
            self.assertEqual(argv[0], str(self.native))
            return subprocess.CompletedProcess(argv, 0, f"coverage: {self.source}\n".encode(), b"")
        if "-E" in argv:
            self.assertEqual(argv[0], str(self.native))
            Path(argv[argv.index("-MF") + 1]).write_text(f"coverage: {self.source}\n")
            return subprocess.CompletedProcess(argv, 0, self.source.read_bytes(), b"")
        if "-ast-dump=json" in argv:
            self.assertEqual(argv[0], str(self.compiler))
            self.assertIn("cpp-output", argv)
            self.assertEqual(argv[argv.index("-target") + 1], "x86_64-linux-gnu")
            self.assertIn(str(HEADER.resolve()), argv)
            self.assertIn("-Werror=vla", argv)
            self.assertEqual(Path(argv[-1]).read_bytes(), self.source.read_bytes())
            Path(argv[argv.index("-serialize-diagnostics") + 1]).write_bytes(b"mock serialized bytes")
            return subprocess.CompletedProcess(argv, 0, json.dumps(self.ast).encode(), b"")
        self.assertIn("--retain-origins", argv)
        return subprocess.CompletedProcess(argv, 0, json.dumps({
            "compilerVersion": VERSION, "libraryVersion": VERSION, "diagnostics": []}).encode(), b"")

    def extract(self):
        with mock.patch.object(coverage, "run_bounded_process", side_effect=self.command):
            return coverage.extract_unit(self.source, compiler=self.compiler, preprocessor=self.native,
                flags=("-std=c11",), cwd=self.root, evidence=self.root / "evidence",
                native_build_pins=self.native_pins)

    def test_receipt_binds_header_target_bytes_and_type_certificate(self):
        unit = self.extract()
        self.assertEqual(len(self.process_calls), 4)
        record = json.loads(unit.receipt.read_text())
        self.assertNotIn(str(HEADER), record["inputs"])
        self.assertIn(str(HEADER.resolve()), record["preprocessingArtifacts"])
        self.assertEqual(record["float128Compatibility"]["certificate"]["parameterOffsets"], [20])
        self.assertEqual(coverage.verified_ast(unit), self.ast)
        for replacement in (None, {}, dict(record["float128Compatibility"], certificate={})):
            changed = dict(record, float128Compatibility=replacement)
            unit.receipt.write_text(json.dumps(changed))
            new = coverage.ExtractedUnit(unit.receipt, coverage._sha(unit.receipt))
            with self.assertRaises(coverage.CoverageFailure):
                coverage.verified_ast(new)

    def test_typedef_adds_no_forbidden_call_or_global_effect_exemption(self):
        def call(name):
            return {"kind": "CallExpr", "inner": [{"kind": "DeclRefExpr",
                    "referencedDecl": {"kind": "FunctionDecl", "id": name}}]}
        for name in ("begin", "allocate", "end"):
            self.ast["inner"].append({"kind": "FunctionDecl", "id": name, "name": name})
        self.ast["inner"].append({"kind": "FunctionDecl", "id": "root", "name": "root", "inner": [
            {"kind": "CompoundStmt", "inner": [call("begin"), call("allocate"), call("end")]}]})
        unit = self.extract()
        with self.assertRaisesRegex(coverage.CoverageFailure, "prohibited call: allocate"):
            coverage.prove_interval({"unit": unit}, begin="begin", end="end",
                                    forbidden={"allocate"}, reviewed_leaves={"allocate"})
        # The type certificate must not filter any other AST declaration.
        for spelling, message in (("int[n]", "array dimension"), ("_Atomic(int)", "atomic")):
            ast = deepcopy(self.ast)
            ast["inner"].append({"kind": "VarDecl", "type": {"qualType": spelling}})
            compatibility.certify_float128(self.source.read_bytes(), ast, HEADER)
            with self.subTest(spelling=spelling), self.assertRaisesRegex(coverage.CoverageFailure, message):
                coverage._Program().add_unit("unit", ast)


if __name__ == "__main__":
    unittest.main()
