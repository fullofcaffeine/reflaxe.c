/**
	Proves an inline method can name an owned child of a managed parent.

	Haxe introduces a synthetic receiver local for `child.sum()`. That local only
	borrows the child embedded in its caller-owned parent for this method call. It
	must not retain, root, move, or heap-box the child independently.
**/

/** One embedded value whose inline method produces the synthetic alias. */
private final class ChildCounter {
	final left:Int;
	final right:Int;

	/** Construct one direct child value. */
	public function new(left:Int, right:Int) {
		this.left = left;
		this.right = right;
	}

	/** Read two fields so Haxe keeps the inlined receiver alias observable. */
	public inline function sum():Int
		return left + right;
}

/** A collector-managed parent that owns its child as embedded class storage. */
private final class CounterOwner {
	final child:ChildCounter = new ChildCounter(3, 4);

	/** Construct one parent and its owned child. */
	public function new() {}

	/** Invoke the inline child method through the natural Haxe field boundary. */
	public function total():Int
		return child.sum();
}

/** Executable Eval/native oracle for one call-bounded child borrow. */
final class Main {
	static function main():Void {
		final owners:Array<CounterOwner> = [];
		owners.push(new CounterOwner());
		if (owners[0].total() != 7)
			throw "borrowed child alias changed its parent-owned value";
	}
}
