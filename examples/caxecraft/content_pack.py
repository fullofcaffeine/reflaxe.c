#!/usr/bin/env python3
"""Validate the built-in Caxecraft content pack and generate its Haxe adapter.

The JSON file is editable game data. This tool checks every field, reference,
bound, mechanic name, and visual cell before producing ordinary typed Haxe.
The generated adapter is temporary packaging glue: it lets the native game use
reviewed pack facts before haxe.c has the general JSON and filesystem support
needed to load the same file at runtime.
"""

from __future__ import annotations

import argparse
import json
import re
from collections.abc import Callable
from dataclasses import dataclass
from pathlib import Path
from typing import Protocol, TypeVar


CASE = Path(__file__).resolve().parent
SOURCE = CASE / "packs/caxecraft/base/content.json"
ASSET_SOURCE = CASE / "assets/manifest.json"
SCENARIO_SOURCE = CASE / "scenarios/first-playable/map.caxemap"
OUTPUT = CASE / "src/caxecraft/content/BaseContentPack.hx"

ROOT_KEYS = {
    "schemaVersion",
    "logicalPath",
    "packId",
    "packVersion",
    "assetManifestId",
    "airBlock",
    "defaultAquaticProfile",
    "features",
    "blocks",
    "fluids",
    "aquaticProfiles",
    "items",
    "npcs",
    "enemies",
    "drops",
    "effects",
    "prefabs",
    "statefulObjects",
    "states",
    "signals",
}
BLOCK_KEYS = {"id", "storageCode", "collision", "edit", "dropItem", "renderProfile"}
FLUID_KEYS = {
    "id",
    "simulationProfile",
    "renderProfile",
    "cameraProfile",
    "audioProfile",
    "presentation",
}
AQUATIC_PROFILE_KEYS = {
    "id",
    "maximumBreathTicks",
    "breathRecoveryPerTick",
    "horizontalControlMilli",
    "ascentAccelerationMilli",
    "descentAccelerationMilli",
    "buoyancyAccelerationMilli",
    "dragPerTickMilli",
    "drowningIntervalTicks",
    "underwaterMining",
    "coldProtection",
}
ITEM_KEYS = {
    "id",
    "maxStack",
    "useProfile",
    "placementBlock",
    "aquaticProfile",
    "icon",
}
NPC_KEYS = {"id", "behaviorProfile", "interactionRadiusMilli", "presentation"}
ENEMY_KEYS = {
    "id",
    "behaviorProfile",
    "maxHealth",
    "noticeRadiusMilli",
    "strikeRadiusMilli",
    "attackRadiusMilli",
    "windupTicks",
    "recoveryTicks",
    "stepMilli",
    "drop",
    "presentation",
}
DROP_KEYS = {"id", "item", "quantity", "pickupRadiusMilli", "presentation"}
EFFECT_KEYS = {"id", "profile"}
PRESENTATION_KEYS = {"asset", "cell"}

CONTENT_ID_PATTERN = re.compile(r"^[a-z][a-z0-9-]{0,31}:[a-z][a-z0-9]*(?:[._-][a-z0-9]+)*$")
PROFILE_PATTERN = re.compile(r"^[a-z][a-z0-9]*(?:-[a-z0-9]+)*$")
LOGICAL_PART_PATTERN = re.compile(r"^[a-z][a-z0-9-]{0,63}$")

COLLISIONS = {"passable", "solid"}
EDITS = {"collectable", "immutable"}
BLOCK_RENDER_PROFILES = {
    "air",
    "ash-field",
    "foundation-rock",
    "forest-leaves",
    "forest-wood",
    "meadow-grass",
    "rich-soil",
    "river-sand",
    "slate-stone",
    "snow-field",
}
FLUID_SIMULATION_PROFILES = {"bounded-water"}
FLUID_RENDER_PROFILES = {"translucent-voxel"}
FLUID_CAMERA_PROFILES = {"clear-submersion"}
FLUID_AUDIO_PROFILES = {"fresh-water"}
ITEM_USE_PROFILES = {
    "consume-one-heart",
    "equip-aquatic",
    "haxeforge-tool",
    "light-source",
    "melee-sword",
    "none",
    "place-block",
}
NPC_BEHAVIOR_PROFILES = {"stationary-dialogue"}
ENEMY_BEHAVIOR_PROFILES = {"wander-chase-melee"}
EFFECT_PROFILES = {"melee-feedback", "pickup-feedback"}


class ContentPackFailure(ValueError):
    """The pack cannot safely become typed game content."""


class IdentifiedContent(Protocol):
    """The one field shared by entries that generate closed Haxe enums."""

    content_id: str


Identified = TypeVar("Identified", bound=IdentifiedContent)


@dataclass(frozen=True)
class Presentation:
    asset: str
    cell: str
    cell_index: int


@dataclass(frozen=True)
class Block:
    content_id: str
    storage_code: int
    collision: str
    edit: str
    drop_item: str | None
    render_profile: str


@dataclass(frozen=True)
class Fluid:
    content_id: str
    simulation_profile: str
    render_profile: str
    camera_profile: str
    audio_profile: str
    presentation: Presentation


@dataclass(frozen=True)
class AquaticProfile:
    content_id: str
    maximum_breath_ticks: int
    breath_recovery_per_tick: int
    horizontal_control_milli: int
    ascent_acceleration_milli: int
    descent_acceleration_milli: int
    buoyancy_acceleration_milli: int
    drag_per_tick_milli: int
    drowning_interval_ticks: int
    underwater_mining: bool
    cold_protection: bool


@dataclass(frozen=True)
class Item:
    content_id: str
    max_stack: int
    use_profile: str
    placement_block: str | None
    aquatic_profile: str | None
    icon: Presentation


@dataclass(frozen=True)
class Npc:
    content_id: str
    behavior_profile: str
    interaction_radius_milli: int
    presentation: Presentation


