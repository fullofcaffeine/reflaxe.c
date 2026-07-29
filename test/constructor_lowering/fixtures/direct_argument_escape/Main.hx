/**
	Defines an object passed to a deliberately overridable call.

	The method body shown here only reads its parameter, but a subclass may replace
	that method and retain the value. haxe.c therefore has no exact synchronous
	body with which to prove a call-bounded borrow and must reject automatic fresh
	storage before it emits plausible C.
**/
final class ArgumentValue {
	public var value:Int = 7;

	/** Create the small value used by the unsafe direct-argument expression. */
	public function new() {}
}

/** An intentionally non-final call target whose method can be overridden. */
class ArgumentSink {
	/** Construct the stateless receiver. */
	public function new() {}

	/** Read the argument in this implementation; an override need not do so. */
	public function observe(value:ArgumentValue):Int
		return value.value;
}

/** Reaches the virtual fresh-argument escape that must fail before C emission. */
final class Main {
	/** Pass an unnamed automatic object to the non-borrowing helper. */
	static function main():Void {
		final sink = new ArgumentSink();
		while (sink.observe(new ArgumentValue()) != 7) {}
	}
}
