#!/usr/bin/env python3
"""Check the target-neutral named Caxecraft game-pilot foundation."""

from __future__ import annotations

import io
import json
import re
import struct
import subprocess
import sys
import tempfile
import zlib
from contextlib import redirect_stderr, redirect_stdout
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
PILOT = CASE / "src/caxecraft/pilot"
PILOT_CATALOG_SOURCE = PILOT / "PilotCatalog.hx"
PILOT_CATALOG = CASE / "pilot-catalog.json"
APP_SCREEN = CASE / "src/caxecraft/app/AppScreen.hx"
MOTION_INTERPOLATION = CASE / "src/caxecraft/app/MotionInterpolation.hx"
HUD_VIEW = CASE / "src/caxecraft/app/HudView.hx"
HUD_RESOURCES = CASE / "src/caxecraft/app/HudResources.hx"
APP = CASE / "src/caxecraft/app/CaxecraftApp.hx"
MAIN = CASE / "src/caxecraft/app/Main.hx"
sys.path.insert(0, str(CASE))

from run import (  # noqa: E402
    CaxecraftFailure,
    alternate_locale,
    pinned_haxe_environment,
    pinned_haxe_installation,
    resolve_haxe_arguments,
    verify_pinned_haxe,
)
import run as domain_runner  # noqa: E402
import play as playable  # noqa: E402
import benchmark_renderer as renderer_benchmark  # noqa: E402

FORBIDDEN_PILOT_TEXT = (
    re.compile(r"#if\b"),
    re.compile(r"\bDynamic\b"),
    re.compile(r"\bAny\b"),
    re.compile(r"\bReflect\b"),
    re.compile(r"\buntyped\b"),
    re.compile(r"\b__c__\b"),
    re.compile(r"\bArray\b"),
    re.compile(r"\bc\."),
    re.compile(r"\braylib\."),
)


class PilotFailure(RuntimeError):
    pass


def check_pilot_catalog_authority() -> None:
    """Require one Haxe-owned pilot catalog and its checked host manifest."""
    if not PILOT_CATALOG_SOURCE.is_file():
        raise PilotFailure(
            "pilot metadata has no Haxe catalog authority at "
            f"{PILOT_CATALOG_SOURCE.relative_to(ROOT)}"
        )
    if not PILOT_CATALOG.is_file():
        raise PilotFailure(
            "pilot metadata has no generated host manifest at "
            f"{PILOT_CATALOG.relative_to(ROOT)}"
        )
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("pilot-catalog.hxml",), locale="C")
    result = subprocess.run(
        [str(installation.compiler), *arguments],
        cwd=CASE,
        env=pinned_haxe_environment("C", installation),
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        timeout=30,
    )
    expected = f"caxecraft-pilot-catalog: OK: {len(playable.PILOT_CATALOG)} Haxe-owned pilot records\n"
    if result.returncode != 0 or result.stdout != expected or result.stderr:
        raise PilotFailure(
            "Haxe-owned pilot catalog is stale or invalid:\n"
            f"exit:            {result.returncode}\n"
            f"expected stdout: {expected!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )

    runner = Path(playable.__file__).read_text(encoding="utf-8")
    for obsolete in (
        "PILOT_SCRIPT_CODES =",
        "PILOT_FRAME_LIMITS =",
        "PILOT_SCREENSHOT_NAMES =",
        "pilot_defines =",
    ):
        if obsolete in runner:
            raise PilotFailure(f"Python restored duplicated pilot metadata through {obsolete!r}")

    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-pilot-catalog-") as temporary:
        malformed = Path(temporary) / "pilot-catalog.json"
        manifest = json.loads(PILOT_CATALOG.read_text(encoding="utf-8"))
        manifest["pilots"] = [{"id": "unsafe/path"}]
        malformed.write_text(json.dumps(manifest), encoding="utf-8")
        try:
            playable.load_pilot_catalog(malformed)
        except playable.PlayFailure as error:
            if "unknown or missing fields" not in str(error):
                raise PilotFailure(f"malformed pilot catalog failed unclearly: {error}") from error
        else:
            raise PilotFailure("host runner admitted malformed pilot metadata")

        manifest["authoritySha256"] = "0" * 64
        malformed.write_text(json.dumps(manifest), encoding="utf-8")
        try:
            playable.load_pilot_catalog(malformed)
        except playable.PlayFailure as error:
            if "pilot catalog is stale" not in str(error):
                raise PilotFailure(f"stale pilot catalog failed unclearly: {error}") from error
        else:
            raise PilotFailure("host runner admitted stale Haxe pilot metadata")


def check_domain_runner_failure_path() -> None:
    """Keep a normal domain error inside the runner's documented failure path."""
    original = domain_runner.validate_asset_pack

    def fail_before_compile(_root: Path) -> None:
        raise domain_runner.CaxecraftFailure("focused failure-path fixture")

    domain_runner.validate_asset_pack = fail_before_compile
    stderr = io.StringIO()
    stdout = io.StringIO()
    try:
        with redirect_stderr(stderr), redirect_stdout(stdout):
            status = domain_runner.main(())
    finally:
        domain_runner.validate_asset_pack = original
    if status != 1 or "caxecraft-domain: ERROR: focused failure-path fixture" not in stderr.getvalue():
        raise PilotFailure("Caxecraft domain runner no longer reports a controlled pre-compile failure")


def png_chunk(kind: bytes, payload: bytes) -> bytes:
    """Build one checksummed PNG chunk for the telemetry decoder fixtures."""
    checksum = zlib.crc32(kind + payload) & 0xFFFFFFFF
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", checksum)


def write_telemetry_fixture(path: Path, words: list[int], *, unknown_color: bool = False) -> None:
    """Write the smallest faithful 1x-scale framebuffer accepted by the native decoder."""
    width, height = 1280, 720
    stride = width * 4
    filtered = bytearray(height * (stride + 1))
    row_start = (height - 1) * (stride + 1) + 1
    for word_index, word in enumerate(words):
        for digit in range(8):
            nibble = (word >> ((7 - digit) * 4)) & 0xF
            sample_x = (word_index * 8 + digit) * 2 + 1
            color = playable.PILOT_TELEMETRY_COLORS[nibble]
            offset = row_start + sample_x * 4
            filtered[offset : offset + 4] = bytes(color)
    if unknown_color:
        filtered[row_start + 4 : row_start + 8] = bytes((1, 2, 3, 255))

    header = struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0)
    path.write_bytes(
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", header)
        + png_chunk(b"IDAT", zlib.compress(bytes(filtered), level=9))
        + png_chunk(b"IEND", b"")
    )


