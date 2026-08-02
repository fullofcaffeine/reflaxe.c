/** An intentionally incompatible alias for one already imported C symbol. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class IncompatibleObserver {
	/** Models the integer selector incorrectly as a C Boolean. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringRef, caseId:Bool):Bool;
}
