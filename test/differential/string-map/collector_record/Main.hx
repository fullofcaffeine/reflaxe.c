/**
	Proves that a StringMap keeps collector-managed references in exact record slots alive.

	The recursive rule makes its child Array collector-owned. The map then stores
	that Array inside a closed record, matching the generic shape used by content
	indexes without depending on any application names or schemas.
**/

private enum Rule {
	Stop;
	Nested(children:Array<Rule>);
}

private typedef StoredRecord = {
	final revision:Int;
	final rules:Array<Rule>;
}

final class Main {
	/** Allocate unrelated recursive graphs to force exact map-slot tracing. */
	static function applyPressure():Int {
		var total = 0;
		for (index in 0...40000) {
			final children = [Stop];
			final value = Nested(children);
			if (index % 997 == 0)
				total += switch value {
					case Nested(items): items.length;
					case Stop: 0;
				};
		}
		return total;
	}

	/** Exercise shared identity, copy, replacement, removal, and clear. */
	static function trace():Bool {
		final values:Map<String, StoredRecord> = [];
		final firstRules = [Nested([Stop])];
		values.set("active", {revision: 1, rules: firstRules});
		final alias = values;
		if (applyPressure() <= 0)
			return false;
		final first = alias.get("active");
		if (first == null || first.revision != 1 || first.rules.length != 1)
			return false;

		final copied = values.copy();
		values.set("active", {revision: 2, rules: [Stop, Nested([Stop])]});
		if (applyPressure() <= 0)
			return false;
		final original = values.get("active");
		final snapshot = copied.get("active");
		if (original == null || original.revision != 2 || original.rules.length != 2)
			return false;
		if (snapshot == null || snapshot.revision != 1 || snapshot.rules.length != 1)
			return false;
		if (!copied.remove("active") || copied.get("active") != null)
			return false;
		values.clear();
		return values.get("active") == null && alias == values;
	}

	/** Fail without product-specific output when Eval and generated behavior differ. */
	static function main():Void {
		if (!trace())
			throw "collector-bearing StringMap record contract failed";
	}
}
