package caxecraft.gameplay;

/** Every possible result of one Adventure-mode attempt to mine a block. */
enum abstract MiningOutcome(Int) {
	/** The pointed-at cell is empty, immutable, or outside the world. */
	var BlockUnavailable = 0;

	/** The matching inventory stack is full, so the block stays in the world. */
	var InventoryFull = 1;

	/** Exactly one block left the world and entered its matching stack. */
	var Collected = 2;
}
