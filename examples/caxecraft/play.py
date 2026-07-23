#!/usr/bin/env python3
"""Build, launch, or deterministically pilot the native Caxecraft slice."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import platform
import re
import shutil
import statistics
import struct
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
PROVISION_DIR = ROOT / "scripts/raylib"
if str(ROOT) not in sys.path:
    sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(PROVISION_DIR))
import provision  # type: ignore  # noqa: E402
from scripts.raygui import provision as raygui_provision  # noqa: E402


STRICT_FLAGS = (
    "-std=c11",
    "-Wall",
    "-Wextra",
    "-Werror",
    "-pedantic-errors",
    "-Wshadow",
    "-Wconversion",
    "-Wsign-conversion",
    "-Wstrict-prototypes",
    "-Wmissing-prototypes",
    "-Wundef",
    "-Wformat=2",
    "-Wimplicit-fallthrough",
    "-Wcast-align",
    "-Wcast-qual",
)
SANITIZER_FLAGS = (
    "-fsanitize=address,undefined",
    "-fno-sanitize-recover=all",
    "-fno-omit-frame-pointer",
)
PLATFORM_NAMES = {"Darwin": "macos", "Linux": "linux", "Windows": "windows"}
RAYLIB_CONFIGURATIONS = ("desktop", "memory-software")
EXPECTED = CASE / "expected"
# Snapshots review generated structure, not whichever desktop runs the updater.
# Native build/play still selects the real host below, and the Raylib CI matrix
# separately validates each platform's libraries and frameworks.
SNAPSHOT_PLATFORM = "linux"
OUTPUT_MARKER = ".hxc-caxecraft-play-root.json"
PLAYABLE_SNAPSHOT_FORMATS = {
    "playable/hxc.manifest.json": "json",
    "playable/hxc.runtime-plan.json": "json",
    "playable/include/hxc/program.h": "header",
    "playable/include/hxc/modules/caxecraft/app/CaxecraftApp.h": "header",
    "playable/include/hxc/modules/caxecraft/app/MotionInterpolation.h": "header",
    "playable/include/hxc/modules/caxecraft/app/TerrainChunkCache.h": "header",
    "playable/include/hxc/modules/caxecraft/app/TerrainChunkLayout.h": "header",
    "playable/include/hxc/modules/caxecraft/app/TerrainRenderer.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/InventoryState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/ItemKind.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/GuideState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/MosslingState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/BerryDropState.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/AquaticState.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/CharacterBody.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/MiningResult.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/Mining.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/Character.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/CharacterIntent.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/CharacterStep.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/EntityStore.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/GameSession.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/PlayerAgent.h": "header",
    "playable/include/hxc/modules/caxecraft/domain/VitalsState.h": "header",
    "playable/src/modules/caxecraft/app/CaxecraftAtlas.c": "c",
    # CaxecraftTextures is an inline resource-path facade. Its calls are owned
    # and reviewed in CaxecraftApp.c; correct split emission has no empty module file.
    "playable/src/modules/caxecraft/app/AuthoredItemRenderer.c": "c",
    "playable/src/modules/caxecraft/app/HudDigits.c": "c",
    "playable/src/modules/caxecraft/app/TerrainAtlas.c": "c",
    "playable/src/modules/caxecraft/app/TerrainChunkCache.c": "c",
    "playable/src/modules/caxecraft/app/TerrainChunkLayout.c": "c",
    "playable/src/modules/caxecraft/app/TerrainRenderer.c": "c",
    "playable/src/modules/caxecraft/app/WaterRenderer.c": "c",
    "playable/src/modules/caxecraft/content/FirstPlayableLevel.c": "c",
    "playable/src/modules/caxecraft/content/FirstPlayableSessionLoader.c": "c",
    "playable/src/modules/caxecraft/content/BaseContentPack.c": "c",
    "playable/src/modules/caxecraft/app/CaxecraftApp.c": "c",
    "playable/src/modules/caxecraft/app/MotionInterpolation.c": "c",
    "playable/src/modules/caxecraft/app/Main.c": "c",
    "playable/src/modules/caxecraft/gameplay/Inventory.c": "c",
    "playable/src/modules/caxecraft/gameplay/GuideNpc.c": "c",
    "playable/src/modules/caxecraft/gameplay/Mossling.c": "c",
    "playable/src/modules/caxecraft/gameplay/BerryDrop.c": "c",
    "playable/src/modules/caxecraft/domain/Vitals.c": "c",
    "playable/src/modules/caxecraft/gameplay/Recovery.c": "c",
    "playable/src/modules/caxecraft/gameplay/Mining.c": "c",
    "playable/src/modules/caxecraft/localization/FirstPlayableCatalog.c": "c",
    "playable/src/modules/caxecraft/localization/UiCatalog.c": "c",
    "playable/src/modules/caxecraft/domain/Aquatics.c": "c",
    "playable/src/modules/caxecraft/domain/Character.c": "c",
    "playable/src/modules/caxecraft/domain/EntityStore.c": "c",
    "playable/src/modules/caxecraft/domain/GameSession.c": "c",
    "playable/src/modules/caxecraft/domain/PlayerAgent.c": "c",
    "playable/src/modules/caxecraft/domain/World.c": "c",
    "playable/src/modules/caxecraft/gameplay/WorldItemPickup.c": "c",
}
RUNTIME_ASSET_IDS = ("caxecraft-wordmark", "title-panorama", "hud", "items", "adventure-items", "adventure-terrain", "entities", "terrain")
RUNTIME_ASSET_REPORT = "caxecraft-runtime-assets.json"
RUNTIME_CONTENT_FILES = (
    "locales/ui.json",
    "packs/caxecraft/base/content.json",
    "scenarios/first-playable/map.caxemap",
)
FIRST_PLAYABLE_MAP = CASE / "scenarios/first-playable/map.caxemap"
FIRST_PLAYABLE_ADAPTER = CASE / "src/caxecraft/content/FirstPlayableLevel.hx"
LEVEL_SOURCE_HASH_PATTERN = re.compile(r'^inline final SOURCE_SHA256:String = "([0-9a-f]{64})";$', re.MULTILINE)


def remove_owned_stale_stage_files(
    destination: Path,
    stage_root: Path,
    stale_files: list[str],
    *,
    unowned_error: str,
) -> None:
    """Remove obsolete launcher output only below a verified owned variant.

    `destination` is the variant's `bin` directory, whose parent is created by
    `prepare_output_root`. The marker proves that Caxecraft owns that bounded
    build directory. Without it, an unexpected staged file may belong to the
    user, so the same condition remains a hard error.
    """

    if not stale_files:
        return
    marker = destination.parent / OUTPUT_MARKER
    if marker.is_symlink() or not marker.is_file():
        raise PlayFailure(unowned_error)
    state = load_object(marker, "Caxecraft output ownership marker")
    if state != {"kind": "caxecraft-play-output", "schemaVersion": 1}:
        raise PlayFailure(f"Caxecraft output ownership marker is invalid: {marker}")

    candidate_directories: set[Path] = set()
    for raw_path in stale_files:
        relative = validated_relative(raw_path, f"stale staged file {raw_path}")
        target = stage_root.joinpath(*relative.parts)
        if not target.is_symlink() and not target.is_file():
            raise PlayFailure(f"stale Caxecraft stage entry is not a file: {target}")
        target.unlink()
        parent = target.parent
        while parent != stage_root:
            candidate_directories.add(parent)
            parent = parent.parent

    for directory in sorted(candidate_directories, key=lambda path: len(path.parts), reverse=True):
        if directory.is_dir() and not directory.is_symlink() and not any(directory.iterdir()):
            directory.rmdir()

# Exact full-opacity colors sampled from the reviewed front-facing entity cells.
# We count a small family instead of one pixel so a driver may interpolate
# sprite edges without making a present, recognizable actor disappear from the
# test. The structural generated-C check independently requires the billboard
# borrow; together the two checks prove that the original atlas reached a real
# presented frame.
NIA_ENTITY_COLORS = {
    (130, 68, 28),
    (130, 67, 27),
    (132, 68, 27),
    (129, 66, 27),
    (129, 69, 29),
    (129, 68, 29),
    (133, 69, 28),
    (132, 69, 28),
    (131, 68, 28),
    (128, 67, 28),
}
MOSSLING_ENTITY_COLORS = {
    (25, 29, 22),
    (21, 24, 18),
    (17, 22, 16),
    (159, 139, 110),
    (35, 39, 30),
    (124, 107, 86),
    (28, 32, 24),
    (167, 146, 114),
    (165, 144, 112),
    (147, 128, 100),
}
PILOT_TELEMETRY_MAGIC = 0x43585054
PILOT_TELEMETRY_VERSION = 5
PILOT_TELEMETRY_WORDS = 40
PILOT_TELEMETRY_COLORS = tuple(
    (
        8 + nibble * 16,
        247 - nibble * 16,
        (170, 186, 138, 154, 234, 250, 202, 218, 42, 58, 10, 26, 106, 122, 74, 90)[nibble],
        255,
    )
    for nibble in range(16)
)
PILOT_SCRIPT_CODES = {
    "launch-smoke": 0,
    "secondary-locale": 0,
    "move-jump-edit": 1,
    "pause-recapture": 2,
    "combat-drop": 3,
    "recovery-use": 4,
    "full-inventory-gift": 5,
    "full-inventory-mining": 6,
    "resize-layout": 7,
    "aquatic-gear": 8,
    "smooth-motion": 9,
}
PILOT_FRAME_LIMITS = {
    "launch-smoke": 4,
    "secondary-locale": 4,
    "move-jump-edit": 14,
    "pause-recapture": 7,
    "combat-drop": 40,
    "recovery-use": 4,
    "full-inventory-gift": 4,
    "full-inventory-mining": 7,
    "resize-layout": 6,
    "aquatic-gear": 96,
    "smooth-motion": 12,
}


class PlayFailure(RuntimeError):
    """The playable could not be built without weakening its contracts."""


def stage_runtime_assets(destination: Path) -> None:
    """Copy only reviewed runtime assets beside the executable.

    The game uses normalized relative paths, so an installed build behaves the
    same as a checkout build. Exact source hashes are checked before copying;
    the package report then records the same hashes without host paths.
    """

    source_root = CASE / "assets"
    manifest = load_object(source_root / "manifest.json", "Caxecraft asset manifest")
    if manifest.get("schemaVersion") != 1 or manifest.get("status") != "partially-runtime-integrated":
        raise PlayFailure("Caxecraft asset manifest is not ready for reviewed partial runtime packaging")
    raw_assets = manifest.get("assets")
    if not isinstance(raw_assets, list):
        raise PlayFailure("Caxecraft asset manifest omitted its asset inventory")
    by_id: dict[str, dict[str, object]] = {}
    for raw_asset in raw_assets:
        if not isinstance(raw_asset, dict):
            raise PlayFailure("Caxecraft asset manifest contains a malformed asset")
        asset_id = raw_asset.get("id")
        if not isinstance(asset_id, str) or asset_id in by_id:
            raise PlayFailure("Caxecraft asset manifest contains a missing or duplicate asset ID")
        by_id[asset_id] = raw_asset

    stage_root = destination / "assets"
    selected: list[dict[str, str]] = []
    expected_files = {RUNTIME_ASSET_REPORT}
    for asset_id in RUNTIME_ASSET_IDS:
        asset = by_id.get(asset_id)
        if asset is None:
            raise PlayFailure(f"Caxecraft runtime asset {asset_id!r} is missing from the manifest")
        raw_path = asset.get("path")
        expected_hash = asset.get("sha256")
        if not isinstance(raw_path, str) or not isinstance(expected_hash, str):
            raise PlayFailure(f"Caxecraft runtime asset {asset_id!r} lost its path or hash")
        relative = validated_relative(raw_path, f"runtime asset {asset_id} path")
        source = source_root.joinpath(*relative.parts)
        if source.is_symlink() or not source.is_file():
            raise PlayFailure(f"Caxecraft runtime asset is missing or a symlink: {raw_path}")
        actual_hash = hashlib.sha256(source.read_bytes()).hexdigest()
        if actual_hash != expected_hash:
            raise PlayFailure(f"Caxecraft runtime asset hash drifted: {raw_path}")
        expected_files.add(raw_path)
        selected.append({"id": asset_id, "path": raw_path, "sha256": expected_hash})

    if stage_root.exists():
        if stage_root.is_symlink() or not stage_root.is_dir():
            raise PlayFailure("Caxecraft staged asset root is not a real directory")
        existing_files = {
            path.relative_to(stage_root).as_posix()
            for path in stage_root.rglob("*")
            if path.is_file() or path.is_symlink()
        }
        unexpected = sorted(existing_files - expected_files)
        if unexpected:
            remove_owned_stale_stage_files(
                destination,
                stage_root,
                unexpected,
                unowned_error=f"unowned files occupy the Caxecraft staged asset root: {unexpected}",
            )
    stage_root.mkdir(parents=True, exist_ok=True)
    for record in selected:
        relative = validated_relative(record["path"], f"runtime asset {record['id']} path")
        target = stage_root.joinpath(*relative.parts)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source_root.joinpath(*relative.parts), target)

    report = {
        "schemaVersion": 1,
        "packId": manifest.get("packId"),
        "assets": selected,
    }
    (stage_root / RUNTIME_ASSET_REPORT).write_text(
        json.dumps(report, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
        encoding="utf-8",
        newline="\n",
    )


def stage_content_catalogs(destination: Path) -> None:
    """Package validated text sources without claiming runtime loading yet.

    The current C adapters embed reviewed facts at build time. Keeping the exact
    UI catalog, content manifest, and CaxeMap beside the executable makes the
    intended package boundary real: reusable definitions live in the content
    pack, global UI text is separate, and authored prose stays inside the map.
    Native loading remains an explicit later capability rather than an
    example-only shortcut.
    """

    stage_root = destination / "content"
    expected = set(RUNTIME_CONTENT_FILES)
    if stage_root.exists():
        if stage_root.is_symlink() or not stage_root.is_dir():
            raise PlayFailure("Caxecraft staged content root is not a real directory")
        existing = {
            path.relative_to(stage_root).as_posix()
            for path in stage_root.rglob("*")
            if path.is_file() or path.is_symlink()
        }
        unexpected = sorted(existing - expected)
        if unexpected:
            remove_owned_stale_stage_files(
                destination,
                stage_root,
                unexpected,
                unowned_error=f"unowned files occupy the Caxecraft staged content root: {unexpected}",
            )
    for raw_path in RUNTIME_CONTENT_FILES:
        relative = validated_relative(raw_path, f"runtime content {raw_path}")
        source = CASE.joinpath(*relative.parts)
        if source.is_symlink() or not source.is_file():
            raise PlayFailure(f"Caxecraft runtime content is missing or a symlink: {raw_path}")
        target = stage_root.joinpath(*relative.parts)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(source, target)


def development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


def verify_level_adapter_provenance() -> None:
    """Fail quickly when native play would use stale generated level facts.

    Haxe owns map parsing, validation, expansion, and adapter generation. This
    launcher checks only the adapter's recorded source hash, so an ordinary play
    command cannot silently render old terrain after a map edit. The complete
    semantic check remains `npm run test:caxecraft-level-adapter`.
    """

    if FIRST_PLAYABLE_MAP.is_symlink() or not FIRST_PLAYABLE_MAP.is_file():
        raise PlayFailure(f"first-playable map is missing or a symlink: {FIRST_PLAYABLE_MAP}")
    if FIRST_PLAYABLE_ADAPTER.is_symlink() or not FIRST_PLAYABLE_ADAPTER.is_file():
        raise PlayFailure(f"generated first-playable adapter is missing or a symlink: {FIRST_PLAYABLE_ADAPTER}")
    adapter = FIRST_PLAYABLE_ADAPTER.read_text(encoding="utf-8")
    match = LEVEL_SOURCE_HASH_PATTERN.search(adapter)
    if match is None:
        raise PlayFailure("generated first-playable adapter omitted its exact source hash")
    actual = hashlib.sha256(FIRST_PLAYABLE_MAP.read_bytes()).hexdigest()
    if match.group(1) != actual:
        raise PlayFailure(
            "generated first-playable adapter is stale; run "
            "`python3 examples/caxecraft/level_adapter.py` and review the generated Haxe"
        )


def run(arguments: list[str], *, cwd: Path, timeout: int, label: str) -> subprocess.CompletedProcess[str]:
    try:
        result = subprocess.run(
            arguments,
            cwd=cwd,
            env={**os.environ, "HAXE_NO_SERVER": "1", "LC_ALL": "C"},
            check=False,
            capture_output=True,
            text=True,
            timeout=timeout,
        )
    except (OSError, subprocess.TimeoutExpired) as error:
        raise PlayFailure(f"{label} could not run: {error}") from error
    if result.returncode != 0:
        detail = "\n".join(value.strip() for value in (result.stdout, result.stderr) if value.strip())
        suffix = f"\n{detail}" if detail else ""
        raise PlayFailure(f"{label} failed with exit {result.returncode}{suffix}")
    return result


def decode_rgba_png(path: Path, label: str) -> tuple[int, int, bytes]:
    """Decode the narrow PNG form emitted by the pinned Raylib screenshot path."""
    try:
        data = path.read_bytes()
    except OSError as error:
        raise PlayFailure(f"Caxecraft {label} did not produce its framebuffer screenshot: {error}") from error
    if len(data) < 33 or data[:8] != b"\x89PNG\r\n\x1a\n" or data[12:16] != b"IHDR":
        raise PlayFailure(f"Caxecraft {label} screenshot is not a structurally valid PNG")
    width, height = struct.unpack(">II", data[16:24])
    if data[24:29] != bytes((8, 6, 0, 0, 0)):
        raise PlayFailure(f"Caxecraft {label} screenshot must be a non-interlaced 8-bit RGBA PNG")

    compressed = bytearray()
    offset = 8
    saw_end = False
    while offset < len(data):
        if offset + 12 > len(data):
            raise PlayFailure(f"Caxecraft {label} screenshot contains a truncated PNG chunk")
        length = struct.unpack(">I", data[offset : offset + 4])[0]
        chunk_end = offset + 12 + length
        if chunk_end > len(data):
            raise PlayFailure(f"Caxecraft {label} screenshot contains an out-of-bounds PNG chunk")
        chunk_type = data[offset + 4 : offset + 8]
        payload = data[offset + 8 : offset + 8 + length]
        expected_crc = struct.unpack(">I", data[offset + 8 + length : chunk_end])[0]
        if zlib.crc32(chunk_type + payload) & 0xFFFFFFFF != expected_crc:
            raise PlayFailure(f"Caxecraft {label} screenshot contains a PNG checksum mismatch")
        if chunk_type == b"IDAT":
            compressed.extend(payload)
        if chunk_type == b"IEND":
            saw_end = True
            break
        offset = chunk_end
    if not compressed or not saw_end:
        raise PlayFailure(f"Caxecraft {label} screenshot omitted PNG image data or its end marker")

    try:
        filtered = zlib.decompress(bytes(compressed))
    except zlib.error as error:
        raise PlayFailure(f"Caxecraft {label} screenshot contains invalid compressed pixels: {error}") from error
    bytes_per_pixel = 4
    stride = width * bytes_per_pixel
    expected_size = height * (stride + 1)
    if len(filtered) != expected_size:
        raise PlayFailure(f"Caxecraft {label} screenshot pixel payload has the wrong size")

    previous = bytearray(stride)
    pixels = bytearray(width * height * bytes_per_pixel)
    pixel_at = 0
    at = 0
    for _row in range(height):
        filter_kind = filtered[at]
        at += 1
        encoded = filtered[at : at + stride]
        at += stride
        decoded = bytearray(stride)
        for index, value in enumerate(encoded):
            left = decoded[index - bytes_per_pixel] if index >= bytes_per_pixel else 0
            above = previous[index]
            upper_left = previous[index - bytes_per_pixel] if index >= bytes_per_pixel else 0
            if filter_kind == 0:
                predictor = 0
            elif filter_kind == 1:
                predictor = left
            elif filter_kind == 2:
                predictor = above
            elif filter_kind == 3:
                predictor = (left + above) // 2
            elif filter_kind == 4:
                estimate = left + above - upper_left
                left_distance = abs(estimate - left)
                above_distance = abs(estimate - above)
                upper_left_distance = abs(estimate - upper_left)
                predictor = left if left_distance <= above_distance and left_distance <= upper_left_distance else (
                    above if above_distance <= upper_left_distance else upper_left
                )
            else:
                raise PlayFailure(f"Caxecraft {label} screenshot uses unknown PNG filter {filter_kind}")
            decoded[index] = (value + predictor) & 0xFF
        pixels[pixel_at : pixel_at + stride] = decoded
        pixel_at += stride
        previous = decoded
    return width, height, bytes(pixels)


def png_chunk(kind: bytes, payload: bytes) -> bytes:
    """Build one deterministic, checksummed PNG chunk."""
    checksum = zlib.crc32(kind + payload) & 0xFFFFFFFF
    return struct.pack(">I", len(payload)) + kind + payload + struct.pack(">I", checksum)


def write_rgba_png(path: Path, width: int, height: int, pixels: bytes) -> None:
    """Write the narrow RGBA PNG form used by deterministic pilot evidence."""
    stride = width * 4
    if width <= 0 or height <= 0 or len(pixels) != stride * height:
        raise PlayFailure("Caxecraft pilot PNG writer received an invalid RGBA image")
    filtered = bytearray(height * (stride + 1))
    source_at = 0
    destination_at = 0
    for _row in range(height):
        # Filter zero stores the exact row bytes. It is larger than an adaptive
        # filter, but makes the renderer-scoped evidence simple to reproduce.
        filtered[destination_at] = 0
        destination_at += 1
        filtered[destination_at : destination_at + stride] = pixels[source_at : source_at + stride]
        destination_at += stride
        source_at += stride
    header = struct.pack(">IIBBBBB", width, height, 8, 6, 0, 0, 0)
    path.write_bytes(
        b"\x89PNG\r\n\x1a\n"
        + png_chunk(b"IHDR", header)
        + png_chunk(b"IDAT", zlib.compress(bytes(filtered), level=9))
        + png_chunk(b"IEND", b"")
    )


def normalize_memory_software_capture(path: Path) -> None:
    """Convert Raylib 6.0's software capture bytes to a normal top-down RGBA PNG.

    The pinned software renderer's ``swReadPixels`` already returns a vertically
    corrected BGRA image. Raylib's shared ``rlReadScreenPixels`` path then treats
    those bytes as bottom-up RGBA and flips them a second time. The pixels are
    real renderer output; this host-side conversion only restores the public PNG
    convention before scene checks, telemetry decoding, and human review.
    """
    width, height, source = decode_rgba_png(path, "memory/software capture")
    stride = width * 4
    normalized = bytearray(len(source))
    for row in range(height):
        source_row = height - row - 1
        source_at = source_row * stride
        destination_at = row * stride
        source_bytes = source[source_at : source_at + stride]
        # Extended byte slices perform each channel copy in native code instead
        # of paying one Python iteration for every framebuffer pixel.
        normalized[destination_at : destination_at + stride : 4] = source_bytes[2::4]
        normalized[destination_at + 1 : destination_at + stride : 4] = source_bytes[1::4]
        normalized[destination_at + 2 : destination_at + stride : 4] = source_bytes[0::4]
        normalized[destination_at + 3 : destination_at + stride : 4] = source_bytes[3::4]
    write_rgba_png(path, width, height, bytes(normalized))


def validate_smoke_screenshot(
    path: Path,
    *,
    platform_name: str,
    expected_logical_size: tuple[int, int] = (1280, 720),
    expected_title: bool = True,
    expected_drop: bool = False,
    expected_attack: bool = False,
    expected_recovery: bool = False,
    expected_inventory_full: bool = False,
    expected_entities: bool = False,
    expected_open_sky: bool = True,
) -> tuple[int, int]:
    """Prove the captured title frame contains staged art and readable UI."""
    width, height, pixels = decode_rgba_png(path, "smoke")
    expected_dimensions = {expected_logical_size}
    if platform_name == "macos":
        expected_dimensions.add((expected_logical_size[0] * 2, expected_logical_size[1] * 2))
    if (width, height) not in expected_dimensions:
        raise PlayFailure(
            f"Caxecraft smoke screenshot must match its logical {expected_logical_size[0]}x{expected_logical_size[1]} window at an admitted pixel scale, "
            f"found {width}x{height}; "
            "this can indicate a broken high-DPI framebuffer"
        )
    bytes_per_pixel = 4
    stride = width * bytes_per_pixel

    brand_cyan_pixels = 0
    brand_orange_pixels = 0
    brand_white_pixels = 0
    warm_scene_pixels = 0
    green_scene_pixels = 0
    sky_scene_pixels = 0
    sun_scene_pixels = 0
    dark_panel_pixels = 0
    light_ui_pixels = 0
    nia_entity_pixels = 0
    mossling_entity_pixels = 0
    berry_pixels = 0
    recovery_pixels = 0
    inventory_full_pixels = 0
    damage_pixels = 0
    non_dark_pixels = 0
    quantized_colors: set[int] = set()
    for row in range(height):
        row_start = row * stride
        decoded = pixels[row_start : row_start + stride]
        for index in range(0, stride, bytes_per_pixel):
            red, green, blue = decoded[index : index + 3]
            column = index // bytes_per_pixel
            quantized_colors.add((red >> 4) << 8 | (green >> 4) << 4 | (blue >> 4))
            if red > 20 or green > 20 or blue > 20:
                non_dark_pixels += 1
            if red > 80 and green > 60 and red > blue * 1.4 and green > blue * 1.2:
                warm_scene_pixels += 1
            if green > red * 0.9 and green > blue * 1.2 and green > 50:
                green_scene_pixels += 1
            if 80 < red < 180 and 130 < green < 220 and 140 < blue < 235:
                sky_scene_pixels += 1
            if row < height // 3 and column > width * 3 // 4 and red > 240 and green > 210 and blue < 225:
                sun_scene_pixels += 1
            if red < 45 and green < 55 and blue < 65:
                dark_panel_pixels += 1
            if red > 210 and green > 210 and blue > 200:
                light_ui_pixels += 1
            if (red, green, blue) in NIA_ENTITY_COLORS:
                nia_entity_pixels += 1
            if (red, green, blue) in MOSSLING_ENTITY_COLORS:
                mossling_entity_pixels += 1
            if (red, green, blue) == (174, 78, 136):
                berry_pixels += 1
            if (red, green, blue) == (94, 212, 136):
                recovery_pixels += 1
            if (red, green, blue) == (238, 113, 78):
                inventory_full_pixels += 1
            if (red, green, blue) == (218, 65, 72):
                damage_pixels += 1
            if row < height // 4 and width * 3 // 10 <= column < width * 7 // 10:
                if red < 80 and green > 120 and blue > 140:
                    brand_cyan_pixels += 1
                if red > 180 and 60 < green < 190 and blue < 100:
                    brand_orange_pixels += 1
                if red > 220 and green > 220 and blue > 210:
                    brand_white_pixels += 1
    if expected_title and (
        brand_cyan_pixels < 200
        or brand_orange_pixels < 100
        or brand_white_pixels < 1_000
        or warm_scene_pixels < 10_000
        or green_scene_pixels < 10_000
        or len(quantized_colors) < 300
    ):
        raise PlayFailure(
            "Caxecraft smoke framebuffer is missing staged title art or readable UI "
            f"(brandCyan={brand_cyan_pixels}, brandOrange={brand_orange_pixels}, "
            f"brandWhite={brand_white_pixels}, warmScene={warm_scene_pixels}, "
            f"greenScene={green_scene_pixels}, colorBuckets={len(quantized_colors)})"
        )
    # Prove independent visual roles and enough color variety to distinguish
    # the reviewed terrain atlas from the old flat-color cube fallback. The
    # threshold remains deliberately broad across graphics drivers; it is a
    # missing-texture/stalled-frame check, not a pixel-perfect art golden.
    if not expected_title and (
        non_dark_pixels < width * height // 3
        # Most pilots keep a broad independent sky region after HUD and actors.
        # The edit pilot deliberately aims at a selected ground block, so its
        # terrain, outline, HUD, text, and semantic counters are the stronger
        # evidence; a first-person camera is allowed to look away from the sun.
        or (expected_open_sky and sky_scene_pixels < width * height // 5)
        or (expected_open_sky and sun_scene_pixels < 1_000)
        or green_scene_pixels < width * height // 20
        or dark_panel_pixels < 2_000
        or light_ui_pixels < 250
        or len(quantized_colors) < 120
    ):
        raise PlayFailure(
            "Caxecraft pilot framebuffer is blank or lacks a presented game scene "
            f"(nonDark={non_dark_pixels}, sky={sky_scene_pixels}, sun={sun_scene_pixels}, green={green_scene_pixels}, "
            f"darkPanel={dark_panel_pixels}, lightUi={light_ui_pixels}, niaEntity={nia_entity_pixels}, "
            f"mosslingEntity={mossling_entity_pixels}, berries={berry_pixels}, recovery={recovery_pixels}, "
            f"inventoryFull={inventory_full_pixels}, damage={damage_pixels}, colorBuckets={len(quantized_colors)})"
        )
    if expected_entities and (nia_entity_pixels < 40 or mossling_entity_pixels < 20):
        raise PlayFailure(
            "Caxecraft actor pilot did not present both reviewed entity-atlas cells "
            f"(niaEntity={nia_entity_pixels}, mosslingEntity={mossling_entity_pixels})"
        )
    if expected_drop and berry_pixels < 20:
        raise PlayFailure(
            "Caxecraft combat pilot did not present its berry drop "
            f"(berries={berry_pixels})"
        )
    if expected_attack and damage_pixels < 20:
        raise PlayFailure(
            "Caxecraft combat pilot did not present its telegraphed attack feedback "
            f"(damage={damage_pixels})"
        )
    if expected_recovery and recovery_pixels < 20:
        raise PlayFailure(
            "Caxecraft recovery pilot did not present its successful semantic feedback "
            f"(recovery={recovery_pixels})"
        )
    if expected_inventory_full and inventory_full_pixels < 20:
        raise PlayFailure(
            "Caxecraft full-inventory pilot did not present its capacity warning "
            f"(inventoryFull={inventory_full_pixels})"
        )
    return width, height


def signed_word(value: int) -> int:
    """Interpret one decoded 32-bit word as Haxe/C's signed `Int`."""
    return value - 0x1_0000_0000 if value >= 0x8000_0000 else value


