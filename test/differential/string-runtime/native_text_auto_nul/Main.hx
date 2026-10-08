/** Proves that borrow-or-copy text rejects embedded NUL before the native call. */
final class Main {
	static function main():Void {
		final accepted = TextObserver.matches(c.CStringArg.to("A\x00B"), 0);
		while (accepted) {}
	}
}
