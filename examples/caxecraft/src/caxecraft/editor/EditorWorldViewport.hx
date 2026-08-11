package caxecraft.editor;

import caxecraft.editor.EditorWorldGrid.decode as decodeWorld;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioObject;
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

	/** Top solid and material for each non-empty x/z column in canonical order. */
	final columns:Array<EditorTerrainColumn>;

	/** Top solid Y for every x/z column, or `-1` when that column is empty. */
	final surfaceTops:Array<Int>;

	/** Greedy rectangles for the visible top surface. */
	final surfacePatches:Array<EditorTerrainPatch>;
}

/** One compact surface-overview column backed by the exact voxel cache. */
typedef EditorTerrainColumn = {
	final x:Int;
	final z:Int;
	final topY:Int;
	final paletteCode:Int;
}

/** One rectangular run of equal-height, equal-material top cells. */
typedef EditorTerrainPatch = {
	final x:Int;
	final z:Int;
	final width:Int;
	final depth:Int;
	final topY:Int;
	final paletteCode:Int;
}

/** Visual role for one authored object without renderer or campaign details. */
enum EditorObjectGizmoKind {
	PlayerSpawnGizmo;
	CheckpointGizmo;
	ItemGizmo;
	EntityGizmo;
	NpcGizmo;
	PrefabGizmo;
	TriggerZoneGizmo;
	StatefulObjectGizmo;
}

/**
 * Read-only box used to show one stable CAXEMAP object in a 3D editor.
 *
 * Point placements use a small standard marker around their authored
 * thousandth-block position. Trigger zones preserve their exact half-open
 * bounds. The box is a selection aid, not an actor collision or art model.
 */
