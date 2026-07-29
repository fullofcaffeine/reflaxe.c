import haxe.io.Bytes;

/**
 * Proves that ordinary Haxe can lend owned bytes to one synchronous C call.
 *
 * The program allocates and initializes the text in Haxe, the independent C
 * function edits those same bytes, and Haxe observes the mutation afterward.
 * An infinite loop makes any semantic mismatch visible as a native test timeout
 * without requiring an output or exception runtime.
 */
final class Main {
	static inline final CAPACITY:Int = 16;

	static function main():Void {
		final text = Bytes.alloc(CAPACITY);
		final initial = Bytes.ofString("Haxe");
		text.blit(0, initial, 0, initial.length);

		final changed = MutableTextApi.replace(c.CStringBufferRef.to(text), text.length);
		while (!changed || text.get(0) != "C".code || text.get(1) != "a".code || text.get(2) != "x".code || text.get(3) != "e".code
			|| text.get(4) != "!".code || text.get(5) != 0) {}
	}
}
