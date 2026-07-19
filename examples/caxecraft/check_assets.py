#!/usr/bin/env python3
"""Validate the pinned Caxecraft design art pack without image dependencies."""

from __future__ import annotations

import hashlib
import json
import re
import shutil
import struct
import sys
import tempfile
import zlib
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Any, Callable


CASE = Path(__file__).resolve().parent
ASSET_ROOT = CASE / "assets"
PNG_SIGNATURE = b"\x89PNG\r\n\x1a\n"
SHA256_RE = re.compile(r"^[0-9a-f]{64}$")
ID_RE = re.compile(r"^[a-z0-9]+(?:-[a-z0-9]+)*$")
ARTIFACT_ID_RE = re.compile(
    r"^exec-[0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12}$"
)
EXPECTED_ASSET_IDS = frozenset(
    {
        "adventure-characters",
        "adventure-items",
        "adventure-terrain",
        "caxecraft-wordmark",
        "entities",
        "hud",
        "items",
        "ivvy",
        "terrain",
        "title-panorama",
    }
)
EXPECTED_GENERATION_RECORD_IDS = frozenset(
    {
        "adventure-characters",
        "adventure-items",
        "adventure-terrain",
        "entities",
        "hud",
        "items",
        "ivvy",
        "panorama",
        "terrain",
        "wordmark",
    }
)
EXPECTED_SUPPORT_FILES = frozenset({"README.md", "manifest.json"})
IVVY_PRIVATE_REFERENCE = (
    "Private user-provided family-cat photograph used only as visual reference; "
    "not included in or distributed from this repository."
)
EXPECTED_GRID_CELLS = {
    "adventure-characters": (
        "haxirio-front", "haxirio-three-quarter", "haxirio-side", "haxirio-back",
        "ceesh-front", "ceesh-three-quarter", "ceesh-side", "ceesh-back",
        "browser-front", "browser-three-quarter", "browser-side", "browser-back",
        "browser-roar", "browser-charge", "browser-tail-sweep", "browser-stunned",
    ),
    "adventure-items": (
        "tideweave-suit-folded", "tideweave-helmet", "tideweave-air-canister", "tideweave-suit-pickup",
        "bounds-ward", "inlining-spark", "zero-cost-charm", "cleanup-crest",
        "glyph-wave", "glyph-leaf", "glyph-ember", "glyph-forge",
        "bridge-input", "bridge-routing-elbow", "checkpoint", "ceesh-scroll",
    ),
    "adventure-terrain": (
        "snow-top", "snow-side", "ancient-ice", "frost-stone",
        "ash-top", "ash-side", "charred-basalt", "ember-rock",
        "pine-canopy", "snowy-pine-canopy", "dark-pine-bark", "submerged-ruin-stone",
        "castle-slate", "fortress-brick", "teal-copper-roof", "bridge-rune-tile",
    ),
    "entities": (
        "moss-front", "moss-three-quarter", "moss-side", "moss-back",
        "nia-front", "nia-three-quarter", "nia-side", "nia-back",
        "mossling-front", "mossling-three-quarter", "mossling-side", "mossling-back",
        "ember-wisp-front", "ember-wisp-three-quarter", "ember-wisp-side", "ember-wisp-back",
    ),
    "hud": (
        "health-full", "health-half", "health-empty", "armor",
        "stamina-full", "stamina-empty", "food-full", "food-empty",
        "hotbar-normal", "hotbar-selected", "hotbar-locked", "backpack",
        "crosshair", "dialogue", "quest-marker", "damage-indicator",
    ),
    "items": (
        "grass-block", "soil-block", "slate-stone", "oak-log",
        "wood-pick", "stone-axe", "stone-shovel", "copper-sword",
        "amber-shard", "copper-nugget", "berries", "bread",
        "map", "lantern", "healing-herbs", "quest-token",
    ),
    "ivvy": (
        "sit-front", "sit-three-quarter", "stand-side", "stand-back",
        "walk-front", "walk-three-quarter", "walk-side", "hop",
        "point-secret", "sniff-drop", "retrieve-drop", "checkpoint-purr",
        "bubble-collar", "cover", "sleep", "victory",
    ),
    "terrain": (
        "meadow-grass-top", "grass-side", "rich-soil", "slate-stone",
        "river-sand", "teal-water", "bark-side", "log-top",
        "leafy-canopy", "oak-planks", "field-cobble", "clay-brick",
        "charcoal-ore", "copper-ore", "amber-crystal-ore", "foundation-rock",
    ),
}


