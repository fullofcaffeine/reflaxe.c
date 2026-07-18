@:c.include("unused_pointlib.h", c.IncludeKind.Local)
@:c.link("unused_pointlib")
@:c.pkgConfig("unused-pointlib")
@:c.framework("UnusedPointlib")
extern class UnusedLib {
	@:c.constant
	@:c.name("UNUSED_POINTLIB_VALUE")
	public static var value(default, never):c.Int32;
}
