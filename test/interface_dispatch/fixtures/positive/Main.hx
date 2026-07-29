/** Runtime-free interface-dispatch probe with a function-bounded object lifetime. */
class Main {
	/**
	 * Read through a call-bounded interface parameter.
	 *
	 * The function may dispatch through `CounterView`, but it never stores,
	 * returns, throws, or captures the interface value. haxe.c can therefore
	 * borrow a caller's stack-constructed `Counter` for exactly this call.
	 */
	static function inspect(counter:CounterView):Int
		return counter.read(2) + counter.doubled();

	static function main():Void {
		var concrete = new Counter(40);
		var counter:CounterView = concrete;
		// Counter implements both interfaces, so ordinary assignment keeps the
		// compile-time proof instead of asking the runtime to check a cast.
		var reset:ResetView = concrete;
		while (counter.read(2) != 42 || counter.doubled() != 80 || reset.resetValue() != 0 || inspect(concrete) != 122) {}
	}
}
