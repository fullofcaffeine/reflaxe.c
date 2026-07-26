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
		return;
	}
}
