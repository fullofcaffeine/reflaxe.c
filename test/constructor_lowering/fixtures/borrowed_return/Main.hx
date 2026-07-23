class BorrowedReturnValue {
	public function new() {}
}

class Main {
	static function expose(value:BorrowedReturnValue):BorrowedReturnValue {
		return value;
	}

	static function main():Void {
		var value = new BorrowedReturnValue();
		while (expose(value) == null) {}
	}
}
