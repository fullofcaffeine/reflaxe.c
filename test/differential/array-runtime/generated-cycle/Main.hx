/**
	Builds observable cyclic Array graphs for Eval and generated native execution.
**/

/**
	Executable oracle for cycles composed only from Array, record, and enum values.

	Each helper checks ordinary Haxe alias and mutation behavior. The native test
	driver separately forces collection after `main` returns and proves that no
	cycle survives merely because the old Array reference counter cannot reach zero.
**/
final class Main {
	/** Build one Array -> enum -> record -> same Array self-cycle. */
	static function selfCycle():Int {
		final nodes:Array<GraphNode> = [];
		final alias = nodes;
		nodes.push(Linked({label: 11, next: alias}));
		return switch nodes[0] {
			case Linked(edge) if (edge.next == nodes): edge.label + edge.next.length;
			case _: -1;
		};
	}

	/** Build two Arrays whose tagged record payloads point at each other. */
	static function mutualCycle():Int {
		final left:Array<GraphNode> = [];
		final right:Array<GraphNode> = [];
		left.push(Linked({label: 20, next: right}));
		right.push(Linked({label: 21, next: left}));
		final alias = right;
		alias.push(Marker(1));
		return switch [left[0], right[0]] {
			case [Linked(leftEdge), Linked(rightEdge)] if (leftEdge.next == right && rightEdge.next == left):
				leftEdge.label + rightEdge.label + right.length;
			case _: -1;
		};
	}

	/** Replace the only back edge and prove that aliases observe the breakup. */
	static function breakCycle():Int {
		final nodes:Array<GraphNode> = [];
		final alias = nodes;
		nodes.push(Linked({label: 30, next: nodes}));
		nodes[0] = Marker(7);
		return switch alias[0] {
			case Marker(value): value + alias.length;
			case _: -1;
		};
	}

	/** Build a deep graph and close its final edge back to the first Array. */
	static function deepCycle():Int {
		final first:Array<GraphNode> = [];
		final tail = buildTail(511, first);
		first.push(Linked({label: 0, next: tail}));
		return first.length + tail.length;
	}

	/** Recursively allocate one link per call without reassigning an Array owner. */
	static function buildTail(depth:Int, first:Array<GraphNode>):Array<GraphNode> {
		final current:Array<GraphNode> = [];
		if (depth == 0)
			current.push(Linked({label: 512, next: first}));
		else
			current.push(Linked({label: 512 - depth, next: buildTail(depth - 1, first)}));
		return current;
	}

	/** Allocate unreachable cycles until the collector crosses its pressure bound. */
	static function pressure():Int {
		var checksum = 0;
		for (index in 0...40000) {
			final nodes:Array<GraphNode> = [];
			nodes.push(Linked({label: index, next: nodes}));
			checksum += nodes.length;
		}
		return checksum;
	}

	static function main():Void {
		while (selfCycle() != 12 || mutualCycle() != 43 || breakCycle() != 8 || deepCycle() != 2 || pressure() != 40000) {}
	}
}
