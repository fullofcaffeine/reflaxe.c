package raylib;

#if c
private typedef RawVector2 = raylib.raw.Vector2;

/** A zero-allocation semantic view of raylib's by-value `Vector2`. */
abstract Vector2(RawVector2) from RawVector2 to RawVector2 {
	/** Direct binary32 constructor; emits one C compound literal. */
	public static inline function make(x:c.Float32, y:c.Float32):Vector2
		return new Vector2(c.StructInit.make({x: x, y: y}));

	/** Explicitly rounds ordinary Haxe `Float` values to C binary32. */
	public static inline function fromFloat(x:Float, y:Float):Vector2
		return make(c.Float32.fromFloat(x), c.Float32.fromFloat(y));

	private inline function new(value:RawVector2)
		this = value;
}
#end
