#!/usr/bin/env python3
"""Run Haxe-authored Caxecraft tests on Eval and generated native C."""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import zipfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath

sys.path.insert(0, str(Path(__file__).resolve().parent))
from dev_haxe_server import (  # noqa: E402
    HaxeInstallation,
    HaxeServerFailure,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)


CASE_ROOT = Path(__file__).resolve().parent
ROOT = CASE_ROOT.parents[1]
STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-pedantic-errors",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
)
SANITIZER_FLAGS = (
    "-O1",
    "-g",
    "-fno-omit-frame-pointer",
    "-fsanitize=address,undefined",
)


@dataclass(frozen=True)
class GeneratedSourceCheck:
    """One split-output module and the readable C shapes it must preserve."""

    path: str
    required_markers: tuple[str, ...]
    forbidden_markers: tuple[str, ...]


@dataclass(frozen=True)
class HaxeCTestCase:
    """Closed host-side facts for one Haxe-authored native test."""

    case_id: str
    eval_hxml: str
    c_hxml: str
    native_harness: str
    generated_source: str
    required_source_markers: tuple[str, ...]
    forbidden_source_markers: tuple[str, ...]
    output_line_count: int
    success_line: str
    expected_runtime_features: tuple[str, ...] = ()
    required_runtime_roots: tuple[tuple[str, str, str], ...] = ()
    split_source_checks: tuple[GeneratedSourceCheck, ...] = ()
    runs_generated_main: bool = False
    embedded_source_path: str | None = None
    embedded_haxe_path: str | None = None
    embedded_source_functions: tuple[str, ...] = ()
    native_sources: tuple[str, ...] = ()
    native_include_directories: tuple[str, ...] = ()
    native_defines: tuple[str, ...] = ()
    haxe_defines: tuple[str, ...] = ()
    native_runs_from_case_root: bool = False
    eval_timeout_seconds: int = 30
    c_build_timeout_seconds: int = 60
    native_timeout_seconds: int = 20


@dataclass(frozen=True)
class EvalProbe:
    """One Haxe-owned assertion program and its small result envelope."""

    hxml: str
    expected_stdout: str


@dataclass(frozen=True)
class SourceAudit:
    """One source group and host-only dependency spellings it must exclude."""

    pattern: str
    forbidden_patterns: tuple[str, ...]


@dataclass(frozen=True)
class NegativeCompile:
    """One source-level type error whose useful Haxe diagnostic is required."""

    hxml: str
    required_fragments: tuple[str, ...]


@dataclass(frozen=True)
class EvalTestCase:
    """Host process checks around behavior assertions implemented in Haxe.

    The Haxe program decides whether the product behavior is correct. This
    record only states how to launch it, which tiny output envelope identifies
    success, and which target dependencies are forbidden at reusable source
    boundaries.
    """

    case_id: str
    probes: tuple[EvalProbe, ...]
    source_audits: tuple[SourceAudit, ...] = ()
    shim_direct: bool = False
    alternate_locale: bool = False
    cold_c_runs: int = 1
    warm_c_runs: int = 0
    negative_compile: NegativeCompile | None = None
    success_message: str = ""


TARGET_NEUTRAL_PATTERNS = (
    r"#if\b",
    r"\bDynamic\b",
    r"\bAny\b",
    r"\bReflect\b",
    r"\buntyped\b",
    r"\b__c__\b",
    r"\bc\.",
    r"\braylib\.",
)
SCENARIO_TARGET_NEUTRAL_PATTERNS = (
    r"#if\b",
    r"\bDynamic\b",
    r"\bAny\b",
    r"\bReflect\b",
    r"\buntyped\b",
    r"\b__c__\b",
    r"^import\s+c\.",
    r"^import\s+raylib\.",
)


EVAL_CASES = {
    "caxeflow": EvalTestCase(
        case_id="caxeflow",
        probes=(
            EvalProbe(
                "caxeflow.hxml",
                "caxeflow: 10 events, 12 predicates, 19 actions; "
                "stable order/repeat/defer/sequence/budgets; trace=-670871898\n",
            ),
        ),
        alternate_locale=True,
        success_message=(
            "caxeflow: OK: closed fixed-tick execution, reverse registration, "
            "C/{locale} locale determinism, and exact runtime budgets"
        ),
    ),
    "inventory": EvalTestCase(
        case_id="inventory",
        probes=(
            EvalProbe(
                "inventory.hxml",
                "caxecraft-inventory: 9 typed slots; selection, wrap, consume, "
                "lossless collect, empty, and full bounds passed\n",
            ),
        ),
        source_audits=(
            SourceAudit("src/caxecraft/gameplay/*.hx", TARGET_NEUTRAL_PATTERNS),
        ),
        alternate_locale=True,
        success_message=(
            "caxecraft-inventory: OK: bounded typed hotbar and C/{locale} determinism"
        ),
    ),
    "gameplay": EvalTestCase(
        case_id="gameplay",
        probes=(
            EvalProbe(
                "gameplay.hxml",
                "caxecraft-gameplay: lossless mining/items, paced Mossling encounter, "
                "berry recovery, and bounded player health passed\n",
            ),
            EvalProbe(
                "terrain-atlas.hxml",
                "caxecraft-terrain-atlas: two typed sheets, material faces, and "
                "inset UV bounds passed\n",
            ),
        ),
        source_audits=(
            SourceAudit("src/caxecraft/gameplay/*.hx", TARGET_NEUTRAL_PATTERNS),
            SourceAudit("src/caxecraft/app/TerrainAtlas.hx", TARGET_NEUTRAL_PATTERNS),
        ),
        alternate_locale=True,
        success_message=(
            "caxecraft-gameplay: OK: target-neutral actor, combat, drop, recovery, "
            "and health state under POSIX C and {locale} locales"
        ),
    ),
    "editor": EvalTestCase(
        case_id="editor",
        probes=(
            EvalProbe(
                "editor.hxml",
                "caxemap-editor: 23 command round trips, 50 protocol checks, "
                "19 focus checks, 18 navigation checks, 12 2D checks, 16 3D "
                "checks, 1890 canonical bytes; bounded history/test-play/recovery; "
                "trace=150575006\n",
            ),
        ),
        source_audits=(
            SourceAudit("src/caxecraft/editor/*.hx", TARGET_NEUTRAL_PATTERNS),
            SourceAudit("src/caxecraft/input/NavigationInput.hx", TARGET_NEUTRAL_PATTERNS),
            SourceAudit(
                "src/caxecraft/app/RaylibNavigationInput.hx",
                (
                    r"raylib\.raw\.",
                    r"IsGamepadButton(?:Down|Pressed)\([^,]+,\s*[0-9]",
                    r"GetGamepadAxisMovement\([^,]+,\s*[0-9]",
                ),
            ),
            SourceAudit("src/caxecraft/app/CaxecraftEditorScreen.hx", (r"Gamepad", r"Controller")),
        ),
        alternate_locale=True,
        success_message=(
            "caxemap-editor: OK: revisioned commands, atomic batches, copy-owned "
            "observations, undo/redo, validation, and test play agree under C/{locale}"
        ),
    ),
    "scenario-model": EvalTestCase(
        case_id="scenario-model",
        probes=(
            EvalProbe("scenario-model.hxml", "scenario-model: -1725217016\n"),
            EvalProbe(
                "scenario-codec.hxml",
                "scenario-codec: 1192 + 4027 + 14612 bytes, staged round-trip and "
                "exact malformed-input audit\n",
            ),
        ),
        source_audits=(
            SourceAudit(
                "src/caxecraft/scenario/*.hx",
                SCENARIO_TARGET_NEUTRAL_PATTERNS,
            ),
        ),
        negative_compile=NegativeCompile(
            "scenario-identity-negative.hxml",
            (
                "ScenarioIdentityMixup.hx:11",
                "caxecraft.scenario.ContentId should be caxecraft.scenario.ScenarioId",
            ),
        ),
        shim_direct=True,
        success_message=(
            "caxemap-model: OK: closed target-neutral model, canonical fixture, and nominal IDs"
        ),
    ),
    "scenario-determinism": EvalTestCase(
        case_id="scenario-determinism",
        probes=(
            EvalProbe(
                "scenario-codec.hxml",
                "scenario-codec: 1192 + 4027 + 14612 bytes, staged round-trip and "
                "exact malformed-input audit\n",
            ),
        ),
        alternate_locale=True,
        cold_c_runs=2,
        warm_c_runs=2,
        success_message=(
            "caxemap-determinism: OK: 3 cold Eval requests (C and {locale}) "
            "plus 2 requests through one pinned Haxe server"
        ),
    ),
}


