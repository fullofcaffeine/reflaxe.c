package caxecraft.gameplay;

/** Why one queued Copper Sword action did or did not become a hit. */
enum abstract SwordCombatDecision(Int) {
	var NotSword = 0;
	var PlayerDefeated = 1;
	var SwordMissing = 2;
	var CoolingDown = 3;
	var TargetMissed = 4;
	var Hit = 5;
}
