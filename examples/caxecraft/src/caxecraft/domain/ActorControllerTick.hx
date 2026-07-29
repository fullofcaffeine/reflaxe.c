package caxecraft.domain;

import caxecraft.domain.ActorControllerDecision.ActorControllerPlanError;

/**
	The deterministic completion status of one authored-controller pass.

	A successful pass visits every controller in authored order. Rejection names
	the first actor whose model, character ownership, movement commit, or local
	attack commit failed; already completed earlier actors remain valid committed
	state and later actors are not advanced.
**/
enum ActorControllerTickStatus {
	/** Every published controller committed once. */
	ControllersAdvanced;

	/** The saved controller state is inconsistent with its live character or recipe. */
	ControllerModelRejected(characterId:EntityId, error:ActorControllerPlanError);

	/** A controller names a character that is no longer owned by this session. */
	ControlledCharacterMissing(characterId:EntityId);

	/** The shared character movement command rejected this planned intent. */
	CharacterCommandRejected(characterId:EntityId);

	/** The shared local-player damage command rejected a planned impact. */
	LocalAttackCommandRejected(characterId:EntityId);
}

/** Compact fixed-tick summary; detailed typed events remain session-owned. */
typedef ActorControllerTickResult = {
	final status:ActorControllerTickStatus;
	final processed:Int;
	final emittedEvents:Int;
}
