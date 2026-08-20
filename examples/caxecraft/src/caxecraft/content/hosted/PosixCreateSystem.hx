package caxecraft.content.hosted;

/**
	Imports the one fixed-arity shim needed for exclusive POSIX file creation.

	`openat` receives a mode only when `O_CREAT` is present, but haxe.c correctly
	rejects calls with optional C variadic arguments. The local header converts
	that reviewed call shape into an ordinary fixed signature. Haxe still owns
	the flags, mode, path confinement, descriptor lifetime, and every outcome.
**/
@:c.include("caxecraft_posix_openat.h", c.IncludeKind.Local)
extern class PosixCreateSystem {
	/** Open one child through the reviewed four-argument `openat` call shape. */
	@:c.name("caxecraft_posix_openat_with_mode")
	public static function openAtWithMode(directoryDescriptor:Int, path:c.CStringBufferRef, flags:Int, mode:PosixMode):Int;
}