def expect_telemetry_failure(path: Path, words: list[int], expected: str, *, unknown_color: bool = False) -> None:
    """Prove malformed or unsupported telemetry fails with an actionable reason."""
    write_telemetry_fixture(path, words, unknown_color=unknown_color)
    try:
        playable.decode_pilot_telemetry(path, (1280, 720))
    except playable.PlayFailure as error:
        if expected not in str(error):
            raise PilotFailure(f"telemetry failure omitted {expected!r}: {error}") from error
        return
    raise PilotFailure(f"telemetry fixture unexpectedly admitted {expected}")


def check_native_telemetry_decoder() -> None:
    """Lock the versioned native report boundary without launching a graphical process."""
    words = [0] * playable.PILOT_TELEMETRY_WORDS
    words[0] = playable.PILOT_TELEMETRY_MAGIC
    words[1] = playable.PILOT_TELEMETRY_VERSION
    words[2] = playable.PILOT_TELEMETRY_WORDS
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-pilot-telemetry-") as temporary:
        path = Path(temporary) / "telemetry.png"
        write_telemetry_fixture(path, words)
        if playable.decode_pilot_telemetry(path, (1280, 720)) != words:
            raise PilotFailure("valid pilot telemetry did not round-trip exactly")

        malformed_magic = list(words)
        malformed_magic[0] ^= 1
        expect_telemetry_failure(path, malformed_magic, "magic drifted")

        unsupported_version = list(words)
        unsupported_version[1] += 1
        expect_telemetry_failure(
            path,
            unsupported_version,
            f"version {playable.PILOT_TELEMETRY_VERSION + 1} is unsupported",
        )

        malformed_length = list(words)
        malformed_length[2] -= 1
        expect_telemetry_failure(
            path,
            malformed_length,
            f"declares {playable.PILOT_TELEMETRY_WORDS - 1} words",
        )

        expect_telemetry_failure(path, words, "has unknown color", unknown_color=True)

        # A structurally valid final frame still fails when native code did not
        # observe its review capture. This keeps the producer diagnosis ahead of
        # the later host-side PNG checks.
        report_words = list(words)
        launch = playable.pilot_metadata("launch-smoke")
        report_words[3] = launch.script_code
        report_words[5] = launch.frame_limit
        report_words[27] = 6
        report_words[28] = 0
        report_words[29] = 0
        report_words[30] = 1
        report_words[31] = 1  # Title visible, capture observation deliberately absent.
        report_words[35] = 1
        report_words[40] = 1
        write_telemetry_fixture(path, report_words)
        try:
            playable.build_pilot_report(
                state_path=path,
                review_screenshot=Path(temporary) / "review.png",
                pilot="launch-smoke",
                platform_name="linux",
                raylib_configuration="memory-software",
                renderer="chunk-cache",
                benchmark_renderer=False,
                sanitizers=False,
                cc="cc",
                compiler_version="fixture compiler",
            )
        except playable.PlayFailure as error:
            if "native Raylib FileExists check did not observe" not in str(error):
                raise PilotFailure(f"native capture failure was not actionable: {error}") from error
        else:
            raise PilotFailure("pilot report admitted a missing native capture observation")

        report_words[31] |= 64
        write_telemetry_fixture(path, report_words)
        report = playable.build_pilot_report(
            state_path=path,
            review_screenshot=Path(temporary) / "review.png",
            pilot="launch-smoke",
            platform_name="linux",
            raylib_configuration="memory-software",
            renderer="chunk-cache",
            benchmark_renderer=False,
            sanitizers=False,
            cc="cc",
            compiler_version="fixture compiler",
        )
        evidence = report.get("evidence")
        if not isinstance(evidence, dict) or evidence.get("nativeObservedReviewScreenshot") is not True:
            raise PilotFailure("pilot report omitted its positive native capture observation")


