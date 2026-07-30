package caxecraft.content;

import caxecraft.content.ActorCompositionPlanner.ActorCompositionResult;
import caxecraft.content.ActorCompositionPlanner.CharacterSpawnRole;
import caxecraft.content.ActorCompositionPlanner.planActorComposition;
import caxecraft.content.ActorPublication.ActorPublicationResult;
import caxecraft.content.ActorPublication.publishActorPlans;
import caxecraft.content.BaseContentPack.BaseContentRegistry;
import caxecraft.content.FirstPlayableLevel.FLUID_INITIAL_VOLUME;
import caxecraft.content.FirstPlayableLevel.FLUID_SOURCE;
import caxecraft.domain.Character.start as startCharacter;
import caxecraft.domain.CharacterPhysics.body as createBody;
import caxecraft.domain.CharacterPhysics.recoverSpawn as recoverPlayerSpawn;
import caxecraft.domain.EntityId;
import caxecraft.domain.GameSession;
import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.domain.World;
import caxecraft.gameplay.AuthoredItemSlots;
import caxecraft.scenario.ScenarioId;

/**
	Connects one generated authored actor ID to its live session ID.

	The role lets the current presentation select its one dialogue and enemy
	slots without checking Nia, Mossling, content, or map names. Callers still
	use typed `GameSession` commands for every observation and mutation.
**/
typedef LoadedActorBinding = {
	final authoredId:ScenarioId;
	final entityId:EntityId;
	final role:CharacterSpawnRole;
}

/**
	Facts the application needs after the first playable candidate is assembled.

	The caller retains ownership of `GameSession`. This result contains only
	read-only presentation bindings derived from validated authored data.
	`valid == false` means the candidate may be partially filled and must be
	discarded before it becomes the live application session.
**/
typedef FirstPlayableSessionLoad = {
	final valid:Bool;
	final waterPresentationCell:Int;
	final actors:Array<LoadedActorBinding>;
}

/**
	Build the temporary first-playable projection into an unpublished session.

	The generated module contains validated CAXEMAP facts while native package
	reading is unfinished. This function checks scalar facts and resolves every
	authored actor before the first session write, then fills one caller-owned
	candidate, binds its local player, and atomically publishes its actor set.

	A later storage or publication failure rejects the complete candidate; the
	caller must discard it rather than repairing or exposing partial state.
	`haxe_c-xge.20.4.3.6` removes this generated bridge after the executable reads
	and validates the map bytes itself.
**/
function loadCandidate(session:GameSession, initialHealth:Int):FirstPlayableSessionLoad {
	final presentationCell = validateFacts(initialHealth);
	if (presentationCell < 0)
		return invalidLoad();

	final actorPlans = switch planActorComposition(FirstPlayableLevel.actorObjects(), new BaseContentRegistry()) {
		case ActorCompositionPlanned(plans): plans;
		case ActorCompositionRejected(_): return invalidLoad();
	};
	final actorBindings:Array<LoadedActorBinding> = [];
	for (actor in actorPlans)
		actorBindings.push({
			authoredId: actor.authoredId,
			entityId: actor.entityId,
			role: actor.role
		});

	session.resetEmptyWorld();
	if (!FirstPlayableLevel.loadTerrain(session))
		return invalidLoad();

	var fluidIndex = 0;
	while (fluidIndex < FirstPlayableLevel.fluidCount()) {
		final coordinate = World.coord(FirstPlayableLevel.fluidX(fluidIndex), FirstPlayableLevel.fluidY(fluidIndex), FirstPlayableLevel.fluidZ(fluidIndex));
		final loaded = if (FirstPlayableLevel.fluidKind(fluidIndex) == FLUID_INITIAL_VOLUME) session.placeInitialWaterVolume(coordinate,
			FirstPlayableLevel.fluidWidth(fluidIndex), FirstPlayableLevel.fluidHeight(fluidIndex),
			FirstPlayableLevel.fluidDepth(fluidIndex)); else session.placeWaterSource(coordinate);
		if (!loaded)
			return invalidLoad();
		fluidIndex++;
	}

	var itemIndex = 0;
	while (itemIndex < FirstPlayableLevel.itemCount()) {
		if (!session.activateAuthoredItemDuringLoad(itemIndex))
			return invalidLoad();
		itemIndex++;
	}

	final spawnBody = recoverPlayerSpawn(session.worldView(),
		createBody(FirstPlayableLevel.spawnXMilli() / 1000.0, FirstPlayableLevel.spawnYMilli() / 1000.0, FirstPlayableLevel.spawnZMilli() / 1000.0));
	if (!session.bindLocalPlayer(startCharacter(EntityId.fromValidatedStorageCode(1), spawnBody,
		BaseContentPack.aquaticProfile(BaseContentPack.defaultAquaticProfile()), initialHealth)))
		return invalidLoad();

	switch publishActorPlans(session, actorPlans) {
		case ActorsPublished(count) if (count == actorBindings.length):
		case ActorsPublished(_) | ActorPublicationRejected(_):
			return invalidLoad();
	}
	return {
		valid: true,
		waterPresentationCell: presentationCell,
		actors: actorBindings
	};
}

