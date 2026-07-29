package caxecraft.qa;

import caxecraft.domain.ActorControllerDecision.ActorControllerPlanError;
import caxecraft.domain.ActorControllerDecision.ActorControllerDecision;
import caxecraft.domain.ActorControllerEvent;
import caxecraft.domain.ActorControllerEvent.*;
import caxecraft.domain.ActorControllerPhase.*;
import caxecraft.domain.ActorControllerProfile;
import caxecraft.domain.ActorControllerScheduler.planActorController;
import caxecraft.domain.ActorControllerScheduler.startActorController;
import caxecraft.domain.ActorControllerState;
import caxecraft.domain.ActorControllerTick.ActorControllerTickStatus;
import caxecraft.domain.Aquatics.profile as aquaticProfile;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterDamagePolicy;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.scenario.ContentId;

/**
	Executable specification for the reusable authored-actor scheduler.

	The probe drives the pure controller planner and the real `GameSession`
	commit boundary. Eval and generated native C must agree on interaction order,
	movement, attack timing, recovery, return intent, one-shot drops, empty
	controller sets, and fail-closed missing or inconsistent state.
**/
private inline final EPSILON:Float = 0.00001;

/**
	Return zero, or the stable number of the first broken controller invariant.

	The result is consumed by `ActorCompositionProbe`, keeping actor planning,
	publication, and execution in one focused Eval/native-C command while the
	failures remain attributable to this smaller scheduler oracle.
**/
function selfCheck():Int {
	final localId = EntityId.fromValidatedStorageCode(700);
	final firstId = EntityId.fromValidatedStorageCode(701);
	final secondId = EntityId.fromValidatedStorageCode(702);
	final stationary:ActorControllerProfile = StationaryDialogue(3500);
	final hostile = hostileProfile();

	final emptySession = new GameSession();
	if (!bindLocal(emptySession, localId, 8.0, 10.0) || !emptySession.replaceAuthoredActors([], []))
		return 1;
	final emptyTick = emptySession.stepAuthoredActorControllers(0, CharacterDamagePolicy.Invulnerable);
	switch emptyTick.status {
		case ControllersAdvanced if (emptyTick.processed == 0 && emptyTick.emittedEvents == 0):
		case _:
			return 2;
	}

	final ordered = new GameSession();
	final first = character(firstId, 9.0, 10.0, 6);
	final second = character(secondId, 10.0, 10.0, 6);
	if (!bindLocal(ordered, localId, 8.0, 10.0) || !ordered.replaceAuthoredActors([first, second], [
		{characterId: firstId, profile: stationary},
		{characterId: secondId, profile: stationary}
	]))
		return 3;
	final orderedTick = ordered.stepAuthoredActorControllers(0, CharacterDamagePolicy.Invulnerable);
	switch orderedTick.status {
		case ControllersAdvanced if (orderedTick.processed == 2 && orderedTick.emittedEvents == 2):
		case _:
			return 4;
	}
	final orderedEvents = ordered.actorControllerEventSnapshots();
	if (orderedEvents.length != 2 || interactionId(orderedEvents[0]) != firstId || interactionId(orderedEvents[1]) != secondId)
		return 5;
	final orderedStates = ordered.actorControllerStateSnapshots();
	if (orderedStates.length != 2
		|| orderedStates[0].characterId != firstId
		|| orderedStates[1].characterId != secondId
		|| orderedStates[0].phase != Stationary
		|| orderedStates[1].phase != Stationary)
		return 6;
	if (!near(ordered.readCharacter(firstId).body.x, first.body.x) || !near(ordered.readCharacter(secondId).body.z, second.body.z))
		return 7;

	final chasing = new GameSession();
	final chaser = character(firstId, 12.0, 10.0, 3);
	if (!bindLocal(chasing, localId, 8.0, 10.0) || !chasing.replaceAuthoredActors([chaser], [{characterId: firstId, profile: hostile}]))
		return 8;
	final chaseTick = chasing.stepAuthoredActorControllers(1, CharacterDamagePolicy.Invulnerable);
	switch chaseTick.status {
		case ControllersAdvanced if (chaseTick.processed == 1 && chaseTick.emittedEvents == 0):
		case _:
			return 9;
	}
	final chaseStates = chasing.actorControllerStateSnapshots();
	if (chaseStates.length != 1 || chaseStates[0].phase != Chasing || !near(chasing.readCharacter(firstId).body.x, 11.92))
		return 10;

	final farPlayer = character(localId, 30.0, 30.0, 6);
	final wanderState = startActorController({characterId: firstId, profile: hostile}, chaser);
	switch planActorController(wanderState, chaser, farPlayer, 20) {
		case ControllerPlanned(next, intent, NoControllerEvent) if (next.phase == Wandering && intent.moveX > 0.0 && near(intent.moveZ, 0.0)):
		case _:
			return 11;
	}
	final returningState:ActorControllerState = {
		characterId: firstId,
		profile: hostile,
		homeX: 12.0,
		homeZ: 10.0,
		phase: Chasing,
		phaseTicks: 0,
		dropPublished: false
	};
	final displaced = character(firstId, 14.0, 10.0, 3);
	switch planActorController(returningState, displaced, farPlayer, 0) {
		case ControllerPlanned(next, intent, NoControllerEvent) if (next.phase == Returning && intent.moveX < 0.0 && near(intent.moveZ, 0.0)):
		case _:
			return 12;
	}

	final attacking = new GameSession();
	final attacker = character(firstId, 9.0, 10.0, 3);
	if (!bindLocal(attacking, localId, 8.0, 10.0)
		|| !attacking.replaceAuthoredActors([attacker], [{characterId: firstId, profile: hostile}]))
		return 13;
	for (tick in 0...8) {
		final windup = attacking.stepAuthoredActorControllers(tick, CharacterDamagePolicy.Invulnerable);
		switch windup.status {
			case ControllersAdvanced:
			case _:
				return 14;
		}
		if (windup.emittedEvents != 0 || attacking.readLocalPlayer().vitals.health != 6)
			return 15;
	}
	final impact = attacking.stepAuthoredActorControllers(8, CharacterDamagePolicy.Invulnerable);
	switch impact.status {
		case ControllersAdvanced if (impact.processed == 1 && impact.emittedEvents == 1):
		case _:
			return 16;
	}
	final attackEvents = attacking.actorControllerEventSnapshots();
	final attackStates = attacking.actorControllerStateSnapshots();
	if (attackEvents.length != 1
		|| attackId(attackEvents[0]) != firstId
		|| attacking.readLocalPlayer().vitals.health != 5
		|| attackStates.length != 1
		|| attackStates[0].phase != Recovering
		|| attackStates[0].phaseTicks != 12)
		return 17;
	final recovery = attacking.stepAuthoredActorControllers(9, CharacterDamagePolicy.Invulnerable);
	if (recovery.emittedEvents != 0 || attacking.actorControllerStateSnapshots()[0].phaseTicks != 11)
		return 18;

	final windupState:ActorControllerState = {
		characterId: firstId,
		profile: hostile,
		homeX: 9.0,
		homeZ: 10.0,
		phase: Windup,
		phaseTicks: 4,
		dropPublished: false
	};
	switch planActorController(windupState, attacker, farPlayer, 0) {
		case ControllerPlanned(next, intent, NoControllerEvent)
			if (next.phase == Chasing && next.phaseTicks == 0 && near(intent.moveX, 0.0) && near(intent.moveZ, 0.0)):
		case _:
			return 19;
	}

	final defeated = character(firstId, 9.0, 10.0, 0);
	final liveState = startActorController({characterId: firstId, profile: hostile}, attacker);
	final afterDrop = switch planActorController(liveState, defeated, farPlayer, 0) {
		case ControllerPlanned(next, intent, DropRequested(source, drop))
			if (source == firstId && drop.text() == "caxecraft:mossling-berries" && near(intent.moveX, 0.0)):
			next;
		case _:
			return 20;
	};
	switch planActorController(afterDrop, defeated, farPlayer, 1) {
		case ControllerPlanned(next, _, NoControllerEvent) if (next.phase == Defeated && next.dropPublished):
		case _:
			return 21;
	}
	final dropping = new GameSession();
	if (!bindLocal(dropping, localId, 8.0, 10.0)
		|| !dropping.replaceAuthoredActors([defeated], [{characterId: firstId, profile: hostile}]))
		return 22;
	final firstDropTick = dropping.stepAuthoredActorControllers(0, CharacterDamagePolicy.Invulnerable);
	final firstDropEvents = dropping.actorControllerEventSnapshots();
	if (firstDropTick.emittedEvents != 1 || firstDropEvents.length != 1 || dropId(firstDropEvents[0]) != firstId)
		return 23;
	final secondDropTick = dropping.stepAuthoredActorControllers(1, CharacterDamagePolicy.Invulnerable);
	if (secondDropTick.emittedEvents != 0 || dropping.actorControllerEventSnapshots().length != 0)
		return 24;

	final mismatchedState:ActorControllerState = {
		characterId: firstId,
		profile: stationary,
		homeX: 9.0,
		homeZ: 10.0,
		phase: Resting,
		phaseTicks: 0,
		dropPublished: false
	};
	switch planActorController(mismatchedState, attacker, farPlayer, 0) {
		case ControllerPlanRejected(ActorControllerPlanError.ProfileStateMismatch):
		case _:
			return 25;
	}

	final missing = new GameSession();
	if (!bindLocal(missing, localId, 8.0, 10.0)
		|| !missing.replaceAuthoredActors([attacker], [{characterId: firstId, profile: hostile}])
		|| !missing.removeCharacter(firstId))
		return 26;
	final missingTick = missing.stepAuthoredActorControllers(0, CharacterDamagePolicy.Invulnerable);
	switch missingTick.status {
		case ControlledCharacterMissing(id) if (id == firstId && missingTick.processed == 0 && missingTick.emittedEvents == 0):
		case _:
			return 27;
	}
	final unbound = new GameSession();
	final rejectedStep = unbound.stepCharacter(firstId, {
		moveX: 0.0,
		moveZ: 0.0,
		jump: false,
		descend: false
	}, CharacterDamagePolicy.Invulnerable);
	if (rejectedStep.resolved || unbound.receiveLocalPlayerAttack().resolved)
		return 28;
	return 0;
}