def check_memory_software_capture_normalization() -> None:
    """Lock the pinned software renderer's vertical and channel conversion."""
    width, height = 2, 2
    intended = bytes(
        (
            255, 0, 0, 255,
            0, 255, 0, 255,
            0, 0, 255, 255,
            255, 255, 255, 255,
        )
    )
    raw = bytearray(len(intended))
    stride = width * 4
    for row in range(height):
        source_row = height - row - 1
        for column in range(width):
            source_at = source_row * stride + column * 4
            destination_at = row * stride + column * 4
            red, green, blue, alpha = intended[source_at : source_at + 4]
            raw[destination_at : destination_at + 4] = bytes((blue, green, red, alpha))
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-software-capture-") as temporary:
        executable = Path(temporary) / "caxecraft"
        path = Path(temporary) / "caxecraft-agent-session.png"
        playable.write_rgba_png(path, width, height, bytes(raw))
        playable.prepare_agent_session_screenshot(
            executable,
            {"screenshot": path.name},
            "memory-software",
        )
        actual_width, actual_height, actual = playable.decode_rgba_png(path, "normalized fixture")
        if (actual_width, actual_height, actual) != (width, height, intended):
            raise PilotFailure("live memory/software capture normalization changed orientation or RGBA channels")


def check_native_sanitizer_profile() -> None:
    """Keep the Linux generated-C instrumentation explicit and fail closed."""
    expected = (
        "-fsanitize=address,undefined",
        "-fno-sanitize-recover=all",
        "-fno-omit-frame-pointer",
    )
    if playable.SANITIZER_FLAGS != expected:
        raise PilotFailure("Caxecraft sanitizer flags drifted from the reviewed profile")

    original_tool_version = playable.tool_version
    try:
        playable.tool_version = lambda _executable: "gcc (GCC) 15.1"
        if playable.sanitizer_flags("gcc", "linux") != expected:
            raise PilotFailure("Linux GCC did not receive the reviewed sanitizer profile")
        playable.tool_version = lambda _executable: "clang version 20.1"
        if playable.sanitizer_flags("clang", "linux") != expected:
            raise PilotFailure("Linux Clang did not receive the reviewed sanitizer profile")
        playable.tool_version = lambda _executable: "unknown compiler 1.0"
        try:
            playable.sanitizer_flags("cc", "linux")
        except playable.PlayFailure as error:
            if "does not recognize compiler identity" not in str(error):
                raise PilotFailure(f"unknown sanitizer compiler failed unclearly: {error}") from error
        else:
            raise PilotFailure("unknown compiler entered the sanitizer profile")
        try:
            playable.sanitizer_flags("clang", "macos")
        except playable.PlayFailure as error:
            if "supported only by the Linux" not in str(error):
                raise PilotFailure(f"unsupported sanitizer host failed unclearly: {error}") from error
        else:
            raise PilotFailure("unsupported host entered the sanitizer profile")
    finally:
        playable.tool_version = original_tool_version

    runner_source = Path(playable.__file__).read_text(encoding="utf-8")
    if runner_source.count("*native_sanitizer_flags") != 2:
        raise PilotFailure("sanitizer flags must enter both native compile and link argument arrays")


