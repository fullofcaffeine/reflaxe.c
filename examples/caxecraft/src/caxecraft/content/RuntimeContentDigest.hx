package caxecraft.content;

import haxe.crypto.Sha256;
import haxe.io.Bytes;

/**
	Owns Caxecraft's package-digest boundary through Haxe's standard SHA-256 API.

	Callers share these small names so package verification and receipt writing
	cannot select different algorithms. The cryptographic implementation remains
	the pinned Haxe standard library's single source of truth.
**/
/** Return the standard SHA-256 digest for one immutable package byte sequence. */
function runtimeSha256(input:Bytes):Bytes
	return Sha256.make(input);

/** Return the package digest as the canonical lowercase 64-digit receipt. */
function runtimeSha256Hex(input:Bytes):String
	return runtimeSha256(input).toHex();
