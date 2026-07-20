#!/usr/bin/env python3
"""Focused contract tests for the generated-C maintainability analyzer."""

from __future__ import annotations

import copy
import importlib.util
import json
from pathlib import Path
import sys
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
MODULE_PATH = ROOT / "scripts" / "test" / "generated_c_maintainability.py"
POLICY_PATH = ROOT / "docs" / "specs" / "generated-c-maintainability-policy.json"
REPORT_SCHEMA_PATH = ROOT / "docs" / "specs" / "generated-c-maintainability.schema.json"
POLICY_SCHEMA_PATH = ROOT / "docs" / "specs" / "generated-c-maintainability-policy.schema.json"
BASELINE_ROOT = ROOT / "test" / "governance" / "fixtures" / "generated_c_maintainability"


def _load_module():
    spec = importlib.util.spec_from_file_location("generated_c_maintainability", MODULE_PATH)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {MODULE_PATH}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    spec.loader.exec_module(module)
    return module


gcm = _load_module()


def _limits(**overrides):
    values = {
        "max_identifier_bytes": 256,
        "max_file_lines": 1000,
        "max_function_lines": 1000,
        "max_branch_count": 100,
        "max_nesting_depth": 100,
        "max_temporary_declarations": 100,
        "max_temporary_references": 100,
        "max_temporary_declarations_per_k_function_lines": 10000,
        "max_function_temporary_declarations_per_k_lines": 10000,
        "max_unexplained_role_encoded_identifiers": 100,
        "max_unexplained_byte_escaped_identifiers": 100,
        "max_unexplained_digest_identifiers": 100,
        "max_unexplained_hash_suffix_identifiers": 100,
        "min_owner_mapped_artifacts_basis_points": 10000,
        "min_function_identity_mapped_basis_points": 10000,
        "min_source_span_mapped_functions_basis_points": 10000,
        "min_line_directive_count": 1,
    }
    values.update(overrides)
    return gcm.MetricLimits(**values)


def _policy(**limit_overrides):
    return gcm.CorpusPolicy(
        policy_id="focused-test-policy",
        corpus_id="focused",
        layout="canonical",
        limits=_limits(**limit_overrides),
        goto_limits=(
            (gcm.GotoCategory.IRREDUCIBLE_CFG, 10),
            (gcm.GotoCategory.LOOP_BREAK_THROUGH_SWITCH, 10),
        ),
    )


def _owner(source=True):
    span = (
        gcm.SourceSpan("fixtures/Main.hx", 1, 1, 20, 1)
        if source
        else None
    )
    return gcm.ArtifactOwner(gcm.OwnerKind.SOURCE_MODULE, "fixtures/Main", span)


def _mapping(c_name="hxc_method_demo", function_id="fixtures.Main.demo", source=True):
    return gcm.FunctionSourceMapping(
        function_id,
        c_name,
        _owner(source=source),
        gcm.SourceSpan("fixtures/Main.hx", 2, 1, 18, 2) if source else None,
    )


def _artifact(content: str, *, path="src/main.c", mappings=None, owner=None):
    return gcm.GeneratedCArtifact(
        path,
        content.encode("utf-8"),
        _owner() if owner is None else owner,
        (_mapping(),) if mappings is None else tuple(mappings),
    )


def _analyze(content: str, **kwargs):
    return gcm.analyze_generated_c(
        corpus_id="focused",
        layout="canonical",
        artifacts=(_artifact(content),),
        policy=kwargs.pop("policy", _policy()),
        symbols=kwargs.pop("symbols", ()),
        goto_authorities=kwargs.pop("goto_authorities", ()),
        **kwargs,
    )


SAMPLE = """#line 10 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  int32_t hxc_tmp_value_n0 = 1;
  if (hxc_tmp_value_n0) {
    while (hxc_tmp_value_n0) {
      hxc_tmp_value_n0 = 0;
    }
  }
  return hxc_tmp_value_n0;
}
"""


