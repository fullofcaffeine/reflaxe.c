package caxecraft.adapterprobe;

import caxecraft.adapterprobe.sourceroot.AdapterPort.normalize as normalizeFromSourceRoot;
#if c
import c.IntConvert;
import c.UInt8;
#end

/**
	Three bounded adapter strategies around one observable gameplay calculation.

	Each path normalizes a target carrier to one byte, then applies the same score
	rule. The probe keeps gameplay arithmetic identical so generated output can
	show only the source-layout and dispatch cost of each boundary choice.
**/
/** The smallest structural contract considered for runtime adapter injection. */
interface NormalizationPort {
	/** Normalize one positive value to the target's admitted byte carrier. */
	function normalize(value:Int):Int;
}

/** Apply the target-specific source-root module selected by the build. */
function sourceRootScore(value:Int):Int
	return score(normalizeFromSourceRoot(value));

/** Apply an injected structural interface selected by the composition root. */
function interfaceScore(port:NormalizationPort, value:Int):Int
	return score(port.normalize(value));

/** Apply one narrow compile-time branch in the same authored module. */
function conditionalScore(value:Int):Int {
	#if c
	final normalized:UInt8 = IntConvert.modulo(value);
	return score(IntConvert.exact(normalized));
	#else
	return score(value % 256);
	#end
}

/** Shared gameplay arithmetic whose result makes every adapter observable. */
inline function score(normalized:Int):Int
	return normalized * 7 + 3;
