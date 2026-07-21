package caxecraft.domain;

/** Finite voxel storage, deterministic terrain, and edit rules. */
final class World {
	public static inline final WIDTH:Int = 32;
	public static inline final HEIGHT:Int = 16;
	public static inline final DEPTH:Int = 32;
	public static inline final VOLUME:Int = WIDTH * HEIGHT * DEPTH;

	public static function coord(x:Int, y:Int, z:Int):BlockCoord {
		return {x: x, y: y, z: z};
	}

	public static function contains(coord:BlockCoord):Bool {
		return coord.x >= 0 && coord.x < WIDTH && coord.y >= 0 && coord.y < HEIGHT && coord.z >= 0 && coord.z < DEPTH;
	}

	/**
		Return a checked linear element index, or `-1` outside the finite world.

		Bounds are checked before arithmetic. The admitted dimensions prove the
		largest intermediate (`16383`) fits in Haxe `Int`, so no overflow-prone
		untrusted coordinate participates in the multiplication.
	**/
	public static function indexOf(coord:BlockCoord):Int {
		if (!contains(coord))
			return -1;
		return coord.x + WIDTH * (coord.y + HEIGHT * coord.z);
	}

	public static function kindCode(kind:BlockKind):Int {
		return switch (kind) {
			case Air: 0;
			case Grass: 1;
			case Dirt: 2;
			case Stone: 3;
			case Bedrock: 4;
		};
	}

	public static function kindFromCode(code:Int):BlockKind {
		return switch (code) {
			case 1: Grass;
			case 2: Dirt;
			case 3: Stone;
			case 4: Bedrock;
			default: Air;
		};
	}

	public static function isSolid(kind:BlockKind):Bool {
		return switch (kind) {
			case Air: false;
			case Grass | Dirt | Stone | Bedrock: true;
		};
	}

	public static function isPlaceable(kind:BlockKind):Bool {
		return switch (kind) {
			case Grass | Dirt | Stone: true;
			case Air | Bedrock: false;
		};
	}

	/** Highest solid block in one column, or `-1` outside/above empty ground. */
	public static function surfaceY(cells:WorldCells, x:Int, z:Int):Int {
		if (x < 0 || x >= WIDTH || z < 0 || z >= DEPTH)
			return -1;
		var y = HEIGHT - 1;
		while (y >= 0) {
			if (isSolid(query(cells, coord(x, y, z))))
				return y;
			y--;
		}
		return -1;
	}

	public static function query(cells:WorldCells, coord:BlockCoord):BlockKind {
		final index = indexOf(coord);
		if (index < 0)
			return Air;
		return kindFromCode(WorldStorage.readCode(cells, index));
	}

	/** Direct checked replacement; higher-level remove/place rules build on it. */
	public static function replace(cells:WorldCells, coord:BlockCoord, kind:BlockKind):Bool {
		final index = indexOf(coord);
		if (index < 0)
			return false;
		WorldStorage.writeCode(cells, index, kindCode(kind));
		return true;
	}

	/** Air and immutable bedrock cannot be removed. */
	public static function remove(cells:WorldCells, coord:BlockCoord):Bool {
		final current = query(cells, coord);
		return switch (current) {
			case Air | Bedrock: false;
			case Grass | Dirt | Stone: replace(cells, coord, Air);
		};
	}

	/** Place only a placeable material into an in-bounds air cell. */
	public static function place(cells:WorldCells, coord:BlockCoord, kind:BlockKind):Bool {
		if (!isPlaceable(kind))
			return false;
		return switch (query(cells, coord)) {
			case Air: replace(cells, coord, kind);
			case Grass | Dirt | Stone | Bedrock: false;
		};
	}

	/** Reset and fill the finite world from a stable integer seed. */
	public static function generate(cells:WorldCells, seed:Int):Void {
		var index = 0;
		while (index < VOLUME) {
			WorldStorage.writeCode(cells, index, 0);
			index++;
		}

		var z = 0;
		while (z < DEPTH) {
			var x = 0;
			while (x < WIDTH) {
				final noise = terrainNoise(seed, x, z) & 0x7fffffff;
				final surface = 3 + noise % 5;
				var y = 0;
				while (y <= surface) {
					if (y == 0)
						replace(cells, coord(x, y, z), BlockKind.Bedrock);
					else if (y == surface)
						replace(cells, coord(x, y, z), BlockKind.Grass);
					else if (y + 2 >= surface)
						replace(cells, coord(x, y, z), BlockKind.Dirt);
					else
						replace(cells, coord(x, y, z), BlockKind.Stone);
					y++;
				}
				x++;
			}
			z++;
		}
	}

	/**
	 * Shape a small, level arrival meadow without changing the general seed.
	 *
	 * The first playable seconds need a safe place to learn movement, see Nia,
	 * and recognize the Mossling silhouette. Keeping this authored edit in the
	 * shared world layer makes Eval, C, future editors, and other renderers agree
	 * on the same blocks; the Raylib adapter does not own hidden level geometry.
	 */
	public static function prepareSpawnMeadow(cells:WorldCells):Void {
		var z = 10;
		while (z <= 21) {
			var x = 10;
			while (x <= 21) {
				var y = 0;
				while (y < HEIGHT) {
					var kind = BlockKind.Air;
					if (y == 0)
						kind = BlockKind.Bedrock;
					else if (y < 4)
						kind = BlockKind.Dirt;
					else if (y == 4)
						kind = BlockKind.Grass;
					replace(cells, coord(x, y, z), kind);
					y++;
				}
				x++;
			}
			z++;
		}
	}

	/** Stable world hash used by deterministic traces and edit regression tests. */
	public static function stateHash(cells:WorldCells):Int {
		var hash = -2128831035;
		var index = 0;
		while (index < VOLUME) {
			hash = (hash ^ WorldStorage.readCode(cells, index)) * 16777619;
			index++;
		}
		return hash;
	}

	static function terrainNoise(seed:Int, x:Int, z:Int):Int {
		var value = seed ^ (x * 374761393) ^ (z * 668265263);
		value = (value ^ (value >>> 13)) * 1274126177;
		return value ^ (value >>> 16);
	}
}
