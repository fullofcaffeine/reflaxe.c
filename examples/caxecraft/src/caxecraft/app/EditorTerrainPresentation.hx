package caxecraft.app;

#if c
import c.CArray;
import c.UInt8;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldStorage;
import caxecraft.domain.WorldView;
import caxecraft.domain.WorldVolume;
import caxecraft.editor.EditorRuntimeTerrain.EditorRuntimeTerrainResult;
import caxecraft.editor.EditorRuntimeTerrain.projectRuntimeTerrain;
import caxecraft.editor.EditorRuntimeTerrain.runtimeCodeForPalette;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.scenario.ScenarioContentRegistry;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioWorld;
import raylib.Texture2D;

/**
 * Owns the editor's disposable terrain-rendering cache, not editable world data.
 *
 * An accepted edit rebuilds fixed-layout bytes from the canonical draft and
 * invalidates the same chunked renderer used by ordinary play. Frames borrow
 * that derived storage read-only. Unsupported draft shapes remain editable and
 * use the screen's compact overview instead of constructing a game session.
 */
final class EditorTerrainPresentation {
	/** Fixed native storage whose lifetime covers every renderer borrow. */
	final storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);

	/** Independent derived-face cache for the editor's independently changing view. */
	final renderer:TerrainRenderer = new TerrainRenderer();

	/** True only when `storage` is a complete projection of the current draft. */
	var ready:Bool = false;

	/** Construct an unavailable view until the editor publishes its first draft. */
	public function new() {}

	/**
	 * Replace the presentation copy after an accepted editor transition.
	 *
	 * Failure is an ordinary fallback state: incomplete or custom-size drafts
	 * continue through the renderer-independent overview and remain repairable.
	 */
	public function refresh(world:ScenarioWorld, projection:EditorWorldProjection, registry:ScenarioContentRegistry):Void {
		switch projectRuntimeTerrain(world, projection, registry) {
			case RuntimeTerrainReady(cells):
				var writable:WorldCells = storage.span();
				for (index in 0...World.VOLUME)
					WorldStorage.writeCode(writable, index, cells[index]);
				renderer.invalidateAll();
				ready = true;
			case RuntimeTerrainUnavailable:
				clear();
		}
	}

	/**
	 * Patch one accepted voxel edit without replacing the complete terrain copy.
	 *
	 * The return value is the number of newly dirty renderer chunks. `-1` means
	 * the retained presentation was unavailable or incompatible, so the caller
	 * must use `refresh` as a fail-closed fallback.
	 */
	public function refreshVoxel(world:ScenarioWorld, projection:EditorWorldProjection, registry:ScenarioContentRegistry, point:VoxelPoint):Int {
		final width = world.size.width;
		final height = world.size.height;
		final depth = world.size.depth;
		if (!ready
			|| !World.admitsAuthoredSize(width, height, depth)
			|| projection.width != width
			|| projection.height != height
			|| projection.depth != depth
			|| projection.cells.length != width * height * depth
			|| point.x < 0
			|| point.x >= width
			|| point.y < 0
			|| point.y >= height
			|| point.z < 0
			|| point.z >= depth)
			return -1;
		final sourceIndex = (point.z * height + point.y) * width + point.x;
		final storageCode = runtimeCodeForPalette(world, projection.cells[sourceIndex], registry);
		if (storageCode < 0)
			return -1;
		final coordinate = World.coord(point.x, point.y, point.z);
		final destinationIndex = World.indexOf(coordinate);
		if (destinationIndex < 0)
			return -1;
		var writable:WorldCells = storage.span();
		WorldStorage.writeCode(writable, destinationIndex, storageCode);
		return renderer.invalidate(coordinate);
	}

	/** Forget an unavailable or closed draft before another frame can draw it. */
	public function clear():Void {
		ready = false;
		renderer.invalidateAll();
	}

	/**
	 * Draw with the ordinary terrain atlas and chunk cache when projection exists.
	 *
	 * The Boolean result tells the editor whether it must draw its compact fallback.
	 */
	public function draw(baseTexture:Texture2D, baseReady:Bool, adventureTexture:Texture2D, adventureReady:Bool, viewerX:Float, viewerZ:Float):Bool {
		if (!ready)
			return false;
		var view:WorldView = storage.constSpan();
		renderer.draw(view, baseTexture, baseReady, adventureTexture, adventureReady, viewerX, viewerZ);
		return true;
	}
}
#end
