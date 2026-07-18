class PassedRecord {
	public function new() {}
}

class Main {
	static function present(value:PassedRecord):Bool {
		return value != null;
	}

	static function main():Void {
		var value = new PassedRecord();
		while (!present(value)) {}
	}
}
