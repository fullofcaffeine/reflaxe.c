package;

/** A mutable record remains owned by the caller for the complete direct call. */
typedef MutableRecord = {
	var value:Int;
}

/** Tries to return the same identity after mutating it. */
class Main {
	/** This escape must fail before the compiler changes the helper's ABI. */
	static function changed(record:MutableRecord):MutableRecord {
		record.value++;
		return record;
	}

	/** Keep the escaping helper reachable by the custom target. */
	static function main():Void {
		var record:MutableRecord = {value: 1};
		while (changed(record).value != 2) {}
	}
}
