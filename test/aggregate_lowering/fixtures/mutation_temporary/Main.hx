package;

/** A mutable record needs caller-owned local storage before it can be lent. */
typedef MutableRecord = {
	var value:Int;
}

/** Tries to lend a temporary object literal that has no stable local identity. */
class Main {
	/** Mutate one field through the direct helper's borrow contract. */
	static function changed(record:MutableRecord):Void {
		record.value++;
	}

	/** Keep the unsupported temporary call reachable by the custom target. */
	static function main():Void {
		changed({value: 1});
	}
}
