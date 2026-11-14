package;

/**
 * Exercises one mutable anonymous-record identity through direct helper calls.
 * The caller and its sibling alias must observe each field update made by the
 * callee, while an immutable neighboring record keeps the ordinary value ABI.
 */
typedef MutableRecord = {
	var value:Int;
	var visits:Int;
}

/** A closed read-only record remains an ordinary by-value aggregate. */
typedef ImmutableRecord = {
	final value:Int;
}

/** Runs the Eval/native parity scenario for mutable record aliases. */
class Main {
	/** Apply assignment, compound assignment, prefix, and postfix mutation. */
	static function changed(record:MutableRecord):Int {
		record.value = 2;
		record.value += 3;
		final prefixed = ++record.value;
		final oldValue = record.value++;
		record.visits++;
		return prefixed * 100 + oldValue;
	}

	/** Forward the same borrow recursively without creating a record copy. */
	static function forward(record:MutableRecord, depth:Int):Int {
		if (depth == 0)
			return changed(record);
		return forward(record, depth - 1);
	}

	/** Preserve the same pointer while a later argument branches. */
	static function changeAfterFlow(record:MutableRecord, amount:Int):Void {
		record.value += amount;
	}

	/** Copy a read-only record by value as the neighboring ABI control. */
	static function immutableValue(record:ImmutableRecord):Int {
		return record.value;
	}

	/** Read the alias through one exact generic specialization without retaining it. */
	static function observe<T>(value:T, record:MutableRecord):T {
		while (record.visits != 1) {}
		return value;
	}

	/** Keep every observable assertion inside the generated executable. */
	static function main():Void {
		var record:MutableRecord = {value: 1, visits: 0};
		var sibling = record;
		final mutationCode = forward(sibling, 2);
		final immutable:ImmutableRecord = {value: 9};
		while (mutationCode != 606) {}
		while (record.value != 7) {}
		while (sibling.value != 7) {}
		while (record.visits != 1) {}
		while (sibling.visits != 1) {}
		while (immutableValue(immutable) != 9) {}
		while (observe(11, sibling) != 11) {}
		changeAfterFlow(sibling, if (immutableValue(immutable) == 9) 1 else 2);
		while (record.value != 8) {}
		while (sibling.value != 8) {}
	}
}
