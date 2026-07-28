/**
	Proves that a fresh Array cannot cross an indirect callable boundary yet.

	The direct target is hidden behind a function value, so haxe.c cannot apply
	the direct callee's borrowed-parameter contract. It must fail before writing
	C rather than guessing whether the callable keeps or consumes the Array.
**/
final class Main {
	static function main():Void {
		final read:Array<Int>->Int = count;
		final result = read([1, 2]);
		while (result != 2) {}
	}

	/** Read one caller-owned Array without retaining it. */
	static function count(values:Array<Int>):Int
		return values.length;
}
