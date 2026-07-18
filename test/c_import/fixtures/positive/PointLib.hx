@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.link("pointlib")
extern class PointLib {
	@:c.constant
	@:c.name("POINTLIB_COORD_ZERO")
	public static var zero(default, never):Coord;

	@:c.constant
	@:c.name("POINTLIB_COORD_ONE")
	public static var one(default, never):Coord;

	@:c.constant
	@:c.name("POINTLIB_COORD_TWO")
	public static var two(default, never):Coord;

	@:c.constant
	@:c.name("POINTLIB_COORD_NEGATIVE_THREE")
	public static var negativeThree(default, never):Coord;

	@:c.constant
	@:c.name("POINTLIB_COORD_FIVE")
	public static var five(default, never):Coord;

	@:c.constant
	@:c.name("POINTLIB_COORD_SEVEN")
	public static var seven(default, never):Coord;

	@:c.name("pointlib_point_make")
	public static function make(x:Coord, y:Coord):Point;

	@:c.name("pointlib_point_translate")
	public static function translate(point:Point, dx:Coord, dy:Coord):Point;

	@:c.name("pointlib_point_dot")
	public static function dot(left:Point, right:Point):c.Int64;

	@:c.name("pointlib_point_component")
	public static function component(point:Point, axis:Axis):Coord;

	@:c.name("pointlib_point_verify")
	public static function verify(left:Point, right:Point, dot:c.Int64, component:Coord, axis:Axis, label:c.CString):Bool;
}
