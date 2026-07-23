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

	A class is used because the scheduler owns counters that persist across ticks
	and must agree with one pending-work buffer. A pure module function would make
	every caller carry and commit those invariants separately. The class remains
	`final` and stores no borrowed buffer; `GameSession` owns both objects and
	passes a short-lived view to each operation. Haxe.c embeds the simulation in
	the session C struct without a heap allocation.
**/
final class WaterSimulation {
	var pendingCount:Int;
	var lowestPending:Int;

	/**
		Initialize only the two scheduling numbers that persist between calls.

		The class deliberately stores no borrowed world or pending span: the C
		compiler cannot prove that such a borrow outlives a class instance. Each
		operation receives its caller-owned buffers for that call. Call
		`resetPending` once after creating or restoring pending storage.
	**/
	public function new() {
		pendingCount = 0;
		lowestPending = World.VOLUME;
	}

	/** Clear scheduled work without changing terrain or water bytes. */
	public function resetPending(pendingCells:WaterPendingCells):Void {
		var index = 0;
		while (index < World.VOLUME) {
			WaterPendingStorage.setMarked(pendingCells, index, false);
			index++;
		}
		pendingCount = 0;
		lowestPending = World.VOLUME;
	}

	/**
		Rebuild the two scheduler counters from restored per-cell queue marks.

		A save owns the marks because they determine which unfinished fluid work
		runs next. The counters are derived state: scanning in ascending world-index
		order reproduces the same next cell without persisting an implementation
		detail separately.
	**/
	public function restorePending(pendingCells:WaterPendingCells):Void {
		pendingCount = 0;
		lowestPending = World.VOLUME;
		var index = 0;
		while (index < World.VOLUME) {
			if (WaterPendingStorage.isMarked(pendingCells, index)) {
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
	public function placeSource(cells:WorldCells, pendingCells:WaterPendingCells, coord:BlockCoord):Bool {
		final index = World.indexOf(coord);
		if (index < 0)
			return false;
		return switch decodeWaterCell(WorldStorage.readCode(cells, index)) {
			case Blocked | InvalidStorage(_): false;
			case Empty | Source | Flowing(_, _):
				WorldStorage.writeCode(cells, index, sourceCode());
				scheduleAround(pendingCells, coord);
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
	public function placeInitialVolume(cells:WorldCells, pendingCells:WaterPendingCells, origin:BlockCoord, width:Int, height:Int, depth:Int):Bool {
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
		scheduleOutsideVolume(pendingCells, origin, width, height, depth);
		return true;
	}

	/** Remove water without removing a solid cell that occupies the coordinate. */
	public function removeWater(cells:WorldCells, pendingCells:WaterPendingCells, coord:BlockCoord):Bool {
		final index = World.indexOf(coord);
		if (index < 0)
			return false;
		return switch decodeWaterCell(WorldStorage.readCode(cells, index)) {
			case Source | Flowing(_, _):
				WorldStorage.writeCode(cells, index, emptyCode());
				scheduleAround(pendingCells, coord);
				true;
			case Empty | Blocked | InvalidStorage(_): false;
		};
	}

	/**
		Replace a voxel with validated solid terrain and invalidate nearby water.

		This is the dam-building hook. Water disappears from the occupied voxel and
		neighboring cells re-evaluate whether they still have a feed or a new route.
	**/
	public function replaceTerrain(cells:WorldCells, pendingCells:WaterPendingCells, coord:BlockCoord, kind:BlockKind):Bool {
		if (!World.isSolid(kind) || !World.replace(cells, coord, kind))
			return false;
		scheduleAround(pendingCells, coord);
		return true;
	}

	/** Remove solid terrain and let nearby water test the new opening. */
	public function removeTerrain(cells:WorldCells, pendingCells:WaterPendingCells, coord:BlockCoord):Bool {
		if (!World.contains(coord) || !World.isSolid(World.query(cells, coord)))
			return false;
		if (!World.replace(cells, coord, BlockKind.Air))
			return false;
		scheduleAround(pendingCells, coord);
		return true;
	}

	/** Place ordinary buildable terrain into air or water and schedule repair. */
	public function placeTerrain(cells:WorldCells, pendingCells:WaterPendingCells, coord:BlockCoord, kind:BlockKind):Bool {
		if (!World.isPlaceable(kind))
			return false;
		final index = World.indexOf(coord);
		if (index < 0)
			return false;
		final code = WorldStorage.readCode(cells, index);
		if (!isValidCode(code) || isSolidCode(code))
			return false;
		WorldStorage.writeCode(cells, index, World.kindCode(kind));
		scheduleAround(pendingCells, coord);
		return true;
	}

	/** Notify water after another transactional mechanic removed terrain. */
	public function terrainChanged(pendingCells:WaterPendingCells, coord:BlockCoord):Void
		scheduleAround(pendingCells, coord);

	/** Add one cell to the duplicate-free deterministic work set. */
	public function schedule(pendingCells:WaterPendingCells, coord:BlockCoord):Bool {
		final index = World.indexOf(coord);
		if (index < 0 || WaterPendingStorage.isMarked(pendingCells, index))
			return false;
		WaterPendingStorage.setMarked(pendingCells, index, true);
		pendingCount++;
		if (index < lowestPending)
			lowestPending = index;
		return true;
	}

	/** Process at most `maximumUpdates` cells and retain unfinished work. */
	public function tick(cells:WorldCells, pendingCells:WaterPendingCells, maximumUpdates:Int):WaterTickResult {
		var processed = 0;
		var changed = 0;
		while (processed < maximumUpdates && pendingCount > 0) {
			final index = takeLowestPending(pendingCells);
			if (index < 0)
				break;
			if (recompute(cells, pendingCells, index))
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
	public function settle(cells:WorldCells, pendingCells:WaterPendingCells, updateBudget:Int, maximumTicks:Int):Bool {
		var ticks = 0;
		while (pendingCount > 0 && ticks < maximumTicks) {
			final result = tick(cells, pendingCells, updateBudget);
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
	function scheduleAround(pendingCells:WaterPendingCells, coord:BlockCoord):Void {
		schedule(pendingCells, coord);
		schedule(pendingCells, World.coord(coord.x, coord.y - 1, coord.z));
		schedule(pendingCells, World.coord(coord.x - 1, coord.y, coord.z));
		schedule(pendingCells, World.coord(coord.x + 1, coord.y, coord.z));
		schedule(pendingCells, World.coord(coord.x, coord.y, coord.z - 1));
		schedule(pendingCells, World.coord(coord.x, coord.y, coord.z + 1));
		schedule(pendingCells, World.coord(coord.x, coord.y + 1, coord.z));
	}

	/** Mark the six outside faces where a newly filled box may begin flowing. */
	function scheduleOutsideVolume(pendingCells:WaterPendingCells, origin:BlockCoord, width:Int, height:Int, depth:Int):Void {
		var z = origin.z;
		while (z < origin.z + depth) {
			var x = origin.x;
			while (x < origin.x + width) {
				schedule(pendingCells, World.coord(x, origin.y - 1, z));
				schedule(pendingCells, World.coord(x, origin.y + height, z));
				x++;
			}
			z++;
		}
		z = origin.z;
		while (z < origin.z + depth) {
			var y = origin.y;
			while (y < origin.y + height) {
				schedule(pendingCells, World.coord(origin.x - 1, y, z));
				schedule(pendingCells, World.coord(origin.x + width, y, z));
				y++;
			}
			z++;
		}
		var y = origin.y;
		while (y < origin.y + height) {
			var x = origin.x;
			while (x < origin.x + width) {
				schedule(pendingCells, World.coord(x, y, origin.z - 1));
				schedule(pendingCells, World.coord(x, y, origin.z + depth));
				x++;
			}
			y++;
		}
	}

	/** Remove and return the lowest marked world index in deterministic order. */
	function takeLowestPending(pendingCells:WaterPendingCells):Int {
		if (pendingCount <= 0 || lowestPending >= World.VOLUME)
			return -1;
		final result = lowestPending;
		WaterPendingStorage.setMarked(pendingCells, result, false);
		pendingCount--;
		lowestPending = World.VOLUME;
		if (pendingCount > 0) {
			var index = result + 1;
			while (index < World.VOLUME && lowestPending == World.VOLUME) {
				if (WaterPendingStorage.isMarked(pendingCells, index))
					lowestPending = index;
				index++;
			}
			index = 0;
			while (index < result && lowestPending == World.VOLUME) {
				if (WaterPendingStorage.isMarked(pendingCells, index))
					lowestPending = index;
				index++;
			}
		}
		return result;
	}

	/** Recompute one non-source cell and propagate only when its byte changes. */
	function recompute(cells:WorldCells, pendingCells:WaterPendingCells, index:Int):Bool {
		final currentCode = WorldStorage.readCode(cells, index);
		final current = decodeWaterCell(currentCode);
		final coord = coordFromIndex(index);
		return switch current {
			case Source | Blocked | InvalidStorage(_): false;
			case Empty | Flowing(_, _):
				final desiredCode = desiredFlowCode(cells, coord);
				if (desiredCode == currentCode) false; else {
					WorldStorage.writeCode(cells, index, desiredCode);
					scheduleAround(pendingCells, coord);
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
