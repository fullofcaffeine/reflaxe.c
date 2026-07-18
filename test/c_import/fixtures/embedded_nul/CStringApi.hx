@:c.include("pointlib.h", c.IncludeKind.Local)
extern class CStringApi {
	@:c.name("pointlib_consume")
	public static function consume(value:c.CString):Bool;
}
