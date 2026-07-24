/** Keeps unsupported IntMap lookup separate from admitted membership testing. */

/** `get` needs a nullable result plan and remains outside the first slice. */
final class Main {
	static function main():Void {
		final values:Map<Int, Bool> = [];
		values.set(7, true);
		final value = values.get(7);
		if (value == null)
			return;
	}
}
