import haxe.io.Bytes;

/**
 * Exercises the first ordinary-Haxe `Bytes` slice without target-specific code.
 *
 * The generated C executable has no output. It terminates only when mutable
 * aliasing, copying, overlapping moves, byte masking, UTF-8 construction, and
 * comparison all agree with the pinned Haxe implementation.
 */
final class Main {
	static function makeBuffer():Bytes {
		final result = Bytes.alloc(8);
		return result;
	}

	static function makeText():Bytes
		return Bytes.ofString("Haxe");

	/** Borrow a Bytes argument without taking or extending its ownership. */
	static function firstByte(value:Bytes):Int
		return value.get(0);

	/** Borrow two Bytes arguments after Haxe evaluates them from left to right. */
	static function sameBytes(left:Bytes, right:Bytes):Bool
		return left.compare(right) == 0;

	/**
		Copy one runtime String parameter into independent mutable byte storage.

		The parameter is a borrowed immutable UTF-8 view. `Bytes.ofString` copies
		its exact logical byte length, so embedded NUL remains content rather than
		ending the value as it would in an unbounded C string.
	**/
	static function copyText(value:String):Bytes
		return Bytes.ofString(value);

	/**
		Return early after passing a fresh managed result directly into another call.

		The caller must own `makeText()` across `firstByte(...)`, then release that
		temporary before this function returns. The false path proves no uncreated
		owner is included in cleanup.
	**/
	static function readFreshText(enabled:Bool):Int {
		if (enabled)
			return firstByte(makeText());
		return -1;
	}

	static function main():Void {
		final bytes = makeBuffer();
		final alias = bytes;
		alias.set(0, 0x141);
		bytes.fill(1, 4, 0x22);
		bytes.blit(3, bytes, 0, 5);

		final copy = bytes.sub(2, 4);
		copy.set(0, 0x33);
		final text = makeText();
		final expected = Bytes.alloc(4);
		expected.set(0, 0x22);
		expected.set(1, 0x41);
		expected.set(2, 0x22);
		expected.set(3, 0x22);
		final compared = bytes.sub(2, 4);
		final lexicographicShort = Bytes.alloc(1);
		lexicographicShort.set(0, 0xff);
		final lexicographicLong = Bytes.alloc(2);
		final nestedCompare = bytes.compare(bytes.sub(0, 8));
		final directFreshByte = firstByte(makeText());
		final twoFreshArguments = sameBytes(makeText(), Bytes.ofString("Haxe"));
		final freshReceiverByte = Bytes.ofString("Haxe").get(0);
		final earlyFreshByte = readFreshText(true);
		final runtimeText = "hé\x00🙂";
		final runtimeAlias = runtimeText;
		final runtimeCopy = copyText(runtimeAlias);
		final repeatedCopy = copyText(runtimeText);
		final emptyCopy = copyText("");

		while (bytes.length != 8
			|| bytes.get(0) != 0x41
			|| bytes.get(2) != 0x22
			|| bytes.get(3) != 0x41
			|| bytes.get(7) != 0x22
			|| copy.get(0) != 0x33
			|| bytes.get(2) != 0x22
			|| text.length != 4
			|| text.get(0) != 0x48
			|| text.get(3) != 0x65
			|| expected.compare(compared) != 0 // The pinned implementation compares shared content before length:
				// [0xff] sorts after [0x00, 0x00] even though it is shorter.
			|| lexicographicShort.compare(lexicographicLong) <= 0
			|| nestedCompare != 0
			|| directFreshByte != 0x48
			|| !twoFreshArguments
			|| freshReceiverByte != 0x48
			|| earlyFreshByte != 0x48
			|| runtimeCopy.length != 8
			|| runtimeCopy.get(0) != 0x68
			|| runtimeCopy.get(1) != 0xc3
			|| runtimeCopy.get(2) != 0xa9
			|| runtimeCopy.get(3) != 0
			|| runtimeCopy.get(4) != 0xf0
			|| runtimeCopy.get(7) != 0x82
			|| repeatedCopy.compare(runtimeCopy) != 0
			|| emptyCopy.length != 0) {}
	}
}
