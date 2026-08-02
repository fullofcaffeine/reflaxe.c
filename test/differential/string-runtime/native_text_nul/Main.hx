/** Proves that embedded NUL fails before a native text consumer executes. */
final class Main {
	static function main():Void {
		final accepted = TextObserver.matches(c.CStringRef.to("A\x00B"), 0);
		while (accepted) {}
	}
}
