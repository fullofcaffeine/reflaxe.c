package caxecraft.content;

import caxecraft.content.ActorContentResolver.ActorContentKind;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.ActorContentResolver.ActorMechanicsProfile;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanError;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanResult;
import caxecraft.content.ActorIdentityPlanner.planActorIdentities;
import caxecraft.content.ActorMechanicsValidation.isValidActorMechanics;
import caxecraft.domain.AquaticProfile;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.EntityId;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Joins validated map placements and content profiles into actor spawn recipes.

	The planner first collects every character-like placement, checks all stable
	runtime IDs together, then resolves every mechanics profile. It returns the
	complete ordered plan only after all candidates pass. A rejected result is
	therefore safe to report without undoing partially created `GameSession`
	state—this module never mutates a session in the first place.

	“Spawn” means construction information here, not a live actor.
	`ActorPublication.publishActorPlans` converts a complete plan into temporary
	`Character` and controller values; `GameSession` publishes those values only
	after its ownership checks pass. Controller execution is a later step owned
	by `haxe_c-xge.20.4.2.4.5`.
**/
/** Why one actor appears in the level and which placement-only data it carries. */
enum CharacterSpawnRole {
	/** An authored NPC whose dialogue is selected by the placed CaxeMap object. */
	DialogueNpc(dialogue:ScenarioId);

	/** A character-like entity whose mechanics come entirely from its profile. */
	EnemyActor;
}

/** Complete immutable recipe for constructing one character-like runtime actor. */
typedef CharacterSpawnPlan = {
	final authoredId:ScenarioId;
	final entityId:EntityId;
	final contentId:ContentId;
	final transform:ScenarioTransform;
	final maximumHealth:Int;
	final aquaticProfile:AquaticProfile;
	final controller:ActorControllerProfile;
	final role:CharacterSpawnRole;
}

/** Exact reason no complete actor composition plan can be published. */
enum ActorCompositionError {
	/** Stable runtime identity planning rejected the complete candidate set. */
	ActorIdentityRejected(error:ActorIdentityPlanError);

	/** The placed content ID is absent from the selected actor content pack. */
	UnknownPlacedActor(authoredId:ScenarioId, contentId:ContentId, expected:ActorContentKind);

	/** The ID exists, but CaxeMap used it in the wrong placement kind. */
	PlacedActorKindMismatch(authoredId:ScenarioId, contentId:ContentId, expected:ActorContentKind, actual:ActorContentKind);

	/** A resolver returned mechanics that cannot safely construct a character. */
	InvalidActorMechanics(authoredId:ScenarioId, contentId:ContentId);
}

/** Complete ordered recipes, or one rejection with no partially usable prefix. */
enum ActorCompositionResult {
	ActorCompositionPlanned(plans:Array<CharacterSpawnPlan>);
	ActorCompositionRejected(error:ActorCompositionError);
}

private typedef CharacterCandidate = {
	final authoredId:ScenarioId;
	final contentId:ContentId;
	final transform:ScenarioTransform;
	final role:CharacterSpawnRole;
	final expected:ActorContentKind;
}

/**
	Plan every `Npc` and character-like `Entity` in authored object order.

	Non-character placements are intentionally ignored: items, checkpoints,
	prefabs, trigger zones, and stateful objects have different construction
	contracts. The caller must pass a scenario that already passed CAXEMAP
	validation; this boundary still rejects resolver mistakes and identity
	collisions before live state can change.
**/
function planActorComposition(objects:Array<ScenarioObject>, resolver:ActorContentResolver):ActorCompositionResult {
	final candidates:Array<CharacterCandidate> = [];
	final authoredIds:Array<ScenarioId> = [];
	for (object in objects) {
		final candidate:CharacterCandidate = switch object.placement {
			case Npc(npcType, dialogue, transform):
				{
					authoredId: object.id,
					contentId: npcType,
					transform: transform,
					role: DialogueNpc(dialogue),
					expected: NpcContent
				};
			case Entity(entityType, transform):
				{
					authoredId: object.id,
					contentId: entityType,
					transform: transform,
					role: EnemyActor,
					expected: EnemyContent
				};
			case _:
				continue;
		};
		candidates.push(candidate);
		authoredIds.push(candidate.authoredId);
	}

	final identities = switch planActorIdentities(authoredIds) {
		case Planned(bindings): bindings;
		case Rejected(error): return ActorCompositionRejected(ActorIdentityRejected(error));
	};

	final plans:Array<CharacterSpawnPlan> = [];
	for (index in 0...candidates.length) {
		final candidate = candidates[index];
		final resolution = switch candidate.expected {
			case NpcContent: resolver.resolveNpc(candidate.contentId);
			case EnemyContent: resolver.resolveEnemy(candidate.contentId);
		};
		final mechanics:ActorMechanicsProfile = switch resolution {
			case ActorContentResolved(profile): profile;
			case UnknownActorContent:
				return ActorCompositionRejected(UnknownPlacedActor(candidate.authoredId, candidate.contentId, candidate.expected));
			case WrongActorContentKind(actual):
				return ActorCompositionRejected(PlacedActorKindMismatch(candidate.authoredId, candidate.contentId, candidate.expected, actual));
		};
		if (!isValidActorMechanics(mechanics))
			return ActorCompositionRejected(InvalidActorMechanics(candidate.authoredId, candidate.contentId));
		plans.push({
			authoredId: candidate.authoredId,
			entityId: identities[index].entityId,
			contentId: candidate.contentId,
			transform: candidate.transform,
			maximumHealth: mechanics.maximumHealth,
			aquaticProfile: mechanics.aquaticProfile,
			controller: mechanics.controller,
			role: candidate.role
		});
	}
	return ActorCompositionPlanned(plans);
}
