/** One non-retaining native observer for the borrow-or-copy CString fixture. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class TextObserver {
	/** Compare one prepared native string with an independently owned case. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringArg, caseId:Int):Bool;
}
