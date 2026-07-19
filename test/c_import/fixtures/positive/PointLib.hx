@:c.include("pointlib.h", c.IncludeKind.Local)
@:c.link("pointlib")
extern class PointLib {
	public static function pointlib_build_fact_probe():Void;

	@:c.constant
	@:c.name("POINTLIB_FLOAT_ONE_POINT_FIVE")
	public static var floatOnePointFive(default, never):c.Float32;

	@:c.constant
	@:c.name("POINTLIB_FLOAT_NEGATIVE_TWO")
	public static var floatNegativeTwo(default, never):c.Float32;

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

	@:c.name("pointlib_point_alias_identity")
	public static function aliasIdentity(point:PointAlias):PointAlias;

	@:c.name("pointlib_point_dot")
	public static function dot(left:Point, right:Point):c.Int64;

	@:c.name("pointlib_point_component")
	public static function component(point:Point, axis:Axis):Coord;

	@:c.name("pointlib_point_verify")
	public static function verify(left:Point, right:Point, dot:c.Int64, component:Coord, axis:Axis, label:c.CString):Bool;

	@:c.name("pointlib_float_point_make")
	public static function makeFloatPoint(x:c.Float32, y:c.Float32):FloatPoint;

	@:c.name("pointlib_float_point_scale")
	public static function scaleFloatPoint(point:FloatPoint, scale:c.Float32):FloatPoint;

	@:c.name("pointlib_float_point_dot")
	public static function dotFloatPoint(left:FloatPoint, right:FloatPoint):c.Float32;

	@:c.name("pointlib_float32_verify")
	public static function verifyFloat32(point:FloatPoint, dot:c.Float32, widened:Float, tie:c.Float32, subnormal:c.Float32, positiveInfinity:c.Float32,
		nan:c.Float32, negativeZero:c.Float32, finiteOverflow:c.Float32):Bool;
}
