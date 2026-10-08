class Box<T> {
	public var value:T;
}

typedef Callback = Int->Int;

class Main {
	static function isNull(value:Box<Callback>):Bool {
		return value == null;
	}

	static function main():Void {
		var value:Box<Callback> = null;
		while (!isNull(value)) {}
	}
}
