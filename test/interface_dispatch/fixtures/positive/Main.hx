/** Runtime-free interface-dispatch probe with a function-bounded object lifetime. */
class Main {
	/**
	 * Read through a call-bounded interface parameter.
	 *
	 * The caller passes a `CounterView`, so this parameter also proves the
	 * ordinary child-interface to parent-interface conversion. The function
	 * never stores, returns, throws, or captures the interface value; haxe.c can
	 * therefore borrow a caller's stack-constructed `Counter` for exactly this
	 * call.
	 */
	static function inspect(readable:ReadableView):Int
		return readable.read(2);

	static function main():Void {
		var concrete = new Counter(40);
		var counter:CounterView = concrete;
		var alternateConcrete = new AlternateCounter(50);
		var alternate:CounterView = alternateConcrete;
		// Counter implements both interfaces, so ordinary assignment keeps the
		// compile-time proof instead of asking the runtime to check a cast.
		var reset:ResetView = concrete;
		while (counter.read(2) != 42 || counter.doubled() != 80 || counter.read(1) != 41 || reset.resetValue() != 0 || inspect(counter) != 42
			|| inspect(alternate) != 52) {}
	}
}
