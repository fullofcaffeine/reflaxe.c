package caxecraft.content.hosted;

/**
	Models the system header's `mode_t` file-kind mask.

	Darwin currently uses a 16-bit carrier while the admitted Linux hosted ABI
	uses 32 bits. The Haxe condition selects matching exact-width semantics, and
	the native ABI harness independently checks the selection against `mode_t`.
**/
#if caxecraft_posix_darwin
typedef PosixMode = c.UInt16;
#else
typedef PosixMode = c.UInt32;
#end
