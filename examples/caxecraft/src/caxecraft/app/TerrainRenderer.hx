package caxecraft.app;

#if c
import caxecraft.app.TerrainAtlas.TerrainSheet;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.app.TerrainChunkLayout.CHUNK_COUNT;
import caxecraft.app.TerrainChunkLayout.unpackFace;
import caxecraft.app.TerrainChunkLayout.unpackKind;
#if caxecraft_renderer_baseline
import caxecraft.app.TerrainImmediateBaseline.drawImmediate;
#end
import caxecraft.domain.BlockCoord;
import caxecraft.domain.BlockKind;
import caxecraft.domain.WorldCells;
import raylib.Color;
import raylib.Raylib;
import raylib.Rlgl;
import raylib.Texture2D;

/** Counts shown by the developer HUD after one terrain submission. */
typedef TerrainRenderCounters = {
	/** Solid voxels with at least one exposed face in a ready texture sheet. */
	final visible:Int;

	/** Exposed opaque faces submitted from the cache. */
	final faces:Int;

	/** Coherent texture batches submitted to rlgl. */
	final drawCalls:Int;

	/** Dirty chunks rebuilt immediately before this submission. */
	final rebuiltChunks:Int;

	/** Chunks rebuilt since this renderer was constructed. */
	final totalRebuiltChunks:Int;

	#if caxecraft_render_benchmark
	/** Time spent preparing dirty chunks; ordinary builds omit this field. */
	final preparationMicroseconds:Int;
	#end

	/** False only if the fixed face-capacity invariant was violated. */
	final cacheValid:Bool;
}

/** Small internal result for one atlas-owned submission. */
private typedef TerrainSheetCounters = {
	final visible:Int;
	final faces:Int;
	final drawCalls:Int;
}

/**
	Stateful chunked renderer for the complete current 32x32 world.

	`TerrainChunkCache` discovers exposed faces once, then rebuilds only chunks
	touched by a successful edit. This class owns that derived state across frames,
	which gives it a meaningful identity and lifetime; the world remains owned by
	`GameSession`. Every ready atlas still uses one direct
	`Rlgl.BeginQuads`/`EndQuads` region, so generated C stays close to a careful
	hand-written raylib loop and no native mesh ownership or heap runtime is added.

	The cache removes repeated neighborhood discovery, not vertex submission.
	Raylib may flush its bounded internal render buffer inside a region. A future
	GPU-mesh path must earn its wider ABI and cleanup policy with measurements;
	this bounded renderer does not claim arbitrary-distance streaming.
**/
final class TerrainRenderer {
	#if !caxecraft_renderer_baseline
	/** Persistent derived face data embedded directly in this renderer. */
	final cache:TerrainChunkCache = new TerrainChunkCache();

	/** Monotonic diagnostic counter; it never participates in gameplay. */
	var totalRebuiltChunks:Int = 0;
	#end

	/** Construct the renderer with every chunk initially dirty. */
	public function new() {}

	/** Notify presentation after one successful terrain edit. */
	public inline function invalidate(coord:BlockCoord):Void {
		#if !caxecraft_renderer_baseline
		cache.invalidate(coord);
		#end
	}

	/** Rebuild dirty chunks and submit at most one batch per ready opaque atlas. */
	public function draw(cells:WorldCells, baseTexture:Texture2D, baseReady:Bool, adventureTexture:Texture2D, adventureReady:Bool, playerX:Float,
			playerZ:Float):TerrainRenderCounters {
		#if caxecraft_renderer_baseline
		return drawImmediate(cells, baseTexture, baseReady, adventureTexture, adventureReady, playerX, playerZ);
		#else
		#if caxecraft_render_benchmark
		final preparationStarted = Raylib.GetTime();
		#end
		final preparation = cache.prepare(cells);
		#if caxecraft_render_benchmark
		final preparationMicroseconds = Std.int((Raylib.GetTime() - preparationStarted) * 1000000.0);
		#end
		totalRebuiltChunks += preparation.rebuiltChunks;
		if (!preparation.valid) {
			// The checkerboard proof reaches the exact maximum. If that invariant ever
			// drifts, omit partial geometry and make the failure visible in telemetry.
			return {
				visible: 0,
				faces: 0,
				drawCalls: 0,
				rebuiltChunks: preparation.rebuiltChunks,
				totalRebuiltChunks: totalRebuiltChunks,
				#if caxecraft_render_benchmark
				preparationMicroseconds: preparationMicroseconds,
				#end
				cacheValid: false
			};
		}
		final playerCellX = Std.int(playerX);
		final playerCellZ = Std.int(playerZ);
		final base = drawSheet(baseTexture, baseReady, TerrainSheet.Base, playerCellX, playerCellZ);
		final adventure = drawSheet(adventureTexture, adventureReady, TerrainSheet.Adventure, playerCellX, playerCellZ);
		return {
			visible: base.visible + adventure.visible,
			faces: base.faces + adventure.faces,
			drawCalls: base.drawCalls + adventure.drawCalls,
			rebuiltChunks: preparation.rebuiltChunks,
			totalRebuiltChunks: totalRebuiltChunks,
			#if caxecraft_render_benchmark
			preparationMicroseconds: preparationMicroseconds,
			#end
			cacheValid: true
		};
		#end
	}

