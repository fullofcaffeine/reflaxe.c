package caxecraft.app;

#if c
import c.ConstSpan;
import caxecraft.app.ActivePlayableLevel.PlayableLevelView;
import caxecraft.content.RuntimeContentPack.RuntimeContentRegistry;
import raylib.Camera3D;
import raylib.Texture2D;
import raylib.Vector3;

/**
	Render validated map items without naming campaign content in the game loop.

	The selected runtime generation supplies positions and pack-local item codes.
	The content pack resolves each code to its reviewed atlas and cell. This
	module only picks the corresponding loaded texture and draws a camera-facing
	sprite; collection and equipment remain fixed-tick gameplay decisions.
**/
function drawWorldItems(registry:RuntimeContentRegistry, camera:Camera3D, active:ConstSpan<Int>, level:PlayableLevelView, itemTexture:Texture2D,
		itemTextureReady:Bool, adventureTexture:Texture2D, adventureTextureReady:Bool):Void {
	final inactive = 0;
	var index = 0;
	while (index < level.loadedItemCount()) {
		if (active[index] != inactive) {
			final loaded = level.loadedItemAt(index);
			final code = loaded.storageCode;
			final presentation = registry.itemPresentation(code);
			if (presentation != null) {
				final position = Vector3.fromFloat(loaded.xMilli / 1000.0, loaded.yMilli / 1000.0 + 0.42, loaded.zMilli / 1000.0);
				drawPresentation(camera, presentation.asset, presentation.cellIndex, position, itemTexture, itemTextureReady, adventureTexture,
					adventureTextureReady, 0.72, 0.72);
			}
		}
		index++;
	}
}

/** Draw the equipped item as a persistent HUD badge. */
function drawEquippedIcon(registry:RuntimeContentRegistry, code:Int, itemTexture:Texture2D, itemTextureReady:Bool, adventureTexture:Texture2D,
		adventureTextureReady:Bool, x:Int, y:Int, size:Int):Void {
	final presentation = registry.itemPresentation(code);
	if (presentation == null)
		return;
	if (presentation.asset == "items" && itemTextureReady)
		CaxecraftTextures.drawAtlasCell(itemTexture, presentation.cellIndex % 4, Std.int(presentation.cellIndex / 4), 4, 4, x, y, size, size,
			CaxecraftPalette.textureTint());
	else if (presentation.asset == "adventure-items" && adventureTextureReady)
		CaxecraftTextures.drawAtlasCell(adventureTexture, presentation.cellIndex % 4, Std.int(presentation.cellIndex / 4), 4, 4, x, y, size, size,
			CaxecraftPalette.textureTint());
}

/** Select one loaded atlas for an already validated content presentation. */
private function drawPresentation(camera:Camera3D, asset:String, cellIndex:Int, position:Vector3, itemTexture:Texture2D, itemTextureReady:Bool,
		adventureTexture:Texture2D, adventureTextureReady:Bool, width:Float, height:Float):Void {
	if (asset == "items" && itemTextureReady)
		CaxecraftTextures.drawAtlasBillboard(camera, itemTexture, cellIndex % 4, Std.int(cellIndex / 4), 4, 4, position, width, height,
			CaxecraftPalette.textureTint());
	else if (asset == "adventure-items" && adventureTextureReady)
		CaxecraftTextures.drawAtlasBillboard(camera, adventureTexture, cellIndex % 4, Std.int(cellIndex / 4), 4, 4, position, width, height,
			CaxecraftPalette.textureTint());
}
#end
