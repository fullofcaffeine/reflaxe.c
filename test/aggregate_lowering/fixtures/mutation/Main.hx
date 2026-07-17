package;

typedef MutableRecord = {
	var value:Int;
}

class Main {
	static function changed(record:MutableRecord):Int {
		record.value = 2;
		return record.value;
	}

	static function main():Void {
		var record:MutableRecord = {value: 1};
		while (changed(record) != 2) {}
	}
}
