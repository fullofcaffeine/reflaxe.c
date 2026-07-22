package caxecraft.domain;

import caxecraft.domain.WaterCellCodec.emptyCode;
import caxecraft.domain.WaterCellCodec.isSolidCode;
import caxecraft.domain.WaterCellCodec.isValidCode;
import caxecraft.domain.WaterCellCodec.isWaterCode;

/**
	Capture and restore the mutable water part of a future player save.

	The canonical snapshot is deliberately small in meaning even though callers
	provide fixed world-sized buffers: each water slot is either zero or one exact
	source/flow byte, and each pending slot is one queue mark. Terrain comes from
	the authored map plus the wider save overlay. Apply that terrain before
	restoring water so an incompatible solid overlap can fail without mutation.

	This module owns no filesystem or text/binary file format. The full CAXESAVE
	codec will embed these ascending-index facts beside other session state under
	`haxe_c-4my`; keeping the mechanic seam here prevents that codec from
	reimplementing fluid rules.
**/
/** Copy exact water and pending marks into caller-owned canonical buffers. */
function capture(cells:WorldCells, pendingCells:WaterPendingCells, snapshotCells:WorldCells, snapshotPending:WaterPendingCells):Bool {
	var index = 0;
	while (index < World.VOLUME) {
		if (!isValidCode(WorldStorage.readCode(cells, index)))
			return false;
		index++;
	}

	index = 0;
	while (index < World.VOLUME) {
		final code = WorldStorage.readCode(cells, index);
		WorldStorage.writeCode(snapshotCells, index, isWaterCode(code) ? code : emptyCode());
		WaterPendingStorage.setMarked(snapshotPending, index, WaterPendingStorage.isMarked(pendingCells, index));
		index++;
	}
	return true;
}

/**
	Replace only mutable water and queue state after validating the whole snapshot.

	Authored water currently in `cells` is cleared first, which means removing a
	permanent source during play survives reload. Solid terrain is preserved. A
	saved water byte that now overlaps a solid or malformed cell rejects the
	complete restore before any live byte changes. After success, the owner calls
	`WaterSimulation.restorePending` on its own scheduler instance; the byte codec
	does not borrow or mutate that stateful object.
**/
function restore(snapshotCells:WorldCells, snapshotPending:WaterPendingCells, cells:WorldCells, pendingCells:WaterPendingCells):Bool {
	var index = 0;
	while (index < World.VOLUME) {
		final savedCode = WorldStorage.readCode(snapshotCells, index);
		if (savedCode != emptyCode() && !isWaterCode(savedCode))
			return false;
		final currentCode = WorldStorage.readCode(cells, index);
		if (!isValidCode(currentCode) || (savedCode != emptyCode() && isSolidCode(currentCode)))
			return false;
		index++;
	}

	index = 0;
	while (index < World.VOLUME) {
		final savedCode = WorldStorage.readCode(snapshotCells, index);
		final currentCode = WorldStorage.readCode(cells, index);
		if (isWaterCode(currentCode))
			WorldStorage.writeCode(cells, index, emptyCode());
		if (savedCode != emptyCode())
			WorldStorage.writeCode(cells, index, savedCode);
		WaterPendingStorage.setMarked(pendingCells, index, WaterPendingStorage.isMarked(snapshotPending, index));
		index++;
	}
	return true;
}

/** Stable ascending-index digest for snapshot and live-state parity tests. */
function stateHash(cells:WorldCells, pendingCells:WaterPendingCells):Int {
	var hash = -2128831035;
	var index = 0;
	while (index < World.VOLUME) {
		final code = WorldStorage.readCode(cells, index);
		hash = (hash ^ (isWaterCode(code) ? code : emptyCode())) * 16777619;
		hash = (hash ^ (WaterPendingStorage.isMarked(pendingCells, index) ? 1 : 0)) * 16777619;
		index++;
	}
	return hash;
}
