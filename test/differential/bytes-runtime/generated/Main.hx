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
			|| lexicographicShort.compare(lexicographicLong) <= 0) {}
	}
}
