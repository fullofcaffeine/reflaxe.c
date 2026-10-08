package caxecraft.app;

#if c
import caxecraft.editor.EditorObjectPresentation.EditorObjectVisual;
import caxecraft.editor.EditorAssetBrowser.EditorAssetThumbnail;
import caxecraft.editor.EditorWorldViewport.EditorObjectGizmo;
import caxecraft.app.TerrainAtlas.TerrainSheet;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.domain.World;
import raylib.Camera3D;
import raylib.Color;
import raylib.Raylib;
import raylib.Texture2D;
import raylib.Vector3;

/**
 * Draws authored objects in Build with the same validated art used by play.
 *
 * The application owns every borrowed texture. This module submits one visual
 * during the active 3D frame and never changes editor state, content, picking
 * bounds, or GPU-resource lifetime.
 */
/** All application-owned GPU resources borrowed for one editor frame. */
typedef EditorRenderResources = {
	/** Shared item and entity textures used by the HUD and editor. */
	final shared:HudResources;

	/** Loaded base-terrain atlas handle. */
	final terrainTexture:Texture2D;

	/** True when the base-terrain atlas handle is valid. */
	final terrainTextureReady:Bool;

	/** Loaded Adventure-terrain atlas handle. */
	final adventureTerrainTexture:Texture2D;

	/** True when the Adventure-terrain atlas handle is valid. */
	final adventureTerrainTextureReady:Bool;

	/** Reloadable atlases owned by the outer application. */
	final runtimeTextures:RuntimeTextureAtlasCatalog;
}

/**
 * Draw one cached browser picture through the same atlas owners used by play.
 *
 * The browser resolves content references only when it opens its immutable
 * catalog. This frame helper therefore performs texture routing but no registry
 * lookup. `false` asks the screen to preserve its category-mark fallback.
 */
function drawEditorAssetThumbnail(thumbnail:EditorAssetThumbnail, x:Int, y:Int, size:Int, resources:EditorRenderResources):Bool {
	return switch thumbnail {
		case TerrainAssetThumbnail(storageCode):
			drawTerrainThumbnail(storageCode, x, y, size, resources);
		case AtlasAssetThumbnail(asset, cellIndex):
			drawAtlasThumbnail(asset, cellIndex, x, y, size, resources);
		case MissingAssetThumbnail:
			false;
	};
}

/** Draw one resolved visual and return true when validated pack art was used. */
function drawEditorObject(camera:Camera3D, visual:EditorObjectVisual, gizmo:EditorObjectGizmo, resources:EditorRenderResources):Bool {
	final center = Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z);
	return switch visual {
		case PlayerSpawnVisual:
			drawPlayerSpawn(gizmo);
			false;
		case CheckpointVisual:
			drawCheckpoint(gizmo);
			false;
		case ItemVisual(asset, cellIndex):
			final drawn = drawSprite(camera, asset, cellIndex, center, 0.72, 0.72, resources.shared, resources.runtimeTextures);
			if (!drawn)
				drawFallback(gizmo);
			drawn;
		case ActorVisual(asset, cellIndex):
			final drawn = drawSprite(camera, asset, cellIndex, Vector3.fromFloat(gizmo.x, gizmo.y + 0.26, gizmo.z), 0.95, 1.52, resources.shared,
				resources.runtimeTextures);
			if (!drawn)
				drawFallback(gizmo);
			drawn;
		case StatefulObjectVisual(asset, cellIndex):
			final drawn = drawBox(asset, cellIndex, center, gizmo.width, gizmo.height, gizmo.depth, resources);
			if (!drawn)
				drawFallback(gizmo);
			drawn;
		case TriggerVolumeVisual: false;
		case FallbackObjectVisual:
			drawFallback(gizmo);
			false;
	};
}

/** Draw a compact solid marker when no admitted GPU presentation is available. */
private function drawFallback(gizmo:EditorObjectGizmo):Void
	Raylib.DrawCube(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(gizmo.width * 0.72), c.Float32.fromFloat(gizmo.height * 0.72),
		c.Float32.fromFloat(gizmo.depth * 0.72), CaxecraftPalette.hudPanel());

/** Draw one start pad and upright arrow without pretending that it is gameplay art. */
private function drawPlayerSpawn(gizmo:EditorObjectGizmo):Void {
	final color = Color.rgba(79, 224, 235);
	Raylib.DrawCube(Vector3.fromFloat(gizmo.x, gizmo.y - 0.42, gizmo.z), c.Float32.fromFloat(0.74), c.Float32.fromFloat(0.10), c.Float32.fromFloat(0.74),
		color);
	Raylib.DrawCube(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(0.12), c.Float32.fromFloat(0.72), c.Float32.fromFloat(0.12), color);
	Raylib.DrawSphere(Vector3.fromFloat(gizmo.x, gizmo.y + 0.40, gizmo.z), c.Float32.fromFloat(0.18), color);
}

