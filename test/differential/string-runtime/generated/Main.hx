/**
	Exercises runtime-created ordinary Haxe Strings without target-specific code.

	The fixture deliberately uses the upstream `StringBuf` implementation rather
	than a compiler-only stand-in. Its inline methods expand to
	`String.fromCharCode`, concatenation, reassignment, and return, so one small
	program checks the same natural Haxe path that Caxecraft's UTF-8 lexer uses.
**/
final class Main {
	/** Counts source evaluations so conversion cannot accidentally duplicate work. */
	static var boolEvaluations:Int = 0;
	static var intEvaluations:Int = 0;

	/** Return one observable Boolean while recording that its source ran once. */
	static function observedBool(value:Bool):Bool {
		boolEvaluations += 1;
		return value;
	}

	/** Return one observable Int while recording that its source ran once. */
	static function observedInt(value:Int):Int {
		intEvaluations += 1;
		return value;
	}

	/** Keep an Int parameter and converted String return visible across a call. */
	static function renderInt(value:Int):String
		return Std.string(value);

	/** Keep a Boolean parameter and converted String return visible across a call. */
	static function renderBool(value:Bool):String
		return Std.string(value);

	/**
		Exercise the statically typed Boolean slice of Haxe's general conversion.

		`Std.string` accepts `Dynamic` at its public boundary, but Haxe's typed
		expression still records that these arguments are Boolean. haxe.c uses
		that fact to select the immutable Haxe spellings without boxing or a
		generic reflection helper.
	**/
	static function boolStringContractHolds():Bool {
		boolEvaluations = 0;
		final direct = Std.string(observedBool(true));
		final interpolated = 'flag=${observedBool(false)}';
		final selected = observedBool(true) ? renderBool(false) : renderBool(true);
		return direct == "true" && interpolated == "flag=false" && selected == "false" && boolEvaluations == 3;
	}

	/**
		Exercise exact signed decimal spelling and implicit interpolation.

		The minimum value is especially important: directly negating it would
		overflow a signed 32-bit C integer, so the runtime must compute its
		magnitude without invoking undefined behavior.
	**/
	static function intStringContractHolds():Bool {
		intEvaluations = 0;
		final direct = Std.string(observedInt(0));
		final positive = renderInt(2147483647);
		final negative = renderInt(-2147483647 - 1);
		final interpolated = 'value=${observedInt(-42)}';
		final selected = observedInt(7) > 0 ? renderInt(19) : renderInt(20);
		return direct == "0"
			&& positive == "2147483647"
			&& negative == "-2147483648"
			&& interpolated == "value=-42"
			&& selected == "19"
			&& intEvaluations == 3;
	}

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

	/** Search backward without a start so omission reaches typed lowering. */
	static function findLast(value:String, needle:String):Int
		return value.lastIndexOf(needle);

	/** Search backward from the latest allowed runtime-supplied position. */
	static function findLastFrom(value:String, needle:String, start:Int):Int
		return value.lastIndexOf(needle, start);

	/**
		Split runtime-created text and then replace the source alias.

		The returned parts must keep the original shared bytes alive themselves;
		the test would expose a dangling borrowed slice if Array<String> copied only
		the small view and forgot to retain its optional owner.
	**/
	static function splitContractHolds():Bool {
		var owned = build(0xE9, 0x1F600);
		final aroundAccent = owned.split("é");
		final scalars = owned.split("");
		owned = "replaced";
		final adjacent = "::A::::😀::".split("::");
		final embedded = "A\x00🙂\x00".split("\x00");
		return aroundAccent.length == 2
			&& aroundAccent[0] == "A"
			&& aroundAccent[1] == "😀"
			&& scalars.length == 3
			&& scalars[0] == "A"
			&& scalars[1] == "é"
			&& scalars[2] == "😀"
			&& adjacent.length == 5
			&& adjacent[0] == ""
			&& adjacent[1] == "A"
			&& adjacent[2] == ""
			&& adjacent[3] == "😀"
			&& adjacent[4] == ""
			&& embedded.length == 3
			&& embedded[0] == "A"
			&& embedded[1] == "🙂"
			&& embedded[2] == ""
			&& "".split("").length == 0
			&& "".split(":").length == 1
			&& "".split(":")[0] == ""
			&& "caxecraft/scenario/token".split("/").length == 3;
	}

	/**
		Exercise aliases, branches, records, enums, arrays, and borrowed views.

		Each aggregate stores a logical String copy. Generated C may copy the small
		carrier, but every copy that outlives its source expression must keep the
		shared UTF-8 allocation alive until its own cleanup.
	**/
	static function contractHolds(enabled:Bool):Bool {
		final built = build(0xE9, 0x1F600);
		final direct = fromCode(0xE9) + fromCode(0x1F600);
		final repeated = built + built;
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
		return built == "Aé😀"
			&& boolStringContractHolds()
			&& intStringContractHolds()
			&& splitContractHolds()
			&& alias.length == 3
			&& direct == "é😀"
			&& record.right == "é😀"
			&& selected.substring(2, 1) == "é"
			&& selected.substring(-3, 1) == "A"
			&& selected.substring(99) == ""
			&& selected.charCodeAt(2) == 0x1F600
			&& values[2] == "😀"
			&& reassigned == built
			&& optional == "é😀"
			&& find(built, "é") == 1
			&& find(built, fromCode(0x1F600)) == 2
			&& find(built, "é😀") == 1
			&& find(built, "missing") == -1
			&& findFrom(built, "", 99) == 3
			&& findFrom(built, "", -1) == 0
			&& findFrom(built, "é", -2) == 1
			&& findFrom(built, "A", -99) == 0
			&& findFrom(built, "😀", -1) == 2
			&& findFrom(built, "é", 2) == -1
			&& find("A\x00🙂", "\x00") == 1
			&& find("A\x00🙂", "🙂") == 2
			&& find("caxecraft:grass", ":") == 9
			&& findLast(repeated, "A") == 3
			&& findLast(repeated, fromCode(0x1F600)) == 5
			&& findLast(repeated, "Aé") == 3
			&& findLastFrom(repeated, "Aé", 2) == 0
			&& findLastFrom(repeated, "Aé", 99) == 3
			&& findLastFrom(repeated, "Aé", -1) == -1
			&& findLast(repeated, "") == 6
			&& findLastFrom(repeated, "", 99) == 6
			&& findLastFrom(repeated, "", -1) == 0
			&& findLast(repeated, "Aé😀Aé😀A") == -1
			&& findLast("ababa", "aba") == 2
			&& findLastFrom("ababa", "aba", 1) == 0
			&& findLastFrom("ababa", "aba", 2) == 2
			&& findLast("A\x00🙂\x00", "\x00") == 3
			&& findLast("ée\u0301é", "e\u0301") == 1
			&& findLast("ée\u0301é", "é") == 3
			&& findLast("caxecraft:grass", ":") == 9;
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
