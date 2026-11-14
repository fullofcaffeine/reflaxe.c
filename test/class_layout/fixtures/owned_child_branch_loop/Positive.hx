/**
 * Checks both branch results without depending on compiler proof internals.
 * Two iterations give 2 * (19 + 19) and 2 * (23 + 19), independently known as 76 and 84.
 */
final class Positive {
	/** Fail visibly if either arm or the shared final read changes its value. */
	static function main():Void {
		final parent = new OwnedBranch();
		if (parent.total(true) != 76 || parent.total(false) != 84)
			throw "owned child branch scalar mismatch";
	}
}
