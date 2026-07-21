#!/usr/bin/env python3
"""Build and launch the first native Caxecraft playable slice."""

from __future__ import annotations

import argparse
import difflib
import hashlib
import json
import os
import platform
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import zlib
from pathlib import Path, PurePosixPath


ROOT = Path(__file__).resolve().parents[2]
CASE = Path(__file__).resolve().parent
PROVISION_DIR = ROOT / "scripts/raylib"
sys.path.insert(0, str(PROVISION_DIR))
import provision  # type: ignore  # noqa: E402


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
PLATFORM_NAMES = {"Darwin": "macos", "Linux": "linux", "Windows": "windows"}
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
    "playable/include/hxc/modules/caxecraft/gameplay/InventoryState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/ItemKind.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/GuideState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/MosslingState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/BerryDropState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/PlayerVitalsState.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/MiningResult.h": "header",
    "playable/include/hxc/modules/caxecraft/gameplay/Mining.h": "header",
    "playable/src/modules/caxecraft/app/CaxecraftAtlas.c": "c",
    "playable/src/modules/caxecraft/app/CaxecraftPalette.c": "c",
    "playable/src/modules/caxecraft/app/HudDigits.c": "c",
    "playable/src/modules/caxecraft/app/Main.c": "c",
    "playable/src/modules/caxecraft/gameplay/Inventory.c": "c",
    "playable/src/modules/caxecraft/gameplay/GuideNpc.c": "c",
    "playable/src/modules/caxecraft/gameplay/Mossling.c": "c",
    "playable/src/modules/caxecraft/gameplay/BerryDrop.c": "c",
    "playable/src/modules/caxecraft/gameplay/PlayerVitals.c": "c",
    "playable/src/modules/caxecraft/gameplay/Recovery.c": "c",
    "playable/src/modules/caxecraft/gameplay/Mining.c": "c",
    "playable/src/modules/caxecraft/domain/World.c": "c",
}
RUNTIME_ASSET_IDS = ("caxecraft-wordmark", "title-panorama", "hud", "items")
RUNTIME_ASSET_REPORT = "caxecraft-runtime-assets.json"


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
            raise PlayFailure(f"unowned files occupy the Caxecraft staged asset root: {unexpected}")
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


def development_tool(name: str) -> str:
    local_name = f"{name}.cmd" if os.name == "nt" else name
    local = ROOT / "node_modules/.bin" / local_name
    return str(local) if local.is_file() else name


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


