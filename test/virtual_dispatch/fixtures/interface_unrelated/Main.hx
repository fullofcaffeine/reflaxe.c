/**
 * Proves that nominally unrelated interface values do not become compatible
 * merely because generated C represents both as object-and-table pairs.
 */
interface ReadableView {
	/** Read one deterministic value. */
	function read():Int;
}

/** An unrelated capability with a different nominal identity. */
interface ResetView {
	/** Reset and return one deterministic value. */
	function reset():Int;
}

/** Concrete source value that implements only the readable capability. */
final class Counter implements ReadableView {
	final value:Int;

	/** Construct one immutable counter. */
	public function new(value:Int) {
		this.value = value;
	}

	/** Read the immutable value. */
	public function read():Int
		return value;
}

/** Entry point containing the unrelated-interface conversion. */
class Main {
	/** Make the rejected conversion reachable from the generated entry point. */
	static function main():Void {
		final concrete = new Counter(1);
		final value:ReadableView = concrete;
		while (value.read() != 1) {}
		// Identical C carrier shapes do not erase Haxe's nominal type boundary.
		final reset:ResetView = cast value;
		while (reset.reset() != 0) {}
	}
}
