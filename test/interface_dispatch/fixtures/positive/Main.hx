/** Runtime-free interface-dispatch probe with a function-bounded object lifetime. */
class Main {
	static function main():Void {
		var concrete = new Counter(40);
		var counter:CounterView = concrete;
		// Counter implements both interfaces, so ordinary assignment keeps the
		// compile-time proof instead of asking the runtime to check a cast.
		var reset:ResetView = concrete;
		while (counter.read(2) != 42 || counter.doubled() != 80 || reset.resetValue() != 0) {}
	}
}
