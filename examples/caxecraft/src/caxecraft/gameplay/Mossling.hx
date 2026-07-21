package caxecraft.gameplay;

import caxecraft.domain.World;
import caxecraft.domain.WorldCells;

/**
 * Bounded fixed-step movement for Caxecraft's first hostile creature.
 *
 * The Mossling follows a small square wander route, notices a nearby player,
 * advances along one horizontal axis per tick, and returns home when the
 * player leaves. It can climb one block and will not enter a column whose
 * surface is missing or too steep. This is a deliberately small state machine,
 * not a general pathfinding engine.
 */
final class Mossling {
	public static inline final MAX_HEALTH:Int = 3;
	public static inline final NOTICE_DISTANCE_SQUARED:Float = 36.0;
	public static inline final STRIKE_DISTANCE_SQUARED:Float = 9.0;
	public static inline final ATTACK_DISTANCE_SQUARED:Float = 1.96;
	public static inline final WINDUP_TICKS:Int = 8;
	public static inline final RECOVERY_TICKS:Int = 12;
	static inline final HOME_EPSILON_SQUARED:Float = 0.0001;
	static inline final STEP_DISTANCE:Float = 0.08;
	static inline final MINIMUM_STRIKE_AIM:Float = 0.35;
	static inline final WANDER_RADIUS:Float = 1.0;
	static inline final WANDER_CYCLE_TICKS:Int = 240;

	public static function start(cells:WorldCells, x:Float, z:Float):MosslingState {
		return {
			x: x,
			y: World.surfaceY(cells, Std.int(x), Std.int(z)) + 1.0,
			z: z,
			homeX: x,
			homeZ: z,
			modeCode: MosslingMode.Resting,
			phaseTicks: 0,
			health: MAX_HEALTH
		};
	}

	/**
	 * True on the one fixed tick when a completed, still-in-range wind-up hits.
	 * The application applies this event to player health before calling `step`.
	 */
	public static function attacksThisTick(state:MosslingState, playerX:Float, playerZ:Float):Bool {
		if (!isAlive(state) || mode(state) != MosslingMode.Windup || state.phaseTicks != 1)
			return false;
		return distanceSquared(state.x, state.z, playerX, playerZ) <= ATTACK_DISTANCE_SQUARED;
	}

