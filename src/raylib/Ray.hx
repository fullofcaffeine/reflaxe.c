package raylib;

#if c
private typedef RawRay = raylib.raw.Ray;

/** Direct by-value ray; no allocation or runtime helper. */
abstract Ray(RawRay) from RawRay to RawRay {
	public static inline function make(position:Vector3, direction:Vector3):Ray
		return new Ray(c.StructInit.make({position: position, direction: direction}));

	private inline function new(value:RawRay)
		this = value;
}
#end
