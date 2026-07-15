package;

@:c.layout(c.Layout.Struct)
@:c.header("widget.h", c.Header.Public)
@:c.name("widget_dimensions")
extern class Dimensions {
	public var width:c.UInt32;
	public var height:c.UInt32;
}
