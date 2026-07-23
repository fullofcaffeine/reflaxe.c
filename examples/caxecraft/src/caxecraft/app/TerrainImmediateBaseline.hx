package caxecraft.app;

#if (c && caxecraft_renderer_baseline)
import caxecraft.app.TerrainAtlas.TerrainSheet;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.app.TerrainRenderer.TerrainRenderCounters;
import caxecraft.app.TerrainRenderer.emitFace;
import caxecraft.app.TerrainRenderer.squareDistance;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import raylib.Rlgl;
import raylib.Texture2D;

/** Internal count for one baseline atlas scan. */
private typedef ImmediateSheetCounters = {
	final visible:Int;
	final faces:Int;
	final drawCalls:Int;
}

/**
	Benchmark-only copy of the renderer used before the chunk cache.

	Each frame scans all 16,384 cells once per ready atlas and rediscovers every
	exposed neighbor. It shares face submission, atlas selection, and tinting with
	the current renderer, so the benchmark changes only discovery/caching policy.
	The `caxecraft_renderer_baseline` define is accepted only by the dedicated
	benchmark command; ordinary builds do not type or emit this module.
**/
function drawImmediate(cells:WorldCells, baseTexture:Texture2D, baseReady:Bool, adventureTexture:Texture2D, adventureReady:Bool, playerX:Float,
		playerZ:Float):TerrainRenderCounters {
	final playerCellX = Std.int(playerX);
	final playerCellZ = Std.int(playerZ);
	final base = drawSheet(cells, baseTexture, baseReady, TerrainSheet.Base, playerCellX, playerCellZ);
	final adventure = drawSheet(cells, adventureTexture, adventureReady, TerrainSheet.Adventure, playerCellX, playerCellZ);
	return {
		visible: base.visible + adventure.visible,
		faces: base.faces + adventure.faces,
		drawCalls: base.drawCalls + adventure.drawCalls,
		rebuiltChunks: 0,
		totalRebuiltChunks: 0,
		#if caxecraft_render_benchmark
		preparationMicroseconds: 0,
		#end
		cacheValid: true
	};
}

/** Submit one atlas after performing the former complete-world scan. */
private function drawSheet(cells:WorldCells, texture:Texture2D, textureReady:Bool, sheet:TerrainSheet, playerCellX:Int,
		playerCellZ:Int):ImmediateSheetCounters {
	if (!textureReady)
		return {visible: 0, faces: 0, drawCalls: 0};

	final halfPixel = 0.5 / texture.width;
	var visible = 0;
	var faces = 0;
	Rlgl.BeginQuads(texture);
	var z = 0;
	while (z < World.DEPTH) {
		var y = 0;
		while (y < World.HEIGHT) {
			var x = 0;
			while (x < World.WIDTH) {
				final kind = World.query(cells, World.coord(x, y, z));
				if (World.isSolid(kind) && TerrainAtlas.sheet(kind) == sheet) {
					var blockVisible = false;
					final distance = squareDistance(x, z, playerCellX, playerCellZ);
					if (!World.isSolid(World.query(cells, World.coord(x, y + 1, z)))) {
						emitFace(kind, VoxelFace.Top, x, y, z, halfPixel, distance);
						faces++;
						blockVisible = true;
					}
					if (!World.isSolid(World.query(cells, World.coord(x, y - 1, z)))) {
						emitFace(kind, VoxelFace.Bottom, x, y, z, halfPixel, distance);
						faces++;
						blockVisible = true;
					}
					if (!World.isSolid(World.query(cells, World.coord(x, y, z - 1)))) {
						emitFace(kind, VoxelFace.North, x, y, z, halfPixel, distance);
						faces++;
						blockVisible = true;
					}
					if (!World.isSolid(World.query(cells, World.coord(x, y, z + 1)))) {
						emitFace(kind, VoxelFace.South, x, y, z, halfPixel, distance);
						faces++;
						blockVisible = true;
					}
					if (!World.isSolid(World.query(cells, World.coord(x + 1, y, z)))) {
						emitFace(kind, VoxelFace.East, x, y, z, halfPixel, distance);
						faces++;
						blockVisible = true;
					}
					if (!World.isSolid(World.query(cells, World.coord(x - 1, y, z)))) {
						emitFace(kind, VoxelFace.West, x, y, z, halfPixel, distance);
						faces++;
						blockVisible = true;
					}
					if (blockVisible)
						visible++;
				}
				x++;
			}
			y++;
		}
		z++;
	}
	Rlgl.EndQuads();
	return {visible: visible, faces: faces, drawCalls: faces > 0 ? 1 : 0};
}
#end
