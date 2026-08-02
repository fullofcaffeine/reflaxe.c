/**
	Independent C observer for one synchronous immutable text borrow.

	The C fixture owns the expected UTF-8 bytes. `CStringRef` promises only that
	the managed Haxe String remains readable until this direct call returns; the C
	function neither stores the pointer nor changes the text.
**/
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class TextObserver {
	/** Compare one borrowed String with the C fixture's manually authored case. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringRef, caseId:Int):Bool;
}
