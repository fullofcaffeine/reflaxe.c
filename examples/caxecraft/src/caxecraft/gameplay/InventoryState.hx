package caxecraft.gameplay;

/**
 * Small immutable inventory used by both the game and renderer-free tests.
 *
 * The first version deliberately has eight fixed slots. Explicit fields keep
 * its capacity, generated C layout, and update cost visible without requiring
 * a dynamic collection or hidden allocation in the game loop.
 */
typedef InventoryState = {
	final selected:Int;
	final grass:Int;
	final dirt:Int;
	final stone:Int;
	final haxeforge:Int;
	final sword:Int;
	final berries:Int;
	final bread:Int;
	final lantern:Int;
}
