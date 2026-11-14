"""Keep unsupported GNU forwarding declarations outside renderer coverage.

These native controls exercise the ordinary extraction and graph owners. A
successful parser compatibility step must never authorize the affected wrapper
as a measured call, marker owner, or reviewed foreign leaf.
"""
from pathlib import Path
import copy
import json
import shutil
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
sys.path[:0] = [str(ROOT), str(ROOT / "examples/caxecraft")]
import renderer_call_coverage as coverage
import renderer_compatibility as compatibility

FORWARD = """
int sink(const char *, ...) __attribute__((format(printf, 1, 2)));
extern __inline __attribute__((__always_inline__, __gnu_inline__))
int forward(const char *format, ...) {
  return sink(format, __builtin_va_arg_pack());
}
"""
RETAINED_EVIDENCE: Path | None = None
MARKERS = "void begin(void); void end(void);\n"

# Authored reduction: no SDK source, path or wrapper-name recognition.
CONDITIONAL = b"""extern int channel(int, ...);
extern int empty(int);
extern void deny(void) __attribute__((error("rejected")));
extern __inline __attribute__((__always_inline__, __gnu_inline__))
int relay(int mode, ...) {
  if (__builtin_va_arg_pack_len() > 2) deny();
  if (__builtin_constant_p(mode)) {
    return channel(mode, __builtin_va_arg_pack());
  }
  if (__builtin_va_arg_pack_len() == 0) return empty(mode);
  return channel(mode, __builtin_va_arg_pack());
}
"""
CONDITIONAL_NATIVE = CONDITIONAL.replace(
    b"__builtin_va_arg_pack_len() == 0",
    b"((mode & 0100) != 0 || (mode & (0200 | 0400)) == (0200 | 0400)) "
    b"&& __builtin_va_arg_pack_len() < 1")


class ConditionalPreparationContracts(unittest.TestCase):
    def test_nested_pack_and_length_have_exact_offset_preserving_edits(self):
        derived, edits = compatibility.prepare_parser_input(CONDITIONAL)
        self.assertEqual(len(derived), 455)
        self.assertEqual(len(edits), 2)
        self.assertEqual([(edit.body_start, edit.body_end) for edit in edits], [(204, 454)] * 2)
        self.assertEqual([edit.call_start for edit in edits], [300, 413])
        for first, last in ((314, 337), (427, 450)):
            self.assertEqual(CONDITIONAL[first:last], b"__builtin_va_arg_pack()")
            self.assertEqual(derived[first:last], b" " * (last - first))
        for first, last in ((212, 239), (350, 377)):
            self.assertEqual(CONDITIONAL[first:last], b"__builtin_va_arg_pack_len()")
            self.assertEqual(derived[first:last], b"0" + b" " * (last - first - 1))
        self.assertEqual(derived.count(b"deny();"), 1)
        self.assertEqual(derived.count(b"if ("), 3)
        self.assertEqual(derived.count(b"return "), 3)

    def test_conditional_extra_effects_and_unclosed_integers_reject(self):
        for statement in (b"int local;", b"mode = 1;", b"while (mode) {}", b"return 1;"):
            with self.subTest(statement=statement), self.assertRaises(compatibility.CompatibilityFailure):
                compatibility.prepare_parser_input(CONDITIONAL.replace(b"  if (", b"  " + statement + b"\n  if (", 1))
        for literal in (b"08", b"0x10", b"2U", b"2147483648"):
            with self.subTest(literal=literal), self.assertRaises(compatibility.CompatibilityFailure):
                compatibility.prepare_parser_input(CONDITIONAL.replace(b"> 2", b"> " + literal))


