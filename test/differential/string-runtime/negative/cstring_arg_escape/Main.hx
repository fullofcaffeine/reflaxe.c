/** Proves that a borrow-or-copy CString value cannot escape its direct import. */
final class Main {
	static function main():Void {
		final escaped = c.CStringArg.to("runtime text");
	}
}
