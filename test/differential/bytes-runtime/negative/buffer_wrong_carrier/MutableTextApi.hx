/** Declares the mutable native boundary used by the wrong-owner fixture. */
@:c.include("mutable_text.h", c.IncludeKind.Local)
extern class MutableTextApi {
	/** Edit caller-owned text only during this direct imported call. */
	@:c.name("fixture_replace_mutable_text")
	public static function replace(text:c.CStringBufferRef, capacity:Int):Bool;
}
