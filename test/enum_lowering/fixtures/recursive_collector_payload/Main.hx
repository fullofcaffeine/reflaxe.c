package;

/**
	Reduce recursive enum ownership without application-specific types.

	Wrapped requires an indirect enum node. Group can form a cycle through its
	mutable Array, so that Array needs collection even though Leaf stores an Int.
	The positive compiler contract remains open under haxe_c-7jb8.
**/
enum Tree {
	Leaf(value:Int);
	Group(children:Array<Tree>);
	Wrapped(child:Tree);
}

/** Exercise returned nodes, shared Array mutation, and a bounded cycle read. */
final class Main {
	/** Return a tree after its producer's local roots have gone away. */
	static function make():Tree
		return Wrapped(Group([Leaf(7)]));

	/** Copying an enum must preserve the identity of its Array payload. */
	static function copy(value:Tree):Tree
		return value;

	/** Sum an acyclic input; the separate cycle check never calls this function. */
	static function sum(value:Tree):Int {
		return switch value {
			case Leaf(number): number;
			case Wrapped(child): sum(child);
			case Group(children):
				var total = 0;
				for (child in children)
					total += sum(child);
				total;
		};
	}

	/** Allocate short-lived collector Arrays while the caller keeps its tree. */
	static function pressure():Void {
		for (index in 0...40000) {
			final temporary:Array<Tree> = [Leaf(index)];
			while (temporary.length != 1) {}
		}
	}

	/** Read one edge of a cyclic graph without recursively traversing the cycle. */
	static function cycleLength():Int {
		final children:Array<Tree> = [];
		final cycle = Wrapped(Group(children));
		children.push(cycle);
		pressure();
		return switch children[0] {
			case Wrapped(Group(shared)): shared.length;
			case _: -1;
		};
	}

	/** Both copies must observe 7 + 11, and the cyclic Array has one element. */
	static function main():Void {
		final original = make();
		final copied = copy(original);
		switch copied {
			case Wrapped(Group(children)):
				children.push(Leaf(11));
			case _:
		}
		pressure();
		if (sum(original) == 18)
			Sys.println("18");
		else
			Sys.println("unexpected original sum");
		if (sum(copied) == 18)
			Sys.println("18");
		else
			Sys.println("unexpected copied sum");
		if (cycleLength() == 1)
			Sys.println("1");
		else
			Sys.println("unexpected cycle length");
	}
}
