class ArgumentBase {}
class ArgumentLeaf extends ArgumentBase {}

class ConsumerBase {
	public function new() {}

	public function accept(value:ArgumentLeaf):Int {
		return value == null ? 1 : 2;
	}
}

class ConsumerLeaf extends ConsumerBase {
	public function new() {
		super();
	}

	public override function accept(value:ArgumentBase):Int {
		return value == null ? 3 : 4;
	}
}

class Main {
	static function main():Void {
		var consumer:ConsumerBase = new ConsumerLeaf();
		var result = consumer.accept(null);
		while (result != 3) {}
	}
}
