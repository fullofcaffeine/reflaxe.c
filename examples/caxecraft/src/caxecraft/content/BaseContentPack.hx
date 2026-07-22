package caxecraft.content;

import caxecraft.domain.AquaticProfile;
import caxecraft.domain.PlayerAquatics.profile as createAquaticProfile;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioContentRegistry;

enum abstract BaseBlock(Int) {
	var Air = 0;
	var Bedrock = 1;
	var Dirt = 2;
	var Grass = 3;
	var Stone = 4;
}

enum abstract BaseFluid(Int) {
	var Water = 0;
}

enum abstract BaseAquaticProfile(Int) {
	var StandardAquatics = 0;
	var TideweaveAquatics = 1;
}

enum abstract BaseItem(Int) {
	var Berries = 0;
	var Bread = 1;
	var CopperSword = 2;
	var DirtBlock = 3;
	var GrassBlock = 4;
	var Haxeforge = 5;
	var Lantern = 6;
	var StoneBlock = 7;
	var TideweaveSuit = 8;
}

enum abstract BaseNpc(Int) {
	var Nia = 0;
}

enum abstract BaseEnemy(Int) {
	var Mossling = 0;
}

enum abstract BaseDrop(Int) {
	var MosslingBerries = 0;
}

enum abstract BaseEffect(Int) {
	var BerryPickup = 0;
	var CopperStrike = 1;
}

enum abstract ContentAsset(Int) {
	var AdventureItems = 0;
	var Entities = 1;
	var Items = 2;
	var Terrain = 3;
}

enum abstract BlockCollision(Int) {
	var Passable = 0;
	var Solid = 1;
}

enum abstract BlockEdit(Int) {
	var Collectable = 0;
	var Immutable = 1;
}

enum abstract BlockRenderProfile(Int) {
	var Air = 0;
	var FoundationRock = 1;
	var MeadowGrass = 2;
	var RichSoil = 3;
	var SlateStone = 4;
}

enum abstract FluidSimulationProfile(Int) {
	var BoundedWater = 0;
}

enum abstract FluidRenderProfile(Int) {
	var TranslucentVoxel = 0;
}

enum abstract FluidCameraProfile(Int) {
	var ClearSubmersion = 0;
}

enum abstract FluidAudioProfile(Int) {
	var FreshWater = 0;
}

enum abstract ItemUseProfile(Int) {
	var ConsumeOneHeart = 0;
	var EquipAquatic = 1;
	var HaxeforgeTool = 2;
	var LightSource = 3;
	var MeleeSword = 4;
	var None = 5;
	var PlaceBlock = 6;
}

enum abstract NpcBehaviorProfile(Int) {
	var StationaryDialogue = 0;
}

enum abstract EnemyBehaviorProfile(Int) {
	var WanderChaseMelee = 0;
}

enum abstract EffectProfile(Int) {
	var MeleeFeedback = 0;
	var PickupFeedback = 1;
}

/** Atlas and cell coordinates already checked against the visual manifest. */
typedef ContentPresentation = {
	final asset:ContentAsset;
	final cellIndex:Int;
}

/**
 * Typed built-in adapter generated from `packs/caxecraft/base/content.json`.
 *
 * Edit and validate the JSON source; do not hand-edit this file. Integer
 * enums keep closed choices exhaustive in Haxe and cheap in generated C.
 * String IDs stay at the scenario/editor boundary instead of becoming
 * unvalidated numeric tags in authored files.
 */
final class BaseContentPack {
	public static inline final PACK_VERSION:Int = 1;

	public static function blockId(value:BaseBlock):ContentId
		return switch (value) {
			case Air: new ContentId("caxecraft:air");
			case Bedrock: new ContentId("caxecraft:bedrock");
			case Dirt: new ContentId("caxecraft:dirt");
			case Grass: new ContentId("caxecraft:grass");
			case Stone: new ContentId("caxecraft:stone");
		}

	public static function blockStorageCode(value:BaseBlock):Int
		return switch (value) {
			case Air: 0;
			case Bedrock: 4;
			case Dirt: 2;
			case Grass: 1;
			case Stone: 3;
		}

	public static function blockCollision(value:BaseBlock):BlockCollision
		return switch (value) {
			case Air: Passable;
			case Bedrock: Solid;
			case Dirt: Solid;
			case Grass: Solid;
			case Stone: Solid;
		}

