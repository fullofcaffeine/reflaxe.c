package caxecraft.domain;

import caxecraft.domain.WaterCellCodec.decode as decodeWaterCell;
import caxecraft.domain.WaterCellCodec.emptyCode;
import caxecraft.domain.WaterCellCodec.encodeFlow;
import caxecraft.domain.WaterCellCodec.FLOW_LEVEL_COUNT;
import caxecraft.domain.WaterCellCodec.isSolidCode;
import caxecraft.domain.WaterCellCodec.isValidCode;
import caxecraft.domain.WaterCellCodec.isWaterCode;
import caxecraft.domain.WaterCellCodec.sourceCode;
import caxecraft.domain.WaterCellCodec.stateAt as waterStateAt;
#if c
import c.CArray;
#end

/**
	Deterministic, bounded voxel water for the target-neutral game domain.

	A source is permanent until explicitly removed. Other water is recomputed
	from nearby sources and flows: water falls first, settles when terrain blocks
	the cell below, then spreads horizontally with one less strength per voxel.
	Opening or closing terrain marks only the changed cell and its six neighbors;
	changes keep propagating locally until the world settles.

	Pending work is stored as one mark per world cell. The lowest marked world
	index always runs next, independent of the order in which callers requested
	updates. `tick` processes no more than its explicit budget, so a large leak
	continues over later fixed ticks instead of blocking rendering.

	The finite world has sealed edges. Sources do not renew from neighboring
	sources in this first model: authored source cells are the only permanent
	water. Rendering, player physics, save files, and campaign content consume
	this state in later vertical slices; they never advance it themselves.

	A class is used because the scheduler owns counters and queue marks that must
	persist and agree across ticks. A pure module function would make every caller
	carry and commit those invariants separately. The class remains `final` and
	owns its queue storage directly. It does not retain the shared world: each
	operation borrows that storage only for the call, making mutation explicit and
	allowing tests or future sessions to supply another world safely. Haxe.c embeds
	the simulation and its fixed queue array in the session C struct without a heap
	allocation.
**/
final class WaterSimulation {
	var pendingCount:Int;
	var lowestPending:Int;

	#if c
	/** One compact work mark per voxel, owned beside the scheduler counters. */
	final pendingStorage:WaterPendingOwner = CArray.zero(World.VOLUME);
	#else

	/** Eval carrier for the same queue ownership and deterministic behavior. */
	final pendingStorage:WaterPendingOwner = [];
	#end

	/**
		Construct one empty scheduler and its matching queue storage.

		The simulation owns the queue because its marks and counters form one
		invariant. The world remains caller-owned and is borrowed only by operations
		that read or change it.
	**/
	public function new() {
		#if !c
		var index = 0;
		while (index < World.VOLUME) {
			pendingStorage.push(0);
			index++;
		}
		#end
		pendingCount = 0;
		lowestPending = World.VOLUME;
	}

	/** Clear scheduled work without changing terrain or water bytes. */
	public function resetPending():Void {
		var index = 0;
		while (index < World.VOLUME) {
			WaterPendingStorage.setOwnerMarked(pendingStorage, index, false);
			index++;
		}
		pendingCount = 0;
		lowestPending = World.VOLUME;
	}

	/**
		Copy the exact unfinished-work set into caller-owned snapshot storage.

		The destination is borrowed only during this call. Save code receives the
		marks but cannot retain or mutate the simulation's live queue.
	**/
	public function capturePending(destination:WaterPendingCells):Void {
		var index = 0;
		while (index < World.VOLUME) {
			WaterPendingStorage.setSnapshotMarked(destination, index, WaterPendingStorage.ownerIsMarked(pendingStorage, index));
			index++;
		}
	}

	/**
		Replace the live work set from validated caller-owned snapshot storage.

		A save owns the marks because they determine which unfinished fluid work
		runs next. The counters are derived state: scanning in ascending world-index
		order reproduces the same next cell without persisting an implementation
		detail separately.
	**/
	public function restorePending(source:WaterPendingCells):Void {
		pendingCount = 0;
		lowestPending = World.VOLUME;
		var index = 0;
		while (index < World.VOLUME) {
			final marked = WaterPendingStorage.snapshotIsMarked(source, index);
			WaterPendingStorage.setOwnerMarked(pendingStorage, index, marked);
			if (marked) {
				pendingCount++;
				if (lowestPending == World.VOLUME)
					lowestPending = index;
			}
			index++;
		}
	}

