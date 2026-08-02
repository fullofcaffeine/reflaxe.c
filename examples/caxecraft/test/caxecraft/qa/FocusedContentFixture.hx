package caxecraft.qa;

import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.LevelContentResolver;
import caxecraft.content.LevelContentResolver.FluidContentResolution;
import caxecraft.content.LevelContentResolver.ItemContentResolution;
import caxecraft.content.LevelContentResolver.ItemStorageCode;
import caxecraft.content.LevelContentResolver.LevelFluidSimulation;
import caxecraft.content.LevelContentResolver.TerrainContentResolution;
import caxecraft.content.LevelContentResolver.TerrainStorageCode;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.AquaticProfile;
import caxecraft.domain.Aquatics.profile as createAquaticProfile;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioContentRegistry;

/**
 * Supplies the smallest manually reviewed content oracle used by focused tests.
 *
 * Product tests that prove JSON loading use `RuntimeContentRegistry` and the
 * real packaged files. Lower-level actor, level, and session tests need only a
 * few stable mechanics facts, so this module states those expectations by hand
 * and avoids making file I/O or schema decoding part of their failure surface.
 */
/** Standard movement and survival facts expected by focused game tests. */
function standardAquaticProfile():AquaticProfile
	return createAquaticProfile(120, 4, 0.35, 14.0, 20.0, 12.0, 0.18, 20, false, false);

/** Equipped Tideweave movement and survival facts expected by session tests. */
function tideweaveAquaticProfile():AquaticProfile
	return createAquaticProfile(1200, 12, 0.9, 18.0, 22.0, 14.0, 0.1, 20, true, true);

/**
 * Resolves only the authored facts needed by focused engine regressions.
 *
 * A class is appropriate despite the lack of mutable state because the parser
 * and level planner require nominal `ScenarioContentRegistry` and
 * `LevelContentResolver` implementations. Module functions cannot satisfy
 * those interfaces. The real game never constructs this test-only type.
 */
final class FocusedContentRegistry implements ScenarioContentRegistry implements LevelContentResolver {
	/** Construct one stateless, manually reviewed test oracle. */
	public function new() {}

	/** Admit the one engine feature required by current focused maps. */
	public function supportsFeature(id:ContentId):Bool
		return id.text() == "caxecraft:core";

	/** Identify the one terrain value that means empty space. */
	public function isAirBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air";

	/** Report whether the fixture owns a compact terrain code for this ID. */
	public function hasBlock(id:ContentId):Bool
		return terrainStorageCode(id) >= 0;

	/** Return the manually reviewed compact terrain byte, or `-1` if unknown. */
	public function blockStorageCode(id:ContentId):Int
		return terrainStorageCode(id);

	/** Admit the bounded-water definition used by current focused maps. */
	public function hasFluid(id:ContentId):Bool
		return id.text() == "caxecraft:water";

	/** Report whether the fixture owns a stable item code for this ID. */
	public function hasItem(id:ContentId):Bool
		return itemStorageCode(id) >= 0;

	/** Return the manually reviewed pack-local item code, or `-1` if unknown. */
	public function itemStorageCode(id:ContentId):Int {
		return switch id.text() {
			case "caxecraft:berries": 0;
			case "caxecraft:bread": 1;
			case "caxecraft:copper-sword": 2;
			case "caxecraft:dirt-block": 3;
			case "caxecraft:grass-block": 4;
			case "caxecraft:haxeforge": 5;
			case "caxecraft:lantern": 6;
			case "caxecraft:stone-block": 7;
			case "caxecraft:tideweave-suit": 8;
			case _: -1;
		}
	}

	/** Admit the one enemy profile used by current focused maps. */
	public function hasEntity(id:ContentId):Bool
		return id.text() == "caxecraft:mossling";

	/** Admit the one non-player character used by current focused maps. */
	public function hasNpc(id:ContentId):Bool
		return id.text() == "caxecraft:nia";

