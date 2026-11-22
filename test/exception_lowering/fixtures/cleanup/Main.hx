/**
 * Proves that a managed owner is released before a local catch continues.
 *
 * Array needs its own selected runtime slice, but the closed Int exception
 * region must still use ordinary status-free C control flow and must not select
 * the general exception runtime.
 */
class Main {
	/** Throw one copied element while the containing Array remains locally owned. */
	static function releaseBeforeCatch(value:Int):Int {
		try {
			final values = [value, value + 1];
			values.push(value + 2);
			if (value > 0)
				throw values[0];
		} catch (number:Int) {
			return number;
		}
		return -1;
	}

	/** Exit normally only when catch continuation observes the thrown element. */
	static function main():Void {
		final actual = releaseBeforeCatch(7);
		if (actual != 7)
			throw actual;
	}
}
