package caxecraft.content.hosted;

/**
	Declares the exact POSIX calls used by the Haxe-authored package reader.

	This is a typed metal boundary, not a platform implementation: Haxe owns path
	traversal, descriptor cleanup, size checks, exact reads, and diagnostics.
	System headers own only their ABI declarations and constants. The current
	hosted slice is intentionally limited to 64-bit POSIX targets and is checked
	by strict native ABI assertions in the focused package-store test.
**/
@:c.include("fcntl.h", c.IncludeKind.System)
@:c.include("errno.h", c.IncludeKind.System)
@:c.include("sys/stat.h", c.IncludeKind.System)
@:c.include("unistd.h", c.IncludeKind.System)
extern class PosixSystem {
	/**
		Read the calling thread's current POSIX error number immediately.

		`errno` may be a system-header macro rather than a linkable variable.
		Keeping it as a read-only imported expression lets the header preserve
		that platform detail while Haxe still owns error classification.
	**/
	@:c.constant
	@:c.name("errno")
	public static var errorNumber(default, never):Int;

	/** Error number reported when a named entry does not exist. */
	@:c.constant
	@:c.name("ENOENT")
	public static var noEntryError(default, never):Int;

	/** Error number reported when a no-follow open encounters a symbolic link. */
	@:c.constant
	@:c.name("ELOOP")
	public static var symbolicLinkError(default, never):Int;

	/** Error number reported when a required path component is not a directory. */
	@:c.constant
	@:c.name("ENOTDIR")
	public static var notDirectoryError(default, never):Int;

	/** Error number reported when a signal interrupts a restartable operation. */
	@:c.constant
	@:c.name("EINTR")
	public static var interruptedError(default, never):Int;

	/** Open for reading only. */
	@:c.constant
	@:c.name("O_RDONLY")
	public static var readOnly(default, never):Int;

	/** Mark a descriptor close-on-exec. */
	@:c.constant
	@:c.name("O_CLOEXEC")
	public static var closeOnExec(default, never):Int;

	/** Reject a symbolic link at the component being opened. */
	@:c.constant
	@:c.name("O_NOFOLLOW")
	public static var noFollow(default, never):Int;

	/** Require the opened component to be a directory. */
	@:c.constant
	@:c.name("O_DIRECTORY")
	public static var directory(default, never):Int;

	/** Prevent special files from blocking the package reader during admission. */
	@:c.constant
	@:c.name("O_NONBLOCK")
	public static var nonBlocking(default, never):Int;

	/** Inspect a failed component without following its final symbolic link. */
	@:c.constant
	@:c.name("AT_SYMLINK_NOFOLLOW")
	public static var atSymlinkNoFollow(default, never):Int;

	/** Mask selecting the file-kind bits in `mode_t`. */
	@:c.constant
	@:c.name("S_IFMT")
	public static var fileKindMask(default, never):PosixMode;

	/** File-kind value for a directory. */
	@:c.constant
	@:c.name("S_IFDIR")
	public static var directoryKind(default, never):PosixMode;

	/** File-kind value for a regular file. */
	@:c.constant
	@:c.name("S_IFREG")
	public static var regularFileKind(default, never):PosixMode;

	/** File-kind value for a symbolic link observed without following it. */
	@:c.constant
	@:c.name("S_IFLNK")
	public static var symbolicLinkKind(default, never):PosixMode;

	/**
		Open one root path through the variadic POSIX prototype.

		No optional mode argument is supplied because the flags never create a
		file. `@:c.variadic` records that header fact while admitting only this
		exact fixed-prefix call.
	**/
	@:c.variadic
	@:c.name("open")
	public static function openPath(path:c.CStringBufferRef, flags:Int):Int;

	/** Open one child relative to an already confined directory descriptor. */
	@:c.variadic
	@:c.name("openat")
	public static function openAt(directoryDescriptor:Int, path:c.CStringBufferRef, flags:Int):Int;

	/** Close one descriptor exactly once. */
	@:c.name("close")
	public static function closeDescriptor(descriptor:Int):Int;

	/** Fill one initialized caller-owned `struct stat` from a descriptor. */
	@:c.name("fstat")
	public static function inspectDescriptor(descriptor:Int, output:c.Ref<PosixStat>):Int;

	/** Inspect one child without following its final symbolic link. */
	@:c.name("fstatat")
	public static function inspectAt(directoryDescriptor:Int, path:c.CStringBufferRef, output:c.Ref<PosixStat>, flags:Int):Int;

	/**
		Read at most `count` bytes into one fixed-array element address.

		The 64-bit carriers intentionally match the admitted hosted POSIX targets'
		`size_t` and `ssize_t`. The focused native harness asserts those ABI facts
		before this declaration is accepted as product evidence.
	**/
	@:c.name("read")
	public static function readBytes(descriptor:Int, output:c.Ref<c.UInt8>, count:c.UInt64):c.Int64;
}
