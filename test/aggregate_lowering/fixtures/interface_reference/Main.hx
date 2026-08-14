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

/** Groups one call-bounded interface pair without taking object ownership. */
typedef InterfaceRecord = {
	final view:ValueView;
}

/** Executes the interface-record contract through a real generated dispatch. */
final class Main {
	/** Read the borrowed interface before this synchronous call returns. */
	static function read(record:InterfaceRecord):Int
		return record.view.read();

	/** Keep running if record construction, projection, or dispatch drifts. */
	static function main():Void {
		final source = new ValueSource(42);
		final record:InterfaceRecord = {view: source};
		while (read(record) != 42) {}
	}
}
