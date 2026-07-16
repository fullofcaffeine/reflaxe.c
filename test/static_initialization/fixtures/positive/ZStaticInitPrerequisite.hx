class ZStaticInitPrerequisite {
	public static var value:Int = MStaticInitTrace.mark(2);

	static function __init__():Void {
		MStaticInitTrace.classOrder = MStaticInitTrace.classOrder * 10 + 2;
	}
}
