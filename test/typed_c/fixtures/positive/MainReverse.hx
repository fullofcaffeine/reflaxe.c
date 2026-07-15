package;

class MainReverse {
	static var surface:SurfaceProbe;
	static var sqlite:SQLite3;
	static var api:WidgetApi;
	static var config:WidgetConfig;
	static var dimensions:Dimensions;

	static function main():Void {
		c.StaticAssert.require(true, "typed C contract fixture is enabled");
	}
}
