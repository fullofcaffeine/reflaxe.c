package caxecraft.content;

import haxe.io.Bytes;

/** Compute the digest of the exact bytes verified during content publication. */
function runtimeSha256(input:Bytes):Bytes
	return haxe.crypto.Sha256.make(input);

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