class AssetValidationError(RuntimeError):
    pass


@dataclass(frozen=True)
class PngInfo:
    width: int
    height: int
    alpha: bool


def fail(message: str) -> None:
    raise AssetValidationError(message)


def require_object(value: Any, label: str) -> dict[str, Any]:
    if not isinstance(value, dict):
        fail(f"{label} must be an object")
    return value


def require_array(value: Any, label: str) -> list[Any]:
    if not isinstance(value, list):
        fail(f"{label} must be an array")
    return value


def require_string(value: Any, label: str) -> str:
    if not isinstance(value, str) or not value:
        fail(f"{label} must be a non-empty string")
    return value


def require_id(value: Any, label: str) -> str:
    identifier = require_string(value, label)
    if ID_RE.fullmatch(identifier) is None:
        fail(f"{label} is not a lowercase kebab-case identifier: {identifier!r}")
    return identifier


def require_positive_int(value: Any, label: str) -> int:
    if not isinstance(value, int) or isinstance(value, bool) or value <= 0:
        fail(f"{label} must be a positive integer")
    return value


def require_safe_path(value: Any, label: str) -> PurePosixPath:
    raw = require_string(value, label)
    if "\\" in raw:
        fail(f"{label} must use forward slashes")
    path = PurePosixPath(raw)
    if (
        path.is_absolute()
        or path.as_posix() != raw
        or not path.parts
        or any(part in ("", ".", "..") for part in path.parts)
    ):
        fail(f"{label} is not a normalized relative path: {raw!r}")
    return path


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def png_info(path: Path, asset_root: Path) -> PngInfo:
    payload = path.read_bytes()
    if not payload.startswith(PNG_SIGNATURE):
        fail(f"asset is not a PNG: {path.relative_to(asset_root).as_posix()}")

    offset = len(PNG_SIGNATURE)
    width: int | None = None
    height: int | None = None
    color_type: int | None = None
    has_transparency_chunk = False
    saw_end = False
    chunk_index = 0
    chunk_counts: dict[bytes, int] = {}
    exif_payload: bytes | None = None
    while offset < len(payload):
        if offset + 12 > len(payload):
            fail(f"truncated PNG chunk in {path.relative_to(asset_root).as_posix()}")
        length = struct.unpack_from(">I", payload, offset)[0]
        chunk_type = payload[offset + 4 : offset + 8]
        data_start = offset + 8
        data_end = data_start + length
        crc_end = data_end + 4
        if crc_end > len(payload):
            fail(f"truncated PNG data in {path.relative_to(asset_root).as_posix()}")
        expected_crc = struct.unpack_from(">I", payload, data_end)[0]
        actual_crc = zlib.crc32(chunk_type)
        actual_crc = zlib.crc32(payload[data_start:data_end], actual_crc) & 0xFFFFFFFF
        if actual_crc != expected_crc:
            fail(f"PNG CRC mismatch in {path.relative_to(asset_root).as_posix()}")

        chunk_counts[chunk_type] = chunk_counts.get(chunk_type, 0) + 1
        if chunk_type not in {b"IHDR", b"sRGB", b"eXIf", b"IDAT", b"IEND"}:
            fail(
                "asset PNG contains an unowned ancillary or palette chunk "
                f"{chunk_type!r}: {path.relative_to(asset_root).as_posix()}"
            )

        if chunk_index == 0 and chunk_type != b"IHDR":
            fail(f"PNG does not begin with IHDR: {path.relative_to(asset_root).as_posix()}")
        if chunk_type == b"IHDR":
            if length != 13 or width is not None:
                fail(f"invalid PNG IHDR in {path.relative_to(asset_root).as_posix()}")
            width, height, bit_depth, color_type, compression, filtering, interlace = struct.unpack(
                ">IIBBBBB", payload[data_start:data_end]
            )
            if bit_depth != 8 or compression != 0 or filtering != 0 or interlace != 0:
                fail(
                    "asset PNG must be non-interlaced 8-bit with standard compression/filtering: "
                    f"{path.relative_to(asset_root).as_posix()}"
                )
        elif chunk_type == b"tRNS":
            has_transparency_chunk = True
        elif chunk_type == b"sRGB":
            if length != 1 or payload[data_start:data_end] != b"\x00":
                fail(f"asset PNG has a nonstandard sRGB intent: {path.relative_to(asset_root).as_posix()}")
        elif chunk_type == b"eXIf":
            exif_payload = payload[data_start:data_end]
        elif chunk_type == b"IEND":
            if length != 0:
                fail(f"invalid PNG IEND in {path.relative_to(asset_root).as_posix()}")
            saw_end = True
            offset = crc_end
            break

        offset = crc_end
        chunk_index += 1

    if not saw_end or offset != len(payload) or width is None or height is None or color_type is None:
        fail(f"incomplete PNG structure in {path.relative_to(asset_root).as_posix()}")
    if (
        chunk_counts.get(b"IHDR") != 1
        or chunk_counts.get(b"sRGB") != 1
        or chunk_counts.get(b"eXIf") != 1
        or chunk_counts.get(b"IEND") != 1
        or chunk_counts.get(b"IDAT", 0) < 1
        or exif_payload != minimal_dimension_exif(width, height)
    ):
        fail(
            "asset PNG metadata must be the exact minimal sRGB/dimension profile: "
            f"{path.relative_to(asset_root).as_posix()}"
        )
    return PngInfo(width, height, color_type in (4, 6) or has_transparency_chunk)


