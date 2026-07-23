package caxecraft.app;

import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.domain.BlockCoord;
import caxecraft.domain.BlockKind;
import caxecraft.domain.World;

/**
	Pure chunk-coordinate rules shared by the terrain cache and its tests.

	A chunk is an 8-by-16-by-8 rendering group, not a level or gameplay region.
	The current finite world therefore has four chunks across and four deep. The
	full world height stays together because 16 cells are already a small bounded
	column and terrain edits need no vertical chunk neighbor.
**/
inline final CHUNK_WIDTH:Int = 8;

inline final CHUNK_DEPTH:Int = 8;
inline final CHUNKS_X:Int = 4;
inline final CHUNKS_Z:Int = 4;
inline final CHUNK_COUNT:Int = CHUNKS_X * CHUNKS_Z;

/**
	Maximum exposed faces owned by one chunk.

	The 8-by-16-by-8 voxel grid is bipartite: color cells like a 3D chessboard.
	At most one color can expose all six faces, so the checkerboard maximum is
	half of 1024 cells times six faces, or 3072. The focused checkerboard test
	reaches this exact bound and protects the fixed-storage proof.
**/
inline final FACES_PER_CHUNK:Int = 3072;

inline final FACE_CAPACITY:Int = CHUNK_COUNT * FACES_PER_CHUNK;

/** Return the stable row-major chunk that owns one in-bounds voxel. */
function chunkFor(coord:BlockCoord):Int {
	if (!World.contains(coord))
		return -1;
	return Std.int(coord.x / CHUNK_WIDTH) + CHUNKS_X * Std.int(coord.z / CHUNK_DEPTH);
}

/** Return the first x coordinate owned by `chunk`, or `-1` when invalid. */
function chunkOriginX(chunk:Int):Int {
	if (chunk < 0 || chunk >= CHUNK_COUNT)
		return -1;
	return (chunk % CHUNKS_X) * CHUNK_WIDTH;
}

/** Return the first z coordinate owned by `chunk`, or `-1` when invalid. */
function chunkOriginZ(chunk:Int):Int {
	if (chunk < 0 || chunk >= CHUNK_COUNT)
		return -1;
	return Std.int(chunk / CHUNKS_X) * CHUNK_DEPTH;
}

/** Return the fixed face-storage partition owned by one chunk. */
function facePartitionStart(chunk:Int):Int
	return chunk * FACES_PER_CHUNK;

/** Encode one material and outward face in a single byte-sized integer. */
function packFace(kind:BlockKind, face:VoxelFace):Int
	return World.kindCode(kind) * 6 + faceCode(face);

/** Recover the material stored by `packFace`. */
function unpackKind(packed:Int):BlockKind
	return World.kindFromCode(Std.int(packed / 6));

/** Recover the outward face stored by `packFace`. */
function unpackFace(packed:Int):VoxelFace {
	return switch (packed % 6) {
		case 0: Top;
		case 1: Bottom;
		case 2: North;
		case 3: South;
		case 4: East;
		default: West;
	};
}

/** Stable compact code used only inside the cache byte. */
function faceCode(face:VoxelFace):Int {
	return switch face {
		case Top: 0;
		case Bottom: 1;
		case North: 2;
		case South: 3;
		case East: 4;
		case West: 5;
	};
}
