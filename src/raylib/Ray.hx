package raylib;

#if c
private typedef RawRay = raylib.raw.Ray;

/**
 * Direct by-value ray with read-only semantic component access.
 *
 * Raylib returns a `Ray` by value from screen picking. Exposing its position
 * and direction as `Vector3` values lets renderer adapters feed
 * target-neutral spatial logic without exposing `raylib.raw` declarations or
 * copying the C struct into a second representation.
 */
abstract Ray(RawRay) from RawRay to RawRay {
	/** World-space point where the ray starts. */
	public var position(get, never):Vector3;

	/** Unit-like world-space direction followed by the ray. */
	public var direction(get, never):Vector3;

	public static inline function make(position:Vector3, direction:Vector3):Ray
		return new Ray(c.StructInit.make({position: position, direction: direction}));

	private inline function get_position():Vector3
		return this.position;

	private inline function get_direction():Vector3
		return this.direction;

	private inline function new(value:RawRay)
		this = value;
}
#end
