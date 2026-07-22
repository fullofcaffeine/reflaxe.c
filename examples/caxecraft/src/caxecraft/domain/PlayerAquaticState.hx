package caxecraft.domain;

/**
	Serializable fixed-tick water state kept beside ordinary `PlayerState`.

	Breath uses integer ticks rather than wall-clock seconds, so replay, saves,
	Eval, and native C make the same decision. `drowningTicks` counts down only
	after breath reaches zero and is reset when the player's head leaves water.
**/
typedef PlayerAquaticState = {
	final medium:PlayerMedium;
	final submersion:Float;
	final headSubmerged:Bool;
	final breathTicks:Int;
	final drowningTicks:Int;
}