	/** Return the typed water meaning at one coordinate. */
	public function cellState(cells:WorldCells, coord:BlockCoord):WaterCellState {
		return waterStateAt(cells, coord);
	}

	/** Number of distinct cells still waiting for bounded work. */
	public inline function pending():Int
		return pendingCount;

	/**
		Place one permanent source into empty or existing water.

		Solid and malformed cells reject the edit. Neighbor recomputation begins
		immediately but remains subject to the next `tick` budget.
	**/
	public function placeSource(cells:WorldCells, coord:BlockCoord):Bool {
		final index = World.indexOf(coord);
		if (index < 0)
			return false;
		return switch decodeWaterCell(WorldStorage.readCode(cells, index)) {
			case Blocked | InvalidStorage(_): false;
			case Empty | Source | Flowing(_, _):
				WorldStorage.writeCode(cells, index, sourceCode());
				scheduleAround(coord);
				true;
		};
	}

	/**
		Fill one finite authored box with non-source water.

		The complete box is checked before the first write, so an out-of-bounds box,
		a solid overlap, or malformed world byte leaves the world unchanged. Existing
		sources survive overlapping declarations. Only cells just outside the box are
		scheduled: a sealed pool stays full, while an open face begins leaking on
		later bounded ticks. Permanent replenishment still requires `placeSource`.
	**/
	public function placeInitialVolume(cells:WorldCells, origin:BlockCoord, width:Int, height:Int, depth:Int):Bool {
		if (width <= 0 || height <= 0 || depth <= 0 || origin.x < 0 || origin.y < 0 || origin.z < 0 || origin.x > World.WIDTH - width
			|| origin.y > World.HEIGHT - height || origin.z > World.DEPTH - depth)
			return false;

		var z = origin.z;
		while (z < origin.z + depth) {
			var y = origin.y;
			while (y < origin.y + height) {
				var x = origin.x;
				while (x < origin.x + width) {
					final code = WorldStorage.readCode(cells, World.indexOf(World.coord(x, y, z)));
					if (!isValidCode(code) || isSolidCode(code))
						return false;
					x++;
				}
				y++;
			}
			z++;
		}

		z = origin.z;
		while (z < origin.z + depth) {
			var y = origin.y;
			while (y < origin.y + height) {
				var x = origin.x;
				while (x < origin.x + width) {
					final index = World.indexOf(World.coord(x, y, z));
					if (WorldStorage.readCode(cells, index) != sourceCode())
						WorldStorage.writeCode(cells, index, encodeFlow(0, false));
					x++;
				}
				y++;
			}
			z++;
		}
		scheduleOutsideVolume(origin, width, height, depth);
		return true;
	}

	/** Remove water without removing a solid cell that occupies the coordinate. */
	public function removeWater(cells:WorldCells, coord:BlockCoord):Bool {
		final index = World.indexOf(coord);
		if (index < 0)
			return false;
		return switch decodeWaterCell(WorldStorage.readCode(cells, index)) {
			case Source | Flowing(_, _):
				WorldStorage.writeCode(cells, index, emptyCode());
				scheduleAround(coord);
				true;
			case Empty | Blocked | InvalidStorage(_): false;
		};
	}

	/**
		Replace a voxel with validated solid terrain and invalidate nearby water.

		This is the dam-building hook. Water disappears from the occupied voxel and
		neighboring cells re-evaluate whether they still have a feed or a new route.
	**/
	public function replaceTerrain(cells:WorldCells, coord:BlockCoord, kind:BlockKind):Bool {
		if (!World.isSolid(kind) || !World.replace(cells, coord, kind))
			return false;
		scheduleAround(coord);
		return true;
	}

	/** Remove solid terrain and let nearby water test the new opening. */
	public function removeTerrain(cells:WorldCells, coord:BlockCoord):Bool {
		if (!World.contains(coord) || !World.isSolid(World.query(cells, coord)))
			return false;
		if (!World.replace(cells, coord, BlockKind.Air))
			return false;
		scheduleAround(coord);
		return true;
	}

