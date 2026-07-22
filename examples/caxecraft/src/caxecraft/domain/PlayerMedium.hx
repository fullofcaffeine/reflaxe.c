package caxecraft.domain;

/**
	Stable player-facing names for the current relationship with water.

	Physics still uses the continuous `submersion` fraction. These names are for
	animation, sound, camera policy, save/debug state, and authored conditions.
	Hysteresis in `PlayerAquatics.classifyMedium` prevents a player resting at a
	waterline from switching names every fixed tick.
**/
enum abstract PlayerMedium(Int) to Int {
	var Dry = 0;
	var Wading = 1;
	var Floating = 2;
	var Submerged = 3;
}
