package caxecraft.editor;

import caxecraft.editor.EditorWorldGrid.decode as decodeWorld;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioWorld;

/**
 * Describes the authored voxel volume and camera math without depending on a
 * renderer.
 *
 * The CAXEMAP draft remains the only editable world. `projectWorld` creates a
 * read-only presentation cache after an accepted edit, camera functions turn
 * bounded input into a fresh snapshot, and `pickWorld` maps a viewing ray back
 * to one authored coordinate. Eval tests and the native Raylib editor therefore
 * share spatial rules without either implementation copying game state.
 */
/** One read-only copy of the complete finite voxel volume. */
typedef EditorWorldProjection = {
	final width:Int;
	final height:Int;
	final depth:Int;
	final cells:Array<Int>;
}

/**
 * The editor camera's position and unit-like forward direction.
 *
 * This is a value snapshot rather than a class because it has no independent
 * identity or resource lifetime. `CaxecraftEditorScreen` owns the current
 * snapshot and replaces it after each input step.
 */
typedef EditorCameraState = {
	final x:Float;
	final y:Float;
	final z:Float;
	final lookX:Float;
	final lookY:Float;
	final lookZ:Float;
}

/** Renderer-neutral movement and look input for one displayed frame. */
typedef EditorCameraInput = {
	final forward:Float;
	final right:Float;
	final vertical:Float;
	final yaw:Float;
	final pitch:Float;
	final wheel:Float;
}

/** One world-space point or direction supplied by a renderer adapter. */
typedef EditorWorldVector = {
	final x:Float;
	final y:Float;
	final z:Float;
}

/** The nearest editable coordinate selected by a viewing ray. */
typedef EditorWorldHit = {
	final point:VoxelPoint;
	final distance:Float;
	final solid:Bool;
}

final CAMERA_SPEED = 8.0;
final WHEEL_DISTANCE = 2.0;
final MAX_FRAME_SECONDS = 0.1;
final MAX_LOOK_STEP = 0.25;
final MIN_PITCH = -0.90;
final MAX_PITCH = 0.90;
final RAY_EPSILON = 0.000001;

/**
 * Decode the whole finite draft once for repeated 3D drawing and picking.
 *
 * The returned array preserves CAXEMAP's `(z * height + y) * width + x`
 * ordering. A malformed chunk layout returns `null`; this function never
 * repairs or partially projects an invalid world.
 */
function projectWorld(world:ScenarioWorld):Null<EditorWorldProjection> {
	final cells = decodeWorld(world);
	if (cells == null)
		return null;
	return {
		width: world.size.width,
		height: world.size.height,
		depth: world.size.depth,
		cells: cells
	};
}

/** Return one palette code, or `-1` when the coordinate is outside the draft. */
function paletteCodeAtWorld(projection:EditorWorldProjection, x:Int, y:Int, z:Int):Int {
	if (x < 0 || y < 0 || z < 0 || x >= projection.width || y >= projection.height || z >= projection.depth)
		return -1;
	return projection.cells[(z * projection.height + y) * projection.width + x];
}

/**
 * Place a perspective camera above and south of the complete draft.
 *
 * Focusing depends only on finite world dimensions, so New World, resize, and
 * the `F` shortcut converge on the same deterministic view. The direction is a
 * reviewed unit-like constant and does not require target-specific trigonometry.
 */
function focusCamera(projection:EditorWorldProjection):EditorCameraState {
	final extent = projection.width > projection.depth ? projection.width : projection.depth;
	return {
		x: projection.width * 0.5,
		y: projection.height + extent * 0.4 + 2.0,
		z: projection.depth + extent * 0.4 + 1.0,
		lookX: 0.0,
		lookY: -0.5,
		lookZ: -0.8660254037844386
	};
}

/**
 * Advance the fly camera by one bounded displayed-frame input.
 *
 * Forward movement follows the view direction; strafe stays horizontal; the
 * vertical axis is explicit. Yaw uses the same small-angle, normalize-after
 * update as the playable camera, avoiding a target-only math dependency.
 * Position clamps leave generous space around the finite draft while
 * preventing one stalled frame or extreme wheel event from losing the camera.
 */
function stepCamera(projection:EditorWorldProjection, state:EditorCameraState, input:EditorCameraInput, frameSeconds:Float):EditorCameraState {
	var seconds = frameSeconds;
	if (seconds < 0.0)
		seconds = 0.0;
	if (seconds > MAX_FRAME_SECONDS)
		seconds = MAX_FRAME_SECONDS;

	var yaw = input.yaw;
	if (yaw > MAX_LOOK_STEP)
		yaw = MAX_LOOK_STEP;
	if (yaw < -MAX_LOOK_STEP)
		yaw = -MAX_LOOK_STEP;
	final candidateX = state.lookX + yaw * state.lookZ;
	final candidateZ = state.lookZ - yaw * state.lookX;
	final lengthSquared = candidateX * candidateX + candidateZ * candidateZ;
	final normalization = 1.5 - 0.5 * lengthSquared;
	final lookX = candidateX * normalization;
	final lookZ = candidateZ * normalization;
	var lookY = state.lookY + input.pitch;
	if (lookY > MAX_PITCH)
		lookY = MAX_PITCH;
	if (lookY < MIN_PITCH)
		lookY = MIN_PITCH;

	final distance = CAMERA_SPEED * seconds;
	final wheelDistance = input.wheel * WHEEL_DISTANCE;
	var x = state.x + (input.forward * lookX - input.right * lookZ) * distance + lookX * wheelDistance;
	var y = state.y + (input.forward * lookY + input.vertical) * distance + lookY * wheelDistance;
	var z = state.z + (input.forward * lookZ + input.right * lookX) * distance + lookZ * wheelDistance;
	final margin = 128.0;
	x = clamp(x, -margin, projection.width + margin);
	y = clamp(y, 0.25, projection.height + margin);
	z = clamp(z, -margin, projection.depth + margin);
	return {
		x: x,
		y: y,
		z: z,
		lookX: lookX,
		lookY: lookY,
		lookZ: lookZ
	};
}

