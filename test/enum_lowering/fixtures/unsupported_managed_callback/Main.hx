package;

/**
	Keep non-String managed enum-constructor callbacks outside the admitted slice.

	A returned enum would need to acquire an independent owner for this Array
	payload. Until the adapter has that typed failure-safe retain plan, compilation
	must stop at the constructor value instead of emitting plausible C.
**/
enum ManagedEvent {
	Collected(values:Array<String>);
}

/** Exercise the unsupported callback boundary through ordinary typed Haxe. */
class Main {
	/** Invoke a constructor only during this call. */
	static function apply(values:Array<String>, constructor:Array<String>->ManagedEvent):ManagedEvent
		return constructor(values);

	/** Keep the unsupported constructor value reachable from the entry point. */
	static function main():Void {
		final event = apply(["one"], Collected);
		while (event == null) {}
	}
}
