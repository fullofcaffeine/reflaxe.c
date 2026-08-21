package caxecraft.editor;

import caxecraft.domain.World;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioWorld;

/**
 * Converts an accepted editor view into the fixed terrain layout used by play.
 *
 * The CAXEMAP draft remains authoritative and stores palette-local codes in its
 * authored dimensions. This module resolves those codes through the active
 * content registry and creates a read-only presentation copy only when the
 * gameplay runtime already admits that world shape. Build can then use the
 * ordinary terrain renderer without constructing a second game session.
 */
/** Whether the gameplay renderer can faithfully present the current draft. */
enum EditorRuntimeTerrainResult {
	/** Complete fixed-layout storage, with an unused compact-world half as air. */
	RuntimeTerrainReady(cells:Array<Int>);

	/** The draft is incomplete, malformed, or uses a shape play cannot load. */
	RuntimeTerrainUnavailable;
}

/**
 * Resolve one editor projection into the gameplay world's exact byte ordering.
 *
 * This function fails closed if dimensions, cell counts, or palette mappings
 * disagree. The caller can retain its renderer-independent overview for that
 * case, which lets creators continue repairing a temporarily unplayable draft.
 */
function projectRuntimeTerrain(world:ScenarioWorld, projection:EditorWorldProjection, registry:ScenarioContentRegistry):EditorRuntimeTerrainResult {
	final width = world.size.width;
	final height = world.size.height;
	final depth = world.size.depth;
	if (!World.admitsAuthoredSize(width, height, depth)
		|| projection.width != width
		|| projection.height != height
		|| projection.depth != depth
		|| projection.cells.length != width * height * depth)
		return RuntimeTerrainUnavailable;

	final storageCodeByPalette:Array<Int> = [];
	for (_ in 0...256)
		storageCodeByPalette.push(-1);
	var hasAir = false;
	for (entry in world.palette) {
		if (entry.code < 0 || entry.code >= storageCodeByPalette.length || storageCodeByPalette[entry.code] >= 0)
			return RuntimeTerrainUnavailable;
		final storageCode = runtimeCodeForBlock(entry.blockType, registry);
		if (storageCode < 0)
			return RuntimeTerrainUnavailable;
		storageCodeByPalette[entry.code] = storageCode;
		if (entry.code == 0 && storageCode == 0 && registry.isAirBlock(entry.blockType))
			hasAir = true;
	}
	if (!hasAir)
		return RuntimeTerrainUnavailable;

	final runtimeCells = [for (_ in 0...World.VOLUME) 0];
	for (z in 0...depth)
		for (y in 0...height)
			for (x in 0...width) {
				final sourceIndex = (z * height + y) * width + x;
				final paletteCode = projection.cells[sourceIndex];
				if (paletteCode < 0 || paletteCode >= storageCodeByPalette.length)
					return RuntimeTerrainUnavailable;
				final storageCode = storageCodeByPalette[paletteCode];
				if (storageCode < 0)
					return RuntimeTerrainUnavailable;
				final destinationIndex = x + World.WIDTH * (y + World.HEIGHT * z);
				runtimeCells[destinationIndex] = storageCode;
			}
	return RuntimeTerrainReady(runtimeCells);
}

/**
	Resolve one palette-local code for an incremental presentation update.

	A missing or duplicate palette code returns `-1`, as does a content mapping
	that is not one of the fixed runtime block codes. The native editor then
	falls back to a complete projection instead of publishing a plausible but
	incorrect cell.
**/
function runtimeCodeForPalette(world:ScenarioWorld, paletteCode:Int, registry:ScenarioContentRegistry):Int {
	var found = false;
	var resolved = -1;
	for (entry in world.palette)
		if (entry.code == paletteCode) {
			if (found)
				return -1;
			found = true;
			resolved = runtimeCodeForBlock(entry.blockType, registry);
			if (resolved < 0)
				return -1;
		}
	return found ? resolved : -1;
}

/** Narrow one validated content block identity to the fixed runtime code set. */
private function runtimeCodeForBlock(blockType:ContentId, registry:ScenarioContentRegistry):Int {
	final storageCode = registry.blockStorageCode(blockType);
	return storageCode >= 0 && World.kindCode(World.kindFromCode(storageCode)) == storageCode ? storageCode : -1;
}