def conditional_ast():
    """Hand-authored AST double for the independent 455-byte reduction.

    The statement inventory is explicit here, without the compatibility parser.
    A native extraction control remains necessary to prove Clang's real shape.
    """
    integer = {"qualType": "int"}

    def node(kind, first, last, children=(), **fields):
        return dict(kind=kind, range={"begin": {"offset": first},
                                     "end": {"offset": last - 1, "tokLen": 1}},
                    inner=list(children), **fields)

    def literal(first, value):
        return node("IntegerLiteral", first, first + 1, value=str(value), type=integer)

    def parameter(first):
        return node("DeclRefExpr", first, first + 4, type=integer,
                    referencedDecl={"kind": "ParmVarDecl", "id": "mode", "name": "mode"})

    def call(name, first, last, args=()):
        return node("CallExpr", first, last,
                    [node("DeclRefExpr", first, first + len(name),
                          referencedDecl={"kind": "FunctionDecl", "id": name, "name": name}), *args],
                    type=integer if name != "deny" else {"qualType": "void"})

    def returned(first, name, call_first, last, param_first):
        return node("ReturnStmt", first, last, [call(name, call_first, last, [parameter(param_first)])])

    # Three if statements and three returns are required even when length is zero.
    first_if = node("IfStmt", 208, 251, [
        node("BinaryOperator", 212, 243, [literal(212, 0), literal(242, 2)], opcode=">", type=integer),
        call("deny", 245, 251)])
    constant = call("__builtin_constant_p", 259, 285, [parameter(280)])
    nested = node("CompoundStmt", 287, 343, [returned(293, "channel", 300, 338, 308)])
    second_if = node("IfStmt", 255, 343, [constant, nested])
    third_if = node("IfStmt", 346, 402, [
        node("BinaryOperator", 350, 382, [literal(350, 0), literal(381, 0)], opcode="==", type=integer),
        returned(384, "empty", 391, 402, 397)])
    body = node("CompoundStmt", 204, 454, [first_if, second_if, third_if,
                                         returned(406, "channel", 413, 451, 421)])
    functions = []
    for name, first, last, signature, variadic in (
            ("channel", 0, 28, "int (int, ...)", True),
            ("empty", 30, 51, "int (int)", False),
            ("deny", 53, 110, "void (void)", False)):
        children = [] if name == "deny" else [{"kind": "ParmVarDecl", "id": name + ".arg", "type": integer}]
        if name == "deny":
            children.append(node("ErrorAttr", 91, 108))
        functions.append(node("FunctionDecl", first, last, children, id=name, name=name,
                              storageClass="extern", variadic=variadic, type={"qualType": signature}))
    functions.append({"kind": "FunctionDecl", "id": "__builtin_constant_p", "name": "__builtin_constant_p",
                      "isImplicit": True, "inner": [{"kind": "BuiltinAttr"}]})
    functions.append(node("FunctionDecl", 112, 454, [
        {"kind": "ParmVarDecl", "id": "mode", "name": "mode", "type": integer}, body,
        {"kind": "GNUInlineAttr"}, {"kind": "AlwaysInlineAttr"}],
        id="relay", name="relay", storageClass="extern", variadic=True,
        type={"qualType": "int (int, ...)"}))
    return {"kind": "TranslationUnitDecl", "inner": functions}


