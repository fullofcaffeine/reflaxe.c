package;

import haxe.io.Bytes;

/**
	Mix collector objects with a separately owned Bytes payload in recursive nodes.

	The collector must preserve shared class identity and release the Bytes owner
	exactly once when the containing node becomes unreachable.
**/
final class Node {
	/** Mutable state makes copied class identity observable. */
	public var value:Int;

	/** Initialize one independently allocated object. */
	public function new(value:Int)
		this.value = value;
}

/** Indirect links share traced nodes; End additionally owns a Bytes handle. */
enum ManagedChain {
	End(node:Node, bytes:Bytes);
	Link(node:Node, next:ManagedChain);
}

/** Return, copy, mutate, and collect a mixed-lifetime recursive value. */
final class Main {
	/** Return after producer roots have expired, retaining the terminal Bytes. */
	static function make():ManagedChain {
		final bytes = Bytes.alloc(1);
		bytes.set(0, 3);
		return Link(new Node(1), End(new Node(2), bytes));
	}

	/** Copies preserve both class and Bytes identity through the shared child. */
	static function copy(value:ManagedChain):ManagedChain
		return value;

	/** A conditional must acquire either a fresh chain or the caller's borrowed value. */
	static function choose(fresh:Bool, borrowed:ManagedChain):ManagedChain
		return fresh ? make() : borrowed;

	/** Read only this finite chain; cyclic graphs are covered by the Array fixture. */
	static function sum(value:ManagedChain):Int
		return switch value {
			case End(node, bytes): node.value + bytes.get(0);
			case Link(node, next): node.value + sum(next);
		};

	/** Trigger collection while the caller retains its returned chain. */
	static function pressure():Void {
		for (index in 0...40000)
			new Node(index);
	}

	/** Both copies must observe the changed terminal object and byte: 1 + 7 + 11. */
	static function main():Void {
		final original = make();
		final copied = copy(original);
		final chosen = choose(false, original);
		final independent = choose(true, original);
		switch copied {
			case Link(_, End(node, bytes)):
				node.value = 7;
				bytes.set(0, 11);
			case _:
		}
		pressure();
		if (sum(original) == 19 && sum(copied) == 19 && sum(chosen) == 19 && sum(independent) == 6)
			Sys.println("19");
		else
			Sys.println("unexpected mixed payload sum");
	}
}
