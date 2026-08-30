/**
 * Provides the focused ordinary-Haxe oracle for ObjectMap text formatting.
 *
 * The fixture keeps one exact final object-key class and admitted Int and Bool
 * values. It checks empty, single, multiple, and null-key output without pulling
 * the exhaustive identity-map and enum-map runtime matrix into this fast lane.
 */
final class Main {
	/** Check Eval-compatible punctuation and exact admitted key/value spellings. */
	static function main():Void {
		final empty:Map<FormatNode, Int> = [];
		if (empty.toString() != "[]")
			while (true) {}

		final single:Map<FormatNode, Int> = [];
		final singleKey = new FormatNode();
		single.set(singleKey, 9);
		if (single.toString() != "[FormatNode => 9]")
			while (true) {}

		final booleans:Map<FormatNode, Bool> = [];
		final booleanKey = new FormatNode();
		booleans.set(booleanKey, true);
		if (booleans.toString() != "[FormatNode => true]")
			while (true) {}

		final several:Map<FormatNode, Int> = [];
		final firstKey = new FormatNode();
		final secondKey = new FormatNode();
		several.set(firstKey, 1);
		several.set(secondKey, 2);
		final severalText = several.toString();
		if (severalText != "[FormatNode => 1, FormatNode => 2]" && severalText != "[FormatNode => 2, FormatNode => 1]")
			while (true) {}

		final nullable:Map<FormatNode, Int> = [];
		final absent:FormatNode = null;
		nullable.set(absent, 10);
		if (nullable.toString() != "[null => 10]")
			while (true) {}
	}
}

/** One exact identity-bearing key that keeps Haxe's default class-name text. */
final class FormatNode {
	/** Create an otherwise empty identity object. */
	public function new() {}
}
