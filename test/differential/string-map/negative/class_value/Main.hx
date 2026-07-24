/**
	Proves that admitting direct records does not admit arbitrary object graphs.

	Class references have identity, mutation, escape, and collector questions
	that a record callback cannot answer. `Map<String, Box>` must therefore fail
	before C output until the class/collector layer supplies an exact plan.
**/

/** Mutable identity value deliberately outside the direct-record subset. */
private final class Box {
	public var value:Int;

	/** Create one identity-bearing value for the rejected map insertion. */
	public function new(value:Int)
		this.value = value;
}

/** Keeps the unsupported class-value diagnostic isolated from valid records. */
final class Main {
	static function main():Void {
		final values:Map<String, Box> = [];
		values.set("box", new Box(7));
	}
}