def decode_pilot_telemetry(path: Path, expected_logical_size: tuple[int, int]) -> list[int]:
    """Read and validate the pilot-only colored record at the framebuffer edge."""
    width, height, pixels = decode_rgba_png(path, "pilot state")
    logical_width, logical_height = expected_logical_size
    if width % logical_width != 0 or height % logical_height != 0:
        raise PlayFailure(
            f"pilot telemetry framebuffer {width}x{height} is not an integer scale of "
            f"{logical_width}x{logical_height}"
        )
    scale_x = width // logical_width
    scale_y = height // logical_height
    if scale_x != scale_y or scale_x not in (1, 2):
        raise PlayFailure(f"pilot telemetry uses unsupported framebuffer scale {scale_x}x{scale_y}")
    scale = scale_x
    required_width = PILOT_TELEMETRY_WORDS * 8 * 2 * scale
    if required_width > width:
        raise PlayFailure("pilot telemetry record does not fit in the captured framebuffer")

    color_to_nibble = {color: nibble for nibble, color in enumerate(PILOT_TELEMETRY_COLORS)}
    words: list[int] = []
    sample_y = height - scale
    for word_index in range(PILOT_TELEMETRY_WORDS):
        value = 0
        for digit in range(8):
            sample_x = (word_index * 8 + digit) * 2 * scale + scale
            offset = (sample_y * width + sample_x) * 4
            color = tuple(pixels[offset : offset + 4])
            nibble = color_to_nibble.get(color)
            if nibble is None:
                raise PlayFailure(
                    f"pilot telemetry word {word_index}, digit {digit} has unknown color {color}"
                )
            value = (value << 4) | nibble
        words.append(value)

    if words[0] != PILOT_TELEMETRY_MAGIC:
        raise PlayFailure(f"pilot telemetry magic drifted: 0x{words[0]:08x}")
    if words[1] != PILOT_TELEMETRY_VERSION:
        raise PlayFailure(
            f"pilot telemetry version {words[1]} is unsupported; expected {PILOT_TELEMETRY_VERSION}"
        )
    if words[2] != PILOT_TELEMETRY_WORDS:
        raise PlayFailure(
            f"pilot telemetry declares {words[2]} words; expected {PILOT_TELEMETRY_WORDS}"
        )
    return words


