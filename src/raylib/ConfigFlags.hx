package raylib;

#if c
/** Type-safe bit set for raylib window/configuration flags. */
enum abstract ConfigFlags(Int) to Int {
	var None = 0;
	var VsyncHint = 64;
	var FullscreenMode = 2;
	var WindowResizable = 4;
	var WindowUndecorated = 8;
	var WindowHidden = 128;
	var WindowMinimized = 512;
	var WindowMaximized = 1024;
	var WindowUnfocused = 2048;
	var WindowTopmost = 4096;
	var WindowAlwaysRun = 256;
	var WindowTransparent = 16;
	var WindowHighDpi = 8192;
	var WindowMousePassthrough = 16384;
	var BorderlessWindowedMode = 32768;
	var Msaa4xHint = 32;
	var InterlacedHint = 65536;

	@:op(A | B)
	public inline function or(other:ConfigFlags):ConfigFlags
		return new ConfigFlags(intOr(this, unwrap(other)));

	@:op(A & B)
	public inline function and(other:ConfigFlags):ConfigFlags
		return new ConfigFlags(intAnd(this, unwrap(other)));

	private static inline function unwrap(value:ConfigFlags):Int
		return value;

	private static inline function intOr(left:Int, right:Int):Int
		return left | right;

	private static inline function intAnd(left:Int, right:Int):Int
		return left & right;

	private inline function new(value:Int)
		this = value;
}
#end
