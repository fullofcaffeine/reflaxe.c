extern class NativeRecord {
	public function new(value:Int);
}

class Main {
	static function main():Void {
		var value = new NativeRecord(1);
		while (value == null) {}
	}
}
