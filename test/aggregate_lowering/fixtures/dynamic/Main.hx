package;

class Main {
	static function value(record:Dynamic):Int {
		return record.value;
	}

	static function main():Void {
		while (value({value: 1}) != 1) {}
	}
}