/** Return the water atlas cell, or `-1` when any generated fact is invalid. */
private function validateFacts(initialHealth:Int):Int {
	if (initialHealth <= 0 || initialHealth > MAX_HEALTH)
		return -1;
	if (!insideWorld(FirstPlayableLevel.spawnXMilli(), FirstPlayableLevel.spawnYMilli(), FirstPlayableLevel.spawnZMilli())
		|| FirstPlayableLevel.spawnYawDegrees() < 0
		|| FirstPlayableLevel.spawnYawDegrees() > 359)
		return -1;
	if (FirstPlayableLevel.itemCount() < 0 || FirstPlayableLevel.itemCount() > AuthoredItemSlots.CAPACITY)
		return -1;

	var itemIndex = 0;
	while (itemIndex < FirstPlayableLevel.itemCount()) {
		if (!BaseContentPack.isValidItemStorageCode(FirstPlayableLevel.itemStorageCode(itemIndex))
			|| FirstPlayableLevel.itemQuantity(itemIndex) <= 0
			|| !insideWorld(FirstPlayableLevel.itemXMilli(itemIndex), FirstPlayableLevel.itemYMilli(itemIndex), FirstPlayableLevel.itemZMilli(itemIndex)))
			return -1;
		itemIndex++;
	}

	var presentationCell = -1;
	var fluidIndex = 0;
	while (fluidIndex < FirstPlayableLevel.fluidCount()) {
		final kind = FirstPlayableLevel.fluidKind(fluidIndex);
		final width = FirstPlayableLevel.fluidWidth(fluidIndex);
		final height = FirstPlayableLevel.fluidHeight(fluidIndex);
		final depth = FirstPlayableLevel.fluidDepth(fluidIndex);
		final x = FirstPlayableLevel.fluidX(fluidIndex);
		final y = FirstPlayableLevel.fluidY(fluidIndex);
		final z = FirstPlayableLevel.fluidZ(fluidIndex);
		final cell = FirstPlayableLevel.fluidPresentationCell(fluidIndex);
		if ((kind != FLUID_INITIAL_VOLUME && kind != FLUID_SOURCE)
			|| width <= 0
			|| height <= 0
			|| depth <= 0
			|| x < 0
			|| y < 0
			|| z < 0
			|| x > World.WIDTH - width
			|| y > World.HEIGHT - height
			|| z > World.DEPTH - depth
			|| cell < 0)
			return -1;
		if (presentationCell < 0)
			presentationCell = cell;
		else if (presentationCell != cell)
			return -1;
		fluidIndex++;
	}
	return presentationCell;
}

/** Test one position expressed in the authored format's integer milliblocks. */
private inline function insideWorld(x:Int, y:Int, z:Int):Bool
	return x >= 0 && x < World.WIDTH * 1000 && y >= 0 && y < World.HEIGHT * 1000 && z >= 0 && z < World.DEPTH * 1000;

/** Construct one fail-closed result without nullable partial state. */
private inline function invalidLoad():FirstPlayableSessionLoad
	return {valid: false, waterPresentationCell: -1, actors: []};
