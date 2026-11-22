/** A polymorphic source contract whose concrete runtime identity is unresolved. */
private interface ValueSource {
	/** Return the visible value without exposing concrete representation. */
	public function read():Int;
}

/** One implementation that must not silently become the interface identity. */
private final class ExactSource implements ValueSource {
	/** Create the concrete implementation. */
	public function new() {}

	/** Return the fixture marker. */
	public function read():Int
		return 7;
}

/** Rejects boxing when only an interface identity is statically available. */
class Main {
	/** Keep the unresolved interface value reachable at a Dynamic boundary. */
	static function main():Void {
		final exact = new ExactSource();
		final source:ValueSource = exact;
		final read = source.read();
		final value:Dynamic = source;
		Sys.println(read + (value == null ? 1 : 0));
	}
}
