"""Host-only contracts for complete build selection and within-run proof reuse."""
from dataclasses import FrozenInstanceError
import hashlib
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path[:0] = [str(ROOT), str(ROOT / "examples/caxecraft")]
import renderer_build_coverage as build
import dev_native_cache as native
from renderer_native_contract import NativeTerrainContract


class TerrainBuildCoverageTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="terrain-build-coverage-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.manifest = self.root / "manifest.json"
        # Authored independently of the supplied native plan.
        self.manifest.write_text('{"build":{"sources":["main.c","helper.c"]}}')
        self.main = self.root / "main.c"
        self.helper = self.root / "helper.c"
        self.include = self.root / "include"
        self.include.mkdir()
        self.header = self.include / "api.h"
        self.executable = self.root / "game"
        self.compiler = self.root / "clang"
        self.preprocessor = self.root / "cc"
        self.observer = self.root / "observer.c"
        for path in (self.main, self.helper, self.header, self.executable,
                     self.compiler, self.preprocessor, self.observer):
            path.write_text(path.name + " original bytes\n")
        self.addCleanup(mock.patch.stopall)
        mock.patch.dict(native.os.environ, {}, clear=True).start()
        mock.patch.object(build, "OBSERVER_SOURCE", self.observer).start()
        self.sources = (("main.c", self.main), ("helper.c", self.helper),
                        (build.OBSERVER_NAME, self.observer))
        self.flags = ("-std=c11", "-DVALUE=1", "-I", str(self.include))
        self.contract = NativeTerrainContract(frozenset({"reviewed_draw"}),
                                             '{"librarySha256":"library","policySha256":"policy"}')
        self.calls = []
        self.native_pins = self.build_pins()

    def build_pins(self):
        """Produce real owner metadata with a mocked compiler identity only."""
        facts = {"path": str(self.preprocessor), "sha256": self.digest(self.preprocessor),
                 "command": str(self.preprocessor), "environment": {}}
        roots = (native.DependencyRoot("project", self.root),)
        includes = (native.IncludeRoot("headers", self.include),)
        objects = []
        for index, (name, source) in enumerate(self.sources):
            directory = self.root / f"object-{index}"
            directory.mkdir()
            obj = directory / "object.o"
            obj.write_text("compiled " + name)
            base = {"schemaVersion": 1, "source": name, "sourceBytes": source.stat().st_size,
                    "sourceSha256": self.digest(source), "compiler": facts,
                    "compileFlags": ["-std=c11", "-DVALUE=1"],
                    "includeInventory": native.include_path_inventory(includes)}
            body = {"schemaVersion": 1, "kind": "caxecraft-native-object", "base": base,
                    "dependencies": native.dependency_snapshot((source, self.header), roots),
                    "objectSha256": self.digest(obj)}
            key = native.canonical_digest(body)
            metadata = {**body, "key": key}
            (directory / "metadata.json").write_text(json.dumps(
                {**metadata, "sha256": native.canonical_digest(metadata)}))
            objects.append(native.NativeObject(name, key, obj, self.digest(obj), False))
        with mock.patch.object(native, "compiler_identity", return_value=facts), \
             mock.patch.dict(native.os.environ, {}, clear=True):
            return native.verify_native_objects(
                objects=objects, sources=self.sources, compiler=str(self.preprocessor),
                compile_flags=("-std=c11", "-DVALUE=1"), include_roots=includes,
                dependency_roots=roots,
            )

    def digest(self, path):
        return hashlib.sha256(path.read_bytes()).hexdigest()

    def extract(self, source, **kwargs):
        self.calls.append((source, kwargs))
        evidence = kwargs["evidence"]
        evidence.mkdir()
        ast = evidence / "ast.json"
        ast.write_text('{"kind":"TranslationUnitDecl"}')
        preprocessed = evidence / "source.i"
        preprocessed.write_text("expanded bytes\n")
        receipt = evidence / "receipt.json"
        receipt.write_text(json.dumps({
            "inputs": {str(source): self.digest(source), str(self.header): self.digest(self.header)},
            "preprocessingArtifacts": {str(preprocessed): self.digest(preprocessed)},
            "ast": str(ast), "astSha256": self.digest(ast), "status": "success",
        }))
        return build.coverage.ExtractedUnit(receipt, self.digest(receipt))

    def invoke(self, **changes):
        args = dict(sources=self.sources, flags=self.flags, compiler=self.compiler,
                    preprocessor=self.preprocessor, cwd=self.root, manifest=self.manifest,
                    executable=self.executable, native_contract=self.contract,
                    native_build_pins=self.native_pins,
                    evidence=self.root / "evidence")
        args.update(changes)
        with mock.patch.object(build.coverage, "extract_unit", side_effect=self.extract) as extract, \
             mock.patch.dict(native.os.environ, {}, clear=True):
            with mock.patch.object(build.coverage, "prove_interval", return_value={
                "functions": [("", "helper"), ("", "reviewed_draw")],
                "foreignLeaves": ["reviewed_draw"],
            }) as prove:
                result = build.build_terrain_coverage(**args)
        return result, extract, prove

    def test_exact_order_once_and_frozen_flags_then_hash_only_reuse(self):
        result, extract, prove = self.invoke()
        self.assertEqual([source for source, _ in self.calls],
                         [self.main, self.helper, self.observer])
        self.assertEqual(extract.call_count, 3)
        for _, kwargs in self.calls:
            self.assertEqual(kwargs["flags"], self.flags)
            self.assertEqual(kwargs["compiler"], self.compiler)
            self.assertEqual(kwargs["preprocessor"], self.preprocessor)
        self.assertEqual(list(prove.call_args.args[0]),
                         ["main.c", "helper.c", build.OBSERVER_NAME])
        self.assertEqual(prove.call_args.kwargs, {
            "begin": "caxecraft_test_interval_begin", "end": "caxecraft_test_interval_end",
            "forbidden": {"hxc_alloc", "hxc_realloc", "hxc_gc_allocate", "hxc_gc_collect",
                          "malloc", "calloc", "realloc", "aligned_alloc", "posix_memalign"},
            "reviewed_leaves": {"reviewed_draw"},
        })
        with mock.patch.object(build.coverage, "extract_unit", side_effect=AssertionError("must not extract")), \
             mock.patch.object(build.coverage, "prove_interval", side_effect=AssertionError("must not rebuild AST")), \
             mock.patch.dict(native.os.environ, {}, clear=True):
            result.verify(native_contract=self.contract)
            result.verify(native_contract=self.contract)
        with self.assertRaises(FrozenInstanceError):
            result.flags = ("-DOTHER",)
        summary = result.summary()
        self.assertEqual(summary["translationUnits"], 3)
        self.assertEqual(summary["reachableFunctions"], 2)
        self.assertNotIn(str(self.root), json.dumps(summary))
        summary["translationUnits"] = 999
        self.assertEqual(result.summary()["translationUnits"], 3)

    def test_missing_extra_reordered_and_repeated_units_fail_before_extraction(self):
        plans = [self.sources[:1] + self.sources[2:], self.sources + (("extra.c", self.main),),
                 (self.sources[1], self.sources[0], self.sources[2]),
                 (self.sources[0], self.sources[0], self.sources[2]), self.sources[:2]]
        for index, plan in enumerate(plans):
            with self.subTest(plan=plan), self.assertRaises(build.coverage.CoverageFailure):
                self.invoke(sources=plan, evidence=self.root / f"bad-{index}")
        self.assertEqual(self.calls, [])

    def test_manifest_duplicate_and_escaping_names_fail_before_extraction(self):
        for index, names in enumerate((["main.c", "main.c"], ["../main.c"], [], ["./main.c"])):
            self.manifest.write_text(json.dumps({"build": {"sources": names}}))
            with self.subTest(names=names), self.assertRaises(build.coverage.CoverageFailure):
                self.invoke(evidence=self.root / f"bad-{index}")
        self.assertEqual(self.calls, [])

    def test_same_names_cannot_select_other_source_or_observer_bytes(self):
        for index, plan in enumerate((
            (("main.c", self.helper), *self.sources[1:]),
            (*self.sources[:2], (build.OBSERVER_NAME, self.main)),
        )):
            with self.subTest(plan=plan), self.assertRaises(build.coverage.CoverageFailure):
                self.invoke(sources=plan, evidence=self.root / f"bad-{index}")
        self.assertEqual(self.calls, [])

    def test_manifest_executable_sources_headers_tools_and_extracted_bytes_are_pinned(self):
        result, _, _ = self.invoke()
        paths = [self.manifest, self.executable, self.main, self.header,
                 self.compiler, self.preprocessor, result.receipt,
                 self.root / "evidence/unit-0000/receipt.json",
                 self.root / "evidence/unit-0000/ast.json",
                 self.root / "evidence/unit-0000/source.i"]
        for path in paths:
            before = path.read_bytes()
            path.write_bytes(before + b"changed")
            try:
                with self.subTest(path=path.name), self.assertRaises(build.coverage.CoverageFailure):
                    result.verify(native_contract=self.contract)
            finally:
                path.write_bytes(before)
        result.verify(native_contract=self.contract)

    def test_fresh_native_identity_or_leaf_policy_drift_fails(self):
        result, _, _ = self.invoke()
        for contract in (
            NativeTerrainContract(self.contract.leaves, '{"librarySha256":"changed","policySha256":"policy"}'),
            NativeTerrainContract(self.contract.leaves, '{"librarySha256":"library","policySha256":"changed"}'),
            NativeTerrainContract(frozenset({"different_leaf"}), self.contract.identity_json),
        ):
            with self.assertRaisesRegex(build.coverage.CoverageFailure, "native terrain contract changed"):
                result.verify(native_contract=contract)

    def test_missing_inputs_and_invalid_manifest_use_the_public_failure_type(self):
        self.manifest.write_text("invalid JSON")
        with self.assertRaises(build.CoverageFailure):
            self.invoke()
        self.manifest.write_text('{"build":{"sources":["main.c","helper.c"]}}')
        result, _, _ = self.invoke(evidence=self.root / "valid")
        self.header.unlink()
        with self.assertRaises(build.CoverageFailure):
            result.verify(native_contract=self.contract)

    def test_extraction_failure_preserves_evidence_and_stops_next_source(self):
        def fail(source, **kwargs):
            kwargs["evidence"].mkdir()
            (kwargs["evidence"] / "stderr").write_text("retained compiler diagnostic")
            raise build.coverage.CoverageFailure("unsupported flag")
        with mock.patch.object(self, "extract", side_effect=fail):
            with self.assertRaisesRegex(build.coverage.CoverageFailure, "unsupported flag"):
                self.invoke()
        receipt = json.loads((self.root / "evidence/receipt.json").read_text())
        self.assertEqual(receipt["status"], "failed")
        self.assertEqual((self.root / "evidence/unit-0000/stderr").read_text(), "retained compiler diagnostic")
        self.assertFalse((self.root / "evidence/unit-0001").exists())

    def test_source_mutation_during_extraction_fails(self):
        original = self.extract
        def changed(source, **kwargs):
            unit = original(source, **kwargs)
            self.executable.write_text("changed during extraction")
            return unit
        with mock.patch.object(self, "extract", side_effect=changed):
            with self.assertRaisesRegex(build.coverage.CoverageFailure, "inputs changed"):
                self.invoke()

    def test_native_source_header_and_object_drift_before_coverage_rejects(self):
        for index, path in enumerate((self.main, self.header, self.native_pins.objects[0].path)):
            original = path.read_bytes()
            path.write_bytes(original + b"changed after link")
            try:
                with self.subTest(path=path.name), self.assertRaises(build.CoverageFailure):
                    self.invoke(evidence=self.root / f"drift-{index}")
            finally:
                path.write_bytes(original)
        self.assertEqual(self.calls, [])

    def test_different_flags_or_native_compiler_cannot_borrow_build_pins(self):
        for index, change in enumerate(({"flags": ("-O3",)}, {"preprocessor": self.compiler})):
            with self.subTest(change=change), self.assertRaisesRegex(build.CoverageFailure, "differs from extraction plan"):
                self.invoke(evidence=self.root / f"plan-{index}", **change)
        self.assertEqual(self.calls, [])

    def test_extraction_cannot_introduce_uncompiled_dependency(self):
        original = self.extract
        added = self.root / "not-a-build-dependency.h"
        added.write_text("new selection")
        def changed(source, **kwargs):
            unit = original(source, **kwargs)
            record = json.loads(unit.receipt.read_text())
            record["inputs"][str(added)] = self.digest(added)
            unit.receipt.write_text(json.dumps(record))
            return build.coverage.ExtractedUnit(unit.receipt, self.digest(unit.receipt))
        with mock.patch.object(self, "extract", side_effect=changed):
            with self.assertRaisesRegex(build.CoverageFailure, "dependencies differ"):
                self.invoke()

    def test_dependency_from_another_compiled_unit_cannot_be_borrowed(self):
        original = self.extract
        def changed(source, **kwargs):
            unit = original(source, **kwargs)
            if source == self.main:
                record = json.loads(unit.receipt.read_text())
                # helper.c belongs to the build, but was not an input to main.o.
                record['inputs'][str(self.helper)] = self.digest(self.helper)
                unit.receipt.write_text(json.dumps(record))
                return build.coverage.ExtractedUnit(unit.receipt, self.digest(unit.receipt))
            return unit
        with mock.patch.object(self, 'extract', side_effect=changed):
            with self.assertRaisesRegex(build.CoverageFailure, 'dependencies differ'):
                self.invoke()

    def test_graph_failure_keeps_all_extraction_receipts_without_success(self):
        with mock.patch.object(build.coverage, "extract_unit", side_effect=self.extract), \
             mock.patch.object(build.coverage, "prove_interval",
                               side_effect=build.coverage.CoverageFailure("prohibited call")):
            with self.assertRaisesRegex(build.coverage.CoverageFailure, "prohibited call"):
                build.build_terrain_coverage(
                    sources=self.sources, flags=self.flags, compiler=self.compiler,
                    preprocessor=self.preprocessor, cwd=self.root, manifest=self.manifest,
                    executable=self.executable, native_contract=self.contract,
                    native_build_pins=self.native_pins,
                    evidence=self.root / "evidence",
                )
        receipt = json.loads((self.root / "evidence/receipt.json").read_text())
        self.assertEqual(receipt["status"], "failed")
        self.assertEqual(len(receipt["extractions"]), 3)
        self.assertNotIn("summary", receipt)
        self.assertTrue((self.root / "evidence/unit-0002/receipt.json").is_file())


if __name__ == "__main__":
    unittest.main()
