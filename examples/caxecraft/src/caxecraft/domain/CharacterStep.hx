package caxecraft.domain;

/**
	Complete reusable result of one character movement/water/health tick.

	Presentation reads `immersion`; gameplay commits `character`. Keeping both in
	one result prevents a camera or renderer from running a second physics step.
**/
typedef CharacterStep = {
	final character:Character;
	final immersion:Immersion;
	final drowningDamage:Int;
}
