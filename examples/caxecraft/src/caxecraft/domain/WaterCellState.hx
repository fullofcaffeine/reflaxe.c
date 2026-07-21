package caxecraft.domain;

/**
	The complete typed meaning of one voxel as seen by water simulation.

	`Blocked` covers solid terrain and the finite world's sealed boundary.
	`InvalidStorage` keeps a malformed byte visible and immovable instead of
	quietly treating corrupt data as air or water.
**/
enum WaterCellState {
	Empty;
	Blocked;
	Source;
	Flowing(level:WaterLevel, falling:Bool);
	InvalidStorage(code:Int);
}