/** Return the point one direction unit ahead of the camera. */
function cameraTarget(state:EditorCameraState):EditorWorldVector {
	return {
		x: state.x + state.lookX,
		y: state.y + state.lookY,
		z: state.z + state.lookZ
	};
}

/**
 * Pick the nearest visible solid voxel, or an empty cell on one edit layer.
 *
 * Solid unit boxes take priority because they are what the author sees in
 * front. If the ray misses every solid, intersection with the selected layer's
 * floor allows painting an empty world. Equal-distance solids keep canonical
 * z/y/x traversal order. Invalid directions, layers, bounds, or distances
 * return `null` without inventing a coordinate.
 */
function pickWorld(projection:EditorWorldProjection, origin:EditorWorldVector, direction:EditorWorldVector, layerY:Int,
		maximumDistance:Float):Null<EditorWorldHit> {
	if (layerY < 0 || layerY >= projection.height || maximumDistance <= 0.0)
		return null;
	var nearestDistance = maximumDistance + 1.0;
	var nearest:Null<VoxelPoint> = null;
	for (z in 0...projection.depth)
		for (y in 0...projection.height)
			for (x in 0...projection.width) {
				if (paletteCodeAtWorld(projection, x, y, z) == 0)
					continue;
				final distance = rayBoxDistance(origin, direction, x, y, z, maximumDistance);
				if (distance >= 0.0 && distance < nearestDistance) {
					nearestDistance = distance;
					nearest = {x: x, y: y, z: z};
				}
			}
	if (nearest != null)
		return {point: nearest, distance: nearestDistance, solid: true};

	if (absolute(direction.y) < RAY_EPSILON)
		return null;
	final floorDistance = (layerY - origin.y) / direction.y;
	if (floorDistance < 0.0 || floorDistance > maximumDistance)
		return null;
	final floorX = origin.x + direction.x * floorDistance;
	final floorZ = origin.z + direction.z * floorDistance;
	if (floorX < 0.0 || floorZ < 0.0 || floorX >= projection.width || floorZ >= projection.depth)
		return null;
	return {
		point: {x: Std.int(floorX), y: layerY, z: Std.int(floorZ)},
		distance: floorDistance,
		solid: false
	};
}

/**
 * Return the first ray distance inside one unit voxel, or `-1` on a miss.
 *
 * This is the three-axis slab test written out directly so picking allocates no
 * temporary arrays or per-axis records while it scans a cached finite volume.
 */
function rayBoxDistance(origin:EditorWorldVector, direction:EditorWorldVector, x:Int, y:Int, z:Int, maximumDistance:Float):Float {
	var near = 0.0;
	var far = maximumDistance;

	if (absolute(direction.x) < RAY_EPSILON) {
		if (origin.x < x || origin.x > x + 1.0)
			return -1.0;
	} else {
		var first = (x - origin.x) / direction.x;
		var second = (x + 1.0 - origin.x) / direction.x;
		if (first > second) {
			final swap = first;
			first = second;
			second = swap;
		}
		if (first > near)
			near = first;
		if (second < far)
			far = second;
		if (near > far)
			return -1.0;
	}

	if (absolute(direction.y) < RAY_EPSILON) {
		if (origin.y < y || origin.y > y + 1.0)
			return -1.0;
	} else {
		var first = (y - origin.y) / direction.y;
		var second = (y + 1.0 - origin.y) / direction.y;
		if (first > second) {
			final swap = first;
			first = second;
			second = swap;
		}
		if (first > near)
			near = first;
		if (second < far)
			far = second;
		if (near > far)
			return -1.0;
	}

	if (absolute(direction.z) < RAY_EPSILON) {
		if (origin.z < z || origin.z > z + 1.0)
			return -1.0;
	} else {
		var first = (z - origin.z) / direction.z;
		var second = (z + 1.0 - origin.z) / direction.z;
		if (first > second) {
			final swap = first;
			first = second;
			second = swap;
		}
		if (first > near)
			near = first;
		if (second < far)
			far = second;
		if (near > far)
			return -1.0;
	}
	return far < 0.0 || near > maximumDistance ? -1.0 : (near < 0.0 ? 0.0 : near);
}

inline function absolute(value:Float):Float
	return value < 0.0 ? -value : value;

inline function clamp(value:Float, minimum:Float, maximum:Float):Float
	return value < minimum ? minimum : (value > maximum ? maximum : value);
