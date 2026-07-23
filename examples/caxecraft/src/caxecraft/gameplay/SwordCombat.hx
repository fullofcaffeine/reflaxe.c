package caxecraft.gameplay;

import caxecraft.domain.VitalsState;
import caxecraft.domain.Vitals.isDefeated;

/**
 * Target-neutral timing and hit decisions for the Copper Sword.
 *
 * A button press is queued by the device adapter, then consumed on the 50 ms
 * game clock. Four cooldown ticks therefore mean the same 200 ms wait on a
 * 60 Hz, 120 Hz, or temporarily slow display.
 */
final class SwordCombat {
	public static inline final COOLDOWN_TICKS:Int = 4;

	public static inline function start():SwordCombatState
		return make(0);

	/** Advance one game tick, reducing a live cooldown by exactly one. */
	public static function step(state:SwordCombatState):SwordCombatState {
		if (state.cooldownTicks <= 0)
			return state;
		return make(state.cooldownTicks - 1);
	}

	/** Classify one queued action without changing inventory, health, or target. */
	public static function decide(state:SwordCombatState, inventory:InventoryState, vitals:VitalsState, target:MosslingState, playerX:Float, playerZ:Float,
			lookX:Float, lookZ:Float):SwordCombatDecision {
		if (!Inventory.selectedIs(inventory, ItemKind.CopperSword))
			return NotSword;
		if (isDefeated(vitals))
			return PlayerDefeated;
		if (inventory.sword <= 0)
			return SwordMissing;
		if (state.cooldownTicks > 0)
			return CoolingDown;
		if (!Mossling.canStrike(target, playerX, playerZ, lookX, lookZ))
			return TargetMissed;
		return Hit;
	}

	/** Start the cooldown only after a confirmed hit. */
	public static function after(decision:SwordCombatDecision, state:SwordCombatState):SwordCombatState {
		if (decision == Hit)
			return make(COOLDOWN_TICKS);
		return state;
	}

	static inline function make(cooldownTicks:Int):SwordCombatState
		return {cooldownTicks: cooldownTicks};
}
