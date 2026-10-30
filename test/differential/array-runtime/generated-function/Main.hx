/**
	Exercises exact non-capturing function pointers as ordinary `Array` elements.

	The two element signatures prove that specialization keeps complete callable
	identity. Construction, indexed calls, replacement, growth, copying, sorting,
	and iteration all use the same unboxed Array runtime as other trivial values.
**/
final class Main {
	/** Add one to the supplied value. */
	static function increment(value:Int):Int
		return value + 1;

	/** Negate the supplied value. */
	static function negate(value:Int):Int
		return -value;

	/** Double the supplied value. */
	static function doubleValue(value:Int):Int
		return value * 2;

	/** Add two integer arguments. */
	static function add(left:Int, right:Int):Int
		return left + right;

	/** Subtract the right argument from the left argument. */
	static function subtract(left:Int, right:Int):Int
		return left - right;

	/** Multiply two integer arguments. */
	static function multiply(left:Int, right:Int):Int
		return left * right;

	/** Call one unary element selected at runtime. */
	static function applyUnaryAt(operations:Array<Int->Int>, index:Int, value:Int):Int
		return operations[index](value);

	/** Call one binary element selected at runtime. */
	static function applyBinaryAt(operations:Array<(Int, Int) -> Int>, index:Int, left:Int, right:Int):Int
		return operations[index](left, right);

	/** Run the two-signature Array contract without producing ordinary output. */
	static function main():Void {
		final unary:Array<Int->Int> = [increment, negate];
		unary.push(doubleValue);
		unary[1] = increment;
		final unaryCopy = unary.copy();
		unaryCopy.sort((left, right) -> right(3) - left(3));
		var unaryTrace = 0;
		for (operation in unaryCopy)
			unaryTrace = unaryTrace * 10 + operation(3);

		final binary:Array<(Int, Int) -> Int> = [add, subtract];
		binary.push(multiply);
		binary[0] = subtract;
		final binaryCopy = binary.copy();
		binaryCopy.sort((left, right) -> right(6, 2) - left(6, 2));
		var binaryTrace = 0;
		for (operation in binaryCopy)
			binaryTrace = binaryTrace * 100 + operation(6, 2);

		if (applyUnaryAt(unary, 2, 4) != 8 || applyBinaryAt(binary, 2, 3, 4) != 12 || unaryTrace != 644 || binaryTrace != 120404)
			throw "function Array result mismatch";
	}
}
