package reflaxe.c.emit;

import haxe.io.Bytes;

/**
	Computes stable content digests for compiler-owned output.

	Generated manifests and build caches use SHA-256, so this module preserves
	that exact public algorithm. The Eval macro host gets a streaming
	implementation because the standard library first expands the whole input
	into boxed integer arrays; multi-megabyte generated C then creates far more
	temporary data than the source itself. Other Haxe targets keep the standard
	library implementation. Both paths consume the same UTF-8 bytes and return
	the same lowercase hexadecimal digest.
**/
function sha256Hex(input:Bytes):String {
	#if eval
	return new EvalSha256().digest(input).toHex();
	#else
	return haxe.crypto.Sha256.make(input).toHex();
	#end
}

#if eval
/**
	A fixed-memory SHA-256 worker specialized for Haxe's Eval macro runtime.

	The worker reads complete 64-byte input blocks directly and allocates only a
	64-word schedule plus the final one or two padded blocks. Direct `Int`
	addition is correct here because Eval integers have the 32-bit wraparound
	needed by SHA-256. This is an execution optimization only: known-vector and
	standard-library differential tests own the cryptographic result.
**/
private class EvalSha256 {
	static final ROUND_CONSTANTS:Bytes = buildRoundConstants();

	final schedule:Bytes = Bytes.alloc(64 * 4);
	var h0:Int = 0x6A09E667;
	var h1:Int = 0xBB67AE85;
	var h2:Int = 0x3C6EF372;
	var h3:Int = 0xA54FF53A;
	var h4:Int = 0x510E527F;
	var h5:Int = 0x9B05688C;
	var h6:Int = 0x1F83D9AB;
	var h7:Int = 0x5BE0CD19;

	public function new() {}

	/** Hash one complete immutable byte sequence and return its 32-byte digest. */
	public function digest(input:Bytes):Bytes {
		var offset = 0;
		while (offset + 64 <= input.length) {
			compress(input, offset);
			offset += 64;
		}

		final remaining = input.length - offset;
		final tail = Bytes.alloc(remaining <= 55 ? 64 : 128);
		tail.blit(0, input, offset, remaining);
		tail.set(remaining, 0x80);
		writeBigEndian(tail, tail.length - 8, input.length >>> 29);
		writeBigEndian(tail, tail.length - 4, input.length << 3);
		compress(tail, 0);
		if (tail.length == 128)
			compress(tail, 64);

		final output = Bytes.alloc(32);
		writeBigEndian(output, 0, h0);
		writeBigEndian(output, 4, h1);
		writeBigEndian(output, 8, h2);
		writeBigEndian(output, 12, h3);
		writeBigEndian(output, 16, h4);
		writeBigEndian(output, 20, h5);
		writeBigEndian(output, 24, h6);
		writeBigEndian(output, 28, h7);
		return output;
	}

	function compress(input:Bytes, offset:Int):Void {
		for (index in 0...16)
			schedule.setInt32(index * 4, readBigEndian(input, offset + index * 4));
		for (index in 16...64) {
			final previous2 = schedule.getInt32((index - 2) * 4);
			final previous15 = schedule.getInt32((index - 15) * 4);
			final gamma1 = rotateRight(previous2, 17) ^ rotateRight(previous2, 19) ^ (previous2 >>> 10);
			final gamma0 = rotateRight(previous15, 7) ^ rotateRight(previous15, 18) ^ (previous15 >>> 3);
			schedule.setInt32(index * 4, gamma1 + schedule.getInt32((index - 7) * 4) + gamma0 + schedule.getInt32((index - 16) * 4));
		}

		var a = h0;
		var b = h1;
		var c = h2;
		var d = h3;
		var e = h4;
		var f = h5;
		var g = h6;
		var h = h7;
		for (index in 0...64) {
			final sigma1 = rotateRight(e, 6) ^ rotateRight(e, 11) ^ rotateRight(e, 25);
			final choose = (e & f) ^ (~e & g);
			final first = h + sigma1 + choose + ROUND_CONSTANTS.getInt32(index * 4) + schedule.getInt32(index * 4);
			final sigma0 = rotateRight(a, 2) ^ rotateRight(a, 13) ^ rotateRight(a, 22);
			final majority = (a & b) ^ (a & c) ^ (b & c);
			final second = sigma0 + majority;
			h = g;
			g = f;
			f = e;
			e = d + first;
			d = c;
			c = b;
			b = a;
			a = first + second;
		}
		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
		h4 += e;
		h5 += f;
		h6 += g;
		h7 += h;
	}

	static inline function rotateRight(value:Int, amount:Int):Int {
		return (value >>> amount) | (value << (32 - amount));
	}

	static inline function readBigEndian(input:Bytes, offset:Int):Int {
		final little = input.getInt32(offset);
		return (little >>> 24) | ((little >>> 8) & 0x0000FF00) | ((little << 8) & 0x00FF0000) | (little << 24);
	}

	static inline function writeBigEndian(output:Bytes, offset:Int, value:Int):Void {
		output.set(offset, value >>> 24);
		output.set(offset + 1, value >>> 16);
		output.set(offset + 2, value >>> 8);
		output.set(offset + 3, value);
	}

	static function buildRoundConstants():Bytes {
		final values = [
			0x428A2F98,
			0x71374491,
			0xB5C0FBCF,
			0xE9B5DBA5,
			0x3956C25B,
			0x59F111F1,
			0x923F82A4,
			0xAB1C5ED5,
			0xD807AA98,
			0x12835B01,
			0x243185BE,
			0x550C7DC3,
			0x72BE5D74,
			0x80DEB1FE,
			0x9BDC06A7,
			0xC19BF174,
			0xE49B69C1,
			0xEFBE4786,
			0xFC19DC6,
			0x240CA1CC,
			0x2DE92C6F,
			0x4A7484AA,
			0x5CB0A9DC,
			0x76F988DA,
			0x983E5152,
			0xA831C66D,
			0xB00327C8,
			0xBF597FC7,
			0xC6E00BF3,
			0xD5A79147,
			0x06CA6351,
			0x14292967,
			0x27B70A85,
			0x2E1B2138,
			0x4D2C6DFC,
			0x53380D13,
			0x650A7354,
			0x766A0ABB,
			0x81C2C92E,
			0x92722C85,
			0xA2BFE8A1,
			0xA81A664B,
			0xC24B8B70,
			0xC76C51A3,
			0xD192E819,
			0xD6990624,
			0xF40E3585,
			0x106AA070,
			0x19A4C116,
			0x1E376C08,
			0x2748774C,
			0x34B0BCB5,
			0x391C0CB3,
			0x4ED8AA4A,
			0x5B9CCA4F,
			0x682E6FF3,
			0x748F82EE,
			0x78A5636F,
			0x84C87814,
			0x8CC70208,
			0x90BEFFFA,
			0xA4506CEB,
			0xBEF9A3F7,
			0xC67178F2
		];
		final bytes = Bytes.alloc(values.length * 4);
		for (index in 0...values.length)
			bytes.setInt32(index * 4, values[index]);
		return bytes;
	}
}
#end
