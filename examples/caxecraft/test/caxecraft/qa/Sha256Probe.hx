package caxecraft.qa;

import haxe.crypto.Sha256;
import haxe.io.Bytes;

/**
	Proves the pinned standard SHA-256 worker on Eval and generated native C.

	Public known vectors guard recognizable algorithm results. A deterministic
	binary corpus then publishes one 32-bit digest trace that the Python runner
	recomputes independently with `hashlib`, including the one-block/two-block
	padding boundaries.
**/
/** Zero when every public known vector matches. */
var observed:Int = 0;

/** Source-ordered rolling trace of every digest byte in the differential corpus. */
var digestTrace:Int = 0;

/** Number of digest cases included in the trace. */
var digestCases:Int = 0;

/** Run the same Haxe assertions on Eval or through the generated native entry. */
function main():Void {
	observed = selfCheck();
	#if !c
	Sys.println(observed);
	Sys.println(digestTrace);
	Sys.println(digestCases);
	#end
}

/** Return zero only when known vectors and the differential corpus complete. */
function selfCheck():Int {
	final knownInputs = ["", "abc", "The quick brown fox jumps over the lazy dog"];
	final knownWords = [
		[
			0xE3B0C442,
			0x98FC1C14,
			0x9AFBF4C8,
			0x996FB924,
			0x27AE41E4,
			0x649B934C,
			0xA495991B,
			0x7852B855
		],
		[
			0xBA7816BF,
			0x8F01CFEA,
			0x414140DE,
			0x5DAE2223,
			0xB00361A3,
			0x96177A9C,
			0xB410FF61,
			0xF20015AD
		],
		[
			0xD7A8FBB3,
			0x07D78094,
			0x69CA9ABC,
			0xB0082E4F,
			0x8D5651E4,
			0x6D3CDB76,
			0x2D02D0BF,
			0x37C9E592
		]
	];
	for (index in 0...knownInputs.length) {
		final digest = Sha256.make(Bytes.ofString(knownInputs[index]));
		if (!matchesWords(digest, knownWords[index]))
			return index + 1;
		if (!includeDigest(digest))
			return 100;
	}
	for (length in [1, 2, 3, 54, 55, 56, 57, 63, 64, 65, 119, 120, 121, 127, 128, 129, 1024, 65537]) {
		final input = Bytes.alloc(length);
		for (index in 0...length)
			input.set(index, (index * 131 + length * 17) & 0xFF);
		if (!includeDigest(Sha256.make(input)))
			return 100;
	}
	return 0;
}

/** Compare one digest with eight independently authored big-endian words. */
function matchesWords(digest:Bytes, expected:Array<Int>):Bool {
	if (digest.length != 32 || expected.length != 8)
		return false;
	for (word in 0...8) {
		final offset = word * 4;
		final actual = (digest.get(offset) << 24) | (digest.get(offset + 1) << 16) | (digest.get(offset + 2) << 8) | digest.get(offset + 3);
		if (actual != expected[word])
			return false;
	}
	return true;
}

/** Fold one exact digest into the target-independent trace and report whether it had 32 bytes. */
function includeDigest(digest:Bytes):Bool {
	if (digest.length != 32)
		return false;
	for (index in 0...digest.length)
		digestTrace = digestTrace * 31 + digest.get(index);
	digestCases++;
	return true;
}
