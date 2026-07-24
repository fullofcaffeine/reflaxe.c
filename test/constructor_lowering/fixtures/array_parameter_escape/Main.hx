/** Proves an Array constructor parameter cannot escape through an unowned field. */

/** A separate object whose mutable field has no constructor ownership proof. */
private final class OtherOwner {
	/** An existing managed field whose replacement lifetime is not yet admitted. */
	public var values:Array<Int> = [];

	/** Uses the declaration initializer as this field's one proven owner. */
	public function new() {}
}

/** Attempts to retain a constructor parameter outside the object being built. */
private final class InvalidArrayCapture {
	final other:OtherOwner = new OtherOwner();

	/**
	 * This assignment needs a general mutable-field replacement contract.
	 *
	 * Until that contract can retain the new value and release the old one
	 * exactly once, haxe.c must reject it rather than leak or dangle the Array.
	 */
	public function new(values:Array<Int>) {
		other.values = values;
	}
}

/** Keeps the invalid constructor reachable by the custom target. */
final class Main {
	static function main():Void {
		final invalid = new InvalidArrayCapture([1]);
		while (invalid == null) {}
	}
}
