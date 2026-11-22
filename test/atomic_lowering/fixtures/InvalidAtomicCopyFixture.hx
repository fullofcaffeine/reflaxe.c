import c.Atomic;

/** Attempts the storage copy that `c.Atomic` deliberately forbids. */
private final class InvalidAtomicCopyOwner {
	var first:Atomic<Int>;
	var second:Atomic<Int>;

	/** The second assignment must not become a C `_Atomic` struct copy. */
	public function new() {
		first = new Atomic(1);
		second = new Atomic(2);
	}

	/** Attempt to replace one initialized location with another location's storage. */
	public function copy():Void {
		second = first;
	}
}

/** Must fail during typed lowering. */
class InvalidAtomicCopyFixture {
	/** Reach the invalid constructor. */
	static function main():Void {
		final owner = new InvalidAtomicCopyOwner();
		owner.copy();
	}
}
