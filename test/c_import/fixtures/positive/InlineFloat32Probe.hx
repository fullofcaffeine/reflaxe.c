/**
 * Focused regression for Haxe inline substitution at a binary32 boundary.
 *
 * Haxe accepts an `Int` wherever a `Float` parameter is required. When the
 * helper below is inlined, however, the substituted typed expression still
 * carries its concrete `Int` type. haxe.c must restore the declared `Float`
 * boundary before performing the explicit `c.Float32` narrowing. The mutable
 * counter proves a call argument is evaluated exactly once and in source order.
 */
class InlineFloat32Probe {
	static var sideEffectCount:Int = 0;

	/** Narrow only after Haxe's ordinary argument-to-`Float` conversion. */
	static inline function narrow(value:Float):c.Float32
		return c.Float32.fromFloat(value);

	/** Retained non-inline form used as the semantic comparison point. */
	static function narrowWithoutInlining(value:Float):c.Float32
		return c.Float32.fromFloat(value);

	/** Produce one observable `Int` argument; the inline body must not repeat it. */
	static function nextInteger():Int {
		sideEffectCount++;
		return 7;
	}

	/** Exercise every argument shape from the compiler regression report. */
	public static function run():Bool {
		final integerLocal = 3;
		final floatLocal:Float = 6.5;
		final fromIntegerLocal = narrow(integerLocal);
		final fromIntegerArithmetic = narrow(integerLocal + 2);
		final fromFloatLocal = narrow(floatLocal);
		final fromSideEffect = narrow(nextInteger());
		final fromNonInline = narrowWithoutInlining(integerLocal + 5);
		return sideEffectCount == 1
			&& PointLib.verifyInlineFloat32(fromIntegerLocal, fromIntegerArithmetic, fromFloatLocal, fromSideEffect, fromNonInline);
	}
}
