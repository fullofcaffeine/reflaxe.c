package c;

/**
	Inline fixed-size C array.

	The compiler admits an `Array<T>` conversion only when its source is an array
	literal and records the literal element count in HxcIR. `N` remains the typed
	length identity; no Haxe `Array` object or target runtime survives lowering.
**/
@:coreType
extern abstract CArray<T, N> from Array<T> {
	/** Checked element access. */
	@:arrayAccess public function get(index:Int):T;

	/** Checked element mutation. */
	@:arrayAccess public function set(index:Int, value:T):T;

	/** Borrow this array as a mutable pointer-and-length view. */
	public function span():Span<T>;

	/** Borrow this array as a read-only pointer-and-length view. */
	public function constSpan():ConstSpan<T>;
}