	/** Prefab definitions are outside this fixture's admitted slice. */
	public function hasPrefab(id:ContentId):Bool
		return false;

	/** Stateful-object definitions are outside this fixture's admitted slice. */
	public function hasStatefulObject(id:ContentId):Bool
		return false;

	/** Named state definitions are outside this fixture's admitted slice. */
	public function hasState(id:ContentId):Bool
		return false;

	/** Admit the two feedback effects referenced by existing scenario tests. */
	public function hasEffect(id:ContentId):Bool
		return id.text() == "caxecraft:berry-pickup" || id.text() == "caxecraft:copper-strike";

	/** Named signal definitions are outside this fixture's admitted slice. */
	public function hasSignal(id:ContentId):Bool
		return false;

	/** Return the manually authored inventory bound, or zero if unknown. */
	public function maximumItemQuantity(id:ContentId):Int {
		return switch id.text() {
			case "caxecraft:berries", "caxecraft:bread", "caxecraft:dirt-block", "caxecraft:grass-block", "caxecraft:lantern", "caxecraft:stone-block": 64;
			case "caxecraft:copper-sword", "caxecraft:haxeforge", "caxecraft:tideweave-suit": 1;
			case _: 0;
		}
	}

	/** Resolve one known terrain ID into its nominal storage value. */
	public function resolveTerrain(id:ContentId):TerrainContentResolution {
		final code = terrainStorageCode(id);
		return code < 0 ? UnknownTerrainContent : TerrainContentResolved(TerrainStorageCode.fromValidated(code));
	}

	/** Resolve water into the bounded simulator and reviewed atlas cell. */
	public function resolveFluid(id:ContentId):FluidContentResolution
		return hasFluid(id) ? FluidContentResolved(LevelFluidSimulation.BoundedWater, 5) : UnknownFluidContent;

	/** Resolve one known item ID into its pack-local storage value. */
	public function resolveItem(id:ContentId):ItemContentResolution {
		final code = itemStorageCode(id);
		return code < 0 ? UnknownItemContent : ItemContentResolved(ItemStorageCode.fromValidated(code));
	}

	/** Resolve Nia's manually reviewed mechanics or report exact kind/absence. */
	public function resolveNpc(id:ContentId):ActorContentResolution {
		if (id.text() == "caxecraft:nia")
			return ActorContentResolved({
				maximumHealth: 6,
				aquaticProfile: standardAquaticProfile(),
				controller: StationaryDialogue(3500)
			});
		return id.text() == "caxecraft:mossling" ? WrongActorContentKind(EnemyContent) : UnknownActorContent;
	}

	/** Resolve Mossling's manually reviewed mechanics or report exact kind/absence. */
	public function resolveEnemy(id:ContentId):ActorContentResolution {
		if (id.text() == "caxecraft:mossling")
			return ActorContentResolved({
				maximumHealth: 3,
				aquaticProfile: standardAquaticProfile(),
				controller: WanderChaseMelee({
					noticeRadiusMilli: 6000,
					strikeRadiusMilli: 3000,
					attackRadiusMilli: 1400,
					windupTicks: 8,
					recoveryTicks: 12,
					stepMilli: 80,
					drop: new ContentId("caxecraft:mossling-berries")
				})
			});
		return id.text() == "caxecraft:nia" ? WrongActorContentKind(NpcContent) : UnknownActorContent;
	}

	/** Map one authored terrain ID to the exact byte asserted by level tests. */
	function terrainStorageCode(id:ContentId):Int {
		return switch id.text() {
			case "caxecraft:air": 0;
			case "caxecraft:ash": 9;
			case "caxecraft:bedrock": 4;
			case "caxecraft:dirt": 2;
			case "caxecraft:grass": 1;
			case "caxecraft:leaves": 7;
			case "caxecraft:sand": 5;
			case "caxecraft:snow": 8;
			case "caxecraft:stone": 3;
			case "caxecraft:wood": 6;
			case _: -1;
		}
	}
}
