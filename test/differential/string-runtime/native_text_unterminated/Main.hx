/** Proves that an interior String view fails before the native consumer. */
final class Main {
	static function main():Void {
		final source = "Ha" + "xe";
		final interior = source.substring(0, 2);
		final accepted = TextObserver.matches(c.CStringRef.to(interior), 1);
		while (accepted) {}
	}
}
