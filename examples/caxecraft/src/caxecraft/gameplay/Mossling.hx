package caxecraft.gameplay;

import caxecraft.domain.World;
import caxecraft.domain.WorldCells;

/**
 * Bounded fixed-step movement for Caxecraft's first hostile creature.
 *
 * The Mossling notices a nearby player, advances along one horizontal axis per
 * tick, and returns home when the player leaves. It can climb one block and
 * will not enter a column whose surface is missing or too steep. This is a
 * deliberately small state machine, not a general pathfinding engine.
 */
final class Mossling {
	public static inline final NOTICE_DISTANCE_SQUARED:Float = 36.0;
	static inline final HOME_EPSILON_SQUARED:Float = 0.0001;
	static inline final STEP_DISTANCE:Float = 0.08;

	public static function start(cells:WorldCells, x:Float, z:Float):MosslingState {
		return {
			x: x,
			y: World.surfaceY(cells, Std.int(x), Std.int(z)) + 1.0,
			z: z,
			homeX: x,
			homeZ: z,
			modeCode: MosslingMode.Resting
		};
	}

	/** Advance exactly one 50 ms game tick. */
	public static function step(cells:WorldCells, state:MosslingState, playerX:Float, playerZ:Float):MosslingState {
		final playerDx = playerX - state.x;
		final playerDz = playerZ - state.z;
		final playerDistanceSquared = playerDx * playerDx + playerDz * playerDz;
		final homeDx = state.homeX - state.x;
		final homeDz = state.homeZ - state.z;
		final homeDistanceSquared = homeDx * homeDx + homeDz * homeDz;

		var mode = MosslingMode.Resting;
		var targetX = state.x;
		var targetZ = state.z;
		if (playerDistanceSquared <= NOTICE_DISTANCE_SQUARED) {
			mode = MosslingMode.Chasing;
			targetX = playerX;
			targetZ = playerZ;
		} else if (homeDistanceSquared > HOME_EPSILON_SQUARED) {
			mode = MosslingMode.Returning;
			targetX = state.homeX;
			targetZ = state.homeZ;
		}

		if (mode == MosslingMode.Resting)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, mode);

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
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, mode);
		if (absolute(candidateY - state.y) > 1.0)
			return make(state.x, state.y, state.z, state.homeX, state.homeZ, mode);
		return make(candidateX, candidateY, candidateZ, state.homeX, state.homeZ, mode);
	}

	/** Recover the closed movement mode from its validated record carrier. */
	public static inline function mode(state:MosslingState):MosslingMode
		return MosslingMode.fromCode(state.modeCode);

	static inline function make(x:Float, y:Float, z:Float, homeX:Float, homeZ:Float, mode:MosslingMode):MosslingState
		return {
			x: x,
			y: y,
			z: z,
			homeX: homeX,
			homeZ: homeZ,
			modeCode: mode
		};

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
