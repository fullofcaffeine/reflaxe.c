package caxecraft.domain;

/**
	A read-only, short-lived view of the loaded world's voxel bytes.

	Gameplay and rendering can inspect this value during one direct call, but
	cannot use it to change terrain. On C, haxe.c lowers the view to a `const`
	pointer plus a checked length; no copy, wrapper object, heap allocation, or
	runtime helper is created. The pointer borrows storage owned by `GameSession`,
	so the compiler rejects saving the returned view for later.

	Eval uses the session's ordinary Haxe array as the independent behavior
	oracle. Haxe selects one branch at compile time; shared world-reading code
	uses `WorldRead` and does not know which carrier was selected.
**/
#if c
typedef WorldView = c.ConstSpan<c.UInt8>;
#else
abstract WorldView(Array<Int>) {
	/**
		Borrow an Eval array through the same read-only API used by native C.

		This does not copy the array. The abstract deliberately defines indexed
		reads but no indexed writes and no conversion back to `Array<Int>`, so
		Eval code cannot accidentally bypass the session's editing commands.
	**/
	public static inline function borrow(cells:Array<Int>):WorldView
		return new WorldView(cells);

	private inline function new(cells:Array<Int>)
		this = cells;

	/** Read one cell while keeping the owned array hidden from the caller. */
	@:arrayAccess
	public inline function get(index:Int):Int
		return this[index];
}
#end
