/** Keeps the unsupported-value diagnostic isolated from the positive Map fixture. */

/** Unsupported value specialization must fail before any C artifact is saved. */
final class Main {
	static function main():Void {
		final values:Map<String, Int> = [];
		values.set("answer", 42);
	}
}
