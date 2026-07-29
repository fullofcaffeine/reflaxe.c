package caxecraft.domain;

/**
	Binds one live character identity to its validated controller recipe.

	The binding is immutable construction state. `ActorControllerScheduler`
	creates a separate phase/timer snapshot, but it still addresses the same
	character by `EntityId` and produces ordinary `CharacterIntent` values.
**/
typedef ActorControllerBinding = {
	final characterId:EntityId;
	final profile:ActorControllerProfile;
}
