package caxecraft.domain;

/**
	The finite set of voxel materials understood by the Caxecraft domain.

	The semantic value is an ordinary Haxe enum. `WorldStorage` owns the compact
	one-byte code used by the C world volume, so gameplay code cannot accidentally
	mix a material with an arbitrary integer.
**/
enum BlockKind {
	Air;
	Grass;
	Dirt;
	Stone;
	Bedrock;
}
