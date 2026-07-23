package caxecraft.qa;

import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseBlock;
import caxecraft.content.BaseContentPack.BaseAquaticProfile;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.content.BaseContentPack.BaseEffect;
import caxecraft.content.BaseContentPack.BaseEnemy;
import caxecraft.content.BaseContentPack.BaseItem;
import caxecraft.content.BaseContentPack.BaseFluid;
import caxecraft.content.BaseContentPack.BaseNpc;
import caxecraft.content.BaseContentPack.BlockCollision;
import caxecraft.content.BaseContentPack.BlockRenderProfile;
import caxecraft.content.BaseContentPack.ContentAsset;
import caxecraft.content.BaseContentPack.FluidRenderProfile;
import caxecraft.editor.EditorSession;
import caxecraft.editor.EditorTypes.EditorOpenResult;
import caxecraft.editor.EditorTypes.EditorValidationResult;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioCodecModel.ScenarioReadResult;
import caxecraft.scenario.ScenarioLexer;
import caxecraft.scenario.ScenarioParser;
import caxecraft.scenario.ScenarioValidator;
import sys.io.File;

/**
 * Reads the real first-playable map through the generated base-pack registry.
 *
 * This Eval probe checks the authoring boundary that is not native-loadable
 * yet: JSON becomes typed Haxe, then CaxeMap names resolve against those exact
 * definitions. The C-specific companion probe checks the scalar adapter path.
 */
final class ContentPackProbe {
	static function main():Void {
		final registry = new BaseContentRegistry();
		require(BaseContentPack.PACK_VERSION == 1, "pack version");
		require(BaseContentPack.compilerProof() == 132089, "stable scalar proof");
		require(BaseContentPack.blockStorageCode(BaseBlock.Air) == 0, "air storage code");
		require(BaseContentPack.blockCollision(BaseBlock.Stone) == BlockCollision.Solid, "stone collision");
		require(BaseContentPack.blockStorageCode(BaseBlock.Sand) == 5, "sand storage code");
		require(BaseContentPack.blockStorageCode(BaseBlock.Wood) == 6, "wood storage code");
		require(BaseContentPack.blockStorageCode(BaseBlock.Leaves) == 7, "leaves storage code");
		require(BaseContentPack.blockStorageCode(BaseBlock.Snow) == 8, "snow storage code");
		require(BaseContentPack.blockStorageCode(BaseBlock.Ash) == 9, "ash storage code");
		require(BaseContentPack.blockRenderProfile(BaseBlock.Snow) == BlockRenderProfile.SnowField, "snow render profile");
		require(BaseContentPack.itemMaximumStack(BaseItem.Berries) == 64, "berry stack bound");
		final tideweaveCode = registry.itemStorageCode(new ContentId("caxecraft:tideweave-suit"));
		require(BaseContentPack.isValidItemStorageCode(tideweaveCode), "Tideweave storage code");
		require(BaseContentPack.itemFromValidatedStorageCode(tideweaveCode) == BaseItem.TideweaveSuit, "Tideweave storage round trip");
		require(BaseContentPack.fluidId(BaseFluid.Water).text() == "caxecraft:water", "water identity");
		require(BaseContentPack.fluidRenderProfile(BaseFluid.Water) == FluidRenderProfile.TranslucentVoxel, "water render profile");
		require(BaseContentPack.fluidPresentation(BaseFluid.Water).asset == ContentAsset.Terrain, "water atlas");
		require(BaseContentPack.defaultAquaticProfile() == BaseAquaticProfile.StandardAquatics, "default aquatics");
		require(BaseContentPack.itemProvidesAquaticProfile(BaseItem.TideweaveSuit), "Tideweave capability");
		require(BaseContentPack.itemAquaticProfile(BaseItem.TideweaveSuit) == BaseAquaticProfile.TideweaveAquatics, "Tideweave profile");
		final tideweave = BaseContentPack.aquaticProfile(BaseAquaticProfile.TideweaveAquatics);
		require(tideweave.maximumBreathTicks == 1200
			&& tideweave.underwaterMining
			&& tideweave.coldProtection, "resolved Tideweave facts");
		require(BaseContentPack.enemyMaxHealth(BaseEnemy.Mossling) == 3, "Mossling health");
		require(BaseContentPack.enemyWindupTicks(BaseEnemy.Mossling) == 8, "Mossling wind-up");
		require(BaseContentPack.npcInteractionRadiusMilli(BaseNpc.Nia) == 3500, "NPC interaction radius");
		require(BaseContentPack.npcPresentation(BaseNpc.Nia).asset == ContentAsset.Entities, "NPC presentation asset");
		require(BaseContentPack.effectId(BaseEffect.CopperStrike).text() == "caxecraft:copper-strike", "effect identity");
		require(registry.hasNpc(new ContentId("caxecraft:nia")), "registered Nia");
		require(registry.hasFluid(new ContentId("caxecraft:water")), "registered water");
		require(registry.hasEntity(new ContentId("caxecraft:mossling")), "registered Mossling");
		require(!registry.hasNpc(new ContentId("caxecraft:mossling")), "wrong-kind actor rejection");
		require(registry.maximumItemQuantity(new ContentId("caxecraft:copper-sword")) == 1, "sword stack bound");

		final bytes = File.getBytes("scenarios/first-playable/map.caxemap");
		final records = switch ScenarioLexer.read(bytes) {
			case ReadOk(value): value;
			case ReadError(_): fail("first-playable lexer");
		};
		final parsed = switch ScenarioParser.parse(records) {
			case ReadOk(value): value;
			case ReadError(_): fail("first-playable parser");
		};
		final scenario = switch ScenarioValidator.validate(parsed, registry) {
			case ReadOk(value): value;
			case ReadError(_): fail("first-playable content resolution");
		};
		require(scenario.assetPack.text() == "packs/caxecraft/base", "map pack reference");
		require(scenario.world.fluids.length == 2, "validated authored water");
		require(scenario.objects.length == 3, "validated placed objects and Tideweave");
		final editor = switch EditorSession.open(parsed.candidate, registry) {
			case EditorOpened(value): value;
			case EditorOpenRejected(_): fail("editor opens with base-pack definitions");
		};
		switch editor.validate() {
			case ValidationPassed(_):
			case ValidationFailed(_) | ValidationBlocked(_):
				fail("editor validates against base-pack definitions");
		}

		Sys.println("caxecraft-content-pack: typed JSON adapter and first-playable resolution passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}

	static function fail<T>(message:String):T
		throw message;
}
