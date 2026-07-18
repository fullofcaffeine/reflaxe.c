class ExplodingBase {
	public var initialized:Int = 1;

	public function new(shouldFail:Bool) {
		if (shouldFail)
			throw 99;
	}
}

class ExplodingLeaf extends ExplodingBase {
	public function new(shouldFail:Bool) {
		super(shouldFail);
	}
}

class Main {
	static function main():Void {
		var value = new ExplodingLeaf(true);
		while (value == null) {}
	}
}