def build_pilot_report(
    *,
    state_path: Path,
    review_screenshot: Path,
    pilot: str,
    platform_name: str,
    raylib_configuration: str,
    renderer: str,
    benchmark_renderer: bool,
    sanitizers: bool,
    cc: str,
    compiler_version: str,
) -> dict[str, object]:
    """Convert one validated native framebuffer record into portable JSON evidence."""
    logical_size = (960, 540) if pilot == "resize-layout" else (1280, 720)
    words = decode_pilot_telemetry(state_path, logical_size)
    script_code = signed_word(words[3])
    expected_code = PILOT_SCRIPT_CODES[pilot]
    if script_code != expected_code:
        raise PlayFailure(f"pilot {pilot!r} emitted script code {script_code}; expected {expected_code}")
    completed_frames = signed_word(words[5])
    completed_ticks = signed_word(words[6])
    expected_frames = PILOT_FRAME_LIMITS[pilot]
    if completed_frames != expected_frames:
        raise PlayFailure(
            f"pilot {pilot!r} completed {completed_frames} frames; expected its bounded {expected_frames}"
        )
    if completed_ticks < 0 or completed_ticks > completed_frames:
        raise PlayFailure(
            f"pilot {pilot!r} reported impossible fixed ticks {completed_ticks}/{completed_frames}"
        )

    signed = [signed_word(value) for value in words]
    for field, value in (
        ("grounded", signed[13]),
        ("selection.hit", signed[15]),
        ("mosslingAlive", signed[30]),
    ):
        if value not in (0, 1):
            raise PlayFailure(f"pilot telemetry {field} must be zero or one, found {value}")
    if signed[22] < 0 or signed[23] < 0 or signed[24] < 0:
        raise PlayFailure("pilot telemetry edit counters cannot be negative")
    if signed[25] < 0 or signed[26] < 0:
        raise PlayFailure("pilot telemetry render counters cannot be negative")
    if not 0 <= signed[27] <= 6 or not 0 <= signed[28] < 8 or not 0 <= signed[29] <= 2:
        raise PlayFailure("pilot telemetry gameplay carriers are outside their closed ranges")
    if not 0 <= signed[31] <= 31:
        raise PlayFailure("pilot telemetry presentation flags contain unknown bits")
    if signed[32] < 0 or signed[33] < 0 or signed[34] < 0 or signed[35] not in (0, 1):
        raise PlayFailure("pilot telemetry terrain-cache counters are outside their closed ranges")
    if any(value < 0 for value in signed[36:40]):
        raise PlayFailure("pilot telemetry renderer timings cannot be negative")
    title_visible = bool(signed[31] & 1)
    if signed[26] > 3:
        raise PlayFailure("pilot telemetry exceeded two opaque terrain batches plus one water batch")
    if title_visible and signed[26] != 0:
        raise PlayFailure("title-only pilot unexpectedly submitted world terrain")
    if title_visible and (signed[32] != 0 or signed[33] != 0 or signed[34] != 0):
        raise PlayFailure("title-only pilot unexpectedly prepared terrain chunks")
    if not title_visible and signed[26] != 3:
        raise PlayFailure("gameplay pilot did not submit the base, adventure, and water batches exactly once")
    if not title_visible and (signed[32] <= 0 or signed[35] != 1):
        raise PlayFailure("gameplay pilot did not submit valid terrain faces")
    if renderer == "chunk-cache":
        if not title_visible and signed[34] < 16:
            raise PlayFailure("gameplay pilot did not prepare the complete terrain cache")
        if not title_visible and signed[33] != 0:
            raise PlayFailure("gameplay pilot rebuilt an unchanged terrain chunk on its final steady frame")
        if pilot == "move-jump-edit" and signed[34] <= 16:
            raise PlayFailure("move-jump-edit pilot did not rebuild terrain after its successful edits")
    elif signed[33] != 0 or signed[34] != 0:
        raise PlayFailure("immediate benchmark baseline unexpectedly reported chunk rebuilds")
    if benchmark_renderer:
        if pilot != "move-jump-edit" or signed[37] != completed_frames - 2 or signed[36] <= 0:
            raise PlayFailure("renderer benchmark did not measure every post-warmup terrain frame")
    elif any(signed[index] != 0 for index in range(36, 40)):
        raise PlayFailure("ordinary pilot unexpectedly retained renderer timing instrumentation")
    aquatic_gear_equipped = bool(signed[31] & 8)
    interpolation_observed = bool(signed[31] & 16)
    if pilot == "aquatic-gear" and not aquatic_gear_equipped:
        raise PlayFailure("aquatic-gear pilot completed without collecting and equipping the authored item")
    if pilot == "smooth-motion" and (
        not interpolation_observed
        or completed_ticks <= 0
        or completed_ticks >= completed_frames
        or signed[8] <= 5000
        or signed[11] <= 0
        or signed[13] != 0
    ):
        raise PlayFailure(
            "smooth-motion pilot did not render a real move-and-jump state between fixed ticks "
            f"(observed={interpolation_observed}, ticks={completed_ticks}, frames={completed_frames}, "
            f"yMilli={signed[8]}, velocityYMilli={signed[11]}, grounded={signed[13]})"
        )
    if pilot == "move-jump-edit" and (
        signed[15] != 1 or signed[22] != 1 or signed[23] != 1 or signed[24] != 0
    ):
        raise PlayFailure(
            "move-jump-edit pilot did not select terrain, remove one block, and place one block "
            f"(selection={signed[15]}, removed={signed[22]}, placed={signed[23]}, rejected={signed[24]})"
        )

    raylib_lock = provision.load_lock()
    upstream = raylib_lock.get("upstream")
    if not isinstance(upstream, dict):
        raise PlayFailure("Raylib lock omitted its upstream identity")
    report: dict[str, object] = {
        "schemaVersion": 1,
        "script": {
            "id": pilot,
            "code": script_code,
            "inputHash": f"{words[4]:08x}",
        },
        "clock": {
            "fixedStepMilliseconds": 50,
            "completedFrames": completed_frames,
            "completedTicks": completed_ticks,
        },
        "player": {
            "xMilli": signed[7],
            "yMilli": signed[8],
            "zMilli": signed[9],
            "velocityXMilli": signed[10],
            "velocityYMilli": signed[11],
            "velocityZMilli": signed[12],
            "grounded": signed[13] == 1,
        },
        "world": {
            "stateHash": f"{words[14]:08x}",
            "selection": {
                "hit": signed[15] == 1,
                "cell": [signed[16], signed[17], signed[18]],
                "previous": [signed[19], signed[20], signed[21]],
            },
            "edits": {
                "removed": signed[22],
                "placed": signed[23],
                "rejected": signed[24],
            },
        },
        "render": {
            "visibleBlocks": signed[25],
            "terrainDrawCalls": signed[26],
            "title": title_visible,
            "paused": bool(signed[31] & 2),
            "cursorCaptured": bool(signed[31] & 4),
            "interpolationObserved": interpolation_observed,
            "visibleTerrainFaces": signed[32],
            "rebuiltTerrainChunks": signed[33],
            "totalRebuiltTerrainChunks": signed[34],
            "terrainCacheValid": signed[35] == 1,
            "implementation": renderer,
        },
        "gameplay": {
            "health": signed[27],
            "hotbarSlot": signed[28],
            "guidePhase": signed[29],
            "mosslingAlive": signed[30] == 1,
            "aquaticGearEquipped": aquatic_gear_equipped,
        },
        "termination": {"reason": "script-complete", "exitCode": 0},
        "native": {
            "platform": platform_name,
            "compiler": {"command": Path(cc).name, "version": compiler_version},
            "sanitizers": ["address", "undefined"] if sanitizers else [],
            "raylib": {
                "release": upstream.get("release"),
                "commit": upstream.get("commit"),
                "configuration": raylib_configuration,
            },
        },
        "evidence": {"reviewScreenshot": review_screenshot.name},
    }
    if benchmark_renderer:
        report["benchmarkSample"] = {
            "terrainMicroseconds": signed[36],
            "measuredFrames": signed[37],
            "updateMicroseconds": signed[38],
            "preparationMicroseconds": signed[39],
        }
    return report