CASES = {
    "runtime-piloscript": HaxeCTestCase(
        case_id="runtime-piloscript",
        eval_hxml="runtime-piloscript.hxml",
        c_hxml="runtime-piloscript-c.hxml",
        native_harness="test/native/runtime_piloscript_harness.c",
        generated_source="src/modules/caxecraft/pilot/RuntimePilotScript.c",
        required_source_markers=(
            "RuntimePilotScript_read",
            "RuntimePilotScript_observe",
            "RuntimePilotReadResult_RuntimePilotReady",
            "RuntimePilotReadResult_RuntimePilotRejected",
        ),
        forbidden_source_markers=("Dynamic", "Reflect", "goto "),
        output_line_count=1,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "string-scalar",
            "string",
            "bytes-string",
            "object",
            "gc",
            "string-split",
        ),
        runs_generated_main=True,
    ),
    "actor-composition": HaxeCTestCase(
        case_id="actor-composition",
        eval_hxml="actor-composition.hxml",
        c_hxml="actor-composition-c.hxml",
        native_harness="test/native/actor_composition_harness.c",
        generated_source="src/modules/caxecraft/content/ActorCompositionPlanner.c",
        required_source_markers=(
            "ActorCompositionPlanner_planActorComposition",
            "ActorCompositionResult_ActorCompositionPlanned",
            "ActorCompositionResult_ActorCompositionRejected",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=1,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "string-scalar",
            "string",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActorMechanicsValidation.c",
                required_markers=("ActorMechanicsValidation_isValidActorMechanics",),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActorPublication.c",
                required_markers=(
                    "ActorPublication_publishActorPlans",
                    "ActorPublicationResult_ActorsPublished",
                    "ActorPublicationResult_ActorPublicationRejected",
                ),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/qa/FocusedContentFixture.c",
                required_markers=(
                    "FocusedContentRegistry_resolveNpc",
                    "FocusedContentRegistry_resolveEnemy",
                    "ActorContentResolution_ActorContentResolved",
                ),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/domain/EntityStore.c",
                required_markers=("EntityStore_replaceOthers",),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/domain/GameSession.c",
                required_markers=(
                    "GameSession_replaceAuthoredActors",
                    "GameSession_stepAuthoredActorControllers",
                    "GameSession_stepCharacter",
                ),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/domain/ActorControllerScheduler.c",
                required_markers=(
                    "ActorControllerScheduler_startActorController",
                    "ActorControllerScheduler_planActorController",
                    "ActorControllerScheduler_planWanderChaseMelee",
                ),
                forbidden_markers=("goto ",),
            ),
        ),
    ),
    "app-screen": HaxeCTestCase(
        case_id="app-screen",
        eval_hxml="app-screen.hxml",
        c_hxml="app-screen-c.hxml",
        native_harness="test/native/app_screen_harness.c",
        generated_source="src/modules/caxecraft/app/AppScreen.c",
        required_source_markers=(
            "AppScreen_initialScreen",
            "AppScreen_startPlaying",
            "AppScreen_loseFocus",
            "AppScreen_togglePause",
            "AppScreen_recapture",
            "switch (hxc_l_screen)",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=1,
        success_line="0",
    ),
    "aquatics": HaxeCTestCase(
        case_id="aquatics",
        eval_hxml="aquatics.hxml",
        c_hxml="aquatics-c.hxml",
        native_harness="test/native/aquatics_harness.c",
        generated_source="src/modules/caxecraft/domain/GameSession.c",
        required_source_markers=("GameSession_tick", "GameSession *hxc_l_self"),
        forbidden_source_markers=("goto ",),
        output_line_count=2,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
        ),
        required_runtime_roots=(
            ("string-literal", "type-carrier", "runtime-representation"),
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="include/hxc/detail/program_types.h",
                required_markers=("#include <hxrt/string_literal.h>",),
                forbidden_markers=(),
            ),
        ),
    ),
    "presentation": HaxeCTestCase(
        case_id="presentation",
        eval_hxml="presentation.hxml",
        c_hxml="presentation-c.hxml",
        native_harness="test/native/presentation_harness.c",
        generated_source="src/modules/caxecraft/app/MotionInterpolation.c",
        required_source_markers=(
            "MotionInterpolation_start",
            "MotionInterpolation_advance",
            "MotionInterpolation_sample",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=1,
        success_line="0",
    ),
    "package-store": HaxeCTestCase(
        case_id="package-store",
        eval_hxml="package-store.hxml",
        c_hxml="package-store-c.hxml",
        native_harness="test/native/content_package_store_harness.c",
        generated_source="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
        required_source_markers=(
            "PosixPackageApi_openRoot",
            "PosixPackageApi_inspect",
            "PosixPackageApi_readExact",
            "openat(",
            "fstat(",
            "read(",
        ),
        forbidden_source_markers=(
            "caxecraft_package_posix_",
            "sys.io.File",
            "LoadFileData",
            "goto ",
        ),
        output_line_count=4,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "string-scalar",
            "string",
            "string-split",
        ),
        runs_generated_main=True,
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ContentPackageStore.c",
                required_markers=(
                    "ContentPackageStore_open",
                    "ContentPackageStore_read",
                ),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
        ),
        haxe_defines=(
            "caxecraft_package_store_testing",
            "caxecraft_posix_hosted",
        ),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
    ),
    "package-manifest": HaxeCTestCase(
        case_id="package-manifest",
        eval_hxml="package-manifest.hxml",
        c_hxml="package-manifest-c.hxml",
        native_harness="test/native/content_package_manifest_harness.c",
        generated_source="src/modules/caxecraft/content/ContentPackageManifest.c",
        required_source_markers=(
            "ContentPackageManifest_decodeContentPackageManifest",
            "ContentPackageManifest_loadContentPackage",
            "ContentPackageManifest_verifyContentPackage",
            "ContentPackageManifestReadResult_ContentPackageManifestReady",
            "ContentPackageLoadResult_ContentPackageReady",
        ),
        forbidden_source_markers=("haxe.Json", "Dynamic", "Reflect", "goto "),
        output_line_count=4,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "string-scalar",
            "string",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
                required_markers=("PosixPackageApi_openRoot", "PosixPackageApi_readExact", "openat(", "read("),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
        eval_timeout_seconds=120,
        native_timeout_seconds=120,
    ),
    "package-zip-source": HaxeCTestCase(
        case_id="package-zip-source",
        eval_hxml="package-zip-source.hxml",
        c_hxml="package-zip-source-c.hxml",
        native_harness="test/native/content_package_zip_source_harness.c",
        generated_source="src/modules/caxecraft/content/ContentPackageZipSource.c",
        required_source_markers=(
            "ContentPackageZipSource_open",
            "ContentPackageZipSource_buildIndex",
            "ContentPackageZipSource_validateLocal",
            "ContentPackageZipSource_crc32Of",
            "ContentPackageZipOpenResult_PackageZipOpened",
            "ContentPackageZipOpenResult_PackageZipRejected",
        ),
        forbidden_source_markers=("haxe.zip", "Dynamic", "Reflect", "goto "),
        output_line_count=4,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "string-scalar",
            "string",
            "bytes-string",
            "object",
            "gc",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
                required_markers=("PosixPackageApi_openRoot", "PosixPackageApi_readExact", "openat(", "read("),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
    ),
    "package-zip-export": HaxeCTestCase(
        case_id="package-zip-export",
        eval_hxml="package-zip-export.hxml",
        c_hxml="package-zip-export-c.hxml",
        native_harness="test/native/content_package_zip_export_harness.c",
        generated_source="src/modules/caxecraft/content/ContentPackageZipExport.c",
        required_source_markers=(
            "exportContentPackageZip",
            "ContentPackageZipExport_writeLocal",
            "ContentPackageZipExport_writeCentral",
            "ContentPackageZipExport_zipCrc32",
            "ContentPackageAssetClosure_verifyContentPackageAssetClosure",
        ),
        forbidden_source_markers=("haxe.zip", "Dynamic", "Reflect", "goto "),
        output_line_count=5,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "string-scalar",
            "string",
            "bytes-string",
            "object",
            "gc",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
                required_markers=("PosixPackageApi_openRoot", "PosixPackageApi_readExact", "openat(", "read("),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
        eval_timeout_seconds=120,
        # This correctness lane writes the complete runtime asset closure through
        # unoptimized generated C. Keep it bounded, but allow the 11-asset
        # flagship package to complete on a developer machine under load.
        native_timeout_seconds=240,
    ),
    "content-json": HaxeCTestCase(
        case_id="content-json",
        eval_hxml="content-json.hxml",
        c_hxml="content-json-c.hxml",
        native_harness="test/native/content_json_harness.c",
        generated_source="src/modules/caxecraft/content/ContentJson.c",
        required_source_markers=(
            "ContentJson_read",
            "ContentJson_parseValue",
            "ContentJsonReadResult_ContentJsonReady",
            "ContentJsonReadResult_ContentJsonRejected",
        ),
        forbidden_source_markers=(
            "haxe.Json",
            "Dynamic",
            "throw",
            "goto ",
        ),
        output_line_count=4,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "string-scalar",
            "string",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/text/Utf8Decoder.c",
                required_markers=(
                    "Utf8Decoder_decode",
                    "Utf8DecodeResult_Utf8Decoded",
                    "Utf8DecodeResult_Utf8Rejected",
                ),
                forbidden_markers=("goto ",),
            ),
        ),
        runs_generated_main=True,
        native_timeout_seconds=40,
    ),
    "runtime-schemas": HaxeCTestCase(
        case_id="runtime-schemas",
        eval_hxml="runtime-schemas.hxml",
        c_hxml="runtime-schemas-c.hxml",
        native_harness="test/native/runtime_schemas_harness.c",
        generated_source="src/modules/caxecraft/content/RuntimeContentPack.c",
        required_source_markers=(
            "RuntimeContentPack_decode",
            "RuntimeContentPackResult_RuntimeContentPackReady",
            "RuntimeContentPackResult_RuntimeContentPackRejected",
            "RuntimeContentRegistry_semanticProof",
        ),
        forbidden_source_markers=(
            "haxe.Json",
            "Dynamic",
            "Reflect",
            "throw",
            "goto ",
        ),
        output_line_count=4,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "string-scalar",
            "string",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/localization/RuntimeUiCatalog.c",
                required_markers=(
                    "RuntimeUiCatalog_decode",
                    "RuntimeUiCatalog_text",
                ),
                forbidden_markers=("haxe.Json", "Dynamic", "Reflect", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ContentPackageStore.c",
                required_markers=(
                    "ContentPackageStore_open",
                    "ContentPackageStore_read",
                ),
                forbidden_markers=("LoadFileData", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
        native_timeout_seconds=40,
    ),
    "runtime-content-generation": HaxeCTestCase(
        case_id="runtime-content-generation",
        eval_hxml="runtime-content-generation.hxml",
        c_hxml="runtime-content-generation-c.hxml",
        native_harness="test/native/runtime_content_generation_harness.c",
        generated_source="src/modules/caxecraft/content/RuntimeContentGeneration.c",
        required_source_markers=(
            "RuntimeContentGeneration_loadRuntimeContent",
            "RuntimeContentGeneration_rebuildRuntimeContentForPublicationTesting",
            "RuntimeContentLoadResult_RuntimeContentReady",
            "RuntimeContentLoadResult_RuntimeContentRejected",
        ),
        forbidden_source_markers=("BaseContentPack", "UiCatalog_text", "haxe.Json", "Dynamic", "Reflect", "goto "),
        output_line_count=6,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "int-map",
            "string-scalar",
            "string",
            "string-map",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActiveRuntimeContent.c",
                required_markers=(
                    "ActiveRuntimeContent_publish",
                    "RuntimeContentPublicationResult_RuntimeContentPublished",
                    "RuntimeContentPublicationResult_RuntimeContentPublicationRejected",
                ),
                forbidden_markers=("BaseContentRegistry", "UiCatalog_text", "Dynamic", "Reflect", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/RuntimeContentPack.c",
                required_markers=("RuntimeContentPack_decode", "RuntimeContentRegistry_semanticProof"),
                forbidden_markers=("haxe.Json", "Dynamic", "Reflect", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/localization/RuntimeUiCatalog.c",
                required_markers=("RuntimeUiCatalog_decode", "RuntimeUiCatalog_text"),
                forbidden_markers=("haxe.Json", "Dynamic", "Reflect", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/RuntimeLevelLoader.c",
                required_markers=(
                    "RuntimeLevelLoader_loadRuntimeLevel",
                    "RuntimeLevelLoader_rebuildRuntimeLevelForPublicationTesting",
                ),
                forbidden_markers=("BaseContentRegistry", "FirstPlayable", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
                required_markers=("PosixPackageApi_openRoot", "PosixPackageApi_readExact", "openat(", "read("),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
    ),
    "campaign-runtime": HaxeCTestCase(
        case_id="campaign-runtime",
        eval_hxml="campaign-runtime.hxml",
        c_hxml="campaign-runtime-c.hxml",
        native_harness="test/native/campaign_runtime_harness.c",
        generated_source="src/modules/caxecraft/content/CampaignRuntime.c",
        required_source_markers=(
            "CampaignRuntime_loadCampaignManifest",
            "CampaignRuntime_loadCampaignLevel",
            "CampaignLevelLoadResult_CampaignLevelReady",
            "CampaignLevelLoadResult_CampaignLevelRejected",
        ),
        forbidden_source_markers=("haxe.Json", "Dynamic", "Reflect", "goto "),
        output_line_count=6,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "string-scalar",
            "string",
            "bytes-string",
            "object",
            "gc",
            "int-map",
            "string-map",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/CampaignManifest.c",
                required_markers=("CampaignManifest_decodeCampaignManifest", "CampaignManifestDecoder_decode"),
                forbidden_markers=("haxe.Json", "Dynamic", "Reflect", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActiveContent.c",
                required_markers=("ActiveContent_publish", "ContentPublicationResult_ContentPublished"),
                forbidden_markers=("Dynamic", "Reflect", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/RuntimeLevelLoader.c",
                required_markers=("RuntimeLevelLoader_loadRuntimeLevel", "RuntimeLevelLoadResult_RuntimeLevelReady"),
                forbidden_markers=("BaseContentRegistry", "FirstPlayable", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
                required_markers=("PosixPackageApi_openRoot", "PosixPackageApi_readExact", "openat(", "read("),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
        native_timeout_seconds=40,
    ),
    "resolved-level-plan": HaxeCTestCase(
        case_id="resolved-level-plan",
        eval_hxml="resolved-level-plan.hxml",
        c_hxml="resolved-level-plan-c.hxml",
        native_harness="test/native/resolved_level_plan_harness.c",
        generated_source="src/modules/caxecraft/content/ResolvedLevelPlan.c",
        required_source_markers=(
            "ResolvedLevelPlan_resolve",
            "ResolvedLevelPlan_semanticTrace",
            "ResolvedLevelPlanResult_LevelPlanResolved",
            "ResolvedLevelPlanResult_LevelPlanRejected",
        ),
        forbidden_source_markers=("ScenarioLexer", "ScenarioParser", "goto "),
        output_line_count=15,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "int-map",
            "string-scalar",
            "string",
            "string-map",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/qa/FocusedContentFixture.c",
                required_markers=(
                    "FocusedContentRegistry_resolveTerrain",
                    "FocusedContentRegistry_resolveFluid",
                    "FocusedContentRegistry_resolveItem",
                ),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActorCompositionPlanner.c",
                required_markers=("ActorCompositionPlanner_planActorComposition",),
                forbidden_markers=("goto ",),
            ),
        ),
        runs_generated_main=True,
    ),
    "content-generation": HaxeCTestCase(
        case_id="content-generation",
        eval_hxml="content-generation.hxml",
        c_hxml="content-generation-c.hxml",
        native_harness="test/native/content_generation_harness.c",
        generated_source="src/modules/caxecraft/content/LoadedContentGeneration.c",
        required_source_markers=(
            "LoadedContentGeneration_build",
            "LoadedContentGeneration_buildInternal",
            "ContentGenerationBuildResult_ContentGenerationReady",
            "ContentGenerationBuildResult_ContentGenerationRejected",
        ),
        forbidden_source_markers=("FirstPlayableLevel", "goto "),
        output_line_count=4,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "int-map",
            "string-scalar",
            "string",
            "string-map",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActiveContent.c",
                required_markers=(
                    "ActiveContent_publish",
                    "ContentPublicationResult_ContentPublished",
                    "ContentPublicationResult_ContentPublicationRejected",
                ),
                forbidden_markers=("FirstPlayable", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/domain/GameSession.c",
                required_markers=(
                    "GameSession_writeTerrainRunDuringLoad",
                    "GameSession_bindLocalPlayer",
                    "GameSession_replaceAuthoredActors",
                ),
                forbidden_markers=("goto ",),
            ),
        ),
        runs_generated_main=False,
    ),
    "runtime-level-loader": HaxeCTestCase(
        case_id="runtime-level-loader",
        eval_hxml="runtime-level-loader.hxml",
        c_hxml="runtime-level-loader-c.hxml",
        native_harness="test/native/runtime_level_loader_harness.c",
        generated_source="src/modules/caxecraft/content/RuntimeLevelLoader.c",
        required_source_markers=(
            "RuntimeLevelLoader_loadRuntimeLevel",
            "RuntimeLevelLoader_loadRuntimeLevelInternal",
            "RuntimeLevelLoadResult_RuntimeLevelReady",
            "RuntimeLevelLoadResult_RuntimeLevelRejected",
        ),
        forbidden_source_markers=(
            "FirstPlayableLevel",
            "FirstPlayableSessionLoader",
            "LoadFileData",
            "goto ",
        ),
        output_line_count=8,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "int-map",
            "string-scalar",
            "string",
            "string-map",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ContentPackageStore.c",
                required_markers=(
                    "ContentPackageStore_open",
                    "ContentPackageStore_read",
                ),
                forbidden_markers=("LoadFileData", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/hosted/PosixPackageApi.c",
                required_markers=(
                    "PosixPackageApi_openRoot",
                    "PosixPackageApi_readExact",
                    "openat(",
                    "read(",
                ),
                forbidden_markers=("caxecraft_package_posix_", "goto "),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/scenario/ScenarioLexer.c",
                required_markers=("ScenarioLexer_read",),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ResolvedLevelPlan.c",
                required_markers=("ResolvedLevelPlan_resolve",),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActiveContent.c",
                required_markers=("ActiveContent_publish",),
                forbidden_markers=("FirstPlayable", "goto "),
            ),
        ),
        runs_generated_main=True,
        haxe_defines=("caxecraft_posix_hosted",),
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
        # The authored village now has more than one thousand compact terrain
        # runs. This complete parser/publication fault matrix is intentionally
        # broader than the focused landmark check. The wider physical world
        # also doubles its resolved terrain initialization. Keep both timeouts
        # bounded without treating this unoptimized diagnostic lane as a load
        # or generation-time budget.
        c_build_timeout_seconds=120,
        native_timeout_seconds=40,
    ),
    "scenario-native-codec": HaxeCTestCase(
        case_id="scenario-native-codec",
        eval_hxml="scenario-native-codec.hxml",
        c_hxml="scenario-native-codec-c.hxml",
        native_harness="test/native/scenario_native_codec_harness.c",
        generated_source="src/modules/caxecraft/scenario/ScenarioLexer.c",
        required_source_markers=(
            "ScenarioLexer_read",
            "ScenarioLexer_tokenize",
        ),
        forbidden_source_markers=("goto ",),
        output_line_count=10,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "string-literal",
            "bytes",
            "object",
            "gc",
            "int-map",
            "string-scalar",
            "string",
            "string-map",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/text/Utf8Decoder.c",
                required_markers=(
                    "Utf8Decoder_decode",
                    "Utf8DecodeResult_Utf8Decoded",
                    "Utf8DecodeResult_Utf8Rejected",
                ),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/scenario/ScenarioParser.c",
                required_markers=("ScenarioParser_parse",),
                forbidden_markers=("goto ",),
            ),
            GeneratedSourceCheck(
                path="src/modules/caxecraft/scenario/ScenarioValidator.c",
                required_markers=("ScenarioValidator_validate",),
                forbidden_markers=("goto ",),
            ),
        ),
        runs_generated_main=True,
        embedded_source_path="scenarios/first-playable/map.caxemap",
        embedded_haxe_path="test/caxecraft/qa/ScenarioNativeCodecProbe.hx",
        embedded_source_functions=(
            "firstPlayablePrefix",
            "firstPlayableSuffix",
        ),
    ),
    "water": HaxeCTestCase(
        case_id="water",
        eval_hxml="water.hxml",
        c_hxml="water-c.hxml",
        native_harness="test/native/water_harness.c",
        generated_source="src/modules/caxecraft/domain/WaterSimulation.c",
        required_source_markers=("WaterSimulation_tick",),
        forbidden_source_markers=("goto ",),
        output_line_count=2,
        success_line="0",
    ),
    "session": HaxeCTestCase(
        case_id="session",
        eval_hxml="session.hxml",
        c_hxml="session-c.hxml",
        native_harness="test/native/session_harness.c",
        generated_source="src/modules/caxecraft/domain/GameSession.c",
        required_source_markers=(
            "GameSession_writeTerrainRunDuringLoad",
            "GameSession_placeInitialWaterVolume",
            "GameSession_authoredItemIsActive",
            "GameSession_bindLocalPlayer",
            "GameSession_collectAuthoredAquaticEquipment",
            "GameSession_receiveLocalPlayerAttack",
            "GameSession_reviveLocalPlayerAt",
            "GameSession_useSelectedRecovery",
            "GameSession_view",
            "hxc_completedTicks",
            "hxc_tickIndex",
        ),
        forbidden_source_markers=(
            "goto ",
            "GameSession_replaceLocalPlayer",
            "GameSession_deactivateAuthoredItem",
        ),
        output_line_count=2,
        success_line="0",
        expected_runtime_features=(
            "runtime-base",
            "status",
            "alloc",
            "array",
            "object",
            "gc",
            "string-literal",
            "string-scalar",
            "string",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/ActorIdentityPlanner.c",
                required_markers=(
                    "ActorIdentityPlanner_actorEntityId",
                    "ActorIdentityPlanner_planActorIdentities",
                    "ActorIdentityPlanResult_Rejected",
                ),
                forbidden_markers=("goto ",),
            ),
        ),
        runs_generated_main=True,
    ),
    "terrain-chunks": HaxeCTestCase(
        case_id="terrain-chunks",
        eval_hxml="terrain-chunks.hxml",
        c_hxml="terrain-chunks-c.hxml",
        native_harness="test/native/terrain_chunks_harness.c",
        generated_source="src/modules/caxecraft/app/TerrainChunkCache.c",
        required_source_markers=(
            "TerrainChunkCache_prepare",
            "TerrainChunkCache_invalidate",
            "TerrainChunkCache_rebuild",
            "hxc_faceX",
            "hxc_packedFaces",
        ),
        forbidden_source_markers=("goto ", "malloc(", "calloc("),
        output_line_count=1,
        success_line="0",
    ),
}


class HaxeCTestFailure(RuntimeError):
    """The host could not prove a Haxe-authored test on both targets."""


def development_tool(name: str) -> str:
    """Resolve a checkout-owned tool before the host fallback."""

    local = ROOT / "node_modules/.bin" / name
    if local.is_file():
        return str(local)
    resolved = shutil.which(name)
    if resolved is None:
        raise HaxeCTestFailure(f"required tool {name!r} is unavailable")
    return resolved


def native_compiler(requested: str | None) -> str:
    """Resolve an explicit compiler or choose one fast local default."""

    resolved = shutil.which(requested) if requested is not None else (
        shutil.which("clang") or shutil.which("gcc") or shutil.which("cc")
    )
    if resolved is None:
        if requested is not None:
            raise HaxeCTestFailure(
                f"requested native C compiler {requested!r} is unavailable"
            )
        raise HaxeCTestFailure("no native C compiler is available")
    return resolved


def run(
    arguments: list[str],
    *,
    cwd: Path,
    timeout: int,
    label: str,
    environment: dict[str, str] | None = None,
) -> subprocess.CompletedProcess[str]:
    """Run one bounded process and preserve stdout and stderr on failure."""

    try:
        result = subprocess.run(
            arguments,
            cwd=cwd,
            env={
                **os.environ,
                "HAXE_NO_SERVER": "1",
                "LC_ALL": "C",
                **(environment or {}),
            },
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        raise HaxeCTestFailure(
            f"{label} failed with exit {result.returncode}\n"
            f"stdout:\n{result.stdout}stderr:\n{result.stderr}"
        )
    return result


def checked_case_path(spelling: str, label: str) -> Path:
    """Resolve one reviewed case-relative path without accepting traversal."""

    parsed = PurePosixPath(spelling)
    if parsed.is_absolute() or ".." in parsed.parts or "\\" in spelling:
        raise HaxeCTestFailure(f"{label} is not a safe case-relative path")
    resolved = CASE_ROOT.joinpath(*parsed.parts)
    if not resolved.is_file():
        raise HaxeCTestFailure(f"{label} does not exist: {spelling}")
    return resolved


def checked_relative_parts(spelling: str, label: str) -> tuple[str, ...]:
    """Validate an output-relative path before resolving generated evidence."""

    parsed = PurePosixPath(spelling)
    if parsed.is_absolute() or ".." in parsed.parts or "\\" in spelling:
        raise HaxeCTestFailure(f"{label} is not a safe relative path")
    return parsed.parts


def checked_case_directory(spelling: str, label: str) -> Path:
    """Resolve one reviewed case-relative directory without traversal."""

    parsed = PurePosixPath(spelling)
    if parsed.is_absolute() or ".." in parsed.parts or "\\" in spelling:
        raise HaxeCTestFailure(f"{label} is not a safe case-relative path")
    resolved = CASE_ROOT.joinpath(*parsed.parts)
    if not resolved.is_dir():
        raise HaxeCTestFailure(f"{label} does not exist: {spelling}")
    return resolved


def validate_oracle(test_case: HaxeCTestCase, output: str) -> None:
    """Validate only the shared test protocol; Haxe owns mechanic assertions."""

    lines = output.splitlines()
    if (
        len(lines) != test_case.output_line_count
        or not lines
        or lines[0] != test_case.success_line
        or not output.endswith("\n")
    ):
        raise HaxeCTestFailure(
            f"{test_case.case_id} Eval test emitted an invalid result envelope: {output!r}"
        )


def validate_package_zip_archive(archive: Path) -> None:
    """Inspect the transported bytes with independent standard ZIP tooling."""

    if not archive.is_file():
        raise HaxeCTestFailure("package ZIP Eval proof did not write its archive")
    raw = archive.read_bytes()
    if not raw:
        raise HaxeCTestFailure("canonical package ZIP is empty")

    manifest_path = CASE_ROOT / "caxecraft.package.json"
    manifest_bytes = manifest_path.read_bytes()
    manifest = json.loads(manifest_bytes)
    entries = manifest.get("entries") if isinstance(manifest, dict) else None
    if not isinstance(entries, list):
        raise HaxeCTestFailure("package manifest omitted its reviewed entry list")
    expected: dict[str, tuple[int, str] | bytes] = {
        "caxecraft.package.json": manifest_bytes
    }
    for entry in entries:
        if not isinstance(entry, dict):
            raise HaxeCTestFailure("package manifest entry is not an object")
        path = entry.get("path")
        byte_length = entry.get("byteLength")
        sha256 = entry.get("sha256")
        if (
            not isinstance(path, str)
            or not isinstance(byte_length, int)
            or isinstance(byte_length, bool)
            or not isinstance(sha256, str)
        ):
            raise HaxeCTestFailure("package manifest entry lost path/length/digest facts")
        if path in expected:
            raise HaxeCTestFailure(f"package manifest repeated {path!r}")
        expected[path] = (byte_length, sha256)

    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-zip-handoff-") as temporary:
        handoff = Path(temporary) / "first-adventure.zip"
        shutil.copy2(archive, handoff)
        try:
            with zipfile.ZipFile(handoff, "r") as package_zip:
                infos = package_zip.infolist()
                names = [info.filename for info in infos]
                if names != sorted(expected):
                    raise HaxeCTestFailure(
                        f"canonical package ZIP names/order differ: {names!r}"
                    )
                if package_zip.comment != b"" or package_zip.testzip() is not None:
                    raise HaxeCTestFailure(
                        "canonical package ZIP has a comment or failing CRC payload"
                    )
                for info in infos:
                    if (
                        info.compress_type != zipfile.ZIP_STORED
                        or info.date_time != (1980, 1, 1, 0, 0, 0)
                        or info.create_system != 3
                        or info.create_version != 30
                        or info.extract_version != 10
                        or info.flag_bits != 0
                        or info.external_attr != 0x81A40000
                        or info.internal_attr != 0
                        or info.extra != b""
                        or info.comment != b""
                    ):
                        raise HaxeCTestFailure(
                            f"canonical ZIP metadata drifted for {info.filename!r}"
                        )
                    data = package_zip.read(info)
                    expected_entry = expected[info.filename]
                    if isinstance(expected_entry, bytes):
                        if data != expected_entry:
                            raise HaxeCTestFailure(
                                "archive package manifest differs from authored bytes"
                            )
                    else:
                        byte_length, sha256 = expected_entry
                        if len(data) != byte_length or hashlib.sha256(data).hexdigest() != sha256:
                            raise HaxeCTestFailure(
                                f"archive payload receipt differs for {info.filename!r}"
                            )
        except zipfile.BadZipFile as error:
            raise HaxeCTestFailure(f"standard ZIP reader rejected handoff: {error}") from error

        unzip = shutil.which("unzip")
        if unzip is not None:
            run(
                [unzip, "-t", handoff.name],
                cwd=handoff.parent,
                timeout=30,
                label="independent Info-ZIP handoff check",
            )


def validate_package_zip_executable_handoff(
    executable: Path, archive: Path, oracle: str, temporary_root: Path, timeout: int
) -> None:
    """Run the already-built reader beside only the shared ZIP and no toolchain."""

    handoff = temporary_root / "isolated-package-handoff"
    handoff.mkdir()
    proof = handoff / "verify-package"
    shutil.copy2(executable, proof)
    shutil.copy2(archive, handoff / "handoff.zip")
    empty_path = handoff / "empty-path"
    empty_path.mkdir()
    output = run(
        [str(proof)],
        cwd=handoff,
        timeout=timeout,
        label="isolated package ZIP executable handoff",
        environment={"PATH": str(empty_path)},
    ).stdout
    if output != oracle:
        raise HaxeCTestFailure(
            "isolated package ZIP handoff changed the verified result envelope"
        )


def validate_embedded_source(test_case: HaxeCTestCase) -> None:
    """Require fixture-only String chunks to equal their authored byte source."""

    if test_case.embedded_source_path is None:
        if test_case.embedded_haxe_path is not None or test_case.embedded_source_functions:
            raise HaxeCTestFailure(
                f"{test_case.case_id} has an incomplete embedded-source contract"
            )
        return
    if (
        test_case.embedded_haxe_path is None
        or not test_case.embedded_source_functions
    ):
        raise HaxeCTestFailure(
            f"{test_case.case_id} has an incomplete embedded-source contract"
        )
    authored = checked_case_path(
        test_case.embedded_source_path, "embedded authored source"
    ).read_bytes()
    haxe_source = checked_case_path(
        test_case.embedded_haxe_path, "embedded Haxe source"
    ).read_text(encoding="utf-8")
    chunks: list[bytes] = []
    for function_name in test_case.embedded_source_functions:
        marker = f"function {function_name}():String\n\treturn "
        start = haxe_source.find(marker)
        if start < 0:
            raise HaxeCTestFailure(
                f"{test_case.case_id} omitted embedded source function "
                f"{function_name!r}"
            )
        literal_start = start + len(marker)
        literal_end = haxe_source.find(";\n", literal_start)
        if literal_end < 0:
            raise HaxeCTestFailure(
                f"{test_case.case_id} has an unterminated embedded source "
                f"function {function_name!r}"
            )
        literal = haxe_source[literal_start:literal_end]
        decoded = json.loads(literal)
        if not isinstance(decoded, str):
            raise HaxeCTestFailure(
                f"{test_case.case_id} embedded source function "
                f"{function_name!r} is not one String literal"
            )
        chunks.append(decoded.encode("utf-8"))
    embedded = b"".join(chunks)
    if embedded != authored:
        raise HaxeCTestFailure(
            f"{test_case.case_id} embedded {len(embedded)} bytes but "
            f"{test_case.embedded_source_path} contains {len(authored)} bytes"
        )


def validate_resolved_level_privacy(test_case: HaxeCTestCase) -> None:
    """Keep the engine plan out of authoring, editor, and package-ingress code."""

    if test_case.case_id != "resolved-level-plan":
        return
    restricted_roots = (
        CASE_ROOT / "src/caxecraft/scenario",
        CASE_ROOT / "src/caxecraft/editor",
        CASE_ROOT / "toolsrc",
    )
    restricted_files = tuple(
        sorted(
            path
            for root in restricted_roots
            for path in root.rglob("*.hx")
            if path.is_file()
        )
    ) + tuple(
        sorted(
            path
            for path in (CASE_ROOT / "src/caxecraft/content").glob(
                "ContentPackage*.hx"
            )
            if path.is_file()
        )
    )
    violations = [
        path.relative_to(CASE_ROOT).as_posix()
        for path in restricted_files
        if "ResolvedLevelPlan" in path.read_text(encoding="utf-8")
    ]
    if violations:
        raise HaxeCTestFailure(
            "private resolved level plan leaked into authoring/editor/package code: "
            + ", ".join(violations)
        )


def validate_source_audits(test_case: EvalTestCase) -> None:
    """Keep reusable Haxe semantics free of target-specific dependencies."""

    for audit in test_case.source_audits:
        sources = sorted(
            (path for path in CASE_ROOT.glob(audit.pattern) if path.is_file()),
            key=lambda path: path.as_posix().encode("utf-8"),
        )
        if not sources:
            raise HaxeCTestFailure(
                f"{test_case.case_id} source audit matched no files: {audit.pattern}"
            )
        compiled_patterns = tuple(
            re.compile(pattern, re.MULTILINE) for pattern in audit.forbidden_patterns
        )
        for source in sources:
            text = source.read_text(encoding="utf-8")
            for pattern in compiled_patterns:
                if pattern.search(text):
                    raise HaxeCTestFailure(
                        f"{source.relative_to(ROOT)} crosses the target-neutral "
                        f"boundary: {pattern.pattern}"
                    )


def run_eval_probe(
    test_case: EvalTestCase,
    probe: EvalProbe,
    installation: HaxeInstallation,
    arguments: tuple[str, ...],
    *,
    locale: str,
    connection: str | None = None,
) -> str:
    """Launch one Haxe assertion program and check only its result envelope."""

    command = [str(installation.compiler)]
    if connection is not None:
        command.extend(("--connect", connection))
    command.extend(arguments)
    try:
        result = subprocess.run(
            command,
            cwd=CASE_ROOT,
            env=pinned_haxe_environment(locale, installation),
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=90,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(
            f"{test_case.case_id} {probe.hxml} could not run: {error}"
        ) from error
    if (
        result.returncode != 0
        or result.stdout != probe.expected_stdout
        or result.stderr
    ):
        raise HaxeCTestFailure(
            f"{test_case.case_id} {probe.hxml} changed under {locale}:\n"
            f"exit: {result.returncode}\n"
            f"expected stdout: {probe.expected_stdout!r}\n"
            f"actual stdout: {result.stdout!r}\n"
            f"actual stderr: {result.stderr!r}"
        )
    return result.stdout


def run_shim_eval_probe(test_case: EvalTestCase, probe: EvalProbe) -> str:
    """Run a cold Eval case through the checkout's ordinary Haxe shim."""

    try:
        result = subprocess.run(
            [development_tool("haxe"), "--cwd", str(CASE_ROOT), probe.hxml],
            cwd=ROOT,
            env={**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=90,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(
            f"{test_case.case_id} {probe.hxml} could not run: {error}"
        ) from error
    if (
        result.returncode != 0
        or result.stdout != probe.expected_stdout
        or result.stderr
    ):
        raise HaxeCTestFailure(
            f"{test_case.case_id} {probe.hxml} changed through the Haxe shim:\n"
            f"exit: {result.returncode}\n"
            f"expected stdout: {probe.expected_stdout!r}\n"
            f"actual stdout: {result.stdout!r}\n"
            f"actual stderr: {result.stderr!r}"
        )
    return result.stdout


def validate_negative_compile(
    test_case: EvalTestCase, installation: HaxeInstallation | None
) -> None:
    """Require one reviewed source-type failure and its useful diagnostic."""

    negative = test_case.negative_compile
    if negative is None:
        return
    if installation is None:
        command = [
            development_tool("haxe"),
            "--cwd",
            str(CASE_ROOT),
            negative.hxml,
        ]
        environment = {**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C"}
    else:
        arguments = resolve_haxe_arguments((negative.hxml,), locale="C")
        command = [str(installation.compiler), *arguments]
        environment = pinned_haxe_environment("C", installation)
    try:
        result = subprocess.run(
            command,
            cwd=ROOT if installation is None else CASE_ROOT,
            env=environment,
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(
            f"{test_case.case_id} negative compile could not run: {error}"
        ) from error
    combined = result.stdout + result.stderr
    if result.returncode == 0:
        raise HaxeCTestFailure(
            f"{test_case.case_id} negative compile unexpectedly succeeded"
        )
    missing = [
        fragment for fragment in negative.required_fragments if fragment not in combined
    ]
    if missing:
        raise HaxeCTestFailure(
            f"{test_case.case_id} negative compile lost diagnostic fragments: "
            + ", ".join(missing)
            + "\n"
            + combined
        )


def execute_eval_case(test_case: EvalTestCase) -> str:
    """Run Haxe-owned assertions through the requested host process profiles."""

    validate_source_audits(test_case)
    if test_case.shim_direct:
        if test_case.alternate_locale or test_case.warm_c_runs:
            raise HaxeCTestFailure(
                f"{test_case.case_id} cannot combine direct HaxeShim and locale/server profiles"
            )
        validate_negative_compile(test_case, None)
        for probe in test_case.probes:
            traces = [
                run_shim_eval_probe(test_case, probe)
                for _ in range(test_case.cold_c_runs)
            ]
            if any(trace != traces[0] for trace in traces[1:]):
                raise HaxeCTestFailure(
                    f"{test_case.case_id} repeated traces differ for {probe.hxml}"
                )
        return "C"
    # Locale and warm-server cases borrow two helpers from the aggregate domain
    # runner. Keep that import lazy so native and direct-shim cases do not load
    # unrelated asset, snapshot, and playable-game machinery.
    from run import alternate_locale, haxe_compilation_server

    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    validate_negative_compile(test_case, installation)
    locale = alternate_locale() if test_case.alternate_locale else "C"
    if test_case.alternate_locale and locale == "C":
        raise HaxeCTestFailure(
            f"{test_case.case_id} requires an installed alternate process locale"
        )

    reference_outputs: dict[str, str] = {}
    c_arguments_by_hxml: dict[str, tuple[str, ...]] = {}
    for probe in test_case.probes:
        c_arguments = resolve_haxe_arguments((probe.hxml,), locale="C")
        c_arguments_by_hxml[probe.hxml] = c_arguments
        traces = [
            run_eval_probe(
                test_case,
                probe,
                installation,
                c_arguments,
                locale="C",
            )
            for _ in range(test_case.cold_c_runs)
        ]
        if test_case.alternate_locale:
            locale_arguments = resolve_haxe_arguments((probe.hxml,), locale=locale)
            traces.append(
                run_eval_probe(
                    test_case,
                    probe,
                    installation,
                    locale_arguments,
                    locale=locale,
                )
            )
        if traces:
            reference_outputs[probe.hxml] = traces[0]
            if any(trace != traces[0] for trace in traces[1:]):
                raise HaxeCTestFailure(
                    f"{test_case.case_id} cold/locale traces differ for {probe.hxml}"
                )

    if test_case.warm_c_runs:
        with haxe_compilation_server() as connection:
            if connection.installation != installation:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} warm server uses a different Haxe installation"
                )
            for probe in test_case.probes:
                warm_arguments = c_arguments_by_hxml[probe.hxml]
                warm = [
                    run_eval_probe(
                        test_case,
                        probe,
                        installation,
                        warm_arguments,
                        locale="C",
                        connection=connection.endpoint,
                    )
                    for _ in range(test_case.warm_c_runs)
                ]
                if any(
                    trace != reference_outputs[probe.hxml] for trace in warm
                ):
                    raise HaxeCTestFailure(
                        f"{test_case.case_id} warm/cold traces differ for {probe.hxml}"
                    )
    return locale


def sanitizer_supported(compiler: str, root: Path) -> bool:
    """Ask whether the selected compiler can link Address/Undefined sanitizers."""

    source = root / "sanitizer-probe.c"
    executable = root / "sanitizer-probe"
    source.write_text("int main(void) { return 0; }\n", encoding="utf-8", newline="\n")
    try:
        result = subprocess.run(
            [compiler, *SANITIZER_FLAGS, str(source), "-o", str(executable)],
            cwd=ROOT,
            env={**os.environ, "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            timeout=30,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise HaxeCTestFailure(f"sanitizer capability probe could not run: {error}") from error
    return result.returncode == 0


def generated_sources(root: Path) -> list[Path]:
    """Return generated modules while leaving compiler-owned main to the harness."""

    return sorted(
        (
            path
            for path in root.rglob("*.c")
            if path.relative_to(root).as_posix() != "src/hxc/main.c"
        ),
        key=lambda path: path.as_posix().encode("utf-8"),
    )


def compile_native(
    compiler: str,
    generated: Path,
    sources: list[Path],
    harness: Path,
    executable: Path,
    *,
    layout: str,
    sanitized: bool,
    runs_generated_main: bool,
    support_sources: tuple[Path, ...],
    support_include_roots: tuple[Path, ...],
    native_defines: tuple[str, ...],
) -> None:
    """Compile one generated test plus its independent native ABI consumer."""

    flags = [*STRICT_FLAGS, *(SANITIZER_FLAGS if sanitized else ())]
    include_roots = [generated / "include", *support_include_roots]
    runtime_include = generated / "runtime/include"
    if runtime_include.is_dir():
        include_roots.append(runtime_include)
    include_flags = [
        argument
        for include_root in include_roots
        for argument in ("-I", str(include_root))
    ]
    define_flags = [f"-D{define}" for define in native_defines]
    if layout == "unity":
        objects: list[Path] = []
        for index, source in enumerate(sources):
            generated_object = executable.parent / f"generated-{index}.o"
            is_unity_program = (
                source.relative_to(generated).as_posix() == "src/program.c"
            )
            if is_unity_program:
                # Strictly check the untouched unity source first. Renaming its
                # standard `main` below is only how this independent harness
                # avoids a second entry point; Clang no longer exempts the
                # renamed function from -Wmissing-prototypes.
                run(
                    [
                        compiler,
                        *flags,
                        *define_flags,
                        *include_flags,
                        "-fsyntax-only",
                        str(source),
                    ],
                    cwd=ROOT,
                    timeout=60,
                    label=f"{'sanitized ' if sanitized else ''}strict unity source check",
                )
            run(
                [
                    compiler,
                    *flags,
                    *define_flags,
                    *include_flags,
                    *(
                        ("-Dmain=hxc_generated_main", "-Wno-missing-prototypes")
                        if is_unity_program
                        else ()
                    ),
                    "-c",
                    str(source),
                    "-o",
                    str(generated_object),
                ],
                cwd=ROOT,
                timeout=60,
                label=f"{'sanitized ' if sanitized else ''}unity generated object",
            )
            objects.append(generated_object)
        for index, source in enumerate(support_sources):
            support_object = executable.parent / f"support-{index}.o"
            run(
                [
                    compiler,
                    *flags,
                    *define_flags,
                    *include_flags,
                    "-c",
                    str(source),
                    "-o",
                    str(support_object),
                ],
                cwd=ROOT,
                timeout=60,
                label=f"{'sanitized ' if sanitized else ''}unity support object",
            )
            objects.append(support_object)
        harness_object = executable.parent / "native-harness.o"
        run(
            [
                compiler,
                *flags,
                *define_flags,
                *include_flags,
                "-c",
                str(harness),
                "-o",
                str(harness_object),
            ],
            cwd=ROOT,
            timeout=60,
            label=f"{'sanitized ' if sanitized else ''}unity native harness object",
        )
        run(
            [
                compiler,
                *flags,
                *(str(path) for path in objects),
                str(harness_object),
                "-o",
                str(executable),
            ],
            cwd=ROOT,
            timeout=60,
            label=f"{'sanitized ' if sanitized else ''}unity native link",
        )
        return
    generated_main_object: Path | None = None
    if runs_generated_main:
        generated_main = generated / "src/hxc/main.c"
        if not generated_main.is_file():
            raise HaxeCTestFailure(
                "generated-main lifecycle test omitted src/hxc/main.c"
            )
        generated_main_object = executable.parent / "generated-main.o"
        run(
            [
                compiler,
                *flags,
                *include_flags,
                "-Dmain=hxc_generated_main",
                "-Wno-missing-prototypes",
                "-c",
                str(generated_main),
                "-o",
                str(generated_main_object),
            ],
            cwd=ROOT,
            timeout=60,
            label=f"{'sanitized ' if sanitized else ''}generated main object",
        )
    run(
        [
            compiler,
            *flags,
            *define_flags,
            *include_flags,
            *(str(path) for path in sources),
            *(str(path) for path in support_sources),
            str(harness),
            *((str(generated_main_object),) if generated_main_object else ()),
            "-o",
            str(executable),
        ],
        cwd=ROOT,
        timeout=60,
        label=f"{'sanitized ' if sanitized else ''}strict native build",
    )


def execute(
    test_case: HaxeCTestCase, requested_compiler: str | None, layout: str
) -> bool:
    """Run one Haxe test on Eval, one C layout, strict C, and sanitizers."""

    validate_embedded_source(test_case)
    validate_resolved_level_privacy(test_case)
    eval_hxml = checked_case_path(test_case.eval_hxml, "Eval HXML")
    c_hxml = checked_case_path(test_case.c_hxml, "C HXML")
    harness = checked_case_path(test_case.native_harness, "native harness")
    support_sources = tuple(
        checked_case_path(path, "native support source")
        for path in test_case.native_sources
    )
    support_include_roots = tuple(
        checked_case_directory(path, "native support include directory")
        for path in test_case.native_include_directories
    )
    compiler = native_compiler(requested_compiler)
    with tempfile.TemporaryDirectory(prefix=f"hxc-caxecraft-{test_case.case_id}-") as temporary:
        temporary_root = Path(temporary)
        eval_archive = (
            temporary_root / "canonical-first-adventure.zip"
            if test_case.case_id == "package-zip-export"
            else None
        )
        oracle = run(
            [development_tool("haxe"), "--cwd", str(CASE_ROOT), eval_hxml.name],
            cwd=ROOT,
            timeout=test_case.eval_timeout_seconds,
            label=f"{test_case.case_id} Eval test",
            environment=(
                {"CAXECRAFT_ZIP_EXPORT_PATH": str(eval_archive)}
                if eval_archive is not None
                else None
            ),
        ).stdout
        validate_oracle(test_case, oracle)
        if eval_archive is not None:
            validate_package_zip_archive(eval_archive)

        generated = temporary_root / "generated"
        run(
            [
                development_tool("haxe"),
                "--cwd",
                str(CASE_ROOT),
                c_hxml.name,
                *(
                    argument
                    for define in (
                        *test_case.haxe_defines,
                        *(("caxecraft_posix_darwin",) if sys.platform == "darwin" else ()),
                    )
                    for argument in ("-D", define)
                ),
                "-D",
                f"hxc_project_layout={layout}",
                "--custom-target",
                f"c={generated}",
            ],
            cwd=ROOT,
            timeout=test_case.c_build_timeout_seconds,
            label=f"{test_case.case_id} Haxe-to-C build",
        )
        runtime_plan = json.loads(
            (generated / "hxc.runtime-plan.json").read_text(encoding="utf-8")
        )
        if not isinstance(runtime_plan, dict):
            raise HaxeCTestFailure(
                f"{test_case.case_id} runtime plan is not a JSON object"
            )
        expected_runtime_features = list(test_case.expected_runtime_features)
        if runtime_plan.get("features") != expected_runtime_features:
            raise HaxeCTestFailure(
                f"{test_case.case_id} selected an unexpected hxrt feature closure: "
                f"expected={expected_runtime_features!r}, "
                f"actual={runtime_plan.get('features')!r}"
            )
        actual_runtime_roots = {
            (
                root.get("featureId"),
                root.get("operationId"),
                root.get("kind"),
            )
            for root in runtime_plan.get("rootReasons", [])
            if isinstance(root, dict)
        }
        missing_runtime_roots = set(test_case.required_runtime_roots).difference(
            actual_runtime_roots
        )
        if missing_runtime_roots:
            raise HaxeCTestFailure(
                f"{test_case.case_id} omitted required runtime root reasons: "
                f"{sorted(missing_runtime_roots)!r}"
            )

        if layout == "split":
            generated_source = generated.joinpath(
                *checked_relative_parts(
                    test_case.generated_source, "generated source path"
                )
            )
            if not generated_source.is_file():
                raise HaxeCTestFailure(
                    f"{test_case.case_id} omitted {test_case.generated_source}"
                )
            for source_check in test_case.split_source_checks:
                checked_path = generated.joinpath(
                    *checked_relative_parts(
                        source_check.path, "checked generated source path"
                    )
                )
                if not checked_path.is_file():
                    raise HaxeCTestFailure(
                        f"{test_case.case_id} omitted {source_check.path}"
                    )
                checked_text = checked_path.read_text(encoding="utf-8")
                for marker in source_check.required_markers:
                    if marker not in checked_text:
                        raise HaxeCTestFailure(
                            f"{test_case.case_id} {source_check.path} omitted "
                            f"{marker!r}"
                        )
                for marker in source_check.forbidden_markers:
                    if marker in checked_text:
                        raise HaxeCTestFailure(
                            f"{test_case.case_id} {source_check.path} retained "
                            f"{marker!r}"
                        )
            source_text = generated_source.read_text(encoding="utf-8")
        else:
            layout_sources = generated_sources(generated)
            if not layout_sources:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} {layout} layout emitted no C sources"
                )
            source_text = "\n".join(
                source.read_text(encoding="utf-8") for source in layout_sources
            )
        for marker in test_case.required_source_markers:
            if marker not in source_text:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} generated source omitted {marker!r}"
                )
        for marker in test_case.forbidden_source_markers:
            if marker in source_text:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} generated source retained {marker!r}"
                )

        sources = generated_sources(generated)
        executable = temporary_root / test_case.case_id
        compile_native(
            compiler,
            generated,
            sources,
            harness,
            executable,
            layout=layout,
            sanitized=False,
            runs_generated_main=test_case.runs_generated_main,
            support_sources=support_sources,
            support_include_roots=support_include_roots,
            native_defines=test_case.native_defines,
        )
        native_output = run(
            [str(executable)],
            cwd=CASE_ROOT if test_case.native_runs_from_case_root else ROOT,
            timeout=test_case.native_timeout_seconds,
            label=f"{test_case.case_id} native test",
        ).stdout
        if native_output != oracle:
            raise HaxeCTestFailure(
                f"{test_case.case_id} Eval/native results differ\n"
                f"Eval={oracle!r}\nnative={native_output!r}"
            )
        if eval_archive is not None:
            validate_package_zip_executable_handoff(
                executable,
                eval_archive,
                oracle,
                temporary_root,
                test_case.native_timeout_seconds,
            )

        sanitizer_ran = sanitizer_supported(compiler, temporary_root)
        if sanitizer_ran:
            sanitized = temporary_root / f"{test_case.case_id}-sanitized"
            compile_native(
                compiler,
                generated,
                sources,
                harness,
                sanitized,
                layout=layout,
                sanitized=True,
                runs_generated_main=test_case.runs_generated_main,
                support_sources=support_sources,
                support_include_roots=support_include_roots,
                native_defines=test_case.native_defines,
            )
            sanitized_output = run(
                [str(sanitized)],
                cwd=CASE_ROOT if test_case.native_runs_from_case_root else ROOT,
                timeout=test_case.native_timeout_seconds,
                label=f"{test_case.case_id} sanitized native test",
                environment={
                    "ASAN_OPTIONS": "halt_on_error=1:abort_on_error=1",
                    "UBSAN_OPTIONS": "halt_on_error=1:print_stacktrace=1",
                },
            ).stdout
            if sanitized_output != oracle:
                raise HaxeCTestFailure(
                    f"{test_case.case_id} sanitizer run changed the test result"
                )
        return sanitizer_ran


def create_transient_fixture(test_case: HaxeCTestCase) -> Path | None:
    """Create host-shaped evidence that must not become a tracked cache input."""

    if test_case.case_id != "package-store":
        return None
    link = CASE_ROOT / "test/fixtures/package-store/root/escape.bin"
    if link.exists() or link.is_symlink():
        raise HaxeCTestFailure(
            "package-store symlink fixture already exists before the isolated run"
        )
    link.symlink_to("../outside.bin")
    return link


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("case", choices=tuple(sorted((*CASES, *EVAL_CASES))))
    parser.add_argument(
        "--cc",
        help="explicit C compiler command or path; the default keeps the focused local lane fast",
    )
    parser.add_argument(
        "--layout",
        choices=("split", "package", "unity"),
        default="split",
        help="generated project layout; package/unity are optional focused parity checks",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.case in EVAL_CASES:
        test_case = EVAL_CASES[args.case]
        try:
            locale = execute_eval_case(test_case)
        except (
            HaxeCTestFailure,
            HaxeServerFailure,
            RuntimeError,
            OSError,
            UnicodeError,
            json.JSONDecodeError,
        ) as error:
            print(
                f"caxecraft-haxe-test: ERROR [{test_case.case_id}]: {error}",
                file=sys.stderr,
            )
            return 1
        print(test_case.success_message.format(locale=locale))
        return 0

    test_case = CASES[args.case]
    transient_fixture: Path | None = None
    try:
        transient_fixture = create_transient_fixture(test_case)
        sanitizer_ran = execute(test_case, args.cc, args.layout)
    except (HaxeCTestFailure, OSError, UnicodeError, json.JSONDecodeError) as error:
        print(
            f"caxecraft-haxe-c-test: ERROR [{test_case.case_id}]: {error}",
            file=sys.stderr,
        )
        return 1
    finally:
        if transient_fixture is not None:
            transient_fixture.unlink(missing_ok=True)
    sanitizer_status = "ran" if sanitizer_ran else "unavailable"
    runtime_status = (
        "+".join(test_case.expected_runtime_features)
        if test_case.expected_runtime_features
        else "none"
    )
    print(
        f"caxecraft-haxe-c-test: OK [{test_case.case_id}/{args.layout}]: "
        f"Haxe assertions passed on Eval/native C; runtime={runtime_status}; "
        f"sanitizers={sanitizer_status}"
        + (
            "; canonical-zip-handoff=standard-reader+Info-ZIP-if-available+isolated-native"
            if test_case.case_id == "package-zip-export"
            else ""
        )
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
