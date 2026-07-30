package caxecraft.content.hosted;

/**
	Models the system header's `dev_t` file-device identity.

	The underlying exact carrier records the admitted host ABI for haxe.c
	semantics. `PosixStat` still reads the real header-owned field, and the native
	ABI harness independently checks that this carrier has the host's width and
	signedness.
**/
#if caxecraft_posix_darwin
typedef PosixDeviceId = c.Int32;
#else
typedef PosixDeviceId = c.UInt64;
#end
