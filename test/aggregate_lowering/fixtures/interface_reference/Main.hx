/** A closed behavior contract that still needs an interface carrier in C. */
interface ValueView {
	/** Return the value visible through this interface. */
	function read():Int;
}

/** One concrete implementation used only to type-check the fixture. */
final class ValueSource implements ValueView {
	final value:Int;

	/** Store the value returned through `ValueView`. */
	public function new(value:Int) {
		this.value = value;
	}

	/** Read the stored value. */
	public function read():Int
		return value;
}

/**
 * The first class-reference record slice deliberately excludes interfaces.
 *
 * An interface is a value pair containing an object and dispatch table, not the
 * single class pointer admitted by the new record-field representation.
 */
typedef InterfaceRecord = {
	final view:ValueView;
}

/** Attempts the unsupported interface-record shape. */
final class Main {
	/** Keep the record visible at a function boundary so lowering must plan it. */
	static function group(view:ValueView):InterfaceRecord
		return {view: view};

	/** A correct compiler rejects the record type before generating C. */
	static function main():Void {
		final source = new ValueSource(42);
		final record = group(source);
		while (record.view.read() != 42) {}
	}
}