	public static function blockEdit(value:BaseBlock):BlockEdit
		return switch (value) {
			case Air: Immutable;
			case Bedrock: Immutable;
			case Dirt: Collectable;
			case Grass: Collectable;
			case Stone: Collectable;
		}

	public static function blockRenderProfile(value:BaseBlock):BlockRenderProfile
		return switch (value) {
			case Air: Air;
			case Bedrock: FoundationRock;
			case Dirt: RichSoil;
			case Grass: MeadowGrass;
			case Stone: SlateStone;
		}

	public static function fluidId(value:BaseFluid):ContentId
		return switch (value) {
			case Water: new ContentId("caxecraft:water");
		}

	public static function fluidSimulationProfile(value:BaseFluid):FluidSimulationProfile
		return switch (value) {
			case Water: BoundedWater;
		}

	public static function fluidRenderProfile(value:BaseFluid):FluidRenderProfile
		return switch (value) {
			case Water: TranslucentVoxel;
		}

	public static function fluidCameraProfile(value:BaseFluid):FluidCameraProfile
		return switch (value) {
			case Water: ClearSubmersion;
		}

	public static function fluidAudioProfile(value:BaseFluid):FluidAudioProfile
		return switch (value) {
			case Water: FreshWater;
		}

	public static function fluidPresentation(value:BaseFluid):ContentPresentation
		return switch (value) {
			case Water: {asset: Terrain, cellIndex: 5};
		}

	/** Default movement profile used when no equipped item overrides it. */
	public static inline function defaultAquaticProfile():BaseAquaticProfile
		return BaseAquaticProfile.StandardAquatics;

	public static function aquaticMaximumBreathTicks(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 120;
			case TideweaveAquatics: 1200;
		}

	public static function aquaticBreathRecoveryPerTick(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 4;
			case TideweaveAquatics: 12;
		}

	public static function aquaticHorizontalControlMilli(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 350;
			case TideweaveAquatics: 900;
		}

	public static function aquaticAscentAccelerationMilli(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 14000;
			case TideweaveAquatics: 18000;
		}

	public static function aquaticDescentAccelerationMilli(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 20000;
			case TideweaveAquatics: 22000;
		}

	public static function aquaticBuoyancyAccelerationMilli(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 12000;
			case TideweaveAquatics: 14000;
		}

	public static function aquaticDragPerTickMilli(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 180;
			case TideweaveAquatics: 100;
		}

	public static function aquaticDrowningIntervalTicks(value:BaseAquaticProfile):Int
		return switch (value) {
			case StandardAquatics: 20;
			case TideweaveAquatics: 20;
		}

	public static function aquaticUnderwaterMining(value:BaseAquaticProfile):Bool
		return switch (value) {
			case StandardAquatics: false;
			case TideweaveAquatics: true;
		}

	public static function aquaticColdProtection(value:BaseAquaticProfile):Bool
		return switch (value) {
			case StandardAquatics: false;
			case TideweaveAquatics: true;
		}

	/** Convert reviewed integer pack facts into the generic physics profile. */
	public static function aquaticProfile(value:BaseAquaticProfile):AquaticProfile {
		return createAquaticProfile(aquaticMaximumBreathTicks(value), aquaticBreathRecoveryPerTick(value), aquaticHorizontalControlMilli(value) / 1000.0,
			aquaticAscentAccelerationMilli(value) / 1000.0, aquaticDescentAccelerationMilli(value) / 1000.0, aquaticBuoyancyAccelerationMilli(value) / 1000.0,
			aquaticDragPerTickMilli(value) / 1000.0, aquaticDrowningIntervalTicks(value), aquaticUnderwaterMining(value), aquaticColdProtection(value));
	}

	public static function itemId(value:BaseItem):ContentId
		return switch (value) {
			case Berries: new ContentId("caxecraft:berries");
			case Bread: new ContentId("caxecraft:bread");
			case CopperSword: new ContentId("caxecraft:copper-sword");
			case DirtBlock: new ContentId("caxecraft:dirt-block");
			case GrassBlock: new ContentId("caxecraft:grass-block");
			case Haxeforge: new ContentId("caxecraft:haxeforge");
			case Lantern: new ContentId("caxecraft:lantern");
			case StoneBlock: new ContentId("caxecraft:stone-block");
			case TideweaveSuit: new ContentId("caxecraft:tideweave-suit");
		}

