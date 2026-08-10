package caxecraft.app;

#if c
import caxecraft.gameplay.ItemKind;
import caxecraft.app.AtlasLayout.entityAtlasCellIsValid;
import caxecraft.app.AtlasLayout.entityAtlasColumn;
import caxecraft.app.AtlasLayout.entityAtlasColumns;
import caxecraft.app.AtlasLayout.entityAtlasRow;
import caxecraft.app.AtlasLayout.entityAtlasRows;
import raylib.Camera3D;
import raylib.Color;
import raylib.Rlgl;
import raylib.Texture2D;
import raylib.Vector3;

/** The two reviewed HUD frame cells currently used by the playable. */
enum HotbarFrame {
	Normal;
	Selected;
}

/** Closed semantic names for the health cells used by the first HUD. */
enum HudGlyph {
	HealthFull;
	HealthHalf;
	HealthEmpty;
}

/**
 * Typed coordinates for the checked-in 4x4 HUD and item atlases.
 *
 * Gameplay names an `ItemKind`; only this presentation table knows where its
 * picture lives. That prevents atlas row/column numbers from leaking into the
 * inventory rules or game loop.
 */
final class CaxecraftAtlas {
	/** Draw one cell from a checked-in 4x4 item or material atlas. */
	public static function drawWorldSprite(camera:Camera3D, texture:Texture2D, cellIndex:Int, position:Vector3, width:Float, height:Float):Void {
		drawAtlasSprite(camera, texture, cellIndex, 4, 4, position, width, height);
	}

	/** Draw one cell from any validated regular atlas as a world billboard. */
	public static function drawAtlasSprite(camera:Camera3D, texture:Texture2D, cellIndex:Int, columns:Int, rows:Int, position:Vector3, width:Float,
			height:Float):Void {
		if (columns <= 0 || rows <= 0 || cellIndex < 0 || cellIndex >= columns * rows)
			return;
		final column = cellIndex % columns;
		final row = Std.int(cellIndex / columns);
		CaxecraftTextures.drawAtlasBillboard(camera, texture, column, row, columns, rows, position, width, height, CaxecraftPalette.textureTint());
	}

	/** Draw exactly one manifest-validated cell from the 4x5 entity atlas. */
	public static function drawEntitySprite(camera:Camera3D, texture:Texture2D, cellIndex:Int, position:Vector3, width:Float, height:Float):Void {
		if (!entityAtlasCellIsValid(cellIndex))
			return;
		CaxecraftTextures.drawAtlasBillboard(camera, texture, entityAtlasColumn(cellIndex), entityAtlasRow(cellIndex), entityAtlasColumns(),
			entityAtlasRows(), position, width, height, CaxecraftPalette.textureTint());
	}

	/**
	 * Cover an authored box with one atlas picture instead of a debug color.
	 *
	 * Each metre-sized section repeats the selected cell. This keeps a wide gate
	 * or wall readable without stretching one cobble, plank, or brick across the
	 * complete structure. The caller owns collision and visibility; this method
	 * only submits the six visible faces to raylib's current 3D frame.
	 */
	public static function drawWorldBox(texture:Texture2D, cellIndex:Int, center:Vector3, width:Float, height:Float, depth:Float):Bool {
		return drawAtlasBox(texture, cellIndex, 4, 4, center, width, height, depth);
	}

	/** Cover one box with a cell from any validated regular atlas grid. */
	public static function drawAtlasBox(texture:Texture2D, cellIndex:Int, columns:Int, rows:Int, center:Vector3, width:Float, height:Float, depth:Float):Bool {
		if (columns <= 0 || rows <= 0 || cellIndex < 0 || cellIndex >= columns * rows)
			return false;
		final column = cellIndex % columns;
		final row = Std.int(cellIndex / columns);
		final halfPixelU = 0.5 / texture.width;
		final halfPixelV = 0.5 / texture.height;
		final u0 = column / (columns + 0.0) + halfPixelU;
		final u1 = (column + 1) / (columns + 0.0) - halfPixelU;
		final v0 = row / (rows + 0.0) + halfPixelV;
		final v1 = (row + 1) / (rows + 0.0) - halfPixelV;
		final centerX = center.x.toFloat();
		final centerY = center.y.toFloat();
		final centerZ = center.z.toFloat();
		final x0 = centerX - width * 0.5;
		final x1 = centerX + width * 0.5;
		final y0 = centerY - height * 0.5;
		final y1 = centerY + height * 0.5;
		final z0 = centerZ - depth * 0.5;
		final z1 = centerZ + depth * 0.5;

		Rlgl.BeginQuads(texture);
		emitTiledNorthSouth(x0, x1, y0, y1, z0, z1, u0, u1, v0, v1);
		emitTiledEastWest(x0, x1, y0, y1, z0, z1, u0, u1, v0, v1);
		emitTiledTopBottom(x0, x1, y0, y1, z0, z1, u0, u1, v0, v1);
		Rlgl.EndQuads();
		return true;
	}