	/** Place ordinary buildable terrain into air or water and schedule repair. */
	public function placeTerrain(cells:WorldCells, coord:BlockCoord, kind:BlockKind):Bool {
		if (!World.isPlaceable(kind))
			return false;
		final index = World.indexOf(coord);
		if (index < 0)
			return false;
		final code = WorldStorage.readCode(cells, index);
		if (!isValidCode(code) || isSolidCode(code))
			return false;
		WorldStorage.writeCode(cells, index, World.kindCode(kind));
		scheduleAround(coord);
		return true;
	}

	/** Notify water after another transactional mechanic removed terrain. */
	public function terrainChanged(coord:BlockCoord):Void
		scheduleAround(coord);

	/** Add one cell to the duplicate-free deterministic work set. */
	public function schedule(coord:BlockCoord):Bool {
		final index = World.indexOf(coord);
		if (index < 0 || WaterPendingStorage.ownerIsMarked(pendingStorage, index))
			return false;
		WaterPendingStorage.setOwnerMarked(pendingStorage, index, true);
		pendingCount++;
		if (index < lowestPending)
			lowestPending = index;
		return true;
	}

	/** Process at most `maximumUpdates` cells and retain unfinished work. */
	public function tick(cells:WorldCells, maximumUpdates:Int):WaterTickResult {
		var processed = 0;
		var changed = 0;
		while (processed < maximumUpdates && pendingCount > 0) {
			final index = takeLowestPending();
			if (index < 0)
				break;
			if (recompute(cells, index))
				changed++;
			processed++;
		}
		return {processed: processed, changed: changed, remaining: pendingCount};
	}

	/**
		Advance bounded ticks until no work remains or `maximumTicks` is reached.

		This is intended for deterministic tests and offline world preparation. A
		rendering loop should call `tick` once per fixed simulation step so a large
		water release cannot monopolize a frame.
	**/
	public function settle(cells:WorldCells, updateBudget:Int, maximumTicks:Int):Bool {
		var ticks = 0;
		while (pendingCount > 0 && ticks < maximumTicks) {
			final result = tick(cells, updateBudget);
			if (result.processed <= 0 || result.processed > updateBudget)
				return false;
			ticks++;
		}
		return pendingCount == 0;
	}

	/** Stable hash of exact terrain, sources, levels, and falling states. */
	public inline function stateHash(cells:WorldCells):Int
		return World.stateHash(cells);

	/** Return whether exact world storage contains a source or flowing cell. */
	public function hasAnyWater(cells:WorldCells):Bool {
		var index = 0;
		while (index < World.VOLUME) {
			if (isWaterCode(WorldStorage.readCode(cells, index)))
				return true;
			index++;
		}
		return false;
	}

	/** Mark an edited cell and its six face-sharing neighbors for local repair. */
	function scheduleAround(coord:BlockCoord):Void {
		schedule(coord);
		schedule(World.coord(coord.x, coord.y - 1, coord.z));
		schedule(World.coord(coord.x - 1, coord.y, coord.z));
		schedule(World.coord(coord.x + 1, coord.y, coord.z));
		schedule(World.coord(coord.x, coord.y, coord.z - 1));
		schedule(World.coord(coord.x, coord.y, coord.z + 1));
		schedule(World.coord(coord.x, coord.y + 1, coord.z));
	}

	/** Mark the six outside faces where a newly filled box may begin flowing. */
	function scheduleOutsideVolume(origin:BlockCoord, width:Int, height:Int, depth:Int):Void {
		var z = origin.z;
		while (z < origin.z + depth) {
			var x = origin.x;
			while (x < origin.x + width) {
				schedule(World.coord(x, origin.y - 1, z));
				schedule(World.coord(x, origin.y + height, z));
				x++;
			}
			z++;
		}
		z = origin.z;
		while (z < origin.z + depth) {
			var y = origin.y;
			while (y < origin.y + height) {
				schedule(World.coord(origin.x - 1, y, z));
				schedule(World.coord(origin.x + width, y, z));
				y++;
			}
			z++;
		}
		var y = origin.y;
		while (y < origin.y + height) {
			var x = origin.x;
			while (x < origin.x + width) {
				schedule(World.coord(x, y, origin.z - 1));
				schedule(World.coord(x, y, origin.z + depth));
				x++;
			}
			y++;
		}
	}

