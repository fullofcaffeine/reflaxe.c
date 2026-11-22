/**
	Provides the executable Haxe oracle for the first integer-keyed Map slice.

	The same ordinary Haxe source runs under Eval and through haxe.c. It keeps
	keys and values dynamic enough to exercise the table rather than letting the
	compiler replace the whole example with constants.
**/
final class Main {
	/** Build one shared table and prove that aliases observe the same mutations. */
	static function sharedMembership(seed:Int):Bool {
		final values:Map<Int, Bool> = [];
		final alias = values;
		values.set(seed, false);
		values.set(seed + 1, true);
		if (!alias.exists(seed) || !alias.exists(seed + 1) || alias.exists(seed + 2))
			return false;
		alias.set(seed + 2, true);
		return values.exists(seed + 2);
	}

	/** Distinguish a stored false value from absence across removal and clear. */
	static function lookupAndDeletion(seed:Int):Bool {
		final values:Map<Int, Bool> = [];
		final alias = values;
		values.set(seed, false);
		values.set(seed + 2, true);
		final storedFalse:Null<Bool> = alias.get(seed);
		final missing:Null<Bool> = alias.get(seed + 1);
		if (storedFalse != false || missing != null)
			return false;
		if (!alias.remove(seed) || alias.remove(seed))
			return false;
		final removed:Null<Bool> = alias.get(seed);
		final survivor:Null<Bool> = values.get(seed + 2);
		if (removed != null || survivor != true)
			return false;
		alias.clear();
		final cleared:Null<Bool> = values.get(seed + 2);
		return !values.exists(seed + 2) && cleared == null;
	}

	/** Copy entries into an independent table while preserving exact values. */
	static function independentCopy(seed:Int):Bool {
		final original:Map<Int, Bool> = [];
		original.set(seed, false);
		original.set(seed + 1, true);
		final copied = original.copy();
		copied.set(seed, true);
		copied.remove(seed + 1);
		return original.get(seed) == false
			&& original.get(seed + 1) == true
			&& copied.get(seed) == true
			&& copied.get(seed + 1) == null;
	}

	/** Prove that all three iterator forms own one creation-time snapshot. */
	static function iteratorTrace(seed:Int):Bool {
		final values:Map<Int, Bool> = [];
		values.set(seed, false);
		values.set(seed + 1, true);
		final valueCursor = values.iterator();
		final keyCursor = values.keys();
		final pairCursor = values.keyValueIterator();
		values.remove(seed);
		values.set(seed + 2, true);

		var falseCount = 0;
		var trueCount = 0;
		while (valueCursor.hasNext()) {
			if (valueCursor.next())
				trueCount++
			else
				falseCount++;
		}
		var keySum = 0;
		while (keyCursor.hasNext())
			keySum += keyCursor.next();
		var pairKeySum = 0;
		var pairTrueCount = 0;
		while (pairCursor.hasNext()) {
			final pair = pairCursor.next();
			pairKeySum += pair.key;
			if (pair.value)
				pairTrueCount++;
		}
		return falseCount == 1 && trueCount == 1 && keySum == seed * 2 + 1 && pairKeySum == keySum && pairTrueCount == 1;
	}

	/** Keep formatting deterministic without making multi-entry hash order public. */
	static function stringTrace(seed:Int):Bool {
		final values:Map<Int, Bool> = [];
		values.set(seed, false);
		final rendered = values.toString();
		return rendered == "[17 => false]";
	}

	/**
		Run the bounded semantic trace without requiring console or file support.

		A correct build returns normally. A mismatch stays in the final loop, so
		the test runner observes it as a bounded timeout on both Eval and native C.
	**/
	static function main():Void {
		if (!sharedMembership(40) || !lookupAndDeletion(40) || !independentCopy(60) || !iteratorTrace(80) || !stringTrace(17))
			while (true) {}
	}
}