def check_renderer_pilot_compatibility() -> None:
    """Keep live resize on a backend that actually implements window resizing."""
    playable.validate_renderer_pilot("desktop", "resize-layout")
    playable.validate_renderer_pilot("memory-software", "move-jump-edit")
    try:
        playable.validate_renderer_pilot("memory-software", "resize-layout")
    except playable.PlayFailure as error:
        message = str(error)
        if "does not implement SetWindowSize" not in message or "desktop/Xvfb" not in message:
            raise PilotFailure(f"memory/software resize failed unclearly: {error}") from error
    else:
        raise PilotFailure("memory/software backend admitted an unsupported live-resize pilot")


def check_renderer_benchmark_contract() -> None:
    """Keep the cheap A/B parity and generated-artifact checks executable."""
    shared = {
        "schemaVersion": 1,
        "script": {"id": "move-jump-edit", "inputHash": "fixed-input"},
        "world": {"stateHash": "fixed-scene"},
        "native": {"platform": "linux"},
    }
    cache = {
        **shared,
        "render": {
            "implementation": "chunk-cache",
            "rebuiltTerrainChunks": 0,
            "totalRebuiltTerrainChunks": 20,
            "terrainCacheValid": True,
            "terrainDrawCalls": 3,
        },
        "benchmark": {"terrainMedianMicrosecondsPerFrame": 2},
    }
    baseline = {
        **shared,
        "render": {
            "implementation": "immediate-baseline",
            "rebuiltTerrainChunks": 0,
            "totalRebuiltTerrainChunks": 0,
            "terrainCacheValid": False,
            "terrainDrawCalls": 3,
        },
        "benchmark": {"terrainMedianMicrosecondsPerFrame": 8},
    }
    renderer_benchmark.require_semantic_parity(cache, baseline)
    changed = {**baseline, "world": {"stateHash": "different-scene"}}
    try:
        renderer_benchmark.require_semantic_parity(cache, changed)
    except renderer_benchmark.BenchmarkFailure as error:
        if "changed gameplay or presented output" not in str(error):
            raise PilotFailure(f"renderer parity drift failed unclearly: {error}") from error
    else:
        raise PilotFailure("renderer benchmark admitted different world state")

    if sum(renderer_benchmark.CHUNK_CACHE_PAYLOAD.values()) != 393_632:
        raise PilotFailure("renderer benchmark cache payload accounting drifted")
    cells, scene_hash = renderer_benchmark.benchmark_scene()
    if len(cells) != 32_768 or re.fullmatch(r"[0-9a-f]{64}", scene_hash) is None:
        raise PilotFailure("handwritten C benchmark scene is incomplete or has no source identity")
    c_source = renderer_benchmark.C_BASELINE.read_text(encoding="utf-8")
    for required in ("draw_sheet", "GetTime", "UnloadTexture(adventure)", "CloseWindow()"):
        if required not in c_source:
            raise PilotFailure(f"handwritten C renderer lost required ownership/evidence step {required!r}")
    if renderer_benchmark.ALLOCATOR_CALL.search(c_source) or renderer_benchmark.GOTO_STATEMENT.search(c_source):
        raise PilotFailure("handwritten C renderer introduced allocation or goto")
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-renderer-contract-") as temporary:
        variant = Path(temporary)
        (variant / "generated/src").mkdir(parents=True)
        (variant / "generated/include").mkdir(parents=True)
        (variant / "generated/src/program.c").write_text(
            "int main(void) { return 0; }\n", encoding="utf-8"
        )
        (variant / "generated/include/program.h").write_text(
            "int main(void);\n", encoding="utf-8"
        )
        (variant / "bin").mkdir()
        (variant / "bin/caxecraft").write_bytes(b"native-fixture")
        evidence = renderer_benchmark.source_evidence(variant)
        if evidence["generatedAllocatorCalls"] != 0 or evidence["generatedGotoStatements"] != 0:
            raise PilotFailure("renderer artifact budget fixture did not remain empty")


