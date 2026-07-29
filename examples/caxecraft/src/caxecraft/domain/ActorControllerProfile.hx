package caxecraft.domain;

import caxecraft.scenario.ContentId;

/**
	Describes the reusable controller selected for one authored character.

	Content chooses one closed profile, while the later actor scheduler turns it
	into `CharacterIntent` values. Keeping this value in the domain lets
	`GameSession` own controller bindings without depending on content loaders.
**/
/** Closed set of reusable controller recipes admitted by the current engine. */
enum ActorControllerProfile {
	/** Remain at the authored transform and offer interaction within this radius. */
	StationaryDialogue(interactionRadiusMilli:Int);

	/** Wander, pursue, and perform a timed melee attack using bounded pack data. */
	WanderChaseMelee(profile:WanderChaseMeleeProfile);
}

/** Validated parameters for the reusable wander/chase/melee controller. */
typedef WanderChaseMeleeProfile = {
	final noticeRadiusMilli:Int;
	final strikeRadiusMilli:Int;
	final attackRadiusMilli:Int;
	final windupTicks:Int;
	final recoveryTicks:Int;
	final stepMilli:Int;
	final drop:ContentId;
}
