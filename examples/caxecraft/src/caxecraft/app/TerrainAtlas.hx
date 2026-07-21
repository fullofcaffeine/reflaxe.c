package caxecraft.app;

import caxecraft.domain.BlockKind;

/** The six outward faces of one voxel. */
enum VoxelFace {
	Top;
	Bottom;
	North;
	South;
	East;
	West;
}

/** Semantic names for the base terrain pictures used by the current world. */
enum TerrainTile {
	MeadowGrassTop;
	GrassSide;
	RichSoil;
	SlateStone;
	FoundationRock;
}

/**
 * Maps gameplay materials to pictures in the reviewed 4x4 terrain atlas.
 *
 * `BlockKind` never stores row or column numbers. That keeps saved worlds and
 * gameplay independent of this particular art pack: another renderer can use
 * the same world data, while a replacement atlas changes only this table.
 */
final class TerrainAtlas {
	public static inline final COLUMNS:Int = 4;
	public static inline final ROWS:Int = 4;

	public static function tile(kind:BlockKind, face:VoxelFace):TerrainTile {
		return switch (kind) {
			case Grass:
				switch (face) {
					case Top: MeadowGrassTop;
					case Bottom: RichSoil;
					case North | South | East | West: GrassSide;
				};
			case Dirt: RichSoil;
			case Stone: SlateStone;
			case Bedrock: FoundationRock;
			case Air: RichSoil;
		};
	}

	public static function column(tile:TerrainTile):Int {
		return switch (tile) {
			case MeadowGrassTop: 0;
			case GrassSide: 1;
			case RichSoil: 2;
			case SlateStone | FoundationRock: 3;
		};
	}

	public static function row(tile:TerrainTile):Int {
		return switch (tile) {
			case MeadowGrassTop | GrassSide | RichSoil | SlateStone: 0;
			case FoundationRock: 3;
		};
	}

	/** Normalized left edge, moved inward to avoid sampling a neighboring cell. */
	public static inline function uMin(tile:TerrainTile, halfPixel:Float):Float
		return column(tile) / COLUMNS + halfPixel;

	/** Normalized right edge, moved inward to avoid sampling a neighboring cell. */
	public static inline function uMax(tile:TerrainTile, halfPixel:Float):Float
		return (column(tile) + 1) / COLUMNS - halfPixel;

	public static inline function vMin(tile:TerrainTile, halfPixel:Float):Float
		return row(tile) / ROWS + halfPixel;

	public static inline function vMax(tile:TerrainTile, halfPixel:Float):Float
		return (row(tile) + 1) / ROWS - halfPixel;
}
