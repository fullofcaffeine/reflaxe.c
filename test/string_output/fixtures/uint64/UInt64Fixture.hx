/** Supply exact C values that cannot pass through Haxe Int or Float literals. */
@:c.include("uint64_fixture.h", c.IncludeKind.Local)
extern class UInt64Fixture {
	/** Return a native unsigned value and increment the observable call count. */
	@:c.name("fixture_uint64_value")
	public static function value(index:Int):c.UInt64;

	/** Count native evaluations independently of the formatting implementation. */
	@:c.name("fixture_uint64_calls")
	public static function calls():Int;
}
