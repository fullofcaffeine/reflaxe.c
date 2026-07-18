package caxecraft.domain;

/** Renderer-independent input sampled once for a fixed simulation tick. */
typedef StepInput = {
	final moveX:Float;
	final moveZ:Float;
	final jump:Bool;
}
