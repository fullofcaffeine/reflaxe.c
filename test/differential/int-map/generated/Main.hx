/**
	Provides the executable Haxe oracle for the first integer-keyed Map slice.

	The same ordinary Haxe source runs under Eval and through haxe.c. It keeps
	keys and values dynamic enough to exercise the table rather than letting the
	compiler replace the whole example with constants.
**/
final class Main {
	/** Build one shared table and prove that aliases observe the same mutations. */
	static function sharedMembership(seed:Int):Bool {
		final values:Map<Int, Bool> = [];
		final alias = values;
		values.set(seed, false);
		values.set(seed + 1, true);
		if (!alias.exists(seed) || !alias.exists(seed + 1) || alias.exists(seed + 2))
			return false;
		alias.set(seed + 2, true);
		return values.exists(seed + 2);
	}

	/**
		Run the bounded semantic trace without requiring console or file support.

		A correct build returns normally. A mismatch stays in the final loop, so
		the test runner observes it as a bounded timeout on both Eval and native C.
	**/
	static function main():Void {
		if (!sharedMembership(40))
			while (true) {}
	}
}