@dataclass(frozen=True)
class Enemy:
    content_id: str
    behavior_profile: str
    max_health: int
    notice_radius_milli: int
    strike_radius_milli: int
    attack_radius_milli: int
    windup_ticks: int
    recovery_ticks: int
    step_milli: int
    drop: str
    presentation: Presentation


@dataclass(frozen=True)
class Drop:
    content_id: str
    item: str
    quantity: int
    pickup_radius_milli: int
    presentation: Presentation


@dataclass(frozen=True)
class Effect:
    content_id: str
    profile: str


@dataclass(frozen=True)
class ContentPack:
    logical_path: str
    pack_id: str
    pack_version: int
    asset_manifest_id: str
    air_block: str
    default_aquatic_profile: str
    features: tuple[str, ...]
    blocks: tuple[Block, ...]
    fluids: tuple[Fluid, ...]
    aquatic_profiles: tuple[AquaticProfile, ...]
    items: tuple[Item, ...]
    npcs: tuple[Npc, ...]
    enemies: tuple[Enemy, ...]
    drops: tuple[Drop, ...]
    effects: tuple[Effect, ...]


def reject_duplicate_keys(pairs: list[tuple[str, object]]) -> dict[str, object]:
    """Reject JSON's normally silent last-value-wins behavior."""

    result: dict[str, object] = {}
    for key, value in pairs:
        if key in result:
            raise ContentPackFailure(f"duplicate JSON key {key!r}")
        result[key] = value
    return result


def decode_document(text: str) -> dict[str, object]:
    try:
        value = json.loads(text, object_pairs_hook=reject_duplicate_keys)
    except json.JSONDecodeError as error:
        raise ContentPackFailure(f"invalid JSON: {error.msg} at line {error.lineno}") from error
    if not isinstance(value, dict):
        raise ContentPackFailure("content-pack root must be an object")
    return value


def require_keys(value: object, expected: set[str], coordinate: str) -> dict[str, object]:
    if not isinstance(value, dict):
        raise ContentPackFailure(f"{coordinate} must be an object")
    actual = set(value)
    if actual != expected:
        raise ContentPackFailure(
            f"{coordinate} keys differ; missing={sorted(expected - actual)}, unknown={sorted(actual - expected)}"
        )
    return value


def require_array(value: object, coordinate: str, *, maximum: int = 256) -> list[object]:
    if not isinstance(value, list) or len(value) > maximum:
        raise ContentPackFailure(f"{coordinate} must be an array with at most {maximum} entries")
    return value


def require_string(value: object, coordinate: str) -> str:
    if not isinstance(value, str) or not value:
        raise ContentPackFailure(f"{coordinate} must be a non-empty string")
    if len(value.encode("utf-8")) > 128 or any(ord(character) < 32 or 0xD800 <= ord(character) <= 0xDFFF for character in value):
        raise ContentPackFailure(f"{coordinate} contains unsupported or overlong text")
    return value


def require_integer(value: object, coordinate: str, minimum: int, maximum: int) -> int:
    if isinstance(value, bool) or not isinstance(value, int) or value < minimum or value > maximum:
        raise ContentPackFailure(f"{coordinate} must be an integer from {minimum} through {maximum}")
    return value


def require_boolean(value: object, coordinate: str) -> bool:
    if not isinstance(value, bool):
        raise ContentPackFailure(f"{coordinate} must be true or false")
    return value


def content_id(value: object, coordinate: str) -> str:
    result = require_string(value, coordinate)
    if CONTENT_ID_PATTERN.fullmatch(result) is None:
        raise ContentPackFailure(f"{coordinate} is not a canonical namespaced content ID")
    return result


def nullable_content_id(value: object, coordinate: str) -> str | None:
    return None if value is None else content_id(value, coordinate)


def profile(value: object, coordinate: str, admitted: set[str]) -> str:
    result = require_string(value, coordinate)
    if PROFILE_PATTERN.fullmatch(result) is None or result not in admitted:
        raise ContentPackFailure(f"{coordinate} names unregistered mechanic/profile {result!r}")
    return result


def logical_path(value: object, coordinate: str) -> str:
    result = require_string(value, coordinate)
    parts = result.split("/")
    if len(parts) < 2 or any(LOGICAL_PART_PATTERN.fullmatch(part) is None for part in parts):
        raise ContentPackFailure(f"{coordinate} must be a canonical relative logical path without dot segments")
    return result


def canonical_array(
    values: list[object],
    coordinate: str,
    parse_entry: Callable[[object, str], Identified],
) -> tuple[Identified, ...]:
    parsed = tuple(parse_entry(value, f"{coordinate}[{index}]") for index, value in enumerate(values))
    ids = [value.content_id for value in parsed]
    if ids != sorted(ids, key=lambda item: item.encode("utf-8")):
        raise ContentPackFailure(f"{coordinate} must be ordered by UTF-8 content ID")
    if len(ids) != len(set(ids)):
        raise ContentPackFailure(f"{coordinate} contains a duplicate content ID")
    return parsed


def load_asset_inventory(document: dict[str, object]) -> tuple[str, dict[str, tuple[str, ...]]]:
    pack_id = require_string(document.get("packId"), "asset manifest packId")
    assets: dict[str, tuple[str, ...]] = {}
    for index, raw_asset in enumerate(require_array(document.get("assets"), "asset manifest assets", maximum=128)):
        asset = raw_asset if isinstance(raw_asset, dict) else None
        if asset is None:
            raise ContentPackFailure(f"asset manifest assets[{index}] must be an object")
        asset_id = require_string(asset.get("id"), f"asset manifest assets[{index}].id")
        if asset_id in assets:
            raise ContentPackFailure(f"asset manifest duplicates asset ID {asset_id!r}")
        raw_grid = asset.get("grid")
        if raw_grid is None:
            assets[asset_id] = ()
            continue
        if not isinstance(raw_grid, dict):
            raise ContentPackFailure(f"asset manifest assets[{index}].grid must be an object")
        cells = tuple(
            require_string(value, f"asset manifest assets[{index}].grid.cells[{cell_index}]")
            for cell_index, value in enumerate(require_array(raw_grid.get("cells"), f"asset manifest assets[{index}].grid.cells"))
        )
        if len(cells) != len(set(cells)):
            raise ContentPackFailure(f"asset manifest asset {asset_id!r} duplicates a cell ID")
        assets[asset_id] = cells
    return pack_id, assets


