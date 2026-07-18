class ConstructorTrace {
	public static var value:Int = 0;

	public static function mark(digit:Int):Int {
		value = value * 10 + digit;
		return digit;
	}
}

class BaseRecord {
	public var defaulted:Int;
	public var baseField:Int = ConstructorTrace.mark(4);

	public function new(value:Int) {
		ConstructorTrace.mark(5);
		defaulted = value;
	}
}

class LeafRecord extends BaseRecord {
	public var firstLeafField:Int = ConstructorTrace.mark(2);
	public var secondLeafField:Int = ConstructorTrace.mark(3);

	public function new(value:Int) {
		super(ConstructorTrace.mark(6) + value);
		ConstructorTrace.mark(7);
	}
}

class EmptyBase {
	public function new() {}
}

class EmptyLeaf extends EmptyBase {
	public function new() {
		super();
	}
}

class FailableBase {
	public var initializedBeforeFailure:Int = ConstructorTrace.mark(8);

	public function new(shouldFail:Bool) {
		var temporary = new EmptyLeaf();
		if (shouldFail && temporary != null)
			throw 99;
		ConstructorTrace.mark(0);
	}
}

class FailableLeaf extends FailableBase {
	public function new(shouldFail:Bool) {
		super(shouldFail);
		ConstructorTrace.mark(9);
	}
}

class Main {
	static function main():Void {
		ConstructorTrace.value = 0;
		var leaf = new LeafRecord(ConstructorTrace.mark(1));
		var empty = new EmptyLeaf();
		var failable = new FailableLeaf(false);
		while (!(ConstructorTrace.value == 1326457809
			&& leaf.defaulted == 7
			&& leaf.baseField == 4
			&& leaf.firstLeafField == 2
			&& leaf.secondLeafField == 3
			&& failable.initializedBeforeFailure == 8
			&& empty != null)) {}
	}
}
