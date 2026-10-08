/** Proves the smallest Date identity and timestamp round trip. */
class Main {
	/** Keep the tracer observable without selecting local calendar services. */
	static function main():Void {
		final first = Date.fromTime(999.75);
		final alias = first;
		final equalValue = Date.fromTime(999.75);
		if (first.getTime() != 999.75)
			throw "Date timestamp did not round trip";
		if (first != alias)
			throw "Date alias lost object identity";
		if (first == equalValue)
			throw "separate Date values shared object identity";
		Sys.println("date-time-pure-ok");
	}
}
