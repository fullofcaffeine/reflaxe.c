package caxecraft.qa;

import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.gameplay.GuideNpc;
import caxecraft.gameplay.GuidePhase;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.ItemKind;
import caxecraft.gameplay.Mossling;
import caxecraft.gameplay.MosslingMode;

/** Renderer-independent proof for the first friendly and hostile actors. */
final class GameplayProbe {
	static function main():Void {
		final storage:Array<Int> = [];
		final cells:WorldCells = storage;
		World.generate(cells, 0x0cafe);
		World.prepareSpawnMeadow(cells);
		require(World.surfaceY(cells, 16, 16) >= 3, "generated surface is discoverable");
		require(World.surfaceY(cells, 16, 16) == 4, "authored spawn meadow is level");
		require(World.surfaceY(cells, -1, 16) == -1, "outside columns have no surface");

		var guide = GuideNpc.start(cells, 17.5, 13.5);
		require(GuideNpc.phase(guide) == GuidePhase.Waiting, "guide starts ready to meet");
		require(GuideNpc.isInRange(guide, 16.5, 16.5), "spawn can reach guide interaction");
		guide = GuideNpc.interact(guide);
		require(GuideNpc.phase(guide) == GuidePhase.Welcomed, "first interaction welcomes");
		require(GuideNpc.sharesBerriesOnNextInteraction(guide), "second interaction declares gift");
		var inventory = Inventory.starter();
		final berriesBefore = inventory.berries;
		inventory = Inventory.collectItem(inventory, ItemKind.Berries, 2);
		guide = GuideNpc.interact(guide);
		require(GuideNpc.phase(guide) == GuidePhase.SharedBerries && inventory.berries == berriesBefore + 2, "one-time gift is explicit");
		require(GuideNpc.phase(GuideNpc.interact(guide)) == GuidePhase.SharedBerries, "finished conversation is stable");

		var mossling = Mossling.start(cells, 12.5, 12.5);
		final initialX = mossling.x;
		mossling = Mossling.step(cells, mossling, 15.5, 12.5);
		require(Mossling.mode(mossling) == MosslingMode.Chasing && mossling.x > initialX, "near player is chased one bounded step");
		final chasedX = mossling.x;
		mossling = Mossling.step(cells, mossling, 30.5, 30.5);
		require(Mossling.mode(mossling) == MosslingMode.Returning && mossling.x < chasedX, "far player releases creature toward home");

		Sys.println("caxecraft-gameplay: Nia welcome/gift and Mossling rest/chase/return passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
