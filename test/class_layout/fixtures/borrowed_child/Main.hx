/**
	Proves the two admitted lifetimes for a child of a managed parent.

	Haxe introduces a synthetic receiver local for `child.sum()`. That local only
	borrows the child for this method call and needs no independent root. Returning
	the collector-managed child is different: its call result needs addressable C
	storage so the caller's exact root frame can keep the child alive.
**/

/** One managed child whose inline method produces the synthetic alias. */
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

/** A collector-managed parent that owns its managed child field. */
private final class CounterOwner {
	final child:ChildCounter = new ChildCounter(3, 4);

	/** Construct one parent and its owned child. */
	public function new() {}

	/** Invoke the inline child method through the natural Haxe field boundary. */
	public function total():Int
		return child.sum();

	/** Return the traced child with its own collector-visible identity. */
	public function escapedChild():ChildCounter
		return child;
}

/** Executable Eval/native oracle for a bounded borrow and managed return. */
final class Main {
	static function main():Void {
		final owners:Array<CounterOwner> = [];
		owners.push(new CounterOwner());
		if (owners[0].total() != 7)
			throw "borrowed child alias changed its parent-owned value";
		owners[0].escapedChild();
		final escaped = owners[0].escapedChild();
		if (escaped.sum() != 7)
			throw "managed child return changed its value";
	}
}
