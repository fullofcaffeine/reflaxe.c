package caxecraft.gameplay;

/** Deterministic spawn and pickup rules for the first visible enemy drop. */
final class BerryDrop {
	public static inline final PICKUP_DISTANCE_SQUARED:Float = 2.25;

	public static inline function none():BerryDropState
		return make(0.0, 0.0, 0.0, 0, false);

	public static function fromDefeatedMossling(state:MosslingState):BerryDropState
		return make(state.x, state.y + 0.24, state.z, 2, true);

	public static function isInRange(state:BerryDropState, playerX:Float, playerY:Float, playerZ:Float):Bool {
		if (!state.active)
			return false;
		final dx = state.x - playerX;
		final dy = state.y - playerY;
		final dz = state.z - playerZ;
		return dx * dx + dy * dy + dz * dz <= PICKUP_DISTANCE_SQUARED;
	}

	public static inline function collect(state:BerryDropState):BerryDropState
		return make(state.x, state.y, state.z, 0, false);

	static inline function make(x:Float, y:Float, z:Float, amount:Int, active:Bool):BerryDropState
		return {
			x: x,
			y: y,
			z: z,
			amount: amount,
			active: active
		};
}
