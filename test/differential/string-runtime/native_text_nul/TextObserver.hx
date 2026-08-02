/** C observer that must never receive the embedded-NUL fixture value. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class TextObserver {
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringRef, caseId:Int):Bool;
}
