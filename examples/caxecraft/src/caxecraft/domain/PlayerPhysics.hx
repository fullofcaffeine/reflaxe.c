package caxecraft.domain;

/** Fixed-step first-person AABB movement independent of raylib and wall time. */
final class PlayerPhysics {
	public static inline final FIXED_SECONDS:Float = 0.05;
	static inline final HALF_WIDTH:Float = 0.30;
	static inline final HEIGHT:Float = 1.80;
	static inline final MOVE_SPEED:Float = 4.0;
	static inline final GRAVITY:Float = -18.0;
	static inline final JUMP_SPEED:Float = 6.5;
	static inline final MAX_SUBSTEP:Float = 0.20;
	static inline final EPSILON:Float = 0.0001;

	public static function player(x:Float, y:Float, z:Float):PlayerState {
		return {
			x: x,
			y: y,
			z: z,
			velocityX: 0.0,
			velocityY: 0.0,
			velocityZ: 0.0,
			grounded: false
		};
	}

	public static function input(moveX:Float, moveZ:Float, jump:Bool):StepInput {
		return {moveX: moveX, moveZ: moveZ, jump: jump};
	}

	/** True when placing one unit block at `coord` cannot overlap the player. */
	public static function canPlaceAt(state:PlayerState, coord:BlockCoord):Bool {
		final playerMinimumX = state.x - HALF_WIDTH;
		final playerMaximumX = state.x + HALF_WIDTH;
		final playerMinimumY = state.y;
		final playerMaximumY = state.y + HEIGHT;
		final playerMinimumZ = state.z - HALF_WIDTH;
		final playerMaximumZ = state.z + HALF_WIDTH;
		var separated = false;
		if (playerMaximumX <= coord.x)
			separated = true;
		if (playerMinimumX >= coord.x + 1)
			separated = true;
		if (playerMaximumY <= coord.y)
			separated = true;
		if (playerMinimumY >= coord.y + 1)
			separated = true;
		if (playerMaximumZ <= coord.z)
			separated = true;
		if (playerMinimumZ >= coord.z + 1)
			separated = true;
		return separated;
	}

	/** Lift an invalid spawn by whole blocks until its AABB is free. */
	public static function recoverSpawn(cells:WorldCells, state:PlayerState):PlayerState {
		var recoveredY = state.y;
		var attempts = 0;
		while (overlaps(cells, state.x, recoveredY, state.z) && attempts < World.HEIGHT) {
			recoveredY += 1.0;
			attempts++;
		}
		return {
			x: state.x,
			y: recoveredY,
			z: state.z,
			velocityX: state.velocityX,
			velocityY: 0.0,
			velocityZ: state.velocityZ,
			grounded: false
		};
	}

	/** Advance exactly one 50 ms simulation tick. */
	public static function step(cells:WorldCells, original:PlayerState, command:StepInput):PlayerState {
		var state = original;
		if (overlaps(cells, original.x, original.y, original.z))
			state = recoverSpawn(cells, original);
		final velocityX = command.moveX * MOVE_SPEED;
		final velocityZ = command.moveZ * MOVE_SPEED;
		var velocityY = command.jump && state.grounded ? JUMP_SPEED : state.velocityY + GRAVITY * FIXED_SECONDS;

		final movedX = moveAxis(cells, state.x, state.y, state.z, velocityX * FIXED_SECONDS, 0);
		final movedZ = moveAxis(cells, movedX.x, movedX.y, movedX.z, velocityZ * FIXED_SECONDS, 2);
		final movedY = moveAxis(cells, movedZ.x, movedZ.y, movedZ.z, velocityY * FIXED_SECONDS, 1);
		var grounded = movedY.blocked && velocityY <= 0.0;
		if (movedY.blocked)
			velocityY = 0.0;
		if (!grounded)
			grounded = overlaps(cells, movedY.x, movedY.y - 0.02, movedY.z);
		var finalVelocityX = velocityX;
		if (movedX.blocked)
			finalVelocityX = 0.0;
		var finalVelocityZ = velocityZ;
		if (movedZ.blocked)
			finalVelocityZ = 0.0;

		return {
			x: movedY.x,
			y: movedY.y,
			z: movedY.z,
			velocityX: finalVelocityX,
			velocityY: velocityY,
			velocityZ: finalVelocityZ,
			grounded: grounded
		};
	}

	static function moveAxis(cells:WorldCells, x:Float, y:Float, z:Float, delta:Float, axis:Int):AxisMove {
		final magnitude = abs(delta);
		if (magnitude == 0.0)
			return {
				x: x,
				y: y,
				z: z,
				blocked: false
			};
		var count = Std.int(magnitude / MAX_SUBSTEP);
		if (count * MAX_SUBSTEP < magnitude)
			count++;
		if (count < 1)
			count = 1;
		final increment = delta / count;
		var currentX = x;
		var currentY = y;
		var currentZ = z;
		var blocked = false;
		var stepIndex = 0;
		while (stepIndex < count) {
			final candidateX = axis == 0 ? currentX + increment : currentX;
			final candidateY = axis == 1 ? currentY + increment : currentY;
			final candidateZ = axis == 2 ? currentZ + increment : currentZ;
			if (overlaps(cells, candidateX, candidateY, candidateZ)) {
				blocked = true;
				break;
			}
			currentX = candidateX;
			currentY = candidateY;
			currentZ = candidateZ;
			stepIndex++;
		}
		return {
			x: currentX,
			y: currentY,
			z: currentZ,
			blocked: blocked
		};
	}

	static function overlaps(cells:WorldCells, x:Float, y:Float, z:Float):Bool {
		final minimumX = floorToInt(x - HALF_WIDTH + EPSILON);
		final maximumX = floorToInt(x + HALF_WIDTH - EPSILON);
		final minimumY = floorToInt(y + EPSILON);
		final maximumY = floorToInt(y + HEIGHT - EPSILON);
		final minimumZ = floorToInt(z - HALF_WIDTH + EPSILON);
		final maximumZ = floorToInt(z + HALF_WIDTH - EPSILON);
		var blockY = minimumY;
		while (blockY <= maximumY) {
			var blockZ = minimumZ;
			while (blockZ <= maximumZ) {
				var blockX = minimumX;
				while (blockX <= maximumX) {
					final coord = World.coord(blockX, blockY, blockZ);
					if (!World.contains(coord) || World.isSolid(World.query(cells, coord)))
						return true;
					blockX++;
				}
				blockZ++;
			}
			blockY++;
		}
		return false;
	}

	static function floorToInt(value:Float):Int {
		final truncated = Std.int(value);
		return value < truncated ? truncated - 1 : truncated;
	}

	static function abs(value:Float):Float {
		return value < 0.0 ? -value : value;
	}
}
