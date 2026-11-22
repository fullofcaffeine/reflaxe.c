/**
 * Checks both branch results without depending on compiler proof internals.
 * The child stores 19 and 23: the two results must be 19 + 19 and 23 + 19.
 */
final class Positive {
	/** Fail visibly if either arm or the shared final read changes its value. */
	static function main():Void {
		final parent = new OwnedBranch();
		if (parent.total(true) != 38 || parent.total(false) != 42)
			throw "owned child branch scalar mismatch";
	}
}
