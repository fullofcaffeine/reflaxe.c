/**
 * Proves that a fresh stack-backed object can receive one immediate method call.
 *
 * `parse` deliberately keeps the natural Haxe expression
 * `new NumberReader(values).read()`. The compiler must give the unnamed reader
 * one automatic C object, call `read` through a temporary borrow, release the
 * Array retained by the reader, and transfer the returned Array to its caller.
 * Naming the reader in Haxe would be equivalent, but requiring that source
 * rewrite would make a C storage detail part of the Haxe API.
 */
final class NumberReader {
	final values:Array<Int>;

	/** Retains the caller's Array for this reader's bounded lifetime. */
	public function new(values:Array<Int>) {
		this.values = values;
	}

	/** Returns fresh managed storage so receiver cleanup cannot consume it. */
	public function read():Array<Int>
		return [values[0] + 1];
}

/** Executes the direct-receiver shape and checks its observable result. */
final class Main {
	/**
	 * Creates a fresh managed argument before the unnamed reader is built.
	 *
	 * This nested call proves that the compiler preserves Haxe's inside-out,
	 * left-to-right evaluation and transfers the temporary Array into the
	 * reader field exactly once.
	 */
	static function valuesFrom(seed:Int):Array<Int>
		return [seed];

	/** Parses without introducing an application-owned reader local. */
	static function parse(values:Array<Int>):Array<Int>
		return new NumberReader(values).read();

	/** Exercises the same receiver with a fresh result nested in `new(...)`. */
	static function parseFresh(seed:Int):Array<Int>
		return new NumberReader(valuesFrom(seed)).read();

	static function main():Void {
		final input = [41];
		final result = parse(input);
		final nested = parseFresh(9);
		while (result.length != 1 || result[0] != 42 || input[0] != 41 || nested.length != 1 || nested[0] != 10) {}
	}
}
