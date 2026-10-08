/**
	Exercises the explicit borrow-or-copy CString argument policy.

	Whole and suffix views keep their existing trailing NUL. The interior prefix
	requires temporary terminated storage, while the C observer owns the expected
	bytes independently.
**/
final class Main {
	/** Build one runtime-owned terminated String. */
	static function build():String {
		final output = new StringBuf();
		output.add("A");
		output.addChar(0xE9);
		output.addChar(0x1F600);
		return output.toString();
	}

	static function main():Void {
		final ascii = "Ha" + "xe";
		final unicode = build();
		final suffix = unicode.substring(1);
		final interior = unicode.substring(0, 2);
		final valid = TextObserver.matches(c.CStringArg.to(ascii), 1)
			&& TextObserver.matches(c.CStringArg.to(suffix), 3)
			&& TextObserver.matches(c.CStringArg.to(interior), 4)
			&& TextObserver.matches(c.CStringArg.to(build()), 2);
		while (!valid) {}
	}
}
