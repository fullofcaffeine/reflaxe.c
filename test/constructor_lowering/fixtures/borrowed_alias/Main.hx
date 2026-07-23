class BorrowedAliasValue {
	public var value:Int = 7;

	public function new() {}
}

class Main {
	static function alias(value:BorrowedAliasValue):Int {
		var second = value;
		return second.value;
	}

	static function main():Void {
		var value = new BorrowedAliasValue();
		while (alias(value) != 7) {}
	}
}
