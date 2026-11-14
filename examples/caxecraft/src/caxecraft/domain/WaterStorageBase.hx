package caxecraft.domain;

#if c
import c.CArray;
import c.IntConvert;
import c.UInt8;
#end

/**
	Selects pending-mark storage for one `WaterSimulation` owner.

	The base exposes only exact mark operations to its cohesive simulation.
	Scheduling order, counters, snapshots, and water rules remain in the shared
	`WaterSimulation` source.
**/
class WaterStorageBase {
	#if c
	private final pendingStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	#else
	private final pendingStorage:Array<Int> = [];
	#end

	/** Construct one zeroed mark for every world cell. */
	public function new() {
		#if !c
		var index = 0;
		while (index < World.VOLUME) {
			pendingStorage.push(0);
			index++;
		}
		#end
	}

	/** Return whether one validated queue index is marked. */
	@:allow(caxecraft.domain.WaterSimulation)
	private inline function pendingIsMarked(index:Int):Bool {
		#if c
		return IntConvert.exact(pendingStorage[index]) != 0;
		#else
		return pendingStorage[index] != 0;
		#end
	}

	/** Store one normalized pending-work mark. */
	@:allow(caxecraft.domain.WaterSimulation)
	private inline function setPendingMarked(index:Int, marked:Bool):Void {
		#if c
		pendingStorage[index] = IntConvert.modulo(marked ? 1 : 0);
		#else
		pendingStorage[index] = marked ? 1 : 0;
		#end
	}
}
