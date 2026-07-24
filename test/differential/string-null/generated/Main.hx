/**
	Defines a nominal String identity without changing its runtime value.

	The fixture keeps this abstract because nullable reference behavior must not
	depend on whether an application gives a String a more precise domain name.
**/
abstract Label(String) from String to String {}

/**
	Exercises Haxe's default, legacy-nullable String contract.

	With null safety disabled (Haxe's default), both `String` and `Null<String>`
	use the same nullable reference value. The native result must therefore keep
	`null` distinct from the real empty string while preserving ordinary
	content equality through calls, aliases, abstracts, and control flow.
**/
class Main {
	/** Classify a String after it crosses an ordinary non-inlined call boundary. */
	static function classify(value:String):Int {
		if (value == null)
			return -1;
		return value == "" ? 0 : 1;
	}

	/** Return either a literal or null through Haxe's documentary `Null` spelling. */
	static function choose(present:Bool):Null<String> {
		if (present)
			return "chosen";
		return null;
	}

	/** Preserve the nullable carrier through an ordinary String parameter and result. */
	static function identity(value:String):String {
		return value;
	}

	/** Prove that the explicit `Null` spelling keeps the same call carrier. */
	static function nullableIdentity(value:Null<String>):Null<String> {
		return value;
	}

	/** Prove the complete bounded contract without relying on pointer identity. */
	static function contractHolds():Bool {
		final missing:String = null;
		final empty:String = "";
		final alias = choose(false);
		final named:Label = "chosen";
		final missingLabel:Null<Label> = null;
		return classify(choose(false)) == -1 && classify(choose(true)) == 1 && classify(missing) == -1 && classify(empty) == 0 && identity(alias) == null
			&& nullableIdentity(missing) == null && missingLabel == null && missing == null && null == missing && missing != empty && empty != null
			&& choose(true) == named;
	}

	/** Publish one deterministic line for Eval/native differential comparison. */
	static function main():Void {
		if (contractHolds())
			Sys.println("string-null: OK");
		else
			Sys.println("string-null: FAIL");
	}
}
