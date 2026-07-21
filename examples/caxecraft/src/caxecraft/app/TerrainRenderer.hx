package caxecraft.app;

#if c
import caxecraft.domain.BlockKind;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.app.TerrainAtlas.VoxelFace;
import raylib.Color;
import raylib.Rlgl;
import raylib.Texture2D;

/** Counts shown by the developer HUD after one terrain submission. */
typedef TerrainRenderCounters = {
	final visible:Int;
	final drawCalls:Int;
}

/**
 * Immediate textured renderer for the complete current 32x32 world.
 *
 * One `Rlgl.BeginQuads`/`EndQuads` region submits every face that touches air.
 * Raylib can flush its bounded internal vertex buffer when necessary without
 * changing textures or forcing one native draw setup per block. This removes
 * the old moving nine-block boundary while keeping the first implementation
 * small and readable. Larger editor-authored worlds will replace this scan
 * with chunk meshes; gameplay storage and atlas policy do not change.
 */
final class TerrainRenderer {
	public static function draw(cells:WorldCells, texture:Texture2D, textureReady:Bool, playerX:Float, playerZ:Float):TerrainRenderCounters {
		if (!textureReady)
			return {visible: 0, drawCalls: 0};

		final halfPixel = 0.5 / texture.width;
		final playerCellX = Std.int(playerX);
		final playerCellZ = Std.int(playerZ);
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
					if (World.isSolid(kind)) {
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
		var drawCalls = 0;
		if (faces > 0)
			drawCalls = 1;
		return {visible: visible, drawCalls: drawCalls};
	}

	/** Chebyshev distance gives square voxel rings without a square root. */
	static inline function squareDistance(x:Int, z:Int, playerX:Int, playerZ:Int):Int {
		var dx = x - playerX;
		if (dx < 0)
			dx = -dx;
		var dz = z - playerZ;
		if (dz < 0)
			dz = -dz;
		return dx > dz ? dx : dz;
	}

	static function emitFace(kind:BlockKind, face:VoxelFace, x:Int, y:Int, z:Int, halfPixel:Float, distance:Int):Void {
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

	// Keep this as one ordinary helper rather than an inline helper. The Float
	// parameter types are the checked boundary before values narrow to raylib's
	// binary32 ABI, and the generated C retains one small reusable vertex step.
	static function vertex(u:Float, v:Float, x:Float, y:Float, z:Float):Void {
		Rlgl.TexCoord(u, v);
		Rlgl.Vertex(x, y, z);
	}

	/**
	 * Vertex color multiplies the atlas pixel. Face direction gives the terrain
	 * shape, while two distant bands blend toward the sky and soften the finite
	 * map edge. The later chunk renderer can replace these bands with real fog.
	 */
	static function faceTint(face:VoxelFace, distance:Int):Color {
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
}
#end
