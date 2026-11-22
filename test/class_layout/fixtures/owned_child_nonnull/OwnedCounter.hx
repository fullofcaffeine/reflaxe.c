/**
 * Keeps an initialized child inside one parent's lifetime.
 *
 * This reduction isolates the owned-child address used by an ordinary method.
 * The class has identity and constructor-owned storage; it is not a namespace.
 * A final field forbids reassignment but does not itself prove a non-null value.
 */

/** Supplies two independent scalar fields without a managed payload. */
private final class ChildCounter {
	final left:Int;
	final right:Int;

	/** Initialize both fields before the parent can read the child. */
	public function new(left:Int, right:Int) {
		this.left = left;
		this.right = right;
	}

	/** Expose one scalar read without an intervening arithmetic operation. */
	public inline function first():Int
		return left;

	/** Keep the synthetic child receiver visible after ordinary Haxe inlining. */
	public inline function sum():Int
		return left + right;
}

/** Owns the child; this fixture neither returns it nor stores it elsewhere. */
final class OwnedCounter {
	final child:ChildCounter = new ChildCounter(19, 23);

	/** Complete child initialization before exposing the parent to callers. */
	public function new() {}

	/** Read the initialized child through the same natural field/method shape. */
	public function total():Int
		return child.sum();

	/** Borrow the child twice while the parent reference stays the same. */
	public function repeatedTotal():Int
		return child.sum() + child.sum();

	/** Both paths check the parent before the conditional borrow. */
	public function checkedBeforeBranch(repeat:Bool):Int {
		final first = child.first();
		return repeat ? first + child.first() : first;
	}

	/** The shared read needs a guard because one path skips the first borrow. */
	public function checkedOnOnePath(repeat:Bool):Int {
		final first = repeat ? child.first() : 0;
		return first + child.first();
	}

	/**
	 * Keep a nullable receiver boundary independent of the positive call site.
	 * Ordinary Haxe class references can be null. A caller with null must still
	 * reach the compiler's checked fail-stop instead of an unchecked C load.
	 */
	public static function readNullable(value:OwnedCounter):Int
		return value.total();
}
