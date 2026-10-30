class Box<T> {
	public var value:T;

	public function new(value:T) {
		this.value = value;
	}
}

typedef Callback = Int->Int;

class Main {
	static function identity(value:Int):Int {
		return value;
	}

	static function main():Void {
		final box = new Box<Callback>(identity);
		if (box == null) {
			return;
		}
	}
}
