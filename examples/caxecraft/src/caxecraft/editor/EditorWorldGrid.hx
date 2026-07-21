package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioLimits;
import caxecraft.scenario.ScenarioWorld;
import caxecraft.scenario.ScenarioWorld.VoxelChunk;
import caxecraft.scenario.ScenarioWorld.VoxelRun;

/** Internal result used while one command edits voxel storage. */
@:noCompletion
enum EditorWorldResult {
	WorldReady(world:ScenarioWorld);
	WorldRejected(error:EditorError);
}

/**
	Turns the chunked file representation into a temporary flat editing grid.

	The grid exists only during one command. Paint and fill preserve the current
	chunk identities; resize creates deterministic chunks no larger than the
	CAXEMAP limit. The saved draft always returns to the public chunked model.

	`@:noCompletion` hides this implementation helper from ordinary suggestions;
	UI code should send commands through `EditorSession` instead.
**/
@:noCompletion
final class EditorWorldGrid {
	public static function resize(world:ScenarioWorld, nextSize:VoxelSize):EditorWorldResult {
		if (!validSize(nextSize))
			return WorldRejected(InvalidWorldSize(nextSize));
		final decoded = decode(world);
		if (decoded == null)
			return WorldRejected(DraftWorldIsNotEditable);
		final cells = [for (_ in 0...volume(nextSize)) 0];
		final copyWidth = world.size.width < nextSize.width ? world.size.width : nextSize.width;
		final copyHeight = world.size.height < nextSize.height ? world.size.height : nextSize.height;
		final copyDepth = world.size.depth < nextSize.depth ? world.size.depth : nextSize.depth;
		for (z in 0...copyDepth)
			for (y in 0...copyHeight)
				for (x in 0...copyWidth)
					cells[index(nextSize, x, y, z)] = decoded[index(world.size, x, y, z)];
		return WorldReady({size: nextSize, palette: world.palette.copy(), chunks: buildChunks(nextSize, cells)});
	}

	public static function paint(world:ScenarioWorld, point:VoxelPoint, paletteCode:Int):EditorWorldResult {
		return paintMany(world, [point], paletteCode);
	}

	/** Decode and rewrite one chunk set for an entire UI paint gesture. */
	public static function paintMany(world:ScenarioWorld, points:Array<VoxelPoint>, paletteCode:Int):EditorWorldResult {
		for (point in points)
			if (!containsPoint(world.size, point))
				return WorldRejected(PointOutsideWorld(point));
		final cells = decode(world);
		if (cells == null)
			return WorldRejected(DraftWorldIsNotEditable);
		for (point in points)
			cells[index(world.size, point.x, point.y, point.z)] = paletteCode;
		return WorldReady({size: world.size, palette: world.palette.copy(), chunks: rewriteChunks(world.chunks, world.size, cells)});
	}

	public static function fill(world:ScenarioWorld, bounds:VoxelBounds, paletteCode:Int):EditorWorldResult {
		if (!containsBounds(world.size, bounds))
			return WorldRejected(BoundsOutsideWorld(bounds));
		final cells = decode(world);
		if (cells == null)
			return WorldRejected(DraftWorldIsNotEditable);
		for (z in bounds.origin.z...bounds.origin.z + bounds.size.depth)
			for (y in bounds.origin.y...bounds.origin.y + bounds.size.height)
				for (x in bounds.origin.x...bounds.origin.x + bounds.size.width)
					cells[index(world.size, x, y, z)] = paletteCode;
		return WorldReady({size: world.size, palette: world.palette.copy(), chunks: rewriteChunks(world.chunks, world.size, cells)});
	}

	public static function containsPoint(size:VoxelSize, point:VoxelPoint):Bool
		return point.x >= 0 && point.y >= 0 && point.z >= 0 && point.x < size.width && point.y < size.height && point.z < size.depth;

	public static function containsBounds(size:VoxelSize, bounds:VoxelBounds):Bool
		return bounds.origin.x >= 0
			&& bounds.origin.y >= 0
			&& bounds.origin.z >= 0
			&& bounds.size.width > 0
			&& bounds.size.height > 0
			&& bounds.size.depth > 0
			&& bounds.size.width <= size.width
			&& bounds.size.height <= size.height
			&& bounds.size.depth <= size.depth
			&& bounds.origin.x <= size.width - bounds.size.width
			&& bounds.origin.y <= size.height - bounds.size.height
			&& bounds.origin.z <= size.depth - bounds.size.depth;

