package caxecraft.content;

import caxecraft.domain.GameSession;
import caxecraft.domain.World;

/**
 * Native level facts generated from the validated first-playable CAXEMAP.
 *
 * The checked-in map is the editable source of truth. Regenerate this typed
 * adapter with `python3 examples/caxecraft/level_adapter.py`; do not hand-edit
 * terrain runs, fluid/item placement, or spawn values here.
 *
 * This temporary adapter proves that validated authored data can drive the
 * native game while file and text parsing support is still incomplete. Bead
 * `haxe_c-xge.39` owns replacing it with runtime CAXEMAP loading; new gameplay
 * content must not make this generated module a permanent application API.
 */
inline final SOURCE_SHA256:String = "f1e992a434edb24bed0ff2d114fc3e6fd88ec7bfc495c6b2a22e99b14732dbc5";

inline final FLUID_INITIAL_VOLUME:Int = 0;
inline final FLUID_SOURCE:Int = 1;

/** Fill an unpublished session from canonical ascending-index terrain runs. */
function loadTerrain(session:GameSession):Bool {
	var index = 0;
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 1);
	index = writeRun(session, index, 5, 6);
	index = writeRun(session, index, 1, 25);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 0, 4);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 1, 24);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 0, 4);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 1, 24);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 0, 4);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 1, 24);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 0, 4);
	index = writeRun(session, index, 5, 2);
	index = writeRun(session, index, 1, 24);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 1);
	index = writeRun(session, index, 5, 6);
	index = writeRun(session, index, 1, 25);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 89);
	index = writeRun(session, index, 7, 3);
	index = writeRun(session, index, 0, 260);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 26);
	index = writeRun(session, index, 6, 1);
	index = writeRun(session, index, 0, 31);
	index = writeRun(session, index, 6, 1);
	index = writeRun(session, index, 0, 30);
	index = writeRun(session, index, 7, 3);
	index = writeRun(session, index, 0, 30);
	index = writeRun(session, index, 7, 1);
	index = writeRun(session, index, 0, 229);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 89);
	index = writeRun(session, index, 7, 3);
	index = writeRun(session, index, 0, 260);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 1, 32);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	index = writeRun(session, index, 4, 32);
	index = writeRun(session, index, 2, 96);
	index = writeRun(session, index, 8, 10);
	index = writeRun(session, index, 1, 12);
	index = writeRun(session, index, 9, 10);
	index = writeRun(session, index, 0, 352);
	return index == World.VOLUME;
}

/** Number of validated authored fluid declarations. */
inline function fluidCount():Int
	return 2;

/** Return fluid kind for one index, or `-1` outside the generated set. */
function fluidKind(index:Int):Int {
	if (index == 0)
		return 0;
	if (index == 1)
		return 1;
	return -1;
}

/** Return fluid x coordinate for one index, or `-1` outside the generated set. */
function fluidX(index:Int):Int {
	if (index == 0)
		return 2;
	if (index == 1)
		return 3;
	return -1;
}

/** Return fluid y coordinate for one index, or `-1` outside the generated set. */
function fluidY(index:Int):Int {
	if (index == 0)
		return 4;
	if (index == 1)
		return 4;
	return -1;
}

/** Return fluid z coordinate for one index, or `-1` outside the generated set. */
function fluidZ(index:Int):Int {
	if (index == 0)
		return 2;
	if (index == 1)
		return 3;
	return -1;
}

/** Return fluid width for one index, or `-1` outside the generated set. */
function fluidWidth(index:Int):Int {
	if (index == 0)
		return 4;
	if (index == 1)
		return 1;
	return -1;
}

/** Return fluid height for one index, or `-1` outside the generated set. */
function fluidHeight(index:Int):Int {
	if (index == 0)
		return 1;
	if (index == 1)
		return 1;
	return -1;
}

/** Return fluid depth for one index, or `-1` outside the generated set. */
function fluidDepth(index:Int):Int {
	if (index == 0)
		return 4;
	if (index == 1)
		return 1;
	return -1;
}

/** Return fluid presentation atlas cell for one index, or `-1` outside the generated set. */
function fluidPresentationCell(index:Int):Int {
	if (index == 0)
		return 5;
	if (index == 1)
		return 5;
	return -1;
}

/** Number of validated authored item placements. */
inline function itemCount():Int
	return 1;

/** Positive fixed-array capacity, including for a level with no items. */
inline function itemStorageCapacity():Int
	return 1;

/** Return item pack-local storage code for one index, or `-1` outside the generated set. */
function itemStorageCode(index:Int):Int {
	if (index == 0)
		return 8;
	return -1;
}

/** Return item quantity for one index, or `-1` outside the generated set. */
function itemQuantity(index:Int):Int {
	if (index == 0)
		return 1;
	return -1;
}

/** Return item x coordinate in milliblocks for one index, or `-1` outside the generated set. */
function itemXMilli(index:Int):Int {
	if (index == 0)
		return 4500;
	return -1;
}

/** Return item y coordinate in milliblocks for one index, or `-1` outside the generated set. */
function itemYMilli(index:Int):Int {
	if (index == 0)
		return 5000;
	return -1;
}

/** Return item z coordinate in milliblocks for one index, or `-1` outside the generated set. */
function itemZMilli(index:Int):Int {
	if (index == 0)
		return 4500;
	return -1;
}

/** Return item yaw in whole degrees for one index, or `-1` outside the generated set. */
function itemYawDegrees(index:Int):Int {
	if (index == 0)
		return 0;
	return -1;
}

/** Authored player spawn x coordinate in integer milliblocks. */
inline function spawnXMilli():Int
	return 16500;

/** Authored player spawn y coordinate in integer milliblocks. */
inline function spawnYMilli():Int
	return 5000;

/** Authored player spawn z coordinate in integer milliblocks. */
inline function spawnZMilli():Int
	return 16500;

/** Authored player spawn yaw in whole degrees. */
inline function spawnYawDegrees():Int
	return 0;

/** Write one already validated compact run and return the next index. */
private function writeRun(session:GameSession, start:Int, code:Int, count:Int):Int {
	if (start < 0)
		return -1;
	return session.writeTerrainRunDuringLoad(start, code, count);
}
