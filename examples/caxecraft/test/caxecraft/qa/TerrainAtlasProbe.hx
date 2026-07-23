package caxecraft.qa;

import caxecraft.app.TerrainAtlas;
import caxecraft.app.TerrainAtlas.TerrainSheet;
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
		require(TerrainAtlas.tile(BlockKind.Sand, VoxelFace.Top) == TerrainTile.RiverSand, "river sand");
		require(TerrainAtlas.tile(BlockKind.Wood, VoxelFace.Top) == TerrainTile.LogTop, "wood end grain");
		require(TerrainAtlas.tile(BlockKind.Wood, VoxelFace.North) == TerrainTile.BarkSide, "wood bark");
		require(TerrainAtlas.tile(BlockKind.Leaves, VoxelFace.Top) == TerrainTile.LeafyCanopy, "leafy canopy");
		require(TerrainAtlas.tile(BlockKind.Snow, VoxelFace.Top) == TerrainTile.SnowTop, "snow top");
		require(TerrainAtlas.tile(BlockKind.Snow, VoxelFace.East) == TerrainTile.SnowSide, "snow side");
		require(TerrainAtlas.tile(BlockKind.Ash, VoxelFace.Top) == TerrainTile.AshTop, "ash top");
		require(TerrainAtlas.tile(BlockKind.Ash, VoxelFace.West) == TerrainTile.AshSide, "ash side");
		require(TerrainAtlas.sheet(BlockKind.Leaves) == TerrainSheet.Base, "forest atlas owner");
		require(TerrainAtlas.sheet(BlockKind.Snow) == TerrainSheet.Adventure, "snow atlas owner");
		require(TerrainAtlas.sheet(BlockKind.Ash) == TerrainSheet.Adventure, "ash atlas owner");

		final halfPixel = 0.5 / 1024.0;
		require(TerrainAtlas.uMin(TerrainTile.MeadowGrassTop, halfPixel) == halfPixel, "first column left edge");
		require(TerrainAtlas.uMax(TerrainTile.MeadowGrassTop, halfPixel) == 0.25 - halfPixel, "first column right edge");
		require(TerrainAtlas.vMin(TerrainTile.FoundationRock, halfPixel) == 0.75 + halfPixel, "last row top edge");
		require(TerrainAtlas.vMax(TerrainTile.FoundationRock, halfPixel) == 1.0 - halfPixel, "last row bottom edge");
		require(TerrainAtlas.uMin(TerrainTile.AshTop, halfPixel) == halfPixel, "adventure first column left edge");
		require(TerrainAtlas.vMin(TerrainTile.AshSide, halfPixel) == 0.25 + halfPixel, "adventure ash row");

		Sys.println("caxecraft-terrain-atlas: two typed sheets, material faces, and inset UV bounds passed");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
