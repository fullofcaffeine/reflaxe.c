class MStaticInitTrace {
	public static var classOrder:Int = classOrder;
	public static var phaseValue:Int = 7;
	public static var trace:Int = seed();

	static function __init__():Void {
		phaseValue = 3;
		classOrder = 1;
	}

	static function seed():Int {
		return 1;
	}

	public static function mark(digit:Int):Int {
		trace = trace * 10 + digit;
		return trace;
	}
}
