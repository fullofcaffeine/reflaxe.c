package caxecraft.domain;

import caxecraft.domain.CharacterPhysics.input as playerInput;
import caxecraft.domain.CharacterPhysics.body as createPlayer;
import caxecraft.domain.CharacterPhysics.recoverSpawn as recoverPlayerSpawn;
import caxecraft.domain.CharacterPhysics.step as stepPlayer;
#if c
// C-only fixed storage imports are absent from the non-C oracle program.
import c.CArray;
import c.UInt8;
#end

/**
	Canonical deterministic traces consumed by Eval and strict generated C.

	Each `#if c` block below changes only how a fresh world buffer is provided:
	the C build uses a fixed stack-shaped `CArray`, while the non-C oracle uses an
	ordinary Haxe array. The trace operations and hashes after that setup are the
	same source on both paths. Haxe removes the inactive branch at compile time;
	there is no target test in the generated game loop.

	The repeated setup is intentionally visible evidence of the representation
	boundary. Future target adapters should centralize their carrier construction
	without moving terrain, edit, ray, or physics semantics behind conditionals.
**/
final class CaxecraftTrace {
	public static function terrainTrace():Int {
		#if c
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = storage.span();
		#else
		var cells:WorldCells = [];
		for (index in 0...World.VOLUME)
			cells.push(0);
		#end
		World.generate(cells, 0x13579bdf);
		return World.stateHash(cells);
	}

	public static function editTrace():Int {
		#if c
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = storage.span();
		#else
		var cells:WorldCells = [];
		for (index in 0...World.VOLUME)
			cells.push(0);
		#end
		World.generate(cells, 0x13579bdf);
		final target = World.coord(4, 10, 4);
		final base = World.coord(4, 9, 4);
		World.remove(cells, base);
		World.place(cells, target, BlockKind.Stone);
		World.place(cells, target, BlockKind.Dirt);
		return mix(World.stateHash(cells), World.kindCode(World.query(cells, target)));
	}

	public static function rayTrace():Int {
		#if c
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = storage.span();
		#else
		var cells:WorldCells = [];
		for (index in 0...World.VOLUME)
			cells.push(0);
		#end
		World.replace(cells, World.coord(8, 8, 8), BlockKind.Stone);
		final hit = VoxelRaycast.trace(cells, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 12.0);
		var hash = hit.hit ? 1 : 0;
		hash = mix(hash, hit.cellX);
		hash = mix(hash, hit.previousX);
		hash = mix(hash, hit.normalX);
		hash = mix(hash, Std.int(hit.distance * 1000.0));
		return mix(hash, hit.visited);
	}

	public static function collisionTrace():Int {
		#if c
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = storage.span();
		#else
		var cells:WorldCells = [];
		for (index in 0...World.VOLUME)
			cells.push(0);
		#end
		makeFloor(cells);
		var state = createPlayer(5.5, 1.0, 5.5);
		state = stepPlayer(cells, state, playerInput(1.0, 0.5, false));
		state = stepPlayer(cells, state, playerInput(0.0, 0.0, true));
		var hash = Std.int(state.x * 1000.0);
		hash = mix(hash, Std.int(state.y * 1000.0));
		hash = mix(hash, Std.int(state.z * 1000.0));
		hash = mix(hash, Std.int(state.velocityY * 1000.0));
		var groundedCode = 0;
		if (state.grounded)
			groundedCode = 1;
		return mix(hash, groundedCode);
	}

	/** Property-style deterministic seed used by the sanitizer and oracle corpus. */
	public static function propertyTrace(seed:Int):Int {
		#if c
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = storage.span();
		#else
		var cells:WorldCells = [];
		for (index in 0...World.VOLUME)
			cells.push(0);
		#end
		World.generate(cells, seed * 1103515245 + 12345);
		final x = (seed * 17 + 5) & 31;
		final z = (seed * 29 + 11) & 31;
		final edit = World.coord(x, 14, z);
		if ((seed & 1) == 0)
			World.place(cells, edit, BlockKind.Dirt);
		else
			World.place(cells, edit, BlockKind.Stone);
		final ray = VoxelRaycast.trace(cells, x + 0.5, 15.5, z + 0.5, 0.25, -1.0, 0.5, 24.0);
		final moveX = (seed & 1) == 0 ? 1.0 : -1.0;
		final moveZ = (seed & 2) == 0 ? 0.5 : -0.5;
		final jump = (seed & 4) != 0;
		var state = createPlayer(1.5 + (seed & 3), 14.0, 1.5 + ((seed >>> 2) & 3));
		state = recoverPlayerSpawn(cells, state);
		state = stepPlayer(cells, state, playerInput(moveX, moveZ, jump));
		var hash = World.stateHash(cells);
		var hitCode = 0;
		if (ray.hit)
			hitCode = 1;
		hash = mix(hash, hitCode);
		hash = mix(hash, ray.cellX);
		hash = mix(hash, ray.cellY);
		hash = mix(hash, ray.cellZ);
		hash = mix(hash, ray.visited);
		hash = mix(hash, Std.int(state.x * 1000.0));
		hash = mix(hash, Std.int(state.y * 1000.0));
		return mix(hash, Std.int(state.z * 1000.0));
	}

	public static function runTrace():Int {
		var hash = terrainTrace();
		hash = mix(hash, editTrace());
		hash = mix(hash, rayTrace());
		hash = mix(hash, collisionTrace());
		var seed = 0;
		while (seed < 32) {
			hash = mix(hash, propertyTrace(seed));
			seed++;
		}
		return hash;
	}

	public static function makeFloor(cells:WorldCells):Void {
		var z = 0;
		while (z < World.DEPTH) {
			var x = 0;
			while (x < World.WIDTH) {
				World.replace(cells, World.coord(x, 0, z), BlockKind.Stone);
				x++;
			}
			z++;
		}
	}

	public static function mix(hash:Int, value:Int):Int {
		return (hash ^ value) * 16777619;
	}
}
