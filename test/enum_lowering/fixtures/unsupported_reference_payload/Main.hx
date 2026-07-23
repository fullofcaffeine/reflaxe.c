package;

/**
 * Proves that haxe.c currently rejects a `Map<String, Bool>` stored in a Haxe
 * enum payload instead of generating C with unsafe ownership.
 *
 * An enum with payloads becomes a C tagged union: an integer records which case
 * is active, and all cases reuse the same payload memory. `Map<String, Bool>`
 * is represented by a reference-counted StringMap pointer. Before this shape
 * can be emitted safely, the generated enum copy and destroy helpers must know
 * to retain or release that pointer only when `UnsupportedValue` is active.
 *
 * That StringMap-specific enum lifetime policy is not implemented yet. Without
 * it, copying the enum could leave one copy pointing at a map that was freed by
 * another copy, or destruction could leak the map. The expected result of this
 * fixture is therefore a source-positioned compiler error and no generated C.
 */
enum UnsupportedResult {
	UnsupportedValue(values:Map<String, Bool>);
}

/**
 * Makes the unsupported enum case reachable through ordinary Haxe.
 *
 * Constructing the value forces enum preparation to inspect the Map payload;
 * no target-specific escape hatch is involved in the expected rejection.
 */
final class Main {
	/**
	 * Reaches the unsupported payload boundary during compilation.
	 *
	 * The switch also keeps the constructed enum live as a real program value,
	 * rather than leaving only an unused type declaration for the compiler.
	 */
	static function main():Void {
		final values:Map<String, Bool> = [];
		final result = UnsupportedValue(values);
		switch result {
			case UnsupportedValue(_):
		}
	}
}
