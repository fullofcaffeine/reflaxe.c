package caxecraft.adapterprobe.sourceroot;

/** Portable source-root implementation of the byte-normalization boundary. */
inline function normalize(value:Int):Int
	return value % 256;
