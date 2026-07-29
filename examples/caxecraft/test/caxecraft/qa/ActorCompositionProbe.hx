package caxecraft.qa;

import caxecraft.content.ActorCompositionPlanner.ActorCompositionError;
import caxecraft.content.ActorCompositionPlanner.ActorCompositionResult;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnPlan;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.ActorCompositionPlanner.planActorComposition;
import caxecraft.content.ActorContentResolver;
import caxecraft.content.ActorContentResolver.ActorContentKind;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.ActorContentResolver.ActorControllerProfile;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanError;
import caxecraft.content.ActorIdentityPlanner.actorEntityId;
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.domain.Aquatics.profile as aquaticProfile;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;

/**
	Executable specification for content-driven actor composition planning.

	Eval and generated native C feed the same validated-shaped placements into
	the production planner. The test covers ordered success and every rejection
	family without constructing or mutating a `GameSession`; publication belongs
	to the next actor-composition task.
**/
var observed:Int = 0;

function main():Void {
	#if c
	observed = selfCheck();
	#else
	Sys.println(selfCheck());
	#end
}

/** Return zero, or the stable number of the first broken planning invariant. */
function selfCheck():Int {
	final registry = new BaseContentRegistry();
	switch planActorComposition([], registry) {
		case ActorCompositionPlanned(plans) if (plans.length == 0):
		case _:
			return 1;
	}

	final niaTransform = transform(17500, 5000, 13500, 270);
	final mosslingTransform = transform(12500, 5000, 12500, 45);
	final nia = object("guide.nia", Npc(new ContentId("caxecraft:nia"), new ScenarioId("dialogue.nia.welcome"), niaTransform));
	final mossling = object("enemy.mossling", Entity(new ContentId("caxecraft:mossling"), mosslingTransform));
	final ignored = object("checkpoint.village", Checkpoint(transform(8000, 2000, 8000, 0)));
	final first = planned([ignored, nia, mossling], registry);
	if (first.length != 2 || !validNia(first[0], niaTransform) || !validMossling(first[1], mosslingTransform))
		return 2;

	final reordered = planned([mossling, ignored, nia], registry);
	final inserted = planned([
		object("item.unrelated", Item(new ContentId("caxecraft:berries"), 1, transform(9000, 3000, 9000, 0))),
		nia,
		mossling
	], registry);
	if (reordered.length != 2
		|| reordered[0].entityId != actorEntityId(new ScenarioId("enemy.mossling"))
		|| reordered[1].entityId != actorEntityId(new ScenarioId("guide.nia"))
		|| inserted[0].entityId != first[0].entityId
		|| inserted[1].entityId != first[1].entityId)
		return 3;

	switch planActorComposition([
		nia,
		object("guide.nia", Npc(new ContentId("caxecraft:nia"), new ScenarioId("dialogue.repeat"), niaTransform))
	], registry) {
		case ActorCompositionRejected(ActorIdentityRejected(DuplicateAuthoredId(id))) if (id.text() == "guide.nia"):
		case _:
			return 4;
	}

	switch planActorComposition([
		object("actor.abn8u2c", Entity(new ContentId("caxecraft:mossling"), mosslingTransform)),
		object("actor.aowprp4", Entity(new ContentId("caxecraft:mossling"), mosslingTransform))
	], registry) {
		case ActorCompositionRejected(ActorIdentityRejected(StorageCodeCollision(firstId, secondId, storageCode)))
			if (firstId.text() == "actor.abn8u2c" && secondId.text() == "actor.aowprp4" && storageCode == 2001336683):
		case _:
			return 5;
	}

	final tooMany:Array<ScenarioObject> = [];
	for (index in 0...caxecraft.domain.EntityStore.MAX_CHARACTERS + 1)
		tooMany.push(object('enemy.actor$index', Entity(new ContentId("caxecraft:mossling"), mosslingTransform)));
	switch planActorComposition(tooMany, registry) {
		case ActorCompositionRejected(ActorIdentityRejected(CharacterCapacityExceeded(count, maximum)))
			if (count == caxecraft.domain.EntityStore.MAX_CHARACTERS + 1 && maximum == caxecraft.domain.EntityStore.MAX_CHARACTERS):
		case _:
			return 6;
	}

	switch planActorComposition([
		object("npc.wrong-kind", Npc(new ContentId("caxecraft:mossling"), new ScenarioId("dialogue.test"), niaTransform))
	], registry) {
		case ActorCompositionRejected(PlacedActorKindMismatch(id, content, NpcContent, EnemyContent))
			if (id.text() == "npc.wrong-kind" && content.text() == "caxecraft:mossling"):
		case _:
			return 7;
	}
	switch planActorComposition([
		object("enemy.wrong-kind", Entity(new ContentId("caxecraft:nia"), mosslingTransform))
	], registry) {
		case ActorCompositionRejected(PlacedActorKindMismatch(id, content, EnemyContent, NpcContent))
			if (id.text() == "enemy.wrong-kind" && content.text() == "caxecraft:nia"):
		case _:
			return 8;
	}
	switch planActorComposition([
		nia,
		object("enemy.unknown", Entity(new ContentId("caxecraft:unknown"), mosslingTransform))
	], registry) {
		case ActorCompositionRejected(UnknownPlacedActor(id, content, EnemyContent)) if (id.text() == "enemy.unknown" && content.text() == "caxecraft:unknown"):
		case _:
			return 9;
	}
	// A named local gives today's bounded class lowerer an explicit automatic
	// lifetime. haxe_c-h2b.7 owns the equivalent inline `new Resolver()` call
	// argument; removing this temporary name must not change the planner API.
	final invalidResolver = new InvalidActorResolver();
	switch planActorComposition([
		object("npc.invalid", Npc(new ContentId("test:invalid"), new ScenarioId("dialogue.invalid"), niaTransform))
	], invalidResolver) {
		case ActorCompositionRejected(InvalidActorMechanics(id, content)) if (id.text() == "npc.invalid" && content.text() == "test:invalid"):
		case _:
			return 10;
	}
	return 0;
}

