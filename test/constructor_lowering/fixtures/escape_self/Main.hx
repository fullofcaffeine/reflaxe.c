class SelfReferencingRecord {
	public var peer:SelfReferencingRecord;

	public function new() {
		peer = this;
	}
}

class Main {
	static function main():Void {
		var value = new SelfReferencingRecord();
		while (value == null) {}
	}
}
