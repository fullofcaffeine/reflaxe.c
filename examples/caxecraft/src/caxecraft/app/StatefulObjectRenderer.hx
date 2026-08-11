package caxecraft.app;

#if c
import caxecraft.app.ActivePlayableLevel.PlayableLevelView;
import caxecraft.app.StatefulObjectVisual.StatefulObjectVisualKind;
import caxecraft.app.StatefulObjectVisual.statefulObjectVisual;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import caxecraft.domain.GameSession;
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
function drawStatefulObjects(registry:RuntimeContentRegistry, session:GameSession, level:PlayableLevelView, entityTexture:Texture2D, entityTextureReady:Bool,
		itemTexture:Texture2D, itemTextureReady:Bool, adventureItemTexture:Texture2D, adventureItemTextureReady:Bool, terrainTexture:Texture2D,
		terrainTextureReady:Bool, runtimeTextures:RuntimeTextureAtlasCatalog):Void {
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
		final visual = statefulObjectVisual(bounds.widthMilli, bounds.heightMilli, bounds.depthMilli, transform.yawDegrees);
		final width = visual.widthMilli / 1000.0;
		final height = visual.heightMilli / 1000.0;
		final depth = visual.depthMilli / 1000.0;
		switch visual.kind {
			case MechanismVisual:
				drawMechanismPresentation(resolved.asset, resolved.cellIndex, transform.xMilli / 1000.0, transform.yMilli / 1000.0, transform.zMilli / 1000.0,
					width, height, depth, entityTexture, entityTextureReady, itemTexture, itemTextureReady, adventureItemTexture, adventureItemTextureReady,
					terrainTexture, terrainTextureReady, runtimeTextures);
			case StructureVisual:
				final position = Vector3.fromFloat(transform.xMilli / 1000.0, transform.yMilli / 1000.0 + height * 0.5, transform.zMilli / 1000.0);
				final drawn = drawBoxPresentation(resolved.asset, resolved.cellIndex, position, width, height, depth, entityTexture, entityTextureReady,
					itemTexture, itemTextureReady, adventureItemTexture, adventureItemTextureReady, terrainTexture, terrainTextureReady, runtimeTextures);
				if (!drawn)
					Raylib.DrawCube(position, c.Float32.fromFloat(width), c.Float32.fromFloat(height), c.Float32.fromFloat(depth),
						CaxecraftPalette.selection());
		}
	}
}

/**
	Draw one small interactive object as a grounded 3D control pedestal.

	The base, cap, and status crystal establish depth from every camera angle.
	The content-owned atlas cell covers the central body, so CaxeFlow state still
	selects the visible identity without a campaign-specific renderer branch.
**/
private function drawMechanismPresentation(asset:String, cellIndex:Int, x:Float, groundY:Float, z:Float, width:Float, height:Float, depth:Float,
		entityTexture:Texture2D, entityTextureReady:Bool, itemTexture:Texture2D, itemTextureReady:Bool, adventureItemTexture:Texture2D,
		adventureItemTextureReady:Bool, terrainTexture:Texture2D, terrainTextureReady:Bool, runtimeTextures:RuntimeTextureAtlasCatalog):Void {
	final baseHeight = height * 0.16;
	final bodyHeight = height * 0.54;
	final capHeight = height * 0.12;
	final baseWidth = width * 0.84;
	final baseDepth = depth * 0.84;
	final bodyWidth = width * 0.60;
	final bodyDepth = depth * 0.60;
	Raylib.DrawCube(Vector3.fromFloat(x, groundY + baseHeight * 0.5, z), c.Float32.fromFloat(baseWidth), c.Float32.fromFloat(baseHeight),
		c.Float32.fromFloat(baseDepth), CaxecraftPalette.mechanismFrame());
	final bodyPosition = Vector3.fromFloat(x, groundY + baseHeight + bodyHeight * 0.5, z);
	final bodyDrawn = drawBoxPresentation(asset, cellIndex, bodyPosition, bodyWidth, bodyHeight, bodyDepth, entityTexture, entityTextureReady, itemTexture,
		itemTextureReady, adventureItemTexture, adventureItemTextureReady, terrainTexture, terrainTextureReady, runtimeTextures);
	if (!bodyDrawn)
		Raylib.DrawCube(bodyPosition, c.Float32.fromFloat(bodyWidth), c.Float32.fromFloat(bodyHeight), c.Float32.fromFloat(bodyDepth),
			CaxecraftPalette.selection());
	final capY = groundY + baseHeight + bodyHeight + capHeight * 0.5;
	Raylib.DrawCube(Vector3.fromFloat(x, capY, z), c.Float32.fromFloat(width * 0.72), c.Float32.fromFloat(capHeight), c.Float32.fromFloat(depth * 0.72),
		CaxecraftPalette.mechanismFrame());
	final coreHeight = height * 0.18;
	Raylib.DrawCube(Vector3.fromFloat(x, groundY + baseHeight + bodyHeight + capHeight + coreHeight * 0.5, z), c.Float32.fromFloat(width * 0.18),
		c.Float32.fromFloat(coreHeight), c.Float32.fromFloat(depth * 0.18), CaxecraftPalette.mechanismCore());
}

/** Select one loaded atlas and cover a structure with its authored cell. */
private function drawBoxPresentation(asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float, depth:Float, entityTexture:Texture2D,
		entityTextureReady:Bool, itemTexture:Texture2D, itemTextureReady:Bool, adventureItemTexture:Texture2D, adventureItemTextureReady:Bool,
		terrainTexture:Texture2D, terrainTextureReady:Bool, runtimeTextures:RuntimeTextureAtlasCatalog):Bool {
	if (asset == "entities" && entityTextureReady)
		return CaxecraftAtlas.drawWorldBox(entityTexture, cellIndex, position, width, height, depth);
	if (asset == "items" && itemTextureReady)
		return CaxecraftAtlas.drawWorldBox(itemTexture, cellIndex, position, width, height, depth);
	if (asset == "adventure-items" && adventureItemTextureReady)
		return CaxecraftAtlas.drawWorldBox(adventureItemTexture, cellIndex, position, width, height, depth);
	if (asset == "terrain" && terrainTextureReady)
		return CaxecraftAtlas.drawWorldBox(terrainTexture, cellIndex, position, width, height, depth);
	return runtimeTextures.drawBox(asset, cellIndex, position, width, height, depth);
}
#end
