package caxecraft.qa;

import caxecraft.content.ActorCompositionPlanner.ActorCompositionError;
import caxecraft.content.ActorCompositionPlanner.ActorCompositionResult;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnPlan;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.ActorCompositionPlanner.planActorComposition;
import caxecraft.content.ActorPublication.ActorPublicationError;
import caxecraft.content.ActorPublication.ActorPublicationResult;
import caxecraft.content.ActorPublication.publishActorPlans;
import caxecraft.content.ActorContentResolver;
import caxecraft.content.ActorContentResolver.ActorContentKind;
import caxecraft.content.ActorContentResolver.ActorContentResolution;
import caxecraft.content.ActorIdentityPlanner.ActorIdentityPlanError;
import caxecraft.content.ActorIdentityPlanner.actorEntityId;
import caxecraft.domain.Aquatics.profile as aquaticProfile;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioObject.ObjectPlacement;
import caxecraft.qa.FocusedContentFixture.FocusedContentRegistry;

/**
	Executable specification for content-driven actor planning and publication.

	Eval and generated native C feed the same validated-shaped placements into
	the production planner and publication boundary. The test covers ordered
	success, rejected construction and replacement, unchanged prior state, retry,
	and empty replacement through a real `GameSession`.
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
	final registry = new FocusedContentRegistry();
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

	final session = new GameSession();
	final localId = EntityId.fromValidatedStorageCode(77);
	if (!session.bindLocalPlayer(startCharacter(localId, createBody(8.5, 2.0, 8.5), aquaticProfile(120, 4, 0.35, 14.0, 20.0, 12.0, 0.18, 20, false, false), 8)))
		return 11;
	switch publishActorPlans(session, first) {
		case ActorsPublished(2):
		case _:
			return 12;
	}
	if (!publishedInOrder(session, localId, first))
		return 13;

	final beforeRejectedCharacters = session.characterSnapshots();
	final beforeRejectedControllers = session.actorControllerSnapshots();
	final invalidPlan:CharacterSpawnPlan = {
		authoredId: first[0].authoredId,
		entityId: first[0].entityId,
		contentId: first[0].contentId,
		transform: first[0].transform,
		maximumHealth: 0,
		aquaticProfile: first[0].aquaticProfile,
		controller: first[0].controller,
		role: first[0].role
	};
	switch publishActorPlans(session, [invalidPlan]) {
		case ActorPublicationRejected(InvalidRuntimePlan(0)):
		case _:
			return 14;
	}
	if (!samePublishedState(session, beforeRejectedCharacters, beforeRejectedControllers))
		return 15;
	final invalidControllerPlan:CharacterSpawnPlan = {
		authoredId: first[1].authoredId,
		entityId: first[1].entityId,
		contentId: first[1].contentId,
		transform: first[1].transform,
		maximumHealth: first[1].maximumHealth,
		aquaticProfile: first[1].aquaticProfile,
		controller: WanderChaseMelee({
			noticeRadiusMilli: 1,
			strikeRadiusMilli: 1,
			attackRadiusMilli: 1,
			windupTicks: 1,
			recoveryTicks: 1,
			stepMilli: 1,
			drop: new ContentId("caxecraft:berries")
		}),
		role: first[1].role
	};
	switch publishActorPlans(session, [invalidControllerPlan]) {
		case ActorPublicationRejected(InvalidRuntimePlan(0)):
		case _:
			return 16;
	}
	if (!samePublishedState(session, beforeRejectedCharacters, beforeRejectedControllers))
		return 17;
	switch publishActorPlans(session, [first[0], first[0]]) {
		case ActorPublicationRejected(SessionPublicationRejected):
		case _:
			return 18;
	}
	if (!samePublishedState(session, beforeRejectedCharacters, beforeRejectedControllers))
		return 19;
	final overCapacity:Array<CharacterSpawnPlan> = [];
	for (index in 0...caxecraft.domain.EntityStore.MAX_CHARACTERS)
		overCapacity.push(withEntityId(first[1], EntityId.fromValidatedStorageCode(1000 + index)));
	switch publishActorPlans(session, overCapacity) {
		case ActorPublicationRejected(SessionPublicationRejected):
		case _:
			return 20;
	}
	if (!samePublishedState(session, beforeRejectedCharacters, beforeRejectedControllers))
		return 21;

	switch publishActorPlans(session, [first[1], first[0]]) {
		case ActorsPublished(2):
		case _:
			return 22;
	}
	if (!publishedInOrder(session, localId, [first[1], first[0]]))
		return 23;
	switch publishActorPlans(session, []) {
		case ActorsPublished(0):
		case _:
			return 24;
	}
	if (session.characterCount() != 1 || session.readLocalPlayer().id != localId || session.actorControllerSnapshots().length != 0)
		return 25;
	final controllerResult = ActorControllerProbe.selfCheck();
	if (controllerResult != 0)
		return 100 + controllerResult;
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

/** Copy one otherwise valid plan under a different stable runtime identity. */
function withEntityId(plan:CharacterSpawnPlan, entityId:EntityId):CharacterSpawnPlan
	return {
		authoredId: plan.authoredId,
		entityId: entityId,
		contentId: plan.contentId,
		transform: plan.transform,
		maximumHealth: plan.maximumHealth,
		aquaticProfile: plan.aquaticProfile,
		controller: plan.controller,
		role: plan.role
	};

/** Check the session's local-first character order and matching controller IDs. */
function publishedInOrder(session:GameSession, localId:EntityId, plans:Array<CharacterSpawnPlan>):Bool {
	final characters = session.characterSnapshots();
	final controllers = session.actorControllerSnapshots();
	if (characters.length != plans.length + 1 || controllers.length != plans.length || characters[0].id != localId)
		return false;
	for (index in 0...plans.length)
		if (characters[index + 1].id != plans[index].entityId
			|| controllers[index].characterId != plans[index].entityId
			|| characters[index + 1].vitals.health != plans[index].maximumHealth)
			return false;
	return true;
}

/** Prove a rejected publication retained both authoritative arrays unchanged. */
function samePublishedState(session:GameSession, characters:Array<caxecraft.domain.Character>,
		controllers:Array<caxecraft.domain.ActorControllerBinding>):Bool {
	final currentCharacters = session.characterSnapshots();
	final currentControllers = session.actorControllerSnapshots();
	if (currentCharacters.length != characters.length || currentControllers.length != controllers.length)
		return false;
	for (index in 0...characters.length)
		if (currentCharacters[index].id != characters[index].id)
			return false;
	for (index in 0...controllers.length)
		if (currentControllers[index].characterId != controllers[index].characterId)
			return false;
	return true;
}

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