typedef EditorObjectGizmo = {
	final id:ScenarioId;
	final kind:EditorObjectGizmoKind;
	final x:Float;
	final y:Float;
	final z:Float;
	final width:Float;
	final height:Float;
	final depth:Float;
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

/** Entry and exit distances for one ray through the finite world box. */
private typedef EditorRayInterval = {
	final near:Float;
	final far:Float;
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
	final columns:Array<EditorTerrainColumn> = [];
	final surfaceTops:Array<Int> = [];
	final surfacePaletteCodes:Array<Int> = [];
	for (z in 0...world.size.depth)
		for (x in 0...world.size.width) {
			var topY = -1;
			var paletteCode = 0;
			for (y in 0...world.size.height) {
				final code = cells[(z * world.size.height + y) * world.size.width + x];
				if (code != 0) {
					topY = y;
					paletteCode = code;
				}
			}
			if (topY >= 0)
				columns.push({
					x: x,
					z: z,
					topY: topY,
					paletteCode: paletteCode
				});
			surfaceTops.push(topY);
			surfacePaletteCodes.push(paletteCode);
		}
	final surfacePatches = projectSurfacePatches(world.size.width, world.size.depth, surfaceTops, surfacePaletteCodes);
	return {
		width: world.size.width,
		height: world.size.height,
		depth: world.size.depth,
		cells: cells,
		columns: columns,
		surfaceTops: surfaceTops,
		surfacePatches: surfacePatches
	};
}

/** Merge adjacent equal top cells so the overview submits little geometry. */
private function projectSurfacePatches(width:Int, depth:Int, tops:Array<Int>, paletteCodes:Array<Int>):Array<EditorTerrainPatch> {
	final used:Array<Bool> = [];
	for (_ in 0...width * depth)
		used.push(false);
	final patches:Array<EditorTerrainPatch> = [];
	for (z in 0...depth)
		for (x in 0...width) {
			final index = z * width + x;
			final topY = tops[index];
			if (topY < 0 || used[index])
				continue;
			final paletteCode = paletteCodes[index];
			var patchWidth = 1;
			while (x + patchWidth < width) {
				final candidate = z * width + x + patchWidth;
				if (used[candidate] || tops[candidate] != topY || paletteCodes[candidate] != paletteCode)
					break;
				patchWidth++;
			}
			var patchDepth = 1;
			var canExtend = true;
			while (z + patchDepth < depth && canExtend) {
				for (offset in 0...patchWidth) {
					final candidate = (z + patchDepth) * width + x + offset;
					if (used[candidate] || tops[candidate] != topY || paletteCodes[candidate] != paletteCode) {
						canExtend = false;
						break;
					}
				}
				if (canExtend)
					patchDepth++;
			}
			for (usedZ in z...z + patchDepth)
				for (usedX in x...x + patchWidth)
					used[usedZ * width + usedX] = true;
			patches.push({
				x: x,
				z: z,
				width: patchWidth,
				depth: patchDepth,
				topY: topY,
				paletteCode: paletteCode
			});
		}
	return patches;
}

/** Return one surface height, or `-1` outside the finite x/z footprint. */
function surfaceTopAt(projection:EditorWorldProjection, x:Int, z:Int):Int {
	if (x < 0 || z < 0 || x >= projection.width || z >= projection.depth)
		return -1;
	return projection.surfaceTops[z * projection.width + x];
}

/**
 * Project every admitted object placement in deterministic authored order.
 *
 * The result keeps stable IDs and closed placement roles. It does not resolve
 * content profiles or invent campaign behavior, so the same projection works
 * for a blank map, a mod, or a shipped Adventure level.
 */
function projectObjects(objects:Array<ScenarioObject>):Array<EditorObjectGizmo> {
	final projected:Array<EditorObjectGizmo> = [];
	for (object in objects)
		projected.push(switch object.placement {
			case PlayerSpawn(transform): pointGizmo(object.id, PlayerSpawnGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
			case Checkpoint(transform): pointGizmo(object.id, CheckpointGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
			case Item(_, _, transform): pointGizmo(object.id, ItemGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
			case Entity(_, transform): pointGizmo(object.id, EntityGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
			case Npc(_, _, transform): pointGizmo(object.id, NpcGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
			case Prefab(_, transform): pointGizmo(object.id, PrefabGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
			case TriggerZone(bounds):
				{
					id: object.id,
					kind: TriggerZoneGizmo,
					x: bounds.origin.x + bounds.size.width * 0.5,
					y: bounds.origin.y + bounds.size.height * 0.5,
					z: bounds.origin.z + bounds.size.depth * 0.5,
					width: bounds.size.width,
					height: bounds.size.height,
					depth: bounds.size.depth
				};
			case StatefulObject(_, _, transform):
				pointGizmo(object.id, StatefulObjectGizmo, transform.xMilli, transform.yMilli, transform.zMilli);
		});
	return projected;
}

/** Make one standard point marker without claiming collision or art bounds. */
private inline function pointGizmo(id:ScenarioId, kind:EditorObjectGizmoKind, xMilli:Int, yMilli:Int, zMilli:Int):EditorObjectGizmo
	return {
		id: id,
		kind: kind,
		x: xMilli / 1000.0,
		y: yMilli / 1000.0 + 0.5,
		z: zMilli / 1000.0,
		width: 0.7,
		height: 1.0,
		depth: 0.7
	};

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
 * Solid cells take priority because they are what the author sees in front.
 * The search enters the finite world box once, then visits only cells crossed
 * by the ray. It does not scan unrelated map volume. If the ray misses every
 * solid, intersection with the selected layer's floor allows painting an
 * empty world. Invalid input returns `null` without inventing a coordinate.
 */
function pickWorld(projection:EditorWorldProjection, origin:EditorWorldVector, direction:EditorWorldVector, layerY:Int,
		maximumDistance:Float):Null<EditorWorldHit> {
	if (layerY < 0 || layerY >= projection.height || maximumDistance <= 0.0)
		return null;
	final interval = rayVolumeInterval(projection, origin, direction, maximumDistance);
	if (interval != null) {
		var distance = interval.near < 0.0 ? 0.0 : interval.near;
		final sampleDistance = distance + RAY_EPSILON;
		var x = Std.int(clamp(origin.x + direction.x * sampleDistance, 0.0, projection.width - RAY_EPSILON));
		var y = Std.int(clamp(origin.y + direction.y * sampleDistance, 0.0, projection.height - RAY_EPSILON));
		var z = Std.int(clamp(origin.z + direction.z * sampleDistance, 0.0, projection.depth - RAY_EPSILON));
		final stepX = direction.x > RAY_EPSILON ? 1 : (direction.x < -RAY_EPSILON ? -1 : 0);
		final stepY = direction.y > RAY_EPSILON ? 1 : (direction.y < -RAY_EPSILON ? -1 : 0);
		final stepZ = direction.z > RAY_EPSILON ? 1 : (direction.z < -RAY_EPSILON ? -1 : 0);
		final unreachable = maximumDistance + projection.width + projection.height + projection.depth + 1.0;
		final deltaX = stepX == 0 ? unreachable : absolute(1.0 / direction.x);
		final deltaY = stepY == 0 ? unreachable : absolute(1.0 / direction.y);
		final deltaZ = stepZ == 0 ? unreachable : absolute(1.0 / direction.z);
		var nextX = stepX == 0 ? unreachable : ((stepX > 0 ? x + 1.0 : x) - origin.x) / direction.x;
		var nextY = stepY == 0 ? unreachable : ((stepY > 0 ? y + 1.0 : y) - origin.y) / direction.y;
		var nextZ = stepZ == 0 ? unreachable : ((stepZ > 0 ? z + 1.0 : z) - origin.z) / direction.z;
		while (x >= 0 && y >= 0 && z >= 0 && x < projection.width && y < projection.height && z < projection.depth && distance <= interval.far
			&& distance <= maximumDistance) {
			if (paletteCodeAtWorld(projection, x, y, z) != 0)
				return {point: {x: x, y: y, z: z}, distance: distance, solid: true};
			var next = nextX;
			if (nextY < next)
				next = nextY;
			if (nextZ < next)
				next = nextZ;
			distance = next;
			if (absolute(nextX - next) < RAY_EPSILON) {
				x += stepX;
				nextX += deltaX;
			}
			if (absolute(nextY - next) < RAY_EPSILON) {
				y += stepY;
				nextY += deltaY;
			}
			if (absolute(nextZ - next) < RAY_EPSILON) {
				z += stepZ;
				nextZ += deltaZ;
			}
		}
	}

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
 * Return the ray interval inside the complete finite world, or `null` on a miss.
 *
 * This three-axis slab test allocates one result for a successful pick. It is
 * separate from grid traversal so parallel axes fail before any cell loop.
 */
private function rayVolumeInterval(projection:EditorWorldProjection, origin:EditorWorldVector, direction:EditorWorldVector,
		maximumDistance:Float):Null<EditorRayInterval> {
	if (absolute(direction.x) < RAY_EPSILON && absolute(direction.y) < RAY_EPSILON && absolute(direction.z) < RAY_EPSILON)
		return null;
	var near = 0.0;
	var far = maximumDistance;

	if (absolute(direction.x) < RAY_EPSILON) {
		if (origin.x < 0.0 || origin.x > projection.width)
			return null;
	} else {
		var first = -origin.x / direction.x;
		var second = (projection.width - origin.x) / direction.x;
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
			return null;
	}

	if (absolute(direction.y) < RAY_EPSILON) {
		if (origin.y < 0.0 || origin.y > projection.height)
			return null;
	} else {
		var first = -origin.y / direction.y;
		var second = (projection.height - origin.y) / direction.y;
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
			return null;
	}

	if (absolute(direction.z) < RAY_EPSILON) {
		if (origin.z < 0.0 || origin.z > projection.depth)
			return null;
	} else {
		var first = -origin.z / direction.z;
		var second = (projection.depth - origin.z) / direction.z;
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
			return null;
	}
	return far < 0.0 || near > maximumDistance ? null : {near: near, far: far};
}

inline function absolute(value:Float):Float
	return value < 0.0 ? -value : value;

inline function clamp(value:Float, minimum:Float, maximum:Float):Float
	return value < minimum ? minimum : (value > maximum ? maximum : value);