	public static function drawHudGlyph(texture:Texture2D, glyph:HudGlyph, x:Int, y:Int, size:Int):Void {
		var column = 0;
		switch (glyph) {
			case HealthFull:
			case HealthHalf:
				column = 1;
			case HealthEmpty:
				column = 2;
		}
		CaxecraftTextures.drawAtlasCell(texture, column, 0, 4, 4, x, y, size, size, CaxecraftPalette.textureTint());
	}

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
			case SandBlock:
				column = 2;
				row = 3;
		}
		CaxecraftTextures.drawAtlasCell(texture, column, row, 4, 4, x, y, size, size, CaxecraftPalette.textureTint());
	}
}

/** Repeat the atlas cell across the front and back faces of one box. */
private function emitTiledNorthSouth(x0:Float, x1:Float, y0:Float, y1:Float, z0:Float, z1:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	var x = x0;
	while (x < x1) {
		final nextX = x + 1.0 < x1 ? x + 1.0 : x1;
		var y = y0;
		while (y < y1) {
			final nextY = y + 1.0 < y1 ? y + 1.0 : y1;
			Rlgl.Color(Color.rgba(211, 225, 220));
			Rlgl.Normal(0.0, 0.0, -1.0);
			vertex(u0, v1, x, y, z0);
			vertex(u0, v0, x, nextY, z0);
			vertex(u1, v0, nextX, nextY, z0);
			vertex(u1, v1, nextX, y, z0);
			Rlgl.Color(Color.rgba(235, 241, 230));
			Rlgl.Normal(0.0, 0.0, 1.0);
			vertex(u0, v1, x, y, z1);
			vertex(u1, v1, nextX, y, z1);
			vertex(u1, v0, nextX, nextY, z1);
			vertex(u0, v0, x, nextY, z1);
			y = nextY;
		}
		x = nextX;
	}
}

/** Repeat the atlas cell across the left and right faces of one box. */
private function emitTiledEastWest(x0:Float, x1:Float, y0:Float, y1:Float, z0:Float, z1:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	var z = z0;
	while (z < z1) {
		final nextZ = z + 1.0 < z1 ? z + 1.0 : z1;
		var y = y0;
		while (y < y1) {
			final nextY = y + 1.0 < y1 ? y + 1.0 : y1;
			Rlgl.Color(Color.rgba(235, 241, 230));
			Rlgl.Normal(1.0, 0.0, 0.0);
			vertex(u0, v1, x1, y, z);
			vertex(u0, v0, x1, nextY, z);
			vertex(u1, v0, x1, nextY, nextZ);
			vertex(u1, v1, x1, y, nextZ);
			Rlgl.Color(Color.rgba(211, 225, 220));
			Rlgl.Normal(-1.0, 0.0, 0.0);
			vertex(u0, v1, x0, y, z);
			vertex(u1, v1, x0, y, nextZ);
			vertex(u1, v0, x0, nextY, nextZ);
			vertex(u0, v0, x0, nextY, z);
			y = nextY;
		}
		z = nextZ;
	}
}

/** Repeat the atlas cell across the top and bottom faces of one box. */
private function emitTiledTopBottom(x0:Float, x1:Float, y0:Float, y1:Float, z0:Float, z1:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	var x = x0;
	while (x < x1) {
		final nextX = x + 1.0 < x1 ? x + 1.0 : x1;
		var z = z0;
		while (z < z1) {
			final nextZ = z + 1.0 < z1 ? z + 1.0 : z1;
			Rlgl.Color(Color.rgba(255, 255, 255));
			Rlgl.Normal(0.0, 1.0, 0.0);
			vertex(u0, v0, x, y1, z);
			vertex(u0, v1, x, y1, nextZ);
			vertex(u1, v1, nextX, y1, nextZ);
			vertex(u1, v0, nextX, y1, z);
			Rlgl.Color(Color.rgba(178, 191, 188));
			Rlgl.Normal(0.0, -1.0, 0.0);
			vertex(u0, v0, x, y0, z);
			vertex(u1, v0, nextX, y0, z);
			vertex(u1, v1, nextX, y0, nextZ);
			vertex(u0, v1, x, y0, nextZ);
			z = nextZ;
		}
		x = nextX;
	}
}

/** Submit one atlas vertex to the already-open textured quad batch. */
private inline function vertex(u:Float, v:Float, x:Float, y:Float, z:Float):Void {
	Rlgl.TexCoord(u, v);
	Rlgl.Vertex(x, y, z);
}
#end
