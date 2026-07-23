#!/usr/bin/env python3
"""Check the target-neutral named Caxecraft game-pilot foundation."""

from __future__ import annotations

import re
import struct
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path

CASE = Path(__file__).resolve().parent
ROOT = CASE.parents[1]
PILOT = CASE / "src/caxecraft/pilot"
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
import play as playable  # noqa: E402
import benchmark_renderer as renderer_benchmark  # noqa: E402

EXPECTED_TRACE = (
    "caxecraft-pilot: 10 named scripts, 194 deterministic frames, 12 checkpoints; "
    "bounded quit and shared input interface\n"
)
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
        path = Path(temporary) / "software.png"
        playable.write_rgba_png(path, width, height, bytes(raw))
        playable.normalize_memory_software_capture(path)
        actual_width, actual_height, actual = playable.decode_rgba_png(path, "normalized fixture")
        if (actual_width, actual_height, actual) != (width, height, intended):
            raise PilotFailure("memory/software capture normalization changed orientation or RGBA channels")


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

    if sum(renderer_benchmark.CHUNK_CACHE_PAYLOAD.values()) != 196_816:
        raise PilotFailure("renderer benchmark cache payload accounting drifted")
    cells, scene_hash = renderer_benchmark.benchmark_scene()
    if len(cells) != 16_384 or scene_hash != "f1e992a434edb24bed0ff2d114fc3e6fd88ec7bfc495c6b2a22e99b14732dbc5":
        raise PilotFailure("handwritten C benchmark scene stopped matching the generated Haxe adapter")
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
    sources = [*PILOT.glob("*.hx"), APP_SCREEN, MOTION_INTERPOLATION, HUD_VIEW]
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
        "drawHud(hudView, hudResources);",
        "static function drawHud(view:HudView, resources:HudResources):Void",
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
        "final session:GameSession = new GameSession();",
        "public function run():Void",
        "Raylib.InitWindow(",
        "while (!quit && !Raylib.WindowShouldClose())",
        "Raylib.CloseWindow();",
    ):
        if required not in app:
            raise PilotFailure(f"CaxecraftApp lost native lifetime marker {required!r}")
    if "final session = new GameSession();" in app:
        raise PilotFailure("CaxecraftApp stopped owning its session as a final child")


def check_motion_interpolation_boundary() -> None:
    """Keep smoothing visual-only and reset at discontinuous app transitions."""
    app = APP.read_text(encoding="utf-8")
    motion = MOTION_INTERPOLATION.read_text(encoding="utf-8")
    for required in (
        "var motionHistory = startMotion(initialPresentation.localPlayer.body);",
        "motionHistory = advanceMotion(motionHistory, character.body);",
        "motionHistory = resetMotion(character.body);",
        "final renderPosition = sampleMotion(motionHistory, accumulator, FIXED_SECONDS);",
        "VoxelRaycast.trace(cells, selectionEyeX, selectionEyeY, selectionEyeZ",
        "terrainRenderer.draw(cells, terrainTexture, terrainTextureReady, adventureTerrainTexture, adventureTerrainTextureReady,",
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
    if result.returncode != 0 or result.stdout != EXPECTED_TRACE or result.stderr:
        raise PilotFailure(
            f"{locale} pilot probe changed:\n"
            f"exit:            {result.returncode}\n"
            f"expected stdout: {EXPECTED_TRACE!r}\n"
            f"actual stdout:   {result.stdout!r}\n"
            f"actual stderr:   {result.stderr!r}"
        )
    return result.stdout


def main() -> int:
    try:
        check_target_neutral_boundary()
        check_hud_presentation_boundary()
        check_outer_application_boundary()
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
