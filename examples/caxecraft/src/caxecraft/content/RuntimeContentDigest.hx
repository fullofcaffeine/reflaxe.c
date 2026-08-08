package caxecraft.content;

import haxe.io.Bytes;

/**
	Computes the package digest needed before runtime content can be published.

	This private worker is a bounded bridge for `haxe_c-0h4.4`: the pinned
	`haxe.crypto.Sha256` implementation uses definitely assigned local
	declarations that haxe.c does not yet admit. Keeping the bridge in ordinary,
	portable Haxe lets Eval and generated C verify the same bytes without a
	target conditional or handwritten C boundary. Remove it in favor of
	`haxe.crypto.Sha256.make` when that Bead closes and the standard worker passes
	the same known vectors and package-to-generation tracer.
**/
function runtimeSha256(input:Bytes):Bytes
	return new RuntimeSha256Worker().digest(input);

/** Return the package digest as the canonical lowercase 64-digit receipt. */
function runtimeSha256Hex(input:Bytes):String {
	final digest = runtimeSha256(input);
	final digits = "0123456789abcdef";
	final output = new StringBuf();
	for (index in 0...digest.length) {
		final value = digest.get(index);
		output.addChar(digits.charCodeAt(value >>> 4));
		output.addChar(digits.charCodeAt(value & 15));
	}
	return output.toString();
}

/**
	Hashes one immutable byte sequence with fixed working storage.

	A short-lived class keeps the eight running words and the reusable message
	schedule under one lifetime. The input is borrowed for the call; the returned
	32-byte digest is newly allocated and independently owned.
**/
private final class RuntimeSha256Worker {
	final roundConstants:Array<Int>;
	final schedule:Array<Int>;
	var h0:Int = 0x6A09E667;
	var h1:Int = 0xBB67AE85;
	var h2:Int = 0x3C6EF372;
	var h3:Int = 0xA54FF53A;
	var h4:Int = 0x510E527F;
	var h5:Int = 0x9B05688C;
	var h6:Int = 0x1F83D9AB;
	var h7:Int = 0x5BE0CD19;

	/** Allocate the fixed 64-word schedule and reviewed SHA-256 constants. */
	public function new() {
		roundConstants = [
			0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5, 0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
			0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3, 0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
			0xE49B69C1, 0xEFBE4786,  0xFC19DC6, 0x240CA1CC, 0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
			0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7, 0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
			0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13, 0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
			0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3, 0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
			0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
			0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208, 0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
		];
		schedule = [];
		for (_ in 0...64)
			schedule.push(0);
	}

	/** Hash the complete input, including SHA-256 padding and bit length. */
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

	/** Fold one complete 64-byte block into the running digest state. */
	function compress(input:Bytes, offset:Int):Void {
		for (index in 0...16)
			schedule[index] = readBigEndian(input, offset + index * 4);
		for (index in 16...64) {
			final previous2 = schedule[index - 2];
			final previous15 = schedule[index - 15];
			final gamma1 = rotateRight(previous2, 17) ^ rotateRight(previous2, 19) ^ (previous2 >>> 10);
			final gamma0 = rotateRight(previous15, 7) ^ rotateRight(previous15, 18) ^ (previous15 >>> 3);
			schedule[index] = gamma1 + schedule[index - 7] + gamma0 + schedule[index - 16];
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
			final first = h + sigma1 + choose + roundConstants[index] + schedule[index];
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

	/** Rotate one 32-bit Haxe integer right by a fixed SHA-256 amount. */
	static inline function rotateRight(value:Int, amount:Int):Int
		return (value >>> amount) | (value << (32 - amount));

	/** Read one unsigned big-endian word without relying on `Bytes.getInt32`. */
	static inline function readBigEndian(input:Bytes, offset:Int):Int
		return (input.get(offset) << 24) | (input.get(offset + 1) << 16) | (input.get(offset + 2) << 8) | input.get(offset + 3);

	/** Write the low 32 bits of one word in SHA-256 byte order. */
	static inline function writeBigEndian(output:Bytes, offset:Int, value:Int):Void {
		output.set(offset, value >>> 24);
		output.set(offset + 1, value >>> 16);
		output.set(offset + 2, value >>> 8);
		output.set(offset + 3, value);
	}
}
