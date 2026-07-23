package;

final class Node {
	public final value:Int;

	public function new(value:Int)
		this.value = value;
}

enum ManagedChain {
	End(node:Node);
	Link(node:Node, next:ManagedChain);
}

final class Main {
	static function main():Void {
		final value = Link(new Node(1), End(new Node(2)));
		switch value {
			case End(_):
			case Link(_, _):
		}
	}
}