def minimal_dimension_exif(width: int, height: int) -> bytes:
    """Exact big-endian TIFF payload produced by the reviewed pack normalizer."""
    return bytes.fromhex(
        "4d4d002a00000008000187690004000000010000001a000000000003"
        "a00100030000000100010000"
        f"a002000400000001{width:08x}"
        f"a003000400000001{height:08x}"
        "00000000"
    )


def validate_generation_records(records: dict[str, Any]) -> None:
    if set(records) != EXPECTED_GENERATION_RECORD_IDS:
        fail("generation record inventory drifted from the admitted art pack")
    for record_id, raw_record in records.items():
        require_id(record_id, f"generationRecords key {record_id!r}")
        record = require_object(raw_record, f"generationRecords.{record_id}")
        require_string(record.get("mode"), f"generationRecords.{record_id}.mode")
        artifact_ids = require_array(
            record.get("artifactIds"), f"generationRecords.{record_id}.artifactIds"
        )
        for index, artifact_id in enumerate(artifact_ids):
            artifact_id = require_string(
                artifact_id, f"generationRecords.{record_id}.artifactIds[{index}]"
            )
            if ARTIFACT_ID_RE.fullmatch(artifact_id) is None:
                fail(f"generation record {record_id} has a malformed artifact ID")
        references = require_array(
            record.get("referenceInputs"), f"generationRecords.{record_id}.referenceInputs"
        )
        for index, reference in enumerate(references):
            text = require_string(reference, f"generationRecords.{record_id}.referenceInputs[{index}]")
            if "/Users/" in text or "/var/" in text or "file://" in text or "\\" in text:
                fail(f"generation record {record_id} leaks a host/private path")
        require_string(record.get("processing"), f"generationRecords.{record_id}.processing")

        prompt = record.get("promptSummary")
        if not isinstance(prompt, str) or not prompt:
            fail(f"generationRecords.{record_id}.promptSummary must be non-empty")
        if not artifact_ids or not str(record.get("mode")).startswith(
            "openai-built-in-imagegen"
        ):
            fail(f"generated art record {record_id} lost its generation identity")
        expected_references = [IVVY_PRIVATE_REFERENCE] if record_id == "ivvy" else []
        if references != expected_references:
            fail(f"generation record {record_id} has an unexpected reference inventory")


