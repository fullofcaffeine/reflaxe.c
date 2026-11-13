/** Exercise the embedded child while managed storage remains live. */
final class Positive {
	/** The independent result is 19 + 23 + 5, through both receiver paths. */
	static function main():Void {
		final owner = new OwnedCounter();
		if (owner.total() != 47 || OwnedCounter.readNullable(owner) != 47)
			throw "managed child scalar mismatch";
	}
}
