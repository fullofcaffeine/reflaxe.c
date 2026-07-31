/**
	Exercises nullable recursive class results returned through a borrowed receiver.

	A tree node may contain an Array of more nodes, so any node that leaves its
	parser helper needs collector-managed storage. The parser itself is borrowed
	only while each method runs; it must not become the owner of the returned tree.
**/

/** Recursive tree value with both fieldless and managed payload variants. */
private enum TreeValue {
	Leaf(value:Int);
	Children(values:Array<TreeNode>);
}

/** One immutable node in the parser-produced recursive tree. */
private final class TreeNode {
	final value:TreeValue;

	/** Retains the complete recursive value for later traversal. */
	public function new(value:TreeValue) {
		this.value = value;
	}

	/** Read one deterministic value after the parser call has ended. */
	public function score():Int {
		return switch value {
			case Leaf(value): value;
			case Children(values): values[0].score();
		};
	}
}

/** Complete parser result or an explicit rejected branch. */
private enum TreeReadResult {
	TreeReady(root:TreeNode);
	TreeRejected;
}

/** Stateful producer whose receiver is borrowed during each method call. */
private final class TreeParser {
	/** Construct one request-local parser with no retained input state. */
	public function new() {}

	/** Return either no node or one newly owned recursive node. */
	function parseValue(value:Int):Null<TreeNode> {
		if (value <= 0)
			return null;
		return new TreeNode(Children([new TreeNode(Leaf(value))]));
	}

	/** Store the nullable method result before checking and publishing it. */
	public function parse(value:Int):TreeReadResult {
		final root = parseValue(value);
		if (root == null)
			return TreeRejected;
		return TreeReady(root);
	}
}

/** Runs the ownership checks without relying on console or file output. */
final class Main {
	/** Project one completed parser result after its producing call has ended. */
	static function score(result:TreeReadResult):Int {
		return switch result {
			case TreeReady(root): root.score();
			case TreeRejected: -1;
		};
	}

	/** Keeps running only when a nullable result, root, or cleanup is wrong. */
	static function main():Void {
		final parser = new TreeParser();
		final rejected = parser.parse(0);
		final accepted = parser.parse(42);
		for (index in 0...40000)
			new TreeNode(Leaf(index));
		while (score(rejected) != -1 || score(accepted) != 42) {}
	}
}
