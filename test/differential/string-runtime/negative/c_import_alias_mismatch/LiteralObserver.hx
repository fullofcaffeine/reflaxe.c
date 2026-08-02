/** One accurate declaration of the fixture's native text observer. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class LiteralObserver {
	/** Compare literal text using the observer's integer case selector. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CString, caseId:Int):Bool;
}
