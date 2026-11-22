/**
 * Keeps a conditional class lifetime inside a repeating loop fail-closed.
 *
 * Reusing one automatic backing local on a loop back edge needs an explicit
 * destroyed-to-uninitialized transition proof. This bounded slice does not yet
 * claim that neighboring lifetime rule.
 */

/** A value used only during one conditional loop iteration. */
final class ConditionalLoopValue {
	/** Construct one otherwise empty value. */
	public function new() {}

	/** Produce one observable primitive while this iteration owns the value. */
	public function read():Int
		return 7;
}

/** Attempts the unsupported conditional lifetime under a loop boundary. */
final class Main {
	/** Keep the condition mutable so Haxe cannot fold away the inner branch. */
	static var chooseBranch:Bool = true;

	/** The class local must be rejected before target code is emitted. */
	static function main():Void {
		var done = false;
		while (!done) {
			if (chooseBranch) {
				final value = new ConditionalLoopValue();
				done = value.read() == 7;
			} else {
				done = true;
			}
		}
	}
}
