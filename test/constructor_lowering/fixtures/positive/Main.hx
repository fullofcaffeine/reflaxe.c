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

/** A small mutable child whose identity is observable through its state. */
class OwnedCounter {
	public var value:Int;

	public function new(seed:Int) {
		value = seed;
	}

	public function add(delta:Int):Int {
		value += delta;
		return value;
	}

	/** Force Haxe to name the owned receiver `_this` inside its caller. */
	public inline function addInline(delta:Int):Int {
		value += delta;
		return value;
	}

	/** Exercise a compiler-created receiver alias across lazy Boolean flow. */
	public inline function matchesInline(expected:Int):Bool
		return value >= 0 && value == expected;
}

/** A parent that owns its final child for exactly the parent's lifetime. */
class CounterOwner {
	public final child:OwnedCounter = new OwnedCounter(11);
	public var observedDuringConstruction:Int;

	public function new() {
		// Seeing the initialized value here proves that child construction ran
		// before the parent's authored constructor body.
		observedDuringConstruction = child.add(4);
	}

	public function run(delta:Int):Int {
		// Mutating the same child again proves stable identity rather than a copy
		// returned for each field access.
		if (!child.matchesInline(15))
			return -1;
		return child.addInline(delta) + observedDuringConstruction;
	}
}

class Main {
	/** Use a caller-owned object for this call without taking ownership of it. */
	static function drive(owner:CounterOwner, delta:Int):Int {
		return owner.run(delta);
	}

	static function main():Void {
		ConstructorTrace.value = 0;
		var leaf = new LeafRecord(ConstructorTrace.mark(1));
		var empty = new EmptyLeaf();
		var failable = new FailableLeaf(false);
		var owner = new CounterOwner();
		// A local alias is another non-owning pointer to the same stack object.
		// The compiler retains the original lifetime and still rejects later escape.
		var ownerAlias = owner;
		var ownedResult = drive(ownerAlias, 2);
		while (!(ConstructorTrace.value == 1326457809
			&& leaf.defaulted == 7
			&& leaf.baseField == 4
			&& leaf.firstLeafField == 2
			&& leaf.secondLeafField == 3
			&& failable.initializedBeforeFailure == 8
			&& ownerAlias.observedDuringConstruction == 15
			&& ownerAlias.child.value == 17
			&& ownedResult == 32
			&& empty != null)) {}
	}
}
