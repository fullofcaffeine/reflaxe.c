package layout.model;

/** One half of a pointer-only cross-module type cycle. */
class Left {
	public var right:Null<Right>;

	public function new() {
		right = null;
	}
}
