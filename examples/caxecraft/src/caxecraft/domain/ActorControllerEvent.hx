package caxecraft.domain;

import caxecraft.scenario.ContentId;

/**
	A typed observation emitted while authored controllers advance.

	The scheduler reports intent without performing presentation, inventory,
	dialogue, or story work. Those consumers can react to an interaction, a
	committed local-player attack, or a one-shot content-defined drop request.
**/
enum ActorControllerEvent {
	/** Internal no-event value; `GameSession` does not publish it to consumers. */
	NoControllerEvent;

	/** This stationary actor is within its content-defined interaction radius. */
	InteractionAvailable(characterId:EntityId);

	/**
		This actor completed one in-range wind-up.

		The pure scheduler requests the attack; `GameSession` publishes this event
		only after its shared local-player damage command commits.
	**/
	LocalPlayerAttack(characterId:EntityId);

	/** This defeated actor requested its content-defined drop exactly once. */
	DropRequested(characterId:EntityId, drop:ContentId);
}
