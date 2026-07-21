package caxecraft.gameplay;

/** Small fixed-step health rules shared by the native game and fast probes. */
final class PlayerVitals {
	/** Six health points are presented as three hearts with half-heart steps. */
	public static inline final MAX_HEALTH:Int = 6;

	public static inline final ATTACK_SAFE_TICKS:Int = 20;

	public static inline function start():PlayerVitalsState
		return make(MAX_HEALTH, 0);

	/** Restore a validated health value, as used by saves and deterministic pilots. */
	public static inline function startAt(health:Int):PlayerVitalsState
		return make(health, 0);

	/**
	 * Advance one 50 ms tick. `safeTicks` is the short protected period after a
	 * hit, so a single enemy cannot drain every heart during one overlap.
	 */
	public static function step(state:PlayerVitalsState):PlayerVitalsState {
		if (isDefeated(state))
			return state;
		if (state.safeTicks > 0)
			return make(state.health, state.safeTicks - 1);
		return state;
	}

	/** Apply one explicit enemy impact, respecting the fixed-step safe period. */
	public static function applyAttack(state:PlayerVitalsState, attacked:Bool):PlayerVitalsState {
		if (!attacked || isDefeated(state) || state.safeTicks > 0)
			return state;
		return make(state.health - 1, ATTACK_SAFE_TICKS);
	}

	public static inline function isDefeated(state:PlayerVitalsState):Bool
		return state.health <= 0;

	public static inline function isFull(state:PlayerVitalsState):Bool
		return state.health >= MAX_HEALTH;

	/** Add a positive amount without exceeding the three-heart maximum. */
	public static function recover(state:PlayerVitalsState, amount:Int):PlayerVitalsState {
		if (amount <= 0 || isDefeated(state) || isFull(state))
			return state;
		return make(state.health + amount, state.safeTicks);
	}

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
