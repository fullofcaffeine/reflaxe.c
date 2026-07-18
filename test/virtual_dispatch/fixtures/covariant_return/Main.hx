class ReturnBase {
	public function new() {}

	public function duplicate():ReturnBase {
		return this;
	}
}

class ReturnLeaf extends ReturnBase {
	public function new() {
		super();
	}

	public override function duplicate():ReturnLeaf {
		return this;
	}
}

class Main {
	static function main():Void {
		var value:ReturnBase = new ReturnLeaf();
		var copy = value.duplicate();
		while (copy == null) {}
	}
}
