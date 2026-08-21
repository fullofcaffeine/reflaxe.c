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
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.scenario.ScenarioContentRegistry;
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
