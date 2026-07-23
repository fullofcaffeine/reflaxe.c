package caxecraft.qa;

import caxecraft.app.TerrainAtlas.TerrainSheet;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.app.TerrainChunkCache;
import caxecraft.app.TerrainChunkLayout.CHUNK_COUNT;
import caxecraft.app.TerrainChunkLayout.CHUNK_DEPTH;
import caxecraft.app.TerrainChunkLayout.CHUNK_WIDTH;
import caxecraft.app.TerrainChunkLayout.CHUNKS_X;
import caxecraft.app.TerrainChunkLayout.CHUNKS_Z;
import caxecraft.app.TerrainChunkLayout.FACE_CAPACITY;
import caxecraft.app.TerrainChunkLayout.FACES_PER_CHUNK;
import caxecraft.app.TerrainChunkLayout.packFace;
import caxecraft.app.TerrainChunkLayout.unpackFace;
import caxecraft.app.TerrainChunkLayout.unpackKind;
import caxecraft.domain.BlockKind;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
#if c
import c.CArray;
import c.UInt8;
import caxecraft.domain.WorldVolume;
#end

/**
	Cross-target specification for terrain chunks and dirty-boundary rebuilds.

	These assertions exercise the same Haxe cache used by the Raylib renderer.
	Eval provides an independent array-backed result, while generated native C
	uses the fixed byte arrays and sanitizer lane. The test never opens a window;
	graphical pilots separately prove that cached faces reach the real renderer.
**/
var observed:Int = 0;

function main():Void {
	#if c
	observed = selfCheck();
	#else
	Sys.println(selfCheck());
	#end
}

/** Return zero, or the stable number of the first broken cache rule. */
function selfCheck():Int {
	// The current bounded stack-object slice requires construction in the entry
	// block. haxe_c-71g owns the broader, path-dominated lifetime rule; keeping
	// this test owner unconditional does not change the renderer's semantics.
	final cache = new TerrainChunkCache();
	if (CHUNK_WIDTH * CHUNKS_X != World.WIDTH
		|| CHUNK_DEPTH * CHUNKS_Z != World.DEPTH
		|| CHUNK_COUNT != 16
		|| FACE_CAPACITY != 49152)
		return 1;
	if (unpackKind(packFace(BlockKind.Ash, VoxelFace.West)) != BlockKind.Ash
		|| unpackFace(packFace(BlockKind.Ash, VoxelFace.West)) != VoxelFace.West)
		return 2;

	#if c
	var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
	var cells:WorldCells = storage.span();
	#else
	var cells:WorldCells = [];
	var emptyIndex = 0;
	while (emptyIndex < World.VOLUME) {
		cells.push(0);
		emptyIndex++;
	}
	#end
	var preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.rebuiltChunks != 16 || preparation.faces != 0 || preparation.visibleBlocks != 0)
		return 3;
	preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.rebuiltChunks != 0)
		return 4;

	final interior = World.coord(2, 2, 2);
	World.replace(cells, interior, BlockKind.Stone);
	if (cache.invalidate(interior) != 1 || cache.invalidate(interior) != 0)
		return 5;
	preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.rebuiltChunks != 1 || preparation.faces != 6 || preparation.visibleBlocks != 1)
		return 6;

	final adjacent = World.coord(3, 2, 2);
	World.replace(cells, adjacent, BlockKind.Stone);
	if (cache.invalidate(adjacent) != 1)
		return 7;
	preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.rebuiltChunks != 1 || preparation.faces != 10 || preparation.visibleBlocks != 2)
		return 8;

	clear(cells);
	cache.invalidateAll();
	final west = World.coord(7, 2, 2);
	final east = World.coord(8, 2, 2);
	World.replace(cells, west, BlockKind.Stone);
	World.replace(cells, east, BlockKind.Stone);
	preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.faces != 10 || cache.chunkFaceCount(0) != 5 || cache.chunkFaceCount(1) != 5)
		return 9;
	World.replace(cells, east, BlockKind.Air);
	if (cache.invalidate(east) != 2)
		return 10;
	preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.rebuiltChunks != 2 || preparation.faces != 6 || cache.chunkFaceCount(0) != 6 || cache.chunkFaceCount(1) != 0)
		return 11;

	preparation = cache.prepare(cells);
	if (preparation.rebuiltChunks != 0 || cache.invalidate(World.coord(7, 2, 7)) != 3 || cache.invalidate(World.coord(7, 2, 7)) != 0)
		return 12;
	preparation = cache.prepare(cells);
	if (!preparation.valid || preparation.rebuiltChunks != 3)
		return 13;

	clear(cells);
	var z = 0;
	while (z < CHUNK_DEPTH) {
		var y = 0;
		while (y < World.HEIGHT) {
			var x = 0;
			while (x < CHUNK_WIDTH) {
				if (((x + y + z) & 1) == 0)
					World.replace(cells, World.coord(x, y, z), BlockKind.Snow);
				x++;
			}
			y++;
		}
		z++;
	}
	cache.invalidateAll();
	preparation = cache.prepare(cells);
	if (!preparation.valid
		|| cache.chunkFaceCount(0) != FACES_PER_CHUNK
		|| preparation.faces != FACES_PER_CHUNK
		|| cache.visibleBlocks(TerrainSheet.Base) != 0
		|| cache.visibleBlocks(TerrainSheet.Adventure) != 512)
		return 14;
	return 0;
}

/** Reset the shared fixture without adding a second terrain implementation. */
function clear(cells:WorldCells):Void {
	var index = 0;
	while (index < World.VOLUME) {
		final x = index % World.WIDTH;
		final layer = Std.int(index / World.WIDTH);
		final y = layer % World.HEIGHT;
		final z = Std.int(layer / World.HEIGHT);
		World.replace(cells, World.coord(x, y, z), BlockKind.Air);
		index++;
	}
}
