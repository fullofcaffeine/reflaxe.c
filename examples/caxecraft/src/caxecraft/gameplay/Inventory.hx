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
		return {
			selected: selected,
			grass: boundedCount(grass),
			dirt: boundedCount(dirt),
			stone: boundedCount(stone),
			haxeforge: boundedCount(haxeforge),
			sword: boundedCount(sword),
			berries: boundedCount(berries),
			bread: boundedCount(bread),
			lantern: boundedCount(lantern)
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

	/** Ask about the selected semantic item without exposing its slot number. */
	public static function selectedIs(state:InventoryState, item:ItemKind):Bool {
		return switch (item) {
			case GrassBlock: state.selected == 0;
			case DirtBlock: state.selected == 1;
			case StoneBlock: state.selected == 2;
			case Haxeforge: state.selected == 3;
			case CopperSword: state.selected == 4;
			case Berries: state.selected == 5;
			case Bread: state.selected == 6;
			case Lantern: state.selected == 7;
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

	/** Count one semantic item without asking callers to know its slot number. */
	public static function countItem(state:InventoryState, item:ItemKind):Int {
		return switch (item) {
			case GrassBlock: state.grass;
			case DirtBlock: state.dirt;
			case StoneBlock: state.stone;
			case Haxeforge: state.haxeforge;
			case CopperSword: state.sword;
			case Berries: state.berries;
			case Bread: state.bread;
			case Lantern: state.lantern;
		};
	}

	/**
	 * How much of one offered stack fits without discarding an existing item.
	 *
	 * Callers use this before changing a world drop or completing an NPC gift.
	 */
	public static function acceptedAmount(state:InventoryState, item:ItemKind, offered:Int):Int {
		if (offered <= 0)
			return 0;
		final space = MAX_STACK - countItem(state, item);
		if (space <= 0)
			return 0;
		return offered < space ? offered : space;
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
			case Air | Bedrock | Sand | Wood | Leaves | Snow | Ash: state;
		};
	}

	/** Add a bounded amount of one declared item, as used by NPC gifts/drops. */
	public static function collectItem(state:InventoryState, kind:ItemKind, amount:Int):InventoryState {
		final accepted = acceptedAmount(state, kind, amount);
		if (accepted <= 0)
			return state;
		return switch (kind) {
			case GrassBlock: make(state.selected, state.grass + accepted, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern);
			case DirtBlock: make(state.selected, state.grass, state.dirt + accepted, state.stone, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern);
			case StoneBlock: make(state.selected, state.grass, state.dirt, state.stone + accepted, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern);
			case Haxeforge: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge + accepted, state.sword, state.berries, state.bread,
					state.lantern);
			case CopperSword: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword
					+ accepted, state.berries, state.bread,
					state.lantern);
			case Berries: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries + accepted, state.bread,
					state.lantern);
			case Bread: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread + accepted,
					state.lantern);
			case Lantern: make(state.selected, state.grass, state.dirt, state.stone, state.haxeforge, state.sword, state.berries, state.bread,
					state.lantern + accepted);
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
