package caxecraft.app;

import caxecraft.content.RuntimeContentPack.RuntimeModelPresentation;
import caxecraft.content.RuntimeContentPack.RuntimeVoxelClip;

/**
	Selects visual voxel frames from generic object-state transitions.

	CaxeFlow remains authoritative for the current state and collision. This
	presentation owner only remembers when the renderer first observed a new
	state. It uses committed simulation ticks, so pause freezes a clip and frame
	rate cannot change its duration.
**/
/** One complete model selected for the current presentation frame. */
typedef VoxelFrameSample = {
	/** Package-relative `.vox` path selected by the static model or clip. */
	final path:String;

	/** Cubic authoring resolution used to scale the model into world bounds. */
	final cellsPerAxis:Int;
}

/**
	Per-level playback state for content-authored interactive objects.

	A class is appropriate because each live level owns transition history across
	render frames. A new content generation clears that history, which makes an
	initial object hold its final authored pose instead of replaying activation.
**/
final class VoxelFrameAnimationPlayer {
	final objectIds:Array<String> = [];
	final stateIds:Array<String> = [];
	final stateStartTicks:Array<Int> = [];
	var generation:Int = -1;

	/** Create an empty player with no object or native-resource ownership. */
	public function new() {}

	/**
		Select the complete voxel model for one object on this render frame.

		The first observation of a level returns the clip's final frame. A later
		state change starts at frame zero and advances once through the clip.
	**/
	public function sample(objectId:String, stateId:String, generationId:Int, completedTicks:Int,
			presentation:RuntimeModelPresentation):Null<VoxelFrameSample> {
		if (generation != generationId) {
			objectIds.resize(0);
			stateIds.resize(0);
			stateStartTicks.resize(0);
			generation = generationId;
		}
		var index = find(objectId);
		var initialObservation = false;
		if (index < 0) {
			index = objectIds.length;
			objectIds.push(objectId);
			stateIds.push(stateId);
			stateStartTicks.push(completedTicks);
			initialObservation = true;
		} else if (stateIds[index] != stateId) {
			stateIds[index] = stateId;
			stateStartTicks[index] = completedTicks;
		}
		return switch presentation {
			case NoRuntimeModel: null;
			case RuntimeVoxelModel(path, cellsPerAxis): {path: path, cellsPerAxis: cellsPerAxis};
			case RuntimeVoxelAnimation(clip):
				if (initialObservation) frame(clip, clip.frameCount() - 1); else frame(clip, frameIndex(clip, completedTicks - stateStartTicks[index]));
		};
	}

	/** Find one object instance in the current generation's bounded state. */
	function find(objectId:String):Int {
		for (index in 0...objectIds.length)
			if (objectIds[index] == objectId)
				return index;
		return -1;
	}

	/** Select one frame index, holding the last frame after the clip completes. */
	static function frameIndex(clip:RuntimeVoxelClip, elapsedTicks:Int):Int {
		var remaining = elapsedTicks < 0 ? 0 : elapsedTicks;
		for (index in 0...clip.frameCount()) {
			final duration = clip.durationAt(index);
			if (remaining < duration)
				return index;
			remaining -= duration;
		}
		return clip.frameCount() - 1;
	}

	/** Copy one validated clip selection into the small renderer-facing value. */
	static inline function frame(clip:RuntimeVoxelClip, index:Int):VoxelFrameSample
		return {path: clip.pathAt(index), cellsPerAxis: clip.cellsPerAxis()};
}
