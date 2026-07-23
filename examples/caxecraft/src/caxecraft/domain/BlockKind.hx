package caxecraft.domain;

/**
	The finite set of voxel materials understood by the Caxecraft domain.

	The semantic value is an ordinary Haxe enum. `WorldStorage` owns the compact
	one-byte code used by the C world volume, so gameplay code cannot accidentally
	mix a material with an arbitrary integer.

	An algebraic enum is used instead of an `enum abstract` because gameplay owns
	semantic constructors, while the storage codec independently owns byte values
	and malformed-input checks. The current payload-free set still lowers to a
	private native C enum; choosing the semantic type does not require boxing.
**/
enum BlockKind {
	Air;
	Grass;
	Dirt;
	Stone;
	Bedrock;
	Sand;
	Wood;
	Leaves;
	Snow;
	Ash;
}
