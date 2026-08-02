/** Proves that an immutable C-text borrow cannot become an ordinary value. */
final class Main {
	static function main():Void {
		final escaped = c.CStringRef.to("runtime text");
	}
}
