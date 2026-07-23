package caxecraft.domain;

/**
	Complete observable result of one character-water fixed tick.

	`drowningDamage` is an explicit request for the health system. Keeping the
	aquatic controller independent of hearts means creative mode, checkpoints,
	and later game modes can decide how to apply or suppress that request without
	changing swimming physics.
**/
typedef AquaticStep = {
	final body:CharacterBody;
	final aquatic:AquaticState;
	final immersion:Immersion;
	final drowningDamage:Int;
}
