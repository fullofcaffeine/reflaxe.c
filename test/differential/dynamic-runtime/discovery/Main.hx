/**
	Keep static-call discovery transparent to ordinary Haxe evaluation.

	Generic calls have specialized targets, while `Std.int` is an intrinsic with
	no ordinary prepared function. Its argument must still be visited: the block
	creates and reads a Dynamic value before the intrinsic converts the result.
**/
class Main {
	/** Keep one ordinary static target distinct from the generic target. */
	static function increment(value:Int):Int {
		return value + 1;
	}

	/** Require separate Int and Bool specializations of the same source function. */
	static function identity<T>(value:T):T {
		return value;
	}

	/** Observe direct, wrapped, specialized, and intrinsic calls in source order. */
	static function main():Void {
		final ordinary = increment(2);
		final parenthesized = (increment)(3);
		final metadata = (@:keep increment)(4);
		final castCallee = (cast increment : Int->Int)(5);
		final integer = identity(7);
		final boolean = identity(true);
		final nested = Std.int({
			final boxed:Dynamic = 9.75;
			final restored:Float = boxed;
			restored;
		});
		final boxedArgument:Dynamic = 10;
		final restoredArgument = increment(boxedArgument);
		Sys.println(ordinary + ":" + parenthesized + ":" + metadata + ":" + castCallee + ":" + integer + ":" + boolean + ":" + nested + ":" + restoredArgument);
	}
}
