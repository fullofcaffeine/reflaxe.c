package caxecraft.domain;

/** Deterministic Amanatides-Woo-style traversal of the finite voxel grid. */
final class VoxelRaycast {
	static inline final MAX_VISITS:Int = World.WIDTH + World.HEIGHT + World.DEPTH + 8;

	public static function trace(cells:WorldCells, originX:Float, originY:Float, originZ:Float, directionX:Float, directionY:Float, directionZ:Float,
			maximumDistance:Float):RaycastHit {
		var cellX = floorToInt(originX);
		var cellY = floorToInt(originY);
		var cellZ = floorToInt(originZ);
		if (maximumDistance < 0.0 || !World.contains(World.coord(cellX, cellY, cellZ)))
			return miss(cellX, cellY, cellZ, 0.0, 0);

		if (World.isSolid(World.query(cells, World.coord(cellX, cellY, cellZ))))
			return hit(cellX, cellY, cellZ, cellX, cellY, cellZ, 0, 0, 0, 0.0, 1);

		final stepX = directionX > 0.0 ? 1 : (directionX < 0.0 ? -1 : 0);
		final stepY = directionY > 0.0 ? 1 : (directionY < 0.0 ? -1 : 0);
		final stepZ = directionZ > 0.0 ? 1 : (directionZ < 0.0 ? -1 : 0);
		if (stepX == 0 && stepY == 0 && stepZ == 0)
			return miss(cellX, cellY, cellZ, 0.0, 1);

		final deltaX = stepX == 0 ? 0.0 : abs(1.0 / directionX);
		final deltaY = stepY == 0 ? 0.0 : abs(1.0 / directionY);
		final deltaZ = stepZ == 0 ? 0.0 : abs(1.0 / directionZ);
		var maximumX = stepX == 0 ? 0.0 : ((stepX > 0 ? cellX + 1 : cellX) - originX) / directionX;
		var maximumY = stepY == 0 ? 0.0 : ((stepY > 0 ? cellY + 1 : cellY) - originY) / directionY;
		var maximumZ = stepZ == 0 ? 0.0 : ((stepZ > 0 ? cellZ + 1 : cellZ) - originZ) / directionZ;
		var previousX = cellX;
		var previousY = cellY;
		var previousZ = cellZ;
		var normalX = 0;
		var normalY = 0;
		var normalZ = 0;
		var distance = 0.0;
		var visited = 1;

		while (visited < MAX_VISITS) {
			previousX = cellX;
			previousY = cellY;
			previousZ = cellZ;
			if (stepX != 0 && (stepY == 0 || maximumX <= maximumY) && (stepZ == 0 || maximumX <= maximumZ)) {
				cellX += stepX;
				distance = maximumX;
				maximumX += deltaX;
				normalX = -stepX;
				normalY = 0;
				normalZ = 0;
			} else if (stepY != 0 && (stepZ == 0 || maximumY <= maximumZ)) {
				cellY += stepY;
				distance = maximumY;
				maximumY += deltaY;
				normalX = 0;
				normalY = -stepY;
				normalZ = 0;
			} else {
				cellZ += stepZ;
				distance = maximumZ;
				maximumZ += deltaZ;
				normalX = 0;
				normalY = 0;
				normalZ = -stepZ;
			}
			visited++;
			if (distance > maximumDistance)
				return miss(previousX, previousY, previousZ, distance, visited);
			final coord = World.coord(cellX, cellY, cellZ);
			if (!World.contains(coord))
				return miss(previousX, previousY, previousZ, distance, visited);
			if (World.isSolid(World.query(cells, coord)))
				return hit(cellX, cellY, cellZ, previousX, previousY, previousZ, normalX, normalY, normalZ, distance, visited);
		}
		return miss(previousX, previousY, previousZ, distance, visited);
	}

	static function floorToInt(value:Float):Int {
		final truncated = Std.int(value);
		return value < truncated ? truncated - 1 : truncated;
	}

	static function abs(value:Float):Float {
		return value < 0.0 ? -value : value;
	}

	static function hit(cellX:Int, cellY:Int, cellZ:Int, previousX:Int, previousY:Int, previousZ:Int, normalX:Int, normalY:Int, normalZ:Int, distance:Float,
			visited:Int):RaycastHit {
		return {
			hit: true,
			cellX: cellX,
			cellY: cellY,
			cellZ: cellZ,
			previousX: previousX,
			previousY: previousY,
			previousZ: previousZ,
			normalX: normalX,
			normalY: normalY,
			normalZ: normalZ,
			distance: distance,
			visited: visited
		};
	}

	static function miss(previousX:Int, previousY:Int, previousZ:Int, distance:Float, visited:Int):RaycastHit {
		return {
			hit: false,
			cellX: 0,
			cellY: 0,
			cellZ: 0,
			previousX: previousX,
			previousY: previousY,
			previousZ: previousZ,
			normalX: 0,
			normalY: 0,
			normalZ: 0,
			distance: distance,
			visited: visited
		};
	}
}