def check_target_neutral_boundary() -> None:
    # The compiled engine-regression pilots remain allocation-free. The runtime
    # content parser and live observation schema have their own Eval/native
    # contracts. They deliberately use managed arrays for bounded records.
    sources = [
        *(
            path
            for path in PILOT.glob("*.hx")
            if path.name not in (
                "AgentWorldObservation.hx",
                "PilotCatalog.hx",
                "RuntimePilotScript.hx",
            )
        ),
        APP_SCREEN,
        MOTION_INTERPOLATION,
        HUD_VIEW,
    ]
    sources = sorted(sources, key=lambda path: path.as_posix().encode("utf-8"))
    if not sources:
        raise PilotFailure("target-neutral pilot source inventory is empty")
    for source in sources:
        text = source.read_text(encoding="utf-8")
        for pattern in FORBIDDEN_PILOT_TEXT:
            if pattern.search(text):
                raise PilotFailure(
                    f"{source.relative_to(ROOT)} crosses the target-neutral pilot "
                    f"boundary: {pattern.pattern}"
                )


def check_hud_presentation_boundary() -> None:
    """Keep the shipped read-only HUD boundary explicit and non-positional."""
    app = APP.read_text(encoding="utf-8")
    resources = HUD_RESOURCES.read_text(encoding="utf-8")
    for required in (
        "final hudView:HudView = {",
        "drawHud(hudView, hudResources, runtimeTextures, contentRegistry, uiCatalog);",
        "static function drawHud(view:HudView, resources:HudResources,",
    ):
        if required not in app:
            raise PilotFailure(f"CaxecraftApp lost the immutable HUD boundary marker {required!r}")
    if "static function drawHud(visible:" in app:
        raise PilotFailure("CaxecraftApp restored the positional HUD scalar argument list")
    for forbidden in (
        "GameSession",
        "c.Span",
        "c.ConstSpan",
        "__c__",
    ):
        if forbidden in resources:
            raise PilotFailure(
                f"HudResources gained simulation or raw-C authority {forbidden!r}"
            )


def check_outer_application_boundary() -> None:
    """Keep the executable entry thin and the native lifetime in one owner."""
    main = MAIN.read_text(encoding="utf-8")
    app = APP.read_text(encoding="utf-8")
    for required in (
        "final app = new CaxecraftApp();",
        "app.run();",
    ):
        if required not in main:
            raise PilotFailure(f"Main lost its application handoff {required!r}")
    for forbidden in ("raylib.", "GameSession", "InitWindow", "while ("):
        if forbidden in main:
            raise PilotFailure(f"Main regained application ownership {forbidden!r}")
    for required in (
        "final class CaxecraftApp",
        "final activeRuntimeContent = new ActiveRuntimeContent(completeCandidate);",
        "final runtimeContent = activeRuntimeContent.generation();",
        "final activeLevel = switch ActivePlayableLevel.create(loadedCandidate)",
        "var session = activeLevel.session();",
        "public function run():Void",
        "Raylib.InitWindow(",
        "while (!quit && !Raylib.WindowShouldClose())",
        "Raylib.CloseWindow();",
    ):
        if required not in app:
            raise PilotFailure(f"CaxecraftApp lost native lifetime marker {required!r}")
    for forbidden in (
        "new BaseContentRegistry()",
        "loadRuntimeLevel(",
        "final session = new GameSession();",
    ):
        if forbidden in app:
            raise PilotFailure(
                f"CaxecraftApp bypassed its complete runtime-content owner through {forbidden!r}"
            )
    for forbidden in (
        "session.replaceLocalPlayer(",
        "session.deactivateAuthoredItem(",
        "adoptCharacterProfile(",
        "applyCharacterAttack(",
        "reviveCharacterAt(",
        "withCharacterVitals(",
        "decideRecovery(",
        "applyRecoveryInventory(",
        "applyRecoveryVitals(",
    ):
        if forbidden in app:
            raise PilotFailure(
                f"CaxecraftApp regained direct simulation mutation through {forbidden!r}"
            )


def check_runtime_campaign_staging_boundary() -> None:
    """Keep package content staged at launch and outside the compile identity."""
    staged_content = {
        "campaigns/first-adventure/campaign.json",
        "scenarios/first-adventure/western-falls.caxemap",
        "scenarios/first-playable/map.caxemap",
    }
    staged = set(playable.runtime_content_files(CASE))
    if not staged_content <= staged:
        raise PilotFailure(
            "runtime campaign staging lost "
            + ", ".join(sorted(staged_content - staged))
        )

    request_inputs = playable.play_build_inputs(
        type(
            "BuildInputOptions",
            (),
            {"source": None, "raygui_source": None, "prebuilt_raylib_report": None},
        )()
    )
    runtime_paths = staged_content | {
        "assets/manifest.json",
        "assets/atlases/terrain.png",
    }
    for relative in runtime_paths:
        runtime_path = (CASE / relative).resolve()
        for build_input in request_inputs:
            build_path = build_input.path.resolve()
            if runtime_path == build_path or (build_path.is_dir() and runtime_path.is_relative_to(build_path)):
                raise PilotFailure(
                    f"runtime content {relative} entered compile identity through {build_input.logical_name}"
                )


