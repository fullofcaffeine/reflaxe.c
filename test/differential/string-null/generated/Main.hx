/**
	Defines a nominal String identity without changing its runtime value.

	The fixture keeps this abstract because nullable reference behavior must not
	depend on whether an application gives a String a more precise domain name.
**/
abstract Label(String) from String to String {}

/** A closed String-backed domain matching Caxecraft's action identifiers. */
enum abstract CommandId(String) from String {
	var Start = "start";
	var Stop = "stop";
}

/**
	Places a nominal String abstract inside a closed record.

	This is the exact shape that exposed the String-switch gap in Caxecraft:
	field projection must keep `CommandId`, then C lowering must compare String
	values because C's native `switch` statement accepts only integral values.
**/
typedef CommandDescriptor = {
	final id:CommandId;
}

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

	/** Select one record-carried command with an explicit unknown-value fallback. */
	static function commandValue(descriptor:CommandDescriptor):Int {
		return switch descriptor.id {
			case Start: 1;
			case Stop: 2;
			case _: -1;
		};
	}

	/** Exercise the same String dispatch when the switch controls statements. */
	static function commandStatement(descriptor:CommandDescriptor):Int {
		var result = 0;
		switch descriptor.id {
			case Start:
				result = 4;
			case Stop:
				result = 5;
			case _:
				result = 6;
		}
		return result;
	}

	/**
		Exercise Haxe's closed enum-abstract proof without an authored default.

		The generated C still contains a fail-stop path for a forged underlying
		String, but normal values need only the two declared text comparisons.
	**/
	static function exhaustiveCommandValue(value:CommandId):Int {
		return switch value {
			case Start: 10;
			case Stop: 20;
		};
	}

	/** Keep a nullable String-backed abstract distinct from every real text value. */
	static function nullableCommandValue(value:Null<CommandId>):Int {
		return switch value {
			case null: -1;
			case Start: 1;
			case Stop: 2;
			case _: 3;
		};
	}

	/** Prove the complete bounded contract without relying on pointer identity. */
	static function contractHolds():Bool {
		final missing:String = null;
		final empty:String = "";
		final alias = choose(false);
		final named:Label = "chosen";
		final missingLabel:Null<Label> = null;
		final unknown:CommandId = "unknown";
		return classify(choose(false)) == -1
			&& classify(choose(true)) == 1
			&& classify(missing) == -1
			&& classify(empty) == 0
			&& identity(alias) == null
			&& nullableIdentity(missing) == null
			&& missingLabel == null
			&& missing == null
			&& null == missing
			&& missing != empty
			&& empty != null
			&& choose(true) == named
			&& commandValue({
				id: Start
			}) == 1
			&& commandValue({id: Stop}) == 2
			&& commandValue({id: unknown}) == -1
			&& commandStatement({id: Start}) == 4
			&& commandStatement({id: Stop}) == 5
			&& commandStatement({id: unknown}) == 6
			&& exhaustiveCommandValue(Start) == 10
			&& exhaustiveCommandValue(Stop) == 20
			&& nullableCommandValue(null) == -1
			&& nullableCommandValue(Start) == 1
			&& nullableCommandValue(unknown) == 3;
	}

	/** Publish one deterministic line for Eval/native differential comparison. */
	static function main():Void {
		if (contractHolds())
			Sys.println("string-null: OK");
		else
			Sys.println("string-null: FAIL");
	}
}
