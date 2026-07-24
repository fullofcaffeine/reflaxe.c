/**
	Exercises runtime-created ordinary Haxe Strings without target-specific code.

	The fixture deliberately uses the upstream `StringBuf` implementation rather
	than a compiler-only stand-in. Its inline methods expand to
	`String.fromCharCode`, concatenation, reassignment, and return, so one small
	program checks the same natural Haxe path that Caxecraft's UTF-8 lexer uses.
**/
final class Main {
	/** Keep direct `String.fromCharCode` observable across a normal Haxe call. */
	static function fromCode(code:Int):String
		return String.fromCharCode(code);

	/** Build one mixed ASCII/Unicode value through the upstream StringBuf API. */
	static function build(accent:Int, emoji:Int):String {
		final output = new StringBuf();
		output.addChar(65);
		output.addChar(accent);
		output.addChar(emoji);
		return output.toString();
	}

	/** Search without a start argument so omission remains visible to lowering. */
	static function find(value:String, needle:String):Int
		return value.indexOf(needle);

	/** Search from one runtime-supplied scalar position. */
	static function findFrom(value:String, needle:String, start:Int):Int
		return value.indexOf(needle, start);

	/**
		Exercise aliases, branches, records, enums, arrays, and borrowed views.

		Each aggregate stores a logical String copy. Generated C may copy the small
		carrier, but every copy that outlives its source expression must keep the
		shared UTF-8 allocation alive until its own cleanup.
	**/
	static function contractHolds(enabled:Bool):Bool {
		final built = build(0xE9, 0x1F600);
		final direct = fromCode(0xE9) + fromCode(0x1F600);
		final alias = built;
		final selected = enabled ? alias : "unused";
		final record = {left: selected, right: selected.substring(1)};
		final choice:TextChoice = Text(record.right);
		final values = [record.left, record.right, selected.charAt(2)];
		var reassigned = "old";
		reassigned = values[0];

		var payload = "";
		switch choice {
			case Text(value):
				payload = value;
			case Empty:
		}
		final optional:Null<String> = enabled ? payload : null;
		return built == "Aé😀" && alias.length == 3 && direct == "é😀" && record.right == "é😀" && selected.substring(2, 1) == "é"
			&& selected.substring(-3, 1) == "A" && selected.substring(99) == "" && selected.charCodeAt(2) == 0x1F600 && values[2] == "😀"
			&& reassigned == built && optional == "é😀" && find(built, "é") == 1 && find(built, fromCode(0x1F600)) == 2 && find(built, "é😀") == 1
			&& find(built, "missing") == -1 && findFrom(built, "", 99) == 3 && findFrom(built, "", -1) == 0 && findFrom(built, "é", -2) == 1
			&& findFrom(built, "A", -99) == 0 && findFrom(built, "😀", -1) == 2 && findFrom(built, "é", 2) == -1 && find("A\x00🙂", "\x00") == 1
			&& find("A\x00🙂", "🙂") == 2 && find("caxecraft:grass", ":") == 9;
	}

	/** Publish one deterministic literal result for Eval/native comparison. */
	static function main():Void {
		if (contractHolds(true))
			Sys.println("string-owned: OK");
		else
			Sys.println("string-owned: FAIL");
	}
}

/**
	A closed enum makes String payload copying and destruction explicit.

	`Empty` keeps the switch exhaustive without introducing a null sentinel into
	the payload representation.
**/
enum TextChoice {
	Text(value:String);
	Empty;
}
