@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.link("pointlib")
@:c.pkgConfig("pointlib-fixture")
@:c.framework("PointlibFixture")
extern class BuildFactApi {
	@:c.name("pointlib_build_fact_probe")
	public static function probe():Void;
}
