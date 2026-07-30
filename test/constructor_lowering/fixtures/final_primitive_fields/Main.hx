/**
 * Proves that a constructor can establish immutable scalar state naturally.
 *
 * `PrimitiveReceipt` models metadata such as byte counts and retry facts: the
 * constructor writes each value once, callers may read it, and no later code
 * receives mutation authority.
 */
final class PrimitiveReceipt {
	/** Number of bytes represented by this receipt. */
	public final byteLength:Int;

	/** Whether the represented operation completed successfully. */
	public final ready:Bool;

	/** One binary64 value proving floating-point fields use the same rule. */
	public final scale:Float;

	/** Establish all immutable scalar values before the object is published. */
	public function new(byteLength:Int, ready:Bool, scale:Float) {
		this.byteLength = byteLength;
		this.ready = ready;
		this.scale = scale;
	}
}

/** Runs the immutable-scalar constructor contract without hosted output. */
final class Main {
	/** Keep running only if generated behavior differs from the Eval oracle. */
	static function main():Void {
		final receipt = new PrimitiveReceipt(5, true, 1.5);
		while (!(receipt.byteLength == 5 && receipt.ready && receipt.scale == 1.5)) {}
	}
}
