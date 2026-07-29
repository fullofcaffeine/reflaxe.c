package caxecraft.content;

import caxecraft.content.ActorContentResolver.ActorContentKind;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.ActorContentResolver.ActorControllerProfile;
import caxecraft.content.ActorContentResolver.ActorMechanicsProfile;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanError;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanResult;
import caxecraft.content.ActorIdentityPlanner.planActorIdentities;
import caxecraft.domain.AquaticProfile;
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

	“Spawn” means construction information here, not a live actor. The next
	transactional publication task, `haxe_c-xge.20.4.2.4.4`, will convert these
	immutable values into `Character` and controller state and commit them
	together.
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
		if (!validMechanics(mechanics))
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

/**
	Defend the construction boundary even when a custom resolver is faulty.

	Built-in JSON validation already enforces these limits. Rechecking the small
	live profile prevents a future runtime pack adapter or test resolver from
	creating impossible health, water movement, or controller timing values.
**/
private function validMechanics(profile:ActorMechanicsProfile):Bool {
	if (profile.maximumHealth < 1 || profile.maximumHealth > 10000)
		return false;
	final aquatic = profile.aquaticProfile;
	if (aquatic.maximumBreathTicks < 1 || aquatic.maximumBreathTicks > 12000 || aquatic.breathRecoveryPerTick < 1 || aquatic.breathRecoveryPerTick > 120
		|| aquatic.horizontalControl < 0.0 || aquatic.horizontalControl > 1.0 || aquatic.ascentAcceleration < 0.0 || aquatic.ascentAcceleration > 40.0
		|| aquatic.descentAcceleration < 0.0 || aquatic.descentAcceleration > 40.0 || aquatic.buoyancyAcceleration < 0.0
		|| aquatic.buoyancyAcceleration > 30.0 || aquatic.dragPerTick < 0.0 || aquatic.dragPerTick > 0.9 || aquatic.drowningIntervalTicks < 1
		|| aquatic.drowningIntervalTicks > 1200)
		return false;
	return switch profile.controller {
		case StationaryDialogue(radius): radius >= 250 && radius <= 32000;
		case WanderChaseMelee(controller):
			controller.noticeRadiusMilli >= 250
			&& controller.noticeRadiusMilli <= 64000
			&& controller.strikeRadiusMilli >= controller.attackRadiusMilli
			&& controller.noticeRadiusMilli >= controller.strikeRadiusMilli
			&& controller.attackRadiusMilli >= 250
			&& controller.windupTicks >= 1
			&& controller.windupTicks <= 1200
			&& controller.recoveryTicks >= 1
			&& controller.recoveryTicks <= 1200
			&& controller.stepMilli >= 1
			&& controller.stepMilli <= 10000;
	};
}
