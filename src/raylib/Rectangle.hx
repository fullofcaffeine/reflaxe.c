package raylib;

// RaylibHx currently models raylib's C ABI, so this public type deliberately
// exists only for the `c` custom target. This is a compile-time package
// boundary, not a run-time branch in Caxecraft; see docs/raylib-semantic-core.md.
#if c
private typedef RawRectangle = raylib.raw.Rectangle;

/** A zero-allocation semantic view of raylib's by-value `Rectangle`. */
abstract Rectangle(RawRectangle) from RawRectangle to RawRectangle {
	/** Direct binary32 constructor; emits one C aggregate value. */
	public static inline function make(x:c.Float32, y:c.Float32, width:c.Float32, height:c.Float32):Rectangle
		return new Rectangle(c.StructInit.make({
			x: x,
			y: y,
			width: width,
			height: height
		}));

	/** Explicitly rounds ordinary Haxe `Float` values to C binary32. */
	public static inline function fromFloat(x:Float, y:Float, width:Float, height:Float):Rectangle
		return make(c.Float32.fromFloat(x), c.Float32.fromFloat(y), c.Float32.fromFloat(width), c.Float32.fromFloat(height));

	private inline function new(value:RawRectangle)
		this = value;
}
#end
