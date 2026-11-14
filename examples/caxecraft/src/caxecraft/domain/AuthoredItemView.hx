package caxecraft.domain;

/**
	The read-only authored-item carrier selected at the target adapter boundary.

	Native C borrows the session's fixed integer array without copying it. The
	portable oracle exposes indexed reads through an abstract that cannot mutate
	or recover the owned `Array<Int>`.
**/
#if c
typedef AuthoredItemView = c.ConstSpan<Int>;
#else
abstract AuthoredItemView(Array<Int>) {
	/** Borrow one portable item array without exposing mutation. */
	public static inline function borrow(items:Array<Int>):AuthoredItemView
		return new AuthoredItemView(items);

	private inline function new(items:Array<Int>)
		this = items;

	/** Read one validated item slot. */
	@:arrayAccess
	public inline function get(index:Int):Int
		return this[index];
}
#end
