package caxecraft.gameplay;

import caxecraft.domain.VitalsState;
import caxecraft.domain.Vitals.isDefeated;
import caxecraft.domain.Vitals.isFull;
import caxecraft.domain.Vitals.recover;

/**
 * Target-neutral rules for the first consumable recovery item.
 *
 * The decision is separate from presentation and device input. A renderer can
 * therefore show useful feedback while every target applies the same bounded
 * inventory and health transition.
 */
/** One berry use restores one complete heart on the six-point health bar. */
inline final BERRY_HEALTH:Int = 2;

function decide(inventory:InventoryState, vitals:VitalsState):RecoveryDecision {
	if (!Inventory.selectedIs(inventory, ItemKind.Berries))
		return NotRecoveryItem;
	if (isDefeated(vitals))
		return PlayerDefeated;
	if (inventory.berries <= 0)
		return RecoveryStackEmpty;
	if (isFull(vitals))
		return HealthAlreadyFull;
	return UseBerries;
}

/** Apply only the inventory half of an already-made decision. */
function applyInventory(decision:RecoveryDecision, inventory:InventoryState):InventoryState {
	if (decision == UseBerries)
		return Inventory.consumeSelected(inventory);
	return inventory;
}

/** Apply only the health half of an already-made decision. */
function applyVitals(decision:RecoveryDecision, vitals:VitalsState):VitalsState {
	if (decision == UseBerries)
		return recover(vitals, BERRY_HEALTH);
	return vitals;
}
