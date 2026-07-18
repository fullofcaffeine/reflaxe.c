class EscapingRecord {
	public function new() {}
}

class Main {
	static function make():EscapingRecord {
		var value = new EscapingRecord();
		return value;
	}

	static function main():Void {
		var value = make();
		while (value == null) {}
	}
}
