/** Proves a borrowed owned-child alias cannot escape its parent method. */

/** Direct child storage must remain owned by its enclosing parent. */
private final class ChildCounter {
	/** Construct one child value. */
	public function new() {}
}

/** Deliberately attempts to replace a local alias to an embedded child. */
private final class CounterOwner {
	final child:ChildCounter = new ChildCounter();
	final replacement:ChildCounter = new ChildCounter();

	/** Construct one parent and its owned child. */
	public function new() {}

	/** This owner replacement must fail before haxe.c emits plausible C. */
	public function replaceAlias():Void {
		var alias = child;
		alias = replacement;
	}
}

/** Keeps the invalid borrowed-child replacement reachable. */
final class Main {
	static function main():Void {
		final owners:Array<CounterOwner> = [];
		owners.push(new CounterOwner());
		owners[0].replaceAlias();
	}
}
