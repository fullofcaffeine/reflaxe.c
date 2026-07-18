class ConditionalRecord {
	public function new() {}
}

class Main {
	static function guard():Bool {
		return true;
	}

	static function main():Void {
		if (guard()) {
			var value = new ConditionalRecord();
			while (value == null) {}
		}
	}
}
