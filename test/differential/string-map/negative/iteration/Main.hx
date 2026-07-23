/** Keeps the unsupported-iteration diagnostic isolated from supported operations. */

/** Iteration remains a separate semantic slice with an explicit diagnostic. */
final class Main {
	static function main():Void {
		final values:Map<String, Bool> = [];
		values.set("ready", true);
		for (value in values)
			if (!value)
				return;
	}
}
