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

/** The reviewed image file that owns a terrain picture. */
enum TerrainSheet {
	Base;
	Adventure;
}

/** Semantic names for reviewed terrain pictures; no gameplay code stores UVs. */
enum TerrainTile {
	MeadowGrassTop;
	GrassSide;
	RichSoil;
	SlateStone;
	FoundationRock;
	RiverSand;
	BarkSide;
	LogTop;
	LeafyCanopy;
	SnowTop;
	SnowSide;
	AshTop;
	AshSide;
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

	/** Select one texture before the renderer begins a coherent material batch. */
	public static function sheet(kind:BlockKind):TerrainSheet {
		return switch (kind) {
			case Snow | Ash: Adventure;
			case Air | Grass | Dirt | Stone | Bedrock | Sand | Wood | Leaves: Base;
		};
	}

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
			case Sand: RiverSand;
			case Wood:
				switch (face) {
					case Top | Bottom: LogTop;
					case North | South | East | West: BarkSide;
				};
			case Leaves: LeafyCanopy;
			case Snow:
				switch (face) {
					case Top: SnowTop;
					case Bottom | North | South | East | West: SnowSide;
				};
			case Ash:
				switch (face) {
					case Top: AshTop;
					case Bottom | North | South | East | West: AshSide;
				};
			case Air: RichSoil;
		};
	}

	public static function column(tile:TerrainTile):Int {
		return switch (tile) {
			case MeadowGrassTop | RiverSand | LeafyCanopy | SnowTop | AshTop: 0;
			case GrassSide | SnowSide | AshSide: 1;
			case RichSoil | BarkSide: 2;
			case SlateStone | FoundationRock | LogTop: 3;
		};
	}

	public static function row(tile:TerrainTile):Int {
		return switch (tile) {
			case MeadowGrassTop | GrassSide | RichSoil | SlateStone | SnowTop | SnowSide: 0;
			case RiverSand | BarkSide | LogTop | AshTop | AshSide: 1;
			case LeafyCanopy: 2;
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
