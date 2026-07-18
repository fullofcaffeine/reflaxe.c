package caxecraft.qa;

import caxecraft.domain.BlockKind;
import caxecraft.domain.CaxecraftTrace;
import caxecraft.domain.PlayerPhysics;
import caxecraft.domain.VoxelRaycast;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import caxecraft.domain.WorldVolume;
#if c
import c.CArray;
import c.UInt8;
#end

/** Shared Eval/generated-C executable specification for the voxel domain. */
final class DomainProbe {
	public static function main():Void {
		#if !c
		final result = selfCheck();
		final trace = CaxecraftTrace.runTrace();
		Sys.println(result);
		Sys.println(CaxecraftTrace.terrainTrace());
		Sys.println(CaxecraftTrace.editTrace());
		Sys.println(CaxecraftTrace.rayTrace());
		Sys.println(CaxecraftTrace.collisionTrace());
		Sys.println(trace);
		var seed = 0;
		while (seed < 32) {
			Sys.println(CaxecraftTrace.propertyTrace(seed));
			seed++;
		}
		#else
		selfCheck();
		CaxecraftTrace.runTrace();
		#end
	}

	/** Return zero on success, or a stable case number for the first failure. */
	public static function selfCheck():Int {
		#if c
		var storage:CArray<UInt8, WorldVolume> = CArray.zero(World.VOLUME);
		var cells:WorldCells = storage.span();
		#else
		var cells:WorldCells = [];
		var fill = 0;
		while (fill < World.VOLUME) {
			cells.push(0);
			fill++;
		}
		#end

		if (World.indexOf(World.coord(0, 0, 0)) != 0)
			return 1;
		if (World.indexOf(World.coord(31, 15, 31)) != World.VOLUME - 1)
			return 2;
		if (World.indexOf(World.coord(-2147483647, 15, 31)) != -1)
			return 3;
		if (World.replace(cells, World.coord(32, 0, 0), BlockKind.Stone))
			return 4;
		if (!World.replace(cells, World.coord(8, 8, 8), BlockKind.Stone))
			return 5;

		final axis = VoxelRaycast.trace(cells, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 12.0);
		if (!axis.hit || axis.cellX != 8 || axis.cellY != 8 || axis.cellZ != 8 || axis.previousX != 7 || axis.normalX != -1 || !near(axis.distance, 5.5)
			|| axis.visited != 7)
			return 10;

		final negative = VoxelRaycast.trace(cells, 12.5, 8.5, 8.5, -1.0, 0.0, 0.0, 12.0);
		if (!negative.hit || negative.previousX != 9 || negative.normalX != 1 || !near(negative.distance, 3.5))
			return 11;

		World.replace(cells, World.coord(8, 8, 8), BlockKind.Air);
		World.replace(cells, World.coord(10, 10, 10), BlockKind.Stone);
		final diagonal = VoxelRaycast.trace(cells, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
		if (!diagonal.hit || diagonal.cellX != 10 || diagonal.cellY != 10 || diagonal.cellZ != 10 || diagonal.visited != 10)
			return 12;
		World.replace(cells, World.coord(8, 7, 7), BlockKind.Stone);
		final tieX = VoxelRaycast.trace(cells, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
		if (!tieX.hit || tieX.cellX != 8 || tieX.cellY != 7 || tieX.cellZ != 7 || tieX.normalX != -1 || tieX.visited != 2)
			return 18;
		World.replace(cells, World.coord(8, 7, 7), BlockKind.Air);
		World.replace(cells, World.coord(8, 8, 7), BlockKind.Stone);
		final tieY = VoxelRaycast.trace(cells, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
		if (!tieY.hit || tieY.cellX != 8 || tieY.cellY != 8 || tieY.cellZ != 7 || tieY.normalY != -1 || tieY.visited != 3)
			return 19;
		World.replace(cells, World.coord(8, 8, 7), BlockKind.Air);
		World.replace(cells, World.coord(8, 8, 8), BlockKind.Stone);
		final tieZ = VoxelRaycast.trace(cells, 7.5, 7.5, 7.5, 1.0, 1.0, 1.0, 12.0);
		if (!tieZ.hit || tieZ.cellX != 8 || tieZ.cellY != 8 || tieZ.cellZ != 8 || tieZ.normalZ != -1 || tieZ.visited != 4)
			return 23;

		final zeroDirection = VoxelRaycast.trace(cells, 1.5, 1.5, 1.5, 0.0, 0.0, 0.0, 12.0);
		if (zeroDirection.hit || zeroDirection.visited != 1)
			return 13;

		final exactBoundary = VoxelRaycast.trace(cells, 9.0, 8.5, 8.5, -1.0, 0.0, 0.0, 2.0);
		if (!exactBoundary.hit || exactBoundary.cellX != 8 || !near(exactBoundary.distance, 0.0))
			return 14;

		final inside = VoxelRaycast.trace(cells, 8.5, 8.5, 8.5, 0.0, 1.0, 0.0, 2.0);
		if (!inside.hit || inside.previousX != 8 || !near(inside.distance, 0.0) || inside.visited != 1)
			return 15;

		final tooShort = VoxelRaycast.trace(cells, 2.5, 8.5, 8.5, 1.0, 0.0, 0.0, 5.49);
		if (tooShort.hit || tooShort.previousX != 7)
			return 16;

		final worldEdge = VoxelRaycast.trace(cells, 30.5, 14.5, 30.5, 1.0, 0.0, 0.0, 8.0);
		if (worldEdge.hit || worldEdge.previousX != 31 || !near(worldEdge.distance, 1.5))
			return 17;

		if (!World.remove(cells, World.coord(8, 8, 8)) || World.remove(cells, World.coord(8, 8, 8)))
			return 20;
		if (!World.place(cells, World.coord(8, 8, 8), BlockKind.Dirt) || World.place(cells, World.coord(8, 8, 8), BlockKind.Stone))
			return 21;
		World.replace(cells, World.coord(8, 8, 8), BlockKind.Bedrock);
		if (World.remove(cells, World.coord(8, 8, 8)))
			return 22;

		clear(cells);
		CaxecraftTrace.makeFloor(cells);
		var player = PlayerPhysics.player(5.5, 1.0, 5.5);
		player = PlayerPhysics.step(cells, player, PlayerPhysics.input(0.0, 0.0, false));
		if (!player.grounded || !near(player.y, 1.0) || !near(player.velocityY, 0.0))
			return 30;

		player = PlayerPhysics.step(cells, player, PlayerPhysics.input(0.0, 0.0, true));
		if (player.grounded || player.velocityY <= 0.0 || player.y <= 1.0)
			return 31;

		World.replace(cells, World.coord(6, 1, 5), BlockKind.Stone);
		World.replace(cells, World.coord(6, 2, 5), BlockKind.Stone);
		var slider = PlayerPhysics.player(5.7, 1.0, 5.2);
		slider = PlayerPhysics.step(cells, slider, PlayerPhysics.input(1.0, 1.0, false));
		if (!near(slider.x, 5.7) || slider.z <= 5.2 || !near(slider.velocityX, 0.0) || slider.velocityZ <= 0.0)
			return 32;

		World.replace(cells, World.coord(5, 1, 6), BlockKind.Stone);
		World.replace(cells, World.coord(5, 2, 6), BlockKind.Stone);
		var corner = PlayerPhysics.player(5.7, 1.0, 5.7);
		corner = PlayerPhysics.step(cells, corner, PlayerPhysics.input(1.0, 1.0, false));
		if (!near(corner.x, 5.7) || !near(corner.z, 5.7) || !near(corner.velocityX, 0.0) || !near(corner.velocityZ, 0.0))
			return 38;

		World.replace(cells, World.coord(4, 3, 4), BlockKind.Stone);
		var ceiling = PlayerPhysics.player(4.5, 1.0, 4.5);
		ceiling = PlayerPhysics.step(cells, ceiling, PlayerPhysics.input(0.0, 0.0, false));
		ceiling = PlayerPhysics.step(cells, ceiling, PlayerPhysics.input(0.0, 0.0, true));
		var ceilingTicks = 0;
		while (ceilingTicks < 4) {
			ceiling = PlayerPhysics.step(cells, ceiling, PlayerPhysics.input(0.0, 0.0, false));
			ceilingTicks++;
		}
		if (ceiling.y + 1.8 > 3.0001 || ceiling.velocityY > 0.0)
			return 33;

		var embedded = PlayerPhysics.player(6.5, 1.0, 5.5);
		embedded = PlayerPhysics.recoverSpawn(cells, embedded);
		if (embedded.y < 3.0)
			return 34;

		var boundary = PlayerPhysics.player(0.31, 1.0, 0.31);
		boundary = PlayerPhysics.step(cells, boundary, PlayerPhysics.input(-1.0, -1.0, false));
		if (boundary.x < 0.29 || boundary.z < 0.29)
			return 35;

		var tunnel = PlayerPhysics.player(10.5, 1.0, 10.5);
		World.replace(cells, World.coord(11, 1, 10), BlockKind.Stone);
		World.replace(cells, World.coord(11, 2, 10), BlockKind.Stone);
		tunnel = PlayerPhysics.step(cells, tunnel, PlayerPhysics.input(20.0, 0.0, false));
		if (tunnel.x > 10.701)
			return 36;

		return 0;
	}

	static function clear(cells:WorldCells):Void {
		var index = 0;
		while (index < World.VOLUME) {
			World.replace(cells, World.coord(index & 31, (index >>> 5) & 15, index >>> 9), BlockKind.Air);
			index++;
		}
	}

	static function near(left:Float, right:Float):Bool {
		final difference = left < right ? right - left : left - right;
		return difference < 0.00001;
	}
}
