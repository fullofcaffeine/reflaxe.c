package caxecraft.domain;

import caxecraft.domain.ActorControllerDecision.ActorControllerPlanError;
import caxecraft.domain.ActorControllerDecision.ActorControllerDecision;
import caxecraft.domain.ActorControllerEvent.*;
import caxecraft.domain.ActorControllerPhase.*;
import caxecraft.domain.ActorControllerProfile.WanderChaseMeleeProfile;
import caxecraft.domain.Character.isValid as isValidCharacter;
import caxecraft.domain.Vitals.isDefeated as characterIsDefeated;

/**
	Calculates reusable authored-actor behavior without mutating game state.

	The scheduler sees immutable character/controller/player snapshots and returns
	one `CharacterIntent`, next controller snapshot, and optional typed event.
	`GameSession` separately commits those results in authored order. Keeping this
	module pure makes the state machine identical under Eval, generated C, tests,
	and later save/replay tools.
**/
private inline final HOME_EPSILON_SQUARED:Float = 0.0001;

private inline final WANDER_RADIUS:Float = 1.0;
private inline final WANDER_CYCLE_TICKS:Int = 240;
private inline final CHARACTER_STEP_MILLI:Int = 200;

/** Capture the initial controller state beside a newly published character. */
function startActorController(binding:ActorControllerBinding, character:Character):ActorControllerState {
	final phase = switch binding.profile {
		case StationaryDialogue(_): Stationary;
		case WanderChaseMelee(_): Resting;
	};
	return makeState(binding.characterId, binding.profile, character.body.x, character.body.z, phase, 0, false);
}

/**
	Plan one actor against the local-player snapshot from this fixed tick's start.

	A mismatched identity or impossible profile/phase pair is rejected before any
	intent can be committed. Distances use squared milliblock radii, avoiding square
	roots while preserving the exact content-authored boundary.
**/
function planActorController(state:ActorControllerState, character:Character, localPlayer:Character, tickNumber:Int):ActorControllerDecision {
	if (!isValidCharacter(character) || !isValidCharacter(localPlayer) || state.characterId != character.id)
		return ControllerPlanRejected(CharacterIdentityMismatch);
	return switch state.profile {
		case StationaryDialogue(interactionRadiusMilli):
			if (state.phase != Stationary || state.phaseTicks != 0 || state.dropPublished) ControllerPlanRejected(ProfileStateMismatch); else {
				final inRange = insideRadius(character.body.x, character.body.z, localPlayer.body.x, localPlayer.body.z, interactionRadiusMilli);
				ControllerPlanned(state, stillIntent(), inRange ? InteractionAvailable(character.id) : NoControllerEvent);
			}
		case WanderChaseMelee(profile):
			if (state.phase == Stationary) ControllerPlanRejected(ProfileStateMismatch); else planWanderChaseMelee(state, character, localPlayer, tickNumber,
				profile);
	};
}

