/**
	Provides an executable semantic oracle for the first String-keyed Map slice.

	The same ordinary Haxe source runs under Eval and through haxe.c, allowing the
	runner to compare behavior without teaching the fixture about generated C.
**/

/**
	Exercises the first ordinary-Haxe `Map<String, Bool>` compiler slice.

	The program keeps every operation observable through control flow so haxe.c
	cannot replace the map with compile-time constants. It deliberately tests
	shared identity, because assigning a Haxe Map copies its reference rather
	than cloning its entries. Missing and stored `false` values are checked
	separately through `exists` and nullable `get`.
**/
final class Main {
	/**
		Create and return one mutable map.

		Returning the value proves that construction ownership can move across a
		function boundary without cloning or prematurely destroying the table.
	**/
	static function makeMap():Map<String, Bool> {
		final result:Map<String, Bool> = [];
		result.set("alpha", false);
		result.set("beta", true);
		return result;
	}

	/** Preserve alias identity across an ordinary parameter and return value. */
	static function identity(values:Map<String, Bool>):Map<String, Bool>
		return values;

	/** Mutate the caller's table through a borrowed parameter. */
	static function replace(values:Map<String, Bool>, key:String, value:Bool):Void
		values.set(key, value);

	/** Keep nullable lookup explicit so missing remains distinct from `false`. */
	static function lookup(values:Map<String, Bool>, key:String):Null<Bool>
		return values.get(key);

	/**
		Return the absent value of the same nullable pointer carrier.

		The caller owns and later cleans up the result just like a present map;
		retain/release must therefore accept the absent `NULL` carrier as a no-op.
	**/
	static function absentMap():Null<Map<String, Bool>>
		return null;

	/**
		Run the complete bounded semantic trace.

		The final empty loop is a portable, output-free assertion: correct Eval
		and generated-C executions terminate, while any semantic mismatch remains
		observable to the test runner as a timeout.
	**/
	static function main():Void {
		final values = makeMap();
		final alias = values;
		final returned = identity(values);
		final absent = absentMap();
		final alphaBefore = lookup(values, "alpha");
		final missingBefore = lookup(values, "missing");

		replace(alias, "alpha", true);
		final removedBeta = returned.remove("beta");
		final removedBetaAgain = returned.remove("beta");
		values.set("gamma", false);
		values.set("", true);
		final gammaBeforeClear = alias.exists("gamma");
		final emptyBeforeClear = alias.exists("");
		alias.clear();

		while (alias != values || absent != null || null != absent || values == null || alphaBefore == null || alphaBefore || missingBefore != null
			|| !removedBeta || removedBetaAgain || !gammaBeforeClear || !emptyBeforeClear || values.exists("alpha") || values.exists("gamma")
			|| values.exists("") || values.get("alpha") != null) {}
	}
}
