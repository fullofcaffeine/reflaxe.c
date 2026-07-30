package caxecraft.content.hosted;

/**
	Names the two POSIX `struct timespec` fields used in file identity checks.

	The C system header owns layout. Haxe sees only the exact signed 64-bit facts
	needed to detect a file changing between inspection and reading.
**/
@:c.layout(c.Layout.Struct)
@:c.name("timespec")
@:c.include("time.h", c.IncludeKind.System)
extern class PosixTimespec {
	/** Whole seconds in the POSIX timestamp. */
	@:c.name("tv_sec")
	public var seconds:PosixTime;

	/** Nanoseconds within `seconds`. */
	@:c.name("tv_nsec")
	public var nanoseconds:c.IntPtr;
}
