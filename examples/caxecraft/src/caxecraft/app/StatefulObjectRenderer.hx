package caxecraft.app;

#if c
import caxecraft.app.ActivePlayableLevel.PlayableLevelView;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.domain.GameSession;
import raylib.Camera3D;
import raylib.Raylib;
import raylib.Texture2D;
import raylib.Vector3;

/**
	Draws generic authored objects from their current CaxeFlow state.

	The loaded level supplies identity and position. `GameSession` supplies the
	current active flag and state. The runtime content pack maps that state to an
	atlas cell. Keeping those owners separate lets a content edit change the
	object, rule, or picture without adding campaign names to the game engine.
**/
/** Draw all active stateful objects using their current content-owned visual. */
function drawStatefulObjects(registry:RuntimeContentRegistry, session:GameSession, level:PlayableLevelView, camera:Camera3D, entityTexture:Texture2D,
		entityTextureReady:Bool, itemTexture:Texture2D, itemTextureReady:Bool, adventureItemTexture:Texture2D, adventureItemTextureReady:Bool,
		terrainTexture:Texture2D, terrainTextureReady:Bool):Void {
	for (index in 0...level.statefulObjectCount()) {
		final authoredId = level.statefulObjectIdAt(index);
		if (!session.statefulObjectIsActive(authoredId))
			continue;
		final state = session.statefulObjectState(authoredId);
		if (state == null)
			continue;
		final resolved = registry.statefulObjectPresentation(level.statefulObjectContentIdAt(index), state);
		if (resolved == null || !registry.statefulObjectVisible(level.statefulObjectContentIdAt(index), state))
			continue;
		final transform = level.statefulObjectTransformAt(index);
		final bounds = level.statefulObjectBoundsAt(index);
		final width = bounds.widthMilli / 1000.0;
		final height = bounds.heightMilli / 1000.0;
		final depth = bounds.depthMilli / 1000.0;
		final position = Vector3.fromFloat(transform.xMilli / 1000.0, transform.yMilli / 1000.0 + height * 0.5, transform.zMilli / 1000.0);
		final boxVisual = width > 1.2 || height > 1.2 || depth > 1.2;
		final drawn = boxVisual ? drawBoxPresentation(resolved.asset, resolved.cellIndex, position, width, height, depth, entityTexture, entityTextureReady,
			itemTexture, itemTextureReady, adventureItemTexture, adventureItemTextureReady, terrainTexture,
			terrainTextureReady) : drawPresentation(camera, resolved.asset, resolved.cellIndex, position, entityTexture, entityTextureReady, itemTexture,
				itemTextureReady, adventureItemTexture, adventureItemTextureReady, terrainTexture, terrainTextureReady);
		if (!drawn)
			Raylib.DrawCube(position, c.Float32.fromFloat(width), c.Float32.fromFloat(height), c.Float32.fromFloat(depth), CaxecraftPalette.selection());
	}
}

/** Select one loaded atlas and cover a structure with its authored cell. */
private function drawBoxPresentation(asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float, depth:Float, entityTexture:Texture2D,
		entityTextureReady:Bool, itemTexture:Texture2D, itemTextureReady:Bool, adventureItemTexture:Texture2D, adventureItemTextureReady:Bool,
		terrainTexture:Texture2D, terrainTextureReady:Bool):Bool {
	if (asset == "entities" && entityTextureReady)
		return CaxecraftAtlas.drawWorldBox(entityTexture, cellIndex, position, width, height, depth);
	if (asset == "items" && itemTextureReady)
		return CaxecraftAtlas.drawWorldBox(itemTexture, cellIndex, position, width, height, depth);
	if (asset == "adventure-items" && adventureItemTextureReady)
		return CaxecraftAtlas.drawWorldBox(adventureItemTexture, cellIndex, position, width, height, depth);
	if (asset == "terrain" && terrainTextureReady)
		return CaxecraftAtlas.drawWorldBox(terrainTexture, cellIndex, position, width, height, depth);
	return false;
}

/** Select one loaded world atlas and report whether a picture was drawn. */
private function drawPresentation(camera:Camera3D, asset:String, cellIndex:Int, position:Vector3, entityTexture:Texture2D, entityTextureReady:Bool,
		itemTexture:Texture2D, itemTextureReady:Bool, adventureItemTexture:Texture2D, adventureItemTextureReady:Bool, terrainTexture:Texture2D,
		terrainTextureReady:Bool):Bool {
	if (asset == "entities" && entityTextureReady) {
		CaxecraftAtlas.drawWorldSprite(camera, entityTexture, cellIndex, position, 0.82, 1.16);
		return true;
	}
	if (asset == "items" && itemTextureReady) {
		CaxecraftAtlas.drawWorldSprite(camera, itemTexture, cellIndex, position, 0.82, 1.16);
		return true;
	}
	if (asset == "adventure-items" && adventureItemTextureReady) {
		CaxecraftAtlas.drawWorldSprite(camera, adventureItemTexture, cellIndex, position, 0.82, 1.16);
		return true;
	}
	if (asset == "terrain" && terrainTextureReady) {
		CaxecraftAtlas.drawWorldSprite(camera, terrainTexture, cellIndex, position, 0.82, 1.16);
		return true;
	}
	return false;
}
#end
