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

	/** Return a value snapshot after its collector-owned producer leaves scope. */
	static function valueSnapshot():Iterator<StoredRecord> {
		final values:Map<String, StoredRecord> = [];
		values.set("first", {revision: 3, rules: [Nested([Stop])]});
		values.set("second", {revision: 5, rules: [Stop, Nested([Stop])]});
		final iterator = values.iterator();
		values.set("later", {revision: 9, rules: [Stop]});
		return iterator;
	}

	/** Return copied keys while the collector keeps the producer anchor exact. */
	static function keySnapshot():Iterator<String> {
		final values:Map<String, StoredRecord> = [];
		values.set("first", {revision: 3, rules: [Stop]});
		values.set("second", {revision: 5, rules: [Stop]});
		final iterator = values.keys();
		values.set("later", {revision: 9, rules: [Stop]});
		return iterator;
	}

	/** Return key/value snapshots with exact roots for each copied record. */
	static function pairSnapshot():KeyValueIterator<String, StoredRecord> {
		final values:Map<String, StoredRecord> = [];
		values.set("first", {revision: 3, rules: [Nested([Stop])]});
		values.set("second", {revision: 5, rules: [Stop, Nested([Stop])]});
		final iterator = values.keyValueIterator();
		values.set("later", {revision: 9, rules: [Stop]});
		return iterator;
	}

	/** Exercise aliases, pressure, and exhaustion for all three snapshots. */
	static function iteratorTrace():Bool {
		final values = valueSnapshot();
		final valueAlias = values;
		final keys = keySnapshot();
		final pairs = pairSnapshot();
		if (applyPressure() <= 0)
			return false;
		var revisions = 0;
		var ruleCount = 0;
		while (valueAlias.hasNext()) {
			final value = valueAlias.next();
			revisions += value.revision;
			ruleCount += value.rules.length;
		}
		var keyBytes = 0;
		while (keys.hasNext())
			keyBytes += keys.next().length;
		var pairRevisions = 0;
		var pairRuleCount = 0;
		var pairKeyBytes = 0;
		while (pairs.hasNext()) {
			final pair = pairs.next();
			pairKeyBytes += pair.key.length;
			pairRevisions += pair.value.revision;
			pairRuleCount += pair.value.rules.length;
		}
		return revisions == 8 && ruleCount == 3 && keyBytes == 11 && pairKeyBytes == 11 && pairRevisions == 8 && pairRuleCount == 3 && !values.hasNext()
			&& !valueAlias.hasNext() && !keys.hasNext() && !pairs.hasNext();
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
		if (!trace() || !iteratorTrace())
			throw "collector-bearing StringMap record contract failed";
	}
}
