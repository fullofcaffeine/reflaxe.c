@:c.layout(c.Layout.Struct)
@:c.name("pointlib_point")
@:c.include("pointlib.h", c.IncludeKind.Local)
extern class Point {
	public var x:Coord;

	public var y:Coord;
}
