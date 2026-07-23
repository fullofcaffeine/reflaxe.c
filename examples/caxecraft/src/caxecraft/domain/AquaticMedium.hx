package caxecraft.domain;

/**
	Stable names for a character's current relationship with water.

	Physics still uses the continuous `submersion` fraction. These names are for
	animation, sound, camera policy, save/debug state, and authored conditions.
	Hysteresis in `Aquatics.classifyMedium` prevents a character resting at a
	waterline from switching names every fixed tick.
**/
enum abstract AquaticMedium(Int) to Int {
	var Dry = 0;
	var Wading = 1;
	var Floating = 2;
	var Submerged = 3;
}
