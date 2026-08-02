from __future__ import annotations

import importlib.util
import io
import json
import math
import sys
import tempfile
import threading
import time
import unittest
from pathlib import Path
from unittest import mock


ROOT = Path(__file__).resolve().parents[2]
RUNNER = ROOT / "scripts/ci/run_toolchain_shard.py"
ROUTE_SELECTOR = ROOT / "scripts/ci/select_pre_commit_route.py"


def load_runner():
    spec = importlib.util.spec_from_file_location("toolchain_shard_subject", RUNNER)
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {RUNNER}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


def load_route_selector():
    spec = importlib.util.spec_from_file_location(
        "pre_commit_route_subject", ROUTE_SELECTOR
    )
    if spec is None or spec.loader is None:
        raise RuntimeError(f"cannot load {ROUTE_SELECTOR}")
    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module
    try:
        spec.loader.exec_module(module)
    finally:
        del sys.modules[spec.name]
    return module


class ToolchainShardTests(unittest.TestCase):
    def setUp(self) -> None:
        self.runner = load_runner()
        self.route_selector = load_route_selector()

    def test_known_compiler_and_product_paths_use_affected_route(
        self,
    ) -> None:
        for path in (
            "src/reflaxe/c/ir/HxcIR.hx",
            "src/reflaxe/c/lowering/CBodyLowering.hx",
            "src/reflaxe/c/CCompiler.hx",
            "src/reflaxe/c/CPhaseTiming.hx",
            "scripts/ci/check_ci_policy.py",
            "scripts/hooks/pre-commit",
            "examples/caxecraft/src/caxecraft/domain/Vitals.hx",
        ):
            with self.subTest(path=path):
                self.assertEqual(
                    self.route_selector.select_route((path,)),
                    self.route_selector.AFFECTED,
                )

    def test_unknown_cross_cutting_paths_use_conservative_affected_route(self) -> None:
        for path in (
            "scripts/ci/run_toolchain_shard.py",
            "scripts/ci/new_check.py",
            "scripts/test/new_helper.py",
            "test/governance/test_new_policy.py",
            "docs/test-performance.md",
            ".github/workflows/governance.yml",
            "package.json",
            "src/reflaxe/c/new_layer/FuturePass.hx",
        ):
            with self.subTest(path=path):
                self.assertEqual(
                    self.route_selector.select_route((path,)),
                    self.route_selector.AFFECTED,
                )

    def test_narrow_paths_keep_focused_route(self) -> None:
        for path in (
            "test/differential/string-runtime/generated/Main.hx",
            "docs/string-runtime.md",
        ):
            with self.subTest(path=path):
                self.assertEqual(
                    self.route_selector.select_route((path,)),
                    self.route_selector.FOCUSED,
                )

    def test_one_affected_path_dominates_a_mixed_focused_change(self) -> None:
        self.assertEqual(
            self.route_selector.select_route(
                (
                    "docs/string-runtime.md",
                    "src/reflaxe/c/lowering/CBodyEmitter.hx",
                )
            ),
            self.route_selector.AFFECTED,
        )

    def test_unknown_cross_cutting_path_preserves_known_affected_owners(self) -> None:
        self.assertEqual(
            self.route_selector.select_route(
                (
                    "src/reflaxe/c/lowering/CBodyEmitter.hx",
                    "scripts/ci/run_toolchain_shard.py",
                )
            ),
            self.route_selector.AFFECTED,
        )

    def test_affected_owners_are_deterministic_and_deduplicated(self) -> None:
        owners = self.route_selector.select_affected_owners(
            (
                "src/reflaxe/c/ir/HxcIR.hx",
                "src/reflaxe/c/lowering/CBodyEmitter.hx",
                "src/reflaxe/c/runtime/RuntimeRequirementAnalyzer.hx",
                "examples/caxecraft/src/caxecraft/domain/WorldView.hx",
            )
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
                "test:body-lowering",
                "test:runtime-features",
                "test:span-lowering",
                "test:caxecraft-domain",
            ),
        )
        self.assertTrue(all(owner.reason for owner in owners))

    def test_automatic_smoke_owners_remain_fixed_and_bounded(self) -> None:
        for paths in (
            ("src/reflaxe/c/lowering/CBodyEmitter.hx",),
            ("src/reflaxe/c/new_layer/FuturePass.hx",),
            (
                "package.json",
                "src/reflaxe/c/lowering/CBodyEmitter.hx",
                "examples/caxecraft/play.py",
            ),
        ):
            with self.subTest(paths=paths):
                owners = self.route_selector.select_smoke_owners(paths)
                self.assertEqual(
                    tuple(owner.script for owner in owners),
                    (
                        "test:all-sources",
                        "test:hxc-ir",
                        "test:hello",
                        "snapshots:catalog",
                    ),
                )

    def test_caxecraft_only_change_selects_product_owner(self) -> None:
        owners = self.route_selector.select_affected_owners(
            ("examples/caxecraft/src/caxecraft/domain/Vitals.hx",)
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
                "test:caxecraft-domain",
            ),
        )

    def test_content_json_change_selects_focused_and_flagship_owners(self) -> None:
        focused_paths = (
            "examples/caxecraft/content-json-c.hxml",
            "examples/caxecraft/test/caxecraft/qa/ContentJsonProbe.hx",
            "examples/caxecraft/test/native/content_json_harness.c",
        )
        for path in focused_paths:
            with self.subTest(path=path):
                plan = self.route_selector.build_test_plan((path,))
                self.assertEqual(
                    [owner["script"] for owner in plan["taskOwners"]],
                    ["test:caxecraft-content-json"],
                )
                self.assertEqual(
                    plan["taskOwners"][0]["productSurfaces"],
                    ["compiler-admitted-slices", "runtime-memory-lifetime"],
                )
                self.assertFalse(plan["fullBackstop"]["required"])

        content_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/src/caxecraft/content/ContentJson.hx",)
        )
        self.assertEqual(
            [owner["script"] for owner in content_plan["taskOwners"]],
            [
                "test:caxecraft-package-manifest",
                "test:caxecraft-package-zip-export",
                "test:caxecraft-content-json",
                "test:caxecraft-runtime-schemas",
            ],
        )

        shared_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/src/caxecraft/text/Utf8Decoder.hx",)
        )
        self.assertEqual(
            [owner["script"] for owner in shared_plan["taskOwners"]],
            [
                "test:caxecraft-content-json",
                "test:caxecraft-runtime-schemas",
                "test:caxecraft-domain",
            ],
        )

    def test_package_transport_change_selects_exact_vertical_owners(self) -> None:
        export_paths = (
            "examples/caxecraft/package-zip-export-c.hxml",
            "examples/caxecraft/src/caxecraft/content/ContentPackageAssetClosure.hx",
            "examples/caxecraft/src/caxecraft/content/ContentPackageZipExport.hx",
            "examples/caxecraft/test/caxecraft/qa/ContentPackageZipExportProbe.hx",
            "examples/caxecraft/test/native/content_package_zip_export_harness.c",
        )
        for path in export_paths:
            with self.subTest(path=path):
                plan = self.route_selector.build_test_plan((path,))
                self.assertEqual(
                    [owner["script"] for owner in plan["taskOwners"]],
                    ["test:caxecraft-package-zip-export"],
                )
                self.assertEqual(
                    plan["taskOwners"][0]["productSurfaces"],
                    [
                        "compiler-admitted-slices",
                        "c-abi-native-ffi",
                        "runtime-memory-lifetime",
                        "diagnostics-source-mapping-downstream",
                    ],
                )
                self.assertFalse(plan["fullBackstop"]["required"])

        source_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/test/caxecraft/qa/ContentPackageZipSourceProbe.hx",)
        )
        self.assertEqual(
            [owner["script"] for owner in source_plan["taskOwners"]],
            ["test:caxecraft-package-zip-source"],
        )

        package_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/caxecraft.package.json",)
        )
        self.assertEqual(
            [owner["script"] for owner in package_plan["taskOwners"]],
            [
                "test:caxecraft-package-manifest",
                "test:caxecraft-package-zip-export",
            ],
        )
        self.assertEqual(
            package_plan["affectedExtended"]["productSurfaces"],
            [
                "c-abi-native-ffi",
                "compiler-admitted-slices",
                "diagnostics-source-mapping-downstream",
                "runtime-memory-lifetime",
            ],
        )

    def test_runtime_schema_change_selects_vertical_schema_owner(self) -> None:
        focused_paths = (
            "examples/caxecraft/runtime-schemas-c.hxml",
            "examples/caxecraft/src/caxecraft/content/RuntimeContentPack.hx",
            "examples/caxecraft/src/caxecraft/localization/RuntimeUiCatalog.hx",
            "examples/caxecraft/test/caxecraft/qa/RuntimeSchemasProbe.hx",
            "examples/caxecraft/test/native/runtime_schemas_harness.c",
        )
        for path in focused_paths:
            with self.subTest(path=path):
                plan = self.route_selector.build_test_plan((path,))
                self.assertEqual(
                    [owner["script"] for owner in plan["taskOwners"]],
                    ["test:caxecraft-runtime-schemas"],
                )
                self.assertEqual(
                    plan["taskOwners"][0]["productSurfaces"],
                    ["compiler-admitted-slices", "runtime-memory-lifetime"],
                )
                self.assertFalse(plan["fullBackstop"]["required"])

        data_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/packs/caxecraft/base/content.json",)
        )
        self.assertEqual(
            [owner["script"] for owner in data_plan["taskOwners"]],
            [
                "test:caxecraft-runtime-schemas",
                "test:caxecraft-runtime-content-generation",
                "test:caxecraft-domain",
            ],
        )

    def test_runtime_content_change_selects_atomic_generation_owner(self) -> None:
        focused_paths = (
            "examples/caxecraft/runtime-content-generation-c.hxml",
            "examples/caxecraft/src/caxecraft/content/RuntimeContentGeneration.hx",
            "examples/caxecraft/src/caxecraft/content/RuntimeContentDigest.hx",
            "examples/caxecraft/test/caxecraft/qa/RuntimeContentGenerationProbe.hx",
            "examples/caxecraft/test/native/runtime_content_generation_harness.c",
            "examples/caxecraft/packs/caxecraft/base/runtime-content.json",
        )
        for path in focused_paths:
            with self.subTest(path=path):
                plan = self.route_selector.build_test_plan((path,))
                self.assertEqual(
                    [owner["script"] for owner in plan["taskOwners"]],
                    ["test:caxecraft-runtime-content-generation"],
                )
                self.assertEqual(
                    plan["taskOwners"][0]["productSurfaces"],
                    [
                        "compiler-admitted-slices",
                        "c-abi-native-ffi",
                        "runtime-memory-lifetime",
                        "diagnostics-source-mapping-downstream",
                    ],
                )
                self.assertFalse(plan["fullBackstop"]["required"])

    def test_complete_publication_change_selects_focused_and_native_owners(self) -> None:
        for path in (
            "examples/caxecraft/runtime-content-publication.hxml",
            "examples/caxecraft/test/caxecraft/qa/RuntimeContentPublicationProbe.hx",
        ):
            with self.subTest(path=path):
                plan = self.route_selector.build_test_plan((path,))
                self.assertEqual(
                    [owner["script"] for owner in plan["taskOwners"]],
                    ["test:caxecraft-runtime-content-publication"],
                )
                self.assertEqual(
                    plan["taskOwners"][0]["productSurfaces"],
                    ["runtime-memory-lifetime"],
                )
        self.assertFalse(plan["fullBackstop"]["required"])

        owner_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/src/caxecraft/content/ActiveRuntimeContent.hx",)
        )
        self.assertEqual(
            [owner["script"] for owner in owner_plan["taskOwners"]],
            [
                "test:caxecraft-runtime-content-generation",
                "test:caxecraft-runtime-content-publication",
            ],
        )
        self.assertFalse(owner_plan["fullBackstop"]["required"])

        shared_level_plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/src/caxecraft/content/RuntimeLevelLoader.hx",)
        )
        self.assertEqual(
            [owner["script"] for owner in shared_level_plan["taskOwners"]],
            [
                "test:caxecraft-runtime-content-generation",
                "test:caxecraft-campaign-runtime",
                "test:caxecraft-domain",
            ],
        )
        self.assertFalse(shared_level_plan["fullBackstop"]["required"])

    def test_campaign_change_selects_its_real_vertical_owner(self) -> None:
        focused_paths = (
            "examples/caxecraft/campaign-runtime-c.hxml",
            "examples/caxecraft/src/caxecraft/content/CampaignManifest.hx",
            "examples/caxecraft/src/caxecraft/content/CampaignRuntime.hx",
            "examples/caxecraft/test/caxecraft/qa/CampaignRuntimeProbe.hx",
            "examples/caxecraft/test/native/campaign_runtime_harness.c",
            "examples/caxecraft/campaigns/first-adventure/campaign.json",
            "examples/caxecraft/scenarios/first-adventure/western-falls.caxemap",
        )
        for path in focused_paths:
            with self.subTest(path=path):
                plan = self.route_selector.build_test_plan((path,))
                self.assertEqual(
                    [owner["script"] for owner in plan["taskOwners"]],
                    ["test:caxecraft-campaign-runtime"],
                )
                self.assertEqual(
                    plan["taskOwners"][0]["productSurfaces"],
                    [
                        "compiler-admitted-slices",
                        "c-abi-native-ffi",
                        "runtime-memory-lifetime",
                        "diagnostics-source-mapping-downstream",
                    ],
                )
                self.assertFalse(plan["fullBackstop"]["required"])

    def test_shared_focused_content_fixture_selects_every_semantic_consumer(self) -> None:
        plan = self.route_selector.build_test_plan(
            ("examples/caxecraft/test/caxecraft/qa/FocusedContentFixture.hx",)
        )
        self.assertEqual(
            [owner["script"] for owner in plan["taskOwners"]],
            [
                "test:caxecraft-actor-composition",
                "test:caxecraft-resolved-level-plan",
                "test:caxecraft-content-generation",
                "test:caxecraft-runtime-level-loader",
                "test:caxecraft-session",
                "test:caxecraft-gameplay",
                "test:caxecraft-domain",
            ],
        )
        self.assertFalse(plan["fullBackstop"]["required"])

    def test_replay_change_selects_incremental_invalidation_owner(self) -> None:
        owners = self.route_selector.select_affected_owners(
            ("src/reflaxe/c/lowering/CBodyFunctionReplayCache.hx",)
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
                "test:incremental-backend",
                "test:body-lowering",
                "test:span-lowering",
            ),
        )

    def test_c_import_and_raygui_change_selects_exact_affected_owners(self) -> None:
        owners = self.route_selector.select_affected_owners(
            (
                "src/reflaxe/c/interop/CImportRegistry.hx",
                "std/c/Ref.hx",
                "src/raygui/Raygui.hx",
                "docs/specs/raygui-core-selection.json",
            )
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
                "test:c-import",
                "test:raygui-binding",
            ),
        )

    def test_shared_order_and_macro_change_selects_exact_affected_owners(self) -> None:
        owners = self.route_selector.select_affected_owners(
            (
                "src/reflaxe/c/CUtf8Order.hx",
                "src/reflaxe/c/macros/TypedCContractMacro.hx",
            )
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
                "test:project-emitter",
                "test:symbol-registry",
                "test:typed-c",
            ),
        )

    def test_profiler_and_specialization_change_selects_exact_affected_owners(
        self,
    ) -> None:
        owners = self.route_selector.select_affected_owners(
            (
                "src/reflaxe/c/CPhaseTiming.hx",
                "src/reflaxe/c/lowering/CBodyEnum.hx",
                "src/reflaxe/c/lowering/CGenericSpecialization.hx",
                "examples/caxecraft/profile_compiler.py",
            )
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
                "test:body-lowering",
                "test:span-lowering",
                "test:generic-specialization",
            ),
        )
        self.assertNotIn(
            "test:caxecraft-domain", tuple(owner.script for owner in owners)
        )

    def test_profile_consumer_uses_governance_without_game_domain(self) -> None:
        owners = self.route_selector.select_affected_owners(
            (
                "examples/caxecraft/profile_compiler.py",
                "test/governance/test_caxecraft_timing.py",
            )
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
            ),
        )

    def test_agent_plan_separates_task_smoke_and_hosted_evidence(self) -> None:
        plan = self.route_selector.build_test_plan(
            (
                "scripts/ci/select_pre_commit_route.py",
                "scripts/ci/run_local_gate.py",
                "docs/test-performance.md",
                "test/governance/test_local_gate_evidence.py",
            )
        )
        self.assertEqual(plan["schemaVersion"], 3)
        self.assertEqual(plan["route"], self.route_selector.AFFECTED)
        self.assertEqual(
            [owner["script"] for owner in plan["taskOwners"]],
            ["test:governance"],
        )
        self.assertEqual(
            plan["taskOwners"][0]["localCommand"],
            "npm run test:governance",
        )
        self.assertEqual(
            [owner["script"] for owner in plan["localCommitSmoke"]],
            [
                "test:governance",
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
            ],
        )
        self.assertEqual(plan["hostedRequired"]["check"], "Governance")
        self.assertEqual(
            plan["affectedExtended"],
            {
                "status": "no-claim-bearing-surface",
                "productSurfaces": [],
                "owners": [],
                "scope": (
                    "secondary compilers, sanitizers, platform profiles, and downstream "
                    "owners selected directly from the affected scorecards; selection "
                    "remains observation-only while R2 stays exhaustive"
                ),
            },
        )
        self.assertEqual(
            plan["fullBackstop"],
            {
                "localCommand": "npm run test:toolchain:parallel -- --with-native",
                "hostedCheck": "Governance",
                "scope": "complete cold active suite plus native backstop and selector-miss audit",
                "required": False,
                "reasons": [],
            },
        )
        self.assertEqual(plan["releaseQualification"]["status"], "unimplemented-not-a-pass")
        self.assertIsNone(plan["releaseQualification"]["command"])
        self.assertEqual(
            plan["coldSnapshotAudit"],
            {
                "schedule": "path-triggered, weekly, or explicit dispatch",
                "script": "snapshots:check",
            },
        )
        self.assertEqual(
            plan["officialHaxeQualification"],
            {
                "status": "readiness-only-not-a-pass",
                "owner": "haxe_c-6k7",
            },
        )

    def test_agent_plan_reports_normal_focused_owner(self) -> None:
        plan = self.route_selector.build_test_plan(("test/body_lowering/run.py",))
        self.assertEqual(plan["route"], self.route_selector.FOCUSED)
        self.assertEqual(
            [owner["script"] for owner in plan["taskOwners"]],
            ["test:body-lowering"],
        )
        self.assertEqual(
            plan["taskOwners"][0]["localCommand"],
            "npm run test:body-lowering",
        )
        self.assertEqual(
            plan["taskOwners"][0]["productSurfaces"],
            ["compiler-admitted-slices"],
        )
        hxc_ir_plan = self.route_selector.build_test_plan(
            ("src/reflaxe/c/ir/HxcIR.hx",)
        )
        self.assertIn(
            "test:hxc-ir",
            [owner["script"] for owner in hxc_ir_plan["taskOwners"]],
        )
        hxc_ir_owner = next(
            owner
            for owner in hxc_ir_plan["taskOwners"]
            if owner["script"] == "test:hxc-ir"
        )
        self.assertEqual(
            hxc_ir_owner["localCommand"],
            "npm run --silent test:local-gate -- test:hxc-ir",
        )
        hello_plan = self.route_selector.build_test_plan(
            ("examples/hello/run.py",)
        )
        self.assertEqual(
            [owner["script"] for owner in hello_plan["taskOwners"]],
            ["test:hello"],
        )
        self.assertEqual(
            hello_plan["taskOwners"][0]["localCommand"],
            "npm run test:hello",
        )
        self.assertEqual(
            hello_plan["affectedExtended"]["productSurfaces"],
            [
                "compiler-admitted-slices",
                "diagnostics-source-mapping-downstream",
                "runtime-memory-lifetime",
                "toolchain-platform-portability",
            ],
        )
        self.assertEqual(
            [owner["script"] for owner in hello_plan["affectedExtended"]["owners"]],
            [
                "test:primitive-differential",
                "test:caxecraft-domain:full",
                "test:diagnostics",
                "test:gc-runtime",
                "test:native",
                "test:build-adapters",
            ],
        )
        project_plan = self.route_selector.build_test_plan(
            ("test/project_emitter/run.py",)
        )
        self.assertEqual(
            [owner["script"] for owner in project_plan["taskOwners"]],
            ["test:project-emitter", "test:build-adapters:local"],
        )
        project_owner = next(
            owner
            for owner in project_plan["taskOwners"]
            if owner["script"] == "test:project-emitter"
        )
        self.assertEqual(
            project_owner["localCommand"],
            "npm run --silent test:local-gate -- test:project-emitter",
        )
        shared_runner_plan = self.route_selector.build_test_plan(
            ("test/typed_ast/run.py",)
        )
        self.assertEqual(
            [owner["script"] for owner in shared_runner_plan["taskOwners"]],
            ["test:incremental-backend", "test:typed-ast"],
        )

    def test_agent_plan_keeps_unmapped_focused_owner_explicit(self) -> None:
        plan = self.route_selector.build_test_plan(("docs/string-runtime.md",))
        self.assertEqual(plan["route"], self.route_selector.FOCUSED)
        self.assertEqual(plan["taskOwners"], [])
        self.assertEqual(
            plan["taskOwnerSelection"],
            "required-from-owning-issue-or-nearest-package-script",
        )
        self.assertEqual(plan["localCommitSmoke"], [])
        self.assertTrue(plan["fullBackstop"]["required"])
        self.assertIn("no semantic task owner", plan["fullBackstop"]["reasons"][0])

    def test_scorecards_are_the_only_product_surface_mapping_authority(self) -> None:
        string_plan = self.route_selector.build_test_plan(
            ("test/differential/string-runtime/run.py",)
        )
        self.assertEqual(
            string_plan["taskOwners"][0]["productSurfaces"],
            ["compiler-admitted-slices", "runtime-memory-lifetime"],
        )
        policy_plan = self.route_selector.build_test_plan(
            ("scripts/ci/check_fixture_policy.py",)
        )
        fixture_owner = next(
            owner
            for owner in policy_plan["taskOwners"]
            if owner["script"] == "test:fixture-policy"
        )
        self.assertEqual(fixture_owner["productSurfaces"], [])
        extended_owner_plan = self.route_selector.build_test_plan(
            ("test/primitive_differential/run.py",)
        )
        self.assertEqual(
            extended_owner_plan["taskOwners"][0]["productSurfaces"],
            ["compiler-admitted-slices"],
        )
        self.assertFalse(extended_owner_plan["fullBackstop"]["required"])

    def test_unknown_target_surface_requires_the_full_r4_backstop(self) -> None:
        plan = self.route_selector.build_test_plan(("std/c/NewSurface.hx",))
        self.assertEqual(plan["route"], self.route_selector.AFFECTED)
        self.assertEqual(plan["taskOwners"], [])
        self.assertTrue(plan["fullBackstop"]["required"])
        self.assertEqual(
            plan["fullBackstop"]["reasons"],
            ["no semantic task owner was inferred"],
        )

    def test_agent_plan_json_is_stable_and_machine_readable(self) -> None:
        stdout = io.StringIO()
        with mock.patch.object(sys, "stdin", io.StringIO("package.json\n")):
            with mock.patch.object(sys, "stdout", stdout):
                self.assertEqual(
                    self.route_selector.main(("--plan", "--json")),
                    0,
                )
        plan = json.loads(stdout.getvalue())
        self.assertEqual(plan["changedPaths"], ["package.json"])
        self.assertEqual(
            [owner["script"] for owner in plan["taskOwners"]],
            ["test:governance"],
        )
        lock_plan = self.route_selector.build_test_plan(("package-lock.json",))
        self.assertEqual(
            [owner["script"] for owner in lock_plan["taskOwners"]],
            ["test:governance"],
        )

    def test_unknown_cross_cutting_change_runs_conservative_base_smoke(self) -> None:
        owners = self.route_selector.select_affected_owners(
            ("src/reflaxe/c/new_layer/FuturePass.hx",)
        )
        self.assertEqual(
            tuple(owner.script for owner in owners),
            (
                "test:all-sources",
                "test:hxc-ir",
                "test:hello",
                "snapshots:catalog",
            ),
        )

    def test_actual_partition_and_local_isolation_are_exact(self) -> None:
        scripts = self.runner.load_scripts()
        canonical = self.runner.validate_partition(scripts)
        self.assertEqual(len(canonical), 74)
        self.assertEqual(tuple(self.runner.SHARDS), self.runner.SHARD_ORDER)
        self.assertEqual(
            tuple(self.runner.LOCAL_PARALLEL_ISOLATION), self.runner.SHARD_ORDER
        )
        self.assertEqual(canonical[-1], "snapshots:catalog")
        self.assertNotIn("snapshots:check", canonical)

    def test_required_instruction_links_are_safe_fingerprint_inputs(self) -> None:
        tracked = self.runner.git_bytes(
            ["ls-files", "-z", "--", *self.runner.RELEVANT_UNTRACKED_ROOTS]
        )
        tracked_names = {
            name.decode("utf-8", errors="strict")
            for name in tracked.split(b"\0")
            if name
        }
        self.assertTrue(
            self.runner.is_reviewed_instruction_link(
                "examples/caxecraft/CLAUDE.md", tracked_names
            )
        )
        self.assertFalse(
            self.runner.is_reviewed_instruction_link("AGENTS.md", tracked_names)
        )
        self.assertEqual(len(self.runner.relevant_worktree_digest()), 64)

    def test_timing_report_is_ordered_and_path_free(self) -> None:
        completed = [
            self.runner.subprocess.CompletedProcess([], 0),
            self.runner.subprocess.CompletedProcess([], 0),
        ]
        clock = iter((0, 1_000_000, 11_000_000, 12_000_000, 32_000_000, 35_000_000))
        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(self.runner, "SHARDS", {"sample": ("one", "two")}),
                mock.patch.object(self.runner.subprocess, "run", side_effect=completed),
                mock.patch.object(
                    self.runner.time, "monotonic_ns", side_effect=lambda: next(clock)
                ),
            ):
                self.runner.run_shard(
                    "sample",
                    {"one": "first", "two": "second"},
                    timing_report=report_path,
                    stream=io.StringIO(),
                )
            report = json.loads(report_path.read_text(encoding="utf-8"))

        self.assertEqual(report["schemaVersion"], 1)
        self.assertEqual(report["shard"], "sample")
        self.assertEqual(report["outcome"], "passed")
        self.assertEqual(report["durationMs"], 35)
        self.assertEqual(
            report["commands"],
            [
                {
                    "script": "one",
                    "outcome": "passed",
                    "exitCode": 0,
                    "durationMs": 10,
                },
                {
                    "script": "two",
                    "outcome": "passed",
                    "exitCode": 0,
                    "durationMs": 20,
                },
            ],
        )
        self.assertNotIn(str(ROOT), json.dumps(report))

    def test_failure_still_writes_the_completed_timing_prefix(self) -> None:
        clock = iter((0, 1_000_000, 6_000_000, 8_000_000))
        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(self.runner, "SHARDS", {"sample": ("broken",)}),
                mock.patch.object(
                    self.runner.subprocess,
                    "run",
                    return_value=self.runner.subprocess.CompletedProcess([], 7),
                ),
                mock.patch.object(
                    self.runner.time, "monotonic_ns", side_effect=lambda: next(clock)
                ),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure, "exit 7"
                ):
                    self.runner.run_shard(
                        "sample",
                        {"broken": "false"},
                        timing_report=report_path,
                        stream=io.StringIO(),
                    )
            report = json.loads(report_path.read_text(encoding="utf-8"))

        self.assertEqual(report["outcome"], "failed")
        self.assertEqual(report["commands"][0]["exitCode"], 7)
        self.assertEqual(report["commands"][0]["durationMs"], 5)
        self.assertEqual(report["durationMs"], 8)

    def test_parallel_job_count_is_bounded(self) -> None:
        self.assertEqual(self.runner.requested_jobs(1), 1)
        self.assertEqual(self.runner.requested_jobs(4), 4)
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "between 1 and 4"
        ):
            self.runner.requested_jobs(5)

    def test_resource_aware_worker_selection(self) -> None:
        idle = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=1.0,
            load_was_probed=True,
        )
        moderate = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=7.0,
            load_was_probed=True,
        )
        loaded = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=13.0,
            load_was_probed=True,
        )
        unavailable = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=None,
            load_was_probed=True,
        )
        invalid = self.runner.resolve_job_selection(
            None,
            environment={},
            logical_cpus=12,
            one_minute_load=math.nan,
            load_was_probed=True,
        )

        self.assertEqual(idle.jobs, 4)
        self.assertEqual(moderate.jobs, 2)
        self.assertEqual(loaded.jobs, 1)
        self.assertEqual(unavailable.jobs, 2)
        self.assertEqual(unavailable.source, "automatic-fallback")
        self.assertEqual(invalid.jobs, 2)

    def test_explicit_worker_precedence_and_malformed_environment(self) -> None:
        cli = self.runner.resolve_job_selection(
            4, environment={"HXC_TEST_JOBS": "1"}, logical_cpus=2
        )
        environment = self.runner.resolve_job_selection(
            None, environment={"HXC_TEST_JOBS": "3"}, logical_cpus=2
        )
        self.assertEqual((cli.jobs, cli.source), (4, "--jobs"))
        self.assertEqual(
            (environment.jobs, environment.source), (3, "HXC_TEST_JOBS")
        )
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "must be an integer"
        ):
            self.runner.resolve_job_selection(
                None, environment={"HXC_TEST_JOBS": "many"}, logical_cpus=8
            )
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "between 1 and 4"
        ):
            self.runner.resolve_job_selection(
                None, environment={"HXC_TEST_JOBS": "0"}, logical_cpus=8
            )

    def test_arbitrary_inherited_environment_drift_invalidates_evidence(self) -> None:
        baseline = self.runner.environment_digest(
            {"PATH": "/tools", "PYTHONOPTIMIZE": "0", "HXC_TEST_JOBS": "1"}
        )
        changed_semantics = self.runner.environment_digest(
            {"PATH": "/tools", "PYTHONOPTIMIZE": "1", "HXC_TEST_JOBS": "1"}
        )
        changed_scheduling_only = self.runner.environment_digest(
            {"PATH": "/tools", "PYTHONOPTIMIZE": "0", "HXC_TEST_JOBS": "4"}
        )
        self.assertNotEqual(baseline, changed_semantics)
        self.assertEqual(baseline, changed_scheduling_only)

    def sample_inputs(self, shard: str, suffix: str = "") -> dict[str, str]:
        return {
            "stagedTree": "a" * 40 + suffix,
            "worktreeDigest": "b" * 64 + suffix,
            "commandDigest": f"command-{shard}{suffix}",
            "locksDigest": "c" * 64 + suffix,
            "toolsDigest": "d" * 64 + suffix,
            "environmentDigest": "e" * 64 + suffix,
            "runnerDigest": "f" * 64 + suffix,
            "hookDigest": "0" * 64 + suffix,
            "hostDigest": "1" * 64 + suffix,
            f"snapshotContent:test:sample-{shard}": "2" * 64 + suffix,
        }

    def successful_report(self, shard: str) -> dict[str, object]:
        commands: list[dict[str, object]] = []
        for script in self.runner.SHARDS[shard]:
            command: dict[str, object] = {
                "script": script,
                "outcome": "passed",
                "exitCode": 0,
                "durationMs": 1,
            }
            if script == self.runner.CAXECRAFT_SCRIPT:
                command["phaseTiming"] = self.caxecraft_timing_report()
            commands.append(command)
        return {
            "schemaVersion": 1,
            "shard": shard,
            "outcome": "passed",
            "durationMs": len(self.runner.SHARDS[shard]),
            "commands": commands,
        }

    def caxecraft_timing_report(self) -> dict[str, object]:
        return {
            "schemaVersion": 1,
            "suite": "caxecraft-domain",
            "mode": "full",
            "outcome": "passed",
            "durationMs": 12,
            "summary": {
                "haxeRequests": sum(
                    requests
                    for _, requests in self.runner.CAXECRAFT_FULL_PHASES
                )
            },
            "phases": [
                {
                    "name": name,
                    "outcome": "passed",
                    "durationMs": 1,
                    "haxeRequests": requests,
                }
                for name, requests in self.runner.CAXECRAFT_FULL_PHASES
            ],
        }

    def test_caxecraft_phase_timing_is_embedded_and_validated(self) -> None:
        def fake_run(command, **kwargs):
            timing_path = Path(kwargs["env"][self.runner.CAXECRAFT_TIMING_ENV])
            timing_path.write_text(
                json.dumps(self.caxecraft_timing_report()), encoding="utf-8"
            )
            return self.runner.subprocess.CompletedProcess(command, 0)

        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(
                    self.runner,
                    "SHARDS",
                    {"caxecraft": (self.runner.CAXECRAFT_SCRIPT,)},
                ),
                mock.patch.object(self.runner.subprocess, "run", side_effect=fake_run),
            ):
                self.runner.run_shard(
                    "caxecraft",
                    {self.runner.CAXECRAFT_SCRIPT: "ignored"},
                    timing_report=report_path,
                    stream=io.StringIO(),
                )
                payload = json.loads(report_path.read_text(encoding="utf-8"))
                self.runner.validate_successful_shard_report(payload, "caxecraft")

        timing = payload["commands"][0]["phaseTiming"]
        self.assertEqual(timing["summary"]["haxeRequests"], 12)
        self.assertEqual(
            [phase["name"] for phase in timing["phases"]],
            [name for name, _ in self.runner.CAXECRAFT_FULL_PHASES],
        )
        self.assertNotIn(str(ROOT), json.dumps(payload))

    def test_failed_caxecraft_phase_prefix_is_preserved(self) -> None:
        failed_timing = self.caxecraft_timing_report()
        failed_timing["outcome"] = "failed"
        failed_timing["phases"] = failed_timing["phases"][:4]
        failed_timing["phases"][-1]["outcome"] = "failed"
        failed_timing["summary"] = {"haxeRequests": 8}

        def fake_run(command, **kwargs):
            timing_path = Path(kwargs["env"][self.runner.CAXECRAFT_TIMING_ENV])
            timing_path.write_text(json.dumps(failed_timing), encoding="utf-8")
            return self.runner.subprocess.CompletedProcess(command, 7)

        with tempfile.TemporaryDirectory() as temporary:
            report_path = Path(temporary) / "timing.json"
            with (
                mock.patch.object(
                    self.runner,
                    "SHARDS",
                    {"caxecraft": (self.runner.CAXECRAFT_SCRIPT,)},
                ),
                mock.patch.object(self.runner.subprocess, "run", side_effect=fake_run),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure, "exit 7"
                ):
                    self.runner.run_shard(
                        "caxecraft",
                        {self.runner.CAXECRAFT_SCRIPT: "ignored"},
                        timing_report=report_path,
                        stream=io.StringIO(),
                    )
            payload = json.loads(report_path.read_text(encoding="utf-8"))

        command = payload["commands"][0]
        self.assertEqual(command["outcome"], "failed")
        self.assertEqual(command["phaseTiming"], failed_timing)

    def test_every_evidence_input_drift_invalidates_reuse(self) -> None:
        inputs = self.sample_inputs("contracts")
        record = self.runner.evidence_record("contracts", inputs, now=100)
        valid, _, _ = self.runner.validate_reusable_evidence(
            record, "contracts", inputs, now=101
        )
        self.assertTrue(valid)

        for field in inputs:
            with self.subTest(field=field):
                changed = dict(inputs)
                changed[field] += "changed"
                valid, reason, _ = self.runner.validate_reusable_evidence(
                    record, "contracts", changed, now=101
                )
                self.assertFalse(valid)
                self.assertIn("inputs changed", reason)

    def test_malformed_expired_failed_future_and_symlink_evidence_is_rejected(self) -> None:
        inputs = self.sample_inputs("contracts")
        record = self.runner.evidence_record("contracts", inputs, now=100)

        malformed = dict(record)
        malformed["extra"] = True
        expired = dict(record)
        failed = dict(record)
        failed["outcome"] = "failed"
        malformed_basis = dict(record)
        malformed_basis["basis"] = {"kind": "snapshot-refresh", "scripts": []}
        future = self.runner.evidence_record("contracts", inputs, now=500)

        self.assertFalse(
            self.runner.validate_reusable_evidence(
                malformed, "contracts", inputs, now=101
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                expired,
                "contracts",
                inputs,
                now=100 + self.runner.EVIDENCE_TTL_SECONDS,
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                failed, "contracts", inputs, now=101
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                malformed_basis, "contracts", inputs, now=101
            )[0]
        )
        self.assertFalse(
            self.runner.validate_reusable_evidence(
                future, "contracts", inputs, now=101
            )[0]
        )

        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            target = root / "target.json"
            target.write_text(json.dumps(record), encoding="utf-8")
            link = root / "contracts.json"
            link.symlink_to(target)
            reusable, reason, _ = self.runner.read_reusable_evidence(
                link, "contracts", inputs, now=101
            )
        self.assertFalse(reusable)
        self.assertIn("symlink", reason)

    def test_retry_reuses_three_successes_and_runs_only_failed_shard(self) -> None:
        inputs = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        failed_shard = "lowering-semantics"
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for shard in self.runner.SHARD_ORDER:
                record = self.runner.evidence_record(shard, inputs[shard], now=100)
                if shard == failed_shard:
                    record["outcome"] = "failed"
                self.runner.write_timing_report(root / f"{shard}.json", record)
            reused, rejected = self.runner.classify_reusable_evidence(
                root, inputs, now=101
            )

        self.assertEqual(
            tuple(reused),
            ("contracts", "lowering-objects", "caxecraft"),
        )
        self.assertEqual(tuple(rejected), (failed_shard,))
        self.assertEqual(self.runner.pending_shards(reused), (failed_shard,))

    def test_snapshot_content_change_invalidates_only_its_owning_shard(self) -> None:
        inputs = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        changed = {shard: dict(values) for shard, values in inputs.items()}
        changed["lowering-objects"][
            "snapshotContent:test:sample-lowering-objects"
        ] = "changed"
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            for shard in self.runner.SHARD_ORDER:
                self.runner.write_timing_report(
                    root / f"{shard}.json",
                    self.runner.evidence_record(shard, inputs[shard], now=100),
                )
            reused, rejected = self.runner.classify_reusable_evidence(
                root, changed, now=101
            )

        self.assertEqual(
            tuple(reused),
            ("contracts", "lowering-semantics", "caxecraft"),
        )
        self.assertEqual(tuple(rejected), ("lowering-objects",))

    def test_snapshot_only_resume_runs_just_the_focused_owner(self) -> None:
        scripts = self.runner.load_scripts()
        baseline = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        snapshot_field = (
            self.runner.SNAPSHOT_CONTENT_PREFIX + "test:generic-specialization"
        )
        baseline["lowering-objects"][snapshot_field] = "old"
        current = {
            shard: dict(values) for shard, values in baseline.items()
        }
        current["lowering-objects"][snapshot_field] = "new"
        calls: list[tuple[str, tuple[str, ...] | None]] = []

        def fake_run_shard(
            shard,
            scripts,
            *,
            timing_report=None,
            stream=None,
            selected_commands=None,
        ):
            calls.append((shard, selected_commands))
            selected = (
                self.runner.SHARDS[shard]
                if selected_commands is None
                else selected_commands
            )
            report = self.successful_report(shard)
            report["commands"] = [
                command
                for command in report["commands"]
                if command["script"] in selected
            ]
            report["durationMs"] = len(report["commands"])
            self.runner.write_timing_report(timing_report, report)

        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            evidence_dir = root / "evidence"
            timing_dir = root / "timing"
            self.runner.prepare_evidence_directory(evidence_dir)
            for shard in self.runner.SHARD_ORDER:
                self.runner.write_timing_report(
                    evidence_dir / f"{shard}.json",
                    self.runner.evidence_record(shard, baseline[shard], now=100),
                )
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(current, current),
                ),
                mock.patch.object(
                    self.runner, "run_shard", side_effect=fake_run_shard
                ),
                mock.patch("sys.stdout", new=io.StringIO()),
                mock.patch.object(self.runner.time, "time", return_value=101),
            ):
                self.runner.run_all_shards(
                    scripts,
                    jobs=1,
                    resume=True,
                    evidence_dir=evidence_dir,
                    timing_dir=timing_dir,
                )
            summary = json.loads(
                (timing_dir / "toolchain-parallel-summary.json").read_text(
                    encoding="utf-8"
                )
            )
            refreshed = json.loads(
                (evidence_dir / "lowering-objects.json").read_text(
                    encoding="utf-8"
                )
            )

        self.assertEqual(
            calls,
            [
                (
                    "lowering-objects",
                    ("test:generic-specialization",),
                )
            ],
        )
        self.assertEqual(
            summary["snapshotOnlyReruns"],
            [
                {
                    "shard": "lowering-objects",
                    "scripts": ["test:generic-specialization"],
                    "priorEvidenceAgeSeconds": 1,
                }
            ],
        )
        self.assertEqual(
            refreshed["basis"],
            {
                "kind": "snapshot-refresh",
                "priorEvidenceKey": self.runner.evidence_key(
                    baseline["lowering-objects"]
                ),
                "scripts": ["test:generic-specialization"],
            },
        )

    def test_catalog_assigns_generic_snapshot_bytes_to_object_lowering(self) -> None:
        scripts = self.runner.load_scripts()
        outputs = self.runner.load_snapshot_outputs(scripts)
        generic = [
            output
            for output in outputs
            if output.root
            == Path("test/generic_specialization/expected/hxc.specializations.json")
        ]
        self.assertEqual(len(generic), 1)
        self.assertEqual(generic[0].owner_script, "test:generic-specialization")
        self.assertIn(generic[0].owner_script, self.runner.SHARDS["lowering-objects"])
        self.assertNotIn(generic[0].owner_script, self.runner.SHARDS["contracts"])

    def test_snapshot_content_digest_changes_only_for_the_selected_owner(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            expected = root / "test/sample/expected/value.json"
            expected.parent.mkdir(parents=True)
            expected.write_text('{"value":1}\n', encoding="utf-8")
            output = self.runner.SnapshotOutput(
                root=Path("test/sample/expected/value.json"),
                owner_script="test:sample",
                directory=False,
            )
            with mock.patch.object(self.runner, "ROOT", root):
                owner_before = self.runner.snapshot_content_digest(
                    (output,), ("test:sample",)
                )
                unrelated_before = self.runner.snapshot_content_digest(
                    (output,), ("test:other",)
                )
                expected.write_text('{"value":2}\n', encoding="utf-8")
                owner_after = self.runner.snapshot_content_digest(
                    (output,), ("test:sample",)
                )
                unrelated_after = self.runner.snapshot_content_digest(
                    (output,), ("test:other",)
                )

        self.assertNotEqual(owner_before, owner_after)
        self.assertEqual(unrelated_before, unrelated_after)

    def test_common_staged_identity_ignores_only_owned_snapshot_bytes(self) -> None:
        output = self.runner.SnapshotOutput(
            root=Path("test/sample/expected/value.json"),
            owner_script="test:sample",
            directory=False,
        )

        def identity(snapshot_blob: bytes, *, mode: bytes = b"100644") -> str:
            listing = (
                mode
                + b" "
                + snapshot_blob
                + b" 0\ttest/sample/expected/value.json\0"
                + b"100644 "
                + b"c" * 40
                + b" 0\tsrc/Compiler.hx\0"
            )
            with mock.patch.object(self.runner, "git_bytes", return_value=listing):
                return self.runner.staged_tree_identity((output,))

        self.assertEqual(identity(b"a" * 40), identity(b"b" * 40))
        self.assertNotEqual(identity(b"a" * 40), identity(b"a" * 40, mode=b"100755"))

        baseline = (
            b"100644 "
            + b"a" * 40
            + b" 0\ttest/sample/expected/value.json\0"
        )
        added = baseline + (
            b"100644 "
            + b"d" * 40
            + b" 0\ttest/sample/expected/added.json\0"
        )
        directory_output = self.runner.SnapshotOutput(
            root=Path("test/sample/expected"),
            owner_script="test:sample",
            directory=True,
        )
        with mock.patch.object(self.runner, "git_bytes", return_value=baseline):
            before = self.runner.staged_tree_identity((directory_output,))
        with mock.patch.object(self.runner, "git_bytes", return_value=added):
            after = self.runner.staged_tree_identity((directory_output,))
        self.assertNotEqual(before, after)

        def ordinary_identity(blob: bytes) -> str:
            listing = b"100644 " + blob + b" 0\tsrc/Compiler.hx\0"
            with mock.patch.object(self.runner, "git_bytes", return_value=listing):
                return self.runner.staged_tree_identity((output,))

        self.assertNotEqual(ordinary_identity(b"a" * 40), ordinary_identity(b"b" * 40))

    def test_pending_shards_remain_in_canonical_order(self) -> None:
        self.assertEqual(
            self.runner.pending_shards(("caxecraft", "contracts")),
            ("lowering-objects", "lowering-semantics"),
        )

    def test_success_report_must_cover_every_command_exactly(self) -> None:
        report = self.successful_report("contracts")
        validated = self.runner.validate_successful_shard_report(report, "contracts")
        self.assertIs(validated, report)
        missing = dict(report)
        missing["commands"] = report["commands"][:-1]
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "every expected command"
        ):
            self.runner.validate_successful_shard_report(missing, "contracts")
        failed = self.successful_report("contracts")
        failed["commands"][0]["exitCode"] = 9
        with self.assertRaisesRegex(
            self.runner.ToolchainShardFailure, "did not pass exactly"
        ):
            self.runner.validate_successful_shard_report(failed, "contracts")

    def test_evidence_directory_requires_ownership_and_rejects_symlinks(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary).resolve()
            owned = root / "owned"
            self.runner.prepare_evidence_directory(owned)
            marker = owned / self.runner.EVIDENCE_MARKER
            self.assertTrue(marker.is_file())

            unowned = root / "unowned"
            unowned.mkdir()
            (unowned / "keep.txt").write_text("user data", encoding="utf-8")
            with self.assertRaisesRegex(
                self.runner.ToolchainShardFailure, "no ownership marker"
            ):
                self.runner.prepare_evidence_directory(unowned)
            self.assertEqual(
                (unowned / "keep.txt").read_text(encoding="utf-8"), "user data"
            )

            target = root / "target"
            target.mkdir()
            link = root / "link"
            link.symlink_to(target, target_is_directory=True)
            with self.assertRaisesRegex(
                self.runner.ToolchainShardFailure, "symlink ancestor"
            ):
                self.runner.prepare_evidence_directory(link / "evidence")

    def test_effective_cpu_count_prefers_process_quota_then_affinity(self) -> None:
        with mock.patch.object(
            self.runner.os, "process_cpu_count", return_value=3, create=True
        ):
            self.assertEqual(self.runner.effective_cpu_count(), 3)
        with (
            mock.patch.object(
                self.runner.os, "process_cpu_count", return_value=None, create=True
            ),
            mock.patch.object(
                self.runner.os, "sched_getaffinity", return_value={0, 1}, create=True
            ),
        ):
            self.assertEqual(self.runner.effective_cpu_count(), 2)

    def test_evidence_tool_resolution_matches_npm_local_precedence(self) -> None:
        self.assertIn("cc", self.runner.EVIDENCE_TOOLS)
        with tempfile.TemporaryDirectory() as temporary:
            isolated_root = Path(temporary)
            expected = isolated_root / "node_modules/.bin/haxe"
            expected.parent.mkdir(parents=True)
            expected.write_text("local shim", encoding="utf-8")
            with (
                mock.patch.object(self.runner, "ROOT", isolated_root),
                mock.patch.object(
                    self.runner.shutil,
                    "which",
                    return_value="/unrelated/global/haxe",
                ),
            ):
                self.assertEqual(
                    self.runner.resolve_evidence_tool("haxe"),
                    str(expected),
                )
                self.runner.shutil.which.reset_mock()
                self.assertEqual(
                    self.runner.resolve_evidence_tool(
                        "missing-tool",
                        {"PATH": "/reviewed/tools"},
                    ),
                    "/unrelated/global/haxe",
                )
                self.runner.shutil.which.assert_called_once_with(
                    "missing-tool",
                    path="/reviewed/tools",
                )

    def test_lix_haxe_identity_hashes_real_compiler_and_standard_library(
        self,
    ) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            isolated_root = Path(temporary) / "checkout"
            isolated_root.mkdir()
            (isolated_root / ".haxerc").write_text(
                '{"version":"5.0.0-preview.1"}\n',
                encoding="utf-8",
            )
            scope = Path(temporary) / "scope"
            installation = scope / "versions/5.0.0-preview.1"
            compiler = installation / "haxe"
            standard = installation / "std"
            standard.mkdir(parents=True)
            compiler.write_bytes(b"compiler-one")
            (standard / "Std.hx").write_text(
                "class Std {}\n",
                encoding="utf-8",
            )
            environment = {"HAXE_ROOT": str(scope), "PATH": "/usr/bin:/bin"}
            with mock.patch.object(self.runner, "ROOT", isolated_root):
                baseline = self.runner.lix_haxe_installation_identity(
                    environment
                )
                compiler.write_bytes(b"compiler-two")
                compiler_changed = self.runner.lix_haxe_installation_identity(
                    environment
                )
                (standard / "Std.hx").write_text(
                    "class Std { static var changed = true; }\n",
                    encoding="utf-8",
                )
                standard_changed = self.runner.lix_haxe_installation_identity(
                    environment
                )
            self.assertNotEqual(
                baseline["compilerSha256"],
                compiler_changed["compilerSha256"],
            )
            self.assertNotEqual(
                compiler_changed["standardLibrarySha256"],
                standard_changed["standardLibrarySha256"],
            )
            override = Path(temporary) / "override-std"
            override.mkdir()
            (override / "Std.hx").write_text(
                "class Std { static var override = true; }\n",
                encoding="utf-8",
            )
            override_environment = {
                "HAXE_ROOT": str(scope),
                "HAXE_STD_PATH": str(override),
                "PATH": "/usr/bin:/bin",
            }
            with mock.patch.object(self.runner, "ROOT", isolated_root):
                override_identity = self.runner.lix_haxe_installation_identity(
                    override_environment
                )
                (standard / "Std.hx").write_text(
                    "class Std { static var changedAgain = true; }\n",
                    encoding="utf-8",
                )
                override_installation_standard_changed = (
                    self.runner.lix_haxe_installation_identity(
                        override_environment
                    )
                )
            self.assertEqual(
                override_identity["compilerSha256"],
                standard_changed["compilerSha256"],
            )
            self.assertEqual(
                override_identity["standardLibraryPath"],
                str(standard.resolve()),
            )
            self.assertNotEqual(
                override_identity["standardLibrarySha256"],
                override_installation_standard_changed["standardLibrarySha256"],
            )

            with mock.patch.object(self.runner, "ROOT", isolated_root):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure,
                    "cannot locate the Lix-pinned Haxe",
                ):
                    self.runner.lix_haxe_installation_identity(
                        {
                            "HAXE_ROOT": str(installation),
                            "PATH": "/usr/bin:/bin",
                        }
                    )

    def test_input_drift_during_execution_fails_without_writing_evidence(self) -> None:
        scripts = self.runner.load_scripts()
        initial = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        changed = {
            shard: self.sample_inputs(shard, "changed")
            for shard in self.runner.SHARD_ORDER
        }

        def fake_run_shard(
            shard,
            scripts,
            *,
            timing_report=None,
            stream=None,
            selected_commands=None,
        ):
            self.runner.write_timing_report(timing_report, self.successful_report(shard))

        with tempfile.TemporaryDirectory() as temporary:
            evidence_dir = Path(temporary).resolve() / "evidence"
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(initial, changed),
                ),
                mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure,
                    "inputs changed while the shards were running",
                ):
                    self.runner.run_all_shards(
                        scripts,
                        jobs=1,
                        resume=True,
                        evidence_dir=evidence_dir,
                    )
            self.assertTrue((evidence_dir / self.runner.EVIDENCE_MARKER).is_file())
            self.assertFalse(
                any((evidence_dir / f"{shard}.json").exists() for shard in self.runner.SHARD_ORDER)
            )

    def test_second_run_executes_only_the_previously_failed_shard(self) -> None:
        scripts = self.runner.load_scripts()
        inputs = {
            shard: self.sample_inputs(shard) for shard in self.runner.SHARD_ORDER
        }
        failed_shard = "lowering-semantics"
        attempts: list[tuple[int, str]] = []
        round_number = 1

        def fake_run_shard(
            shard,
            scripts,
            *,
            timing_report=None,
            stream=None,
            selected_commands=None,
        ):
            attempts.append((round_number, shard))
            if round_number == 1 and shard == failed_shard:
                report = self.successful_report(shard)
                report["outcome"] = "failed"
                report["commands"][-1]["outcome"] = "failed"
                report["commands"][-1]["exitCode"] = 7
                self.runner.write_timing_report(timing_report, report)
                raise self.runner.ToolchainShardFailure("intentional shard failure")
            self.runner.write_timing_report(timing_report, self.successful_report(shard))

        with tempfile.TemporaryDirectory() as temporary:
            evidence_dir = Path(temporary).resolve() / "evidence"
            timing_dir = Path(temporary).resolve() / "timings"
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(inputs, inputs),
                ),
                mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                with self.assertRaisesRegex(
                    self.runner.ToolchainShardFailure, "intentional shard failure"
                ):
                    self.runner.run_all_shards(
                        scripts,
                        jobs=4,
                        resume=True,
                        evidence_dir=evidence_dir,
                    )

            round_number = 2
            with (
                mock.patch.object(
                    self.runner,
                    "collect_evidence_inputs",
                    side_effect=(inputs, inputs),
                ),
                mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                self.runner.run_all_shards(
                    scripts,
                    jobs=4,
                    resume=True,
                    evidence_dir=evidence_dir,
                    timing_dir=timing_dir,
                )
            summary = json.loads(
                (timing_dir / "toolchain-parallel-summary.json").read_text(
                    encoding="utf-8"
                )
            )

        first_round = tuple(shard for attempt, shard in attempts if attempt == 1)
        self.assertEqual(len(first_round), len(self.runner.SHARD_ORDER))
        self.assertEqual(set(first_round), set(self.runner.SHARD_ORDER))
        self.assertEqual(
            tuple(shard for attempt, shard in attempts if attempt == 2),
            (failed_shard,),
        )
        self.assertEqual(summary["schemaVersion"], 3)
        self.assertEqual(
            [report["shard"] for report in summary["executedShards"]],
            [failed_shard],
        )
        self.assertEqual(
            [record["shard"] for record in summary["reusedShards"]],
            ["contracts", "lowering-objects", "caxecraft"],
        )

    def test_hook_keeps_local_smoke_and_hosted_exhaustive_routes_separate(
        self,
    ) -> None:
        hook = (ROOT / "scripts/hooks/pre-commit").read_text(encoding="utf-8")
        workflow = (ROOT / ".github/workflows/governance.yml").read_text(
            encoding="utf-8"
        )
        affected = (
            "[pre-commit] Running conservative local smoke; focused owners remain "
            "task evidence and complete cold coverage remains required in CI..."
        )
        self.assertIn(affected, hook)
        self.assertIn('"$PRE_COMMIT_ROUTE" = "affected"', hook)
        self.assertIn(
            "git diff --cached --name-only --diff-filter=ACMRD",
            hook,
        )
        self.assertIn('select_pre_commit_route.py" --smoke-owners', hook)
        self.assertIn(
            'npm run --silent test:local-gate -- --hook "$owner_script"',
            hook,
        )
        self.assertIn('npm run "$owner_script"', hook)
        self.assertIn(
            "npm run test:governance",
            hook,
        )
        for reusable_owner in (
            "test:all-sources",
            "test:hxc-ir",
            "test:project-emitter",
        ):
            with self.subTest(reusable_owner=reusable_owner):
                self.assertIn(
                    f"test:local-gate -- --hook {reusable_owner}",
                    hook,
                )
        self.assertIn('python3 "$ROOT_DIR/examples/hello/run.py"', hook)
        self.assertIn("npm run test:build-adapters:local", hook)
        self.assertIn("scripts/ci/select_pre_commit_route.py", hook)
        for uncached_check in (
            "Exporting Beads issues",
            "Formatting staged Haxe files",
            "Running local path guard",
            "Checking staged whitespace",
            "Running staged secret scan",
        ):
            with self.subTest(uncached_check=uncached_check):
                self.assertLess(hook.index(uncached_check), hook.index(affected))
        self.assertIn("npm run test:governance", hook)
        self.assertNotIn("npm run test:toolchain:parallel", hook)
        self.assertNotIn("parallel-exhaustive", hook)
        self.assertNotIn("\n  npm run test:native\n", hook)
        self.assertNotIn("--resume", workflow)
        self.assertIn("npm run test:toolchain:shard", workflow)
        self.assertIn("python3 scripts/ci/runtime_smoke.py", workflow)

    def test_native_lane_shares_the_global_worker_limit(self) -> None:
        scripts = self.runner.load_scripts()
        for jobs in (1, 2, 4):
            with self.subTest(jobs=jobs):
                active = 0
                maximum_active = 0
                native_calls = 0
                lock = threading.Lock()

                def enter_work() -> None:
                    nonlocal active, maximum_active
                    with lock:
                        active += 1
                        maximum_active = max(maximum_active, active)
                    time.sleep(0.02)
                    with lock:
                        active -= 1

                def fake_run_shard(
                    shard,
                    scripts,
                    *,
                    timing_report=None,
                    stream=None,
                    selected_commands=None,
                ):
                    enter_work()
                    self.runner.write_timing_report(
                        timing_report, self.successful_report(shard)
                    )

                def fake_subprocess_run(command, **kwargs):
                    nonlocal native_calls
                    self.assertEqual(
                        command, ["npm", "run", self.runner.NATIVE_SCRIPT]
                    )
                    native_calls += 1
                    enter_work()
                    return self.runner.subprocess.CompletedProcess(command, 0)

                with (
                    mock.patch.object(
                        self.runner, "run_shard", side_effect=fake_run_shard
                    ),
                    mock.patch.object(
                        self.runner.subprocess,
                        "run",
                        side_effect=fake_subprocess_run,
                    ),
                    mock.patch("sys.stdout", new=io.StringIO()),
                ):
                    self.runner.run_all_shards(
                        scripts,
                        jobs=jobs,
                        include_native=True,
                    )

                self.assertEqual(native_calls, 1)
                self.assertEqual(maximum_active, jobs)

    def test_native_lane_failure_is_attributed_without_hiding_shard_results(
        self,
    ) -> None:
        scripts = self.runner.load_scripts()

        def fake_run_shard(
            shard,
            scripts,
            *,
            timing_report=None,
            stream=None,
            selected_commands=None,
        ):
            self.runner.write_timing_report(
                timing_report, self.successful_report(shard)
            )

        with (
            mock.patch.object(self.runner, "run_shard", side_effect=fake_run_shard),
            mock.patch.object(
                self.runner.subprocess,
                "run",
                return_value=self.runner.subprocess.CompletedProcess(
                    ["npm", "run", self.runner.NATIVE_SCRIPT], 9
                ),
            ),
            mock.patch("sys.stdout", new=io.StringIO()),
        ):
            with self.assertRaisesRegex(
                self.runner.ToolchainShardFailure,
                "native: native lane stopped with exit 9",
            ):
                self.runner.run_all_shards(
                    scripts,
                    jobs=4,
                    include_native=True,
                )

    def test_native_lane_is_visible_in_parallel_timing_report(self) -> None:
        scripts = self.runner.load_scripts()

        def fake_run_shard(
            shard,
            scripts,
            *,
            timing_report=None,
            stream=None,
            selected_commands=None,
        ):
            self.runner.write_timing_report(
                timing_report, self.successful_report(shard)
            )

        with tempfile.TemporaryDirectory() as temporary:
            timing_dir = Path(temporary) / "timings"
            with (
                mock.patch.object(
                    self.runner, "run_shard", side_effect=fake_run_shard
                ),
                mock.patch.object(
                    self.runner.subprocess,
                    "run",
                    return_value=self.runner.subprocess.CompletedProcess(
                        ["npm", "run", self.runner.NATIVE_SCRIPT], 0
                    ),
                ),
                mock.patch("sys.stdout", new=io.StringIO()),
            ):
                self.runner.run_all_shards(
                    scripts,
                    jobs=4,
                    include_native=True,
                    timing_dir=timing_dir,
                )
            summary = json.loads(
                (timing_dir / "toolchain-parallel-summary.json").read_text(
                    encoding="utf-8"
                )
            )

        self.assertEqual(
            summary["auxiliaryLanes"],
            [
                {
                    "lane": "native",
                    "script": "test:native",
                    "outcome": "passed",
                    "exitCode": 0,
                    "durationMs": mock.ANY,
                }
            ],
        )


if __name__ == "__main__":
    unittest.main()
