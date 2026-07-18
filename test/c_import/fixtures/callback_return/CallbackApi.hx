@:c.include("pointlib.h", c.IncludeKind.Local)
extern class CallbackApi {
	@:c.name("pointlib_callback")
	public static function callback():c.FunctionPtr<(c.Int32) -> c.Int32>;
}