def validate_presented_screenshot(
    path: Path,
    *,
    platform_name: str,
    expected_drop: bool = False,
    expected_attack: bool = False,
    expected_recovery: bool = False,
    expected_inventory_full: bool = False,
    expected_entities: bool = False,
    expected_open_sky: bool = True,
) -> tuple[int, int]:
    """Require a real, nonblank presented frame without prescribing its scene."""

    return validate_smoke_screenshot(
        path,
        platform_name=platform_name,
        expected_title=False,
        expected_drop=expected_drop,
        expected_attack=expected_attack,
        expected_recovery=expected_recovery,
        expected_inventory_full=expected_inventory_full,
        expected_entities=expected_entities,
        expected_open_sky=expected_open_sky,
    )


def host_platform() -> str:
    value = PLATFORM_NAMES.get(platform.system())
    if value is None:
        raise PlayFailure(f"Caxecraft does not yet know how to link on {platform.system()!r}")
    return value


def tool_version(executable: str) -> str:
    result = run([executable, "--version"], cwd=ROOT, timeout=30, label=f"{executable} identity")
    for line in result.stdout.splitlines():
        if line.strip():
            return line.strip()
    raise PlayFailure(f"{executable} did not report a version")


def sanitizer_flags(executable: str, platform_name: str) -> tuple[str, ...]:
    """Return the reviewed generated-C sanitizer profile or fail closed."""
    if platform_name != "linux":
        raise PlayFailure("--sanitizers is currently supported only by the Linux GCC/Clang headless lanes")
    identity = tool_version(executable).lower()
    if "clang" not in identity and "gcc" not in identity and "free software foundation" not in identity:
        raise PlayFailure(f"--sanitizers does not recognize compiler identity {identity!r}")
    return SANITIZER_FLAGS


def validate_renderer_pilot(raylib_configuration: str, pilot: str | None) -> None:
    """Reject a pilot when the selected pinned backend cannot perform its action."""
    if raylib_configuration == "memory-software" and pilot == "resize-layout":
        raise PlayFailure(
            "resize-layout requires Raylib's desktop backend: pinned Raylib 6.0 "
            "PLATFORM=Memory does not implement SetWindowSize; use the desktop/Xvfb lane"
        )


def load_object(path: Path, label: str) -> dict[str, object]:
    try:
        value: object = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        raise PlayFailure(f"cannot read {label}: {error}") from error
    if not isinstance(value, dict):
        raise PlayFailure(f"{label} must contain a JSON object")
    return value


def text_array(value: object, label: str) -> list[str]:
    if not isinstance(value, list) or not all(isinstance(item, str) for item in value):
        raise PlayFailure(f"{label} must be a string array")
    return list(value)


def owned_fact_names(value: object, label: str) -> list[str]:
    if not isinstance(value, list):
        raise PlayFailure(f"{label} must be an array")
    names: list[str] = []
    for index, item in enumerate(value):
        if not isinstance(item, dict) or set(item) != {"name", "ownerModulePaths"}:
            raise PlayFailure(f"{label}[{index}] must contain a name and owners")
        name = item.get("name")
        owners = item.get("ownerModulePaths")
        if not isinstance(name, str) or not name or not text_array(owners, f"{label}[{index}].ownerModulePaths"):
            raise PlayFailure(f"{label}[{index}] contains an invalid name or empty owners")
        names.append(name)
    if len(names) != len(set(names)):
        raise PlayFailure(f"{label} contains duplicate names")
    return names


def prepare_output_root(path: Path) -> Path:
    forbidden = {Path(path.anchor), ROOT.resolve(), Path.home().resolve()}
    if path in forbidden:
        raise PlayFailure(f"--output-root cannot own broad directory {path}")
    if path.exists() and (not path.is_dir() or path.is_symlink()):
        raise PlayFailure(f"--output-root must be a real directory: {path}")
    path.mkdir(parents=True, exist_ok=True)
    marker = path / OUTPUT_MARKER
    if marker.is_file() and not marker.is_symlink():
        state = load_object(marker, "Caxecraft output ownership marker")
        if state != {"kind": "caxecraft-play-output", "schemaVersion": 1}:
            raise PlayFailure(f"Caxecraft output ownership marker is invalid: {marker}")
        return path
    if any(path.iterdir()):
        raise PlayFailure(f"unowned files occupy --output-root: {path}")
    marker.write_text(
        json.dumps({"kind": "caxecraft-play-output", "schemaVersion": 1}, sort_keys=True) + "\n",
        encoding="utf-8",
    )
    return path


def validated_relative(value: str, label: str) -> PurePosixPath:
    path = PurePosixPath(value)
    if path.is_absolute() or path.as_posix() != value or any(part in ("", ".", "..") for part in path.parts):
        raise PlayFailure(f"{label} must be a normalized relative path")
    return path