def parse_presentation(value: object, coordinate: str, assets: dict[str, tuple[str, ...]]) -> Presentation:
    raw = require_keys(value, PRESENTATION_KEYS, coordinate)
    asset = require_string(raw["asset"], f"{coordinate}.asset")
    cell = require_string(raw["cell"], f"{coordinate}.cell")
    cells = assets.get(asset)
    if cells is None:
        raise ContentPackFailure(f"{coordinate}.asset references unknown visual asset {asset!r}")
    if cell not in cells:
        raise ContentPackFailure(f"{coordinate}.cell references unknown cell {asset}:{cell}")
    return Presentation(asset, cell, cells.index(cell))


def validate_document(document: dict[str, object], asset_document: dict[str, object]) -> ContentPack:
    root = require_keys(document, ROOT_KEYS, "content pack")
    if root["schemaVersion"] != 2:
        raise ContentPackFailure("only content-pack schemaVersion 2 is supported")
    pack_version = require_integer(root["packVersion"], "packVersion", 1, 2_147_483_647)
    pack_id = content_id(root["packId"], "packId")
    path = logical_path(root["logicalPath"], "logicalPath")
    asset_manifest_id = require_string(root["assetManifestId"], "assetManifestId")
    actual_asset_manifest_id, assets = load_asset_inventory(asset_document)
    if asset_manifest_id != actual_asset_manifest_id:
        raise ContentPackFailure("assetManifestId does not name the reviewed visual asset manifest")

    raw_features = require_array(root["features"], "features", maximum=32)
    features = tuple(content_id(value, f"features[{index}]") for index, value in enumerate(raw_features))
    if not features or list(features) != sorted(features, key=lambda item: item.encode("utf-8")) or len(features) != len(set(features)):
        raise ContentPackFailure("features must be a non-empty, unique UTF-8-ordered content-ID array")
    if set(features) != {"caxecraft:core"}:
        raise ContentPackFailure("the current engine registers exactly the caxecraft:core feature")

    def parse_block(value: object, coordinate: str) -> Block:
        raw = require_keys(value, BLOCK_KEYS, coordinate)
        collision_value = require_string(raw["collision"], f"{coordinate}.collision")
        edit_value = require_string(raw["edit"], f"{coordinate}.edit")
        if collision_value not in COLLISIONS or edit_value not in EDITS:
            raise ContentPackFailure(f"{coordinate} uses an unknown block collision/edit policy")
        drop_item = nullable_content_id(raw["dropItem"], f"{coordinate}.dropItem")
        if (edit_value == "collectable") != (drop_item is not None):
            raise ContentPackFailure(f"{coordinate} collectable blocks need one dropItem and immutable blocks need null")
        return Block(
            content_id(raw["id"], f"{coordinate}.id"),
            require_integer(raw["storageCode"], f"{coordinate}.storageCode", 0, 255),
            collision_value,
            edit_value,
            drop_item,
            profile(raw["renderProfile"], f"{coordinate}.renderProfile", BLOCK_RENDER_PROFILES),
        )

    def parse_fluid(value: object, coordinate: str) -> Fluid:
        raw = require_keys(value, FLUID_KEYS, coordinate)
        return Fluid(
            content_id(raw["id"], f"{coordinate}.id"),
            profile(
                raw["simulationProfile"],
                f"{coordinate}.simulationProfile",
                FLUID_SIMULATION_PROFILES,
            ),
            profile(
                raw["renderProfile"],
                f"{coordinate}.renderProfile",
                FLUID_RENDER_PROFILES,
            ),
            profile(
                raw["cameraProfile"],
                f"{coordinate}.cameraProfile",
                FLUID_CAMERA_PROFILES,
            ),
            profile(
                raw["audioProfile"],
                f"{coordinate}.audioProfile",
                FLUID_AUDIO_PROFILES,
            ),
            parse_presentation(raw["presentation"], f"{coordinate}.presentation", assets),
        )

    def parse_aquatic_profile(value: object, coordinate: str) -> AquaticProfile:
        raw = require_keys(value, AQUATIC_PROFILE_KEYS, coordinate)
        return AquaticProfile(
            content_id(raw["id"], f"{coordinate}.id"),
            require_integer(raw["maximumBreathTicks"], f"{coordinate}.maximumBreathTicks", 1, 12_000),
            require_integer(raw["breathRecoveryPerTick"], f"{coordinate}.breathRecoveryPerTick", 1, 120),
            require_integer(raw["horizontalControlMilli"], f"{coordinate}.horizontalControlMilli", 0, 1_000),
            require_integer(raw["ascentAccelerationMilli"], f"{coordinate}.ascentAccelerationMilli", 0, 40_000),
            require_integer(raw["descentAccelerationMilli"], f"{coordinate}.descentAccelerationMilli", 0, 40_000),
            require_integer(raw["buoyancyAccelerationMilli"], f"{coordinate}.buoyancyAccelerationMilli", 0, 30_000),
            require_integer(raw["dragPerTickMilli"], f"{coordinate}.dragPerTickMilli", 0, 900),
            require_integer(raw["drowningIntervalTicks"], f"{coordinate}.drowningIntervalTicks", 1, 1_200),
            require_boolean(raw["underwaterMining"], f"{coordinate}.underwaterMining"),
            require_boolean(raw["coldProtection"], f"{coordinate}.coldProtection"),
        )

    def parse_item(value: object, coordinate: str) -> Item:
        raw = require_keys(value, ITEM_KEYS, coordinate)
        use = profile(raw["useProfile"], f"{coordinate}.useProfile", ITEM_USE_PROFILES)
        placement = nullable_content_id(raw["placementBlock"], f"{coordinate}.placementBlock")
        aquatic = nullable_content_id(raw["aquaticProfile"], f"{coordinate}.aquaticProfile")
        if (use == "place-block") != (placement is not None):
            raise ContentPackFailure(f"{coordinate} place-block items need placementBlock and other items need null")
        if (use == "equip-aquatic") != (aquatic is not None):
            raise ContentPackFailure(f"{coordinate} equip-aquatic items need aquaticProfile and other items need null")
        return Item(
            content_id(raw["id"], f"{coordinate}.id"),
            require_integer(raw["maxStack"], f"{coordinate}.maxStack", 1, 64),
            use,
            placement,
            aquatic,
            parse_presentation(raw["icon"], f"{coordinate}.icon", assets),
        )

    def parse_npc(value: object, coordinate: str) -> Npc:
        raw = require_keys(value, NPC_KEYS, coordinate)
        return Npc(
            content_id(raw["id"], f"{coordinate}.id"),
            profile(raw["behaviorProfile"], f"{coordinate}.behaviorProfile", NPC_BEHAVIOR_PROFILES),
            require_integer(raw["interactionRadiusMilli"], f"{coordinate}.interactionRadiusMilli", 250, 32_000),
            parse_presentation(raw["presentation"], f"{coordinate}.presentation", assets),
        )

    def parse_enemy(value: object, coordinate: str) -> Enemy:
        raw = require_keys(value, ENEMY_KEYS, coordinate)
        notice = require_integer(raw["noticeRadiusMilli"], f"{coordinate}.noticeRadiusMilli", 250, 64_000)
        strike = require_integer(raw["strikeRadiusMilli"], f"{coordinate}.strikeRadiusMilli", 250, 64_000)
        attack = require_integer(raw["attackRadiusMilli"], f"{coordinate}.attackRadiusMilli", 250, 64_000)
        if not attack <= strike <= notice:
            raise ContentPackFailure(f"{coordinate} requires attackRadius <= strikeRadius <= noticeRadius")
        return Enemy(
            content_id(raw["id"], f"{coordinate}.id"),
            profile(raw["behaviorProfile"], f"{coordinate}.behaviorProfile", ENEMY_BEHAVIOR_PROFILES),
            require_integer(raw["maxHealth"], f"{coordinate}.maxHealth", 1, 10_000),
            notice,
            strike,
            attack,
            require_integer(raw["windupTicks"], f"{coordinate}.windupTicks", 1, 1_200),
            require_integer(raw["recoveryTicks"], f"{coordinate}.recoveryTicks", 1, 1_200),
            require_integer(raw["stepMilli"], f"{coordinate}.stepMilli", 1, 10_000),
            content_id(raw["drop"], f"{coordinate}.drop"),
            parse_presentation(raw["presentation"], f"{coordinate}.presentation", assets),
        )

    def parse_drop(value: object, coordinate: str) -> Drop:
        raw = require_keys(value, DROP_KEYS, coordinate)
        return Drop(
            content_id(raw["id"], f"{coordinate}.id"),
            content_id(raw["item"], f"{coordinate}.item"),
            require_integer(raw["quantity"], f"{coordinate}.quantity", 1, 64),
            require_integer(raw["pickupRadiusMilli"], f"{coordinate}.pickupRadiusMilli", 100, 32_000),
            parse_presentation(raw["presentation"], f"{coordinate}.presentation", assets),
        )

    def parse_effect(value: object, coordinate: str) -> Effect:
        raw = require_keys(value, EFFECT_KEYS, coordinate)
        return Effect(
            content_id(raw["id"], f"{coordinate}.id"),
            profile(raw["profile"], f"{coordinate}.profile", EFFECT_PROFILES),
        )

    blocks = canonical_array(require_array(root["blocks"], "blocks"), "blocks", parse_block)
    fluids = canonical_array(require_array(root["fluids"], "fluids"), "fluids", parse_fluid)
    aquatic_profiles = canonical_array(
        require_array(root["aquaticProfiles"], "aquaticProfiles"),
        "aquaticProfiles",
        parse_aquatic_profile,
    )
    items = canonical_array(require_array(root["items"], "items"), "items", parse_item)
    npcs = canonical_array(require_array(root["npcs"], "npcs"), "npcs", parse_npc)
    enemies = canonical_array(require_array(root["enemies"], "enemies"), "enemies", parse_enemy)
    drops = canonical_array(require_array(root["drops"], "drops"), "drops", parse_drop)
    effects = canonical_array(require_array(root["effects"], "effects"), "effects", parse_effect)

    # Schema 1 reserves these exact closed kinds but the first playable does not
    # yet use them. Requiring empty arrays prevents aspirational definitions from
    # masquerading as implemented engine capability.
    for name in ("prefabs", "statefulObjects", "states", "signals"):
        if require_array(root[name], name):
            raise ContentPackFailure(f"{name} must remain empty until its engine mechanic has executable evidence")

    all_ids = [*features]
    for values in (blocks, fluids, aquatic_profiles, items, npcs, enemies, drops, effects):
        all_ids.extend(value.content_id for value in values)
    if len(all_ids) != len(set(all_ids)):
        raise ContentPackFailure("content IDs must be collision-free across every closed kind")

    item_ids = {item.content_id for item in items}
    block_ids = {block.content_id for block in blocks}
    aquatic_profile_ids = {value.content_id for value in aquatic_profiles}
    drop_ids = {drop.content_id for drop in drops}
    codes = [block.storage_code for block in blocks]
    if len(codes) != len(set(codes)):
        raise ContentPackFailure("block storageCode values must be unique")
    air_block = content_id(root["airBlock"], "airBlock")
    air = next((block for block in blocks if block.content_id == air_block), None)
    if air is None or air.storage_code != 0 or air.collision != "passable" or air.render_profile != "air":
        raise ContentPackFailure("airBlock must resolve to storage code 0 with passable collision and the air render profile")
    if sum(block.collision == "passable" for block in blocks) != 1:
        raise ContentPackFailure("schema 2 requires exactly one passable terrain block")
    if not fluids:
        raise ContentPackFailure("schema 2 requires at least one validated fluid")
    default_aquatic_profile = content_id(root["defaultAquaticProfile"], "defaultAquaticProfile")
    if default_aquatic_profile not in aquatic_profile_ids:
        raise ContentPackFailure("defaultAquaticProfile references an unknown aquatic profile")
    for block in blocks:
        if block.drop_item is not None and block.drop_item not in item_ids:
            raise ContentPackFailure(f"block {block.content_id!r} references unknown drop item {block.drop_item!r}")
    for item in items:
        if item.placement_block is not None and item.placement_block not in block_ids:
            raise ContentPackFailure(f"item {item.content_id!r} references unknown placement block {item.placement_block!r}")
        if item.aquatic_profile is not None and item.aquatic_profile not in aquatic_profile_ids:
            raise ContentPackFailure(f"item {item.content_id!r} references unknown aquatic profile {item.aquatic_profile!r}")
    for drop in drops:
        if drop.item not in item_ids:
            raise ContentPackFailure(f"drop {drop.content_id!r} references unknown item {drop.item!r}")
        maximum = next(item.max_stack for item in items if item.content_id == drop.item)
        if drop.quantity > maximum:
            raise ContentPackFailure(f"drop {drop.content_id!r} exceeds the target item's stack bound")
    for enemy in enemies:
        if enemy.drop not in drop_ids:
            raise ContentPackFailure(f"enemy {enemy.content_id!r} references unknown drop {enemy.drop!r}")

    return ContentPack(
        path,
        pack_id,
        pack_version,
        asset_manifest_id,
        air_block,
        default_aquatic_profile,
        features,
        blocks,
        fluids,
        aquatic_profiles,
        items,
        npcs,
        enemies,
        drops,
        effects,
    )


