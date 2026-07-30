package c;

/** Exact-width unsigned C integer contract with direct mask and equality operations. */
@:coreType
extern abstract UInt16 {
	/** Preserve all bits while intersecting two exact masks. */
	@:op(A & B) public function bitAnd(other:UInt16):UInt16;

	/** Compare two exact unsigned 16-bit values. */
	@:op(A == B) public function equals(other:UInt16):Bool;

	/** Compare two exact unsigned 16-bit values. */
	@:op(A != B) public function notEquals(other:UInt16):Bool;
}
