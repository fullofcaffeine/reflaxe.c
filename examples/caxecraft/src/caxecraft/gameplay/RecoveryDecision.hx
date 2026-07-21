package caxecraft.gameplay;

/** Closed outcomes for one attempt to use the selected recovery item. */
enum abstract RecoveryDecision(Int) {
	/** The selected item has another secondary action, such as block placement. */
	var NotRecoveryItem = 0;

	/** Consume one berry bundle and restore its declared amount of health. */
	var UseBerries = 1;

	/** Recovery is unnecessary, so the item must remain in the inventory. */
	var HealthAlreadyFull = 2;

	/** The berry slot is selected but contains nothing to consume. */
	var RecoveryStackEmpty = 3;

	/** A fallen player must return to the meadow before using an item. */
	var PlayerDefeated = 4;
}
