/**
	Exercises only allocation-free Dynamic scalar carriers.

	The fixture deliberately has no output or managed values. Its generated
	runtime plan must therefore select the Dynamic carrier without allocation,
	object, or collector support.
**/
class Main {
	/** Keep exact boxes, casts, and equality reachable without adding I/O. */
	static function main():Void {
		final integer:Dynamic = 7;
		final floating:Dynamic = 2.5;
		final boolean:Dynamic = true;
		final absent:Dynamic = null;
		final restoredInteger:Int = integer;
		final restoredFloating:Float = floating;
		final restoredBoolean:Bool = boolean;
		final sameInteger = integer == 7;
		final sameNull = absent == null;
		if (restoredInteger == 0 && restoredFloating == 0.0 && !restoredBoolean && sameInteger && !sameNull)
			return;
	}
}