/** Unwrap a successful plan; a rejection makes the caller's assertion fail. */
function planned(objects:Array<ScenarioObject>, resolver:ActorContentResolver):Array<CharacterSpawnPlan>
	return switch planActorComposition(objects, resolver) {
		case ActorCompositionPlanned(plans): plans;
		case ActorCompositionRejected(_): [];
	};

/** Check every map-owned and content-owned fact carried by Nia's spawn recipe. */
function validNia(plan:CharacterSpawnPlan, expected:ScenarioTransform):Bool {
	if (plan.authoredId.text() != "guide.nia"
		|| plan.entityId != actorEntityId(plan.authoredId)
		|| plan.contentId.text() != "caxecraft:nia"
		|| !sameTransform(plan.transform, expected)
		|| plan.maximumHealth != 6
		|| plan.aquaticProfile.maximumBreathTicks != 120)
		return false;
	final correctRole = switch plan.role {
		case DialogueNpc(dialogue): dialogue.text() == "dialogue.nia.welcome";
		case EnemyActor: false;
	};
	final correctController = switch plan.controller {
		case StationaryDialogue(radius): radius == 3500;
		case WanderChaseMelee(_): false;
	};
	return correctRole && correctController;
}

/** Check every map-owned and content-owned fact carried by the enemy recipe. */
function validMossling(plan:CharacterSpawnPlan, expected:ScenarioTransform):Bool {
	if (plan.authoredId.text() != "enemy.mossling"
		|| plan.entityId != actorEntityId(plan.authoredId)
		|| plan.contentId.text() != "caxecraft:mossling"
		|| !sameTransform(plan.transform, expected)
		|| plan.maximumHealth != 3
		|| plan.aquaticProfile.maximumBreathTicks != 120)
		return false;
	final correctRole = switch plan.role {
		case EnemyActor: true;
		case DialogueNpc(_): false;
	};
	final correctController = switch plan.controller {
		case WanderChaseMelee(profile):
			profile.noticeRadiusMilli == 6000
			&& profile.strikeRadiusMilli == 3000
			&& profile.attackRadiusMilli == 1400
			&& profile.windupTicks == 8
			&& profile.recoveryTicks == 12
			&& profile.stepMilli == 80
			&& profile.drop.text() == "caxecraft:mossling-berries";
		case StationaryDialogue(_): false;
	};
	return correctRole && correctController;
}

/** Build one validated-shaped object without giving tests a private engine path. */
function object(id:String, placement:ObjectPlacement):ScenarioObject
	return {id: new ScenarioId(id), tags: [], placement: placement};

/** Build one deterministic milliblock transform used by the focused fixture. */
function transform(xMilli:Int, yMilli:Int, zMilli:Int, yawDegrees:Int):ScenarioTransform
	return {
		xMilli: xMilli,
		yMilli: yMilli,
		zMilli: zMilli,
		yawDegrees: yawDegrees
	};

/** Compare all authored transform fields without converting to floating point. */
function sameTransform(left:ScenarioTransform, right:ScenarioTransform):Bool
	return left.xMilli == right.xMilli && left.yMilli == right.yMilli && left.zMilli == right.zMilli && left.yawDegrees == right.yawDegrees;

/**
	A deliberately faulty content adapter used to prove defensive rejection.

	The class exists because `ActorContentResolver` is a nominal interface whose
	implementation may later carry runtime pack state. This fixture returns zero
	health for one known NPC so the planner—not a test-only duplicate—owns the
	construction-safety check.
**/
final class InvalidActorResolver implements ActorContentResolver {
	public function new() {}

	public function resolveNpc(id:ContentId):ActorContentResolution {
		if (id.text() != "test:invalid")
			return UnknownActorContent;
		return ActorContentResolved({
			maximumHealth: 0,
			aquaticProfile: aquaticProfile(120, 4, 0.35, 14.0, 20.0, 12.0, 0.18, 20, false, false),
			controller: StationaryDialogue(3500)
		});
	}

	public function resolveEnemy(id:ContentId):ActorContentResolution
		return id.text() == "test:invalid" ? WrongActorContentKind(NpcContent) : UnknownActorContent;
}
