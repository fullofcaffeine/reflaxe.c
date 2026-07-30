package c;

/** Exact-width unsigned C integer contract with representation-preserving equality. */
@:coreType
extern abstract UInt64 {
	/** Compare two exact unsigned 64-bit values. */
	@:op(A == B) public function equals(other:UInt64):Bool;

	/** Compare two exact unsigned 64-bit values. */
	@:op(A != B) public function notEquals(other:UInt64):Bool;
}
