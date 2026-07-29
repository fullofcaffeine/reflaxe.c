package caxecraft.domain;

/**
	Binds one live character identity to its validated controller recipe.

	The binding is immutable construction state. The later scheduler may own
	mutable phase/timer state, but it will still address the same character by
	`EntityId` and produce ordinary `CharacterIntent` values.
**/
typedef ActorControllerBinding = {
	final characterId:EntityId;
	final profile:ActorControllerProfile;
}
