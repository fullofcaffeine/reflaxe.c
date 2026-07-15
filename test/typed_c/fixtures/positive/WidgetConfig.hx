package;

@:c.layout(c.Layout.Struct)
@:c.header("widget.h", c.Header.Public)
@:c.name("widget_config")
@:c.align(8)
extern class WidgetConfig {
	public var dimensions:Dimensions;
	public var capacity:c.Size;
	public var storage:c.NullablePtr<WidgetStorage>;
	@:c.align(4)
	public var flags:c.UInt32;
}
