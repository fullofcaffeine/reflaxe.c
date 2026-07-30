/**
	Provides ordinary Haxe constructor calls with fresh managed record arguments.

	The same record shape owns a String and an Array, then crosses named stack,
	unnamed receiver, inline child, super-constructor, and collector-managed
	construction paths. Eval and generated C can therefore compare one language
	rule without target-specific source.
**/

/** One copy-owned constructor value with two nested managed fields. */
private typedef Receipt = {
	final label:String;
	final values:Array<Int>;
}

/** Managed return value whose scalar field is projected from one fresh owner. */
private typedef ManagedReading = {
	final label:String;
	final values:Array<Int>;
	final reading:Int;
}

/** Stores one record in a class whose complete lifetime stays in its caller. */
private final class StackReceiptReader {
	final receipt:Receipt;

	/** Retains an independent field owner while the caller keeps its argument. */
	public function new(receipt:Receipt) {
		this.receipt = receipt;
	}

	/** Makes the nested String and Array ownership observable after construction. */
	public function read():Int
		return receipt.label.length + receipt.values[0];
}

/** Base class that retains a fresh record supplied by a derived constructor. */
private class BaseReceiptReader {
	final receipt:Receipt;

	/** Retains the record for later reads through the derived value. */
	public function new(receipt:Receipt) {
		this.receipt = receipt;
	}

	/** Reads the complete retained record after the base constructor returns. */
	public function read():Int
		return receipt.label.length + receipt.values[0];
}

/** Creates the managed record directly in a `super(...)` argument. */
private final class DerivedReceiptReader extends BaseReceiptReader {
	/** Exercises the base-call ownership handoff without a source temporary. */
	public function new(value:Int) {
		super({label: "derived", values: [value]});
	}
}

/** Inline child whose constructor receives a fresh managed record. */
private final class ReceiptChild {
	final receipt:Receipt;

	/** Keeps a separate owner for the parent-owned child's later read. */
	public function new(receipt:Receipt) {
		this.receipt = receipt;
	}

	/** Reads the nested fields through the inline child. */
	public function read():Int
		return receipt.label.length + receipt.values[0];
}

/** Owns one child directly so its constructor uses the inline-field path. */
private final class ReceiptParent {
	final child:ReceiptChild;

	/** Constructs the child and its fresh record inside the parent storage. */
	public function new(value:Int) {
		child = new ReceiptChild({label: "owned", values: [value]});
	}

	/** Delegates to the fully initialized child after construction. */
	public function read():Int
		return child.read();
}

/** Escaping class whose storage and retained record are collector-managed. */
private final class ManagedReceiptReader {
	final receipt:Receipt;

	/** Retains the record inside a collector-managed object. */
	public function new(receipt:Receipt) {
		this.receipt = receipt;
	}

	/** Reads the record after its creating helper has returned. */
	public function read():Int
		return receipt.label.length + receipt.values[0];
}

/** Returning this enum makes its class payload escape the constructing helper. */
private enum ManagedReceiptResult {
	ManagedReceiptReady(reader:ManagedReceiptReader);
}

/** Executes every admitted constructor spelling and checks one stable result. */
final class Main {
	static var projectionEvaluations = 0;

	/** Builds an escaping object so whole-program planning selects managed storage. */
	static function buildManaged():ManagedReceiptResult {
		return ManagedReceiptReady(new ManagedReceiptReader({label: "managed", values: [35]}));
	}

	/** Return one fresh managed record and make receiver evaluation observable. */
	static function buildReading():ManagedReading {
		projectionEvaluations += 1;
		return {label: "projected", values: [40, 2], reading: 42};
	}

	/** Keep running only if Eval and generated C disagree about retained values. */
	static function main():Void {
		final named = new StackReceiptReader({label: "n", values: [40]});
		final direct = new StackReceiptReader({label: "dd", values: [40]}).read();
		final derived = new DerivedReceiptReader(35);
		final parent = new ReceiptParent(37);
		final projected = buildReading().reading;
		final managed = switch buildManaged() {
			case ManagedReceiptReady(reader): reader.read();
		};
		while (named.read() != 41 || direct != 42 || derived.read() != 42 || parent.read() != 42 || projected != 42 || projectionEvaluations != 1
			|| managed != 42) {}
	}
}