	public static inline function volume(size:VoxelSize):Int
		return size.width * size.height * size.depth;

	static function validSize(size:VoxelSize):Bool {
		if (size.width <= 0
			|| size.width > ScenarioLimits.MAX_WORLD_WIDTH
			|| size.height <= 0
			|| size.height > ScenarioLimits.MAX_WORLD_HEIGHT
			|| size.depth <= 0
			|| size.depth > ScenarioLimits.MAX_WORLD_DEPTH)
			return false;
		final cells = volume(size);
		return cells > 0 && cells <= ScenarioLimits.MAX_WORLD_CELLS;
	}

	static function decode(world:ScenarioWorld):Null<Array<Int>> {
		if (!validSize(world.size))
			return null;
		final cells = [for (_ in 0...volume(world.size)) -1];
		for (chunk in world.chunks) {
			if (!containsBounds(world.size, {origin: chunk.origin, size: chunk.size}))
				return null;
			final expected = volume(chunk.size);
			var localIndex = 0;
			for (run in chunk.runs) {
				if (run.count <= 0 || localIndex > expected - run.count)
					return null;
				for (_ in 0...run.count) {
					final localX = localIndex % chunk.size.width;
					final layer = Std.int(localIndex / chunk.size.width);
					final localY = layer % chunk.size.height;
					final localZ = Std.int(layer / chunk.size.height);
					final target = index(world.size, chunk.origin.x + localX, chunk.origin.y + localY, chunk.origin.z + localZ);
					if (cells[target] != -1)
						return null;
					cells[target] = run.paletteCode;
					localIndex++;
				}
			}
			if (localIndex != expected)
				return null;
		}
		for (cell in cells)
			if (cell < 0)
				return null;
		return cells;
	}

	static function rewriteChunks(chunks:Array<VoxelChunk>, worldSize:VoxelSize, cells:Array<Int>):Array<VoxelChunk> {
		final result:Array<VoxelChunk> = [];
		for (chunk in chunks) {
			final values:Array<Int> = [];
			for (z in chunk.origin.z...chunk.origin.z + chunk.size.depth)
				for (y in chunk.origin.y...chunk.origin.y + chunk.size.height)
					for (x in chunk.origin.x...chunk.origin.x + chunk.size.width)
						values.push(cells[index(worldSize, x, y, z)]);
			result.push({
				id: chunk.id,
				origin: chunk.origin,
				size: chunk.size,
				runs: runs(values)
			});
		}
		return result;
	}

	static function buildChunks(size:VoxelSize, cells:Array<Int>):Array<VoxelChunk> {
		final result:Array<VoxelChunk> = [];
		var z = 0;
		while (z < size.depth) {
			var y = 0;
			while (y < size.height) {
				var x = 0;
				while (x < size.width) {
					final chunkSize:VoxelSize = {
						width: remaining(size.width, x),
						height: remaining(size.height, y),
						depth: remaining(size.depth, z)
					};
					final values:Array<Int> = [];
					for (localZ in 0...chunkSize.depth)
						for (localY in 0...chunkSize.height)
							for (localX in 0...chunkSize.width)
								values.push(cells[index(size, x + localX, y + localY, z + localZ)]);
					result.push({
						id: new ScenarioId('editor.chunk.x$x.y$y.z$z'),
						origin: {x: x, y: y, z: z},
						size: chunkSize,
						runs: runs(values)
					});
					x += ScenarioLimits.MAX_CHUNK_EDGE;
				}
				y += ScenarioLimits.MAX_CHUNK_EDGE;
			}
			z += ScenarioLimits.MAX_CHUNK_EDGE;
		}
		return result;
	}

	static inline function remaining(edge:Int, origin:Int):Int {
		final available = edge - origin;
		return available < ScenarioLimits.MAX_CHUNK_EDGE ? available : ScenarioLimits.MAX_CHUNK_EDGE;
	}

	static function runs(values:Array<Int>):Array<VoxelRun> {
		final result:Array<VoxelRun> = [];
		if (values.length == 0)
			return result;
		var code = values[0];
		var count = 1;
		for (entry in values.slice(1)) {
			if (entry == code) {
				count++;
			} else {
				result.push({paletteCode: code, count: count});
				code = entry;
				count = 1;
			}
		}
		result.push({paletteCode: code, count: count});
		return result;
	}

	static inline function index(size:VoxelSize, x:Int, y:Int, z:Int):Int
		return (z * size.height + y) * size.width + x;
}
