package c;

/**
	Exact-width signed C integer contract.

	Comparisons keep this exact carrier and produce Haxe `Bool`; they do not opt
	the value into Haxe numeric promotion or arithmetic.
**/
@:coreType
extern abstract Int64 {
	/** Compare two exact signed 64-bit values. */
	@:op(A == B) public function equals(other:Int64):Bool;

	/** Compare two exact signed 64-bit values. */
	@:op(A != B) public function notEquals(other:Int64):Bool;

	/** Order two exact signed 64-bit values. */
	@:op(A < B) public function lessThan(other:Int64):Bool;

	/** Order two exact signed 64-bit values. */
	@:op(A <= B) public function lessThanOrEqual(other:Int64):Bool;

	/** Order two exact signed 64-bit values. */
	@:op(A > B) public function greaterThan(other:Int64):Bool;

	/** Order two exact signed 64-bit values. */
	@:op(A >= B) public function greaterThanOrEqual(other:Int64):Bool;
}
