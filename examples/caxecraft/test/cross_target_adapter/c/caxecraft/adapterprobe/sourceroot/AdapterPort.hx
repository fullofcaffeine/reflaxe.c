package caxecraft.adapterprobe.sourceroot;

import c.IntConvert;
import c.UInt8;

/** Native-C source-root implementation using the exact byte conversion API. */
inline function normalize(value:Int):Int {
	final normalized:UInt8 = IntConvert.modulo(value);
	return IntConvert.exact(normalized);
}
