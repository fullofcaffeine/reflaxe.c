package raylib;

// RaylibHx currently models raylib's C ABI, so this public type deliberately
// exists only for the `c` custom target. Portable game code stays above the
// renderer adapter; see docs/raylib-semantic-core.md.
#if c
private typedef RawTexture2D = raylib.raw.Texture2D;

/**
 * Read-only Haxe view of raylib's native `Texture2D` handle.
 *
 * This value is not an owning Haxe object. A successful raw `LoadTexture`
 * call creates exactly one GPU resource, and every copied `Texture2D` value
 * refers to that same resource. Its owner must call raw `UnloadTexture`
 * exactly once on the render thread before `CloseWindow`; no finalizer or
 * garbage collector performs that cleanup.
 */
abstract Texture2D(RawTexture2D) from RawTexture2D to RawTexture2D {
	public var width(get, never):Int;
	public var height(get, never):Int;

	private inline function get_width():Int
		return c.IntConvert.exact(this.width);

	private inline function get_height():Int
		return c.IntConvert.exact(this.height);
}
#end
