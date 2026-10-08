/**
 * Proves the runtime-free exception path through generated strict C.
 *
 * The inner String catch is incompatible with the thrown Int. The following
 * Int catch handles it and rethrows to the enclosing Int catch. A wrong result
 * reaches the established uncaught fail-stop path, so native exit status is an
 * independent observer without adding output or another runtime feature.
 */
class Main {
	/** Exercise first-compatible matching and rethrow to an enclosing region. */
	static function nestedCatchAndRethrow(value:Int):Int {
		try {
			try {
				if (value > 0)
					throw value;
			} catch (message:String) {
				return 99;
			} catch (number:Int) {
				throw number + 1;
			}
		} catch (number:Int) {
			return number;
		}
		return -1;
	}

	/** Exit normally only when generated C preserves the Eval-owned result. */
	static function main():Void {
		final actual = nestedCatchAndRethrow(7);
		if (actual != 8)
			throw actual;
	}
}
