"""Host-only admission tests; these fixtures do not claim real-game coverage."""
from __future__ import annotations

import copy
import hashlib
import json
from pathlib import Path
import sys
import tempfile
from types import SimpleNamespace
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[2]
sys.path[:0] = [str(ROOT), str(ROOT / "examples/caxecraft")]
import benchmark_renderer as benchmark
import dev_build_state as state
import dev_generation as generation
import dev_native_cache as native
import play
from renderer_interval import PREFIX, parse_interval_evidence
from renderer_native_contract import NativeTerrainContract

REAL_REQUEST_SNAPSHOT = play.play_request_snapshot


class RendererBenchmarkIntervalTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory(prefix="renderer-benchmark-contract-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        self.compiler = self.root / "cc"
        self.compiler.write_text("fixture compiler bytes; never executed")
        self.input = self.root / "authored.hx"
        self.input.write_text("one shared authored input")
        self.raylib = self.root / "raylib"
        self.raygui = self.root / "raygui"
        for directory in (self.raylib, self.raygui):
            directory.mkdir()
            (directory / "api.h").write_text("/* external header */")
        self.native_dependency = self.raylib / "detail.h"
        self.native_dependency.write_text("/* compiler-reported dependency outside the external header summary */")
        self.library = self.root / "raylib.a"
        self.library.write_text("fixed library")
        self.gui_library = self.root / "raygui.a"
        self.gui_library.write_text("fixed GUI library")
        self.args = benchmark.parse_args([])
        self.args.cc = str(self.compiler)
        self.args.source = self.root / "raylib-source"
        self.args.source.mkdir()
        self.args.authority = "offline-source"
        self.args.output_root = self.root
        self.addCleanup(mock.patch.stopall)
        mock.patch.dict(native.os.environ, {}, clear=True).start()
        mock.patch.object(benchmark.platform, "system", return_value="Linux").start()
        self.facts = {"schemaVersion": 1, "command": str(self.compiler), "path": str(self.compiler),
                      "bytes": self.compiler.stat().st_size, "sha256": self.sha(self.compiler),
                      "target": "fixture-target", "version": "fixture compiler", "environment": {}}
        mock.patch.object(native, "compiler_identity", return_value=self.facts).start()
        mock.patch.object(play, "play_request_snapshot", side_effect=self.request).start()
        self.contract = NativeTerrainContract(frozenset({"reviewed_draw"}), '{"librarySha256":"reviewed"}')
        mock.patch.object(play, "terrain_native_contract", return_value=self.contract).start()
        self.variants = {}
        self.reports = {}
        for renderer in ("chunk-cache", "immediate-baseline"):
            for observed in (False, True):
                key = (renderer, observed)
                self.variants[key], self.reports[key] = self.fixture(renderer, observed)
        self.handwritten = {"terrainMedianMicrosecondsPerFrame": 4, "faces": 23,
                            "artifacts": {"allocatorCallsInHandwrittenSource": 0,
                                          "gotoStatementsInHandwrittenSource": 0}}

    @staticmethod
    def sha(path):
        return hashlib.sha256(path.read_bytes()).hexdigest()

    def request(self, args, **unused):
        return state.request_snapshot(
            configuration={"renderer": args.renderer, "observeRendererInterval": args.observe_renderer_interval,
                           "platform": "linux", "optimization": "2", "layout": "split"},
            inputs=(state.InputPath("authored", self.input),), tools=(self.facts,), environment={},
        )

    def fixture(self, renderer, observed):
        variant = self.root / (renderer + ("-observed" if observed else "-timing"))
        variant.mkdir()
        transaction = generation.begin_transaction(variant)
        (transaction.generated / "include").mkdir()
        header = transaction.generated / "include/api.h"
        header.write_text("/* generated header */")
        # Static source counts are deliberately nonzero outside terrain work.
        (transaction.generated / "main.c").write_text(
            "void startup(void) { malloc(1); } /* hxrt_startup */\nvoid terrain(void) {}\n")
        manifest = {"build": {"sources": ["main.c"], "includeDirectories": ["include"]}}
        (transaction.generated / "hxc.manifest.json").write_text(json.dumps(manifest))
        generated = generation.publish_pointer(variant, generation.finalize_transaction(variant, transaction)).generated
        binary = variant / "bin/caxecraft"
        binary.parent.mkdir()
        binary.write_text(renderer + (" observed" if observed else " timing"))
        (binary.parent / "assets").mkdir()
        content = binary.parent / "content"
        content.mkdir()
        (content / "map.caxemap").write_text("shared runtime content")
        args = benchmark.benchmark_request_args(self.args, renderer, observed)
        request = self.request(args)
        external = (state.ExternalFile("raylib/library", self.library),
                    state.ExternalFile("raygui/library", self.gui_library),
                    state.ExternalFile("raylib/include/api.h", self.raylib / "api.h"),
                    state.ExternalFile("raygui/include/api.h", self.raygui / "api.h"))
        state.atomic_write_state(variant / play.PLAY_BUILD_STATE, state.build_state(
            request=request, outputs=state.output_snapshot(generated, binary), external_native_files=external))
        plan = play.native_compile_plan(
            generated, manifest, include_directory=self.raylib, raygui_include_directory=self.raygui,
            platform_name="linux", optimization="2", native_sanitizer_flags=(), observe_renderer_interval=observed)
        objects = []
        for index, (name, source) in enumerate(plan.sources):
            directory = variant / f"object-{index}"
            directory.mkdir()
            obj = directory / "object.o"
            obj.write_text("fixture object: " + name)
            # Independently authored owner metadata. Only compiler identification
            # is mocked; the production verifier checks keys, bytes and dependencies.
            base = {"schemaVersion": 1, "source": name, "sourceBytes": source.stat().st_size,
                    "sourceSha256": self.sha(source), "compiler": self.facts,
                    "compileFlags": list(plan.compile_flags),
                    "includeInventory": native.include_path_inventory(plan.include_roots)}
            dependencies = (source, generated / "include/api.h", self.raylib / "api.h", self.raygui / "api.h", self.native_dependency)
            body = {"schemaVersion": 1, "kind": "caxecraft-native-object", "base": base,
                    "dependencies": native.dependency_snapshot(dependencies, plan.dependency_roots),
                    "objectSha256": self.sha(obj)}
            key = state.canonical_digest(body)
            metadata = {**body, "key": key}
            state.atomic_write_state(directory / "metadata.json", {**metadata, "sha256": state.canonical_digest(metadata)})
            objects.append(native.NativeObject(name, key, obj, self.sha(obj), False))
        result = native.NativeBuildResult(tuple(objects), 0, len(objects), False)
        pins = native.verify_native_objects(
            objects=objects, sources=plan.sources, compiler=str(self.compiler), compile_flags=plan.compile_flags,
            include_roots=plan.include_roots, dependency_roots=plan.dependency_roots)
        report = {
            "world": {"stateHash": "same-scene"}, "script": {"id": "same-pilot", "inputHash": "same-input"},
            "native": {"platform": "linux"},
            "render": {"implementation": renderer, "rebuiltTerrainChunks": 1 if renderer == "chunk-cache" else 0,
                       "totalRebuiltTerrainChunks": 1 if renderer == "chunk-cache" else 0,
                       "terrainCacheValid": renderer == "chunk-cache", "visibleTerrainFaces": 23},
            "benchmark": {"measurementKind": "instrumented-correctness" if observed else "timing",
                          "reviewScreenshotSha256": "same-screenshot", "terrainMedianMicrosecondsPerFrame": 8,
                          "updateMedianMicrosecondsPerFrame": 2, "preparationMedianMicrosecondsPerFrame": 3},
        }
        static = None
        if observed:
            warmup = {"schemaVersion": 1, "phase": "warmup", "valid": True, "frames": 2,
                      "portalFrames": 0, "completed": 2, "allocationAttempts": 4,
                      "reallocationAttempts": 2, "collectionAttempts": 1,
                      "rootPushAttempts": 11, "rootPopAttempts": 10}
            total = dict(warmup, phase="complete", frames=14, completed=14,
                         rootPushAttempts=4294967308, rootPopAttempts=27)
            report["terrainInterval"] = parse_interval_evidence(
                PREFIX + json.dumps(warmup) + "\n" + PREFIX + json.dumps(total), measured_frames=12)
            evidence = variant / "terrain-coverage-fixture"
            evidence.mkdir()
            receipt = evidence / "receipt.json"
            ast = evidence / "proof.ast"
            ast.write_text("host-only extraction fixture")
            summary = {"schemaVersion": 1, "translationUnits": len(plan.sources), "reachableFunctions": 2,
                       "foreignLeaves": 1, "manifestSha256": self.sha(generated / "hxc.manifest.json"),
                       "executableSha256": self.sha(binary), "nativeContractSha256": "fixture-identity",
                       "claim": "host-only fixture; no actual static proof claimed"}
            inputs = {str(path): digest for path, digest in pins.pins}
            inputs.update({str(ast): self.sha(ast), str(binary): self.sha(binary),
                           str(generated / "hxc.manifest.json"): self.sha(generated / "hxc.manifest.json")})
            receipt.write_text(json.dumps({
                "status": "success", "summary": summary, "inputs": inputs,
                "nativeContract": {"identity": self.contract.identity_report(), "leaves": sorted(self.contract.leaves)},
                "manifest": str(generated / "hxc.manifest.json"), "executable": str(binary),
                "sources": [{"name": name, "path": str(path)} for name, path in plan.sources],
                "flags": list(pins.flags), "nativeObjects": [
                    {"source": item.source, "key": item.key, "sha256": item.sha256,
                     "path": str(item.path), "metadata": str(item.path.parent / "metadata.json")} for item in objects],
            }))
            static = SimpleNamespace(receipt=receipt, receipt_sha256=self.sha(receipt))
            report["staticTerrainCoverage"] = {**summary, "receiptSha256": self.sha(receipt)}
        binding = play.renderer_benchmark_binding(
            output_root=variant, generated=generated, executable=binary, request=request,
            content=state.inventory_inputs((state.InputPath("runtime-content", content),)),
            plan=plan, native_result=result, static_coverage=static)
        path = variant / "renderer-benchmark-binding.json"
        state.atomic_write_state(path, binding)
        report["benchmarkBindingSha256"] = self.sha(path)
        return variant, report

    def verify(self, renderer="chunk-cache", observed=True):
        key = (renderer, observed)
        return benchmark.verify_build_binding(self.reports[key], self.variants[key],
                                              args=self.args, renderer=renderer, observed=observed)

    def comparison(self):
        return benchmark.build_comparison(
            self.reports[("chunk-cache", False)], self.reports[("immediate-baseline", False)],
            self.variants[("chunk-cache", False)], self.variants[("immediate-baseline", False)], self.handwritten,
            cache_correctness=self.reports[("chunk-cache", True)], baseline_correctness=self.reports[("immediate-baseline", True)],
            cache_correctness_variant=self.variants[("chunk-cache", True)],
            baseline_correctness_variant=self.variants[("immediate-baseline", True)], args=self.args)

    def update_binding(self, mutate, key=("chunk-cache", True)):
        path = self.variants[key] / "renderer-benchmark-binding.json"
        binding = json.loads(path.read_text())
        mutate(binding)
        state.atomic_write_state(path, binding)
        self.reports[key]["benchmarkBindingSha256"] = self.sha(path)

    def test_startup_metrics_and_exact_root_attempts_are_retained(self):
        result = self.comparison()
        cache = result["chunkCache"]
        self.assertEqual(cache["artifacts"]["generatedAllocatorCalls"], 1)
        self.assertEqual(cache["artifacts"]["generatedHxrtReferences"], 1)
        self.assertEqual(cache["rootBookkeeping"], {"rootPushAttempts": 4294967297, "rootPopAttempts": 17})
        self.assertEqual(cache["terrainInterval"]["warmup"]["allocationAttempts"], 4)
        self.assertEqual(cache["terrainInterval"]["measured"]["allocationAttempts"], 0)
        self.assertEqual(cache["timing"]["measurementKind"], "timing")

    def test_timing_reports_alone_do_not_admit_interval_acceptance(self):
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "real-game interval and static coverage"):
            benchmark.build_comparison(
                self.reports[("chunk-cache", False)], self.reports[("immediate-baseline", False)],
                self.variants[("chunk-cache", False)], self.variants[("immediate-baseline", False)], self.handwritten)

    def test_missing_interval_or_static_coverage_cannot_pass(self):
        report = self.reports[("chunk-cache", True)]
        for name in ("terrainInterval", "staticTerrainCoverage"):
            saved = report.pop(name)
            with self.subTest(name=name), self.assertRaises(benchmark.BenchmarkFailure):
                self.comparison()
            report[name] = saved

    def test_each_measured_forbidden_attempt_fails_even_with_static_evidence(self):
        interval = self.reports[("chunk-cache", True)]["terrainInterval"]
        original = copy.deepcopy(interval)
        for name in ("allocationAttempts", "reallocationAttempts", "collectionAttempts"):
            interval["total"][name] += 1
            with self.subTest(name=name), self.assertRaises(benchmark.BenchmarkFailure):
                self.comparison()
            interval.clear(); interval.update(copy.deepcopy(original))

    def test_interval_frame_coverage_and_summary_are_rechecked(self):
        interval = self.reports[("chunk-cache", True)]["terrainInterval"]
        interval["measured"]["rootPushAttempts"] = 7
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "summary differs"):
            self.verify()

    def test_binding_digest_and_wrong_variant_receipt_fail(self):
        key = ("chunk-cache", True)
        self.reports[key]["benchmarkBindingSha256"] = "0" * 64
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "binding digest"):
            self.verify()
        path = self.variants[key] / "renderer-benchmark-binding.json"
        self.reports[key]["benchmarkBindingSha256"] = self.sha(path)
        self.reports[key]["staticTerrainCoverage"] = self.reports[("immediate-baseline", True)]["staticTerrainCoverage"]
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "receipt changed"):
            self.verify()

    def test_serialized_flags_sources_and_roots_do_not_authorize_another_plan(self):
        path = self.variants[("chunk-cache", True)] / "renderer-benchmark-binding.json"
        original = path.read_bytes()
        for field in ("compileFlags", "sources", "includeRoots", "dependencyRoots"):
            self.update_binding(lambda binding: binding["nativeBuild"][field].clear())
            with self.subTest(field=field), self.assertRaisesRegex(benchmark.BenchmarkFailure, "native plan"):
                self.verify()
            path.write_bytes(original)
            self.reports[("chunk-cache", True)]["benchmarkBindingSha256"] = self.sha(path)

    def test_native_object_and_header_drift_are_rejected(self):
        for path in (self.variants[("chunk-cache", False)] / "object-0/object.o",
                     self.native_dependency, self.compiler):
            original = path.read_bytes()
            path.write_text("changed")
            with self.subTest(path=path.name), self.assertRaises(benchmark.BenchmarkFailure):
                self.verify(observed=False)
            path.write_bytes(original)

    def test_source_content_and_binary_drift_are_rejected(self):
        variant = self.variants[("chunk-cache", True)]
        for path in (self.input, variant / "bin/content/map.caxemap", variant / "bin/caxecraft"):
            original = path.read_bytes()
            path.write_text("changed")
            with self.subTest(path=path.name), self.assertRaises(benchmark.BenchmarkFailure):
                self.verify()
            path.write_bytes(original)

    def test_static_receipt_failure_does_not_become_runtime_zero_pass(self):
        self.update_binding(lambda binding: binding["staticCoverageReceipt"].update(path="../receipt.json"))
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "escapes"):
            self.verify()

    def test_failed_static_proof_cannot_be_replaced_by_runtime_zero(self):
        variant = self.variants[("chunk-cache", True)]
        receipt = variant / "terrain-coverage-fixture/receipt.json"
        value = json.loads(receipt.read_text())
        value.update(status="failed", error="prohibited call: malloc")
        value.pop("summary")
        receipt.write_text(json.dumps(value))
        self.reports[("chunk-cache", True)]["staticTerrainCoverage"]["receiptSha256"] = self.sha(receipt)
        self.update_binding(lambda binding: binding["staticCoverageReceipt"].update(sha256=self.sha(receipt)))
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "another build"):
            self.verify()

    def test_static_extraction_input_drift_fails(self):
        path = self.variants[("chunk-cache", True)] / "terrain-coverage-fixture/proof.ast"
        path.write_text("changed extraction bytes")
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "static coverage input changed"):
            self.verify()

    def test_timing_and_correctness_cannot_use_different_content_or_native_inputs(self):
        timing = self.verify(observed=False)
        correctness = self.verify()
        for field in ("runtimeContent", "externalNativeFiles"):
            changed = copy.deepcopy(correctness)
            changed[field][0]["sha256"] = "0" * 64
            with self.subTest(field=field), self.assertRaises(benchmark.BenchmarkFailure):
                benchmark.require_observer_pair(timing, changed)

    def test_same_build_runtime_content_drift_fails_before_binding_publication(self):
        key = ("chunk-cache", False)
        variant = self.variants[key]
        path = variant / "renderer-benchmark-binding.json"
        before = json.loads(path.read_text())["runtimeContent"]
        (variant / "bin/content/map.caxemap").write_text("changed between pilot samples")
        with self.assertRaisesRegex(play.PlayFailure, "runtime content changed"):
            play.renderer_benchmark_binding(
                output_root=variant, generated=generation.current_generation(variant).generated,
                executable=variant / "bin/caxecraft", request=self.request(
                    benchmark.benchmark_request_args(self.args, *key)), content=before,
                plan=mock.sentinel.plan, native_result=mock.sentinel.result, static_coverage=None)

    def test_only_observer_configuration_can_differ(self):
        timing = self.verify(observed=False)
        correctness = self.verify()
        benchmark.require_observer_pair(timing, correctness)
        for field in ("files", "tools", "environment", "configuration"):
            changed = copy.deepcopy(correctness)
            if field in ("files", "tools"):
                changed["request"][field].append({"unexpected": "input"})
            else:
                changed["request"][field]["unexpected"] = "input"
            with self.subTest(field=field), self.assertRaises(benchmark.BenchmarkFailure):
                benchmark.require_observer_pair(timing, changed)

    def test_instrumented_timing_gameplay_screenshot_and_face_gates_remain(self):
        for key, path, value in [
            (("chunk-cache", False), ("benchmark", "measurementKind"), "instrumented-correctness"),
            (("chunk-cache", True), ("world", "stateHash"), "changed-scene"),
            (("chunk-cache", True), ("benchmark", "reviewScreenshotSha256"), "changed-frame"),
        ]:
            report = self.reports[key]
            old = report[path[0]][path[1]]
            report[path[0]][path[1]] = value
            with self.subTest(path=path), self.assertRaises(benchmark.BenchmarkFailure):
                self.comparison()
            report[path[0]][path[1]] = old
        self.handwritten["faces"] = 24
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "different final terrain faces"):
            self.comparison()

    def test_goto_remains_a_whole_project_rejection(self):
        variant = self.variants[("chunk-cache", False)]
        transaction = generation.begin_transaction(variant)
        (transaction.generated / "main.c").write_text("void f(void) { goto end; end:; }")
        (transaction.generated / "main.h").write_text("void f(void);")
        generation.publish_pointer(variant, generation.finalize_transaction(variant, transaction))
        with self.assertRaisesRegex(benchmark.BenchmarkFailure, "structural budget"):
            benchmark.source_evidence(variant)

    def test_prebuilt_raylib_uses_its_existing_source_cache_owner(self):
        source = self.args.source
        self.args.source = None
        self.args.prebuilt_raylib_cache = self.root / "prebuilt-source-cache"
        with mock.patch.object(play.provision, "pinned_source", return_value=source) as pinned:
            self.verify()
        self.assertEqual(pinned.call_args.args[0], self.args.prebuilt_raylib_cache.resolve())
        self.assertFalse(pinned.call_args.kwargs["allow_network"])

    def test_cache_off_benchmark_can_retain_native_object_evidence(self):
        variant = self.variants[("chunk-cache", False)]
        generated = generation.current_generation(variant).generated
        manifest = json.loads((generated / "hxc.manifest.json").read_text())
        manifest["build"].update(
            libraries=[{"name": name, "ownerModulePaths": ["Fixture"]} for name in ("raylib", "raygui", "m")],
            frameworks=[])
        with mock.patch.object(play, "NativeCache") as cache_type, \
                mock.patch.object(play.provision, "load_lock", return_value={}), \
                mock.patch.object(play.provision, "link_facts", return_value=(["raylib"], [])):
            cache = cache_type.return_value
            cache.objects.return_value = ()
            cache.link.return_value = False
            play.compile_native(
                generated, manifest, output=variant / "bin/caxecraft", include_directory=self.raylib,
                library=self.library, raygui_include_directory=self.raygui, raygui_library=self.gui_library,
                platform_name="linux", raylib_configuration="memory-software", cc=str(self.compiler),
                optimization="2", native_sanitizer_flags=(), cache_root=variant / "cache",
                cache_enabled=False, jobs=1, retain_objects=True)
        self.assertTrue(cache.link.call_args.kwargs["retain_objects"])
        self.assertFalse(cache_type.call_args.kwargs["enabled"])

    def test_reconstructed_request_matches_real_parser_and_snapshot_owner(self):
        # The other tests mock the request boundary to isolate admission. This
        # case exercises its real schema and parser, mocking only input/tool IO.
        with mock.patch.object(play, "play_request_snapshot", REAL_REQUEST_SNAPSHOT), \
                mock.patch.object(play, "play_build_inputs", return_value=(state.InputPath("authored", self.input),)), \
                mock.patch.object(play, "tool_identity", side_effect=lambda name, command, **kw: {"name": name, "command": command}), \
                mock.patch.dict(play.os.environ, {"AR": "fixture-archiver", "HAXE_STD_PATH": "/fixture/std"}):
            for observed in (False, True):
                for prebuilt in (False, True):
                    args = copy.copy(self.args)
                    args.generator = "Unix Makefiles" if prebuilt else "Ninja"
                    args.source = None if prebuilt else self.args.source
                    args.authority = "pinned-source" if prebuilt else "offline-source"
                    args.allow_network = not prebuilt
                    if prebuilt:
                        args.prebuilt_raylib_cache = self.root / "prebuilt-cache"
                        args.prebuilt_raylib_build = self.root / "prebuilt-build"
                        args.prebuilt_raylib_report = self.root / "prebuilt-report.json"
                    expected = self.root / (
                        "variants/linux-memory-software-split-pilot-move-jump-edit-immediate-baseline-benchmark"
                        + ("-interval-observer" if observed else ""))
                    (expected / "bin").mkdir(parents=True, exist_ok=True)
                    (expected / "bin/caxecraft-pilot-report.json").write_text("{}")
                    with mock.patch.object(benchmark.subprocess, "run", return_value=SimpleNamespace(returncode=0)) as run:
                        benchmark.run_renderer(args, "immediate-baseline", observed=observed)
                    parsed = play.parse_args(run.call_args.args[0][2:])
                    rebuilt = benchmark.benchmark_request_args(args, "immediate-baseline", observed)
                    actual = play.play_request_snapshot(parsed, platform_name="linux", selected_pilot=parsed.pilot,
                                                        native_sanitizer_flags=())
                    reconstructed = play.play_request_snapshot(
                        rebuilt, platform_name="linux", selected_pilot=benchmark.SCRIPT_ID,
                        native_sanitizer_flags=())
                    with self.subTest(observed=observed, prebuilt=prebuilt):
                        self.assertEqual(actual, reconstructed)
                        self.assertEqual(parsed.ar, "fixture-archiver")
                        self.assertEqual(parsed.allow_network, args.allow_network)
                        for field in ("source", "prebuilt_raylib_cache", "prebuilt_raylib_build", "prebuilt_raylib_report"):
                            value = getattr(args, field)
                            self.assertEqual(getattr(parsed, field), value.resolve() if value is not None else None)

    def test_runner_uses_distinct_observer_variant(self):
        expected = self.root / "variants/linux-memory-software-split-pilot-move-jump-edit-benchmark-interval-observer"
        (expected / "bin").mkdir(parents=True)
        (expected / "bin/caxecraft-pilot-report.json").write_text("{}")
        with mock.patch.object(benchmark.subprocess, "run", return_value=SimpleNamespace(returncode=0)) as run:
            self.assertEqual(benchmark.run_renderer(self.args, "chunk-cache", observed=True), expected)
        self.assertIn("--observe-renderer-interval", run.call_args.args[0])


if __name__ == "__main__":
    unittest.main()
