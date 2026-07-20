package raylib;

#if c
private typedef RawBoundingBox = raylib.raw.BoundingBox;

/** Direct by-value axis-aligned bounds; no allocation or runtime helper. */
abstract BoundingBox(RawBoundingBox) from RawBoundingBox to RawBoundingBox {
	public static inline function make(min:Vector3, max:Vector3):BoundingBox
		return new BoundingBox(c.StructInit.make({min: min, max: max}));

	private inline function new(value:RawBoundingBox)
		this = value;
}
#end
