/**
 * Exercises borrowed and retained Array constructor parameters.
 *
 * The same shared Array identity may be read only during construction or kept
 * by the new object. The generated C must borrow for the first case, retain one
 * field owner for reference-counted arrays in the second case, and keep that
 * owner valid while the caller mutates the shared container and unrelated
 * Array allocations are created and released.
 */

/** Reads a caller-owned Array only while its constructor is running. */
private final class BorrowedArrayReader {
	/** The constructor's result, kept separately from the borrowed Array. */
	public var observed:Int;

	/** Computes a value without keeping `values` after this call returns. */
	public function new(values:Array<Int>) {
		observed = values[0] + values.length;
	}
}

/** Keeps the same shared Array identity for reads after construction. */
private final class RetainedArrayReader {
	final values:Array<Int>;

	/**
	 * Gives this object its own lifetime claim on `values`.
	 *
	 * The source Array is not copied. In generated C, the field gains one
	 * reference-counted owner that the object's cleanup later releases.
	 */
	public function new(values:Array<Int>) {
		this.values = values;
	}

	/** Reads the retained container after its constructor and caller have returned. */
	public function observed():Int
		return values[0] + values.length;
}

/** Runs the semantic checks without depending on console or file output. */
final class Main {
	/**
	 * Repeatedly allocates and releases unrelated Array owners.
	 *
	 * `Array<Int>` uses reference counting rather than the tracing collector, so
	 * allocation pressure—not a forced collection—is the relevant lifetime
	 * check for this specialization.
	 */
	static function forceArrayPressure():Void {
		for (index in 0...40000) {
			final temporary = [index];
			if (temporary[0] != index)
				return;
		}
	}

	/** Keeps running only if either constructor ownership contract is wrong. */
	static function main():Void {
		final values = [39, 1];
		final borrowed = new BorrowedArrayReader(values);
		final retained = new RetainedArrayReader(values);
		values.push(1);
		final fresh = new RetainedArrayReader([40]);
		forceArrayPressure();
		while (borrowed.observed != 41 || retained.observed() != 42 || fresh.observed() != 41 || values.length != 3) {}
	}
}
