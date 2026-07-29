package caxecraft.content;

import caxecraft.content.ActorCompositionPlanner.CharacterSpawnPlan;
import caxecraft.content.ActorContentResolver.ActorMechanicsProfile;
import caxecraft.content.ActorMechanicsValidation.isValidActorMechanics;
import caxecraft.domain.ActorControllerBinding;
import caxecraft.domain.Character;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.GameSession;
import caxecraft.domain.World;

/**
	Builds complete runtime actor values, then asks `GameSession` to publish them.

	All conversion happens in temporary Arrays. Invalid plans return before the
	session changes; the session performs a second ownership preflight before its
	synchronous commit. A controller binding records which reusable behavior was
	selected, but this module does not run that behavior; the fixed-tick scheduler
	owned by `haxe_c-xge.20.4.2.4.5` will turn it into `CharacterIntent` values.
**/
/** Success count or the exact stage that rejected an actor publication. */
enum ActorPublicationResult {
	ActorsPublished(count:Int);
	ActorPublicationRejected(error:ActorPublicationError);
}

/** Exact reason an immutable spawn plan could not become live session state. */
enum ActorPublicationError {
	InvalidRuntimePlan(index:Int);
	SessionPublicationRejected;
}

/**
	Construct and publish one complete actor-plan generation.

	Characters and controller bindings are built off-session first. The session
	then validates their IDs and replaces its authored actors without exposing a
	half-published state. A rejection therefore leaves the previously playable
	session unchanged and safe to retry.
**/
function publishActorPlans(session:GameSession, plans:Array<CharacterSpawnPlan>):ActorPublicationResult {
	final characters:Array<Character> = [];
	final controllers:Array<ActorControllerBinding> = [];
	for (index in 0...plans.length) {
		final plan = plans[index];
		if (!validPlan(plan))
			return ActorPublicationRejected(InvalidRuntimePlan(index));
		final character = startCharacter(plan.entityId,
			createBody(plan.transform.xMilli / 1000.0, plan.transform.yMilli / 1000.0, plan.transform.zMilli / 1000.0), plan.aquaticProfile,
			plan.maximumHealth);
		characters.push(character);
		controllers.push({characterId: plan.entityId, profile: plan.controller});
	}
	return session.replaceAuthoredActors(characters, controllers) ? ActorsPublished(characters.length) : ActorPublicationRejected(SessionPublicationRejected);
}

/** Defend the live boundary even when a caller bypasses the validated planner. */
private function validPlan(plan:CharacterSpawnPlan):Bool
	return plan.entityId.isValid()
		&& isValidActorMechanics({
			maximumHealth: plan.maximumHealth,
			aquaticProfile: plan.aquaticProfile,
			controller: plan.controller
		})
		&& plan.transform.xMilli >= 0
		&& plan.transform.xMilli < World.WIDTH * 1000
		&& plan.transform.yMilli >= 0
		&& plan.transform.yMilli < World.HEIGHT * 1000
		&& plan.transform.zMilli >= 0
		&& plan.transform.zMilli < World.DEPTH * 1000;
