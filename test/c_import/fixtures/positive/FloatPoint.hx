@:c.layout(c.Layout.Struct)
@:c.name("pointlib_float_point")
@:c.include("pointlib.h", c.IncludeKind.Local)
extern class FloatPoint {
	@:c.name("x")
	public var x:c.Float32;

	@:c.name("y")
	public var y:c.Float32;
}
