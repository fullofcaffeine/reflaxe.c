/** A call-bounded behavior contract used by the escape-negative fixture. */
interface ValueView {
	/** Return one visible value. */
	function read():Int;
}

/** Concrete caller-owned storage behind the interface pair. */
final class ValueSource implements ValueView {
	final value:Int;

	/** Store the value read through the interface. */
	public function new(value:Int) {
		this.value = value;
	}

	/** Return the stored value. */
	public function read():Int
		return value;
}

/** A record whose interface pair still borrows the caller-owned object. */
typedef InterfaceRecord = {
	final view:ValueView;
}

/** Attempts to return a record after its synchronous borrow boundary. */
final class Main {
	/** Returning this record could outlive the concrete object behind `view`. */
	static function escape(view:ValueView):InterfaceRecord
		return {view: view};

	/** Keep the unsupported escape reachable. */
	static function main():Void {
		final source = new ValueSource(42);
		final escaped = escape(source);
		while (escaped.view.read() != 42) {}
	}
}