class ConditionalCertificateContracts(unittest.TestCase):
    def setUp(self):
        self.derived, self.edits = compatibility.prepare_parser_input(CONDITIONAL)
        self.ast = conditional_ast()

    def certify(self):
        return compatibility._certify_forwarding(
            CONDITIONAL, self.derived, self.edits, self.ast, parse_returncode=0, marker_names=set())

    def test_one_owner_requires_all_original_branches_calls_and_pack_occurrences(self):
        excluded = self.certify()
        self.assertEqual(excluded, [{"name": "relay", "storage": "extern", "declarationId": "relay",
                                     "bodyRange": [204, 454]}])

    def test_altered_ast_cannot_hide_a_branch_or_call(self):
        original = copy.deepcopy(self.ast)
        for mutate in (
                lambda body: body["inner"].pop(0),
                lambda body: body["inner"][0]["inner"].pop(),
                lambda body: body["inner"][1]["inner"][1]["inner"].clear(),
                lambda body: body["inner"][0]["inner"][0]["inner"][0].update(value="1")):
            self.ast = copy.deepcopy(original)
            mutate(self.ast["inner"][-1]["inner"][1])
            with self.assertRaises(compatibility.CompatibilityFailure):
                self.certify()

    def test_helper_builtin_parameter_and_declaration_id_are_independent_checks(self):
        original = copy.deepcopy(self.ast)
        for mutate in (
                lambda ast: ast["inner"][2]["inner"].clear(),
                lambda ast: ast["inner"][3].update(isImplicit=False),
                lambda ast: ast["inner"][-1]["inner"][0]["type"].update(qualType="volatile int"),
                lambda ast: ast["inner"].append(copy.deepcopy(ast["inner"][0]))):
            self.ast = copy.deepcopy(original)
            mutate(self.ast)
            with self.assertRaises(compatibility.CompatibilityFailure):
                self.certify()

    def test_octal_value_is_bound_to_exact_original_token(self):
        source = b"0100"
        literal = {"kind": "IntegerLiteral", "type": {"qualType": "int"}, "value": "64",
                   "range": {"begin": {"offset": 0}, "end": {"offset": 0, "tokLen": 4}}}
        compatibility._certify_condition(literal, tuple(compatibility._tokens(source)), {}, {}, source, source, ())
        literal["value"] = "100"
        with self.assertRaisesRegex(compatibility.CompatibilityFailure, "value mismatch"):
            compatibility._certify_condition(literal, tuple(compatibility._tokens(source)), {}, {}, source, source, ())

    def test_gnu_inline_linkage_rejects_cross_unit_leaf_escape_in_either_order(self):
        self.certify()
        def call(name):
            return {"kind": "CallExpr", "inner": [{"kind": "DeclRefExpr", "referencedDecl": {
                "kind": "FunctionDecl", "id": "caller." + name, "name": name}}]}
        caller = {"kind": "TranslationUnitDecl", "inner": [
            *[{"kind": "FunctionDecl", "id": "caller." + name, "name": name}
              for name in ("begin", "end", "relay")],
            {"kind": "FunctionDecl", "id": "measured", "name": "measured", "inner": [
                {"kind": "CompoundStmt", "inner": [call("begin"), call("relay"), call("end")]}]}]}
        for units in ((('caller', caller), ('wrapper', self.ast)),
                      (('wrapper', self.ast), ('caller', caller))):
            with self.subTest(order=[name for name, _ in units]):
                program = coverage._Program()
                for name, ast in units:
                    program.add_unit(name, ast)
                with self.assertRaisesRegex(coverage.CoverageFailure, "unproved declaration"):
                    program.prove("begin", "end", {"allocate"}, {"relay", "channel", "empty", "deny"})

    def test_retained_marker_owner_and_modified_parse_are_not_certificates(self):
        with self.assertRaisesRegex(compatibility.CompatibilityFailure, "modified marker"):
            compatibility._certify_forwarding(CONDITIONAL, self.derived, self.edits, self.ast,
                                               parse_returncode=0, marker_names={"relay"})
        for marker in ("channel", "empty", "deny", "__builtin_constant_p"):
            with self.subTest(marker=marker), self.assertRaisesRegex(
                    compatibility.CompatibilityFailure, "marker conditional call"):
                compatibility._certify_forwarding(CONDITIONAL, self.derived, self.edits, self.ast,
                                                   parse_returncode=0, marker_names={marker})
        with self.assertRaisesRegex(compatibility.CompatibilityFailure, "successful parse"):
            compatibility._certify_forwarding(CONDITIONAL, self.derived, self.edits, self.ast,
                                               parse_returncode=1, marker_names=set())
        self.edits = self.edits[:-1]
        with self.assertRaisesRegex(compatibility.CompatibilityFailure, "transformation mismatch"):
            self.certify()


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

    def test_unused_conditional_requires_real_parser_and_complete_ast(self):
        header = self.root / "conditional.h"
        header.write_bytes(b"#pragma GCC system_header\n" + CONDITIONAL_NATIVE)
        unit = self.extract("conditional", '#include "conditional.h"\n' + MARKERS +
                            "void measured(void) { begin(); end(); }\n", system_forwarder=False)
        self.assertEqual(self.prove({"conditional": unit})["functions"], [])
        ast = coverage.verified_ast(unit)
        relay = next(node for node in ast["inner"] if node.get("name") == "relay")
        body = next(node for node in relay["inner"] if node["kind"] == "CompoundStmt")
        nodes = list(compatibility._walk(body))
        self.assertEqual(sum(node["kind"] == "IfStmt" for node in nodes), 3)
        self.assertEqual(sum(node["kind"] == "ReturnStmt" for node in nodes), 3)
        self.assertEqual(sum(node["kind"] == "CallExpr" for node in nodes), 5)


if __name__ == "__main__":
    unittest.main()
