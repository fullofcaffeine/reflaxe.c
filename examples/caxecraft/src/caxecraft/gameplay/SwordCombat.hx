package caxecraft.gameplay;

import caxecraft.domain.Character;
import caxecraft.domain.VitalsState;
import caxecraft.domain.Vitals.isDefeated;

/**
 * Owns target-neutral timing and hit decisions for the Copper Sword.
 *
 * A button press is queued by the device adapter, then consumed on the 50 ms
 * game clock. Four cooldown ticks therefore mean the same 200 ms wait on a
 * 60 Hz, 120 Hz, or temporarily slow display. These operations are module
 * functions because all changing state is explicit in `SwordCombatState`;
 * there is no long-lived object identity for a wrapper class to own.
 */
inline final COOLDOWN_TICKS:Int = 4;

/** Create the ready state used when a player or actor equips the sword. */
inline function start():SwordCombatState
	return make(0);

/** Advance one game tick, reducing a live cooldown by exactly one. */
function step(state:SwordCombatState):SwordCombatState {
	if (state.cooldownTicks <= 0)
		return state;
	return make(state.cooldownTicks - 1);
}

/**
	Classify one queued action against any committed character snapshot.

	The weapon knows reach and aim, not whether the target came from a player,
	NPC, or enemy placement. `GameSession.damageCharacter` owns the later
	stable-ID commit, so this pure decision cannot mutate actor health.
**/
function decide(state:SwordCombatState, inventory:InventoryState, vitals:VitalsState, target:Character, playerX:Float, playerZ:Float, lookX:Float,
		lookZ:Float):SwordCombatDecision {
	if (!Inventory.selectedIs(inventory, ItemKind.CopperSword))
		return NotSword;
	if (isDefeated(vitals))
		return PlayerDefeated;
	if (inventory.sword <= 0)
		return SwordMissing;
	if (state.cooldownTicks > 0)
		return CoolingDown;
	if (!canStrike(target, playerX, playerZ, lookX, lookZ))
		return TargetMissed;
	return Hit;
}

/** Start the cooldown only after a confirmed hit. */
function after(decision:SwordCombatDecision, state:SwordCombatState):SwordCombatState {
	if (decision == Hit)
		return make(COOLDOWN_TICKS);
	return state;
}

private inline function make(cooldownTicks:Int):SwordCombatState
	return {cooldownTicks: cooldownTicks};

/** Whether one living target is near enough and in front of the player. */
private function canStrike(target:Character, playerX:Float, playerZ:Float, lookX:Float, lookZ:Float):Bool {
	if (!target.id.isValid() || isDefeated(target.vitals))
		return false;
	final dx = target.body.x - playerX;
	final dz = target.body.z - playerZ;
	if (dx * dx + dz * dz > 9.0)
		return false;
	return dx * lookX + dz * lookZ >= 0.35;
}
