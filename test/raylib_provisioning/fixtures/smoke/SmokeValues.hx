@:c.include("hxc_raylib_smoke_constants.h", c.IncludeKind.Local)
extern class SmokeValues {
	@:c.callingConvention(c.CallingConvention.C)
	@:c.name("raylib_smoke_fixture_camera")
	public static function camera():raylib.raw.Camera;

	@:c.callingConvention(c.CallingConvention.C)
	@:c.name("raylib_smoke_fixture_screen_position")
	public static function screenPosition():raylib.raw.Vector2;
}
