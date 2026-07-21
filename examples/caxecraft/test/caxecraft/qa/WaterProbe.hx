package caxecraft.qa;

import caxecraft.domain.BlockKind;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.WaterCellState;
import caxecraft.domain.WaterLevel;
import caxecraft.domain.WaterPendingCells;
import caxecraft.domain.WaterSimulation;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldStorage;
import caxecraft.domain.WorldVolume;
#if c
import c.CArray;
import c.UInt8;
#end

/**
	Executable specification for deterministic bounded voxel water.

	Eval prints the oracle. The C build executes the same checks and retains the
	public scalar functions for the strict native harness. Only fixed-storage
	construction differs at compile time; every water rule below is shared Haxe.
**/
final class WaterProbe {
	static var observed:Int = 0;

	public static function main():Void {
		#if c
		observed = selfCheck() + trace();
		#else
		Sys.println(selfCheck());
		Sys.println(trace());
		#end
	}

	/** Return zero, or the stable number of the first broken water invariant. */
	public static function selfCheck():Int {
		#if c
		var worldStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var pendingStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = worldStorage.span();
		var pending:WaterPendingCells = pendingStorage.span();
		#else
		var cells:WorldCells = zeroes();
		var pending:WaterPendingCells = zeroes();
		#end
		final water = new WaterSimulation();
		water.resetPending(pending);

		final malformed = World.coord(1, 1, 1);
		WorldStorage.writeCode(cells, World.indexOf(malformed), 99);
		final malformedState = water.cellState(cells, malformed);
		switch malformedState {
			case InvalidStorage(code):
				if (code != 99)
					return 1;
			case Empty | Blocked | Source | Flowing(_, _):
				return 1;
		}
		water.schedule(pending, malformed);
		water.tick(cells, pending, 1);
		if (WorldStorage.readCode(cells, World.indexOf(malformed)) != 99)
			return 2;

		clear(cells);
		makeFloor(cells);
		water.resetPending(pending);
		final dryFloorHash = water.stateHash(cells);
		if (!water.placeSource(cells, pending, World.coord(8, 6, 8)) || !water.settle(cells, pending, 32, 1000))
			return 10;
		if (!isFlow(water.cellState(cells, World.coord(8, 5, 8)), WaterLevel.Full, true))
			return 11;
		if (!isFlow(water.cellState(cells, World.coord(8, 1, 8)), WaterLevel.Full, false))
			return 12;
		if (!isFlow(water.cellState(cells, World.coord(9, 1, 8)), WaterLevel.One, false))
			return 13;
		if (!isFlow(water.cellState(cells, World.coord(15, 1, 8)), WaterLevel.Thin, false))
			return 14;
		if (!isEmpty(water.cellState(cells, World.coord(16, 1, 8))))
			return 15;
		if (!water.removeWater(cells, pending, World.coord(8, 6, 8)) || !water.settle(cells, pending, 32, 2000))
			return 16;
		if (water.stateHash(cells) != dryFloorHash || water.hasAnyWater(cells))
			return 17;

		clear(cells);
		makeFloor(cells);
		makeDam(cells, true);
		water.resetPending(pending);
		if (!water.placeSource(cells, pending, World.coord(8, 1, 8)) || !water.settle(cells, pending, 16, 1000))
			return 20;
		if (!isWater(water.cellState(cells, World.coord(11, 1, 8))))
			return 21;
		if (!water.replaceTerrain(cells, pending, World.coord(10, 1, 8), BlockKind.Stone) || !water.settle(cells, pending, 16, 1000))
			return 22;
		if (!isEmpty(water.cellState(cells, World.coord(11, 1, 8))))
			return 23;
		if (!water.removeTerrain(cells, pending, World.coord(10, 1, 8)) || !water.settle(cells, pending, 16, 1000))
			return 24;
		if (!isWater(water.cellState(cells, World.coord(11, 1, 8))))
			return 25;
		if (!water.replaceTerrain(cells, pending, World.coord(10, 1, 8), BlockKind.Stone) || !water.settle(cells, pending, 16, 1000))
			return 26;
		if (!isEmpty(water.cellState(cells, World.coord(11, 1, 8))))
			return 27;

		clear(cells);
		makeFloor(cells);
		World.replace(cells, World.coord(9, 1, 8), BlockKind.Stone);
		water.resetPending(pending);
		if (!water.placeSource(cells, pending, World.coord(8, 1, 8)) || !water.settle(cells, pending, 16, 1000))
			return 30;
		if (!isWater(water.cellState(cells, World.coord(10, 1, 8))))
			return 31;

		clear(cells);
		makeFloor(cells);
		water.resetPending(pending);
		water.placeSource(cells, pending, World.coord(8, 6, 8));
		final bounded = water.tick(cells, pending, 1);
		if (bounded.processed != 1 || bounded.remaining <= 0 || water.pending() != bounded.remaining)
			return 40;

		final slow = deterministicHash(1, false);
		final grouped = deterministicHash(23, true);
		if (slow == 0 || slow != grouped)
			return 41;
		return 0;
	}

