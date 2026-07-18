package c;

/**
	Inline fixed-size C array.

	The compiler admits an `Array<T>` conversion only when its source is an array
	literal and records the literal element count in HxcIR. `N` remains the typed
	length identity; no Haxe `Array` object or target runtime survives lowering.
**/
@:coreType
extern abstract CArray<T, N> from Array<T> {
	/**
		Create compile-time-sized, zero-initialized automatic storage.

		`length` must be a positive compiler-known integer expression. The C target
		validates its storage budget before emitting a real fixed C array; no Haxe
		`Array`, allocation, or runtime feature survives lowering.
	**/
	public static function zero<T, N>(length:Int):CArray<T, N>;

	/** Checked element access. */
	@:arrayAccess public function get(index:Int):T;

	/** Checked element mutation. */
	@:arrayAccess public function set(index:Int, value:T):T;

	/** Borrow this array as a mutable pointer-and-length view. */
	public function span():Span<T>;

	/** Borrow this array as a read-only pointer-and-length view. */
	public function constSpan():ConstSpan<T>;
}
