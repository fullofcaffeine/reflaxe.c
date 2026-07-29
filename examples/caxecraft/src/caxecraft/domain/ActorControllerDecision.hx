package caxecraft.domain;

/**
	Why a controller recipe and its runtime state could not be interpreted.

	These are ownership/model failures rather than normal gameplay outcomes. The
	session stops at the first rejection so it cannot silently advance a state
	that no longer describes its character.
**/
enum ActorControllerPlanError {
	/** The runtime state names a different or invalid character snapshot. */
	CharacterIdentityMismatch;

	/** The saved execution phase cannot belong to the selected controller recipe. */
	ProfileStateMismatch;
}

/**
	Pure plan for one authored actor, or an exact model rejection.

	The planned branch contains no mutation: `GameSession` still owns committing
	the intent, local-player damage, next controller state, and published event.
**/
enum ActorControllerDecision {
	/** A valid next state, shared movement intent, and optional typed observation. */
	ControllerPlanned(next:ActorControllerState, intent:CharacterIntent, event:ActorControllerEvent);

	/** No mutation may occur because the controller model is internally inconsistent. */
	ControllerPlanRejected(error:ActorControllerPlanError);
}
