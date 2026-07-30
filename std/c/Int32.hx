package c;

/** Exact-width signed C integer contract with representation-preserving equality. */
@:coreType
extern abstract Int32 {
	/** Compare two exact signed 32-bit values. */
	@:op(A == B) public function equals(other:Int32):Bool;

	/** Compare two exact signed 32-bit values. */
	@:op(A != B) public function notEquals(other:Int32):Bool;
}
