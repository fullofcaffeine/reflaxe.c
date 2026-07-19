@:c.include("hxc_raylib_smoke_constants.h", c.IncludeKind.Local)
extern class SmokeConstants {
	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_SIZE")
	public static var size(default, never):c.Int32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_CENTER")
	public static var center(default, never):c.Int32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_COLOR")
	public static var color(default, never):c.UInt32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_KEY_NULL")
	public static var keyNull(default, never):c.Int32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_MOUSE_LEFT")
	public static var mouseLeft(default, never):c.Int32;

	@:c.constant
	@:c.name("HXC_RAYLIB_SMOKE_LOG_NONE")
	public static var logNone(default, never):c.Int32;
}
