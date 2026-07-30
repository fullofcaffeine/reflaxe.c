package c;

/** Exact-width unsigned C integer contract with direct mask and equality operations. */
@:coreType
extern abstract UInt32 {
	/** Preserve all bits while intersecting two exact masks. */
	@:op(A & B) public function bitAnd(other:UInt32):UInt32;

	/** Compare two exact unsigned 32-bit values. */
	@:op(A == B) public function equals(other:UInt32):Bool;

	/** Compare two exact unsigned 32-bit values. */
	@:op(A != B) public function notEquals(other:UInt32):Bool;
}
