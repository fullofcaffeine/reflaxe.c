/**
 * Keeps recursive payload enums outside the unmanaged constructor copy rule.
 *
 * Recursive enum payloads use owned indirect storage. Passing or retaining one
 * by value requires an explicit deep-copy or borrow contract, so constructor
 * admission must fail before generated C is emitted.
 */
private enum Chain {
	End;
	Link(next:Chain);
}

/** A deliberately unsupported recursive-enum constructor parameter. */
private final class ChainOwner {
	final value:Chain;

	/** Must remain rejected until recursive constructor ownership is proven. */
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
