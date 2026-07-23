package caxecraft.domain;

/** Feet-position, velocity, and contact state for the fixed-step controller. */
typedef CharacterBody = {
	final x:Float;
	final y:Float;
	final z:Float;
	final velocityX:Float;
	final velocityY:Float;
	final velocityZ:Float;
	final grounded:Bool;
}
