package caxecraft.app;

#if c
import caxecraft.gameplay.ItemKind;
import raylib.Texture2D;

/** The two reviewed HUD frame cells currently used by the playable. */
enum HotbarFrame {
	Normal;
	Selected;
}

/**
 * Typed coordinates for the checked-in 4x4 HUD and item atlases.
 *
 * Gameplay names an `ItemKind`; only this presentation table knows where its
 * picture lives. That prevents atlas row/column numbers from leaking into the
 * inventory rules or game loop.
 */
final class CaxecraftAtlas {
	public static function drawHotbarFrame(texture:Texture2D, frame:HotbarFrame, x:Int, y:Int, size:Int):Void {
		// haxe_c's exhaustive value-switch carrier is tracked by haxe_c-c9i.
		// An initialized local keeps the same closed mapping explicit today.
		var column = 0;
		switch (frame) {
			case Normal:
			case Selected:
				column = 1;
		}
		CaxecraftTextures.drawAtlasCell(texture, column, 2, 4, 4, x, y, size, size, CaxecraftPalette.textureTint());
	}

	public static function drawItem(texture:Texture2D, item:ItemKind, x:Int, y:Int, size:Int):Void {
		var column = 0;
		var row = 0;
		switch (item) {
			case GrassBlock:
			case DirtBlock:
				column = 1;
			case StoneBlock:
				column = 2;
			case Haxeforge:
				row = 1;
			case CopperSword:
				column = 3;
				row = 1;
			case Berries:
				column = 2;
				row = 2;
			case Bread:
				column = 3;
				row = 2;
			case Lantern:
				column = 1;
				row = 3;
		}
		CaxecraftTextures.drawAtlasCell(texture, column, row, 4, 4, x, y, size, size, CaxecraftPalette.textureTint());
	}
}
#end
