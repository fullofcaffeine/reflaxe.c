package caxecraft.content;

import caxecraft.domain.AquaticProfile;
import caxecraft.scenario.ContentId;

/**
	Converts a validated content ID into the mechanics needed to build an actor.

	CaxeMap says which NPC or enemy is placed; a content pack says how that kind
	of actor behaves. This interface joins those two sources without teaching the
	generic level loader any built-in name such as Nia or Mossling. The generated
	base-pack registry implements it today, and a future runtime pack loader can
	implement the same contract without changing actor planning.
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
	A controller recipe selected by validated content.

	A controller turns world observations into ordinary `CharacterIntent` values
	during later simulation ticks. This value stores only reviewed parameters; it
	does not execute artificial intelligence, dialogue, combat, or CaxeFlow while
	a level is being planned.
**/
enum ActorControllerProfile {
	/** Remain at the authored transform and offer dialogue within this radius. */
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
