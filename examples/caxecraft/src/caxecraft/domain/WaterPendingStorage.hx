package caxecraft.domain;

#if c
import c.IntConvert;
#end

/**
	Target-specific byte access for the deterministic water work set.

	This is a storage adapter, not game logic. The owner operations access the
	live array embedded in `WaterSimulation`. The snapshot operations access a
	short-lived caller buffer during save or restore. Keeping the exact-byte
	conversions here lets the scheduler remain ordinary shared Haxe.
**/
/** Return whether one live queue index is waiting for work. */
inline function ownerIsMarked(storage:WaterPendingOwner, index:Int):Bool {
	#if c
	return IntConvert.exact(storage[index]) != 0;
	#else
	return storage[index] != 0;
	#end
}

/** Store one mark in the queue owned by the simulation. */
inline function setOwnerMarked(storage:WaterPendingOwner, index:Int, marked:Bool):Void {
	#if c
	storage[index] = IntConvert.modulo(marked ? 1 : 0);
	#else
	storage[index] = marked ? 1 : 0;
	#end
}

/** Read one mark from a caller-owned snapshot buffer. */
inline function snapshotIsMarked(cells:WaterPendingCells, index:Int):Bool {
	#if c
	return IntConvert.exact(cells[index]) != 0;
	#else
	return cells[index] != 0;
	#end
}

/** Write one normalized mark into a caller-owned snapshot buffer. */
inline function setSnapshotMarked(cells:WaterPendingCells, index:Int, marked:Bool):Void {
	#if c
	cells[index] = IntConvert.modulo(marked ? 1 : 0);
	#else
	cells[index] = marked ? 1 : 0;
	#end
}
