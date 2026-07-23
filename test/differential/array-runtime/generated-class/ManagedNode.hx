/**
	A mutable class deliberately stored in `Array<ManagedNode>`.

	The Array forces stable heap identity: copying a node reference must not copy
	its fields. `links` also lets two nodes form a cycle, proving that exact tracing
	keeps reachable children alive and that collection does not rely on reference
	counts to reclaim the graph.
**/
final class ManagedNode {
	public var value:Int;
	public final links:Array<ManagedNode> = [];

	/** Ordinary value Array released by this class's generated GC finalizer. */
	public final samples:Array<Int> = [];

	public function new(value:Int) {
		this.value = value;
		samples.push(value);
	}

	public function connect(other:ManagedNode):Void
		links.push(other);

	public function add(delta:Int):Void
		value += delta;

	public function linkCount():Int
		return links.length;

	public function sampleCount():Int
		return samples.length;
}
