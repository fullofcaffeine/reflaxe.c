package caxecraft.app;

import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;

/**
 * Owns the camera direction selected when a player enters or revives in a level.
 *
 * Keeping this conversion target-neutral lets focused tests compare authored
 * CAXEMAP orientation with the same values used by movement and rendering.
 */
typedef SpawnCameraDirection = {
	final x:Float;
	final y:Float;
	final z:Float;
}

/**
 * Derive the initial camera direction from one validated player spawn.
 *
 * CAXEMAP yaw rotates clockwise from negative Z. The conversion applies one
 * degree at a time because this admitted haxe.c slice does not yet depend on
 * trigonometric standard-library lowering. The loop runs only when a level is
 * entered or revived, and validation bounds it to at most 359 iterations.
 */
function headingForSpawn(transform:ScenarioTransform):SpawnCameraDirection {
	var x = 0.0;
	var z = -1.0;
	var remaining = transform.yawDegrees;
	while (remaining > 0) {
		final nextX = x * 0.9998476952 - z * 0.0174524064;
		final nextZ = x * 0.0174524064 + z * 0.9998476952;
		x = nextX;
		z = nextZ;
		remaining--;
	}
	return {x: x, y: -0.18, z: z};
}
