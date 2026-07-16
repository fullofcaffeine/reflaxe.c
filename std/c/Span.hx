package c;

/**
	Mutable borrowed pointer-and-length view.

	These operations are compiler intrinsics. They allocate no wrapper or iterator
	object and select no runtime feature.
**/
@:coreType
extern abstract Span<T> {
	/** Number of addressable elements, represented as C `size_t`. */
	public var length(get, never):Size;

	/** Checked element access. */
	@:arrayAccess public function get(index:Int):T;

	/** Checked element mutation. */
	@:arrayAccess public function set(index:Int, value:T):T;

	/** Typed source iterator protocol; lowered directly to an indexed C loop. */
	public function iterator():Iterator<T>;
}
