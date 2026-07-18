@:c.layout(c.Layout.Struct)
@:c.name("Color")
@:c.include("raylib.h", c.IncludeKind.System)
extern class Color {
	@:c.name("r")
	public var r:c.UInt8;

	@:c.name("g")
	public var g:c.UInt8;

	@:c.name("b")
	public var b:c.UInt8;

	@:c.name("a")
	public var a:c.UInt8;
}
