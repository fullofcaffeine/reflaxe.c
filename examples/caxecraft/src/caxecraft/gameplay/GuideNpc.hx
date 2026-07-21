package caxecraft.gameplay;

import caxecraft.domain.World;
import caxecraft.domain.WorldCells;

/** Deterministic rules for Nia, the first friendly Caxecraft guide. */
final class GuideNpc {
	public static inline final INTERACTION_DISTANCE_SQUARED:Float = 12.25;

	/** Place Nia on the highest solid block in one world column. */
	public static function start(cells:WorldCells, x:Float, z:Float):GuideState {
		return {
			x: x,
			y: World.surfaceY(cells, Std.int(x), Std.int(z)) + 1.0,
			z: z,
			phaseCode: GuidePhase.Waiting
		};
	}

	public static function isInRange(state:GuideState, playerX:Float, playerZ:Float):Bool {
		final dx = state.x - playerX;
		final dz = state.z - playerZ;
		return dx * dx + dz * dz <= INTERACTION_DISTANCE_SQUARED;
	}

	/** True only for the conversation step that grants the one-time gift. */
	public static function sharesBerriesOnNextInteraction(state:GuideState):Bool
		return phase(state) == GuidePhase.Welcomed;

	/** Recover the closed phase from its validated record carrier. */
	public static inline function phase(state:GuideState):GuidePhase
		return GuidePhase.fromCode(state.phaseCode);

	/** Advance one step; repeated interaction after the gift is safely inert. */
	public static function interact(state:GuideState):GuideState {
		var nextPhase = phase(state);
		if (nextPhase == GuidePhase.Waiting)
			nextPhase = GuidePhase.Welcomed;
		else if (nextPhase == GuidePhase.Welcomed)
			nextPhase = GuidePhase.SharedBerries;
		return {
			x: state.x,
			y: state.y,
			z: state.z,
			phaseCode: nextPhase
		};
	}
}
