package caxecraft.qa;

import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseBlock;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.content.BaseContentPack.BaseEffect;
import caxecraft.content.BaseContentPack.BaseEnemy;
import caxecraft.content.BaseContentPack.BaseItem;
import caxecraft.content.BaseContentPack.BaseNpc;
import caxecraft.content.BaseContentPack.BlockCollision;
import caxecraft.content.BaseContentPack.ContentAsset;
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
		require(BaseContentPack.compilerProof() == 130884, "stable scalar proof");
		require(BaseContentPack.blockStorageCode(BaseBlock.Air) == 0, "air storage code");
		require(BaseContentPack.blockCollision(BaseBlock.Stone) == BlockCollision.Solid, "stone collision");
		require(BaseContentPack.itemMaximumStack(BaseItem.Berries) == 64, "berry stack bound");
		require(BaseContentPack.enemyMaxHealth(BaseEnemy.Mossling) == 3, "Mossling health");
		require(BaseContentPack.enemyWindupTicks(BaseEnemy.Mossling) == 8, "Mossling wind-up");
		require(BaseContentPack.npcInteractionRadiusMilli(BaseNpc.Nia) == 3500, "NPC interaction radius");
		require(BaseContentPack.npcPresentation(BaseNpc.Nia).asset == ContentAsset.Entities, "NPC presentation asset");
		require(BaseContentPack.effectId(BaseEffect.CopperStrike).text() == "caxecraft:copper-strike", "effect identity");
		require(registry.hasNpc(new ContentId("caxecraft:nia")), "registered Nia");
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
		require(scenario.objects.length == 2, "validated placed objects");
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
