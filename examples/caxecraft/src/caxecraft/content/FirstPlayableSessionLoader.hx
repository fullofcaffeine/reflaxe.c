package caxecraft.content;

import caxecraft.content.FirstPlayableLevel.FLUID_INITIAL_VOLUME;
import caxecraft.content.FirstPlayableLevel.FLUID_SOURCE;
import caxecraft.domain.GameSession;
import caxecraft.domain.World;
import caxecraft.gameplay.AuthoredItemSlots;

/**
	Facts the application needs after the first playable level is fully assembled.

	The session itself remains owned by the caller. This small immutable record
	contains only the validated presentation fact that is not simulation state.
	`valid == false` means the candidate session must be discarded.
**/
typedef FirstPlayableSessionLoad = {
	final valid:Bool;
	final waterPresentationCell:Int;
}

/**
	Validate and assemble the temporary built-in level into an unpublished session.

	`FirstPlayableLevel` is generated from the checked-in CAXEMAP while runtime file
	loading is still being built. This bridge checks every scalar fact before the
	first write, then asks `GameSession` to create its own short-lived storage views.
	A failure may leave the candidate partially filled, but the caller has not
	published it and must discard it. No mutable span crosses this module boundary.
**/
function loadCandidate(session:GameSession):FirstPlayableSessionLoad {
	final presentationCell = validateFacts();
	if (presentationCell < 0)
		return invalidLoad();

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
	return {valid: true, waterPresentationCell: presentationCell};
}

/** Return the water atlas cell, or `-1` when any generated fact is invalid. */
private function validateFacts():Int {
	if (!insideWorld(FirstPlayableLevel.spawnXMilli(), FirstPlayableLevel.spawnYMilli(), FirstPlayableLevel.spawnZMilli()))
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

/** Construct the single fail-closed result without nullable partial state. */
private inline function invalidLoad():FirstPlayableSessionLoad
	return {valid: false, waterPresentationCell: -1};
