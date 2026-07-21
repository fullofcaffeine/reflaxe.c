package caxecraft.gameplay;

/** Small fixed-step health rules shared by the native game and fast probes. */
final class PlayerVitals {
	/** Six health points are presented as three hearts with half-heart steps. */
	public static inline final MAX_HEALTH:Int = 6;

	public static inline final CONTACT_SAFE_TICKS:Int = 20;
	public static inline final CONTACT_DISTANCE_SQUARED:Float = 1.44;

	public static inline function start():PlayerVitalsState
		return make(MAX_HEALTH, 0);

	/**
	 * Advance one 50 ms tick and apply at most one point of contact damage.
	 * `safeTicks` prevents a nearby enemy from draining all health in one second.
	 */
	public static function step(state:PlayerVitalsState, playerX:Float, playerZ:Float, threatX:Float, threatZ:Float, threatActive:Bool):PlayerVitalsState {
		if (isDefeated(state))
			return state;
		if (state.safeTicks > 0)
			return make(state.health, state.safeTicks - 1);
		if (!threatActive)
			return state;
		final dx = threatX - playerX;
		final dz = threatZ - playerZ;
		if (dx * dx + dz * dz > CONTACT_DISTANCE_SQUARED)
			return state;
		return make(state.health - 1, CONTACT_SAFE_TICKS);
	}

	public static inline function isDefeated(state:PlayerVitalsState):Bool
		return state.health <= 0;

	/** Start again with full health after the player accepts the return prompt. */
	public static inline function revive(_state:PlayerVitalsState):PlayerVitalsState
		return start();

	static function make(health:Int, safeTicks:Int):PlayerVitalsState {
		var boundedHealth = health;
		if (boundedHealth < 0)
			boundedHealth = 0;
		var boundedSafeTicks = safeTicks;
		if (boundedSafeTicks < 0)
			boundedSafeTicks = 0;
		return {health: boundedHealth, safeTicks: boundedSafeTicks};
	}
}
