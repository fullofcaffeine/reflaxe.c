@:c.layout(c.Layout.Struct)
@:c.name("pointlib_point")
@:c.include("pointlib.h", c.IncludeKind.Local)
extern class Point {
	@:c.name("x")
	public var x:Coord;

	@:c.name("y")
	public var y:Coord;
}
