class Box<T> {
	public var value:T;

	public function new(value:T) {
		this.value = value;
	}
}

class Main {
	static function main():Void {
		final box = new Box<Int>(3);
		if (box == null) {
			return;
		}
	}
}
