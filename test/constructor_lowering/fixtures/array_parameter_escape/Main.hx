/** Proves an Array constructor parameter cannot escape through an unowned field. */

/** Holds a process-wide Array whose replacement ownership is not admitted. */
private final class OtherOwner {
	/** A static destination has no instance cleanup owner for a replacement yet. */
	public static var values:Array<Int> = [];
}

/** Attempts to retain a constructor parameter outside the object being built. */
private final class InvalidArrayCapture {
	/**
	 * This assignment needs a static-field replacement contract.
	 *
	 * Instance fields now acquire the replacement, release the previous owner,
	 * and store safely. A static owner has a different lifetime, so haxe.c must
	 * still reject this escape rather than leak or dangle the Array.
	 */
	public function new(values:Array<Int>) {
		OtherOwner.values = values;
	}
}

/** Keeps the invalid constructor reachable by the custom target. */
final class Main {
	static function main():Void {
		final invalid = new InvalidArrayCapture([1]);
		while (invalid == null) {}
	}
}
