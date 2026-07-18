package caxecraft.domain;

/**
	Result of a bounded voxel ray traversal.

	`hit` makes a miss explicit without a sentinel coordinate. On a hit, the
	previous coordinates identify the last empty cell and the normal points out
	of the entered face. `distance` is the ray parameter, so callers should pass a
	normalized direction when they need world-space distance.
**/
typedef RaycastHit = {
	final hit:Bool;
	final cellX:Int;
	final cellY:Int;
	final cellZ:Int;
	final previousX:Int;
	final previousY:Int;
	final previousZ:Int;
	final normalX:Int;
	final normalY:Int;
	final normalZ:Int;
	final distance:Float;
	final visited:Int;
}
