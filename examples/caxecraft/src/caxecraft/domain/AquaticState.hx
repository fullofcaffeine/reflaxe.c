package caxecraft.domain;

/**
	Serializable fixed-tick water state kept beside an ordinary `CharacterBody`.

	Breath uses integer ticks rather than wall-clock seconds, so replay, saves,
	Eval, and native C make the same decision. `drowningTicks` counts down only
	after breath reaches zero and is reset when the character's head leaves water.
**/
typedef AquaticState = {
	final medium:AquaticMedium;
	final submersion:Float;
	final headSubmerged:Bool;
	final breathTicks:Int;
	final drowningTicks:Int;
}
