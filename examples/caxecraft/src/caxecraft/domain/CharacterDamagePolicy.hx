package caxecraft.domain;

/** Closed choice for whether one character tick advances damage and recovery. */
enum abstract CharacterDamagePolicy(Int) {
	/** Movement and water still advance, but health and safe-time do not. */
	var Invulnerable = 0;

	/** Advance safe-time and apply any drowning damage produced by aquatics. */
	var Survival = 1;
}
