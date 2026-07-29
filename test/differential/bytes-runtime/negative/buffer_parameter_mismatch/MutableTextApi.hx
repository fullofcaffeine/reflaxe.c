/** Declares the deliberately mismatched read-only native text boundary. */
@:c.include("mutable_text.h", c.IncludeKind.Local)
extern class MutableTextApi {
	/** Accept immutable NUL-terminated text, not caller-owned mutable storage. */
	@:c.name("fixture_replace_const_text")
	public static function replace(text:c.CString, capacity:Int):Bool;
}
