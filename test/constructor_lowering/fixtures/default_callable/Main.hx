/**
 * Keeps an optional callable constructor parameter fail-closed.
 *
 * Direct optional/default completion does not prove the representation,
 * ownership, or lifetime of every possible value family. Function values need
 * their own constructor-call contract before an omitted `null` can be admitted.
 */

/**
 * Uses a parameter representation that haxe.c does not yet admit in constructors.
 *
 * The Haxe constructor is valid. Only the compiler's C representation is
 * pending: a function value may carry captured state whose ownership and
 * lifetime must be explicit before it can cross this call boundary.
 */
private final class CallbackOwner {
	/** Require a clear compiler diagnostic before any incomplete C is emitted. */
	public function new(?callback:() -> Int) {
		if (callback != null)
			callback();
	}
}

/** Reaches the unsupported constructor so its diagnostic remains executable. */
class Main {
	static function main():Void {
		final owner = new CallbackOwner();
		owner;
	}
}
