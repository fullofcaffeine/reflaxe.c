package;

/**
 * Proves that source-ordered Array elements survive later control-flow joins.
 *
 * Each builder evaluates an earlier value before a later element branches.
 * The generated program checks order and value semantics for primitive,
 * managed String, managed enum, and managed record element representations.
 */
enum ManagedChoice {
	Empty;
	Named(value:String);
}

/** A direct record whose String field gives it a managed lifetime. */
typedef ManagedRecord = {
	final name:String;
	final count:Int;
}

/** Runs the Array-literal flow-staging contract as one generated executable. */
class Main {
	/** Keep a straight-line literal as the no-staging representation control. */
	static function straightLine():Array<Int>
		return [1, 2, 3];

	/** Preserve the first primitive while the second element uses an if. */
	static function primitiveValues(select:Bool):Array<Int> {
		var order = 0;
		final values:Array<Int> = [
			{
				order = order * 10 + 1;
				10;
			},
			{
				order = order * 10 + 2;
				select ? 20 : 21;
			}
		];
		while (order != 12) {}
		return values;
	}

	/** Preserve one runtime-created String while the next element switches. */
	static function stringValues(selector:Int, code:Int):Array<String> {
		var order = 0;
		final values:Array<String> = [
			{
				order = order * 10 + 1;
				String.fromCharCode(code);
			},
			{
				order = order * 10 + 2;
				switch selector {
					case 0: "B";
					case _: "C";
				};
			}
		];
		while (order != 12) {}
		return values;
	}

	/** Preserve one managed enum while the next element selects a branch. */
	static function choiceValues(select:Bool, code:Int):Array<ManagedChoice> {
		var order = 0;
		final values:Array<ManagedChoice> = [
			{
				order = order * 10 + 1;
				Named(String.fromCharCode(code));
			},
			{
				order = order * 10 + 2;
				select ? Named(String.fromCharCode(69)) : Empty;
			}
		];
		while (order != 12) {}
		return values;
	}

	/**
	 * Preserve one managed record while short-circuit flow chooses the next.
	 *
	 * The right side counter also proves that && does not evaluate its right
	 * expression when the left side is false.
	 */
	static function recordValues(select:Bool, code:Int):Array<ManagedRecord> {
		var order = 0;
		var rightCount = 0;
		final values:Array<ManagedRecord> = [
			{
				order = order * 10 + 1;
				{name: String.fromCharCode(code), count: 1};
			},
			{
				order = order * 10 + 2;
				final selected = select && {
					rightCount++;
					true;
				};
				final name = selected ? "G" : "H";
				final count = selected ? 2 : 3;
				{name: name, count: count};
			}
		];
		while (order != 12 || rightCount != (select ? 1 : 0)) {}
		return values;
	}

	/** Read one managed enum without creating another owner. */
	static function choiceCode(value:ManagedChoice):Int
		return switch value {
			case Empty: 0;
			case Named(text): text.length;
		};

	/** Keep every semantic assertion inside both Eval and generated C runs. */
	static function main():Void {
		final direct = straightLine();
		final primitiveTrue = primitiveValues(true);
		final primitiveFalse = primitiveValues(false);
		final strings = stringValues(0, 65);
		final choicesTrue = choiceValues(true, 68);
		final choicesFalse = choiceValues(false, 68);
		final recordsTrue = recordValues(true, 70);
		final recordsFalse = recordValues(false, 70);
		while (direct[0] != 1 || direct[2] != 3 || primitiveTrue[0] != 10 || primitiveTrue[1] != 20 || primitiveFalse[1] != 21 || strings[0].length != 1
			|| strings[1].length != 1 || choiceCode(choicesTrue[0]) != 1 || choiceCode(choicesTrue[1]) != 1 || choiceCode(choicesFalse[1]) != 0
			|| recordsTrue[0].name.length != 1 || recordsTrue[1].count != 2 || recordsFalse[1].count != 3) {}
	}
}
