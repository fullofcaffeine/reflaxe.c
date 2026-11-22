/**
 * Provides the focused ordinary-Haxe oracle for typed-map text formatting.
 *
 * The fixture keeps one exact final object-key class, one fieldless enum, and
 * admitted Int and Bool values. It checks each bounded spelling without pulling
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

		final enumEmpty:Map<FormatKey, Int> = [];
		if (enumEmpty.toString() != "[]")
			while (true) {}

		final enumValues:Map<FormatKey, Int> = [];
		enumValues.set(Second, 2);
		enumValues.set(First, 1);
		if (enumValues.toString() != "[First => 1, Second => 2]")
			while (true) {}
	}
}

/** Closed fieldless keys whose constructor order defines tree traversal. */
enum FormatKey {
	First;
	Second;
}

/** One exact identity-bearing key that keeps Haxe's default class-name text. */
final class FormatNode {
	/** Create an otherwise empty identity object. */
	public function new() {}
}
