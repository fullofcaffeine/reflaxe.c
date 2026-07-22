package caxecraft.domain;

/**
	Complete observable result of one player-water fixed tick.

	`drowningDamage` is an explicit request for the health system. Keeping the
	aquatic controller independent of hearts means creative mode, checkpoints,
	and later game modes can decide how to apply or suppress that request without
	changing swimming physics.
**/
typedef PlayerAquaticStep = {
	final player:PlayerState;
	final aquatic:PlayerAquaticState;
	final immersion:PlayerImmersion;
	final drowningDamage:Int;
}
