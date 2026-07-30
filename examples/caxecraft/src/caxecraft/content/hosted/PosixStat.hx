package caxecraft.content.hosted;

/**
	Projects the bounded `struct stat` facts needed by the package reader.

	The complete structure remains owned by `<sys/stat.h>`. This Haxe declaration
	names only fields that participate in confinement, type checks, size limits,
	and changed-during-read detection; `fstat` initializes the whole value.
**/
@:c.layout(c.Layout.Struct)
@:c.name("stat")
@:c.include("sys/stat.h", c.IncludeKind.System)
extern class PosixStat {
	/** Filesystem device identity. */
	@:c.name("st_dev")
	public var device:PosixDeviceId;

	/** File identity within `device`. */
	@:c.name("st_ino")
	public var inode:PosixInodeId;

	/** POSIX file-kind and permission bits. */
	@:c.name("st_mode")
	public var mode:PosixMode;

	/** Exact byte size reported by the opened descriptor. */
	@:c.name("st_size")
	public var size:PosixOffset;

	/** Last modification timestamp used to reject a changing file. */
	#if caxecraft_posix_darwin
	@:c.name("st_mtimespec")
	#else
	@:c.name("st_mtim")
	#end
	public var modified:PosixTimespec;
}
