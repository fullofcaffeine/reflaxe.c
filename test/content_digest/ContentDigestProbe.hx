import haxe.crypto.Sha256;
import haxe.io.Bytes;
import reflaxe.c.emit.CContentDigest.sha256Hex;

/**
	Checks the compiler's Eval-optimized SHA-256 against independent evidence.

	Known public vectors catch a shared regression in both implementations.
	Boundary-sized binary payloads then compare the streaming implementation
	with Haxe's standard library across every one-block/two-block padding edge.
**/
class ContentDigestProbe {
	static function main():Void {
		assertDigest("", "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
		assertDigest("abc", "ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
		assertDigest("The quick brown fox jumps over the lazy dog", "d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592");
		for (length in [1, 2, 3, 54, 55, 56, 57, 63, 64, 65, 119, 120, 121, 127, 128, 129, 1024, 65537]) {
			final input = Bytes.alloc(length);
			for (index in 0...length)
				input.set(index, (index * 131 + length * 17) & 0xFF);
			final expected = Sha256.make(input).toHex();
			final actual = sha256Hex(input);
			if (actual != expected)
				throw 'SHA-256 differential failed for $length bytes: expected $expected, got $actual';
		}
		Sys.println("content-digest: OK");
	}

	static function assertDigest(value:String, expected:String):Void {
		final actual = sha256Hex(Bytes.ofString(value));
		if (actual != expected)
			throw 'SHA-256 known vector failed for `$value`: expected $expected, got $actual';
	}
}
