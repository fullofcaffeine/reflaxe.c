class Item {
	public var value:Int;

	public function new(value:Int) {
		this.value = value;
	}
}

class Main {
	static function main():Void {
		var value = new Item(7);
		while (value == null) {}
	}
}