def load_json_document(path: Path, label: str) -> dict[str, object]:
    try:
        raw = path.read_bytes()
    except OSError as error:
        raise ContentPackFailure(f"cannot read {label}: {error}") from error
    if raw.startswith(b"\xef\xbb\xbf") or b"\r" in raw or not raw.endswith(b"\n"):
        raise ContentPackFailure(f"{label} must use canonical UTF-8 with LF and a final newline")
    try:
        return decode_document(raw.decode("utf-8"))
    except UnicodeDecodeError as error:
        raise ContentPackFailure(f"{label} is not valid UTF-8") from error


def load_pack(path: Path = SOURCE) -> ContentPack:
    return validate_document(load_json_document(path, str(path.relative_to(CASE))), load_json_document(ASSET_SOURCE, "assets/manifest.json"))


def haxe_symbol(value: str) -> str:
    local = value.split(":", 1)[-1]
    symbol = "".join(part[:1].upper() + part[1:] for part in re.split(r"[._-]", local))
    if not re.fullmatch(r"[A-Z][A-Za-z0-9]{0,63}", symbol):
        raise ContentPackFailure(f"content ID {value!r} cannot form a bounded Haxe symbol")
    return symbol


def profile_symbol(value: str) -> str:
    return "".join(part[:1].upper() + part[1:] for part in value.split("-"))


