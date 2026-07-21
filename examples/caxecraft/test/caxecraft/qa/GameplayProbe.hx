package caxecraft.qa;

import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.gameplay.GuideNpc;
import caxecraft.gameplay.GuidePhase;
import caxecraft.gameplay.BerryDrop;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.ItemKind;
import caxecraft.gameplay.Mossling;
import caxecraft.gameplay.MosslingMode;
import caxecraft.gameplay.PlayerVitals;

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

		var target = Mossling.start(cells, 12.5, 12.5);
		inventory = Inventory.select(inventory, 4);
		require(Inventory.selectedIs(inventory, ItemKind.CopperSword), "combat asks for a semantic sword rather than a slot number");
		require(Mossling.canStrike(target, 10.5, 12.5, 1.0, 0.0), "near aimed sword reaches Mossling");
		require(!Mossling.canStrike(target, 10.5, 12.5, -1.0, 0.0), "sword does not strike behind player");
		target = Mossling.strike(target);
		target = Mossling.strike(target);
		require(target.health == 1 && Mossling.isAlive(target), "two strikes leave one health");
		target = Mossling.strike(target);
		require(target.health == 0 && !Mossling.isAlive(target), "third strike defeats Mossling");
		require(Mossling.step(cells, target, 10.5, 12.5) == target, "defeated Mossling state is stable");

		var drop = BerryDrop.fromDefeatedMossling(target);
		require(drop.active && drop.amount == 2, "defeat creates one visible berry drop");
		require(BerryDrop.isInRange(drop, drop.x, drop.y, drop.z), "near player can collect drop");
		final berriesBeforeDrop = inventory.berries;
		inventory = Inventory.collectItem(inventory, ItemKind.Berries, drop.amount);
		drop = BerryDrop.collect(drop);
		require(!drop.active && inventory.berries == berriesBeforeDrop + 2, "drop collection is one-time and explicit");

		var vitals = PlayerVitals.start();
		vitals = PlayerVitals.step(vitals, 10.5, 12.5, 10.8, 12.5, true);
		require(vitals.health == PlayerVitals.MAX_HEALTH - 1
			&& vitals.safeTicks == PlayerVitals.CONTACT_SAFE_TICKS, "first contact deals one bounded point");
		final healthAfterContact = vitals.health;
		vitals = PlayerVitals.step(vitals, 10.5, 12.5, 10.8, 12.5, true);
		require(vitals.health == healthAfterContact
			&& vitals.safeTicks == PlayerVitals.CONTACT_SAFE_TICKS - 1, "safe ticks prevent instant repeated damage");
		while (!PlayerVitals.isDefeated(vitals)) {
			while (vitals.safeTicks > 0)
				vitals = PlayerVitals.step(vitals, 10.5, 12.5, 30.0, 30.0, false);
			vitals = PlayerVitals.step(vitals, 10.5, 12.5, 10.8, 12.5, true);
		}
		require(PlayerVitals.revive(vitals).health == PlayerVitals.MAX_HEALTH, "return prompt restores full health");

		Sys.println("caxecraft-gameplay: Nia gift, Mossling combat/drop, and bounded player health passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
