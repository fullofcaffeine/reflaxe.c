/** Proves that shared native names still reject incompatible C signatures. */
final class Main {
	static function main():Void {
		final runtimeText = "Ha" + "xe";
		while (!LiteralObserver.matches("Haxe", 1)) {}
		while (!IncompatibleObserver.matches(c.CStringRef.to(runtimeText), true)) {}
	}
}
