package c;

/**
	Signed integer capable of round-tripping a C object pointer.

	Comparisons retain this target-resolved ABI carrier and never reinterpret it
	as an ordinary Haxe `Int`.
**/
@:coreType
extern abstract IntPtr {
	/** Compare two pointer-width signed values. */
	@:op(A == B) public function equals(other:IntPtr):Bool;

	/** Compare two pointer-width signed values. */
	@:op(A != B) public function notEquals(other:IntPtr):Bool;
}
