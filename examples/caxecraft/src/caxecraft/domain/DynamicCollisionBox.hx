package caxecraft.domain;

/**
	One runtime-authored axis-aligned collision box.

	Terrain remains in the fixed voxel world. Stateful doors, gates, and similar
	objects use these boxes because their collision can change without rewriting
	the authored terrain or rebuilding the game. Values use world blocks and a
	half-open maximum edge, matching the character body's separation checks.
**/
typedef DynamicCollisionBox = {
	/** Included lower world-space X edge. */
	final minimumX:Float;

	/** Excluded upper world-space X edge. */
	final maximumX:Float;

	/** Included lower world-space Y edge. */
	final minimumY:Float;

	/** Excluded upper world-space Y edge. */
	final maximumY:Float;

	/** Included lower world-space Z edge. */
	final minimumZ:Float;

	/** Excluded upper world-space Z edge. */
	final maximumZ:Float;
}
