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
import caxecraft.gameplay.Mining;
import caxecraft.gameplay.MiningOutcome;
import caxecraft.gameplay.PlayerVitals;
import caxecraft.gameplay.Recovery;
import caxecraft.gameplay.RecoveryDecision;
import caxecraft.gameplay.SwordCombat;
import caxecraft.gameplay.SwordCombatDecision;

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

		// The same operation runs here under Eval and in the native Raylib pilot.
		// Checking capacity before removal is what makes a rejected mine lossless.
		final minedCoordinate = World.coord(16, 4, 16);
		var miningInventory = Inventory.starter();
		final grassBeforeMining = miningInventory.grass;
		var mining = Mining.attempt(cells, minedCoordinate, miningInventory);
		miningInventory = mining.inventory;
		require(mining.outcome == MiningOutcome.Collected
			&& World.query(cells, minedCoordinate) == caxecraft.domain.BlockKind.Air
			&& miningInventory.grass == grassBeforeMining + 1,
			"available capacity moves exactly one block into its matching stack");
		require(World.replace(cells, minedCoordinate, caxecraft.domain.BlockKind.Grass), "mining fixture restores its grass block");
		final fullMiningInventory = Inventory.make(0, Inventory.MAX_STACK, 0, 0, 0, 0, 0, 0, 0);
		mining = Mining.attempt(cells, minedCoordinate, fullMiningInventory);
		require(mining.outcome == MiningOutcome.InventoryFull
			&& mining.inventory == fullMiningInventory
			&& World.query(cells, minedCoordinate) == caxecraft.domain.BlockKind.Grass,
			"full matching stack leaves both the block and inventory unchanged");
		final bedrockMining = Mining.attempt(cells, World.coord(16, 0, 16), fullMiningInventory);
		require(bedrockMining.outcome == MiningOutcome.BlockUnavailable
			&& World.query(cells, World.coord(16, 0, 16)) == caxecraft.domain.BlockKind.Bedrock,
			"immutable bedrock remains a separate non-capacity outcome");

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
		mossling = Mossling.step(cells, mossling, 15.5, 12.5, 0);
		require(Mossling.mode(mossling) == MosslingMode.Chasing && mossling.x > initialX, "near player is chased one bounded step");
		final chasedX = mossling.x;
		mossling = Mossling.step(cells, mossling, 30.5, 30.5, 1);
		require(Mossling.mode(mossling) == MosslingMode.Returning && mossling.x < chasedX, "far player releases creature toward home");

		var wanderA = Mossling.start(cells, 12.5, 12.5);
		var wanderB = Mossling.start(cells, 12.5, 12.5);
		var wanderTick = 0;
		while (wanderTick < 240) {
			wanderA = Mossling.step(cells, wanderA, 30.5, 30.5, wanderTick);
			wanderB = Mossling.step(cells, wanderB, 30.5, 30.5, wanderTick);
			require(sameMossling(wanderA, wanderB), "identical fixed ticks choose the same wander route");
			require(absolute(wanderA.x - wanderA.homeX) <= 1.08 && absolute(wanderA.z - wanderA.homeZ) <= 1.08,
				"wander route remains inside its declared home square");
			wanderTick++;
		}
		require(Mossling.mode(Mossling.step(cells, Mossling.start(cells, 12.5, 12.5), 30.5, 30.5, 20)) == MosslingMode.Wandering,
			"far Mossling begins its bounded route after resting");

		var attacker = Mossling.start(cells, 12.5, 12.5);
		attacker = Mossling.step(cells, attacker, 13.5, 12.5, 0);
		require(Mossling.mode(attacker) == MosslingMode.Windup && attacker.phaseTicks == Mossling.WINDUP_TICKS,
			"attack begins with a visible fixed-tick warning");
		var windupTicks = 0;
		while (attacker.phaseTicks > 1) {
			require(!Mossling.attacksThisTick(attacker, 13.5, 12.5), "warning ticks cannot damage early");
			attacker = Mossling.step(cells, attacker, 13.5, 12.5, windupTicks + 1);
			windupTicks++;
		}
		require(windupTicks == Mossling.WINDUP_TICKS - 1
			&& Mossling.attacksThisTick(attacker, 13.5, 12.5), "wind-up produces one exact impact tick");
		attacker = Mossling.step(cells, attacker, 13.5, 12.5, windupTicks + 1);
		require(Mossling.mode(attacker) == MosslingMode.Recovering
			&& attacker.phaseTicks == Mossling.RECOVERY_TICKS
			&& !Mossling.attacksThisTick(attacker, 13.5, 12.5),
			"impact enters a non-damaging recovery period");
		var escaped = Mossling.step(cells, Mossling.start(cells, 12.5, 12.5), 13.5, 12.5, 0);
		escaped = Mossling.step(cells, escaped, 16.5, 12.5, 1);
		require(Mossling.mode(escaped) != MosslingMode.Windup && !Mossling.attacksThisTick(escaped, 16.5, 12.5),
			"leaving the marked range cancels the pending hit");

		var target = Mossling.start(cells, 12.5, 12.5);
		inventory = Inventory.select(inventory, 4);
		require(Inventory.selectedIs(inventory, ItemKind.CopperSword), "combat asks for a semantic sword rather than a slot number");
		require(Mossling.canStrike(target, 10.5, 12.5, 1.0, 0.0), "near aimed sword reaches Mossling");
		require(!Mossling.canStrike(target, 10.5, 12.5, -1.0, 0.0), "sword does not strike behind player");
		var swordCombat = SwordCombat.start();
		var swordDecision = SwordCombat.decide(swordCombat, inventory, PlayerVitals.start(), target, 10.5, 12.5, 1.0, 0.0);
		require(swordDecision == SwordCombatDecision.Hit, "ready aimed sword action is admitted on the fixed clock");
		swordCombat = SwordCombat.after(swordDecision, swordCombat);
		swordCombat = SwordCombat.step(swordCombat);
		require(SwordCombat.decide(swordCombat, inventory, PlayerVitals.start(), target, 10.5, 12.5, 1.0, 0.0) == SwordCombatDecision.CoolingDown,
			"rapid display-frame presses cannot bypass the sword cooldown");
		while (swordCombat.cooldownTicks > 0)
			swordCombat = SwordCombat.step(swordCombat);
		require(SwordCombat.decide(swordCombat, inventory, PlayerVitals.start(), target, 10.5, 12.5, 1.0, 0.0) == SwordCombatDecision.Hit,
			"sword becomes ready after exactly the declared fixed ticks");
		require(SwordCombat.decide(swordCombat, Inventory.select(inventory, 0), PlayerVitals.start(), target, 10.5, 12.5, 1.0,
			0.0) == SwordCombatDecision.NotSword,
			"non-sword primary actions remain outside combat");
		require(SwordCombat.decide(swordCombat, inventory, PlayerVitals.start(), target, 10.5, 12.5, -1.0, 0.0) == SwordCombatDecision.TargetMissed,
			"fixed-step combat preserves the aiming boundary");
		target = Mossling.strike(target);
		target = Mossling.strike(target);
		require(target.health == 1 && Mossling.isAlive(target), "two strikes leave one health");
		target = Mossling.strike(target);
		require(target.health == 0 && !Mossling.isAlive(target), "third strike defeats Mossling");
		require(Mossling.step(cells, target, 10.5, 12.5, 0) == target, "defeated Mossling state is stable");

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
		vitals = PlayerVitals.applyAttack(vitals, true);
		require(vitals.health == PlayerVitals.MAX_HEALTH - 1 && vitals.safeTicks == PlayerVitals.ATTACK_SAFE_TICKS,
			"first explicit impact deals one bounded point");
		final healthAfterAttack = vitals.health;
		vitals = PlayerVitals.applyAttack(vitals, true);
		require(vitals.health == healthAfterAttack, "safe ticks reject a repeated impact");
		vitals = PlayerVitals.step(vitals);
		require(vitals.safeTicks == PlayerVitals.ATTACK_SAFE_TICKS - 1, "safe period advances only on the fixed clock");

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
				vitals = PlayerVitals.step(vitals);
			vitals = PlayerVitals.applyAttack(vitals, true);
		}
		require(PlayerVitals.revive(vitals).health == PlayerVitals.MAX_HEALTH, "return prompt restores full health");

		require(Recovery.decide(inventory, vitals) == RecoveryDecision.PlayerDefeated, "fallen player cannot consume recovery items");

		Sys.println("caxecraft-gameplay: lossless mining/items, paced Mossling encounter, berry recovery, and bounded player health passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}

	static inline function absolute(value:Float):Float
		return value < 0.0 ? -value : value;

	static inline function sameMossling(left:caxecraft.gameplay.MosslingState, right:caxecraft.gameplay.MosslingState):Bool
		return left.x == right.x && left.y == right.y && left.z == right.z && left.homeX == right.homeX && left.homeZ == right.homeZ
			&& left.modeCode == right.modeCode && left.phaseTicks == right.phaseTicks && left.health == right.health;
}
