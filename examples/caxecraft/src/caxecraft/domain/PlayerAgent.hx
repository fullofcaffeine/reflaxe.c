package caxecraft.domain;

/**
	Binds human input and player-only presentation policy to one character ID.

	The binding owns no body, breath, or health copy. Camera and HUD code resolve
	the committed character through this ID; an NPC controller can use the same
	character mechanics without constructing a `PlayerAgent`.

	This is an immutable `typedef` record rather than a class because the current
	value has no independent identity, mutable state, resource, or lifetime. Haxe.c
	lowers its admitted record shape to a small C value, so binding a player needs
	neither allocation nor an object pointer. A class would add ownership semantics
	that the current model does not need.

	Reconsider that choice only if the binding starts to own persistent player-only
	state such as buffered input, controller assignment, or camera preferences.
	Those capabilities are planned by the runtime-architecture work, but they do
	not predetermine a class: keep this value small if they remain explicit values
	owned elsewhere. See `docs/haxe-code-architecture.md` for the general rule.
**/
typedef PlayerAgent = {
	final characterId:EntityId;
}

/**
	Create the local-player binding after the entity store accepts the ID.

	This is a module function because construction is a stateless operation, not
	behavior owned by a runtime object. Haxe.c emits an ordinary direct C function;
	there is no generated namespace object or singleton.
**/
function bind(characterId:EntityId):PlayerAgent
	return {characterId: characterId};
