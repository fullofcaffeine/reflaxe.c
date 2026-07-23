/** Exercises constants copied from declarations into compiler-known calls. */
class Main {
	static function defaultValue(value:Int = 7):Int {
		return value;
	}

	static function packed(first:Int, second:Int = 2, third:Int = 3):Int {
		return first * 100 + second * 10 + third;
	}

	static function countdown(value:Int, step:Int = 1):Int {
		return value <= 0 ? 0 : 1 + countdown(value - step);
	}

	static function main():Void {
		var cursor = 0;
		while (defaultValue() != 7 || defaultValue(9) != 9 || defaultValue(defaultValue()) != 7 || packed(++cursor) != 123 || cursor != 1
			|| packed(4, 5) != 453 || packed(4, 5, 6) != 456 || countdown(3) != 3 || DefaultSupport.crossModule(2) != 26
			|| DefaultSupport.genericIdentity(8) != 8) {}
	}
}
