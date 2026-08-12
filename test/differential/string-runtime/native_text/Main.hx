/**
	Proves that runtime-created Haxe Strings can cross one non-retaining C call.

	Every expectation lives in the independent C fixture. The Haxe side supplies
	empty, ASCII, Unicode, borrowed, suffix-view, and fresh values so lowering must
	keep each real owner alive without copying the observer's algorithm.
**/
final class Main {
	/** Build runtime-owned UTF-8 through ordinary Haxe source. */
	static function build():String {
		final output = new StringBuf();
		output.add("A");
		output.addChar(0xE9);
		output.addChar(0x1F600);
		return output.toString();
	}

	/** Borrow a caller-owned value through one normal Haxe parameter. */
	static function borrowedMatches(value:String):Bool
		return TextObserver.matches(c.CStringRef.to(value), 2);

	/**
		Copy an interior view before lending it to a C API that expects terminated text.

		`StringBuf` copies the view into stable storage with a trailing NUL byte.
	**/
	static function copiedViewMatches(value:String):Bool {
		final buffer = new StringBuf();
		buffer.add(value);
		final terminated = buffer.toString();
		return TextObserver.matches(c.CStringRef.to(terminated), 4);
	}

	static function main():Void {
		final ascii = "Ha" + "xe";
		final unicode = build();
		final suffix = unicode.substring(1);
		final interior = unicode.substring(0, 2);
		final valid = TextObserver.matches(c.CStringRef.to(""), 0)
			&& TextObserver.matches(c.CStringRef.to(ascii), 1)
			&& borrowedMatches(unicode)
			&& TextObserver.matches(c.CStringRef.to(suffix), 3)
			&& copiedViewMatches(interior)
			&& TextObserver.matches(c.CStringRef.to(build()), 2);
		while (!valid) {}
	}
}
