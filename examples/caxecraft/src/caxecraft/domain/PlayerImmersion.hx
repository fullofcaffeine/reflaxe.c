package caxecraft.domain;

/**
	One fixed-tick observation of how the player's body intersects water.

	`submersion` is the water-covered share of the 1.8-unit body height, from
	zero to one. The three named samples make interaction and presentation rules
	explicit. `cameraBlend` changes smoothly only near the eye waterline and is a
	presentation input; it never feeds collision or breath decisions.
**/
typedef PlayerImmersion = {
	final submersion:Float;
	final feetWet:Bool;
	final bodyWet:Bool;
	final headWet:Bool;
	final cameraBlend:Float;
}