/**
	Bind a local fixture without returning the session object from this helper.

	The caller keeps the session's lifetime visible in `selfCheck`, allowing
	haxe.c to prove automatic C storage instead of conservatively selecting a
	garbage-collected allocation for an object returned across a function boundary.
**/
private function bindLocal(session:GameSession, id:EntityId, x:Float, z:Float):Bool
	return session.bindLocalPlayer(character(id, x, z, 6));

/** Construct one direct character fixture using the same shared domain modules. */
private function character(id:EntityId, x:Float, z:Float, health:Int):caxecraft.domain.Character
	return startCharacter(id, createBody(x, 5.0, z), aquaticProfile(120, 4, 0.35, 14.0, 20.0, 12.0, 0.18, 20, false, false), health);

/** Use the built-in hostile recipe without coupling the scheduler to Mossling code. */
private function hostileProfile():ActorControllerProfile
	return WanderChaseMelee({
		noticeRadiusMilli: 6000,
		strikeRadiusMilli: 3000,
		attackRadiusMilli: 1400,
		windupTicks: 8,
		recoveryTicks: 12,
		stepMilli: 80,
		drop: new ContentId("caxecraft:mossling-berries")
	});

/** Read a stationary interaction ID while returning an invalid sentinel otherwise. */
private function interactionId(event:ActorControllerEvent):EntityId
	return switch event {
		case InteractionAvailable(id): id;
		case _: EntityId.invalid();
	};

/** Read a committed attack source while returning an invalid sentinel otherwise. */
private function attackId(event:ActorControllerEvent):EntityId
	return switch event {
		case LocalPlayerAttack(id): id;
		case _: EntityId.invalid();
	};

/** Read a defeated actor's drop source while rejecting every other event shape. */
private function dropId(event:ActorControllerEvent):EntityId
	return switch event {
		case DropRequested(id, _): id;
		case _: EntityId.invalid();
	};

/** Compare deterministic movement values without depending on Float text output. */
private inline function near(left:Float, right:Float):Bool {
	final difference = left - right;
	return difference >= -EPSILON && difference <= EPSILON;
}
