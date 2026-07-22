package caxecraft.content;

import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldStorage;

/**
 * Native level facts generated from the validated first-playable CAXEMAP.
 *
 * The checked-in map is the editable source of truth. Regenerate this typed
 * adapter with `python3 examples/caxecraft/level_adapter.py`; do not hand-edit
 * terrain runs, water placement, or spawn values here.
 */
inline final SOURCE_SHA256:String = "3a23b1229d92a2201cea8407f2e3e65b4a0f39f42371685993278c43ab53b47c";

inline final FLUID_INITIAL_VOLUME:Int = 0;
inline final FLUID_SOURCE:Int = 1;

/** Fill caller-owned world storage from canonical ascending-index runs. */
function loadTerrain(cells:WorldCells):Bool {
	var index = 0;
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 2);
	index = writeRun(cells, index, 0, 4);
	index = writeRun(cells, index, 1, 26);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 2);
	index = writeRun(cells, index, 0, 4);
	index = writeRun(cells, index, 1, 26);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 2);
	index = writeRun(cells, index, 0, 4);
	index = writeRun(cells, index, 1, 26);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 2);
	index = writeRun(cells, index, 0, 4);
	index = writeRun(cells, index, 1, 26);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
	index = writeRun(cells, index, 4, 32);
	index = writeRun(cells, index, 2, 96);
	index = writeRun(cells, index, 1, 32);
	index = writeRun(cells, index, 0, 352);
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
private function writeRun(cells:WorldCells, start:Int, code:Int, count:Int):Int {
	var index = start;
	final end = start + count;
	while (index < end) {
		WorldStorage.writeCode(cells, index, code);
		index++;
	}
	return index;
}
