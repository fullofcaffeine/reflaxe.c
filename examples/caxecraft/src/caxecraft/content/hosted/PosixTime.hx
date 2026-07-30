package caxecraft.content.hosted;

/**
	Models the system header's signed `time_t` timestamp carrier.

	The admitted hosted ABI uses pointer-width signed time. Keeping the typedef
	behind a local Haxe name keeps the application readable, while the native ABI
	harness independently checks the carrier against the real header typedef.
**/
typedef PosixTime = c.IntPtr;
