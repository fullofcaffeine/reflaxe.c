package caxecraft.content;

import caxecraft.domain.AquaticProfile;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.scenario.ContentId;

/**
	Converts a validated content ID into the mechanics needed to build an actor.

	CaxeMap says which NPC or enemy is placed; a content pack says how that kind
	of actor behaves. This interface joins those two sources without teaching the
	generic level loader any built-in name such as Nia or Mossling. The active
	`RuntimeContentRegistry` implements it from validated package bytes, so actor
	planning stays unchanged when a different admitted package supplies the facts.
**/
interface ActorContentResolver {
	/** Resolve an ID written in an `Npc` placement as an NPC mechanics profile. */
	function resolveNpc(id:ContentId):ActorContentResolution;

	/** Resolve an ID written in an `Entity` placement as an enemy profile. */
	function resolveEnemy(id:ContentId):ActorContentResolution;
}

/**
	The two character-like content kinds currently admitted by CAXEMAP 1.

	The kind remains explicit so a known enemy used in an NPC placement produces
	a useful wrong-kind diagnostic instead of looking like an unknown ID.
**/
enum ActorContentKind {
	NpcContent;
	EnemyContent;
}

/**
	Complete reusable mechanics for one character-like content definition.

	This is an immutable record because it describes construction; it has no live
	identity or mutable lifetime of its own. The later publication boundary uses
	it to create a `Character` and the selected controller state.
**/
typedef ActorMechanicsProfile = {
	final maximumHealth:Int;
	final aquaticProfile:AquaticProfile;
	final controller:ActorControllerProfile;
}

/** Exact result of resolving one content ID for its authored placement kind. */
enum ActorContentResolution {
	/** The content exists, has the expected kind, and owns a complete profile. */
	ActorContentResolved(profile:ActorMechanicsProfile);

	/** No character-like definition in this content pack owns the supplied ID. */
	UnknownActorContent;

	/** The ID exists, but its declared content kind does not match the placement. */
	WrongActorContentKind(actual:ActorContentKind);
}
