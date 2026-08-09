package caxecraft.app;

/**
	Defines the camera-relative order for transparent water geometry.

	Transparent faces blend with the color that is already in the frame. They
	must therefore be submitted from far to near. These small scalar functions
	keep that rule independent from Raylib, so the ordering contract has a fast
	deterministic test as well as the real native rendering proof.
**/
/** Return the squared camera-to-face distance without an unnecessary square root. */
function faceDepthSquared(eyeX:Float, eyeY:Float, eyeZ:Float, faceX:Float, faceY:Float, faceZ:Float):Float {
	final dx = faceX - eyeX;
	final dy = faceY - eyeY;
	final dz = faceZ - eyeZ;
	return dx * dx + dy * dy + dz * dz;
}

/**
	Compare two transparent faces for far-to-near submission.

	The stable key resolves equal-distance views. This makes a still camera
	produce the same bytes instead of letting the sort implementation choose an
	arbitrary order for ties.
**/
function compareDepth(leftDepth:Float, leftKey:Int, rightDepth:Float, rightKey:Int):Int {
	if (leftDepth > rightDepth)
		return -1;
	if (leftDepth < rightDepth)
		return 1;
	if (leftKey < rightKey)
		return -1;
	if (leftKey > rightKey)
		return 1;
	return 0;
}
