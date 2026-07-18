@:c.include("pointlib.h", c.IncludeKind.Local)
extern class VariadicApi {
	@:c.name("pointlib_log")
	public static function log(format:c.CString, values:haxe.Rest<c.Int32>):Void;
}
