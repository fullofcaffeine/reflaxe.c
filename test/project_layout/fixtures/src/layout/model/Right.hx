package layout.model;

/** The other half; common forwards avoid mutually including implementation. */
class Right {
	public var left:Null<Left>;

	public function new() {
		left = null;
	}
}
