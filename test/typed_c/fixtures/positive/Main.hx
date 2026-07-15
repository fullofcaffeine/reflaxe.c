package;

class Main {
	static var dimensions:Dimensions;
	static var config:WidgetConfig;
	static var api:WidgetApi;
	static var sqlite:SQLite3;
	static var surface:SurfaceProbe;

	static function main():Void {
		c.StaticAssert.require(true, "typed C contract fixture is enabled");
	}
}
