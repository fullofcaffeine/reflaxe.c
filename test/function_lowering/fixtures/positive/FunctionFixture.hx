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
		return;
	}
}
