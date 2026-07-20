package raylib;

#if c
private typedef RawColor = raylib.raw.Color;

/** A zero-allocation semantic view of raylib's four-byte `Color`. */
abstract Color(RawColor) from RawColor to RawColor {
	/**
	 * Creates an RGBA color directly.
	 *
	 * Literal channels are checked against the imported `c.UInt8` fields by the
	 * compiler. Dynamic `Int` channels are deliberately rejected until a named
	 * checked or clamping conversion policy is admitted.
	 */
	public static inline function rgba(r:Int, g:Int, b:Int, a:Int = 255):Color
		return new Color(c.StructInit.make({
			r: r,
			g: g,
			b: b,
			a: a
		}));

	private inline function new(value:RawColor)
		this = value;
}
#end