/** Draw one compact flag so checkpoints remain readable without content art. */
private function drawCheckpoint(gizmo:EditorObjectGizmo):Void {
	final color = Color.rgba(255, 214, 92);
	Raylib.DrawCube(Vector3.fromFloat(gizmo.x, gizmo.y, gizmo.z), c.Float32.fromFloat(0.10), c.Float32.fromFloat(0.90), c.Float32.fromFloat(0.10), color);
	Raylib.DrawCube(Vector3.fromFloat(gizmo.x + 0.18, gizmo.y + 0.28, gizmo.z), c.Float32.fromFloat(0.38), c.Float32.fromFloat(0.26),
		c.Float32.fromFloat(0.08), color);
}

/** Route a sprite to one already-loaded fixed or reloadable atlas. */
private function drawSprite(camera:Camera3D, asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float, resources:HudResources,
		runtimeTextures:RuntimeTextureAtlasCatalog):Bool {
	if (asset == "entities" && resources.entityTextureReady) {
		CaxecraftAtlas.drawEntitySprite(camera, resources.entityTexture, cellIndex, position, width, height);
		return true;
	}
	if (asset == "items" && resources.itemTextureReady) {
		CaxecraftAtlas.drawWorldSprite(camera, resources.itemTexture, cellIndex, position, width, height);
		return true;
	}
	if (asset == "adventure-items" && resources.adventureItemTextureReady) {
		CaxecraftAtlas.drawWorldSprite(camera, resources.adventureItemTexture, cellIndex, position, width, height);
		return true;
	}
	return runtimeTextures.drawSprite(camera, asset, cellIndex, position, width, height);
}

/** Route a mechanism texture to one already-loaded fixed or reloadable atlas. */
private function drawBox(asset:String, cellIndex:Int, position:Vector3, width:Float, height:Float, depth:Float, resources:EditorRenderResources):Bool {
	if (asset == "entities" && resources.shared.entityTextureReady)
		return CaxecraftAtlas.drawAtlasBox(resources.shared.entityTexture, cellIndex, 4, 5, position, width, height, depth);
	if (asset == "items" && resources.shared.itemTextureReady)
		return CaxecraftAtlas.drawWorldBox(resources.shared.itemTexture, cellIndex, position, width, height, depth);
	if (asset == "terrain" && resources.terrainTextureReady)
		return CaxecraftAtlas.drawWorldBox(resources.terrainTexture, cellIndex, position, width, height, depth);
	if (asset == "adventure-terrain" && resources.adventureTerrainTextureReady)
		return CaxecraftAtlas.drawWorldBox(resources.adventureTerrainTexture, cellIndex, position, width, height, depth);
	return resources.runtimeTextures.drawBox(asset, cellIndex, position, width, height, depth);
}

/** Draw the existing top-face terrain picture for one validated storage code. */
private function drawTerrainThumbnail(storageCode:Int, x:Int, y:Int, size:Int, resources:EditorRenderResources):Bool {
	final kind = World.kindFromCode(storageCode);
	if (storageCode <= 0 || World.kindCode(kind) != storageCode)
		return false;
	final tile = TerrainAtlas.tile(kind, VoxelFace.Top);
	final cellIndex = TerrainAtlas.row(tile) * TerrainAtlas.COLUMNS + TerrainAtlas.column(tile);
	return switch TerrainAtlas.sheet(kind) {
		case Base:
			drawFixedAtlasCell(resources.terrainTexture, resources.terrainTextureReady, cellIndex, 4, 4, x, y, size);
		case Adventure:
			drawFixedAtlasCell(resources.adventureTerrainTexture, resources.adventureTerrainTextureReady, cellIndex, 4, 4, x, y, size);
	};
}

/** Route one cached atlas pair without duplicating content-to-picture facts. */
private function drawAtlasThumbnail(asset:String, cellIndex:Int, x:Int, y:Int, size:Int, resources:EditorRenderResources):Bool {
	if (asset == "entities" && resources.shared.entityTextureReady)
		return CaxecraftAtlas.drawEntityPortrait(resources.shared.entityTexture, cellIndex, x, y, size, size);
	if (asset == "items")
		return drawFixedAtlasCell(resources.shared.itemTexture, resources.shared.itemTextureReady, cellIndex, 4, 4, x, y, size);
	if (asset == "adventure-items")
		return drawFixedAtlasCell(resources.shared.adventureItemTexture, resources.shared.adventureItemTextureReady, cellIndex, 4, 4, x, y, size);
	if (asset == "terrain")
		return drawFixedAtlasCell(resources.terrainTexture, resources.terrainTextureReady, cellIndex, 4, 4, x, y, size);
	if (asset == "adventure-terrain")
		return drawFixedAtlasCell(resources.adventureTerrainTexture, resources.adventureTerrainTextureReady, cellIndex, 4, 4, x, y, size);
	return resources.runtimeTextures.drawCell(asset, cellIndex, x, y, size, size);
}

/** Draw one checked cell from a loaded regular atlas. */
private function drawFixedAtlasCell(texture:Texture2D, ready:Bool, cellIndex:Int, columns:Int, rows:Int, x:Int, y:Int, size:Int):Bool {
	if (!ready || cellIndex < 0 || cellIndex >= columns * rows)
		return false;
	CaxecraftTextures.drawAtlasCell(texture, cellIndex % columns, Std.int(cellIndex / columns), columns, rows, x, y, size, size,
		CaxecraftPalette.textureTint());
	return true;
}
#end
