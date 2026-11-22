/** One unchecked incoming arm suffices to invalidate the implication at a join. */
final class UncheckedJoin {
	/** Overwriting the decision can admit a value that failed the presence check. */
	static function read(selected:Null<Reading>, overrideDecision:Bool):Int {
		var visible = selected != null && selected.enabled(true);
		if (overrideDecision)
			visible = true;
		return visible ? selected.value : -1;
	}

	/** Select the unchecked arm with an absent reading. */
	static function main():Void {
		Sys.println(read(null, true));
	}
}
