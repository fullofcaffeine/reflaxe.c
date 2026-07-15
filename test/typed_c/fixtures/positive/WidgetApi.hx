package;

@:c.header("widget.h", c.Header.Public)
extern class WidgetApi {
	@:c.export
	@:c.name("widget_recommended_capacity")
	@:c.linkage(c.Linkage.External)
	@:c.callingConvention(c.CallingConvention.C)
	public static function recommendedCapacity(config:c.ConstRef<WidgetConfig>):c.Size;

	@:c.constant
	@:c.name("WIDGET_API_VERSION")
	public static var version(default, never):c.UInt32;
}
