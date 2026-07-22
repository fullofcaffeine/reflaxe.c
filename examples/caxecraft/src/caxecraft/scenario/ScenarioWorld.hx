package caxecraft.scenario;

import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;

/** Finite voxel volume, deterministic terrain runs, and authored fluid setup. */
typedef ScenarioWorld = {
	final size:VoxelSize;
	final palette:Array<BlockPaletteEntry>;
	final chunks:Array<VoxelChunk>;
	final fluids:Array<ScenarioFluid>;
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

/** One stable fluid declaration authored and saved independently of terrain. */
typedef ScenarioFluid = {
	final id:ScenarioId;
	final fluidType:ContentId;
	final placement:ScenarioFluidPlacement;
}

/**
	The two initial fluid shapes admitted by CAXEMAP 1.

	A source point continuously feeds simulation. An initial volume fills a
	bounded box once when the level loads; it may then flow, drain, or be saved as
	mutable world state. Keeping both explicit avoids treating every pool cell as
	a permanent source.
**/
enum ScenarioFluidPlacement {
	Source(point:VoxelPoint);
	InitialVolume(bounds:VoxelBounds);
}
