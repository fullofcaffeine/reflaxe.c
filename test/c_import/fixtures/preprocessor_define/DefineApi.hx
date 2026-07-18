@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.define("POINTLIB_IMPORT_MODE", 1)
extern class DefineApi {
	@:c.constant
	@:c.name("POINTLIB_COORD_ONE")
	public static var one(default, never):c.Int32;
}
