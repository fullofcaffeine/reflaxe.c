package caxecraft.gameplay;

/** The complete, immutable result of one Adventure-mode mining attempt. */
typedef MiningResult = {
	final inventory:InventoryState;
	final outcome:MiningOutcome;
}
