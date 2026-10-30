/**
	Focused executable for the bounded `Array<String>.toString` contract.

	The values cover empty and singleton arrays, shared Array identity, Unicode,
	embedded NUL bytes, and runtime-created managed Strings. A silent exit means
	the generated program matches the pinned Eval oracle.
**/
final class Main {
	/** Create a runtime String so the test cannot collapse to literals. */
	static function fromCode(code:Int):String
		return String.fromCharCode(code);

	static function main():Void {
		final labels = ["ready", fromCode(0xE9), "a\u0000b"];
		final alias = labels;
		final empty:Array<String> = [];
		final singleton = [fromCode(0x1F642)];
		while (alias.toString() != "[ready,é,a\u0000b]" || empty.toString() != "[]" || singleton.toString() != "[🙂]") {}
	}
}
