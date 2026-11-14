/**
	A generic base remains dynamically overridable when a child is reachable.

	The C target does not yet emit generic virtual slots, so this fixture must
	fail closed instead of selecting the base implementation by traversal order.
**/
class Box<T> {
	final value:T;

	public function new(value:T) {
		this.value = value;
	}

	public function get():T {
		return value;
	}
}

/** A second effective target for `Box<Int>.get`. */
final class ChildBox extends Box<Int> {
	public function new(value:Int) {
		super(value);
	}

	override public function get():Int {
		return super.get() + 1;
	}
}

/** Keeps both target classes reachable before calling through the base type. */
class Main {
	static function main():Void {
		final base:Box<Int> = new Box<Int>(1);
		final child = new ChildBox(2);
		if (base.get() + child.get() != 4)
			throw "unreachable";
	}
}
