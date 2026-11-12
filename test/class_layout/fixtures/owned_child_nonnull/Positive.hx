/**
 * Checks an independently specified result for a locally constructed parent.
 *
 * Both calls must return 42: the child stores 19 and 23, whose sum is 42.
 * Success proves this scalar behavior only; it does not prove that a generated
 * null check is redundant or that an entire measured interval is allocation-free.
 */
final class Positive {
	/** Fail visibly if either ordinary method path changes its scalar result. */
	static function main():Void {
		final owner = new OwnedCounter();
		final direct = owner.total();
		final throughNullableBoundary = OwnedCounter.readNullable(owner);
		if (direct != 42 || throughNullableBoundary != 42)
			throw "owned child scalar mismatch";
	}
}
