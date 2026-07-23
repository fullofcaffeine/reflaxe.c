package caxecraft.app;

import caxecraft.app.TerrainAtlas.TerrainSheet;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.app.TerrainChunkLayout.CHUNK_COUNT;
import caxecraft.app.TerrainChunkLayout.CHUNK_DEPTH;
import caxecraft.app.TerrainChunkLayout.CHUNK_WIDTH;
import caxecraft.app.TerrainChunkLayout.CHUNKS_X;
import caxecraft.app.TerrainChunkLayout.CHUNKS_Z;
import caxecraft.app.TerrainChunkLayout.FACE_CAPACITY;
import caxecraft.app.TerrainChunkLayout.FACES_PER_CHUNK;
import caxecraft.app.TerrainChunkLayout.chunkFor;
import caxecraft.app.TerrainChunkLayout.chunkOriginX;
import caxecraft.app.TerrainChunkLayout.chunkOriginZ;
import caxecraft.app.TerrainChunkLayout.facePartitionStart;
import caxecraft.app.TerrainChunkLayout.packFace;
import caxecraft.domain.BlockCoord;
import caxecraft.domain.BlockKind;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
#if c
import c.CArray;
import c.IntConvert;
import c.UInt8;
#end

/** Summary of the cache work performed before one terrain frame. */
typedef TerrainChunkPreparation = {
	/** Number of dirty chunks rebuilt during this call. */
	final rebuiltChunks:Int;

	/** Exposed opaque faces now stored across every chunk. */
	final faces:Int;

	/** Visible solid voxels now stored across every chunk. */
	final visibleBlocks:Int;

	/** False only if an internal fixed-capacity proof was violated. */
	final valid:Bool;
}

/**
	Persistent, allocation-free exposed-face cache for the finite voxel world.

	A class is appropriate here because the same cache survives across frames and
	owns mutable derived state with one lifetime. Gameplay still owns the voxels;
	this object stores only renderer-facing coordinates plus a packed material and
	face code. A successful edit invalidates its chunk and any axial neighbor that
	shares the edited boundary. `prepare` rebuilds only those dirty partitions.

	The C build embeds four fixed byte arrays and small per-chunk counters directly
	in the owner. Eval uses ordinary arrays so the exact chunk and invalidation
	rules can run as an independent oracle. Haxe removes the inactive carrier at
	compile time; the cache algorithm is shared and the C frame loop allocates
	nothing.
**/
final class TerrainChunkCache {
	#if c
	final faceX:CArray<UInt8, TerrainFaceSlots> = CArray.zero(FACE_CAPACITY);
	final faceY:CArray<UInt8, TerrainFaceSlots> = CArray.zero(FACE_CAPACITY);
	final faceZ:CArray<UInt8, TerrainFaceSlots> = CArray.zero(FACE_CAPACITY);
	final packedFaces:CArray<UInt8, TerrainFaceSlots> = CArray.zero(FACE_CAPACITY);
	final faceCounts:CArray<Int, TerrainChunkSlots> = CArray.zero(CHUNK_COUNT);
	final baseVisible:CArray<Int, TerrainChunkSlots> = CArray.zero(CHUNK_COUNT);
	final adventureVisible:CArray<Int, TerrainChunkSlots> = CArray.zero(CHUNK_COUNT);
	final dirty:CArray<UInt8, TerrainChunkSlots> = CArray.zero(CHUNK_COUNT);
	#else
	final faceX:Array<Int> = [];
	final faceY:Array<Int> = [];
	final faceZ:Array<Int> = [];
	final packedFaces:Array<Int> = [];
	final faceCounts:Array<Int> = [];
	final baseVisible:Array<Int> = [];
	final adventureVisible:Array<Int> = [];
	final dirty:Array<Int> = [];
	#end

	/** Construct empty storage and require the first `prepare` to build all chunks. */
	public function new() {
		#if !c
		var face = 0;
		while (face < FACE_CAPACITY) {
			faceX.push(0);
			faceY.push(0);
			faceZ.push(0);
			packedFaces.push(0);
			face++;
		}
		var chunk = 0;
		while (chunk < CHUNK_COUNT) {
			faceCounts.push(0);
			baseVisible.push(0);
			adventureVisible.push(0);
			dirty.push(0);
			chunk++;
		}
		#end
		invalidateAll();
	}

