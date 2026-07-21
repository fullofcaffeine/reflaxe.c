package caxecraft.domain;

#if c
import c.IntConvert;
#end

/**
	Target-specific byte access for the deterministic water work set.

	This is a storage adapter, not game logic. Keeping the two conversions here
	lets `WaterSimulation` remain the same ordinary Haxe on Eval and C.
**/
/** Return whether one valid world index is already waiting for work. */
function isMarked(cells:WaterPendingCells, index:Int):Bool {
	#if c
	return IntConvert.exact(cells[index]) != 0;
	#else
	return cells[index] != 0;
	#end
}

/** Store one queue mark using the exact carrier selected for this target. */
function setMarked(cells:WaterPendingCells, index:Int, marked:Bool):Void {
	#if c
	cells[index] = IntConvert.modulo(marked ? 1 : 0);
	#else
	cells[index] = marked ? 1 : 0;
	#end
}
