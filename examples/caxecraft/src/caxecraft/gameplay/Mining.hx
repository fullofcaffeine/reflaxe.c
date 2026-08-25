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
	final item = itemForCollectableBlock(kind);
	return item == null ? result(inventory, BlockUnavailable) : collect(cells, coordinate, inventory, kind, item);
}

/** Return the inventory item produced by a currently mineable terrain kind. */
function itemForCollectableBlock(kind:BlockKind):Null<ItemKind>
	return switch kind {
		case Grass: ItemKind.GrassBlock;
		case Dirt: ItemKind.DirtBlock;
		case Stone: ItemKind.StoneBlock;
		case Sand: ItemKind.SandBlock;
		case Air | Bedrock | Wood | Leaves | Snow | Ash: null;
	};

private function collect(cells:WorldCells, coordinate:BlockCoord, inventory:InventoryState, kind:BlockKind, item:ItemKind):MiningResult {
	if (Inventory.acceptedAmount(inventory, item, 1) != 1)
		return result(inventory, InventoryFull);
	if (!World.remove(cells, coordinate))
		return result(inventory, BlockUnavailable);
	return result(Inventory.collectBlock(inventory, kind), Collected);
}

private inline function result(inventory:InventoryState, outcome:MiningOutcome):MiningResult
	return {inventory: inventory, outcome: outcome};