def check_motion_interpolation_boundary() -> None:
    """Keep smoothing visual-only and reset at discontinuous app transitions."""
    app = APP.read_text(encoding="utf-8")
    motion = MOTION_INTERPOLATION.read_text(encoding="utf-8")
    for required in (
        "var motionHistory = startMotion(initialPresentation.localPlayer.body);",
        "motionHistory = advanceMotion(motionHistory, character.body);",
        "motionHistory = resetMotion(character.body);",
        "final renderPosition = sampleMotion(motionHistory, accumulator, FIXED_SECONDS);",
        "VoxelRaycast.trace(session.worldView(), selectionEyeX, selectionEyeY, selectionEyeZ",
        "terrainRenderer.draw(session.worldView(), terrainTexture, terrainTextureReady, adventureTerrainTexture,",
    ):
        if required not in app:
            raise PilotFailure(
                f"CaxecraftApp lost presentation interpolation marker {required!r}"
            )
    if app.count("resetMotionThisFrame = true;") < 5:
        raise PilotFailure(
            "CaxecraftApp no longer resets interpolation across its known discontinuities"
        )
    for forbidden in ("GameSession", "VoxelRaycast", "TerrainRenderer", "raylib.", "c."):
        if forbidden in motion:
            raise PilotFailure(
                f"MotionInterpolation gained simulation or target authority {forbidden!r}"
            )


def run_probe(locale: str) -> str:
    installation = pinned_haxe_installation()
    verify_pinned_haxe(installation)
    arguments = resolve_haxe_arguments(("pilot.hxml",), locale=locale)
    result = subprocess.run(
        [str(installation.compiler), *arguments],
        cwd=CASE,
        env=pinned_haxe_environment(locale, installation),
        check=False,
        capture_output=True,
        text=True,
        encoding="utf-8",
        timeout=30,
    )
    trace = re.fullmatch(
        r"caxecraft-pilot: (\d+) compiled scripts, (\d+) deterministic frames, "
        r"(\d+) checkpoints; bounded quit and shared input interface\n",
        result.stdout,
    )
    compiled: dict[int, int] = {}
    for metadata in playable.PILOT_CATALOG:
        if metadata.execution != "compiled":
            continue
        previous = compiled.get(metadata.script_code)
        if previous is not None and previous != metadata.frame_limit:
            raise PilotFailure(
                f"compiled pilot aliases disagree on script {metadata.script_code} frame bounds"
            )
        compiled[metadata.script_code] = metadata.frame_limit
    expected_scripts = len(compiled)
    expected_frames = sum(compiled.values())
    if (
        result.returncode != 0
        or trace is None
        or int(trace.group(1)) != expected_scripts
        or int(trace.group(2)) != expected_frames
        or int(trace.group(3)) <= 0
        or result.stderr
    ):
        raise PilotFailure(
            f"{locale} pilot probe changed:\n"
            f"exit:            {result.returncode}\n"
            f"expected scripts/frames: {expected_scripts}/{expected_frames}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        check_pilot_catalog_authority()
        check_domain_runner_failure_path()
        check_target_neutral_boundary()
        check_hud_presentation_boundary()
        check_outer_application_boundary()
        check_runtime_campaign_staging_boundary()
        check_motion_interpolation_boundary()
        check_native_telemetry_decoder()
        check_memory_software_capture_normalization()
        check_native_sanitizer_profile()
        check_renderer_pilot_compatibility()
        check_renderer_benchmark_contract()
        locale = alternate_locale()
        if locale == "C":
            raise PilotFailure("no alternate locale is installed for the pilot lane")
        baseline = run_probe("C")
        localized = run_probe(locale)
        if baseline != localized:
            raise PilotFailure("C and alternate-locale pilot traces did not converge")
    except (
        CaxecraftFailure,
        PilotFailure,
        OSError,
        subprocess.TimeoutExpired,
        UnicodeError,
    ) as error:
        print(f"caxecraft-pilot: ERROR: {error}", file=sys.stderr)
        return 1

    print(
        "caxecraft-pilot: OK: named semantic input, exact checkpoints, "
        f"bounded quit, and POSIX C/{locale} locale determinism"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
