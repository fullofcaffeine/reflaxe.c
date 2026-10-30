/**
 * Switch results with collector edges need a safe traced carrier before an arm
 * allocates. The final enum remains a tagged value with precise class pointers.
 */
private final class Box {
	/** The immutable payload lets the fixture observe the retained object. */
	public final value:Int;

	/** Store one independently expected value. */
	public function new(value:Int)
		this.value = value;
}

/** Direct and record payloads must both expose their active collector edges. */
private enum Choice {
	One(box:Box);
	Pair(left:{final item:Box;}, right:{final item:Box;});
}

/** Exercise branch initialization, early return and collection after the join. */
final class Main {
	/** Allocate short-lived objects so a native observer can force collection. */
	static function pressure():Void {
		for (i in 0...4000) {
			final box = new Box(i);
			if (box.value != i)
				throw "temporary object changed";
		}
	}

	/** Return a switch-selected value after allocating before and after its store. */
	static function choose(kind:String):Choice {
		final first = new Box(3);
		final result = switch kind {
			case "one":
				pressure();
				One(first);
			case "pair":
				pressure();
				Pair({item: first}, {item: new Box(5)});
			case _:
				return One(new Box(0));
		};
		pressure();
		return result;
	}

	/** Read only the active constructor and require each source-level result. */
	static function main():Void {
		final one = choose("one");
		final pair = choose("pair");
		final fallback = choose("other");
		pressure();
		switch one {
			case One(box):
				if (box.value != 3)
					throw "one payload lost";
			case _:
				throw "one constructor changed";
		}
		switch pair {
			case Pair(left, right):
				if (left.item.value != 3 || right.item.value != 5)
					throw "pair payload lost";
			case _:
				throw "pair constructor changed";
		}
		switch fallback {
			case One(box):
				if (box.value != 0)
					throw "early return changed";
			case _:
				throw "fallback constructor changed";
		}
	}
}
