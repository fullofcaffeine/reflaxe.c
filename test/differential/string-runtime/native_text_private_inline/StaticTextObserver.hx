/** The literal-only view of the independently authored C text observer. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class StaticTextObserver {
	/** Compare static text without introducing a managed runtime borrow. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CString, caseId:Int):Bool;
}
