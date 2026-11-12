/**
	Proves embedded borrowing and escaping identity with distinct child types.

	EmbeddedCounter stays inside its parent, so an inline receiver alias borrows
	its storage. ChildCounter escapes through a return and requires traced storage
	and rooted call results. Separate types keep both whole-program decisions live.
**/

/** An inline child whose references never leave its parent. */
private final class EmbeddedCounter {
	final left:Int;
	final right:Int;

	/** Initialize the two values used by the independent embedded-child oracle. */
	public function new(left:Int, right:Int) {
		this.left = left;
		this.right = right;
	}

	/** Read both fields to keep the synthetic inline receiver alias observable. */
	public inline function sum():Int
		return left + right;
}

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
	final embedded:EmbeddedCounter = new EmbeddedCounter(5, 6);

	/** Construct one parent and its owned child. */
	public function new() {}

	/** Invoke the inline child method through the natural Haxe field boundary. */
	public function total():Int
		return embedded.sum();

	/** Return the traced child with its own collector-visible identity. */
	public function escapedChild():ChildCounter
		return child;
}

/** Executable Eval/native oracle for a bounded borrow and managed return. */
final class Main {
	static function main():Void {
		final owners:Array<CounterOwner> = [];
		owners.push(new CounterOwner());
		if (owners[0].total() != 11)
			throw "borrowed child alias changed its parent-owned value";
		owners[0].escapedChild();
		final escaped = owners[0].escapedChild();
		if (escaped.sum() != 7)
			throw "managed child return changed its value";
	}
}
