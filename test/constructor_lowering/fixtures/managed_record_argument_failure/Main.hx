/**
	Provides the failure half of fresh managed-record constructor ownership.

	The constructor first retains the record, then throws. Generated C must clean
	the partial object and the caller's independent temporary owner exactly once
	before propagating the uncaught exception.
**/

/** One record with nested managed state on both caller and callee cleanup paths. */
private typedef FailureReceipt = {
	final label:String;
	final values:Array<Int>;
}

/** Retains its argument before deliberately exercising constructor failure. */
private final class FailingReceiptReader {
	final receipt:FailureReceipt;

	/** Stores the record, then throws so partial-object cleanup has real work. */
	public function new(receipt:FailureReceipt) {
		this.receipt = receipt;
		throw 99;
	}
}

/** Runs the intentional failure; successful continuation would be a compiler bug. */
final class Main {
	/** Passes a fresh record directly so no source local can hide caller ownership. */
	static function main():Void {
		final ignored = new FailingReceiptReader({label: "failed", values: [99]});
		ignored;
	}
}
