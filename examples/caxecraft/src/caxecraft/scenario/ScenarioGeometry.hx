package caxecraft.scenario;

/** Integer voxel coordinate. */
typedef VoxelPoint = {
	final x:Int;
	final y:Int;
	final z:Int;
}

/** Positive voxel dimensions. */
typedef VoxelSize = {
	final width:Int;
	final height:Int;
	final depth:Int;
}

/** Half-open voxel box: origin is included and origin + size is excluded. */
typedef VoxelBounds = {
	final origin:VoxelPoint;
	final size:VoxelSize;
}

/**
	Deterministic placement in thousandths of a block.

	The text format stores integers rather than locale-sensitive decimal text.
	`yawDegrees` is validated in the inclusive range 0 through 359.
**/
typedef ScenarioTransform = {
	final xMilli:Int;
	final yMilli:Int;
	final zMilli:Int;
	final yawDegrees:Int;
}
