/**
 * Independent native operations for the observer import tracer.
 * Native runtime calls expose event ordering without depending on compiler roots;
 * all assertions and console output occur outside observation regions.
 */
@:c.include("interval_fixture.h", c.IncludeKind.Local)
extern class FixtureApi {
	/** Initialize a stopped native collector and one root owner outside observation. */
	@:c.name("interval_fixture_setup")
	public static function setup():Void;

	/** Allocate and release a real block through hxrt while observation is active. */
	@:c.name("interval_fixture_allocate")
	public static function allocate():Bool;

	/** Push and pop one real frame without allocating or collecting. */
	@:c.name("interval_fixture_roots")
	public static function roots():Bool;

	/** Inject a synthetic count above 32 bits solely to test exact counter transport. */
	@:c.name("interval_fixture_wide_count")
	public static function wideCount():Void;

	/** Fail the independent native process if one authored expectation is false. */
	@:c.name("interval_fixture_require")
	public static function require(condition:Bool):Void;

	/** Dispose the native test owner and print the success marker. */
	@:c.name("interval_fixture_finish")
	public static function finish():Void;

	/** Header-owned exact zero, avoiding an implicit integer conversion. */
	@:c.constant @:c.name("INTERVAL_FIXTURE_ZERO")
	public static var zero(default, never):c.UInt64;

	/** Header-owned exact one for independently authored counts. */
	@:c.constant @:c.name("INTERVAL_FIXTURE_ONE")
	public static var one(default, never):c.UInt64;

	/** More than 32 significant bits: truncation must fail the transport check. */
	@:c.constant @:c.name("INTERVAL_FIXTURE_WIDE")
	public static var wide(default, never):c.UInt64;
}