	/** Remove and return the lowest marked world index in deterministic order. */
	function takeLowestPending():Int {
		if (pendingCount <= 0 || lowestPending >= World.VOLUME)
			return -1;
		final result = lowestPending;
		WaterPendingStorage.setOwnerMarked(pendingStorage, result, false);
		pendingCount--;
		lowestPending = World.VOLUME;
		if (pendingCount > 0) {
			var index = result + 1;
			while (index < World.VOLUME && lowestPending == World.VOLUME) {
				if (WaterPendingStorage.ownerIsMarked(pendingStorage, index))
					lowestPending = index;
				index++;
			}
			index = 0;
			while (index < result && lowestPending == World.VOLUME) {
				if (WaterPendingStorage.ownerIsMarked(pendingStorage, index))
					lowestPending = index;
				index++;
			}
		}
		return result;
	}

	/** Recompute one non-source cell and propagate only when its byte changes. */
	function recompute(cells:WorldCells, index:Int):Bool {
		final currentCode = WorldStorage.readCode(cells, index);
		final current = decodeWaterCell(currentCode);
		final coord = coordFromIndex(index);
		return switch current {
			case Source | Blocked | InvalidStorage(_): false;
			case Empty | Flowing(_, _):
				final desiredCode = desiredFlowCode(cells, coord);
				if (desiredCode == currentCode) false; else {
					WorldStorage.writeCode(cells, index, desiredCode);
					scheduleAround(coord);
					true;
				}
		};
	}

	/** Choose falling water first, otherwise the strongest horizontal feed. */
	function desiredFlowCode(cells:WorldCells, coord:BlockCoord):Int {
		final verticalLevel = feedLevel(cellState(cells, World.coord(coord.x, coord.y + 1, coord.z)), true);
		if (verticalLevel >= 0) {
			final supported = isBlocked(cellState(cells, World.coord(coord.x, coord.y - 1, coord.z)));
			return encodeFlow(verticalLevel, !supported);
		}

		var horizontalLevel = FLOW_LEVEL_COUNT;
		horizontalLevel = smaller(horizontalLevel, feedLevel(cellState(cells, World.coord(coord.x - 1, coord.y, coord.z)), false));
		horizontalLevel = smaller(horizontalLevel, feedLevel(cellState(cells, World.coord(coord.x + 1, coord.y, coord.z)), false));
		horizontalLevel = smaller(horizontalLevel, feedLevel(cellState(cells, World.coord(coord.x, coord.y, coord.z - 1)), false));
		horizontalLevel = smaller(horizontalLevel, feedLevel(cellState(cells, World.coord(coord.x, coord.y, coord.z + 1)), false));
		return horizontalLevel < FLOW_LEVEL_COUNT ? encodeFlow(horizontalLevel, false) : emptyCode();
	}

	/** Convert one neighboring state into the level it can provide here. */
	static function feedLevel(state:WaterCellState, vertical:Bool):Int {
		return switch state {
			case Source: vertical ? 0 : 1;
			case Flowing(level, falling):
				if (!vertical && falling) -1; else {
					final value:Int = level;
					vertical ? value : (value < 7 ? value + 1 : -1);
				}
			case Empty | Blocked | InvalidStorage(_): -1;
		};
	}

	/** Treat solids, world edges, and malformed storage as flow barriers. */
	static function isBlocked(state:WaterCellState):Bool {
		return switch state {
			case Blocked | InvalidStorage(_): true;
			case Empty | Source | Flowing(_, _): false;
		};
	}

	/** Keep the smaller valid feed level without admitting the -1 sentinel. */
	static inline function smaller(current:Int, candidate:Int):Int
		return candidate >= 0 && candidate < current ? candidate : current;

	/** Reverse the fixed 32 x 16 x 32 linear layout without division. */
	static inline function coordFromIndex(index:Int):BlockCoord
		return World.coord(index & 31, (index >>> 5) & 15, index >>> 9);
}
