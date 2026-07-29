package caxecraft.domain;

/**
	One immutable runtime snapshot for an authored actor controller.

	`profile` is the validated, content-selected recipe. The remaining fields are
	the smallest state needed to resume deterministic execution on the next fixed
	tick. Home position is captured when the actor is published; phase and timer
	change only after `GameSession` commits the matching character step.
**/
typedef ActorControllerState = {
	final characterId:EntityId;
	final profile:ActorControllerProfile;
	final homeX:Float;
	final homeZ:Float;
	final phase:ActorControllerPhase;
	final phaseTicks:Int;

	/** True after the scheduler has emitted this defeated actor's one drop request. */
	final dropPublished:Bool;
}
