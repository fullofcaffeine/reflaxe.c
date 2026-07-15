class BodyFixture {
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

	static function main():Void {
		return;
	}
}
