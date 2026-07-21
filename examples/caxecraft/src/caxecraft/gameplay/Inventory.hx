package caxecraft.gameplay;

import caxecraft.domain.BlockKind;

/** Deterministic operations for Caxecraft's bounded eight-slot inventory. */
final class Inventory {
	public static inline final SLOT_COUNT:Int = 8;
	public static inline final MAX_STACK:Int = 64;

	/** A useful finite starting set for the first Adventure gameplay slice. */
	public static function starter():InventoryState
		return make(0, 16, 24, 12, 1, 1, 6, 3, 1);

	public static function make(selected:Int, grass:Int, dirt:Int, stone:Int, haxeforge:Int, sword:Int, berries:Int, bread:Int, lantern:Int):InventoryState {
		// Materialize each checked scalar before constructing the record. This
		// keeps source evaluation order explicit and avoids carrying a branch-local
		// temporary into a later aggregate field. haxe_c-940 owns lifting that
		// current compiler constraint.
		final safeGrass = boundedCount(grass);
		final safeDirt = boundedCount(dirt);
		final safeStone = boundedCount(stone);
		final safeHaxeforge = boundedCount(haxeforge);
		final safeSword = boundedCount(sword);
		final safeBerries = boundedCount(berries);
		final safeBread = boundedCount(bread);
		final safeLantern = boundedCount(lantern);
		return {
			selected: selected,
			grass: safeGrass,
			dirt: safeDirt,
			stone: safeStone,
			haxeforge: safeHaxeforge,
			sword: safeSword,
			berries: safeBerries,
			bread: safeBread,
			lantern: safeLantern
		};
	}

	/** Select an exact slot. Invalid device/script values leave state unchanged. */
	public static function select(state:InventoryState, slot:Int):InventoryState {
		if (slot < 0 || slot >= SLOT_COUNT || slot == state.selected)
			return state;
		return make(slot, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
	}

	/** Move one or more slots and wrap at either end. */
	public static function cycle(state:InventoryState, delta:Int):InventoryState {
		if (delta == 0)
			return state;
		var next = state.selected + delta;
		while (next < 0)
			next += SLOT_COUNT;
		while (next >= SLOT_COUNT)
			next -= SLOT_COUNT;
		return select(state, next);
	}

	public static function itemAt(slot:Int):ItemKind {
		return switch (slot) {
			case 0: GrassBlock;
			case 1: DirtBlock;
			case 2: StoneBlock;
			case 3: Haxeforge;
			case 4: CopperSword;
			case 5: Berries;
			case 6: Bread;
			case _: Lantern;
		};
	}

	public static function countAt(state:InventoryState, slot:Int):Int {
		return switch (slot) {
			case 0: state.grass;
			case 1: state.dirt;
			case 2: state.stone;
			case 3: state.haxeforge;
			case 4: state.sword;
			case 5: state.berries;
			case 6: state.bread;
			case 7: state.lantern;
			case _: 0;
		};
	}

	/** The selected material, or Air when the selected item cannot be placed. */
	public static function selectedBlock(state:InventoryState):BlockKind {
		return switch (state.selected) {
			case 0: Grass;
			case 1: Dirt;
			case 2: Stone;
			case _: Air;
		};
	}

	/** Remove one selected item. Empty and invalid slots fail without mutation. */
	public static function consumeSelected(state:InventoryState):InventoryState {
		if (countAt(state, state.selected) <= 0)
			return state;
		return switch (state.selected) {
			case 0: make(state.selected, state.grass - 1, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
			case 1: make(state.selected, state.grass, state.dirt - 1, state.stone, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
			case 2: make(state.selected, state.grass, state.dirt, state.stone - 1, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
			case 3: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge
					- 1, state.sword, state.berries, state.bread, state.lantern);
			case 4: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword
					- 1, state.berries, state.bread, state.lantern);
			case 5: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries
					- 1, state.bread, state.lantern);
			case 6: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread
					- 1, state.lantern);
			case 7: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread, state.lantern
					- 1);
			case _: state;
		};
	}

	/** Add one mined block to its declared stack; full/non-item blocks are ignored. */
	public static function collectBlock(state:InventoryState, kind:BlockKind):InventoryState {
		return switch (kind) {
			case Grass:
				make(state.selected, state.grass + 1, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
			case Dirt:
				make(state.selected, state.grass, state.dirt + 1, state.stone, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
			case Stone:
				make(state.selected, state.grass, state.dirt, state.stone + 1, state.haxeforge, state.sword, state.berries, state.bread, state.lantern);
			case Air | Bedrock: state;
		};
	}

	/** Add a bounded amount of one declared item, as used by NPC gifts/drops. */
	public static function collectItem(state:InventoryState, kind:ItemKind, amount:Int):InventoryState {
		if (amount <= 0)
			return state;
		return switch (kind) {
			case GrassBlock: make(state.selected, state.grass + amount, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern);
			case DirtBlock: make(state.selected, state.grass, state.dirt + amount, state.stone, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern);
			case StoneBlock: make(state.selected, state.grass, state.dirt, state.stone + amount, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern);
			case Haxeforge: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge + amount, state.sword, state.berries, state.bread,
					state.lantern);
			case CopperSword: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword + amount, state.berries, state.bread,
					state.lantern);
			case Berries: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries + amount, state.bread,
					state.lantern);
			case Bread: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread + amount,
					state.lantern);
			case Lantern: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern + amount);
		};
	}

	static inline function boundedCount(value:Int):Int {
		if (value < 0)
			return 0;
		if (value > MAX_STACK)
			return MAX_STACK;
		return value;
	}
}
