package caxecraft.gameplay;

/** Why a bounded inventory transfer was rejected, for clear player feedback. */
enum abstract InventoryFullReason(Int) {
	/** No capacity message is active. */
	var None = 0;

	/** A berry gift or pickup could not fit in the berry stack. */
	var BerryStack = 1;

	/** A mined block could not fit in its matching material stack. */
	var BlockStack = 2;
}
