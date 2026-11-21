/** A nullable reading gives the fixture a value that only a present receiver can supply. */
final class Reading {
	/** The observed number also determines whether the reading can be enabled. */
	public final value:Int;

	/** Keep the number on the instance so the final read must use its receiver. */
	public function new(value:Int) {
		this.value = value;
	}

	/** A present reading can still make the right side of the conjunction false. */
	public function enabled(allow:Bool):Bool {
		return allow && value > 0;
	}

	/** Inlining the calculation puts the result assignment inside the present arm. */
	public inline function above(minimum:Int):Bool {
		final excess = value - minimum;
		return excess > 0;
	}
}

/** Return a nullable reading so the caller exercises the same code with a present or absent object. */
function readingIf(present:Bool):Null<Reading> {
	return present ? new Reading(17) : null;
}
