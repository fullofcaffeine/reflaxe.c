/**
	Exercises the first ordinary-Haxe String method admitted by haxe.c.

	The cases make `charAt` observable through one final literal message while
	keeping the source natural Haxe. Eval and generated C must agree on ASCII,
	non-BMP Unicode, embedded NUL, and every empty-result boundary.
**/
class Main {
	/** Return one character through a non-inlined call so the index stays dynamic. */
	static function characterAt(value:String, index:Int):String {
		return value.charAt(index);
	}

	/** Check the exact scalar-indexing and total out-of-range contract. */
	static function contractHolds():Bool {
		final embeddedNul = "A\x00🙂";
		return characterAt("ASCII", 2) == "C"
			&& characterAt("A🙂Z", 1) == "🙂"
			&& characterAt(embeddedNul, 1) == "\x00"
			&& characterAt("", 0) == ""
			&& characterAt("abc", -1) == ""
			&& characterAt("abc", 3) == "";
	}

	/** Publish one deterministic result for Eval/native differential execution. */
	static function main():Void {
		if (contractHolds())
			Sys.println("string-char-at: OK");
		else
			Sys.println("string-char-at: FAIL");
	}
}
