class AliasedRecord {
	public function new() {}
}

class Main {
	static var escaped:AliasedRecord;

	static function main():Void {
		var first = new AliasedRecord();
		var second = first;
		escaped = second;
	}
}
