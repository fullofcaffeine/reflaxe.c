/**
	Prove that a fresh nullable Array stays owned through a direct null check.

	Each helper compares the result of an ordinary Haxe call without first storing
	it in a source local. A silent exit means both equality operators preserve the
	present and absent cases.
**/
final class Main {
	/** Return either no Array or one newly owned Array. */
	static function maybeValues(present:Bool):Null<Array<Int>> {
		if (!present)
			return null;
		return [3, 4];
	}

	/** Report whether a freshly returned nullable Array is present. */
	static function freshArrayIsPresent(present:Bool):Bool
		return maybeValues(present) != null;

	/** Report whether a freshly returned nullable Array is absent. */
	static function freshArrayIsAbsent(present:Bool):Bool
		return maybeValues(present) == null;

	/** Check a caller-owned Array without taking another owner. */
	static function borrowedArrayIsPresent(values:Array<Int>):Bool
		return values != null;

	/** Exercise both operators with present and absent results. */
	static function main():Void {
		final borrowed = [5, 6];
		while (!borrowedArrayIsPresent(borrowed) || !freshArrayIsPresent(true) || freshArrayIsPresent(false) || freshArrayIsAbsent(true)
			|| !freshArrayIsAbsent(false)) {}
	}
}