def compile_haxe(
    generated: Path,
    *,
    layout: str,
    platform_name: str,
    raylib_configuration: str,
    pilot: str | None = None,
    renderer: str = "chunk-cache",
    benchmark_renderer: bool = False,
) -> dict[str, object]:
    verify_level_adapter_provenance()
    if raylib_configuration not in RAYLIB_CONFIGURATIONS:
        raise PlayFailure(f"unknown Raylib configuration {raylib_configuration!r}")
    arguments = [
        development_tool("haxe"),
        "--cwd",
        str(CASE),
        "play.hxml",
        "-D",
        "hxc_runtime_diagnostics=off",
        "-D",
        f"raylib_platform_{platform_name}",
        "-D",
        (
            "raylib_configuration_desktop"
            if raylib_configuration == "desktop"
            else "raylib_configuration_memory"
        ),
    ]
    if layout != "split":
        arguments.extend(["-D", f"hxc_project_layout={layout}"])
    if renderer == "immediate-baseline":
        arguments.extend(["-D", "caxecraft_renderer_baseline"])
    elif renderer != "chunk-cache":
        raise PlayFailure(f"unknown Caxecraft renderer {renderer!r}")
    if benchmark_renderer:
        arguments.extend(["-D", "caxecraft_render_benchmark"])
    if pilot is not None:
        pilot_defines = {
            "launch-smoke": "caxecraft_pilot_launch_smoke",
            "secondary-locale": "caxecraft_pilot_secondary_locale",
            "move-jump-edit": "caxecraft_pilot_move_jump_edit",
            "pause-recapture": "caxecraft_pilot_pause_recapture",
            "combat-drop": "caxecraft_pilot_combat_drop",
            "recovery-use": "caxecraft_pilot_recovery_use",
            "full-inventory-gift": "caxecraft_pilot_full_inventory_gift",
            "full-inventory-mining": "caxecraft_pilot_full_inventory_mining",
            "resize-layout": "caxecraft_pilot_resize_layout",
            "aquatic-gear": "caxecraft_pilot_aquatic_gear",
            "smooth-motion": "caxecraft_pilot_smooth_motion",
        }
        pilot_define = pilot_defines.get(pilot)
        if pilot_define is None:
            raise PlayFailure(f"unknown Caxecraft pilot script {pilot!r}")
        arguments.extend(["-D", "caxecraft_pilot", "-D", pilot_define])
    arguments.extend(["--custom-target", f"c={generated}"])
    run(arguments, cwd=ROOT, timeout=120, label="Caxecraft Haxe-to-C compile")

    manifest = load_object(generated / "hxc.manifest.json", "generated Caxecraft manifest")
    runtime_plan = load_object(generated / "hxc.runtime-plan.json", "generated Caxecraft runtime plan")
    configuration = manifest.get("configuration")
    build = manifest.get("build")
    if not isinstance(configuration, dict) or configuration.get("projectLayout") != layout:
        raise PlayFailure("generated Caxecraft manifest lost the requested project layout")
    if (
        manifest.get("compilationStatus") != "lowered-direct-value-executable"
        or not isinstance(build, dict)
        or build.get("artifact") != {"targetName": "hxc_program", "kind": "executable"}
    ):
        raise PlayFailure("generated Caxecraft manifest does not describe a direct executable")
    if runtime_plan.get("selectedFeatures") != [] or runtime_plan.get("artifacts") != []:
        raise PlayFailure("Caxecraft unexpectedly selected hxrt")
    validate_generated_playable(generated, layout=layout, pilot=pilot, renderer=renderer)
    return manifest


def validate_generated_playable(generated: Path, *, layout: str, pilot: str | None, renderer: str) -> None:
    sources = sorted(generated.glob("src/**/*.c"), key=lambda path: path.as_posix().encode("utf-8"))
    if not sources:
        raise PlayFailure("Caxecraft emitted no C sources")
    combined = "\n".join(path.read_text(encoding="utf-8") for path in sources)
    app_relative = {
        "split": "src/modules/caxecraft/app/CaxecraftApp.c",
        "package": "src/packages/caxecraft/app/package.c",
        "unity": "src/program.c",
    }.get(layout)
    if app_relative is None:
        raise PlayFailure(f"unknown generated Caxecraft layout {layout!r}")
    app_path = generated / app_relative
    if not app_path.is_file():
        raise PlayFailure(f"generated Caxecraft {layout} app source is missing: {app_relative}")
    app = app_path.read_text(encoding="utf-8")
    if layout == "split":
        entry_relative = "src/modules/caxecraft/app/Main.c"
        entry_path = generated / entry_relative
        if not entry_path.is_file():
            raise PlayFailure(f"generated Caxecraft entry source is missing: {entry_relative}")
        entry = entry_path.read_text(encoding="utf-8")
        if "InitWindow(" in entry or "WindowShouldClose(" in entry:
            raise PlayFailure("generated Caxecraft Main regained native application lifetime")
        if len(entry.splitlines()) > 30:
            raise PlayFailure("generated Caxecraft Main is no longer a small executable handoff")
        for required in ("CaxecraftApp", "_run("):
            if required not in entry:
                raise PlayFailure(
                    f"generated Caxecraft Main omitted application handoff {required!r}"
                )
        app_header_relative = "include/hxc/modules/caxecraft/app/CaxecraftApp.h"
        app_header_path = generated / app_header_relative
        if not app_header_path.is_file():
            raise PlayFailure(
                f"generated Caxecraft application header is missing: {app_header_relative}"
            )
        app_header = app_header_path.read_text(encoding="utf-8")
        if (
            "struct hxc_caxecraft_domain_GameSession hxc_session;" not in app_header
            or "struct hxc_caxecraft_domain_GameSession *hxc_session;" in app_header
        ):
            raise PlayFailure(
                "generated CaxecraftApp must own GameSession as one direct child struct"
            )
    for required in (
        "InitWindow(",
        "IsWindowReady(",
        "WindowShouldClose(",
        "BeginDrawing(",
        "BeginMode3D(",
        "DrawCube(",
        "DrawCubeWires(",
        "FirstPlayableSessionLoader_loadCandidate(",
        "GameSession_bindLocalPlayer(",
        "GameSession_tick(",
        "GameSession_view(",
        "hxc_completedTicks",
        "WaterRenderer_draw(",
    ):
        if required not in app:
            raise PlayFailure(f"generated Caxecraft app omitted required call {required}")
    if renderer == "chunk-cache":
        if "TerrainChunkCache_prepare(" not in combined or "TerrainImmediateBaseline_drawImmediate(" in combined:
            raise PlayFailure("generated Caxecraft did not retain only the selected chunk-cache renderer")
    elif "TerrainImmediateBaseline_drawImmediate(" not in combined or "TerrainChunkCache_prepare(" in combined:
        raise PlayFailure("generated Caxecraft did not retain only the selected immediate benchmark baseline")
    loader_call = app.find("FirstPlayableSessionLoader_loadCandidate(")
    window_call = app.find("InitWindow(")
    if loader_call < 0 or window_call < 0 or loader_call >= window_call:
        raise PlayFailure(
            "generated Caxecraft must validate and assemble its candidate session before opening Raylib"
        )
    for forbidden in (
        "FirstPlayableLevel_loadTerrain(",
        "GameSession_resetEmptyWorld(",
        "GameSession_activateAuthoredItemDuringLoad(",
        "GameSession_writeTerrainRunDuringLoad(",
        "PlayerAgent_bind(",
    ):
        if forbidden in app:
            raise PlayFailure(
                f"generated Caxecraft app bypassed the typed session loader through {forbidden}"
            )
    if "hxc_updateCount" in app:
        raise PlayFailure(
            "generated Caxecraft app retained a second authoritative fixed-tick counter"
        )
    if "EntityStore_read(" in app:
        raise PlayFailure(
            "generated Caxecraft presentation bypassed GameSession.view to read mutable entity storage"
        )
    for required in (
        "MotionInterpolation_start(",
        "MotionInterpolation_advance(",
        "MotionInterpolation_sample(",
    ):
        if required not in app:
            raise PlayFailure(
                f"generated Caxecraft app omitted presentation call {required}"
            )
    raycast_call = app.find("VoxelRaycast_trace(")
    interpolation_call = app.find("MotionInterpolation_sample(")
    terrain_call = app.find("TerrainRenderer_draw(")
    if (
        min(raycast_call, interpolation_call, terrain_call) < 0
        or raycast_call >= interpolation_call
        or interpolation_call >= terrain_call
    ):
        raise PlayFailure(
            "generated Caxecraft must select from committed state before sampling visual motion"
        )
    motion_relative = {
        "split": "src/modules/caxecraft/app/MotionInterpolation.c",
        "package": "src/packages/caxecraft/app/package.c",
        "unity": "src/program.c",
    }[layout]
    motion_source = generated.joinpath(motion_relative).read_text(encoding="utf-8")
    for forbidden in ("goto ", "malloc(", "calloc(", "realloc(", "hxrt"):
        if forbidden in motion_source:
            raise PlayFailure(
                f"generated Caxecraft interpolation contains forbidden pattern {forbidden!r}"
            )
    session_relative = {
        "split": "src/modules/caxecraft/domain/GameSession.c",
        "package": "src/packages/caxecraft/domain/package.c",
        "unity": "src/program.c",
    }[layout]
    session_source = generated.joinpath(session_relative).read_text(encoding="utf-8")
    for required in (
        "GameSession_bindLocalPlayer(",
        "GameSession_tick(",
        "GameSession_view(",
        "hxc_completedTicks",
        "hxc_tickIndex",
        "GameSession_placeInitialWaterVolume(",
        "GameSession_placeWaterSource(",
        "WaterSimulation_tick(",
        "WaterSimulation_placeInitialVolume(",
        "WaterSimulation_placeSource(",
        "Character_step(",
    ):
        if required not in session_source:
            raise PlayFailure(
                f"generated Caxecraft session omitted fixed-tick call {required}"
            )
    for required in ("FirstPlayableLevel_loadTerrain(", "FirstPlayableSessionLoader_loadCandidate("):
        if required not in combined:
            raise PlayFailure(f"generated Caxecraft output omitted level assembly call {required}")
    # Catalogs select text and the application renderer draws it. Both lookup
    # functions must reach C, while every Raylib draw remains outside their
    # generated modules. Catalog completeness is checked against source data by
    # test:caxecraft-localization rather than inferred from call-site counts.
    for lookup in ("UiCatalog_text(", "FirstPlayableCatalog_text("):
        if lookup not in combined:
            raise PlayFailure(f"generated Caxecraft output omitted typed localization lookup {lookup}")
    if "DrawText(" not in app:
        raise PlayFailure("generated Caxecraft app omitted localized Raylib text rendering")
    # Pilot builds replace live keyboard and mouse sampling with a deterministic
    # in-process input provider. Requiring GetMouseDelta there would reject the
    # exact dead-code removal that makes the two providers a clean compile-time
    # choice. Normal playable builds must still prove the real input path.
    if pilot is None and "GetMouseDelta(" not in app:
        raise PlayFailure("generated Caxecraft app omitted direct Raylib call GetMouseDelta(")
    if pilot == "resize-layout" and "SetWindowSize(" not in app:
        raise PlayFailure("generated Caxecraft resize pilot omitted direct Raylib call SetWindowSize(")
    if pilot == "secondary-locale" and "UiCatalog_nextLocale(" not in app:
        raise PlayFailure("generated Caxecraft secondary-locale pilot did not select the next validated catalog")
    # This first slice draws wire geometry only for the block under the
    # crosshair. A second call site would permit per-block fill/wire switching,
    # which previously made raylib flush thousands of tiny GPU batches before
    # the first frame could appear.
    if app.count("DrawCubeWires(") != 1:
        raise PlayFailure("generated Caxecraft app must contain exactly one selected-block wire-outline call site")
    terrain_relative = {
        "split": "src/modules/caxecraft/app/TerrainRenderer.c",
        "package": "src/packages/caxecraft/app/package.c",
        "unity": "src/program.c",
    }[layout]
    terrain_source = generated.joinpath(terrain_relative).read_text(encoding="utf-8")
    if layout == "split" and renderer == "immediate-baseline":
        baseline_path = generated / "src/modules/caxecraft/app/TerrainImmediateBaseline.c"
        if not baseline_path.is_file():
            raise PlayFailure("generated immediate renderer omitted its benchmark-only module")
        terrain_source += "\n" + baseline_path.read_text(encoding="utf-8")
    if "DrawCube(" in terrain_source:
        raise PlayFailure("generated terrain renderer regressed to one native DrawCube call per visible block")
    for required in (
        "rlSetTexture(",
        "rlBegin(",
        "rlEnd(",
        "rlNormal3f(",
        "rlColor4ub(",
        "rlTexCoord2f(",
        "rlVertex3f(",
    ):
        if required not in terrain_source:
            raise PlayFailure(f"generated terrain renderer omitted selected direct rlgl call {required}")
    # Texture selection happens outside the voxel loop. One generated rlBegin/
    # rlEnd call site belongs to the reusable per-sheet helper; `draw` invokes
    # that helper once for each of the two reviewed opaque atlases.
    if layout == "split" and (terrain_source.count("rlBegin(") != 1 or terrain_source.count("rlEnd(") != 1):
        raise PlayFailure("generated terrain renderer must retain one reusable coherent-batch helper")
    mining_relative = {
        "split": "src/modules/caxecraft/gameplay/Mining.c",
        "package": "src/packages/caxecraft/gameplay/package.c",
        "unity": "src/program.c",
    }[layout]
    mining_source = generated.joinpath(mining_relative).read_text(encoding="utf-8")
    # Start at the last spelling of the helper name: the earlier spelling is
    # the call from `attempt`, while the last is the helper definition. This
    # keeps the assertion valid in split, package, and unity layouts.
    mining_start = mining_source.rfind("hxc_caxecraft_gameplay_Mining_collect(")
    mining_section = mining_source[mining_start:] if mining_start >= 0 else ""
    capacity_index = mining_section.find("hxc_caxecraft_gameplay_Inventory_acceptedAmount(")
    removal_index = mining_section.find("hxc_caxecraft_domain_World_remove(")
    collection_index = mining_section.find("hxc_caxecraft_gameplay_Inventory_collectBlock(")
    if (
        mining_start < 0
        or min(capacity_index, removal_index, collection_index) < 0
        or not capacity_index < removal_index < collection_index
        or ".hxc_outcome = 1" not in mining_section
        or ".hxc_outcome = 2" not in mining_section
    ):
        raise PlayFailure(
            "generated Caxecraft mining must check capacity before removal, collect after removal, and retain closed full/collected outcomes"
        )
    # Eight reviewed image owners enter the application, are checked before
    # use, and leave in reverse order before CloseWindow. Exact counts make a
    # missing unload or an accidental hidden resource registry fail locally.
    for function_name, expected_count in (
        ("LoadTexture", 8),
        ("IsTextureValid", 8),
        ("UnloadTexture", 8),
    ):
        actual_count = app.count(f"{function_name}(")
        if actual_count != expected_count:
            raise PlayFailure(
                f"generated Caxecraft app contains {actual_count} direct {function_name} call sites; expected {expected_count}"
            )
    draw_texture_count = combined.count("DrawTexturePro(")
    # Title, wordmark, hotbar frame, item, and health-glyph helpers own the
    # original five sites. The equipped-item badge adds one reviewed branch for
    # each admitted item atlas. Runtime loops still reuse those fixed helpers.
    if draw_texture_count != 7:
        raise PlayFailure(
            f"generated Caxecraft sources contain {draw_texture_count} direct DrawTexturePro call sites; expected 7"
        )
    billboard_count = combined.count("DrawBillboardRec(")
    # Actors use one entity-atlas borrow. Authored world items add one branch
    # for each admitted item atlas; all three remain typed by-value raylib calls.
    if billboard_count != 3:
        raise PlayFailure(
            f"generated Caxecraft sources contain {billboard_count} direct DrawBillboardRec call sites; expected three typed atlas borrows"
        )
    for forbidden in (r"\bgoto\b", r"\bmalloc\s*\(", r"\bcalloc\s*\(", r"\brealloc\s*\(", r"\bfree\s*\(", r"\bhxrt_"):
        if re.search(forbidden, combined):
            raise PlayFailure(f"generated Caxecraft sources contain forbidden pattern {forbidden}")


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-playable-snapshot-") as temporary:
        generated = Path(temporary) / "generated"
        compile_haxe(
            generated,
            layout="split",
            platform_name=SNAPSHOT_PLATFORM,
            raylib_configuration="desktop",
        )
        values: dict[str, object] = {}
        for name, format_name in PLAYABLE_SNAPSHOT_FORMATS.items():
            relative = name.removeprefix("playable/")
            path = generated / relative
            values[name] = load_object(path, f"playable snapshot {name}") if format_name == "json" else path.read_text(encoding="utf-8")
        return values


