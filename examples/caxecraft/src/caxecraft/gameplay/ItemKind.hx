package caxecraft.gameplay;

/**
 * The first eight things a player can hold in the Caxecraft hotbar.
 *
 * This is an ordinary closed Haxe enum. Adding an item makes every exhaustive
 * `switch` point out the rules and presentation that also need a decision.
 */
enum ItemKind {
	GrassBlock;
	DirtBlock;
	StoneBlock;
	Haxeforge;
	CopperSword;
	Berries;
	Bread;
	Lantern;
}
