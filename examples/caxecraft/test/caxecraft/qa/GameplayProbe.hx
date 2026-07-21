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
import caxecraft.gameplay.Recovery;
import caxecraft.gameplay.RecoveryDecision;

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
		var waitingGift = GuideNpc.interact(GuideNpc.start(cells, 17.5, 13.5));
		final fullBerries = Inventory.make(5, 0, 0, 0, 0, 0, Inventory.MAX_STACK, 0, 0);
		final acceptedGift = Inventory.acceptedAmount(fullBerries, ItemKind.Berries, 2);
		if (acceptedGift == 2)
			waitingGift = GuideNpc.interact(waitingGift);
		require(acceptedGift == 0
			&& GuideNpc.phase(waitingGift) == GuidePhase.Welcomed, "full stack keeps Nia's gift available for retry");

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
		drop = BerryDrop.collectAmount(drop, drop.amount);
		require(!drop.active && inventory.berries == berriesBeforeDrop + 2, "drop collection is one-time and explicit");

		var partialDrop = BerryDrop.fromDefeatedMossling(target);
		var nearlyFull = Inventory.make(5, 0, 0, 0, 0, 0, Inventory.MAX_STACK - 1, 0, 0);
		final acceptedDrop = Inventory.acceptedAmount(nearlyFull, ItemKind.Berries, partialDrop.amount);
		nearlyFull = Inventory.collectItem(nearlyFull, ItemKind.Berries, acceptedDrop);
		partialDrop = BerryDrop.collectAmount(partialDrop, acceptedDrop);
		require(acceptedDrop == 1 && nearlyFull.berries == Inventory.MAX_STACK && partialDrop.active && partialDrop.amount == 1,
			"partial pickup leaves the uncollected berry visible");
		final fullAcceptedDrop = Inventory.acceptedAmount(nearlyFull, ItemKind.Berries, partialDrop.amount);
		require(fullAcceptedDrop == 0
			&& BerryDrop.collectAmount(partialDrop, fullAcceptedDrop) == partialDrop, "full stack cannot destroy the remaining drop");

		var vitals = PlayerVitals.start();
		vitals = PlayerVitals.step(vitals, 10.5, 12.5, 10.8, 12.5, true);
		require(vitals.health == PlayerVitals.MAX_HEALTH - 1
			&& vitals.safeTicks == PlayerVitals.CONTACT_SAFE_TICKS, "first contact deals one bounded point");
		final healthAfterContact = vitals.health;
		vitals = PlayerVitals.step(vitals, 10.5, 12.5, 10.8, 12.5, true);
		require(vitals.health == healthAfterContact
			&& vitals.safeTicks == PlayerVitals.CONTACT_SAFE_TICKS - 1, "safe ticks prevent instant repeated damage");

		inventory = Inventory.select(inventory, 5);
		vitals = PlayerVitals.startAt(PlayerVitals.MAX_HEALTH - Recovery.BERRY_HEALTH);
		final berriesBeforeRecovery = inventory.berries;
		final safeTicksBeforeRecovery = vitals.safeTicks;
		var recoveryDecision = Recovery.decide(inventory, vitals);
		require(recoveryDecision == RecoveryDecision.UseBerries, "damaged player can use selected berries");
		inventory = Recovery.applyInventory(recoveryDecision, inventory);
		vitals = Recovery.applyVitals(recoveryDecision, vitals);
		require(inventory.berries == berriesBeforeRecovery - 1
			&& vitals.health == PlayerVitals.MAX_HEALTH
			&& vitals.safeTicks == safeTicksBeforeRecovery,
			"berries restore one heart without changing damage safety");
		recoveryDecision = Recovery.decide(inventory, vitals);
		require(recoveryDecision == RecoveryDecision.HealthAlreadyFull
			&& Recovery.applyInventory(recoveryDecision, inventory) == inventory,
			"full health preserves the berry stack");
		final emptyRecovery = Inventory.make(5, 0, 0, 0, 0, 0, 0, 0, 0);
		require(Recovery.decide(emptyRecovery, PlayerVitals.startAt(2)) == RecoveryDecision.RecoveryStackEmpty, "empty berry stack is explicit");
		require(Recovery.decide(Inventory.select(inventory, 0), PlayerVitals.startAt(2)) == RecoveryDecision.NotRecoveryItem,
			"block placement remains a separate secondary action");

		while (!PlayerVitals.isDefeated(vitals)) {
			while (vitals.safeTicks > 0)
				vitals = PlayerVitals.step(vitals, 10.5, 12.5, 30.0, 30.0, false);
			vitals = PlayerVitals.step(vitals, 10.5, 12.5, 10.8, 12.5, true);
		}
		require(PlayerVitals.revive(vitals).health == PlayerVitals.MAX_HEALTH, "return prompt restores full health");

		require(Recovery.decide(inventory, vitals) == RecoveryDecision.PlayerDefeated, "fallen player cannot consume recovery items");

		Sys.println("caxecraft-gameplay: lossless gifts/drops, Mossling combat, berry recovery, and bounded player health passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
