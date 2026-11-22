/** Keep the fixture's intentional untyped boundary in one private alias. */
private typedef ReferenceDynamic = Dynamic;

/**
	Records the reference Haxe interpreter's observable Dynamic behavior.

	The compiler fixtures use this result as an independent oracle for casts,
	calls, equality, null, and left-to-right call evaluation. It deliberately
	contains no haxe.c types or lowering helpers.
**/
class DynamicSemantics {
	static final events:Array<String> = [];

	/** Return an ordinary typed sum for the Dynamic call checks. */
	static function add(left:Int, right:Int):Int
		return left + right;

	/** Record when the callable expression is selected. */
	static function selectCallable(label:String):ReferenceDynamic {
		events.push(label);
		return add;
	}

	/** Record one argument evaluation while preserving its Dynamic carrier. */
	static function argument(label:String, value:Int):ReferenceDynamic {
		events.push(label);
		return value;
	}

	/** Print one stable line that the target differential suite can share. */
	static function main():Void {
		final integer:ReferenceDynamic = 7;
		final floating:ReferenceDynamic = 7.0;
		final text:ReferenceDynamic = "seven";
		final absent:ReferenceDynamic = null;
		final callable:ReferenceDynamic = add;
		final castInteger:Int = integer;
		final called:Int = callable(2, 3);
		final ordered:Int = selectCallable("callee")(argument("left", 1), argument("right", 2));
		final equalities = [
			integer == 7,
			integer == floating,
			text == "seven",
			integer == text,
			absent == null
		];
		Sys.println('DYNAMIC_ORACLE=cast:$castInteger;call:$called;equal:${equalities.join(",")};order:${events.join(",")};ordered:$ordered');
	}
}
