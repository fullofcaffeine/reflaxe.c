/** Keeps unsupported IntMap value storage separate from the positive fixture. */

/** An Int value must not silently enter the first Bool-only specialization. */
final class Main {
	static function main():Void {
		final values:Map<Int, Int> = [];
		values.set(7, 11);
	}
}