	/** Mark every partition dirty after replacing or loading a complete world. */
	public function invalidateAll():Void {
		var chunk = 0;
		while (chunk < CHUNK_COUNT) {
			writeDirty(chunk, 1);
			chunk++;
		}
	}

	/**
		Invalidate the edited cell's partition and each shared axial boundary.

		A corner edit can dirty three chunks: its owner, one x neighbor, and one z
		neighbor. Diagonal chunks share no voxel face and therefore remain valid.
		The return value counts newly dirty chunks, which makes repeated edits and
		test expectations observable without exposing the storage arrays.
	**/
	public function invalidate(coord:BlockCoord):Int {
		final owner = chunkFor(coord);
		if (owner < 0)
			return 0;
		var changed = markDirty(owner);
		final chunkX = owner % CHUNKS_X;
		final chunkZ = Std.int(owner / CHUNKS_X);
		final localX = coord.x % CHUNK_WIDTH;
		final localZ = coord.z % CHUNK_DEPTH;
		if (localX == 0 && chunkX > 0)
			changed += markDirty(owner - 1);
		if (localX == CHUNK_WIDTH - 1 && chunkX + 1 < CHUNKS_X)
			changed += markDirty(owner + 1);
		if (localZ == 0 && chunkZ > 0)
			changed += markDirty(owner - CHUNKS_X);
		if (localZ == CHUNK_DEPTH - 1 && chunkZ + 1 < CHUNKS_Z)
			changed += markDirty(owner + CHUNKS_X);
		return changed;
	}

	/** Rebuild every dirty partition, then return complete renderer counters. */
	public function prepare(cells:WorldCells):TerrainChunkPreparation {
		var rebuilt = 0;
		var valid = true;
		var chunk = 0;
		while (chunk < CHUNK_COUNT) {
			if (readDirty(chunk) != 0) {
				if (!rebuild(cells, chunk))
					valid = false;
				writeDirty(chunk, 0);
				rebuilt++;
			}
			chunk++;
		}
		return {
			rebuiltChunks: rebuilt,
			faces: totalFaces(),
			visibleBlocks: visibleBlocks(TerrainSheet.Base) + visibleBlocks(TerrainSheet.Adventure),
			valid: valid
		};
	}

	/** Number of cached faces in one checked chunk. */
	public function chunkFaceCount(chunk:Int):Int {
		if (chunk < 0 || chunk >= CHUNK_COUNT)
			return 0;
		return faceCounts[chunk];
	}

	/** Total cached faces across all partitions. */
	public function totalFaces():Int {
		var total = 0;
		var chunk = 0;
		while (chunk < CHUNK_COUNT) {
			total += faceCounts[chunk];
			chunk++;
		}
		return total;
	}

	/** Visible solid-cell count for one texture sheet. */
	public function visibleBlocks(sheet:TerrainSheet):Int {
		var total = 0;
		var chunk = 0;
		while (chunk < CHUNK_COUNT) {
			switch sheet {
				case Base:
					total += baseVisible[chunk];
				case Adventure:
					total += adventureVisible[chunk];
			}
			chunk++;
		}
		return total;
	}

	/** First cache slot owned by one chunk. */
	public inline function partitionStart(chunk:Int):Int
		return facePartitionStart(chunk);

	/** Read one cached x coordinate after the caller checks its chunk count. */
	public inline function xAt(index:Int):Int
		return readFaceByte(faceX, index);

	/** Read one cached y coordinate after the caller checks its chunk count. */
	public inline function yAt(index:Int):Int
		return readFaceByte(faceY, index);

	/** Read one cached z coordinate after the caller checks its chunk count. */
	public inline function zAt(index:Int):Int
		return readFaceByte(faceZ, index);

	/** Read one cached material/face code after the caller checks its chunk count. */
	public inline function packedAt(index:Int):Int
		return readFaceByte(packedFaces, index);

