package caxecraft.gameplay;

/**
	Names the bounded storage reserved for authored world-item state.

	`CAPACITY` is an engine limit, not the item count of the bundled Adventure.
	A runtime content loader must reject a level that exceeds it before publishing
	the new session. The phantom class also keeps this fixed array distinct from
	voxel and water work storage in Haxe and generated C; it creates no object at
	runtime.
**/
final class AuthoredItemSlots {
	/** Maximum authored item placements in one currently loaded level. */
	public static inline final CAPACITY:Int = 256;
}
