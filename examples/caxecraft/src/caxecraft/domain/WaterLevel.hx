package caxecraft.domain;

/**
	A flowing cell's bounded distance from its strongest local water feed.

	`Full` is used by a vertical fall or its landing cell. Horizontal flow loses
	one step per voxel until `Thin`, after which it cannot spread farther. A source
	is a separate `WaterCellState`, so flowing water can never silently become a
	permanent source.
**/
enum abstract WaterLevel(Int) to Int {
	var Full = 0;
	var One = 1;
	var Two = 2;
	var Three = 3;
	var Four = 4;
	var Five = 5;
	var Six = 6;
	var Thin = 7;
}
