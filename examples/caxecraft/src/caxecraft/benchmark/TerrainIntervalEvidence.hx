package caxecraft.benchmark;

#if caxecraft_observe_terrain
import caxecraft.benchmark.IntervalObserver.IntervalEvent;

/**
 * Reports cumulative native observations outside the terrain interval.
 *
 * Haxe owns the report; the independent C observer only counts runtime entries.
 * Two snapshots retain warmup activity and let the host compute the measured
 * difference without resetting counters or hiding sticky observer errors.
 * This module is absent from ordinary play and uninstrumented timing builds.
 */
/** Closed phases prevent an arbitrary label from entering the report protocol. */
enum abstract TerrainIntervalPhase(String) to String {
	/** Immediately before the first terrain interval after the two warmup frames. */
	var Warmup = "warmup";

	/** After the last rendered frame, before native resources are released. */
	var Complete = "complete";
}

/**
 * Emit one exact snapshot after a region closes, never while timing terrain.
 *
 * UInt64 values remain decimal integers through Haxe formatting. Root counters
 * count API entry attempts; they do not prove balanced or successful root frames.
 * The host checks phases, validity, monotonic counters and completed-frame counts.
 */
function writeTerrainIntervalSnapshot(phase:TerrainIntervalPhase, frames:Int, portalFrames:Int):Void {
	Sys.println('CAXECRAFT_TERRAIN_INTERVAL={"schemaVersion":1,"phase":"${phase}","valid":${IntervalObserver.valid()},'
		+ '"frames":${frames},"portalFrames":${portalFrames},"completed":${IntervalObserver.completed()},'
		+ '"allocationAttempts":${IntervalObserver.count(AllocateAttempt)},"reallocationAttempts":${IntervalObserver.count(ReallocateAttempt)},'
		+ '"collectionAttempts":${IntervalObserver.count(CollectAttempt)},"rootPushAttempts":${IntervalObserver.count(RootPushAttempt)},'
		+ '"rootPopAttempts":${IntervalObserver.count(RootPopAttempt)}}');
}
#end