	#if !caxecraft_renderer_baseline
	/** Replay cached faces through one coherent texture owner. */
	function drawSheet(texture:Texture2D, textureReady:Bool, sheet:TerrainSheet, playerCellX:Int, playerCellZ:Int):TerrainSheetCounters {
		if (!textureReady)
			return {visible: 0, faces: 0, drawCalls: 0};

		final halfPixel = 0.5 / texture.width;
		var faces = 0;
		Rlgl.BeginQuads(texture);
		var chunk = 0;
		while (chunk < CHUNK_COUNT) {
			final start = cache.partitionStart(chunk);
			final count = cache.chunkFaceCount(chunk);
			var offset = 0;
			while (offset < count) {
				final index = start + offset;
				final packed = cache.packedAt(index);
				final kind = unpackKind(packed);
				if (TerrainAtlas.sheet(kind) == sheet) {
					final x = cache.xAt(index);
					final y = cache.yAt(index);
					final z = cache.zAt(index);
					emitFace(kind, unpackFace(packed), x, y, z, halfPixel, squareDistance(x, z, playerCellX, playerCellZ));
					faces++;
				}
				offset++;
			}
			chunk++;
		}
		Rlgl.EndQuads();
		return {visible: cache.visibleBlocks(sheet), faces: faces, drawCalls: faces > 0 ? 1 : 0};
	}
	#end
}

/** Chebyshev distance gives square voxel rings without a square root. */
function squareDistance(x:Int, z:Int, playerX:Int, playerZ:Int):Int {
	var dx = x - playerX;
	if (dx < 0)
		dx = -dx;
	var dz = z - playerZ;
	if (dz < 0)
		dz = -dz;
	return dx > dz ? dx : dz;
}

/** Emit one already-discovered face with its atlas cell and distance tint. */
function emitFace(kind:BlockKind, face:VoxelFace, x:Int, y:Int, z:Int, halfPixel:Float, distance:Int):Void {
	final tile = TerrainAtlas.tile(kind, face);
	final u0 = TerrainAtlas.uMin(tile, halfPixel);
	final u1 = TerrainAtlas.uMax(tile, halfPixel);
	final v0 = TerrainAtlas.vMin(tile, halfPixel);
	final v1 = TerrainAtlas.vMax(tile, halfPixel);
	Rlgl.Color(faceTint(face, distance));
	switch (face) {
		case Top:
			Rlgl.Normal(0.0, 1.0, 0.0);
			vertex(u0, v0, x, y + 1, z);
			vertex(u0, v1, x, y + 1, z + 1);
			vertex(u1, v1, x + 1, y + 1, z + 1);
			vertex(u1, v0, x + 1, y + 1, z);
		case Bottom:
			Rlgl.Normal(0.0, -1.0, 0.0);
			vertex(u0, v0, x, y, z);
			vertex(u1, v0, x + 1, y, z);
			vertex(u1, v1, x + 1, y, z + 1);
			vertex(u0, v1, x, y, z + 1);
		case North:
			Rlgl.Normal(0.0, 0.0, -1.0);
			vertex(u0, v1, x, y, z);
			vertex(u0, v0, x, y + 1, z);
			vertex(u1, v0, x + 1, y + 1, z);
			vertex(u1, v1, x + 1, y, z);
		case South:
			Rlgl.Normal(0.0, 0.0, 1.0);
			vertex(u0, v1, x, y, z + 1);
			vertex(u1, v1, x + 1, y, z + 1);
			vertex(u1, v0, x + 1, y + 1, z + 1);
			vertex(u0, v0, x, y + 1, z + 1);
		case East:
			Rlgl.Normal(1.0, 0.0, 0.0);
			vertex(u0, v1, x + 1, y, z);
			vertex(u0, v0, x + 1, y + 1, z);
			vertex(u1, v0, x + 1, y + 1, z + 1);
			vertex(u1, v1, x + 1, y, z + 1);
		case West:
			Rlgl.Normal(-1.0, 0.0, 0.0);
			vertex(u0, v1, x, y, z);
			vertex(u1, v1, x, y, z + 1);
			vertex(u1, v0, x, y + 1, z + 1);
			vertex(u0, v0, x, y + 1, z);
	}
}

/**
	Submit one textured vertex on the renderer's hottest repeated path.

	`inline` asks Haxe to substitute this small body at each call. That avoids a
	tiny C function call per vertex while keeping the readable Haxe helper. The
	declared `Float` parameters still matter after substitution: haxe.c preserves
	Haxe's implicit numeric conversion before the Rlgl facade explicitly narrows
	the value to raylib's binary32 (`float`) application binary interface.
**/
private inline function vertex(u:Float, v:Float, x:Float, y:Float, z:Float):Void {
	Rlgl.TexCoord(u, v);
	Rlgl.Vertex(x, y, z);
}

/**
	Vertex color multiplies the atlas pixel. Face direction gives the terrain
	shape, while two distant bands blend toward the sky and soften the finite map
	edge. A later fog slice may replace the bands without changing the cache.
**/
private function faceTint(face:VoxelFace, distance:Int):Color {
	if (distance >= 22) {
		return switch (face) {
			case Top: Color.rgba(220, 235, 226);
			case South | East: Color.rgba(197, 218, 212);
			case North | West: Color.rgba(181, 205, 204);
			case Bottom: Color.rgba(158, 182, 185);
		};
	}
	if (distance >= 12) {
		return switch (face) {
			case Top: Color.rgba(240, 246, 239);
			case South | East: Color.rgba(219, 231, 222);
			case North | West: Color.rgba(199, 216, 211);
			case Bottom: Color.rgba(173, 190, 188);
		};
	}
	return switch (face) {
		case Top: Color.rgba(255, 255, 255);
		case South | East: Color.rgba(235, 241, 230);
		case North | West: Color.rgba(211, 225, 220);
		case Bottom: Color.rgba(178, 191, 188);
	};
}
#end
