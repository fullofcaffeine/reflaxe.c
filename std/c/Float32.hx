package c;

/**
	Exact C `float` carrier for admitted binary32 targets.

	`Float32` is deliberately distinct from Haxe `Float`, which remains a
	binary64 C `double` on this target. Use `fromFloat` when a value crosses into
	a C API and `toFloat` when it returns to ordinary Haxe arithmetic. Neither
	operation allocates or selects `hxrt`.

	The narrowing conversion rounds to binary32 using round-to-nearest,
	ties-to-even under the target's required default floating environment.
	NaN stays NaN, infinities and signed zero are preserved, representable
	subnormals remain subnormal, and finite overflow becomes signed infinity.
	NaN payload bits are not part of the portable contract.
**/
@:coreType
extern abstract Float32 {
	/** Explicitly round a binary64 Haxe `Float` to the C binary32 carrier. */
	public static function fromFloat(value:Float):Float32;

	/** Exactly widen a finite binary32 value for ordinary Haxe arithmetic. */
	public function toFloat():Float;
}
