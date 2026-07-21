package caxecraft.app;

#if c
import raylib.Color;
import raylib.Camera3D;
import raylib.Rectangle;
import raylib.Texture2D;
import raylib.Vector2;
import raylib.Vector3;
import raylib.raw.Raylib as RawRaylib;

/**
 * The one Caxecraft boundary that owns loaded GPU textures.
 *
 * RaylibHx currently keeps resource operations in its exact raw namespace
 * because haxe.c cannot yet prove automatic cleanup on every function exit.
 * Game and domain code stay on ordinary Haxe types; this adapter makes each
 * load, validity check, temporary draw borrow, and final unload visible in one
 * reviewable place. Call `unload` exactly once for every value for which
 * `isValid` returned true, and do so before `Raylib.CloseWindow`.
 */
final class CaxecraftTextures {
	public static inline function loadTitlePanorama():Texture2D
		return RawRaylib.LoadTexture("assets/showcase/title-panorama.png");

	public static inline function loadWordmark():Texture2D
		return RawRaylib.LoadTexture("assets/branding/caxecraft-wordmark.png");

	public static inline function loadHudAtlas():Texture2D
		return RawRaylib.LoadTexture("assets/atlases/hud.png");

	public static inline function loadItemAtlas():Texture2D
		return RawRaylib.LoadTexture("assets/atlases/items.png");

	public static inline function loadEntityAtlas():Texture2D
		return RawRaylib.LoadTexture("assets/atlases/entities.png");

	/** Load the reviewed 4x4 material atlas used by the base voxel renderer. */
	public static inline function loadTerrainAtlas():Texture2D
		return RawRaylib.LoadTexture("assets/atlases/terrain.png");

	public static inline function isValid(texture:Texture2D):Bool
		return RawRaylib.IsTextureValid(texture);

	/** Draw the image edge-to-edge without stretching its aspect ratio. */
	public static inline function drawCover(texture:Texture2D, width:Int, height:Int, tint:Color):Void {
		final imageAspect = texture.width / texture.height;
		final screenAspect = width / height;
		var sourceX = 0.0;
		var sourceY = 0.0;
		var sourceWidth:Float = texture.width;
		var sourceHeight:Float = texture.height;
		if (screenAspect > imageAspect) {
			sourceHeight = texture.width / screenAspect;
			sourceY = (texture.height - sourceHeight) * 0.5;
		} else {
			sourceWidth = texture.height * screenAspect;
			sourceX = (texture.width - sourceWidth) * 0.5;
		}
		RawRaylib.DrawTexturePro(texture, Rectangle.fromFloat(sourceX, sourceY, sourceWidth, sourceHeight),
			Rectangle.fromFloat(0.0, 0.0, width + 0.0, height + 0.0), Vector2.fromFloat(0.0, 0.0), c.Float32.fromFloat(0.0), tint);
	}

	/** Draw the complete image inside a box while preserving its aspect ratio. */
	public static inline function drawContained(texture:Texture2D, centerX:Int, top:Int, maximumWidth:Int, maximumHeight:Int, tint:Color):Void {
		var scale = maximumWidth / texture.width;
		final heightScale = maximumHeight / texture.height;
		if (heightScale < scale)
			scale = heightScale;
		final width = texture.width * scale;
		final height = texture.height * scale;
		RawRaylib.DrawTexturePro(texture, Rectangle.fromFloat(0.0, 0.0, texture.width + 0.0, texture.height + 0.0),
			Rectangle.fromFloat(centerX - width * 0.5, top + 0.0, width, height), Vector2.fromFloat(0.0, 0.0), c.Float32.fromFloat(0.0), tint);
	}

	/** Draw one cell from a regular atlas without creating a sprite object. */
	public static inline function drawAtlasCell(texture:Texture2D, column:Int, row:Int, columns:Int, rows:Int, x:Int, y:Int, width:Int, height:Int,
			tint:Color):Void {
		final sourceWidth = texture.width / columns;
		final sourceHeight = texture.height / rows;
		RawRaylib.DrawTexturePro(texture, Rectangle.fromFloat(column * sourceWidth, row * sourceHeight, sourceWidth, sourceHeight),
			Rectangle.fromFloat(x + 0.0, y + 0.0, width + 0.0, height + 0.0), Vector2.fromFloat(0.0, 0.0), c.Float32.fromFloat(0.0), tint);
	}

	/** Draw one atlas cell as a camera-facing world sprite. */
	public static inline function drawAtlasBillboard(camera:Camera3D, texture:Texture2D, column:Int, row:Int, columns:Int, rows:Int, position:Vector3,
			width:Float, height:Float, tint:Color):Void {
		final sourceWidth = texture.width / columns;
		final sourceHeight = texture.height / rows;
		RawRaylib.DrawBillboardRec(camera, texture, Rectangle.fromFloat(column * sourceWidth, row * sourceHeight, sourceWidth, sourceHeight), position,
			Vector2.fromFloat(width, height), tint);
	}

	public static inline function unload(texture:Texture2D):Void
		RawRaylib.UnloadTexture(texture);
}
#end
