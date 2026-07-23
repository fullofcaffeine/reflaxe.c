class BorrowedForwardValue {
	public var value:Int = 7;

	public function new() {}
}

class BorrowedForwardSink {
	public function new() {}

	public function consume(value:BorrowedForwardValue):Int {
		return value.value;
	}
}

class Main {
	static function forward(value:BorrowedForwardValue, sink:BorrowedForwardSink):Int {
		return sink.consume(value);
	}

	static function main():Void {
		var value = new BorrowedForwardValue();
		var sink = new BorrowedForwardSink();
		while (forward(value, sink) != 7) {}
	}
}
