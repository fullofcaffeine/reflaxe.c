@:c.include("pointlib.h", c.IncludeKind.Local)
extern class RefApi {
	@:c.name("pointlib_flip")
	public static function flip(value:c.Ref<Bool>):Void;
}
