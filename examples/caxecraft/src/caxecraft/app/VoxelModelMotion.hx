package caxecraft.app;

import caxecraft.content.RuntimeContentPack.RuntimeModelMotion;

/**
	Calculates presentation-only movement for content-authored voxel states.

	The engine owns the small set of safe motion profiles. Content chooses a
	profile for each visual state, while simulation, collision, and puzzle rules
	remain unchanged.
**/
/**
	Return a deterministic lift for one content-selected model motion.

	The triangle pulse uses committed simulation ticks. Pausing therefore freezes
	the prop, and returning to a static state stops the motion immediately.
**/
function voxelModelLiftMilli(motion:RuntimeModelMotion, completedTicks:Int):Int {
	return switch motion {
		case StaticModel: 0;
		case PulseModel:
			final phase = completedTicks % 20;
			final rising = phase <= 10 ? phase : 20 - phase;
			rising * 12;
	};
}
