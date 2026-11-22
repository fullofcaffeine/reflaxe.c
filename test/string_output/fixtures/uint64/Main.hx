/** Exercise exact native integers through ordinary Haxe text operations. */
class Main {
	/** Keep each native value exact until the shared String formatter consumes it. */
	static function main():Void {
		final zero = UInt64Fixture.value(0);
		final wide = UInt64Fixture.value(1);
		final maximum = UInt64Fixture.value(2);
		Sys.println(zero);
		Sys.println(wide);
		final text = Std.string(maximum);
		Sys.println(text);
		Sys.println('prefix=${UInt64Fixture.value(2)}');
		Sys.println(UInt64Fixture.calls());
	}
}
