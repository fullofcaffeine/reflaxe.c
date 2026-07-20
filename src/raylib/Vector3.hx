package raylib;

#if c
private typedef RawVector3 = raylib.raw.Vector3;

/** A zero-allocation semantic view of raylib's by-value `Vector3`. */
abstract Vector3(RawVector3) from RawVector3 to RawVector3 {
	/** Direct binary32 constructor; emits one C compound literal. */
	public static inline function make(x:c.Float32, y:c.Float32, z:c.Float32):Vector3
		return new Vector3(c.StructInit.make({x: x, y: y, z: z}));

	/** Explicitly rounds ordinary Haxe `Float` values to C binary32. */
	public static inline function fromFloat(x:Float, y:Float, z:Float):Vector3
		return make(c.Float32.fromFloat(x), c.Float32.fromFloat(y), c.Float32.fromFloat(z));

	private inline function new(value:RawVector3)
		this = value;
}
#end
