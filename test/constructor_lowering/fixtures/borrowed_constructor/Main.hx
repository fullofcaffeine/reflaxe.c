class BorrowedConstructorValue {
	public var value:Int = 7;

	public function new() {}
}

class BorrowedConstructorObserver {
	public var observed:Int;

	public function new(value:BorrowedConstructorValue) {
		observed = value.value;
	}
}

class Main {
	static function observe(value:BorrowedConstructorValue):Int {
		var observer = new BorrowedConstructorObserver(value);
		return observer.observed;
	}

	static function main():Void {
		var value = new BorrowedConstructorValue();
		while (observe(value) != 7) {}
	}
}
