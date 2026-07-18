@:c.include("pointlib.h", c.IncludeKind.Local)
extern class PointerApi {
	@:c.name("pointlib_data")
	public static function data():c.ConstPtr<c.Int32>;
}