def validate_asset_pack(asset_root: Path = ASSET_ROOT) -> int:
    if asset_root.is_symlink() or not asset_root.is_dir():
        fail("Caxecraft asset root must be a real directory")
    manifest_path = asset_root / "manifest.json"
    if manifest_path.is_symlink() or not manifest_path.is_file():
        fail("Caxecraft asset manifest is missing or is a symlink")
    readme_path = asset_root / "README.md"
    if readme_path.is_symlink() or not readme_path.is_file():
        fail("Caxecraft asset README is missing or is a symlink")
    try:
        payload = manifest_path.read_text(encoding="utf-8")
        if payload.startswith("\ufeff"):
            fail("Caxecraft asset manifest must not contain a UTF-8 BOM")
        manifest = require_object(json.loads(payload), "manifest")
    except (OSError, UnicodeError, json.JSONDecodeError) as error:
        fail(f"cannot read Caxecraft asset manifest: {error}")

    if manifest.get("schemaVersion") != 1:
        fail("Caxecraft asset manifest schemaVersion must be 1")
    if manifest.get("status") != "design-source-not-runtime-integrated":
        fail("asset status must not claim runtime integration before haxe_c-xge.15 closes")
    reproducibility = require_object(manifest.get("reproducibility"), "reproducibility")
    if (
        reproducibility.get("cleanBuildNetworkRequired") is not False
        or reproducibility.get("selectedOutputBytesPinned") is not True
        or reproducibility.get("sourceToByteRegeneration") != "not-applicable-primary-source"
        or reproducibility.get("derivedRuntimeAssets") != "deterministic-regeneration-required"
        or reproducibility.get("gitStorage") != "ordinary-git-v1-reassess-before-runtime-integration"
        or reproducibility.get("owner") != "haxe_c-xge.15"
    ):
        fail("asset reproducibility policy drifted or overclaims the current design pack")

    rights = require_object(manifest.get("rights"), "rights")
    if set(rights) != {"caxecraft-original"}:
        fail("the initial pack must contain only repository-original reviewed source art")
    original_rights = require_object(rights.get("caxecraft-original"), "rights.caxecraft-original")
    if (
        original_rights.get("kind") != "repository-original-reviewed-source-art"
        or original_rights.get("licenseExpression") != "GPL-3.0-only"
    ):
        fail("original Caxecraft art must retain the repository license expression")

    records = require_object(manifest.get("generationRecords"), "generationRecords")
    validate_generation_records(records)

    raw_assets = require_array(manifest.get("assets"), "assets")
    ids: set[str] = set()
    paths: set[str] = set()
    for index, raw_asset in enumerate(raw_assets):
        label = f"assets[{index}]"
        asset = require_object(raw_asset, label)
        asset_id = require_id(asset.get("id"), f"{label}.id")
        if asset_id in ids:
            fail(f"duplicate asset id: {asset_id}")
        ids.add(asset_id)

        relative = require_safe_path(asset.get("path"), f"{label}.path")
        rendered_path = relative.as_posix()
        if relative.suffix != ".png" or rendered_path in paths:
            fail(f"asset path is duplicate or not PNG: {rendered_path}")
        paths.add(rendered_path)
        path = asset_root.joinpath(*relative.parts)
        try:
            path.resolve(strict=False).relative_to(asset_root.resolve())
        except ValueError:
            fail(f"asset path escapes the pack: {rendered_path}")
        if path.is_symlink() or not path.is_file() or path.stat().st_size == 0:
            fail(f"asset is missing, empty, or a symlink: {rendered_path}")

        expected_hash = asset.get("sha256")
        if not isinstance(expected_hash, str) or SHA256_RE.fullmatch(expected_hash) is None:
            fail(f"{label}.sha256 must be a lowercase SHA-256 digest")
        if sha256(path) != expected_hash:
            fail(f"asset hash mismatch: {rendered_path}")
        info = png_info(path, asset_root)
        width = require_positive_int(asset.get("width"), f"{label}.width")
        height = require_positive_int(asset.get("height"), f"{label}.height")
        alpha = asset.get("alpha")
        if not isinstance(alpha, bool):
            fail(f"{label}.alpha must be boolean")
        if (info.width, info.height, info.alpha) != (width, height, alpha):
            fail(
                f"asset PNG metadata mismatch for {rendered_path}: "
                f"manifest={(width, height, alpha)!r}, bytes={info!r}"
            )

        generation_record = require_id(asset.get("generationRecord"), f"{label}.generationRecord")
        if generation_record not in records:
            fail(f"asset {asset_id} references unknown generation record {generation_record}")
        rights_id = require_id(asset.get("rights"), f"{label}.rights")
        if rights_id not in rights:
            fail(f"asset {asset_id} references unknown rights record {rights_id}")

        raw_grid = asset.get("grid")
        if raw_grid is not None:
            grid = require_object(raw_grid, f"{label}.grid")
            columns = require_positive_int(grid.get("columns"), f"{label}.grid.columns")
            rows = require_positive_int(grid.get("rows"), f"{label}.grid.rows")
            cell_width = require_positive_int(grid.get("cellWidth"), f"{label}.grid.cellWidth")
            cell_height = require_positive_int(grid.get("cellHeight"), f"{label}.grid.cellHeight")
            if columns * cell_width != width or rows * cell_height != height:
                fail(f"atlas grid does not tile the full image: {rendered_path}")
            raw_cells = require_array(grid.get("cells"), f"{label}.grid.cells")
            cells = [require_id(value, f"{label}.grid.cells[{cell}]") for cell, value in enumerate(raw_cells)]
            if len(cells) != rows * columns or len(cells) != len(set(cells)):
                fail(f"atlas cells are incomplete or duplicated: {rendered_path}")
            if tuple(cells) != EXPECTED_GRID_CELLS.get(asset_id):
                fail(f"atlas semantic cell order drifted: {rendered_path}")
        elif asset_id in EXPECTED_GRID_CELLS:
            fail(f"atlas lost its semantic grid: {rendered_path}")

    if ids != EXPECTED_ASSET_IDS:
        missing = ", ".join(sorted(EXPECTED_ASSET_IDS - ids)) or "none"
        extra = ", ".join(sorted(ids - EXPECTED_ASSET_IDS)) or "none"
        fail(f"asset inventory drifted; missing={missing}; extra={extra}")
    discovered: set[str] = set()
    for path in asset_root.rglob("*"):
        relative = path.relative_to(asset_root).as_posix()
        if path.is_symlink():
            fail(f"asset tree contains a forbidden symlink: {relative}")
        if not path.is_dir():
            discovered.add(relative)
    expected_files = paths | set(EXPECTED_SUPPORT_FILES)
    if discovered != expected_files:
        unlisted = ", ".join(sorted(discovered - expected_files)) or "none"
        absent = ", ".join(sorted(expected_files - discovered)) or "none"
        fail(f"asset file set differs from manifest; unlisted={unlisted}; absent={absent}")
    return len(raw_assets)