	public static function itemMaximumStack(value:BaseItem):Int
		return switch (value) {
			case Berries: 64;
			case Bread: 64;
			case CopperSword: 1;
			case DirtBlock: 64;
			case GrassBlock: 64;
			case Haxeforge: 1;
			case Lantern: 64;
			case StoneBlock: 64;
			case TideweaveSuit: 1;
		}

	public static function itemUseProfile(value:BaseItem):ItemUseProfile
		return switch (value) {
			case Berries: ConsumeOneHeart;
			case Bread: None;
			case CopperSword: MeleeSword;
			case DirtBlock: PlaceBlock;
			case GrassBlock: PlaceBlock;
			case Haxeforge: HaxeforgeTool;
			case Lantern: LightSource;
			case StoneBlock: PlaceBlock;
			case TideweaveSuit: EquipAquatic;
		}

	public static function itemProvidesAquaticProfile(value:BaseItem):Bool
		return switch (value) {
			case Berries: false;
			case Bread: false;
			case CopperSword: false;
			case DirtBlock: false;
			case GrassBlock: false;
			case Haxeforge: false;
			case Lantern: false;
			case StoneBlock: false;
			case TideweaveSuit: true;
		}

	public static function itemAquaticProfile(value:BaseItem):BaseAquaticProfile
		return switch (value) {
			case Berries: StandardAquatics;
			case Bread: StandardAquatics;
			case CopperSword: StandardAquatics;
			case DirtBlock: StandardAquatics;
			case GrassBlock: StandardAquatics;
			case Haxeforge: StandardAquatics;
			case Lantern: StandardAquatics;
			case StoneBlock: StandardAquatics;
			case TideweaveSuit: TideweaveAquatics;
		}

	public static function itemIcon(value:BaseItem):ContentPresentation
		return switch (value) {
			case Berries: {asset: Items, cellIndex: 10};
			case Bread: {asset: Items, cellIndex: 11};
			case CopperSword: {asset: Items, cellIndex: 7};
			case DirtBlock: {asset: Items, cellIndex: 1};
			case GrassBlock: {asset: Items, cellIndex: 0};
			case Haxeforge: {asset: Items, cellIndex: 4};
			case Lantern: {asset: Items, cellIndex: 13};
			case StoneBlock: {asset: Items, cellIndex: 2};
			case TideweaveSuit: {asset: AdventureItems, cellIndex: 0};
		}

	public static function npcId(value:BaseNpc):ContentId
		return switch (value) {
			case Nia: new ContentId("caxecraft:nia");
		}

	public static function npcBehaviorProfile(value:BaseNpc):NpcBehaviorProfile
		return switch (value) {
			case Nia: StationaryDialogue;
		}

	public static function npcInteractionRadiusMilli(value:BaseNpc):Int
		return switch (value) {
			case Nia: 3500;
		}

	public static function npcPresentation(value:BaseNpc):ContentPresentation
		return switch (value) {
			case Nia: {asset: Entities, cellIndex: 4};
		}

	public static function enemyId(value:BaseEnemy):ContentId
		return switch (value) {
			case Mossling: new ContentId("caxecraft:mossling");
		}

	public static function enemyMaxHealth(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 3;
		}

	public static function enemyNoticeRadiusMilli(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 6000;
		}

	public static function enemyStrikeRadiusMilli(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 3000;
		}

	public static function enemyAttackRadiusMilli(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 1400;
		}

	public static function enemyWindupTicks(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 8;
		}

	public static function enemyRecoveryTicks(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 12;
		}

	public static function enemyStepMilli(value:BaseEnemy):Int
		return switch (value) {
			case Mossling: 80;
		}

	public static function enemyBehaviorProfile(value:BaseEnemy):EnemyBehaviorProfile
		return switch (value) {
			case Mossling: WanderChaseMelee;
		}

	public static function enemyPresentation(value:BaseEnemy):ContentPresentation
		return switch (value) {
			case Mossling: {asset: Entities, cellIndex: 8};
		}

	public static function dropId(value:BaseDrop):ContentId
		return switch (value) {
			case MosslingBerries: new ContentId("caxecraft:mossling-berries");
		}

	public static function dropQuantity(value:BaseDrop):Int
		return switch (value) {
			case MosslingBerries: 2;
		}

