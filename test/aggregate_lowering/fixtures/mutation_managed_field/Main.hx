package;

/** Replacing a collector-reachable field needs a root-refresh protocol. */
typedef MutableRecord = {
	var values:Array<Int>;
}

/** Tries to replace a managed field through a call-bounded record borrow. */
class Main {
	/** This replacement remains unsupported until lifecycle planning owns it. */
	static function changed(record:MutableRecord):Void {
		record.values = [];
	}

	/** Keep the unsupported managed-field replacement reachable. */
	static function main():Void {
		var record:MutableRecord = {values: [1]};
		changed(record);
	}
}
