class AliasedRecord {
	public function new() {}
}

class Main {
	static function main():Void {
		var first = new AliasedRecord();
		var second = first;
		while (second == null) {}
	}
}
