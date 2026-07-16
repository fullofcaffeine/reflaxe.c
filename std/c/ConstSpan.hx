package c;

/**
	Read-only borrowed pointer-and-length view.

	These operations are compiler intrinsics. They allocate no wrapper or iterator
	object and select no runtime feature.
**/
@:coreType
extern abstract ConstSpan<T> {
	/** Number of addressable elements, represented as C `size_t`. */
	public var length(get, never):Size;

	/** Checked element access. */
	@:arrayAccess public function get(index:Int):T;

	/** Typed source iterator protocol; lowered directly to an indexed C loop. */
	public function iterator():Iterator<T>;
}
