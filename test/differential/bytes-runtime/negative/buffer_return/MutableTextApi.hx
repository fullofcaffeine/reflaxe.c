/** Declares the deliberately invalid retained-pointer return boundary. */
@:c.include("mutable_text.h", c.IncludeKind.Local)
extern class MutableTextApi {
	/** Attempt to return a pointer whose ownership and lifetime are unspecified. */
	@:c.name("fixture_return_mutable_text")
	public static function retained():c.CStringBufferRef;
}
