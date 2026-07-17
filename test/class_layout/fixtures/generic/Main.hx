class Box<T> {
	public var value:T;
}

class Main {
	static function isNull(value:Box<Int>):Bool {
		return value == null;
	}

	static function main():Void {
		var value:Box<Int> = null;
		while (!isNull(value)) {}
	}
}
