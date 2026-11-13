/**
 * Keeps an initialized child inside one parent's lifetime.
 *
 * This reduction keeps a managed Array live while the method reads its embedded child.
 * The generated report must establish a real cleanup region; source syntax alone
 * does not prove that the compiler selected one.
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

	/** Keep the synthetic child receiver visible after ordinary Haxe inlining. */
	public inline function sum():Int
		return left + right;
}

/** Owns the child; this fixture neither returns it nor stores it elsewhere. */
final class OwnedCounter {
	final child:ChildCounter = new ChildCounter(19, 23);

	/** Complete child initialization before exposing the parent to callers. */
	public function new() {}

	/** Keep managed storage live across the child read; 19 + 23 + 5 is 47. */
	public function total():Int {
		final scratch = new Array<Int>();
		scratch.push(5);
		final sum = child.sum();
		return sum + scratch[0];
	}

	/**
	 * Keep a nullable receiver boundary independent of the positive call site.
	 * Ordinary Haxe class references can be null. A caller with null must still
	 * reach the compiler's checked fail-stop instead of an unchecked C load.
	 */
	public static function readNullable(value:OwnedCounter):Int
		return value.total();
}