def validate_smoke_screenshot(
    path: Path,
    *,
    platform_name: str,
    expected_title: bool = True,
    expected_drop: bool = False,
    expected_attack: bool = False,
    expected_recovery: bool = False,
    expected_inventory_full: bool = False,
) -> tuple[int, int]:
    """Prove the captured title frame contains staged art and readable UI."""
    try:
        data = path.read_bytes()
    except OSError as error:
        raise PlayFailure(f"Caxecraft smoke did not produce its framebuffer screenshot: {error}") from error
    if len(data) < 33 or data[:8] != b"\x89PNG\r\n\x1a\n" or data[12:16] != b"IHDR":
        raise PlayFailure("Caxecraft smoke screenshot is not a structurally valid PNG")
    width, height = struct.unpack(">II", data[16:24])
    expected_dimensions = {(1280, 720)}
    if platform_name == "macos":
        expected_dimensions.add((2560, 1440))
    if (width, height) not in expected_dimensions:
        raise PlayFailure(
            f"Caxecraft smoke screenshot must match its logical 1280x720 window at an admitted pixel scale, "
            f"found {width}x{height}; "
            "this can indicate a broken high-DPI framebuffer"
        )
    if data[24:29] != bytes((8, 6, 0, 0, 0)):
        raise PlayFailure("Caxecraft smoke screenshot must be a non-interlaced 8-bit RGBA PNG")

    compressed = bytearray()
    offset = 8
    saw_end = False
    while offset < len(data):
        if offset + 12 > len(data):
            raise PlayFailure("Caxecraft smoke screenshot contains a truncated PNG chunk")
        length = struct.unpack(">I", data[offset : offset + 4])[0]
        chunk_end = offset + 12 + length
        if chunk_end > len(data):
            raise PlayFailure("Caxecraft smoke screenshot contains an out-of-bounds PNG chunk")
        chunk_type = data[offset + 4 : offset + 8]
        payload = data[offset + 8 : offset + 8 + length]
        expected_crc = struct.unpack(">I", data[offset + 8 + length : chunk_end])[0]
        if zlib.crc32(chunk_type + payload) & 0xFFFFFFFF != expected_crc:
            raise PlayFailure("Caxecraft smoke screenshot contains a PNG checksum mismatch")
        if chunk_type == b"IDAT":
            compressed.extend(payload)
        if chunk_type == b"IEND":
            saw_end = True
            break
        offset = chunk_end
    if not compressed or not saw_end:
        raise PlayFailure("Caxecraft smoke screenshot omitted PNG image data or its end marker")

    try:
        filtered = zlib.decompress(bytes(compressed))
    except zlib.error as error:
        raise PlayFailure(f"Caxecraft smoke screenshot contains invalid compressed pixels: {error}") from error
    bytes_per_pixel = 4
    stride = width * bytes_per_pixel
    expected_size = height * (stride + 1)
    if len(filtered) != expected_size:
        raise PlayFailure("Caxecraft smoke screenshot pixel payload has the wrong size")

    previous = bytearray(stride)
    brand_cyan_pixels = 0
    brand_orange_pixels = 0
    brand_white_pixels = 0
    warm_scene_pixels = 0
    green_scene_pixels = 0
    sky_scene_pixels = 0
    dark_panel_pixels = 0
    light_ui_pixels = 0
    nia_pixels = 0
    mossling_pixels = 0
    berry_pixels = 0
    recovery_pixels = 0
    inventory_full_pixels = 0
    damage_pixels = 0
    non_dark_pixels = 0
    quantized_colors: set[int] = set()
    at = 0
    for row in range(height):
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
                raise PlayFailure(f"Caxecraft smoke screenshot uses unknown PNG filter {filter_kind}")
            decoded[index] = (value + predictor) & 0xFF
        for index in range(0, stride, bytes_per_pixel):
            red, green, blue = decoded[index : index + 3]
            quantized_colors.add((red >> 4) << 8 | (green >> 4) << 4 | (blue >> 4))
            if red > 20 or green > 20 or blue > 20:
                non_dark_pixels += 1
            if red > 80 and green > 60 and red > blue * 1.4 and green > blue * 1.2:
                warm_scene_pixels += 1
            if green > red * 0.9 and green > blue * 1.2 and green > 50:
                green_scene_pixels += 1
            if 80 < red < 180 and 130 < green < 220 and 140 < blue < 235:
                sky_scene_pixels += 1
            if red < 45 and green < 55 and blue < 65:
                dark_panel_pixels += 1
            if red > 210 and green > 210 and blue > 200:
                light_ui_pixels += 1
            if (red, green, blue) == (42, 150, 160):
                nia_pixels += 1
            if (red, green, blue) == (157, 190, 82):
                mossling_pixels += 1
            if (red, green, blue) == (174, 78, 136):
                berry_pixels += 1
            if (red, green, blue) == (94, 212, 136):
                recovery_pixels += 1
            if (red, green, blue) == (238, 113, 78):
                inventory_full_pixels += 1
            if (red, green, blue) == (218, 65, 72):
                damage_pixels += 1
            column = index // bytes_per_pixel
            if row < height // 4 and width * 3 // 10 <= column < width * 7 // 10:
                if red < 80 and green > 120 and blue > 140:
                    brand_cyan_pixels += 1
                if red > 180 and 60 < green < 190 and blue < 100:
                    brand_orange_pixels += 1
                if red > 220 and green > 220 and blue > 210:
                    brand_white_pixels += 1
        previous = decoded
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
    # The first playable renderer deliberately uses a small flat-color palette;
    # color-count thresholds would confuse clean voxel shading with a blank
    # frame. Instead prove the scene's independent visual roles: broad sky,
    # terrain, a dark heads-up-display panel, and light text/crosshair pixels.
    missing_actor_evidence = berry_pixels < 20 if expected_drop else mossling_pixels < 30
    if not expected_title and (
        non_dark_pixels < width * height // 3
        # A fifth of the complete framebuffer is still a broad independent sky
        # region after opaque HUD panels, actors, and text cover scene pixels.
        or sky_scene_pixels < width * height // 5
        or green_scene_pixels < width * height // 20
        or dark_panel_pixels < 2_000
        or light_ui_pixels < 250
        or nia_pixels < 50
        or missing_actor_evidence
    ):
        raise PlayFailure(
            "Caxecraft pilot framebuffer is blank or lacks a presented game scene "
            f"(nonDark={non_dark_pixels}, sky={sky_scene_pixels}, green={green_scene_pixels}, "
            f"darkPanel={dark_panel_pixels}, lightUi={light_ui_pixels}, nia={nia_pixels}, "
            f"mossling={mossling_pixels}, berries={berry_pixels}, recovery={recovery_pixels}, "
            f"inventoryFull={inventory_full_pixels}, damage={damage_pixels}, colorBuckets={len(quantized_colors)})"
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


def validate_presented_screenshot(
    path: Path,
    *,
    platform_name: str,
    expected_drop: bool = False,
    expected_attack: bool = False,
    expected_recovery: bool = False,
    expected_inventory_full: bool = False,
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


def compile_haxe(generated: Path, *, layout: str, platform_name: str, pilot: str | None = None) -> dict[str, object]:
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
        "raylib_configuration_desktop",
    ]
    if layout != "split":
        arguments.extend(["-D", f"hxc_project_layout={layout}"])
    if pilot is not None:
        pilot_defines = {
            "launch-smoke": "caxecraft_pilot_launch_smoke",
            "move-jump-edit": "caxecraft_pilot_move_jump_edit",
            "pause-recapture": "caxecraft_pilot_pause_recapture",
            "combat-drop": "caxecraft_pilot_combat_drop",
            "recovery-use": "caxecraft_pilot_recovery_use",
            "full-inventory-gift": "caxecraft_pilot_full_inventory_gift",
            "full-inventory-mining": "caxecraft_pilot_full_inventory_mining",
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
    validate_generated_playable(generated, layout=layout, pilot=pilot)
    return manifest


def validate_generated_playable(generated: Path, *, layout: str, pilot: str | None) -> None:
    sources = sorted(generated.glob("src/**/*.c"), key=lambda path: path.as_posix().encode("utf-8"))
    if not sources:
        raise PlayFailure("Caxecraft emitted no C sources")
    combined = "\n".join(path.read_text(encoding="utf-8") for path in sources)
    app_relative = {
        "split": "src/modules/caxecraft/app/Main.c",
        "package": "src/packages/caxecraft/app/package.c",
        "unity": "src/program.c",
    }.get(layout)
    if app_relative is None:
        raise PlayFailure(f"unknown generated Caxecraft layout {layout!r}")
    app_path = generated / app_relative
    if not app_path.is_file():
        raise PlayFailure(f"generated Caxecraft {layout} app source is missing: {app_relative}")
    app = app_path.read_text(encoding="utf-8")
    for required in (
        "InitWindow(",
        "WindowShouldClose(",
        "BeginDrawing(",
        "BeginMode3D(",
        "DrawCube(",
        "DrawCubeWires(",
        "DrawText(",
    ):
        if required not in app:
            raise PlayFailure(f"generated Caxecraft app omitted direct Raylib call {required}")
    # Pilot builds replace live keyboard and mouse sampling with a deterministic
    # in-process input provider. Requiring GetMouseDelta there would reject the
    # exact dead-code removal that makes the two providers a clean compile-time
    # choice. Normal playable builds must still prove the real input path.
    if pilot is None and "GetMouseDelta(" not in app:
        raise PlayFailure("generated Caxecraft app omitted direct Raylib call GetMouseDelta(")
    # This first slice draws wire geometry only for the block under the
    # crosshair. A second call site would permit per-block fill/wire switching,
    # which previously made raylib flush thousands of tiny GPU batches before
    # the first frame could appear.
    if app.count("DrawCubeWires(") != 1:
        raise PlayFailure("generated Caxecraft app must contain exactly one selected-block wire-outline call site")
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
    # Four reviewed image owners enter the application, are checked before
    # use, and leave in reverse order before CloseWindow. Exact counts make a
    # missing unload or an accidental hidden resource registry fail locally.
    for function_name, expected_count in (
        ("LoadTexture", 4),
        ("IsTextureValid", 4),
        ("UnloadTexture", 4),
    ):
        actual_count = app.count(f"{function_name}(")
        if actual_count != expected_count:
            raise PlayFailure(
                f"generated Caxecraft app contains {actual_count} direct {function_name} call sites; expected {expected_count}"
            )
    draw_texture_count = combined.count("DrawTexturePro(")
    # Title, wordmark, hotbar frame, item, and health-glyph helpers each own one
    # structural texture draw site. Runtime loops reuse those fixed helpers.
    if draw_texture_count != 5:
        raise PlayFailure(
            f"generated Caxecraft sources contain {draw_texture_count} direct DrawTexturePro call sites; expected 5"
        )
    for forbidden in (r"\bgoto\b", r"\bmalloc\s*\(", r"\bcalloc\s*\(", r"\brealloc\s*\(", r"\bfree\s*\(", r"\bhxrt_"):
        if re.search(forbidden, combined):
            raise PlayFailure(f"generated Caxecraft sources contain forbidden pattern {forbidden}")


def snapshot_values() -> dict[str, object]:
    with tempfile.TemporaryDirectory(prefix="hxc-caxecraft-playable-snapshot-") as temporary:
        generated = Path(temporary) / "generated"
        compile_haxe(generated, layout="split", platform_name=SNAPSHOT_PLATFORM)
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
    *, authority: str, platform_name: str, source: Path | None, cc: str, cxx: str, cmake: str, generator: str
) -> str:
    lock = provision.load_lock()
    identity = {
        "schemaVersion": 1,
        "authority": authority,
        "configuration": "desktop",
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
        # The backend and every other reviewed CMake choice are build inputs.
        # Including them prevents a lock change from reusing a native library
        # that was compiled under an older configuration.
        "cmakeDefinitions": list(provision.configuration_definitions(lock, platform_name, "desktop")),
    }
    encoded = json.dumps(identity, ensure_ascii=False, sort_keys=True, separators=(",", ":")).encode("utf-8")
    return hashlib.sha256(encoded).hexdigest()[:16]


def provision_raylib(
    *,
    authority: str,
    platform_name: str,
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
        if (
            state.get("schemaVersion") != 1
            or state.get("raylibCommit") != provision.PINNED_COMMIT
            or state.get("librarySha256") != provision.sha256_file(library)
        ):
            raise PlayFailure("cached Raylib build does not match its verified state; use --rebuild-raylib")
        return source_root / "src", library

    if build_root.exists():
        if any(build_root.iterdir()):
            raise PlayFailure(f"unowned files occupy the Raylib build cache: {build_root}")
    result = provision.build_source(
        lock=lock,
        authority=authority,
        configuration="desktop",
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


def resolve_prebuilt_raylib(*, cache_root: Path, build_root: Path, report_path: Path, platform_name: str) -> tuple[Path, Path]:
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
    if (
        report.get("authority") != "pinned-source"
        or not isinstance(target, dict)
        or target.get("platform") != platform_name
        or not isinstance(configuration, dict)
        or configuration.get("id") != "desktop"
        or not isinstance(claims, dict)
        or claims.get("raylibBuilt") is not True
        or not isinstance(library_report, dict)
        or library_report.get("sha256") != provision.sha256_file(library)
    ):
        raise PlayFailure("prebuilt Raylib inputs do not match the pinned desktop provisioning report")
    return source_root / "src", library


def compile_native(
    generated: Path,
    manifest: dict[str, object],
    *,
    output: Path,
    include_directory: Path,
    library: Path,
    platform_name: str,
    cc: str,
    optimization: str,
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
            "-I",
            str(generated / "include"),
            "-I",
            str(include_directory),
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
    libraries, frameworks = provision.link_facts(lock, platform_name, "desktop")
    manifest_libraries = owned_fact_names(build.get("libraries"), "generated Caxecraft libraries")
    manifest_frameworks = owned_fact_names(build.get("frameworks"), "generated Caxecraft frameworks")
    if len(manifest_libraries) != len(libraries) or set(manifest_libraries) != set(libraries):
        raise PlayFailure("generated Caxecraft libraries differ from the pinned Raylib link plan")
    if len(manifest_frameworks) != len(frameworks) or set(manifest_frameworks) != set(frameworks):
        raise PlayFailure("generated Caxecraft frameworks differ from the pinned Raylib link plan")
    arguments = [cc, *[str(path) for path in objects], str(library)]
    for name in libraries:
        if name != "raylib":
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
        choices=("launch-smoke", "move-jump-edit", "pause-recapture", "combat-drop", "recovery-use", "full-inventory-gift", "full-inventory-mining"),
        help="run one deterministic in-process input script, capture its visual checkpoint, and quit",
    )
    parser.add_argument("--allow-network", action="store_true", help="allow the first checksum-pinned Raylib archive download")
    parser.add_argument("--authority", choices=("pinned-source", "offline-source"), default="pinned-source")
    parser.add_argument("--source", type=Path, help="exact Raylib 6.0 source tree for offline-source authority")
    parser.add_argument("--layout", choices=("split", "package", "unity"), default="split")
    parser.add_argument("--optimization", choices=("0", "2"), default="2")
    parser.add_argument("--cc", default=os.environ.get("CC", "clang" if platform.system() == "Darwin" else "gcc"))
    parser.add_argument("--cxx", default=os.environ.get("CXX", "clang++" if platform.system() == "Darwin" else "g++"))
    parser.add_argument("--cmake", default="cmake")
    parser.add_argument("--generator", choices=("Ninja", "Unix Makefiles"), default="Ninja" if shutil.which("ninja") else "Unix Makefiles")
    parser.add_argument("--cache-root", type=Path, default=ROOT / ".cache/caxecraft/raylib")
    parser.add_argument("--output-root", type=Path, default=CASE / "_build/play")
    parser.add_argument("--rebuild-raylib", action="store_true")
    parser.add_argument("--prebuilt-raylib-cache", type=Path, help="verified pinned-source cache produced by the Raylib integration lane")
    parser.add_argument("--prebuilt-raylib-build", type=Path, help="verified desktop build produced by the Raylib integration lane")
    parser.add_argument("--prebuilt-raylib-report", type=Path, help="normalized report for the verified prebuilt Raylib library")
    return parser.parse_args(argv)


def main(argv: list[str]) -> int:
    try:
        args = parse_args(argv)
        if args.smoke and args.pilot is not None:
            raise PlayFailure("--smoke is the launch-smoke pilot alias and cannot be combined with --pilot")
        if (args.smoke or args.pilot is not None) and (args.check_snapshots or args.compile_only or args.build_only):
            raise PlayFailure("a running smoke/pilot cannot be combined with a non-running mode")
        if args.check_snapshots:
            check_snapshots()
            print("caxecraft: playable snapshots and direct-C invariants passed")
            return 0
        platform_name = host_platform()
        if args.authority == "offline-source" and args.source is None:
            raise PlayFailure("--authority offline-source requires --source")
        if args.authority == "pinned-source" and args.source is not None:
            raise PlayFailure("--source is accepted only with --authority offline-source")
        output_root = prepare_output_root(args.output_root.resolve())
        generated = output_root / "generated"
        executable = output_root / "bin" / ("caxecraft.exe" if platform_name == "windows" else "caxecraft")
        selected_pilot = "launch-smoke" if args.smoke else args.pilot
        manifest = compile_haxe(generated, layout=args.layout, platform_name=platform_name, pilot=selected_pilot)
        print(f"caxecraft: generated {args.layout} C project at {generated}")
        if args.compile_only:
            print("caxecraft: compile-only proof passed (direct C, empty hxrt plan)")
            return 0

        prebuilt_values = (args.prebuilt_raylib_cache, args.prebuilt_raylib_build, args.prebuilt_raylib_report)
        if any(value is not None for value in prebuilt_values):
            if not all(value is not None for value in prebuilt_values):
                raise PlayFailure("prebuilt Raylib reuse requires cache, build, and report paths together")
            if args.allow_network or args.source is not None or args.rebuild_raylib:
                raise PlayFailure("prebuilt Raylib reuse rejects network, source, and rebuild options")
            include_directory, library = resolve_prebuilt_raylib(
                cache_root=args.prebuilt_raylib_cache,
                build_root=args.prebuilt_raylib_build,
                report_path=args.prebuilt_raylib_report,
                platform_name=platform_name,
            )
        else:
            include_directory, library = provision_raylib(
                authority=args.authority,
                platform_name=platform_name,
                source=args.source,
                cache_root=args.cache_root.resolve(),
                cc=args.cc,
                cxx=args.cxx,
                cmake=args.cmake,
                generator=args.generator,
                allow_network=args.allow_network,
                rebuild=args.rebuild_raylib,
            )
        compile_native(
            generated,
            manifest,
            output=executable,
            include_directory=include_directory,
            library=library,
            platform_name=platform_name,
            cc=args.cc,
            optimization=args.optimization,
        )
        stage_runtime_assets(executable.parent)
        print(f"caxecraft: built native executable at {executable}")
        if args.build_only:
            return 0
        if selected_pilot is not None:
            screenshot_names = {
                "launch-smoke": "caxecraft-smoke.png",
                "move-jump-edit": "caxecraft-pilot-move.png",
                "pause-recapture": "caxecraft-pilot-pause.png",
                "combat-drop": "caxecraft-pilot-combat.png",
                "recovery-use": "caxecraft-pilot-recovery.png",
                "full-inventory-gift": "caxecraft-pilot-full-inventory.png",
                "full-inventory-mining": "caxecraft-pilot-full-mining.png",
            }
            screenshot = executable.parent / screenshot_names[selected_pilot]
            if screenshot.exists():
                screenshot.unlink()
            run([str(executable)], cwd=executable.parent, timeout=15, label=f"Caxecraft {selected_pilot} graphical pilot")
            width, height = validate_smoke_screenshot(screenshot, platform_name=platform_name) if selected_pilot == "launch-smoke" else validate_presented_screenshot(
                screenshot,
                platform_name=platform_name,
                expected_drop=selected_pilot == "combat-drop",
                expected_attack=selected_pilot == "combat-drop",
                expected_recovery=selected_pilot == "recovery-use",
                expected_inventory_full=selected_pilot in ("full-inventory-gift", "full-inventory-mining"),
            )
            print(
                f"caxecraft: {selected_pilot} graphical pilot passed "
                f"({width}x{height} presented framebuffer and bounded exit within 15 seconds)"
            )
            return 0
        print("caxecraft: launching; press Q to quit")
        return subprocess.run([str(executable)], cwd=executable.parent, check=False).returncode
    except (OSError, UnicodeError, provision.ProvisionFailure, PlayFailure) as error:
        print(f"caxecraft: ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main(sys.argv[1:]))
