@:c.include("pointlib.h", c.IncludeKind.Local)
extern class MissingNameApi {
	@:c.callingConvention(c.CallingConvention.C)
	public static function value():c.Int32;
}
