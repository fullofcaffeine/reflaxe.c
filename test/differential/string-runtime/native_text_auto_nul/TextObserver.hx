/** Native observer that must remain unreachable for embedded-NUL text. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class TextObserver {
	/** Observe one prepared call-scoped C string. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringArg, caseId:Int):Bool;
}
