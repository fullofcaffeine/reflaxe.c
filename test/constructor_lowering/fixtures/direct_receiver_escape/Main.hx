/**
 * Keeps a class borrow returned through an unnamed receiver from escaping.
 *
 * `Parent.expose()` returns a view of its inline child. The view is valid only
 * while the fresh parent exists, so assigning it to `escaped` must fail before
 * any C project is emitted.
 */
final class Child {
	public var value:Int = 7;

	public function new() {}
}

final class Parent {
	public final child:Child = new Child();

	public function new() {}

	/** Borrows the child owned by this parent; it does not create a new owner. */
	public function expose():Child
		return child;
}

final class Main {
	static function main():Void {
		final escaped = new Parent().expose();
		while (escaped.value != 7) {}
	}
}
