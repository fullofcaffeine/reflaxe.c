package caxecraft.scenario;

import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;

/** Finite voxel volume and its deterministic z/y/x run encoding. */
typedef ScenarioWorld = {
	final size:VoxelSize;
	final palette:Array<BlockPaletteEntry>;
	final chunks:Array<VoxelChunk>;
}

/** Local compact code mapped to a registered semantic block type. */
typedef BlockPaletteEntry = {
	final code:Int;
	final blockType:ContentId;
}

/** One bounded rectangular group of z/y/x-ordered voxel runs. */
typedef VoxelChunk = {
	final id:ScenarioId;
	final origin:VoxelPoint;
	final size:VoxelSize;
	final runs:Array<VoxelRun>;
}

/** Repeat one palette code for `count` cells. */
typedef VoxelRun = {
	final paletteCode:Int;
	final count:Int;
}
