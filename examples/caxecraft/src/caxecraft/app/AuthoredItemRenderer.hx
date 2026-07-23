package caxecraft.app;

#if c
import c.ConstSpan;
import caxecraft.content.BaseContentPack;
import caxecraft.content.BaseContentPack.ContentAsset;
import caxecraft.content.FirstPlayableLevel.itemCount;
import caxecraft.content.FirstPlayableLevel.itemStorageCode;
import caxecraft.content.FirstPlayableLevel.itemXMilli;
import caxecraft.content.FirstPlayableLevel.itemYMilli;
import caxecraft.content.FirstPlayableLevel.itemZMilli;
import raylib.Camera3D;
import raylib.Texture2D;
import raylib.Vector3;

/**
	Render validated map items without naming campaign content in the game loop.

	The generated level supplies positions and pack-local item codes. The content
	pack resolves each code to its reviewed atlas and cell. This module only picks
	the corresponding loaded texture and draws a camera-facing sprite; collection
	and equipment remain fixed-tick gameplay decisions.
**/
function drawWorldItems(camera:Camera3D, active:ConstSpan<Int>, itemTexture:Texture2D, itemTextureReady:Bool, adventureTexture:Texture2D,
		adventureTextureReady:Bool):Void {
	final inactive = 0;
	var index = 0;
	while (index < itemCount()) {
		if (active[index] != inactive) {
			final code = itemStorageCode(index);
			if (BaseContentPack.isValidItemStorageCode(code)) {
				final item = BaseContentPack.itemFromValidatedStorageCode(code);
				final presentation = BaseContentPack.itemIcon(item);
				final position = Vector3.fromFloat(itemXMilli(index) / 1000.0, itemYMilli(index) / 1000.0 + 0.42, itemZMilli(index) / 1000.0);
				drawPresentation(camera, presentation.asset, presentation.cellIndex, position, itemTexture, itemTextureReady, adventureTexture,
					adventureTextureReady, 0.72, 0.72);
			}
		}
		index++;
	}
}

/** Draw the equipped item as a persistent HUD badge. */
function drawEquippedIcon(code:Int, itemTexture:Texture2D, itemTextureReady:Bool, adventureTexture:Texture2D, adventureTextureReady:Bool, x:Int, y:Int,
		size:Int):Void {
	if (!BaseContentPack.isValidItemStorageCode(code))
		return;
	final presentation = BaseContentPack.itemIcon(BaseContentPack.itemFromValidatedStorageCode(code));
	if (presentation.asset == ContentAsset.Items && itemTextureReady)
		CaxecraftTextures.drawAtlasCell(itemTexture, presentation.cellIndex % 4, Std.int(presentation.cellIndex / 4), 4, 4, x, y, size, size,
			CaxecraftPalette.textureTint());
	else if (presentation.asset == ContentAsset.AdventureItems && adventureTextureReady)
		CaxecraftTextures.drawAtlasCell(adventureTexture, presentation.cellIndex % 4, Std.int(presentation.cellIndex / 4), 4, 4, x, y, size, size,
			CaxecraftPalette.textureTint());
}

/** Select one loaded atlas for an already validated content presentation. */
private function drawPresentation(camera:Camera3D, asset:ContentAsset, cellIndex:Int, position:Vector3, itemTexture:Texture2D, itemTextureReady:Bool,
		adventureTexture:Texture2D, adventureTextureReady:Bool, width:Float, height:Float):Void {
	if (asset == ContentAsset.Items && itemTextureReady)
		CaxecraftTextures.drawAtlasBillboard(camera, itemTexture, cellIndex % 4, Std.int(cellIndex / 4), 4, 4, position, width, height,
			CaxecraftPalette.textureTint());
	else if (asset == ContentAsset.AdventureItems && adventureTextureReady)
		CaxecraftTextures.drawAtlasBillboard(camera, adventureTexture, cellIndex % 4, Std.int(cellIndex / 4), 4, 4, position, width, height,
			CaxecraftPalette.textureTint());
}
#end