	/** Advance exactly one 50 ms game tick. */
	public static function step(cells:WorldCells, state:MosslingState, playerX:Float, playerZ:Float, tickNumber:Int):MosslingState {
		if (!isAlive(state))
			return state;
		final currentMode = mode(state);
		final playerDistanceSquared = distanceSquared(state.x, state.z, playerX, playerZ);

		if (currentMode == MosslingMode.Windup) {
			// Leaving the marked range cancels the attack before its impact tick.
			if (playerDistanceSquared > ATTACK_DISTANCE_SQUARED)
				return make(state.x, state.y, state.z, state.homeX, state.homeZ, MosslingMode.Chasing, 0, state.health);
			if (state.phaseTicks > 1)
				return make(state.x, state.y, state.z, state.homeX, state.homeZ, currentMode, state.phaseTicks - 1, state.health);
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, MosslingMode.Recovering, RECOVERY_TICKS, state.health);
		}
		if (currentMode == MosslingMode.Recovering && state.phaseTicks > 1)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, currentMode, state.phaseTicks - 1, state.health);

		if (playerDistanceSquared <= ATTACK_DISTANCE_SQUARED)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, MosslingMode.Windup, WINDUP_TICKS, state.health);

		final homeDx = state.homeX - state.x;
		final homeDz = state.homeZ - state.z;
		final homeDistanceSquared = homeDx * homeDx + homeDz * homeDz;

		var nextMode = MosslingMode.Resting;
		var targetX = state.x;
		var targetZ = state.z;
		if (playerDistanceSquared <= NOTICE_DISTANCE_SQUARED) {
			nextMode = MosslingMode.Chasing;
			targetX = playerX;
			targetZ = playerZ;
		} else if ((currentMode == MosslingMode.Chasing || currentMode == MosslingMode.Returning)
			&& homeDistanceSquared > HOME_EPSILON_SQUARED) {
			nextMode = MosslingMode.Returning;
			targetX = state.homeX;
			targetZ = state.homeZ;
		} else {
			// One 12-second route is derived only from the fixed tick number. Two
			// runs with the same world and input therefore make the same choices.
			var cycleTick = tickNumber % WANDER_CYCLE_TICKS;
			if (cycleTick < 0)
				cycleTick += WANDER_CYCLE_TICKS;
			targetX = state.homeX;
			targetZ = state.homeZ;
			if (cycleTick >= 20 && cycleTick < 60)
				targetX += WANDER_RADIUS;
			else if (cycleTick >= 60 && cycleTick < 100) {
				targetX += WANDER_RADIUS;
				targetZ += WANDER_RADIUS;
			} else if (cycleTick >= 100 && cycleTick < 140) {
				targetX -= WANDER_RADIUS;
				targetZ += WANDER_RADIUS;
			} else if (cycleTick >= 140 && cycleTick < 180) {
				targetX -= WANDER_RADIUS;
				targetZ -= WANDER_RADIUS;
			} else if (cycleTick >= 180 && cycleTick < 220)
				targetZ -= WANDER_RADIUS;

			final wanderDx = targetX - state.x;
			final wanderDz = targetZ - state.z;
			if (wanderDx * wanderDx + wanderDz * wanderDz > HOME_EPSILON_SQUARED)
				nextMode = MosslingMode.Wandering;
		}

		if (nextMode == MosslingMode.Resting)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, nextMode, 0, state.health);

		var candidateX = state.x;
		var candidateZ = state.z;
		final dx = targetX - state.x;
		final dz = targetZ - state.z;
		if (absolute(dx) >= absolute(dz))
			candidateX += boundedStep(dx);
		else
			candidateZ += boundedStep(dz);

		final surface = World.surfaceY(cells, Std.int(candidateX), Std.int(candidateZ));
		final candidateY = surface + 1.0;
		if (surface < 0)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, nextMode, 0, state.health);
		if (absolute(candidateY - state.y) > 1.0)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, nextMode, 0, state.health);
		return make(candidateX, candidateY, candidateZ, state.homeX, state.homeZ, nextMode, 0, state.health);
	}

	/**
	 * Whether one sword swing reaches the living Mossling in front of the player.
	 *
	 * The dot product is a multiplication-and-addition test for “in front”. It
	 * keeps combat deterministic without asking a renderer or physics engine to
	 * perform the hit test.
	 */
	public static function canStrike(state:MosslingState, playerX:Float, playerZ:Float, lookX:Float, lookZ:Float):Bool {
		if (!isAlive(state))
			return false;
		final dx = state.x - playerX;
		final dz = state.z - playerZ;
		if (dx * dx + dz * dz > STRIKE_DISTANCE_SQUARED)
			return false;
		return dx * lookX + dz * lookZ >= MINIMUM_STRIKE_AIM;
	}

	/** Apply one point of sword damage. Defeated state remains stable. */
	public static function strike(state:MosslingState):MosslingState {
		var nextHealth = state.health;
		if (nextHealth > 0)
			nextHealth--;
		final currentMode = mode(state);
		return make(state.x, state.y, state.z, state.homeX, state.homeZ, currentMode, state.phaseTicks, nextHealth);
	}

	public static inline function isAlive(state:MosslingState):Bool
		return state.health > 0;

	/** Recover the closed movement mode from its validated record carrier. */
	public static inline function mode(state:MosslingState):MosslingMode
		return MosslingMode.fromCode(state.modeCode);

	static inline function make(x:Float, y:Float, z:Float, homeX:Float, homeZ:Float, mode:MosslingMode, phaseTicks:Int, health:Int):MosslingState
		return {
			x: x,
			y: y,
			z: z,
			homeX: homeX,
			homeZ: homeZ,
			modeCode: mode,
			phaseTicks: phaseTicks,
			health: health
		};

	static inline function distanceSquared(leftX:Float, leftZ:Float, rightX:Float, rightZ:Float):Float {
		final dx = rightX - leftX;
		final dz = rightZ - leftZ;
		return dx * dx + dz * dz;
	}

	static inline function boundedStep(delta:Float):Float {
		if (delta > STEP_DISTANCE)
			return STEP_DISTANCE;
		if (delta < -STEP_DISTANCE)
			return -STEP_DISTANCE;
		return delta;
	}

	static inline function absolute(value:Float):Float
		return value < 0.0 ? -value : value;
}
