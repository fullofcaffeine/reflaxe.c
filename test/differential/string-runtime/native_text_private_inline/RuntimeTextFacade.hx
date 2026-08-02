/** Module-private C declaration reached only after the public facade is inlined. */
@:c.include("text_observer.h", c.IncludeKind.Local)
private extern class PrivateTextObserver {
	/** Compare borrowed text with the independent fixture's ASCII byte sequence. */
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringRef, caseId:Int):Bool;
}

/**
	Lends one runtime-owned String through an ordinary public Haxe API.

	Keeping the extern private prevents a C-shaped helper from becoming product
	API. haxe.c must still retain its exact typed declaration after this facade is
	inlined into the caller.
**/
final class RuntimeTextFacade {
	/** Pass one compile-time literal to the same native observer. */
	public static inline function matchesStatic():Bool
		return StaticTextObserver.matches("Haxe", 1);

	/** Borrow `text` for the one synchronous, non-retaining native comparison. */
	public static inline function matches(text:String):Bool
		return PrivateTextObserver.matches(c.CStringRef.to(text), 1);
}
