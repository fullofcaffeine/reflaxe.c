/**
	Provides an invalid conditional function selection for the diagnostic check.

	The two branches deliberately have different parameter and result types, so
	the Haxe type checker must reject the source before haxe.c emits any files.
**/

/** Confirms that control flow cannot erase an exact function signature. **/
class Main {
	static function integerIdentity(value:Int):Int {
		return value;
	}

	static function floatIdentity(value:Float):Float {
		return value;
	}

	static function main():Void {
		final operation:Int->Int = true ? integerIdentity : floatIdentity;
		operation(1);
	}
}
