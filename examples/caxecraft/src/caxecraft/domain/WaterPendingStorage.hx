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
