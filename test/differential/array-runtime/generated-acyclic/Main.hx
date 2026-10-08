/**
	Runs the acyclic Array fixture through ordinary Haxe operations and Eval.
**/

/** Executable oracle for the neighboring proven-acyclic collection shape. */
final class Main {
	static function main():Void {
		final nodes:Array<AcyclicNode> = [Item({label: 7}), Empty];
		final alias = nodes;
		alias.push(Item({label: 11}));
		while (nodes.length != 3 || switch nodes[2] {
				case Item(value): value.label != 11;
				case Empty: true;
			}) {}
	}
}