	function rebuild(cells:WorldCells, chunk:Int):Bool {
		final originX = chunkOriginX(chunk);
		final originZ = chunkOriginZ(chunk);
		var count = 0;
		var visibleBase = 0;
		var visibleAdventure = 0;
		var valid = true;
		var z = originZ;
		while (z < originZ + CHUNK_DEPTH) {
			var y = 0;
			while (y < World.HEIGHT) {
				var x = originX;
				while (x < originX + CHUNK_WIDTH) {
					final kind = World.query(cells, World.coord(x, y, z));
					if (World.isSolid(kind)) {
						var blockVisible = false;
						var nextCount = appendIfExposed(cells, chunk, count, kind, VoxelFace.Top, x, y, z, x, y + 1, z);
						if (nextCount < 0) {
							valid = false;
							nextCount = count;
						}
						if (nextCount > count)
							blockVisible = true;
						count = nextCount;
						nextCount = appendIfExposed(cells, chunk, count, kind, VoxelFace.Bottom, x, y, z, x, y - 1, z);
						if (nextCount < 0) {
							valid = false;
							nextCount = count;
						}
						if (nextCount > count)
							blockVisible = true;
						count = nextCount;
						nextCount = appendIfExposed(cells, chunk, count, kind, VoxelFace.North, x, y, z, x, y, z - 1);
						if (nextCount < 0) {
							valid = false;
							nextCount = count;
						}
						if (nextCount > count)
							blockVisible = true;
						count = nextCount;
						nextCount = appendIfExposed(cells, chunk, count, kind, VoxelFace.South, x, y, z, x, y, z + 1);
						if (nextCount < 0) {
							valid = false;
							nextCount = count;
						}
						if (nextCount > count)
							blockVisible = true;
						count = nextCount;
						nextCount = appendIfExposed(cells, chunk, count, kind, VoxelFace.East, x, y, z, x + 1, y, z);
						if (nextCount < 0) {
							valid = false;
							nextCount = count;
						}
						if (nextCount > count)
							blockVisible = true;
						count = nextCount;
						nextCount = appendIfExposed(cells, chunk, count, kind, VoxelFace.West, x, y, z, x - 1, y, z);
						if (nextCount < 0) {
							valid = false;
							nextCount = count;
						}
						if (nextCount > count)
							blockVisible = true;
						count = nextCount;
						if (blockVisible) {
							switch TerrainAtlas.sheet(kind) {
								case Base:
									visibleBase++;
								case Adventure:
									visibleAdventure++;
							}
						}
					}
					x++;
				}
				y++;
			}
			z++;
		}
		if (!valid)
			return invalidateBrokenChunk(chunk);
		faceCounts[chunk] = count;
		baseVisible[chunk] = visibleBase;
		adventureVisible[chunk] = visibleAdventure;
		return true;
	}

	function appendIfExposed(cells:WorldCells, chunk:Int, count:Int, kind:BlockKind, face:VoxelFace, x:Int, y:Int, z:Int, neighborX:Int, neighborY:Int,
			neighborZ:Int):Int {
		if (World.isSolid(World.query(cells, World.coord(neighborX, neighborY, neighborZ))))
			return count;
		if (count >= FACES_PER_CHUNK)
			return -1;
		final index = facePartitionStart(chunk) + count;
		writeFaceByte(faceX, index, x);
		writeFaceByte(faceY, index, y);
		writeFaceByte(faceZ, index, z);
		writeFaceByte(packedFaces, index, packFace(kind, face));
		return count + 1;
	}

	function invalidateBrokenChunk(chunk:Int):Bool {
		faceCounts[chunk] = 0;
		baseVisible[chunk] = 0;
		adventureVisible[chunk] = 0;
		return false;
	}

	inline function markDirty(chunk:Int):Int {
		if (readDirty(chunk) != 0)
			return 0;
		writeDirty(chunk, 1);
		return 1;
	}

	inline function readDirty(chunk:Int):Int {
		#if c
		return IntConvert.exact(dirty[chunk]);
		#else
		return dirty[chunk];
		#end
	}

	inline function writeDirty(chunk:Int, value:Int):Void {
		#if c
		dirty[chunk] = IntConvert.modulo(value);
		#else
		dirty[chunk] = value;
		#end
	}

	#if c
	inline function readFaceByte(storage:CArray<UInt8, TerrainFaceSlots>, index:Int):Int
		return IntConvert.exact(storage[index]);

	inline function writeFaceByte(storage:CArray<UInt8, TerrainFaceSlots>, index:Int, value:Int):Void
		storage[index] = IntConvert.modulo(value);
	#else
	inline function readFaceByte(storage:Array<Int>, index:Int):Int
		return storage[index];

	inline function writeFaceByte(storage:Array<Int>, index:Int, value:Int):Void
		storage[index] = value;
	#end
}
