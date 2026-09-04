package;

/**
	Exercises recursive enum nodes whose active payloads contain collector values.

	The direct enum value stays on the Haxe stack. Recursive child nodes use
	collector storage, so their descriptor must trace only the active case and
	keep class, Array, and deeper enum references alive through collection.
**/
/** Mutable identity that reveals a missing or duplicated collector edge. */
final class Node {
	/** Value shared through every alias of this object. */
	public var value:Int;

	/** Create one observable identity with its initial value. */
	public function new(value:Int)
		this.value = value;
}

/** A recursive value with class, recursive-child, and cyclic Array edges. */
enum ManagedChain {
	End(node:Node);
	Link(node:Node, next:ManagedChain);
	Branch(node:Node, children:Array<ManagedChain>);
}

/** Builds and observes representative recursive collector graphs. */
final class Main {
	/** Return an initially absent parser-style local after publishing a chain. */
	static function makeNullableChain():Null<ManagedChain> {
		var value:Null<ManagedChain> = null;
		value = Link(new Node(7), Link(new Node(11), End(new Node(13))));
		return value;
	}

	/** Close an Array edge back to the enum that owns the Array. */
	static function makeCycle():Array<ManagedChain> {
		final children:Array<ManagedChain> = [];
		children.push(Branch(new Node(17), children));
		return children;
	}

	/** Allocate enough unreachable class objects to force deterministic tracing. */
	static function forceCollectionPressure():Void {
		for (index in 0...40000)
			new Node(index);
	}

	/** Read the bounded chain without traversing its intentional Array cycle. */
	static function score(value:ManagedChain):Int {
		return switch value {
			case End(node): node.value;
			case Link(node, next): node.value + score(next);
			case Branch(node, _): node.value;
		};
	}

	/** Keep running only if a recursive trace, active tag, alias, or null is wrong. */
	static function main():Void {
		final absent:Null<ManagedChain> = null;
		final nullable = makeNullableChain();
		final cycle = makeCycle();
		final alias = cycle;

		forceCollectionPressure();

		if (nullable != null)
			switch nullable {
				case Link(node, _):
					node.value += 1;
				case End(_) | Branch(_, _):
			}
		while (absent != null || nullable == null || score(nullable) != 32 || score(cycle[0]) != 17 || score(alias[0]) != 17) {}
	}
}
