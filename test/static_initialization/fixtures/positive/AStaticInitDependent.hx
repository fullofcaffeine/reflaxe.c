class AStaticInitDependent {
	public static var first:Int = ZStaticInitPrerequisite.value;
	public static var second:Int = first + 5;
	public static var value:Int = MStaticInitTrace.mark(3) + ZStaticInitPrerequisite.value + second;

	static function __init__():Void {
		MStaticInitTrace.classOrder = MStaticInitTrace.classOrder * 10 + 3;
	}
}
