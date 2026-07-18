@:c.include("raylib.h", c.IncludeKind.System)
@:c.include("hxc_raylib_smoke_constants.h", c.IncludeKind.Local)
	#if raylib_system_pkg_config
	@:c.pkgConfig("raylib")
	#elseif raylib_platform_linux
	@:c.link("raylib") @:c.link("m")
		#if raylib_configuration_desktop
		@:c.link("pthread") @:c.link("GL") @:c.link("X11") @:c.link("dl") @:c.link("rt")
		#end
	#elseif raylib_platform_macos
	@:c.link("raylib")
		#if raylib_configuration_desktop
		@:c.framework("OpenGL") @:c.framework("Cocoa") @:c.framework("IOKit") @:c.framework("CoreFoundation")
		#end
	#elseif raylib_platform_windows
	@:c.link("raylib") @:c.link("winmm")
		#if raylib_configuration_desktop
		@:c.link("opengl32") @:c.link("gdi32") @:c.link("shell32") @:c.link("user32")
		#end
	#else
	#error "Select exactly one reviewed raylib provisioning platform"
	#end
extern class RaylibNative {
	@:c.name("SetTraceLogLevel")
	public static function SetTraceLogLevel(logLevel:TraceLogLevel):Void;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_SIZE")
	public static var smokeSize(default, never):c.Int32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_CENTER")
	public static var smokeCenter(default, never):c.Int32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_COLOR")
	public static var smokeColor(default, never):c.UInt32;

	@:c.name("GetColor")
	public static function GetColor(hexValue:c.UInt32):Color;

	@:c.name("InitWindow")
	public static function InitWindow(width:c.Int32, height:c.Int32, title:c.CString):Void;

	@:c.name("IsWindowReady")
	public static function IsWindowReady():Bool;

	@:c.name("BeginDrawing")
	public static function BeginDrawing():Void;

	@:c.name("ClearBackground")
	public static function ClearBackground(color:Color):Void;

	@:c.name("DrawPixel")
	public static function DrawPixel(x:c.Int32, y:c.Int32, color:Color):Void;

	@:c.name("EndDrawing")
	public static function EndDrawing():Void;

	@:c.name("CloseWindow")
	public static function CloseWindow():Void;
}
