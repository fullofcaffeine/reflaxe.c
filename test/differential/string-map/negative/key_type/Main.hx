/** Keeps the unsupported-key diagnostic isolated from the positive Map fixture. */

/** Another key family must not be mistaken for the String-keyed runtime. */
final class Main {
	static function main():Void {
		final values:Map<Int, Bool> = [];
		values.set(1, true);
	}
}
