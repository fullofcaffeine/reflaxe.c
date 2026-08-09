package caxecraft.app;

/**
	Owns the fixed grid used by the reviewed Caxecraft entity atlas.

	The entity image has four columns and five rows. Keeping that fact here lets
	the renderer and the renderer-free presentation test use the same cell
	boundary without coupling gameplay or campaign content to pixel coordinates.
**/
/** Number of actor poses across one entity-atlas row. */
inline function entityAtlasColumns():Int
	return 4;

/** Number of actor rows in the entity atlas. */
inline function entityAtlasRows():Int
	return 5;

/** Total number of cells that the entity atlas can name. */
inline function entityAtlasCellCount():Int
	return entityAtlasColumns() * entityAtlasRows();

/** Report whether an index selects exactly one reviewed entity cell. */
inline function entityAtlasCellIsValid(cellIndex:Int):Bool
	return cellIndex >= 0 && cellIndex < entityAtlasCellCount();

/** Return the horizontal cell coordinate, or `-1` for an invalid index. */
inline function entityAtlasColumn(cellIndex:Int):Int
	return entityAtlasCellIsValid(cellIndex) ? cellIndex % entityAtlasColumns() : -1;

/** Return the vertical cell coordinate, or `-1` for an invalid index. */
inline function entityAtlasRow(cellIndex:Int):Int
	return entityAtlasCellIsValid(cellIndex) ? Std.int(cellIndex / entityAtlasColumns()) : -1;