	/** Stable cross-target summary used by the native differential harness. */
	public static function trace():Int {
		var hash = deterministicHash(7, false);
		hash = CaxecraftTrace.mix(hash, deterministicHash(19, true));
		return CaxecraftTrace.mix(hash, 0x57415452);
	}

	static function deterministicHash(budget:Int, reverseScheduling:Bool):Int {
		#if c
		var worldStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var pendingStorage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = worldStorage.span();
		var pending:WaterPendingCells = pendingStorage.span();
		#else
		var cells:WorldCells = zeroes();
		var pending:WaterPendingCells = zeroes();
		#end
		makeFloor(cells);
		makeDam(cells, true);
		final water = new WaterSimulation();
		water.resetPending(pending);
		water.placeSource(cells, pending, World.coord(8, 6, 8));
		if (reverseScheduling) {
			water.schedule(pending, World.coord(14, 1, 8));
			water.schedule(pending, World.coord(4, 1, 8));
		} else {
			water.schedule(pending, World.coord(4, 1, 8));
			water.schedule(pending, World.coord(14, 1, 8));
		}
		if (!water.settle(cells, pending, budget, 10000))
			return 0;
		var hash = water.stateHash(cells);
		hash = CaxecraftTrace.mix(hash, water.pending());
		return hash;
	}

	static function clear(cells:WorldCells):Void {
		var index = 0;
		while (index < World.VOLUME) {
			WorldStorage.writeCode(cells, index, 0);
			index++;
		}
	}

	static function makeFloor(cells:WorldCells):Void {
		var z = 0;
		while (z < World.DEPTH) {
			var x = 0;
			while (x < World.WIDTH) {
				World.replace(cells, World.coord(x, 0, z), BlockKind.Bedrock);
				x++;
			}
			z++;
		}
	}

	static function makeDam(cells:WorldCells, opening:Bool):Void {
		var z = 0;
		while (z < World.DEPTH) {
			World.replace(cells, World.coord(10, 1, z), BlockKind.Stone);
			z++;
		}
		if (opening)
			World.replace(cells, World.coord(10, 1, 8), BlockKind.Air);
	}

	static function isWater(state:WaterCellState):Bool {
		return switch state {
			case Source | Flowing(_, _): true;
			case Empty | Blocked | InvalidStorage(_): false;
		};
	}

	static function isEmpty(state:WaterCellState):Bool {
		return switch state {
			case Empty: true;
			case Blocked | Source | Flowing(_, _) | InvalidStorage(_): false;
		};
	}

	static function isFlow(state:WaterCellState, expected:WaterLevel, falling:Bool):Bool {
		return switch state {
			case Flowing(level, actualFalling): level == expected && actualFalling == falling;
			case Empty | Blocked | Source | InvalidStorage(_): false;
		};
	}

	#if !c
	static function zeroes():Array<Int> {
		final values:Array<Int> = [];
		var index = 0;
		while (index < World.VOLUME) {
			values.push(0);
			index++;
		}
		return values;
	}
	#end
}