/** Advance the closed hostile-controller state machine by one exact fixed tick. */
private function planWanderChaseMelee(state:ActorControllerState, character:Character, localPlayer:Character, tickNumber:Int,
		profile:WanderChaseMeleeProfile):ActorControllerDecision {
	if (characterIsDefeated(character.vitals)) {
		final event = state.dropPublished ? NoControllerEvent : DropRequested(character.id, profile.drop);
		return ControllerPlanned(withPhase(state, Defeated, 0, true), stillIntent(), event);
	}
	if (state.phase == Defeated || state.dropPublished)
		return ControllerPlanRejected(ProfileStateMismatch);

	final playerDistanceSquared = distanceSquaredMilli(character.body.x, character.body.z, localPlayer.body.x, localPlayer.body.z);
	final attackRadiusSquared = squared(profile.attackRadiusMilli);
	if (state.phase == Windup) {
		if (playerDistanceSquared > attackRadiusSquared)
			return ControllerPlanned(withPhase(state, Chasing, 0, false), stillIntent(), NoControllerEvent);
		if (state.phaseTicks > 1)
			return ControllerPlanned(withPhase(state, Windup, state.phaseTicks - 1, false), stillIntent(), NoControllerEvent);
		return ControllerPlanned(withPhase(state, Recovering, profile.recoveryTicks, false), stillIntent(), LocalPlayerAttack(character.id));
	}
	if (state.phase == Recovering && state.phaseTicks > 1)
		return ControllerPlanned(withPhase(state, Recovering, state.phaseTicks - 1, false), stillIntent(), NoControllerEvent);
	if (playerDistanceSquared <= attackRadiusSquared)
		return ControllerPlanned(withPhase(state, Windup, profile.windupTicks, false), stillIntent(), NoControllerEvent);

	final homeDx = state.homeX - character.body.x;
	final homeDz = state.homeZ - character.body.z;
	final homeDistanceSquared = homeDx * homeDx + homeDz * homeDz;
	var nextPhase = Resting;
	var targetX = character.body.x;
	var targetZ = character.body.z;
	if (playerDistanceSquared <= squared(profile.noticeRadiusMilli)) {
		nextPhase = Chasing;
		targetX = localPlayer.body.x;
		targetZ = localPlayer.body.z;
	} else if ((state.phase == Chasing || state.phase == Returning) && homeDistanceSquared > HOME_EPSILON_SQUARED) {
		nextPhase = Returning;
		targetX = state.homeX;
		targetZ = state.homeZ;
	} else {
		var cycleTick = tickNumber % WANDER_CYCLE_TICKS;
		if (cycleTick < 0)
			cycleTick += WANDER_CYCLE_TICKS;
		targetX = state.homeX;
		targetZ = state.homeZ;
		if (cycleTick >= 20 && cycleTick < 60)
			targetX += WANDER_RADIUS;
		else if (cycleTick >= 60 && cycleTick < 100) {
			targetX += WANDER_RADIUS;
			targetZ += WANDER_RADIUS;
		} else if (cycleTick >= 100 && cycleTick < 140) {
			targetX -= WANDER_RADIUS;
			targetZ += WANDER_RADIUS;
		} else if (cycleTick >= 140 && cycleTick < 180) {
			targetX -= WANDER_RADIUS;
			targetZ -= WANDER_RADIUS;
		} else if (cycleTick >= 180 && cycleTick < 220)
			targetZ -= WANDER_RADIUS;
		if (distanceSquared(character.body.x, character.body.z, targetX, targetZ) > HOME_EPSILON_SQUARED)
			nextPhase = Wandering;
	}

	if (nextPhase == Resting)
		return ControllerPlanned(withPhase(state, Resting, 0, false), stillIntent(), NoControllerEvent);
	final dx = targetX - character.body.x;
	final dz = targetZ - character.body.z;
	final amount = profile.stepMilli / CHARACTER_STEP_MILLI;
	final moveX = absolute(dx) >= absolute(dz) ? boundedDirection(dx) * amount : 0.0;
	final moveZ = absolute(dx) >= absolute(dz) ? 0.0 : boundedDirection(dz) * amount;
	return ControllerPlanned(withPhase(state, nextPhase, 0, false), {
		moveX: moveX,
		moveZ: moveZ,
		jump: false,
		descend: false
	}, NoControllerEvent);
}

/** Construct one immutable controller snapshot with stable binding and home. */
private inline function makeState(characterId:EntityId, profile:ActorControllerProfile, homeX:Float, homeZ:Float, phase:ActorControllerPhase, phaseTicks:Int,
		dropPublished:Bool):ActorControllerState
	return {
		characterId: characterId,
		profile: profile,
		homeX: homeX,
		homeZ: homeZ,
		phase: phase,
		phaseTicks: phaseTicks,
		dropPublished: dropPublished
	};

/** Preserve binding/home fields while replacing only per-tick execution state. */
private inline function withPhase(state:ActorControllerState, phase:ActorControllerPhase, phaseTicks:Int, dropPublished:Bool):ActorControllerState
	return makeState(state.characterId, state.profile, state.homeX, state.homeZ, phase, phaseTicks, dropPublished);

/** Zero controller input still advances shared gravity, water, breath, and vitals. */
private inline function stillIntent():CharacterIntent
	return {
		moveX: 0.0,
		moveZ: 0.0,
		jump: false,
		descend: false
	};

/** Compare world-space positions against one radius authored in milliblocks. */
private inline function insideRadius(leftX:Float, leftZ:Float, rightX:Float, rightZ:Float, radiusMilli:Int):Bool
	return distanceSquaredMilli(leftX, leftZ, rightX, rightZ) <= squared(radiusMilli);

/** Squared distance in milliblocks, matching the integer content profile. */
private inline function distanceSquaredMilli(leftX:Float, leftZ:Float, rightX:Float, rightZ:Float):Float {
	final dx = (rightX - leftX) * 1000.0;
	final dz = (rightZ - leftZ) * 1000.0;
	return dx * dx + dz * dz;
}

private inline function squared(value:Int):Float
	return value * 1.0 * value;

private inline function distanceSquared(leftX:Float, leftZ:Float, rightX:Float, rightZ:Float):Float {
	final dx = rightX - leftX;
	final dz = rightZ - leftZ;
	return dx * dx + dz * dz;
}

private inline function boundedDirection(delta:Float):Float
	return delta < 0.0 ? -1.0 : delta > 0.0 ? 1.0 : 0.0;

private inline function absolute(value:Float):Float
	return value < 0.0 ? -value : value;
