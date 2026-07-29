/**
 * Proves that a root-level guard may finish before a later local object exists.
 *
 * The false path returns before construction. The surviving path creates one
 * nonescaping `GuardedValue`, uses it, and then returns. Generated HxcIR must
 * therefore attach no object cleanup to the early return and exactly one
 * initialized-object cleanup to the later return.
 */

/** A small identity-bearing value that remains inside one `run` call. */
final class GuardedValue {
	var stored:Int;

	/** Store the already validated input in this instance. */
	public function new(value:Int)
		stored = value;

	/** Read the value while the caller-owned instance is still alive. */
	public function read():Int
		return stored;
}

/** Exercises both the rejected guard path and the constructed path. */
final class Main {
	/**
	 * Return early for invalid input, otherwise use one stack-backed object.
	 *
	 * `GuardedValue` neither escapes nor needs collector storage. Its automatic
	 * C backing storage may be declared for the function, but initialization must
	 * remain after this guard so constructor effects are never moved earlier.
	 */
	static function run(value:Int):Int {
		if (value <= 0)
			return 11;
		final guarded = new GuardedValue(value);
		return guarded.read();
	}

	/** Keep running only if generated C disagrees with the Haxe Eval oracle. */
	static function main():Void
		while (run(0) != 11 || run(37) != 37) {}
}
