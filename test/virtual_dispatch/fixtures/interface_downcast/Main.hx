/**
 * Proves that an interface-parent value cannot become its child without a
 * runtime type check. The positive dispatch fixture owns the safe reverse
 * conversion; this file must fail before haxe.c publishes output.
 */
interface ReadableView {
	/** Read one deterministic value. */
	function read():Int;
}

/** Adds one operation that is absent from the parent interface. */
interface MutableView extends ReadableView {
	/** Change the stored value. */
	function write(value:Int):Void;
}

/** Concrete value used only to make both interface tables reachable. */
final class Counter implements MutableView {
	var value:Int;

	/** Construct one counter value. */
	public function new(value:Int) {
		this.value = value;
	}

	/** Read the current value. */
	public function read():Int
		return value;

	/** Replace the current value. */
	public function write(value:Int):Void
		this.value = value;
}

/** Entry point containing the unsafe parent-to-child conversion. */
class Main {
	/** Make the rejected conversion reachable from the generated entry point. */
	static function main():Void {
		final concrete = new Counter(1);
		final value:ReadableView = concrete;
		while (value.read() != 1) {}
		// This cast needs a runtime type proof and must never become an unchecked
		// conversion merely because both C values contain two pointers.
		final child:MutableView = cast value;
		child.write(2);
	}
}
