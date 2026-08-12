/** Minimal imported API that makes the invalid field-hidden type reachable. */
@:c.include("pointlib.h", c.IncludeKind.Local)
extern class PointLib {
	@:c.name("pointlib_hidden_point_make")
	public static function hiddenPointMake():HiddenPoint;

	@:c.name("pointlib_hidden_point_identity")
	public static function hiddenPointIdentity(point:HiddenPoint):HiddenPoint;

	@:c.name("pointlib_hidden_point_verify")
	public static function hiddenPointVerify(point:HiddenPoint):Bool;
}
