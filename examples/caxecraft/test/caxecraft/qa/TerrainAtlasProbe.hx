package caxecraft.qa;

import caxecraft.app.TerrainAtlas;
import caxecraft.app.TerrainAtlas.TerrainTile;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.domain.BlockKind;

/** Focused target-neutral proof for material-to-picture selection and UVs. */
final class TerrainAtlasProbe {
	static function main():Void {
		require(TerrainAtlas.tile(BlockKind.Grass, VoxelFace.Top) == TerrainTile.MeadowGrassTop, "grass top");
		require(TerrainAtlas.tile(BlockKind.Grass, VoxelFace.Bottom) == TerrainTile.RichSoil, "grass bottom");
		for (side in [VoxelFace.North, VoxelFace.South, VoxelFace.East, VoxelFace.West])
			require(TerrainAtlas.tile(BlockKind.Grass, side) == TerrainTile.GrassSide, "grass side");
		require(TerrainAtlas.tile(BlockKind.Dirt, VoxelFace.Top) == TerrainTile.RichSoil, "dirt");
		require(TerrainAtlas.tile(BlockKind.Stone, VoxelFace.North) == TerrainTile.SlateStone, "stone");
		require(TerrainAtlas.tile(BlockKind.Bedrock, VoxelFace.Bottom) == TerrainTile.FoundationRock, "bedrock");

		final halfPixel = 0.5 / 1024.0;
		require(TerrainAtlas.uMin(TerrainTile.MeadowGrassTop, halfPixel) == halfPixel, "first column left edge");
		require(TerrainAtlas.uMax(TerrainTile.MeadowGrassTop, halfPixel) == 0.25 - halfPixel, "first column right edge");
		require(TerrainAtlas.vMin(TerrainTile.FoundationRock, halfPixel) == 0.75 + halfPixel, "last row top edge");
		require(TerrainAtlas.vMax(TerrainTile.FoundationRock, halfPixel) == 1.0 - halfPixel, "last row bottom edge");

		Sys.println("caxecraft-terrain-atlas: typed faces, material cells, and inset UV bounds passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
