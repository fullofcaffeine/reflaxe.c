package caxecraft.gameplay;

import caxecraft.domain.BlockCoord;
import caxecraft.domain.BlockKind;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;

/**
 * Try to collect the pointed-at block as one indivisible game operation.
 *
 * Capacity is checked before the world changes. This order matters: removing
 * first and discovering a full stack second would silently destroy the block.
 * Creative mode intentionally bypasses this rule and calls `World.remove`
 * directly because its building inventory is unlimited.
 */
function attempt(cells:WorldCells, coordinate:BlockCoord, inventory:InventoryState):MiningResult {
	final kind = World.query(cells, coordinate);
	return switch (kind) {
		case Grass: collect(cells, coordinate, inventory, kind, ItemKind.GrassBlock);
		case Dirt: collect(cells, coordinate, inventory, kind, ItemKind.DirtBlock);
		case Stone: collect(cells, coordinate, inventory, kind, ItemKind.StoneBlock);
		case Air | Bedrock: result(inventory, BlockUnavailable);
	};
}

private function collect(cells:WorldCells, coordinate:BlockCoord, inventory:InventoryState, kind:BlockKind, item:ItemKind):MiningResult {
	if (Inventory.acceptedAmount(inventory, item, 1) != 1)
		return result(inventory, InventoryFull);
	if (!World.remove(cells, coordinate))
		return result(inventory, BlockUnavailable);
	return result(Inventory.collectBlock(inventory, kind), Collected);
}

private inline function result(inventory:InventoryState, outcome:MiningOutcome):MiningResult
	return {inventory: inventory, outcome: outcome};
