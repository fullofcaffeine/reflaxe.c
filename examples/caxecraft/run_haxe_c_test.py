#!/usr/bin/env python3
"""Run Haxe-authored Caxecraft tests on Eval and generated native C."""

from __future__ import annotations

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path, PurePosixPath


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


CASES = {
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
                path="src/modules/caxecraft/content/BaseContentPack.c",
                required_markers=(
                    "BaseContentRegistry_resolveNpc",
                    "BaseContentRegistry_resolveEnemy",
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
            "string-scalar",
            "string",
            "string-split",
        ),
        split_source_checks=(
            GeneratedSourceCheck(
                path="src/modules/caxecraft/content/BaseContentPack.c",
                required_markers=(
                    "BaseContentRegistry_resolveTerrain",
                    "BaseContentRegistry_resolveFluid",
                    "BaseContentRegistry_resolveItem",
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
        native_defines=("_POSIX_C_SOURCE=200809L", "_DARWIN_C_SOURCE=1"),
        native_runs_from_case_root=True,
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
    oracle = run(
        [development_tool("haxe"), "--cwd", str(CASE_ROOT), eval_hxml.name],
        cwd=ROOT,
        timeout=30,
        label=f"{test_case.case_id} Eval test",
    ).stdout
    validate_oracle(test_case, oracle)

    compiler = native_compiler(requested_compiler)
    with tempfile.TemporaryDirectory(prefix=f"hxc-caxecraft-{test_case.case_id}-") as temporary:
        temporary_root = Path(temporary)
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
            timeout=60,
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
            timeout=20,
            label=f"{test_case.case_id} native test",
        ).stdout
        if native_output != oracle:
            raise HaxeCTestFailure(
                f"{test_case.case_id} Eval/native results differ\n"
                f"Eval={oracle!r}\nnative={native_output!r}"
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
                timeout=20,
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
    parser.add_argument("case", choices=tuple(sorted(CASES)))
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
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
