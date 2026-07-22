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

	/**
	 * Creates an RGBA color from values that may be computed at run time.
	 *
	 * Each channel is clamped to the byte range before the explicit C integer
	 * conversion. Use `rgba` for literal channels, where an out-of-range value is
	 * better reported at compile time; use this function for fades, animation,
	 * decoded settings, and other values that are known only while the program runs.
	 */
	public static inline function rgbaClamped(r:Int, g:Int, b:Int, a:Int = 255):Color {
		final red:c.UInt8 = c.IntConvert.modulo(clampChannel(r));
		final green:c.UInt8 = c.IntConvert.modulo(clampChannel(g));
		final blue:c.UInt8 = c.IntConvert.modulo(clampChannel(b));
		final alpha:c.UInt8 = c.IntConvert.modulo(clampChannel(a));
		return new Color(c.StructInit.make({
			r: red,
			g: green,
			b: blue,
			a: alpha
		}));
	}

	/** Keep the narrowing rule visible and shared by all four channel fields. */
	private static inline function clampChannel(value:Int):Int {
		if (value < 0)
			return 0;
		if (value > 255)
			return 255;
		return value;
	}

	private inline function new(value:RawColor)
		this = value;
}
#end
