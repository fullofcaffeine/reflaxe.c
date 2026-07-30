/**
	Provides a small ordinary-Haxe program for function-lowering tests.

	The module keeps all results observable without target-specific source so Haxe
	Eval and generated C can execute the same program.
**/

/**
	Exercises ordinary Haxe function calls through the semantic C pipeline.

	The class groups the fixture entry point with its private call shapes. Haxe
	Eval is the behavior oracle, while generated strict C proves that immutable
	incoming parameters, mutable parameter storage, recursion, and stack closures
	share one source-language contract.
**/
class FunctionFixture {
	static function passthrough(value:Int):Int {
		return value;
	}

	static function asFloat(value:Float):Float {
		return value;
	}

	static function chain(value:Int):Int {
		return passthrough(value);
	}

	static function convert(value:Int):Float {
		return asFloat(value);
	}

	static function discarded(value:Int):Void {
		passthrough(value);
		return;
	}

	static function first(left:Int, right:Int):Int {
		right;
		return left;
	}

	static function ordered(value:Int):Int {
		return first(passthrough(value), chain(value));
	}

	static function apply(value:Int, operation:Int->Int):Int {
		return operation(value);
	}

	/**
		Invoke one callback twice while its caller-owned capture environment lives.

		This is the positive lifetime boundary for stack closures: the callback is
		used only during this direct call and is never stored, returned, or forwarded
		somewhere that could outlive its captured variables.
	**/
	static function applyTwice(value:Int, operation:Int->Int):Int {
		final first = operation(value);
		return operation(first);
	}

	/**
		Exercise shared mutation through a captured environment.

		`calls` must be observed by reference across both callback invocations.
		Changing `seed` inside the callback must also change what the outer function
		reads afterward, proving that a captured parameter is moved into one shared,
		addressable caller slot rather than copied into the closure.
	**/
	static function captureRoundTrip(seed:Int):Int {
		var calls = 0;
		final result = applyTwice(1, function(value:Int):Int {
			var output = 0;
			if (calls == 0) {
				calls = 1;
				output = seed;
				seed = 6;
			} else {
				calls = 2;
				if (value == 5 && seed == 6)
					output = 15;
			}
			return output;
		});
		if (calls != 2 || seed != 6)
			return 0;
		return result;
	}

	static function choose():Int->Int {
		return chain;
	}

	static function indirect(value:Int):Int {
		var operation:Int->Int = choose();
		return apply(operation(value), passthrough);
	}

	/**
		Exercise direct `Int` and `Bool` parameter reassignment.

		`seed` is read before its first write, so its mutable HxcIR local must be
		initialized at function entry rather than created lazily at the assignment.
		The assignment expression also proves that storing a new value still
		returns that same value to the enclosing expression.
	**/
	static function mutateParameters(seed:Int, remaining:Int, flag:Bool):Int {
		final original = seed;
		seed = remaining;
		final assigned = (seed = original);
		while (remaining > 0) {
			seed = remaining;
			remaining = 0;
		}
		flag = !flag;
		if (flag)
			seed = original;
		if (assigned != original)
			return 0;
		return seed;
	}

	/**
		Exercise compound reassignment without adding signed-integer helpers here.

		The arithmetic suite owns UB-safe signed integer helper coverage. Float
		addition is direct C arithmetic, so this function keeps the present fixture
		focused on parameter storage and its independent-header contract.
	**/
	static function mutateFloat(value:Float):Float {
		value += 1.5;
		return value;
	}

	/**
		Keep neighboring read-only parameters on the immutable value path.

		This function has no local carrier. Its generated C and HxcIR therefore
		guard against turning every parameter into mutable storage just because the
		compiler now supports reassignment elsewhere.
	**/
	static function readOnlyParameters(left:Int, right:Int, enabled:Bool):Int {
		return enabled ? left : right;
	}

	static function recursive(left:Int, right:Int):Void {
		recursive(right, left);
	}

	static function mutualLeft(value:Int):Void {
		mutualRight(value);
	}

	static function mutualRight(value:Int):Void {
		mutualLeft(value);
	}

	static function main():Void {
		var integer:Int = chain(7);
		integer;
		var floating:Float = convert(9);
		floating;
		discarded(11);
		var orderedValue:Int = ordered(13);
		orderedValue;
		var indirectValue:Int = indirect(17);
		// Native execution must prove the function pointer was called with the
		// right argument and result, not merely that the generated C compiled.
		while (indirectValue != 17) {}
		final captured = captureRoundTrip(5);
		// The first result (5) becomes the second call's argument; its result is 15.
		while (captured != 15) {}
		final mutated = mutateParameters(3, 2, false);
		while (mutated != 3) {}
		final mutatedFloat = mutateFloat(3.0);
		while (mutatedFloat != 4.5) {}
		final readOnly = readOnlyParameters(9, 4, true);
		while (readOnly != 9) {}
		return;
	}
}