def write_manifest(asset_root: Path, manifest: dict[str, Any]) -> None:
    (asset_root / "manifest.json").write_text(
        json.dumps(manifest, ensure_ascii=False, indent=2) + "\n", encoding="utf-8"
    )


def expect_rejected(label: str, mutate: Callable[[Path], None]) -> None:
    with tempfile.TemporaryDirectory(prefix="caxecraft-assets-negative-") as temporary:
        asset_root = Path(temporary) / "assets"
        shutil.copytree(ASSET_ROOT, asset_root)
        mutate(asset_root)
        try:
            validate_asset_pack(asset_root)
        except AssetValidationError:
            return
        fail(f"negative asset contract unexpectedly accepted {label}")


def negative_contracts() -> None:
    def add_sidecar(asset_root: Path) -> None:
        (asset_root / "private-family-photo.jpg").write_bytes(b"not admitted\n")

    def add_dangling_symlink(asset_root: Path) -> None:
        (asset_root / "private-reference").symlink_to("missing-private-photo.jpg")

    def reorder_cells(asset_root: Path) -> None:
        manifest = json.loads((asset_root / "manifest.json").read_text(encoding="utf-8"))
        cells = manifest["assets"][0]["grid"]["cells"]
        cells[0], cells[1] = cells[1], cells[0]
        write_manifest(asset_root, manifest)

    def broaden_private_claim(asset_root: Path) -> None:
        manifest = json.loads((asset_root / "manifest.json").read_text(encoding="utf-8"))
        manifest["generationRecords"]["ivvy"]["referenceInputs"] = [
            "Private photo retained elsewhere."
        ]
        write_manifest(asset_root, manifest)

    def add_text_metadata(asset_root: Path) -> None:
        relative = PurePosixPath("atlases/terrain.png")
        path = asset_root.joinpath(*relative.parts)
        payload = path.read_bytes()
        offset = len(PNG_SIGNATURE)
        iend_offset: int | None = None
        while offset < len(payload):
            length = struct.unpack_from(">I", payload, offset)[0]
            if payload[offset + 4 : offset + 8] == b"IEND":
                iend_offset = offset
                break
            offset += length + 12
        if iend_offset is None:
            fail("negative metadata fixture could not find IEND")
        chunk_type = b"tEXt"
        chunk_data = b"Comment\x00private metadata must be rejected"
        crc = zlib.crc32(chunk_data, zlib.crc32(chunk_type)) & 0xFFFFFFFF
        chunk = struct.pack(">I", len(chunk_data)) + chunk_type + chunk_data + struct.pack(">I", crc)
        path.write_bytes(payload[:iend_offset] + chunk + payload[iend_offset:])

        manifest = json.loads((asset_root / "manifest.json").read_text(encoding="utf-8"))
        for asset in manifest["assets"]:
            if asset["path"] == relative.as_posix():
                asset["sha256"] = sha256(path)
                break
        write_manifest(asset_root, manifest)

    expect_rejected("unlisted private sidecar", add_sidecar)
    try:
        expect_rejected("dangling private-reference symlink", add_dangling_symlink)
    except OSError:
        # Some Windows runners do not grant symlink creation; the positive tree
        # walk still rejects links independently of this host capability probe.
        pass
    expect_rejected("semantic atlas reorder", reorder_cells)
    expect_rejected("broadened private-reference claim", broaden_private_claim)
    expect_rejected("textual PNG metadata", add_text_metadata)


def main() -> int:
    try:
        count = validate_asset_pack()
        negative_contracts()
    except (AssetValidationError, OSError, UnicodeError, ValueError) as error:
        print(f"caxecraft-assets: ERROR: {error}", file=sys.stderr)
        return 1
    print(
        "caxecraft-assets: OK: "
        f"{count} exact offline primary-source PNGs, semantic atlas grids, complete file inventory, "
        "minimal metadata, repository-scoped privacy records, and negative mutations passed"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