def check_snapshots() -> None:
    actual = snapshot_values()
    for name, format_name in PLAYABLE_SNAPSHOT_FORMATS.items():
        expected_path = EXPECTED / name
        if not expected_path.is_file():
            raise PlayFailure(
                "playable snapshot is missing; run "
                "`npm run snapshots:update -- --suite caxecraft-domain`"
            )
        expected: object = load_object(expected_path, f"expected {name}") if format_name == "json" else expected_path.read_text(encoding="utf-8")
        if actual[name] == expected:
            continue
        detail = "semantic JSON differs"
        if isinstance(actual[name], str) and isinstance(expected, str):
            detail = "".join(
                difflib.unified_diff(
                    expected.splitlines(keepends=True),
                    actual[name].splitlines(keepends=True),
                    fromfile=f"expected/{name}",
                    tofile=f"actual/{name}",
                    n=3,
                )
            )[:4000]
        raise PlayFailure(
            f"playable snapshot {name!r} drifted; run "
            "`npm run snapshots:update -- --suite caxecraft-domain`\n"
            f"{detail}"
        )


def raylib_cache_key(
    *,
    authority: str,
    platform_name: str,
    raylib_configuration: str,
    source: Path | None,
    cc: str,
    cxx: str,
    cmake: str,
    generator: str,
) -> str:
    lock = provision.load_lock()
    identity = {
        "schemaVersion": 1,
        "authority": authority,
        "configuration": raylib_configuration,
        "platform": platform_name,
        "source": str(source.resolve()) if source is not None else None,
        "cc": cc,
        "ccVersion": tool_version(cc),
        "cxx": cxx,
        "cxxVersion": tool_version(cxx),
        "cmake": cmake,
        "cmakeVersion": tool_version(cmake),
        "generator": generator,
        "raylibCommit": provision.PINNED_COMMIT,
        "patches": provision.patch_lock_identity(
            lock, platform_name, raylib_configuration
        ),
        # The backend and every other reviewed CMake choice are build inputs.
        # Including them prevents a lock change from reusing a native library
        # that was compiled under an older configuration.
        "cmakeDefinitions": list(
            provision.configuration_definitions(lock, platform_name, raylib_configuration)
        ),
    }
    encoded = json.dumps(identity, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8")
    return hashlib.sha256(encoded).hexdigest()[:16]


def provision_raylib(
    *,
    authority: str,
    platform_name: str,
    raylib_configuration: str,
    source: Path | None,
    cache_root: Path,
    cc: str,
    cxx: str,
    cmake: str,
    generator: str,
    allow_network: bool,
    rebuild: bool,
) -> tuple[Path, Path]:
    lock = provision.load_lock()
    key = raylib_cache_key(
        authority=authority,
        platform_name=platform_name,
        raylib_configuration=raylib_configuration,
        source=source,
        cc=cc,
        cxx=cxx,
        cmake=cmake,
        generator=generator,
    )
    source_cache = cache_root / "source"
    build_root = cache_root / "build" / key
    state_path = build_root / "hxc-caxecraft-raylib.json"
    if rebuild and build_root.exists():
        if not state_path.is_file() or state_path.is_symlink():
            raise PlayFailure(f"cannot rebuild an unowned Raylib cache directory: {build_root}")
        state = load_object(state_path, "Caxecraft Raylib cache state")
        if state.get("schemaVersion") != 1 or state.get("raylibCommit") != provision.PINNED_COMMIT:
            raise PlayFailure(f"cannot rebuild an invalid Raylib cache directory: {build_root}")
        shutil.rmtree(build_root)

    if state_path.is_file() and not state_path.is_symlink():
        state = load_object(state_path, "Caxecraft Raylib cache state")
        if authority == "pinned-source":
            source_root = provision.pinned_source(source_cache, lock, allow_network=False)
        else:
            if source is None:
                raise PlayFailure("offline-source authority requires --source")
            source_root = source.resolve()
            provision.verify_source(source_root, lock)
        library = provision.locate_raylib_library(build_root, platform_name)
        expected_patches = provision.patch_lock_identity(
            lock, platform_name, raylib_configuration
        )
        include_directory = (
            build_root / "hxc-patched-source/src"
            if expected_patches
            else source_root / "src"
        )
        provisioning_report = state.get("provisioningReport")
        if (
            state.get("schemaVersion") != 1
            or state.get("raylibCommit") != provision.PINNED_COMMIT
            or state.get("librarySha256") != provision.sha256_file(library)
            or not isinstance(provisioning_report, dict)
            or provisioning_report.get("configuration", {}).get("id") != raylib_configuration
            or provision.patch_report_identity(provisioning_report) != expected_patches
            or not include_directory.is_dir()
        ):
            raise PlayFailure("cached Raylib build does not match its verified state; use --rebuild-raylib")
        return include_directory, library

    if build_root.exists():
        if any(build_root.iterdir()):
            raise PlayFailure(f"unowned files occupy the Raylib build cache: {build_root}")
    result = provision.build_source(
        lock=lock,
        authority=authority,
        configuration=raylib_configuration,
        platform_name=platform_name,
        cache_root=source_cache if authority == "pinned-source" else None,
        source_root=source if authority == "offline-source" else None,
        build_root=build_root,
        compiler=cc,
        cxx_compiler=cxx,
        cmake=cmake,
        generator=generator,
        allow_network=allow_network,
    )
    state = {
        "schemaVersion": 1,
        "raylibCommit": provision.PINNED_COMMIT,
        "librarySha256": provision.sha256_file(result.library_file),
        "provisioningReport": result.report,
    }
    state_path.write_text(json.dumps(state, ensure_ascii=False, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return result.include_directory, result.library_file


def provision_raygui(
    *,
    raylib_source: Path,
    source: Path | None,
    cache_root: Path,
    cc: str,
    archiver: str,
    allow_network: bool,
    rebuild: bool,
) -> tuple[Path, Path]:
    """Build or reuse the one pinned raygui implementation archive.

    Raygui is a header-only library, so merely adding its include directory is
    not enough: exactly one translation unit must define
    ``RAYGUI_IMPLEMENTATION``. The dedicated provisioner owns that unit. This
    small cache wrapper keeps interactive Caxecraft builds fast while rejecting
    an archive produced from another header, compiler, or archiver.
    """
    lock = raygui_provision.load_lock()
    if source is None:
        source_root = raygui_provision.pinned_source(cache_root / "source", lock, allow_network)
        source_authority = "pinned-source"
    else:
        source_root = raygui_provision.verify_raygui_source(source, lock)
        source_authority = "offline-source"
    raylib_source = raylib_source.resolve()
    raylib_header = raylib_source / "src/raylib.h"
    if raylib_header.is_symlink() or not raylib_header.is_file():
        raise PlayFailure(f"Raygui requires the verified Raylib header: {raylib_header}")
    identity = {
        "schemaVersion": 1,
        "authority": source_authority,
        "rayguiSourceTreeSha256": raygui_provision.PINNED_TREE[0],
        "raylibHeaderSha256": raygui_provision.sha256_file(raylib_header),
        "compiler": cc,
        "compilerVersion": tool_version(cc),
        "archiver": raygui_provision.tool_file_identity(archiver),
    }
    encoded = json.dumps(identity, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8")
    key = hashlib.sha256(encoded).hexdigest()[:16]
    build_root = cache_root.resolve() / "build" / key
    state_path = build_root / "hxc-caxecraft-raygui.json"
    if rebuild and build_root.exists():
        if not state_path.is_file() or state_path.is_symlink():
            raise PlayFailure(f"cannot rebuild an unowned Raygui cache directory: {build_root}")
        state = load_object(state_path, "Caxecraft Raygui cache state")
        if state.get("schemaVersion") != 1 or state.get("identity") != identity:
            raise PlayFailure(f"cannot rebuild an invalid Raygui cache directory: {build_root}")
        shutil.rmtree(build_root)

    if state_path.is_file() and not state_path.is_symlink():
        state = load_object(state_path, "Caxecraft Raygui cache state")
        library = build_root / "libraygui.a"
        report = state.get("provisioningReport")
        output = report.get("output") if isinstance(report, dict) else None
        if (
            state.get("schemaVersion") != 1
            or state.get("identity") != identity
            or not library.is_file()
            or library.is_symlink()
            or not isinstance(output, dict)
            or output.get("librarySha256") != raygui_provision.sha256_file(library)
        ):
            raise PlayFailure("cached Raygui build does not match its verified state; use --rebuild-raygui")
        return source_root / "src", library

    if build_root.exists() and (build_root.is_symlink() or not build_root.is_dir() or any(build_root.iterdir())):
        raise PlayFailure(f"unowned files occupy the Raygui build cache: {build_root}")
    result = raygui_provision.build_static(
        source_root=source_root,
        raylib_source=raylib_source,
        build_root=build_root,
        compiler=cc,
        archiver=archiver,
    )
    state = {
        "schemaVersion": 1,
        "identity": identity,
        "provisioningReport": result.report,
    }
    state_path.write_text(json.dumps(state, ensure_ascii=False, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    return result.include_directory, result.library_file


def resolve_prebuilt_raylib(
    *,
    cache_root: Path,
    build_root: Path,
    report_path: Path,
    platform_name: str,
    raylib_configuration: str,
) -> tuple[Path, Path]:
    lock = provision.load_lock()
    source_root = provision.pinned_source(cache_root.resolve(), lock, allow_network=False)
    library = provision.locate_raylib_library(build_root.resolve(), platform_name)
    integration_report = load_object(report_path, "prebuilt Raylib integration report")
    report = integration_report.get("provision")
    if not isinstance(report, dict):
        raise PlayFailure("prebuilt Raylib integration report omitted its provisioning record")
    target = report.get("target")
    configuration = report.get("configuration")
    outputs = report.get("outputs")
    claims = report.get("claims")
    library_report = outputs.get("library") if isinstance(outputs, dict) else None
    expected_patches = provision.patch_lock_identity(
        lock, platform_name, raylib_configuration
    )
    include_directory = (
        build_root.resolve() / "hxc-patched-source/src"
        if expected_patches
        else source_root / "src"
    )
    if (
        report.get("authority") != "pinned-source"
        or not isinstance(target, dict)
        or target.get("platform") != platform_name
        or not isinstance(configuration, dict)
        or configuration.get("id") != raylib_configuration
        or not isinstance(claims, dict)
        or claims.get("raylibBuilt") is not True
        or not isinstance(library_report, dict)
        or library_report.get("sha256") != provision.sha256_file(library)
        or provision.patch_report_identity(report) != expected_patches
        or not include_directory.is_dir()
    ):
        raise PlayFailure(
            "prebuilt Raylib inputs do not match the pinned "
            f"{raylib_configuration} provisioning report"
        )
    return include_directory, library


def compile_native(
    generated: Path,
    manifest: dict[str, object],
    *,
    output: Path,
    include_directory: Path,
    library: Path,
    raygui_include_directory: Path,
    raygui_library: Path,
    platform_name: str,
    raylib_configuration: str,
    cc: str,
    optimization: str,
    native_sanitizer_flags: tuple[str, ...],
) -> None:
    if platform_name == "windows":
        raise PlayFailure("the one-command Windows linker adapter is deferred; generated C remains available with --compile-only")
    build = manifest.get("build")
    if not isinstance(build, dict):
        raise PlayFailure("generated Caxecraft manifest omitted its build plan")
    source_values = text_array(build.get("sources"), "generated Caxecraft sources")
    object_root = output.parent / "obj"
    if object_root.exists():
        shutil.rmtree(object_root)
    object_root.mkdir(parents=True)
    objects: list[Path] = []
    for index, source_value in enumerate(source_values):
        relative = validated_relative(source_value, f"generated source {index}")
        source_path = generated.joinpath(*relative.parts)
        if not source_path.is_file():
            raise PlayFailure(f"generated source is missing: {source_value}")
        object_path = object_root / f"{index:03d}.o"
        compile_arguments = [
            cc,
            *STRICT_FLAGS,
            f"-O{optimization}",
            *native_sanitizer_flags,
            "-I",
            str(generated / "include"),
            "-I",
            str(include_directory),
            "-I",
            str(raygui_include_directory),
        ]
        compile_arguments.extend(
            [
                "-c",
                str(source_path),
                "-o",
                str(object_path),
            ]
        )
        run(
            compile_arguments,
            cwd=ROOT,
            timeout=180,
            label=f"native compile of {source_value}",
        )
        objects.append(object_path)

    lock = provision.load_lock()
    libraries, frameworks = provision.link_facts(lock, platform_name, raylib_configuration)
    manifest_libraries = owned_fact_names(build.get("libraries"), "generated Caxecraft libraries")
    manifest_frameworks = owned_fact_names(build.get("frameworks"), "generated Caxecraft frameworks")
    expected_libraries = list(libraries)
    if "raygui" not in expected_libraries:
        expected_libraries.append("raygui")
    if len(manifest_libraries) != len(expected_libraries) or set(manifest_libraries) != set(expected_libraries):
        raise PlayFailure("generated Caxecraft libraries differ from the pinned Raylib + Raygui link plan")
    if len(manifest_frameworks) != len(frameworks) or set(manifest_frameworks) != set(frameworks):
        raise PlayFailure("generated Caxecraft frameworks differ from the pinned Raylib link plan")
    # Static-link order is significant: generated code needs raygui, and the
    # raygui implementation in turn needs raylib. System libraries follow both.
    arguments = [cc, *native_sanitizer_flags, *[str(path) for path in objects], str(raygui_library), str(library)]
    for name in expected_libraries:
        if name not in ("raylib", "raygui"):
            arguments.append(f"-l{name}")
    for name in frameworks:
        arguments.extend(["-framework", name])
    output.parent.mkdir(parents=True, exist_ok=True)
    arguments.extend(["-o", str(output)])
    run(arguments, cwd=ROOT, timeout=180, label="Caxecraft native link")


def parse_args(argv: list[str]) -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check-snapshots", action="store_true", help="compile once and compare the registered playable snapshots")
    parser.add_argument("--compile-only", action="store_true", help="emit and validate C without provisioning or linking Raylib")
    parser.add_argument("--build-only", action="store_true", help="link the native executable without opening a window")
    parser.add_argument("--smoke", action="store_true", help="render three real frames, require timely exit, and do not wait for input")
    parser.add_argument(
        "--pilot",
        choices=(
            "launch-smoke",
            "secondary-locale",
            "move-jump-edit",
            "pause-recapture",
            "combat-drop",
            "recovery-use",
            "full-inventory-gift",
            "full-inventory-mining",
            "resize-layout",
            "aquatic-gear",
            "smooth-motion",
        ),
        help="run one deterministic in-process input script, capture its visual checkpoint, and quit",
    )
    parser.add_argument("--allow-network", action="store_true", help="allow the first checksum-pinned Raylib and Raygui archive downloads")
    parser.add_argument("--authority", choices=("pinned-source", "offline-source"), default="pinned-source")
    parser.add_argument("--source", type=Path, help="exact Raylib 6.0 source tree for offline-source authority")
    parser.add_argument("--raygui-source", type=Path, help="exact Raygui 5.0 source tree; otherwise use the pinned archive cache")
    parser.add_argument("--layout", choices=("split", "package", "unity"), default="split")
    parser.add_argument(
        "--renderer",
        choices=("chunk-cache", "immediate-baseline"),
        default="chunk-cache",
        help="select the shipped cache or the benchmark-only former whole-world scan",
    )
    parser.add_argument(
        "--benchmark-renderer",
        action="store_true",
        help="measure post-warmup terrain and update work in the move-jump-edit pilot",
    )
    parser.add_argument(
        "--raylib-configuration",
        choices=RAYLIB_CONFIGURATIONS,
        default="desktop",
        help="use a real desktop window or Raylib's deterministic in-memory software renderer",
    )
    parser.add_argument("--optimization", choices=("0", "2"), default="2")
    parser.add_argument(
        "--sanitizers",
        action="store_true",
        help="instrument generated C with the reviewed Linux AddressSanitizer/UndefinedBehaviorSanitizer profile",
    )
    parser.add_argument("--cc", default=os.environ.get("CC", "clang" if platform.system() == "Darwin" else "gcc"))
    parser.add_argument("--cxx", default=os.environ.get("CXX", "clang++" if platform.system() == "Darwin" else "g++"))
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument("--generator", choices=("Ninja", "Unix Makefiles"), default="Ninja" if shutil.which("ninja") else "Unix Makefiles")
    parser.add_argument("--cache-root", type=Path, default=ROOT / ".cache/caxecraft/raylib")
    parser.add_argument("--raygui-cache-root", type=Path, default=ROOT / ".cache/caxecraft/raygui")
    parser.add_argument("--ar", default=os.environ.get("AR", "ar"))
    parser.add_argument("--output-root", type=Path, default=CASE / "_build/play")
    parser.add_argument("--rebuild-raylib", action="store_true")
    parser.add_argument("--rebuild-raygui", action="store_true")
    parser.add_argument("--prebuilt-raylib-cache", type=Path, help="verified pinned-source cache produced by the Raylib integration lane")
    parser.add_argument("--prebuilt-raylib-build", type=Path, help="verified build produced by the matching Raylib integration lane")
    parser.add_argument("--prebuilt-raylib-report", type=Path, help="normalized report for the verified prebuilt Raylib library")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    try:
        args = parse_args(argv)
        selected_pilot = "launch-smoke" if args.smoke else args.pilot
        if args.smoke and args.pilot is not None:
            raise PlayFailure("--smoke is the launch-smoke pilot alias and cannot be combined with --pilot")
        if (args.smoke or args.pilot is not None) and (args.check_snapshots or args.compile_only or args.build_only):
            raise PlayFailure("a running smoke/pilot cannot be combined with a non-running mode")
        if args.sanitizers and args.compile_only:
            raise PlayFailure("--sanitizers requires a native build and cannot be combined with --compile-only")
        if args.sanitizers and args.check_snapshots:
            raise PlayFailure("--sanitizers is a native execution profile and cannot be combined with --check-snapshots")
        if args.renderer == "immediate-baseline" and selected_pilot is None:
            raise PlayFailure("the immediate renderer is benchmark evidence and requires a bounded pilot")
        if args.benchmark_renderer and selected_pilot != "move-jump-edit":
            raise PlayFailure("--benchmark-renderer requires --pilot move-jump-edit")
        if args.check_snapshots:
            check_snapshots()
            print("caxecraft: playable snapshots and direct-C invariants passed")
            return 0
        platform_name = host_platform()
        if args.authority == "offline-source" and args.source is None:
            raise PlayFailure("--authority offline-source requires --source")
        if args.authority == "pinned-source" and args.source is not None:
            raise PlayFailure("--source is accepted only with --authority offline-source")
        validate_renderer_pilot(args.raylib_configuration, selected_pilot)
        native_sanitizer_flags = sanitizer_flags(args.cc, platform_name) if args.sanitizers else ()
        if (
            args.raylib_configuration == "memory-software"
            and selected_pilot is None
            and not (args.compile_only or args.build_only)
        ):
            raise PlayFailure(
                "memory-software has no interactive window; select --pilot, --smoke, --build-only, or --compile-only"
            )
        output_base = prepare_output_root(args.output_root.resolve())
        variants = output_base / "variants"
        if variants.exists() and (not variants.is_dir() or variants.is_symlink()):
            raise PlayFailure(f"Caxecraft variant root must be a real directory: {variants}")
        variants.mkdir(exist_ok=True)
        profile = "interactive" if selected_pilot is None else f"pilot-{selected_pilot}"
        renderer_part = "" if args.renderer == "chunk-cache" else "-immediate-baseline"
        benchmark_part = "-benchmark" if args.benchmark_renderer else ""
        configuration_part = "" if args.raylib_configuration == "desktop" else f"-{args.raylib_configuration}"
        sanitizer_part = "-sanitized" if args.sanitizers else ""
        output_root = prepare_output_root(
            variants / f"{platform_name}{configuration_part}-{args.layout}{sanitizer_part}-{profile}{renderer_part}{benchmark_part}"
        )
        generated = output_root / "generated"
        executable = output_root / "bin" / ("caxecraft.exe" if platform_name == "windows" else "caxecraft")
        manifest = compile_haxe(
            generated,
            layout=args.layout,
            platform_name=platform_name,
            raylib_configuration=args.raylib_configuration,
            pilot=selected_pilot,
            renderer=args.renderer,
            benchmark_renderer=args.benchmark_renderer,
        )
        print(f"caxecraft: generated {args.layout} C project at {generated}")
        if args.compile_only:
            print("caxecraft: compile-only proof passed (direct C, empty hxrt plan)")
            return 0

        prebuilt_values = (args.prebuilt_raylib_cache, args.prebuilt_raylib_build, args.prebuilt_raylib_report)
        if any(value is not None for value in prebuilt_values):
            if not all(value is not None for value in prebuilt_values):
                raise PlayFailure("prebuilt Raylib reuse requires cache, build, and report paths together")
            if args.source is not None or args.rebuild_raylib:
                raise PlayFailure("prebuilt Raylib reuse rejects Raylib source and rebuild options")
            include_directory, library = resolve_prebuilt_raylib(
                cache_root=args.prebuilt_raylib_cache,
                build_root=args.prebuilt_raylib_build,
                report_path=args.prebuilt_raylib_report,
                platform_name=platform_name,
                raylib_configuration=args.raylib_configuration,
            )
            raylib_source = provision.pinned_source(
                args.prebuilt_raylib_cache.resolve(), provision.load_lock(), allow_network=False
            )
        else:
            include_directory, library = provision_raylib(
                authority=args.authority,
                platform_name=platform_name,
                raylib_configuration=args.raylib_configuration,
                source=args.source,
                cache_root=args.cache_root.resolve(),
                cc=args.cc,
                cxx=args.cxx,
                cmake=args.cmake,
                generator=args.generator,
                allow_network=args.allow_network,
                rebuild=args.rebuild_raylib,
            )
            if args.authority == "pinned-source":
                raylib_source = provision.pinned_source(
                    args.cache_root.resolve() / "source", provision.load_lock(), allow_network=False
                )
            else:
                if args.source is None:
                    raise PlayFailure("offline Raylib authority lost its verified source")
                raylib_source = args.source.resolve()
        raygui_include_directory, raygui_library = provision_raygui(
            raylib_source=raylib_source,
            source=args.raygui_source,
            cache_root=args.raygui_cache_root,
            cc=args.cc,
            archiver=args.ar,
            allow_network=args.allow_network,
            rebuild=args.rebuild_raygui,
        )
        compile_native(
            generated,
            manifest,
            output=executable,
            include_directory=include_directory,
            library=library,
            raygui_include_directory=raygui_include_directory,
            raygui_library=raygui_library,
            platform_name=platform_name,
            raylib_configuration=args.raylib_configuration,
            cc=args.cc,
            optimization=args.optimization,
            native_sanitizer_flags=native_sanitizer_flags,
        )
        stage_runtime_assets(executable.parent)
        stage_content_catalogs(executable.parent)
        print(f"caxecraft: built native executable at {executable}")
        if args.build_only:
            return 0
        if selected_pilot is not None:
            screenshot_names = {
                "launch-smoke": "caxecraft-smoke.png",
                "secondary-locale": "caxecraft-secondary-locale.png",
                "move-jump-edit": "caxecraft-pilot-move.png",
                "pause-recapture": "caxecraft-pilot-pause.png",
                "combat-drop": "caxecraft-pilot-combat.png",
                "recovery-use": "caxecraft-pilot-recovery.png",
                "full-inventory-gift": "caxecraft-pilot-full-inventory.png",
                "full-inventory-mining": "caxecraft-pilot-full-mining.png",
                "resize-layout": "caxecraft-pilot-resize.png",
                "aquatic-gear": "caxecraft-pilot-aquatic-gear.png",
                "smooth-motion": "caxecraft-pilot-smooth-motion.png",
            }
            screenshot = executable.parent / screenshot_names[selected_pilot]
            state_screenshot = executable.parent / "caxecraft-pilot-state.png"
            reports: list[dict[str, object]] = []
            screenshot_hashes: list[str] = []
            compiler_version = tool_version(args.cc)
            width = 0
            height = 0
            repetitions = 7 if args.benchmark_renderer else 2
            for repeat in range(repetitions):
                for stale in (screenshot, state_screenshot):
                    if stale.exists():
                        stale.unlink()
                run(
                    [str(executable)],
                    cwd=executable.parent,
                    timeout=15,
                    label=f"Caxecraft {selected_pilot} graphical pilot run {repeat + 1}",
                )
                if args.raylib_configuration == "memory-software":
                    # Raylib 6.0's software readback has a documented, focused
                    # conversion above. Normalize both captures before visual
                    # checks and before reading the bottom-edge telemetry strip.
                    normalize_memory_software_capture(screenshot)
                    normalize_memory_software_capture(state_screenshot)
                if selected_pilot in ("launch-smoke", "secondary-locale"):
                    width, height = validate_smoke_screenshot(screenshot, platform_name=platform_name)
                elif selected_pilot == "resize-layout":
                    width, height = validate_smoke_screenshot(
                        screenshot,
                        platform_name=platform_name,
                        expected_logical_size=(960, 540),
                    )
                else:
                    width, height = validate_presented_screenshot(
                        screenshot,
                        platform_name=platform_name,
                        expected_drop=selected_pilot == "combat-drop",
                        expected_attack=selected_pilot == "combat-drop",
                        expected_recovery=selected_pilot == "recovery-use",
                        expected_inventory_full=selected_pilot in ("full-inventory-gift", "full-inventory-mining"),
                        expected_entities=selected_pilot == "full-inventory-gift",
                        expected_open_sky=selected_pilot != "move-jump-edit",
                    )
                reports.append(
                    build_pilot_report(
                        state_path=state_screenshot,
                        review_screenshot=screenshot,
                        pilot=selected_pilot,
                        platform_name=platform_name,
                        raylib_configuration=args.raylib_configuration,
                        renderer=args.renderer,
                        benchmark_renderer=args.benchmark_renderer,
                        sanitizers=args.sanitizers,
                        cc=args.cc,
                        compiler_version=compiler_version,
                    )
                )
                screenshot_hashes.append(hashlib.sha256(screenshot.read_bytes()).hexdigest())

            semantic_reports: list[dict[str, object]] = []
            for report in reports:
                comparable = dict(report)
                comparable.pop("benchmarkSample", None)
                semantic_reports.append(comparable)
            for index, report in enumerate(semantic_reports[1:], start=2):
                if semantic_reports[0] != report:
                    first = json.dumps(semantic_reports[0], ensure_ascii=False, indent=2, sort_keys=True).splitlines()
                    other = json.dumps(report, ensure_ascii=False, indent=2, sort_keys=True).splitlines()
                    difference = "\n".join(
                        difflib.unified_diff(first, other, fromfile="pilot-run-1", tofile=f"pilot-run-{index}", lineterm="")
                    )
                    raise PlayFailure(f"repeated native pilot semantic evidence drifted:\n{difference}")
            if len(set(screenshot_hashes)) != 1:
                raise PlayFailure("repeated native pilot review screenshots differed byte-for-byte")

            final_report = reports[0]
            if args.benchmark_renderer:
                samples = [report.pop("benchmarkSample") for report in reports]
                terrain = [sample["terrainMicroseconds"] for sample in samples]
                update = [sample["updateMicroseconds"] for sample in samples]
                preparation = [sample["preparationMicroseconds"] for sample in samples]
                measured_frames = samples[0]["measuredFrames"]
                if any(sample["measuredFrames"] != measured_frames for sample in samples):
                    raise PlayFailure("renderer benchmark samples used different measured frame counts")
                terrain_median = statistics.median(terrain)
                update_median = statistics.median(update)
                preparation_median = statistics.median(preparation)
                final_report["benchmark"] = {
                    "clock": "raylib-monotonic-time",
                    "warmupFrames": 2,
                    "sampleCount": repetitions,
                    "measuredFramesPerSample": measured_frames,
                    "terrainMicroseconds": terrain,
                    "terrainMedianMicroseconds": terrain_median,
                    "terrainMedianMicrosecondsPerFrame": terrain_median / measured_frames,
                    "updateMicroseconds": update,
                    "updateMedianMicroseconds": update_median,
                    "updateMedianMicrosecondsPerFrame": update_median / measured_frames,
                    "preparationMicroseconds": preparation,
                    "preparationMedianMicroseconds": preparation_median,
                    "preparationMedianMicrosecondsPerFrame": preparation_median / measured_frames,
                    "reviewScreenshotSha256": screenshot_hashes[0],
                }
            report_path = executable.parent / "caxecraft-pilot-report.json"
            report_path.write_text(
                json.dumps(final_report, ensure_ascii=False, indent=2, sort_keys=True) + "\n",
                encoding="utf-8",
                newline="\n",
            )
            state_screenshot.unlink()
            print(
                f"caxecraft: {selected_pilot} graphical pilot passed "
                f"({width}x{height} presented framebuffer, {repetitions} identical semantic runs, "
                f"and bounded exit within 15 seconds; report {report_path})"
            )
            return 0
        print("caxecraft: launching; press Q to quit")
        return subprocess.run([str(executable)], cwd=executable.parent, check=False).returncode
    except (OSError, UnicodeError, provision.ProvisionFailure, PlayFailure) as error:
        print(f"caxecraft: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