class GeneratedCMaintainabilityTests(unittest.TestCase):
    def test_metrics_and_real_line_directive_mapping_are_structural(self):
        report = _analyze(
            SAMPLE,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        self.assertEqual(report["status"], "within-reviewed-budgets")
        self.assertEqual(report["inputs"][0]["sha256"], report["files"][0]["sha256"])
        self.assertEqual(report["summary"]["lineDirectiveCount"], 1)
        self.assertEqual(report["summary"]["mappedPaths"], ["fixtures/Main.hx"])
        function = report["files"][0]["functions"][0]
        self.assertEqual(function["branchCount"], 2)
        self.assertEqual(function["maxNestingDepth"], 2)
        self.assertEqual(function["temporaryDeclarationCount"], 1)
        self.assertEqual(function["temporaryReferenceCount"], 4)
        self.assertEqual(function["mappedPaths"], ["fixtures/Main.hx"])
        self.assertGreater(function["mappedBodyLineCount"], 0)
        self.assertEqual(report["summary"]["functionIdentityMappedBasisPoints"], 10000)
        self.assertEqual(report["summary"]["sourceSpanMappedFunctionBasisPoints"], 10000)
        self.assertEqual(report["summary"]["lineMappedFunctionBasisPoints"], 10000)

    def test_comments_strings_and_character_literals_are_lexically_ignored(self):
        content = """#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  // goto fake; hxc_tmp_comment_n0 zxFF hxc_type_fake_ hxc_symbol_h123456789abc
  const char *text = \"goto fake; hxc_tmp_string_n0 zxAA hxc_type_nope_\";
  int field_value = 'g'; /* 0123456789abcdef0123456789abcdef and goto x; */
  return field_value + (text != 0);
}
"""
        report = _analyze(
            content,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        self.assertEqual(report["summary"]["gotoCount"], 0)
        self.assertEqual(report["summary"]["temporaryDeclarationCount"], 0)
        findings = {(row["category"], row["identifier"]) for row in report["identifierFindings"]}
        self.assertEqual(findings, {("compiler-role", "hxc_method_demo")})
        self.assertNotIn("field_value", {identifier for _, identifier in findings})

    def test_temporary_references_are_not_miscounted_as_declarations(self):
        content = """#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  int32_t hxc_tmp_value_n0 = 1;
  (void)hxc_tmp_value_n0;
  hxc_consume(hxc_tmp_value_n0);
  return hxc_tmp_value_n0;
}
"""
        report = _analyze(
            content,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        function = report["files"][0]["functions"][0]
        self.assertEqual(function["temporaryDeclarationCount"], 1)
        self.assertEqual(function["temporaryReferenceCount"], 3)

    def test_temporary_reference_budget_is_independent_and_source_positioned(self):
        content = """#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  int32_t hxc_tmp_value_n0 = 1;
  hxc_consume(hxc_tmp_value_n0);
  hxc_consume(hxc_tmp_value_n0);
  return hxc_tmp_value_n0;
}
"""
        report = _analyze(
            content,
            policy=_policy(max_temporary_references=2),
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        violation = next(
            row for row in report["violations"] if row["rule"] == "temporary-references"
        )
        self.assertEqual(violation["actual"], 3)
        self.assertEqual(violation["limit"], 2)
        self.assertEqual(
            violation["coordinate"],
            {
                "path": "src/main.c",
                "line": 4,
                "column": 15,
                "functionId": "fixtures.Main.demo",
            },
        )

    def test_legacy_byte_escape_semantic_digest_and_hash_suffix_are_distinct(self):
        digest = "hxc_value_0123456789abcdef0123456789abcdef"
        compact = "hxc_symbol_h123456789abc"
        expanded = "hxc_symbol_h" + "1" * 20
        byte_name = "hxc_zxC3_name"
        content = f"""#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {{
  int {digest} = 1;
  int {compact} = {digest};
  int {expanded} = {compact};
  int {byte_name} = {expanded};
  return {byte_name};
}}
"""
        symbols = (
            gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),
            gcm.SymbolLedgerEntry(digest, "semantic.digest"),
            gcm.SymbolLedgerEntry(compact, "collision", True),
            gcm.SymbolLedgerEntry(expanded, "expanded.collision", True),
            gcm.SymbolLedgerEntry(byte_name, "ré-name", False, ("non-c-identifier-byte",)),
        )
        report = _analyze(content, symbols=symbols)
        findings = {(row["category"], row["identifier"], row["explained"]) for row in report["identifierFindings"]}
        self.assertIn(("semantic-digest", digest, True), findings)
        self.assertNotIn(("hash-suffix", digest, True), findings)
        self.assertIn(("hash-suffix", compact, True), findings)
        self.assertIn(("hash-suffix", expanded, True), findings)
        self.assertIn(("byte-escape", byte_name, True), findings)
        self.assertEqual(report["summary"]["unexplainedDigestIdentifierCount"], 0)
        self.assertEqual(report["summary"]["unexplainedHashSuffixIdentifierCount"], 0)

    def test_unexplained_spelling_violation_points_to_first_coordinate(self):
        content = """#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  int hxc_symbol_h123456789abc = 0;
  return hxc_symbol_h123456789abc;
}
"""
        report = _analyze(
            content,
            policy=_policy(max_unexplained_hash_suffix_identifiers=0),
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        self.assertEqual(report["status"], "violations-found")
        violation = next(row for row in report["violations"] if row["rule"] == "unexplained-hash-suffix")
        self.assertEqual(violation["coordinate"], {"path": "src/main.c", "line": 3, "column": 7, "functionId": None})
        self.assertEqual(report["firstViolation"]["coordinate"], violation["coordinate"])

    def test_typed_goto_authority_matches_exact_function_target_and_ordinal(self):
        content = """#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  goto hxc_label;
  goto hxc_label;
hxc_label:
  return 0;
}
"""
        authorities = (
            gcm.GotoAuthority("src/main.c", "fixtures.Main.demo", "hxc_label", 1, gcm.GotoCategory.IRREDUCIBLE_CFG),
            gcm.GotoAuthority("src/main.c", "fixtures.Main.demo", "hxc_label", 2, gcm.GotoCategory.IRREDUCIBLE_CFG),
        )
        report = _analyze(
            content,
            goto_authorities=authorities,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        self.assertEqual(report["summary"]["gotoCount"], 2)
        self.assertEqual(report["summary"]["unauthorizedGotoCount"], 0)
        self.assertEqual([row["occurrence"] for row in report["gotoOccurrences"]], [1, 2])
        self.assertEqual({row["category"] for row in report["gotoOccurrences"]}, {"irreducible-cfg"})
        self.assertTrue(all(row["labelLine"] == 5 for row in report["gotoOccurrences"]))
        self.assertEqual(
            gcm.replay_report_from_c(
                report,
                {"src/main.c": content.encode("utf-8")},
                _policy(),
            ),
            report,
        )

    def test_missing_and_unused_goto_authorities_fail_closed(self):
        content = """#line 1 \"fixtures/Main.hx\"
int hxc_method_demo(void) {
  goto hxc_label;
hxc_label:
  return 0;
}
"""
        report = _analyze(
            content,
            goto_authorities=(
                gcm.GotoAuthority("src/main.c", "fixtures.Main.demo", "other_label", 1, gcm.GotoCategory.IRREDUCIBLE_CFG),
            ),
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        rules = {row["rule"] for row in report["violations"]}
        self.assertIn("goto-unauthorized", rules)
        self.assertIn("goto-authority-unused", rules)

    def test_report_is_independent_of_caller_input_order(self):
        content_a = "#line 1 \"fixtures/A.hx\"\nint a(void) { return 1; }\n"
        content_b = "#line 1 \"fixtures/B.hx\"\nint b(void) { return 2; }\n"
        artifact_a = _artifact(
            content_a,
            path="src/a.c",
            owner=gcm.ArtifactOwner(gcm.OwnerKind.SOURCE_MODULE, "fixtures/A"),
            mappings=(gcm.FunctionSourceMapping("fixtures.A.a", "a", _owner(), gcm.SourceSpan("fixtures/A.hx", 1, 1, 1, 20)),),
        )
        artifact_b = _artifact(
            content_b,
            path="src/b.c",
            owner=gcm.ArtifactOwner(gcm.OwnerKind.SOURCE_MODULE, "fixtures/B"),
            mappings=(gcm.FunctionSourceMapping("fixtures.B.b", "b", _owner(), gcm.SourceSpan("fixtures/B.hx", 1, 1, 1, 20)),),
        )
        kwargs = dict(corpus_id="focused", layout="canonical", policy=_policy())
        forward = gcm.analyze_generated_c(artifacts=(artifact_a, artifact_b), **kwargs)
        reverse = gcm.analyze_generated_c(artifacts=(artifact_b, artifact_a), **kwargs)
        self.assertEqual(forward, reverse)
        self.assertEqual([row["path"] for row in forward["inputs"]], ["src/a.c", "src/b.c"])

    def test_report_replay_relexes_exact_c_bytes(self):
        symbols = (gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),)
        report = _analyze(SAMPLE, symbols=symbols)
        replayed = gcm.replay_report_from_c(
            report,
            {"src/main.c": SAMPLE.encode("utf-8")},
            _policy(),
        )
        self.assertEqual(replayed, report)

        changed = SAMPLE.replace("return hxc_tmp_value_n0;", "return 7;")
        changed_report = gcm.replay_report_from_c(
            report,
            {"src/main.c": changed.encode("utf-8")},
            _policy(),
        )
        self.assertNotEqual(changed_report, report)
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.replay_report_from_c(report, {}, _policy())

    def test_package_artifact_owner_round_trips_without_losing_module_functions(self):
        report = gcm.analyze_generated_c(
            corpus_id="focused",
            layout="canonical",
            artifacts=(
                _artifact(
                    SAMPLE,
                    owner=gcm.ArtifactOwner(
                        gcm.OwnerKind.SOURCE_PACKAGE,
                        package_path="fixtures/domain",
                    ),
                ),
            ),
            policy=_policy(),
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        owner = report["files"][0]["owner"]
        self.assertEqual(owner["kind"], "source-package")
        self.assertEqual(owner["packagePath"], "fixtures/domain")
        self.assertIsNone(owner["modulePath"])
        self.assertEqual(report["files"][0]["functions"][0]["owner"]["kind"], "source-module")
        self.assertEqual(
            gcm.replay_report_from_c(
                report,
                {"src/main.c": SAMPLE.encode("utf-8")},
                _policy(),
            ),
            report,
        )

    def test_identity_span_and_line_mapping_coverage_are_not_conflated(self):
        content = "int hxc_method_demo(void) { return 0; }\n"
        artifact = _artifact(content, mappings=(_mapping(source=False),), owner=_owner(source=False))
        policy = _policy(
            min_function_identity_mapped_basis_points=10000,
            min_source_span_mapped_functions_basis_points=0,
            min_line_directive_count=0,
        )
        report = gcm.analyze_generated_c(
            corpus_id="focused", layout="canonical", artifacts=(artifact,), policy=policy,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        self.assertEqual(report["summary"]["functionIdentityMappedBasisPoints"], 10000)
        self.assertEqual(report["summary"]["sourceSpanMappedFunctionBasisPoints"], 0)
        self.assertEqual(report["summary"]["lineMappedFunctionBasisPoints"], 0)

    def test_line_directive_floor_detects_snapshot_mapping_loss(self):
        content = "int hxc_method_demo(void) { return 0; }\n"
        report = _analyze(
            content,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        self.assertIn("line-directive-floor", {row["rule"] for row in report["violations"]})

    def test_invalid_paths_utf8_crlf_and_duplicate_authority_are_rejected(self):
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.analyze_generated_c(
                corpus_id="focused",
                layout="canonical",
                artifacts=(gcm.GeneratedCArtifact("../bad.c", b"int x;\n", _owner()),),
                policy=_policy(),
            )
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.analyze_generated_c(
                corpus_id="focused",
                layout="canonical",
                artifacts=(gcm.GeneratedCArtifact("src/bad.c", b"\xff", _owner()),),
                policy=_policy(),
            )
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.analyze_generated_c(
                corpus_id="focused",
                layout="canonical",
                artifacts=(gcm.GeneratedCArtifact("src/bad.c", b"int x;\r\n", _owner()),),
                policy=_policy(),
            )
        authority = gcm.GotoAuthority(
            "src/main.c", "fixtures.Main.demo", "label", 1, gcm.GotoCategory.IRREDUCIBLE_CFG
        )
        with self.assertRaises(gcm.MaintainabilityError):
            _analyze(SAMPLE, goto_authorities=(authority, authority))

    def test_report_validation_rejects_unknown_fields_order_and_first_violation_drift(self):
        report = _analyze(
            SAMPLE,
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        unknown = copy.deepcopy(report)
        unknown["files"][0]["surprise"] = True
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.validate_report(unknown)

        two_files = copy.deepcopy(report)
        second_input = {"path": "src/z.c", "sha256": "0" * 64}
        second_file = copy.deepcopy(two_files["files"][0])
        second_file["path"] = "src/z.c"
        second_file["sha256"] = "0" * 64
        two_files["inputs"].append(second_input)
        two_files["files"].append(second_file)
        two_files["inputs"].reverse()
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.validate_report(two_files)

        violating = _analyze(
            SAMPLE,
            policy=_policy(max_file_lines=1),
            symbols=(gcm.SymbolLedgerEntry("hxc_method_demo", "fixtures.Main.demo"),),
        )
        drift = copy.deepcopy(violating)
        drift["firstViolation"] = dict(drift["firstViolation"])
        drift["firstViolation"]["rule"] = "different"
        with self.assertRaises(gcm.MaintainabilityError):
            gcm.validate_report(drift)

    def test_reviewed_policy_corpus_set_and_outliers_are_explicit(self):
        document = json.loads(POLICY_PATH.read_text(encoding="utf-8"))
        gcm.validate_policy_document(document)
        pairs = [(row["corpusId"], row["layout"]) for row in document["corpora"]]
        self.assertEqual(
            pairs,
            [
                ("body-lowering", "canonical"),
                ("caxecraft-domain", "package"),
                ("caxecraft-domain", "split"),
                ("caxecraft-domain", "unity"),
                ("evaluation-order-program", "canonical"),
                ("evaluation-order-synthetic", "synthetic"),
                ("handwritten-ordered-calls-baseline", "handwritten"),
                ("handwritten-structured-control-flow-baseline", "handwritten"),
            ],
        )
        body = gcm.load_corpus_policy(POLICY_PATH, corpus_id="body-lowering", layout="canonical")
        package = gcm.load_corpus_policy(POLICY_PATH, corpus_id="caxecraft-domain", layout="package")
        split = gcm.load_corpus_policy(POLICY_PATH, corpus_id="caxecraft-domain", layout="split")
        unity = gcm.load_corpus_policy(POLICY_PATH, corpus_id="caxecraft-domain", layout="unity")
        evaluation = gcm.load_corpus_policy(
            POLICY_PATH,
            corpus_id="evaluation-order-program",
            layout="canonical",
        )
        self.assertGreaterEqual(body.limits.min_line_directive_count, 1)
        self.assertGreaterEqual(
            evaluation.limits.min_source_span_mapped_functions_basis_points,
            9000,
        )
        self.assertEqual(split.file_overrides[0].path, "src/modules/caxecraft/qa/DomainProbe.c")
        self.assertEqual(package.file_overrides[0].path, "src/packages/caxecraft/domain/package.c")
        self.assertEqual(package.file_overrides[1].path, "src/packages/caxecraft/qa/package.c")
        self.assertEqual(unity.file_overrides[0].path, "src/program.c")
        self.assertEqual(split.function_overrides[0].function_id, "caxecraft.qa.DomainProbe.selfCheck")

    def test_handwritten_calibration_kernels_are_real_and_within_policy(self):
        cases = (
            (
                "handwritten-ordered-calls-baseline",
                "ordered_calls.c",
                "handwritten/ordered-calls",
                3,
            ),
            (
                "handwritten-structured-control-flow-baseline",
                "structured_control_flow.c",
                "handwritten/structured-control-flow",
                1,
            ),
        )
        for corpus_id, filename, owner_path, function_count in cases:
            with self.subTest(corpus_id=corpus_id):
                source = (BASELINE_ROOT / filename).read_bytes()
                report = gcm.analyze_generated_c(
                    corpus_id=corpus_id,
                    layout="handwritten",
                    artifacts=(
                        gcm.GeneratedCArtifact(
                            f"handwritten/{filename}",
                            source,
                            gcm.ArtifactOwner(gcm.OwnerKind.HANDWRITTEN_BASELINE, owner_path),
                        ),
                    ),
                    policy=gcm.load_corpus_policy(
                        POLICY_PATH,
                        corpus_id=corpus_id,
                        layout="handwritten",
                    ),
                )
                gcm.validate_report(report)
                self.assertEqual(report["status"], "within-reviewed-budgets")
                self.assertEqual(report["summary"]["functionCount"], function_count)
                self.assertEqual(report["summary"]["gotoCount"], 0)
                self.assertEqual(report["summary"]["temporaryDeclarationCount"], 0)

        ordered_text = (BASELINE_ROOT / "ordered_calls.c").read_text(encoding="utf-8")
        self.assertLess(
            ordered_text.index("take_next(cursor);"),
            ordered_text.index("combine_pair(left, right);"),
        )
        self.assertIn("int32_t left =", ordered_text)
        self.assertIn("int32_t right =", ordered_text)

    def test_schema_documents_close_every_object_shape(self):
        for path in (REPORT_SCHEMA_PATH, POLICY_SCHEMA_PATH):
            schema = json.loads(path.read_text(encoding="utf-8"))
            self.assertEqual(schema["$schema"], "https://json-schema.org/draft/2020-12/schema")
            self.assertEqual(schema["additionalProperties"], False)

            def visit(value):
                if isinstance(value, dict):
                    if value.get("type") == "object":
                        self.assertIs(value.get("additionalProperties"), False, msg=f"open object in {path}: {value}")
                    for child in value.values():
                        visit(child)
                elif isinstance(value, list):
                    for child in value:
                        visit(child)

            visit(schema)

    def test_policy_loader_rejects_unknown_fields(self):
        document = json.loads(POLICY_PATH.read_text(encoding="utf-8"))
        document["corpora"][0]["limits"]["unknown"] = 1
        with tempfile.TemporaryDirectory(prefix="hxc-maintainability-") as temporary:
            path = Path(temporary) / "policy.json"
            path.write_text(json.dumps(document), encoding="utf-8")
            with self.assertRaises(gcm.MaintainabilityError):
                gcm.load_corpus_policy(path, corpus_id="body-lowering", layout="canonical")


if __name__ == "__main__":
    unittest.main()
