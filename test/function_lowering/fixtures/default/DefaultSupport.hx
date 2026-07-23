/** Keeps cross-module default ownership visible to the call-graph test. */
final class DefaultSupport {
	public static function crossModule(value:Int, scale:Int = 10, add:Int = 6):Int {
		return value * scale + add;
	}

	public static function genericIdentity<T>(value:T, ignored:Int = 3):T {
		ignored;
		return value;
	}
}