	public static function dropPickupRadiusMilli(value:BaseDrop):Int
		return switch (value) {
			case MosslingBerries: 1500;
		}

	public static function effectId(value:BaseEffect):ContentId
		return switch (value) {
			case BerryPickup: new ContentId("caxecraft:berry-pickup");
			case CopperStrike: new ContentId("caxecraft:copper-strike");
		}

	public static function effectProfile(value:BaseEffect):EffectProfile
		return switch (value) {
			case BerryPickup: PickupFeedback;
			case CopperStrike: MeleeFeedback;
		}

	/**
	 * Small scalar checksum used by haxe.c's generated-C regression.
	 * It proves that validated pack facts, not a copied C table, reach C.
	 */
	public static function compilerProof():Int {
		final mossling = BaseEnemy.Mossling;
		final berries = BaseItem.Berries;
		var proof = PACK_VERSION * 100000;
		proof += enemyMaxHealth(mossling) * 10000;
		proof += enemyWindupTicks(mossling) * 100;
		proof += dropQuantity(BaseDrop.MosslingBerries) * 10;
		proof += itemMaximumStack(berries);
		proof += aquaticMaximumBreathTicks(BaseAquaticProfile.TideweaveAquatics);
		return proof + fluidPresentation(BaseFluid.Water).cellIndex;
	}
}

/** Scenario/editor lookup over the same generated definitions. */
final class BaseContentRegistry implements ScenarioContentRegistry {
	public function new() {}

	public function supportsFeature(id:ContentId):Bool
		return id.text() == "caxecraft:core";

	public function isAirBlock(id:ContentId):Bool
		return id.text() == "caxecraft:air";

	public function hasBlock(id:ContentId):Bool {
		if (id.text() == "caxecraft:air")
			return true;
		if (id.text() == "caxecraft:bedrock")
			return true;
		if (id.text() == "caxecraft:dirt")
			return true;
		if (id.text() == "caxecraft:grass")
			return true;
		if (id.text() == "caxecraft:stone")
			return true;
		return false;
	}

	public function hasFluid(id:ContentId):Bool {
		if (id.text() == "caxecraft:water")
			return true;
		return false;
	}

	public function hasItem(id:ContentId):Bool {
		if (id.text() == "caxecraft:berries")
			return true;
		if (id.text() == "caxecraft:bread")
			return true;
		if (id.text() == "caxecraft:copper-sword")
			return true;
		if (id.text() == "caxecraft:dirt-block")
			return true;
		if (id.text() == "caxecraft:grass-block")
			return true;
		if (id.text() == "caxecraft:haxeforge")
			return true;
		if (id.text() == "caxecraft:lantern")
			return true;
		if (id.text() == "caxecraft:stone-block")
			return true;
		if (id.text() == "caxecraft:tideweave-suit")
			return true;
		return false;
	}

	public function hasEntity(id:ContentId):Bool {
		if (id.text() == "caxecraft:mossling")
			return true;
		return false;
	}

	public function hasNpc(id:ContentId):Bool {
		if (id.text() == "caxecraft:nia")
			return true;
		return false;
	}

	public function hasPrefab(id:ContentId):Bool
		return false;

	public function hasStatefulObject(id:ContentId):Bool
		return false;

	public function hasState(id:ContentId):Bool
		return false;

	public function hasSignal(id:ContentId):Bool
		return false;

	public function hasEffect(id:ContentId):Bool {
		if (id.text() == "caxecraft:berry-pickup")
			return true;
		if (id.text() == "caxecraft:copper-strike")
			return true;
		return false;
	}

	public function maximumItemQuantity(id:ContentId):Int {
		if (id.text() == "caxecraft:berries")
			return 64;
		if (id.text() == "caxecraft:bread")
			return 64;
		if (id.text() == "caxecraft:copper-sword")
			return 1;
		if (id.text() == "caxecraft:dirt-block")
			return 64;
		if (id.text() == "caxecraft:grass-block")
			return 64;
		if (id.text() == "caxecraft:haxeforge")
			return 1;
		if (id.text() == "caxecraft:lantern")
			return 64;
		if (id.text() == "caxecraft:stone-block")
			return 64;
		if (id.text() == "caxecraft:tideweave-suit")
			return 1;
		return 0;
	}
}
