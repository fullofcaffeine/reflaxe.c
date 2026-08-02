/** Proves that Haxe rejects a non-String owner before target lowering. */
@:c.include("text_observer.h", c.IncludeKind.Local)
extern class TextObserver {
	@:c.name("fixture_text_matches")
	public static function matches(text:c.CStringRef, caseId:Int):Bool;
}

final class Main {
	static function main():Void
		TextObserver.matches(c.CStringRef.to(42), 0);
}
