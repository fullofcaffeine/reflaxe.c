class BodyFixture {
	/** Returns a primitive local assigned after its declaration. */
	static function definitelyAssignedInteger():Int {
		var value:Int;
		value = 31;
		return value;
	}

	/** Returns a local established by both continuing branch arms. */
	static function definitelyAssignedBranches(choice:Bool):Int {
		var value:Int;
		if (choice)
			value = 7;
		else
			value = 11;
		return value;
	}

	/** Returns a local whose unassigned branch exits before the read. */
	static function definitelyAssignedAfterReturn(choice:Bool):Int {
		var value:Int;
		if (choice)
			value = 13;
		else
			return 17;
		return value;
	}

	/** Reads and updates a local only after its straight-line first store. */
	static function definitelyAssignedLoop(limit:Int):Int {
		var value:Int;
		value = 0;
		while (value < limit)
			value = limit;
		return value;
	}

	static function integerValue():Int {
		var value:Int = 17;
		value;
		{
			var value:Int = 42;
			return value;
		}
	}

	static function directInteger():Int {
		return 23;
	}

	static function floatingValue():Float {
		var value:Float = 1.5;
		return value;
	}

	static function booleanValue():Bool {
		var value:Bool = true;
		return value;
	}

	static function explicitVoid():Void {
		var first:Int = 1;
		first;
		{
			var second:Int = 2;
			second;
		}
		return;
	}

	static function implicitVoid():Void {
		var value:Bool = false;
		value;
	}

	static function unsignedValue():UInt {
		var value:UInt = 29;
		return value;
	}

	static function throwPayload():Int {
		return 99;
	}

	static function chooseOrThrow(choice:Int):Int {
		return switch choice {
			case 0: 41;
			case 1: throw throwPayload();
			default: 43;
		}
	}

	static function main():Void {
		return;
	}
}