def haxe_string(value: str) -> str:
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"') + '"'


def enum_abstract(name: str, values: tuple[str, ...], *, content_values: bool = False) -> list[str]:
    lines = [f"enum abstract {name}(Int) {{"]
    for index, value in enumerate(values):
        symbol = haxe_symbol(value) if content_values else profile_symbol(value)
        lines.append(f"\tvar {symbol} = {index};")
    lines.append("}")
    return lines


def switch_function(
    name: str,
    argument_type: str,
    result_type: str,
    values: tuple[Identified, ...],
    expression: Callable[[Identified], str],
) -> list[str]:
    lines = [f"\tpublic static function {name}(value:{argument_type}):{result_type}", "\t\treturn switch (value) {"]
    for value in values:
        lines.append(f"\t\t\tcase {haxe_symbol(value.content_id)}: {expression(value)};")
    lines.extend(["\t\t}", ""])
    return lines


def render_haxe(pack: ContentPack) -> str:
    block_ids = tuple(value.content_id for value in pack.blocks)
    fluid_ids = tuple(value.content_id for value in pack.fluids)
    aquatic_profile_ids = tuple(value.content_id for value in pack.aquatic_profiles)
    item_ids = tuple(value.content_id for value in pack.items)
    npc_ids = tuple(value.content_id for value in pack.npcs)
    enemy_ids = tuple(value.content_id for value in pack.enemies)
    drop_ids = tuple(value.content_id for value in pack.drops)
    effect_ids = tuple(value.content_id for value in pack.effects)
    asset_ids = tuple(
        sorted(
            {value.icon.asset for value in pack.items}
            | {value.presentation.asset for value in pack.fluids}
            | {value.presentation.asset for value in pack.npcs}
            | {value.presentation.asset for value in pack.enemies}
            | {value.presentation.asset for value in pack.drops}
        )
    )
    lines = [
        "package caxecraft.content;",
        "",
        "import caxecraft.domain.AquaticProfile;",
        "import caxecraft.domain.Aquatics.profile as createAquaticProfile;",
        "import caxecraft.scenario.ContentId;",
        "import caxecraft.scenario.ScenarioContentRegistry;",
        "",
    ]
    for name, values, is_content in (
        ("BaseBlock", block_ids, True),
        ("BaseFluid", fluid_ids, True),
        ("BaseAquaticProfile", aquatic_profile_ids, True),
        ("BaseItem", item_ids, True),
        ("BaseNpc", npc_ids, True),
        ("BaseEnemy", enemy_ids, True),
        ("BaseDrop", drop_ids, True),
        ("BaseEffect", effect_ids, True),
        ("ContentAsset", asset_ids, False),
        ("BlockCollision", tuple(sorted(COLLISIONS)), False),
        ("BlockEdit", tuple(sorted(EDITS)), False),
        ("BlockRenderProfile", tuple(sorted(BLOCK_RENDER_PROFILES)), False),
        ("FluidSimulationProfile", tuple(sorted(FLUID_SIMULATION_PROFILES)), False),
        ("FluidRenderProfile", tuple(sorted(FLUID_RENDER_PROFILES)), False),
        ("FluidCameraProfile", tuple(sorted(FLUID_CAMERA_PROFILES)), False),
        ("FluidAudioProfile", tuple(sorted(FLUID_AUDIO_PROFILES)), False),
        ("ItemUseProfile", tuple(sorted(ITEM_USE_PROFILES)), False),
        ("NpcBehaviorProfile", tuple(sorted(NPC_BEHAVIOR_PROFILES)), False),
        ("EnemyBehaviorProfile", tuple(sorted(ENEMY_BEHAVIOR_PROFILES)), False),
        ("EffectProfile", tuple(sorted(EFFECT_PROFILES)), False),
    ):
        lines.extend(enum_abstract(name, values, content_values=is_content))
        lines.append("")
    lines.extend(
        [
            "/** Atlas and cell coordinates already checked against the visual manifest. */",
            "typedef ContentPresentation = {",
            "\tfinal asset:ContentAsset;",
            "\tfinal cellIndex:Int;",
            "}",
            "",
            "/**",
            " * Typed built-in adapter generated from `packs/caxecraft/base/content.json`.",
            " *",
            " * Edit and validate the JSON source; do not hand-edit this file. Integer",
            " * enums keep closed choices exhaustive in Haxe and cheap in generated C.",
            " * String IDs stay at the scenario/editor boundary instead of becoming",
            " * unvalidated numeric tags in authored files.",
            " */",
            "final class BaseContentPack {",
            f"\tpublic static inline final PACK_VERSION:Int = {pack.pack_version};",
            "",
        ]
    )
    lines.extend(switch_function("blockId", "BaseBlock", "ContentId", pack.blocks, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    lines.extend(switch_function("blockStorageCode", "BaseBlock", "Int", pack.blocks, lambda value: str(value.storage_code)))
    lines.extend(switch_function("blockCollision", "BaseBlock", "BlockCollision", pack.blocks, lambda value: profile_symbol(value.collision)))
    lines.extend(switch_function("blockEdit", "BaseBlock", "BlockEdit", pack.blocks, lambda value: profile_symbol(value.edit)))
    lines.extend(switch_function("blockRenderProfile", "BaseBlock", "BlockRenderProfile", pack.blocks, lambda value: profile_symbol(value.render_profile)))
    lines.extend(switch_function("fluidId", "BaseFluid", "ContentId", pack.fluids, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    lines.extend(
        switch_function(
            "fluidSimulationProfile",
            "BaseFluid",
            "FluidSimulationProfile",
            pack.fluids,
            lambda value: profile_symbol(value.simulation_profile),
        )
    )
    lines.extend(
        switch_function(
            "fluidRenderProfile",
            "BaseFluid",
            "FluidRenderProfile",
            pack.fluids,
            lambda value: profile_symbol(value.render_profile),
        )
    )
    lines.extend(
        switch_function(
            "fluidCameraProfile",
            "BaseFluid",
            "FluidCameraProfile",
            pack.fluids,
            lambda value: profile_symbol(value.camera_profile),
        )
    )
    lines.extend(
        switch_function(
            "fluidAudioProfile",
            "BaseFluid",
            "FluidAudioProfile",
            pack.fluids,
            lambda value: profile_symbol(value.audio_profile),
        )
    )
    lines.extend(
        switch_function(
            "fluidPresentation",
            "BaseFluid",
            "ContentPresentation",
            pack.fluids,
            lambda value: f"{{asset: {profile_symbol(value.presentation.asset)}, cellIndex: {value.presentation.cell_index}}}",
        )
    )
    default_aquatic_symbol = haxe_symbol(pack.default_aquatic_profile)
    lines.extend(
        [
            "\t/** Default movement profile used when no equipped item overrides it. */",
            "\tpublic static inline function defaultAquaticProfile():BaseAquaticProfile",
            f"\t\treturn BaseAquaticProfile.{default_aquatic_symbol};",
            "",
        ]
    )
    for function_name, field_name in (
        ("aquaticMaximumBreathTicks", "maximum_breath_ticks"),
        ("aquaticBreathRecoveryPerTick", "breath_recovery_per_tick"),
        ("aquaticHorizontalControlMilli", "horizontal_control_milli"),
        ("aquaticAscentAccelerationMilli", "ascent_acceleration_milli"),
        ("aquaticDescentAccelerationMilli", "descent_acceleration_milli"),
        ("aquaticBuoyancyAccelerationMilli", "buoyancy_acceleration_milli"),
        ("aquaticDragPerTickMilli", "drag_per_tick_milli"),
        ("aquaticDrowningIntervalTicks", "drowning_interval_ticks"),
    ):
        lines.extend(
            switch_function(
                function_name,
                "BaseAquaticProfile",
                "Int",
                pack.aquatic_profiles,
                lambda value, field=field_name: str(getattr(value, field)),
            )
        )
    for function_name, field_name in (
        ("aquaticUnderwaterMining", "underwater_mining"),
        ("aquaticColdProtection", "cold_protection"),
    ):
        lines.extend(
            switch_function(
                function_name,
                "BaseAquaticProfile",
                "Bool",
                pack.aquatic_profiles,
                lambda value, field=field_name: "true" if getattr(value, field) else "false",
            )
        )
    lines.extend(
        [
            "\t/** Convert reviewed integer pack facts into the generic physics profile. */",
            "\tpublic static function aquaticProfile(value:BaseAquaticProfile):AquaticProfile {",
            "\t\treturn createAquaticProfile(aquaticMaximumBreathTicks(value), aquaticBreathRecoveryPerTick(value), aquaticHorizontalControlMilli(value) / 1000.0,",
            "\t\t\taquaticAscentAccelerationMilli(value) / 1000.0, aquaticDescentAccelerationMilli(value) / 1000.0, aquaticBuoyancyAccelerationMilli(value) / 1000.0,",
            "\t\t\taquaticDragPerTickMilli(value) / 1000.0, aquaticDrowningIntervalTicks(value), aquaticUnderwaterMining(value), aquaticColdProtection(value));",
            "\t}",
            "",
        ]
    )
    item_codes = {value.content_id: index for index, value in enumerate(pack.items)}
    lines.extend(switch_function("itemId", "BaseItem", "ContentId", pack.items, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    lines.extend(switch_function("itemStorageCode", "BaseItem", "Int", pack.items, lambda value: str(item_codes[value.content_id])))
    lines.extend(
        [
            "\t/** True when a map-supplied item code can be converted to `BaseItem`. */",
            "\tpublic static inline function isValidItemStorageCode(code:Int):Bool",
            f"\t\treturn code >= 0 && code < {len(pack.items)};",
            "",
            "\t/**",
            "\t * Convert a code after `isValidItemStorageCode` accepted it.",
            "\t *",
            "\t * The fallback is unreachable for validated generated levels. Keeping it",
            "\t * explicit lets ordinary Haxe retain a non-null closed result in native C.",
            "\t */",
            "\tpublic static function itemFromValidatedStorageCode(code:Int):BaseItem {",
        ]
    )
    for index, item in enumerate(pack.items):
        lines.extend([f"\t\tif (code == {index})", f"\t\t\treturn BaseItem.{haxe_symbol(item.content_id)};"])
    lines.extend([f"\t\treturn BaseItem.{haxe_symbol(pack.items[0].content_id)};", "\t}", ""])
    lines.extend(switch_function("itemMaximumStack", "BaseItem", "Int", pack.items, lambda value: str(value.max_stack)))
    lines.extend(switch_function("itemUseProfile", "BaseItem", "ItemUseProfile", pack.items, lambda value: profile_symbol(value.use_profile)))
    lines.extend(
        switch_function(
            "itemProvidesAquaticProfile",
            "BaseItem",
            "Bool",
            pack.items,
            lambda value: "true" if value.aquatic_profile is not None else "false",
        )
    )
    lines.extend(
        switch_function(
            "itemAquaticProfile",
            "BaseItem",
            "BaseAquaticProfile",
            pack.items,
            lambda value: haxe_symbol(value.aquatic_profile or pack.default_aquatic_profile),
        )
    )
    lines.extend(
        switch_function(
            "itemIcon",
            "BaseItem",
            "ContentPresentation",
            pack.items,
            lambda value: f"{{asset: {profile_symbol(value.icon.asset)}, cellIndex: {value.icon.cell_index}}}",
        )
    )
    lines.extend(switch_function("npcId", "BaseNpc", "ContentId", pack.npcs, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    lines.extend(switch_function("npcBehaviorProfile", "BaseNpc", "NpcBehaviorProfile", pack.npcs, lambda value: profile_symbol(value.behavior_profile)))
    lines.extend(switch_function("npcInteractionRadiusMilli", "BaseNpc", "Int", pack.npcs, lambda value: str(value.interaction_radius_milli)))
    lines.extend(
        switch_function(
            "npcPresentation",
            "BaseNpc",
            "ContentPresentation",
            pack.npcs,
            lambda value: f"{{asset: {profile_symbol(value.presentation.asset)}, cellIndex: {value.presentation.cell_index}}}",
        )
    )
    lines.extend(switch_function("enemyId", "BaseEnemy", "ContentId", pack.enemies, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    for function_name, field_name in (
        ("enemyMaxHealth", "max_health"),
        ("enemyNoticeRadiusMilli", "notice_radius_milli"),
        ("enemyStrikeRadiusMilli", "strike_radius_milli"),
        ("enemyAttackRadiusMilli", "attack_radius_milli"),
        ("enemyWindupTicks", "windup_ticks"),
        ("enemyRecoveryTicks", "recovery_ticks"),
        ("enemyStepMilli", "step_milli"),
    ):
        lines.extend(switch_function(function_name, "BaseEnemy", "Int", pack.enemies, lambda value, field=field_name: str(getattr(value, field))))
    lines.extend(switch_function("enemyBehaviorProfile", "BaseEnemy", "EnemyBehaviorProfile", pack.enemies, lambda value: profile_symbol(value.behavior_profile)))
    lines.extend(
        switch_function(
            "enemyPresentation",
            "BaseEnemy",
            "ContentPresentation",
            pack.enemies,
            lambda value: f"{{asset: {profile_symbol(value.presentation.asset)}, cellIndex: {value.presentation.cell_index}}}",
        )
    )
    lines.extend(switch_function("dropId", "BaseDrop", "ContentId", pack.drops, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    lines.extend(switch_function("dropQuantity", "BaseDrop", "Int", pack.drops, lambda value: str(value.quantity)))
    lines.extend(switch_function("dropPickupRadiusMilli", "BaseDrop", "Int", pack.drops, lambda value: str(value.pickup_radius_milli)))
    lines.extend(switch_function("effectId", "BaseEffect", "ContentId", pack.effects, lambda value: f"new ContentId({haxe_string(value.content_id)})"))
    lines.extend(switch_function("effectProfile", "BaseEffect", "EffectProfile", pack.effects, lambda value: profile_symbol(value.profile)))
    lines.extend(
        [
            "\t/**",
            "\t * Small scalar checksum used by haxe.c's generated-C regression.",
            "\t * It proves that validated pack facts, not a copied C table, reach C.",
            "\t */",
            "\tpublic static function compilerProof():Int {",
            "\t\tfinal mossling = BaseEnemy.Mossling;",
            "\t\tfinal berries = BaseItem.Berries;",
            "\t\tvar proof = PACK_VERSION * 100000;",
            "\t\tproof += enemyMaxHealth(mossling) * 10000;",
            "\t\tproof += enemyWindupTicks(mossling) * 100;",
            "\t\tproof += dropQuantity(BaseDrop.MosslingBerries) * 10;",
            "\t\tproof += itemMaximumStack(berries);",
            "\t\tproof += aquaticMaximumBreathTicks(BaseAquaticProfile.TideweaveAquatics);",
            "\t\treturn proof + fluidPresentation(BaseFluid.Water).cellIndex;",
            "\t}",
            "}",
            "",
            "/** Scenario/editor lookup over the same generated definitions. */",
            "final class BaseContentRegistry implements ScenarioContentRegistry {",
            "\tpublic function new() {}",
            "",
            f"\tpublic function supportsFeature(id:ContentId):Bool\n\t\treturn id.text() == {haxe_string(pack.features[0])};",
            "",
            f"\tpublic function isAirBlock(id:ContentId):Bool\n\t\treturn id.text() == {haxe_string(pack.air_block)};",
            "",
        ]
    )

    def registry_membership(method: str, values: tuple[str, ...]) -> None:
        lines.extend([f"\tpublic function {method}(id:ContentId):Bool {{"])
        for value in values:
            lines.extend([f"\t\tif (id.text() == {haxe_string(value)})", "\t\t\treturn true;"])
        lines.extend(["\t\treturn false;", "\t}", ""])

    registry_membership("hasBlock", block_ids)
    lines.extend(["\tpublic function blockStorageCode(id:ContentId):Int {"])
    for block in pack.blocks:
        lines.extend(
            [
                f"\t\tif (id.text() == {haxe_string(block.content_id)})",
                f"\t\t\treturn {block.storage_code};",
            ]
        )
    lines.extend(["\t\treturn -1;", "\t}", ""])
    registry_membership("hasFluid", fluid_ids)
    lines.extend(["\tpublic function fluidPresentationCell(id:ContentId):Int {"])
    for fluid in pack.fluids:
        lines.extend(
            [
                f"\t\tif (id.text() == {haxe_string(fluid.content_id)})",
                f"\t\t\treturn {fluid.presentation.cell_index};",
            ]
        )
    lines.extend(["\t\treturn -1;", "\t}", ""])
    registry_membership("hasItem", item_ids)
    lines.extend(["\tpublic function itemStorageCode(id:ContentId):Int {"])
    for index, item in enumerate(pack.items):
        lines.extend([f"\t\tif (id.text() == {haxe_string(item.content_id)})", f"\t\t\treturn {index};"])
    lines.extend(["\t\treturn -1;", "\t}", ""])
    registry_membership("hasEntity", enemy_ids)
    registry_membership("hasNpc", npc_ids)
    for method in ("hasPrefab", "hasStatefulObject", "hasState", "hasSignal"):
        lines.extend([f"\tpublic function {method}(id:ContentId):Bool", "\t\treturn false;", ""])
    registry_membership("hasEffect", effect_ids)
    lines.extend(["\tpublic function maximumItemQuantity(id:ContentId):Int {"])
    for item in pack.items:
        lines.extend([f"\t\tif (id.text() == {haxe_string(item.content_id)})", f"\t\t\treturn {item.max_stack};"])
    lines.extend(["\t\treturn 0;", "\t}", "}", ""])
    return "\n".join(lines)


def check_scenario_reference(pack: ContentPack) -> None:
    try:
        lines = SCENARIO_SOURCE.read_text(encoding="utf-8").splitlines()
    except OSError as error:
        raise ContentPackFailure(f"cannot read first-playable CaxeMap: {error}") from error
    references = [line[len("asset-pack ") :] for line in lines if line.startswith("asset-pack ")]
    if references != [pack.logical_path]:
        raise ContentPackFailure(f"first-playable asset-pack must resolve exactly to {pack.logical_path!r}")


def write_generated() -> None:
    pack = load_pack()
    check_scenario_reference(pack)
    OUTPUT.parent.mkdir(parents=True, exist_ok=True)
    OUTPUT.write_text(render_haxe(pack), encoding="utf-8", newline="\n")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if the generated Haxe adapter is stale")
    arguments = parser.parse_args()
    try:
        pack = load_pack()
        check_scenario_reference(pack)
        rendered = render_haxe(pack)
        if arguments.check:
            if not OUTPUT.is_file() or OUTPUT.read_text(encoding="utf-8") != rendered:
                raise ContentPackFailure(f"generated adapter is stale: {OUTPUT.relative_to(CASE)}")
        else:
            OUTPUT.parent.mkdir(parents=True, exist_ok=True)
            OUTPUT.write_text(rendered, encoding="utf-8", newline="\n")
    except (ContentPackFailure, OSError, UnicodeError) as error:
        print(f"caxecraft-content-pack: ERROR: {error}")
        return 1
    action = "current" if arguments.check else "generated"
    print(
        "caxecraft-content-pack: OK: "
        f"{len(pack.blocks)} blocks, {len(pack.fluids)} fluid definitions, {len(pack.aquatic_profiles)} aquatic profiles, "
        f"{len(pack.items)} items, {len(pack.npcs) + len(pack.enemies)} actors; adapter {action}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
