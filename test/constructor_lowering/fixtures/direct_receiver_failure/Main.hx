/**
 * Exercises cleanup when an unnamed receiver's constructor throws.
 *
 * The fresh Array argument and the partially initialized reader each own
 * managed storage before `throw` runs. Generated C must release both owners
 * exactly once before propagating the constructor failure to the program's
 * fail-closed abort boundary. `read` is deliberately unreachable.
 */
final class ExplodingReader {
	final values:Array<Int>;

	/** Retains `values`, then fails when requested so partial cleanup is tested. */
	public function new(values:Array<Int>, shouldFail:Bool) {
		this.values = values;
		if (shouldFail)
			throw 99;
	}

	/** Supplies the ordinary instance call following construction. */
	public function read():Int
		return values[0];
}

/** Runs the failure path; successful continuation would be a compiler bug. */
final class Main {
	static function main():Void {
		final result = new ExplodingReader([41], true).read();
		while (result != 41) {}
	}
}
