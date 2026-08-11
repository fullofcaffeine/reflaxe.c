package caxecraft.domain;

/**
	Names the reusable execution phase of one authored actor controller.

	The phase describes behavior, not content identity: any character using the
	same profile can be stationary, wandering, chasing, returning, winding up,
	recovering, or defeated. Haxe.c lowers this closed enum to a readable native C
	enum field.
**/
enum ActorControllerPhase {
	/** Hold the authored position while offering a nearby interaction. */
	Stationary;

	/** Wait at home until the deterministic wander cycle or player pursuit begins. */
	Resting;

	/** Follow the tick-number-driven path around the authored home position. */
	Wandering;

	/** Move toward the observed local-player position. */
	Chasing;

	/** Move back to the authored home after pursuit ends. */
	Returning;

	/** Hold position while counting down to one melee impact. */
	Windup;

	/** Hold position until another melee wind-up may begin. */
	Recovering;

	/** Hold position while a large attack announces the coming charge. */
	Roaring;

	/** Move toward the player after the warning has completed. */
	Charging;

	/** Hold position while the close-range sweep remains avoidable. */
	TailSweep;

	/** Hold position and accept counterattacks before the next cycle. */
	Stunned;

	/** Stop movement and remember that any one-shot drop was requested. */
	Defeated;
}
