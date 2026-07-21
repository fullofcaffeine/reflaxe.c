package caxecraft.qa;

import caxecraft.domain.BlockKind;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.ItemKind;

/** Renderer-independent acceptance proof for the first bounded hotbar. */
final class InventoryProbe {
	static function main():Void {
		var state = Inventory.starter();
		require(state.selected == 0 && Inventory.countAt(state, 0) == 16, "starter grass slot");
		require(Inventory.itemAt(3) == ItemKind.Haxeforge && Inventory.itemAt(7) == ItemKind.Lantern, "closed item order");
		require(Inventory.selectedBlock(state) == BlockKind.Grass, "selected block mapping");

		state = Inventory.cycle(state, -1);
		require(state.selected == 7, "negative cycle wraps");
		state = Inventory.cycle(state, 2);
		require(state.selected == 1, "positive cycle wraps");
		state = Inventory.select(state, 99);
		require(state.selected == 1, "invalid selection is inert");

		final dirtBefore = state.dirt;
		state = Inventory.consumeSelected(state);
		require(state.dirt == dirtBefore - 1, "selected stack consumes exactly one");
		state = Inventory.collectBlock(state, BlockKind.Dirt);
		require(state.dirt == dirtBefore, "mined block returns to its stack");

		var empty = Inventory.make(2, -4, 200, 0, 1, 1, 1, 1, 1);
		require(empty.grass == 0 && empty.dirt == Inventory.MAX_STACK, "counts clamp to fixed bounds");
		empty = Inventory.consumeSelected(empty);
		require(empty.stone == 0, "empty consume is inert");
		empty = Inventory.collectBlock(empty, BlockKind.Stone);
		require(empty.stone == 1, "collect fills empty stack");
		empty = Inventory.collectBlock(Inventory.make(2, 0, 0, Inventory.MAX_STACK, 1, 1, 1, 1, 1), BlockKind.Stone);
		require(empty.stone == Inventory.MAX_STACK, "full stack stays bounded");
		require(Inventory.collectBlock(empty, BlockKind.Bedrock).stone == Inventory.MAX_STACK, "bedrock is not an item drop");

		var partial = Inventory.make(5, 0, 0, 0, 0, 0, Inventory.MAX_STACK - 1, 0, 0);
		require(Inventory.acceptedAmount(partial, ItemKind.Berries, 2) == 1, "partial stack reports exact remaining capacity");
		partial = Inventory.collectItem(partial, ItemKind.Berries, 2);
		require(partial.berries == Inventory.MAX_STACK, "collection accepts only the amount that fits");
		require(Inventory.acceptedAmount(partial, ItemKind.Berries, 2) == 0
			&& Inventory.collectItem(partial, ItemKind.Berries, 2) == partial,
			"full item stack rejects collection without mutation");

		Sys.println("caxecraft-inventory: 8 typed slots; selection, wrap, consume, lossless collect, empty, and full bounds passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
