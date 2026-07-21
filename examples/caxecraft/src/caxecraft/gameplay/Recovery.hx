package caxecraft.gameplay;

/**
 * Target-neutral rules for the first consumable recovery item.
 *
 * The decision is separate from presentation and device input. A renderer can
 * therefore show useful feedback while every target applies the same bounded
 * inventory and health transition.
 */
final class Recovery {
	/** One berry use restores one complete heart on the six-point health bar. */
	public static inline final BERRY_HEALTH:Int = 2;

	public static function decide(inventory:InventoryState, vitals:PlayerVitalsState):RecoveryDecision {
		if (!Inventory.selectedIs(inventory, ItemKind.Berries))
			return NotRecoveryItem;
		if (PlayerVitals.isDefeated(vitals))
			return PlayerDefeated;
		if (inventory.berries <= 0)
			return RecoveryStackEmpty;
		if (PlayerVitals.isFull(vitals))
			return HealthAlreadyFull;
		return UseBerries;
	}

	/** Apply only the inventory half of an already-made decision. */
	public static function applyInventory(decision:RecoveryDecision, inventory:InventoryState):InventoryState {
		if (decision == UseBerries)
			return Inventory.consumeSelected(inventory);
		return inventory;
	}

	/** Apply only the health half of an already-made decision. */
	public static function applyVitals(decision:RecoveryDecision, vitals:PlayerVitalsState):PlayerVitalsState {
		if (decision == UseBerries)
			return PlayerVitals.recover(vitals, BERRY_HEALTH);
		return vitals;
	}
}
