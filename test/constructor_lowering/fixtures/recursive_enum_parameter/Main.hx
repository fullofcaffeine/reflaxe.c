/**
 * Keeps fresh recursive payload-enum arguments fail-closed until their
 * caller-side ownership has a complete lifecycle.
 *
 * Recursive enum payloads use owned indirect storage. The compiler can now
 * identify the constructor's enum representation, but passing `Link(End)` by
 * value still needs an explicit transfer, copy, or release plan. The
 * function-exit validator rejects the program before C emission instead of
 * leaking that fresh owner.
 */
private enum Chain {
	End;
	Link(next:Chain);
}

/** A constructor whose recursive enum still needs call-lifecycle work. */
private final class ChainOwner {
	final value:Chain;

	/** Retains the value only after the compiler can plan caller ownership. */
	public function new(value:Chain) {
		this.value = value;
	}
}

/** Reaches the unsupported constructor boundary. */
final class Main {
	static function main():Void {
		final owner = new ChainOwner(Link(End));
		while (owner == null) {}
	}
}
