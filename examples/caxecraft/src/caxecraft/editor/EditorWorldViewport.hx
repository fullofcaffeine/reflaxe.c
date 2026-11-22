package caxecraft.editor;

import caxecraft.editor.EditorWorldGrid.decode as decodeWorld;
import caxecraft.scenario.ScenarioId;
import caxecraft.scenario.ScenarioGeometry.ScenarioTransform;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioWorld;

/**
 * Describes the authored voxel volume and camera math without depending on a
 * renderer.
 *
 * The CAXEMAP draft remains the only editable world. `projectWorld` creates a
 * screen-owned presentation cache after an accepted edit, camera functions turn
 * bounded input into a fresh snapshot, and `pickWorld` maps a viewing ray back
 * to one authored coordinate. Eval tests and the native Raylib editor therefore
 * share spatial rules without either implementation copying game state.
 */
/** One screen-owned copy of the complete finite voxel volume and its overview. */
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

/** State whether an editor marker owns a facing direction and its validated yaw. */
enum EditorObjectFacing {
	/** Bounded trigger volumes do not face a direction. */
	NoObjectFacing;

	/** Transform-backed placements face this many degrees clockwise from north. */
	ObjectYaw(degrees:Int);
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

	/** Authored cell origin used by exact whole-cell placement gestures. */
	final origin:VoxelPoint;

	final x:Float;
	final y:Float;
	final z:Float;
	final width:Float;
	final height:Float;
	final depth:Float;
	final facing:EditorObjectFacing;
}

/** The nearest stable authored object reached by one bounded world ray. */
typedef EditorObjectHit = {
	final id:ScenarioId;
	final distance:Float;
}

/** The camera position and unit-like forward direction for one frame. */
typedef EditorCameraPose = {
	final x:Float;
	final y:Float;
	final z:Float;
	final lookX:Float;
	final lookY:Float;
	final lookZ:Float;
}

/**
 * The three creator views that Build can use.
 *
 * Walk follows the authored surface for direct edits. Fly keeps unrestricted
 * movement for large worlds. Orbit keeps one explicit target in view for
 * inspection. An enum abstract gives the closed choice an integer C value.
 */
enum abstract EditorCameraMode(Int) {
	/** Follow each column's top surface for direct world edits. */
	var WalkCamera = 0;

	/** Move freely inside generous bounds around the finite world. */
	var FlyCamera = 1;

	/** Keep one selected object or world point at the center of the view. */
	var OrbitCamera = 2;
}

/**
 * One complete camera snapshot, including mode-specific state.
 *
 * Orbit alone owns a target and horizontal distance. The typed enum prevents
 * Walk or Fly from carrying inactive orbit fields. The screen replaces this
 * immutable snapshot after each input frame.
 */
enum EditorCameraState {
	/** A surface-following creator pose. This is not player collision physics. */
	WalkingCamera(pose:EditorCameraPose);

	/** An unrestricted creator pose. */
	FlyingCamera(pose:EditorCameraPose);

	/** A creator pose with the exact point and distance that it keeps in view. */
	OrbitingCamera(pose:EditorCameraPose, target:EditorWorldVector, horizontalDistance:Float);
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

/**
 * One bounded selection volume prepared for a single renderer draw call.
 *
 * A selection can contain up to 65,536 cells. Projecting only its outer box
 * keeps the authored bounds visible without making frame cost grow per cell.
 */
typedef EditorSelectionGizmo = {
	final x:Float;
	final y:Float;
	final z:Float;
	final width:Float;
	final height:Float;
	final depth:Float;
}

/**
 * The nearest editable coordinate selected by a viewing ray.
 *
 * `placement` is the last empty cell before a solid target. An empty-floor hit
 * uses its own point. A solid at the first visited cell has no safe placement.
 */
typedef EditorWorldHit = {
	final point:VoxelPoint;
	final placement:Null<VoxelPoint>;
	final distance:Float;
	final solid:Bool;
}

/** Entry and exit distances for one ray through the finite world box. */
private typedef EditorRayInterval = {
	final near:Float;
	final far:Float;
}

/** Convert one half-open voxel selection into its exact world-space box. */
function projectSelection(bounds:VoxelBounds):EditorSelectionGizmo {
	return {
		x: bounds.origin.x + bounds.size.width * 0.5,
		y: bounds.origin.y + bounds.size.height * 0.5,
		z: bounds.origin.z + bounds.size.depth * 0.5,
		width: bounds.size.width,
		height: bounds.size.height,
		depth: bounds.size.depth
	};
}

final CAMERA_SPEED = 8.0;
final WALK_SPEED = 5.0;
final WALK_EYE_HEIGHT = 1.62;
final WHEEL_DISTANCE = 2.0;
final MIN_ORBIT_DISTANCE = 2.0;
final MAX_FRAME_SECONDS = 0.1;
final MAX_LOOK_STEP = 0.25;
final MIN_PITCH = -0.90;
final MAX_PITCH = 0.90;
final MAX_ORBIT_PITCH = -0.05;
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

/**
 * Apply one accepted voxel edit to this screen-owned projection.
 *
 * The editor session has already validated and stored the command. This
 * function updates the exact cell and rebuilds only the small x/z overview.
 * It validates all array bounds before mutation, so `false` leaves the cache
 * unchanged and lets the caller request a complete projection.
 */
function patchProjectedVoxel(projection:EditorWorldProjection, point:VoxelPoint, paletteCode:Int):Bool {
	if (paletteCode < 0 || point.x < 0 || point.y < 0 || point.z < 0 || point.x >= projection.width || point.y >= projection.height
		|| point.z >= projection.depth)
		return false;
	final volume = projection.width * projection.height * projection.depth;
	final area = projection.width * projection.depth;
	if (projection.width <= 0 || projection.height <= 0 || projection.depth <= 0 || projection.cells.length != volume || projection.surfaceTops.length != area)
		return false;

	final changedCell = (point.z * projection.height + point.y) * projection.width + point.x;
	final changedColumn = point.z * projection.width + point.x;
	final nextTops = projection.surfaceTops.copy();
	var changedTop = -1;
	for (y in 0...projection.height) {
		final index = (point.z * projection.height + y) * projection.width + point.x;
		final code = index == changedCell ? paletteCode : projection.cells[index];
		if (code != 0)
			changedTop = y;
	}
	nextTops[changedColumn] = changedTop;

	final nextColumns:Array<EditorTerrainColumn> = [];
	final nextPaletteCodes:Array<Int> = [];
	for (z in 0...projection.depth)
		for (x in 0...projection.width) {
			final column = z * projection.width + x;
			final topY = nextTops[column];
			if (topY < -1 || topY >= projection.height)
				return false;
			final index = topY < 0 ? -1 : (z * projection.height + topY) * projection.width + x;
			final code = index < 0 ? 0 : index == changedCell ? paletteCode : projection.cells[index];
			if (topY >= 0)
				nextColumns.push({
					x: x,
					z: z,
					topY: topY,
					paletteCode: code
				});
			nextPaletteCodes.push(code);
		}
	final nextPatches = projectSurfacePatches(projection.width, projection.depth, nextTops, nextPaletteCodes);

	projection.cells[changedCell] = paletteCode;
	projection.surfaceTops[changedColumn] = changedTop;
	projection.columns.resize(0);
	for (column in nextColumns)
		projection.columns.push(column);
	projection.surfacePatches.resize(0);
	for (patch in nextPatches)
		projection.surfacePatches.push(patch);
	return true;
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
			case PlayerSpawn(transform): pointGizmo(object.id, PlayerSpawnGizmo, transform);
			case Checkpoint(transform): pointGizmo(object.id, CheckpointGizmo, transform);
			case Item(_, _, transform): pointGizmo(object.id, ItemGizmo, transform);
			case Entity(_, transform): pointGizmo(object.id, EntityGizmo, transform);
			case Npc(_, _, transform): pointGizmo(object.id, NpcGizmo, transform);
			case Prefab(_, transform): pointGizmo(object.id, PrefabGizmo, transform);
			case TriggerZone(bounds):
				{
					id: object.id,
					kind: TriggerZoneGizmo,
					origin: {x: bounds.origin.x, y: bounds.origin.y, z: bounds.origin.z},
					x: bounds.origin.x + bounds.size.width * 0.5,
					y: bounds.origin.y + bounds.size.height * 0.5,
					z: bounds.origin.z + bounds.size.depth * 0.5,
					width: bounds.size.width,
					height: bounds.size.height,
					depth: bounds.size.depth,
					facing: NoObjectFacing
				};
			case StatefulObject(_, _, transform):
				pointGizmo(object.id, StatefulObjectGizmo, transform);
		});
	return projected;
}

/** True when one object selection box crosses the displayed voxel layer. */
function gizmoIntersectsLayer(gizmo:EditorObjectGizmo, layerY:Int):Bool {
	final minimum = gizmo.y - gizmo.height * 0.5;
	final maximum = gizmo.y + gizmo.height * 0.5;
	return maximum > layerY && minimum < layerY + 1;
}

/**
 * Select the nearest authored object box reached by a bounded world ray.
 *
 * Raylib and future adapters supply the same origin and direction values. This
 * function owns semantic hit ordering: nearer objects win, and canonical
 * authored order wins an exact overlap. A miss returns `null` without changing
 * editor selection.
 */
function pickObject(gizmos:Array<EditorObjectGizmo>, origin:EditorWorldVector, direction:EditorWorldVector, maximumDistance:Float):Null<EditorObjectHit> {
	if (maximumDistance < 0.0)
		return null;
	var nearest:Null<EditorObjectHit> = null;
	var nearestDistance = maximumDistance + 1.0;
	for (gizmo in gizmos) {
		final distance = rayBoxDistance(gizmo, origin, direction, maximumDistance);
		if (distance != null && distance < nearestDistance) {
			nearestDistance = distance;
			nearest = {id: gizmo.id, distance: distance};
		}
	}
	return nearest;
}

/** Clip one ray against a gizmo's axis-aligned selection box. */
private function rayBoxDistance(gizmo:EditorObjectGizmo, origin:EditorWorldVector, direction:EditorWorldVector, maximumDistance:Float):Null<Float> {
	var interval:EditorRayInterval = {near: 0.0, far: maximumDistance};
	interval = switch clipRayAxis(origin.x, direction.x, gizmo.x - gizmo.width * 0.5, gizmo.x + gizmo.width * 0.5, interval) {
		case null: return null;
		case value: value;
	};
	interval = switch clipRayAxis(origin.y, direction.y, gizmo.y - gizmo.height * 0.5, gizmo.y + gizmo.height * 0.5, interval) {
		case null: return null;
		case value: value;
	};
	interval = switch clipRayAxis(origin.z, direction.z, gizmo.z - gizmo.depth * 0.5, gizmo.z + gizmo.depth * 0.5, interval) {
		case null: return null;
		case value: value;
	};
	return interval.near;
}

/** Narrow one ray interval along an axis, including parallel rays inside it. */
private function clipRayAxis(origin:Float, direction:Float, minimum:Float, maximum:Float, interval:EditorRayInterval):Null<EditorRayInterval> {
	final magnitude = direction < 0.0 ? -direction : direction;
	if (magnitude < 0.000001)
		return origin < minimum || origin > maximum ? null : interval;
	var first = (minimum - origin) / direction;
	var second = (maximum - origin) / direction;
	if (first > second) {
		final swap = first;
		first = second;
		second = swap;
	}
	final near = first > interval.near ? first : interval.near;
	final far = second < interval.far ? second : interval.far;
	return near > far ? null : {near: near, far: far};
}

/** Make one standard point marker without claiming collision or art bounds. */
private inline function pointGizmo(id:ScenarioId, kind:EditorObjectGizmoKind, transform:ScenarioTransform):EditorObjectGizmo
	return {
		id: id,
		kind: kind,
		origin: {
			x: Std.int(transform.xMilli / 1000),
			y: Std.int(transform.yMilli / 1000),
			z: Std.int(transform.zMilli / 1000)
		},
		x: transform.xMilli / 1000.0,
		y: transform.yMilli / 1000.0 + 0.5,
		z: transform.zMilli / 1000.0,
		width: 0.7,
		height: 1.0,
		depth: 0.7,
		facing: ObjectYaw(transform.yawDegrees)
	};

/** Return one palette code, or `-1` when the coordinate is outside the draft. */
function paletteCodeAtWorld(projection:EditorWorldProjection, x:Int, y:Int, z:Int):Int {
	if (x < 0 || y < 0 || z < 0 || x >= projection.width || y >= projection.height || z >= projection.depth)
		return -1;
	return projection.cells[(z * projection.height + y) * projection.width + x];
}

/**
 * Place the requested camera at a deterministic useful view.
 *
 * Walk starts inside the south edge. Fly frames the complete draft from above.
 * Orbit frames the supplied target, or the world center if no target exists.
 * New World, resize, and the F shortcut therefore produce the same view for
 * the same mode and target.
 */
function focusCamera(projection:EditorWorldProjection, mode:EditorCameraMode = FlyCamera, ?orbitTarget:EditorWorldVector,
		orbitDistance:Float = 0.0):EditorCameraState {
	final extent = projection.width > projection.depth ? projection.width : projection.depth;
	return switch mode {
		case FlyCamera:
			FlyingCamera({
				x: projection.width * 0.5,
				y: projection.height + extent * 0.4 + 2.0,
				z: projection.depth + extent * 0.4 + 1.0,
				lookX: 0.0,
				lookY: -0.5,
				lookZ: -0.8660254037844386
			});
		case WalkCamera:
			final x = clamp(projection.width * 0.5, 0.001, projection.width - 0.001);
			final z = clamp(projection.depth - 0.5, 0.001, projection.depth - 0.001);
			WalkingCamera({
				x: x,
				y: walkEyeY(projection, x, z),
				z: z,
				lookX: 0.0,
				lookY: -0.20,
				lookZ: -1.0
			});
		case OrbitCamera:
			final target:EditorWorldVector = if (orbitTarget == null) {
				x: projection.width * 0.5,
				y: projection.height * 0.5,
				z: projection.depth * 0.5
			} else {
				x: orbitTarget.x,
				y: orbitTarget.y,
				z: orbitTarget.z
			};
			var distance = orbitDistance;
			if (distance <= 0.0)
				distance = extent * 0.75 + 2.0;
			distance = clamp(distance, MIN_ORBIT_DISTANCE, maximumOrbitDistance(projection));
			final direction:EditorCameraPose = {
				x: 0.0,
				y: 0.0,
				z: 0.0,
				lookX: 0.0,
				lookY: -0.35,
				lookZ: -1.0
			};
			OrbitingCamera(orbitPose(direction, target, distance), target, distance);
	};
}

/**
 * Advance the active camera by one bounded displayed-frame input.
 *
 * Walk follows the visible surface and ignores flight input. Fly keeps the
 * original free movement. Orbit changes its angle and distance but keeps its
 * target fixed. Each mode uses the same bounded look update.
 */
function stepCamera(projection:EditorWorldProjection, state:EditorCameraState, input:EditorCameraInput, frameSeconds:Float):EditorCameraState {
	return switch state {
		case WalkingCamera(pose): WalkingCamera(stepWalkCamera(projection, pose, input, frameSeconds));
		case FlyingCamera(pose): FlyingCamera(stepFlyCamera(projection, pose, input, frameSeconds));
		case OrbitingCamera(pose, target, horizontalDistance):
			var distance = horizontalDistance - input.wheel * WHEEL_DISTANCE;
			distance = clamp(distance, MIN_ORBIT_DISTANCE, maximumOrbitDistance(projection));
			final direction = turnCamera(pose, input, MIN_PITCH, MAX_ORBIT_PITCH);
			OrbitingCamera(orbitPose(direction, target, distance), target, distance);
	};
}

/** Return the active mode without exposing its private state shape. */
function cameraMode(state:EditorCameraState):EditorCameraMode {
	return switch state {
		case WalkingCamera(_): WalkCamera;
		case FlyingCamera(_): FlyCamera;
		case OrbitingCamera(_, _, _): OrbitCamera;
	};
}

/** Return the next mode used by the visible Camera control and the C key. */
function cycleCameraMode(mode:EditorCameraMode):EditorCameraMode {
	return switch mode {
		case WalkCamera: FlyCamera;
		case FlyCamera: OrbitCamera;
		case OrbitCamera: WalkCamera;
	};
}

/** Return the renderer-ready pose for the active camera state. */
function cameraPose(state:EditorCameraState):EditorCameraPose {
	return switch state {
		case WalkingCamera(pose): pose;
		case FlyingCamera(pose): pose;
		case OrbitingCamera(pose, _, _): pose;
	};
}

/** Move only an Orbit target while preserving its angle and zoom distance. */
function retargetOrbitCamera(state:EditorCameraState, target:EditorWorldVector):EditorCameraState {
	return switch state {
		case OrbitingCamera(pose, _, horizontalDistance):
			final ownedTarget:EditorWorldVector = {x: target.x, y: target.y, z: target.z};
			OrbitingCamera(orbitPose(pose, ownedTarget, horizontalDistance), ownedTarget, horizontalDistance);
		case WalkingCamera(_) | FlyingCamera(_): state;
	};
}

/** Advance unrestricted Fly movement and retain its generous outer bounds. */
private function stepFlyCamera(projection:EditorWorldProjection, state:EditorCameraPose, input:EditorCameraInput, frameSeconds:Float):EditorCameraPose {
	var seconds = frameSeconds;
	if (seconds < 0.0)
		seconds = 0.0;
	if (seconds > MAX_FRAME_SECONDS)
		seconds = MAX_FRAME_SECONDS;
	final direction = turnCamera(state, input, MIN_PITCH, MAX_PITCH);

	final distance = CAMERA_SPEED * seconds;
	final wheelDistance = input.wheel * WHEEL_DISTANCE;
	var x = state.x + (input.forward * direction.lookX - input.right * direction.lookZ) * distance + direction.lookX * wheelDistance;
	var y = state.y + (input.forward * direction.lookY + input.vertical) * distance + direction.lookY * wheelDistance;
	var z = state.z + (input.forward * direction.lookZ + input.right * direction.lookX) * distance + direction.lookZ * wheelDistance;
	final margin = 128.0;
	x = clamp(x, -margin, projection.width + margin);
	y = clamp(y, 0.25, projection.height + margin);
	z = clamp(z, -margin, projection.depth + margin);
	return {
		x: x,
		y: y,
		z: z,
		lookX: direction.lookX,
		lookY: direction.lookY,
		lookZ: direction.lookZ
	};
}

/** Advance grounded movement and place the camera eye above the new column. */
private function stepWalkCamera(projection:EditorWorldProjection, state:EditorCameraPose, input:EditorCameraInput, frameSeconds:Float):EditorCameraPose {
	var seconds = frameSeconds;
	if (seconds < 0.0)
		seconds = 0.0;
	if (seconds > MAX_FRAME_SECONDS)
		seconds = MAX_FRAME_SECONDS;
	final direction = turnCamera(state, input, MIN_PITCH, MAX_PITCH);
	final distance = WALK_SPEED * seconds;
	var x = state.x + (input.forward * direction.lookX - input.right * direction.lookZ) * distance;
	var z = state.z + (input.forward * direction.lookZ + input.right * direction.lookX) * distance;
	x = clamp(x, 0.001, projection.width - 0.001);
	z = clamp(z, 0.001, projection.depth - 0.001);
	return {
		x: x,
		y: walkEyeY(projection, x, z),
		z: z,
		lookX: direction.lookX,
		lookY: direction.lookY,
		lookZ: direction.lookZ
	};
}

/** Apply one bounded yaw and pitch change without target-specific math calls. */
private function turnCamera(state:EditorCameraPose, input:EditorCameraInput, minimumPitch:Float, maximumPitch:Float):EditorCameraPose {
	var yaw = input.yaw;
	if (yaw > MAX_LOOK_STEP)
		yaw = MAX_LOOK_STEP;
	if (yaw < -MAX_LOOK_STEP)
		yaw = -MAX_LOOK_STEP;
	final candidateX = state.lookX + yaw * state.lookZ;
	final candidateZ = state.lookZ - yaw * state.lookX;
	final lengthSquared = candidateX * candidateX + candidateZ * candidateZ;
	final normalization = 1.5 - 0.5 * lengthSquared;
	var lookY = state.lookY + input.pitch;
	if (lookY > maximumPitch)
		lookY = maximumPitch;
	if (lookY < minimumPitch)
		lookY = minimumPitch;
	return {
		x: state.x,
		y: state.y,
		z: state.z,
		lookX: candidateX * normalization,
		lookY: lookY,
		lookZ: candidateZ * normalization
	};
}

/** Place an Orbit pose so its direction reaches the fixed target exactly. */
private function orbitPose(direction:EditorCameraPose, target:EditorWorldVector, horizontalDistance:Float):EditorCameraPose {
	return {
		x: target.x - direction.lookX * horizontalDistance,
		y: target.y - direction.lookY * horizontalDistance,
		z: target.z - direction.lookZ * horizontalDistance,
		lookX: direction.lookX,
		lookY: direction.lookY,
		lookZ: direction.lookZ
	};
}

/** Return a player-like eye height above a solid surface or the empty floor. */
private function walkEyeY(projection:EditorWorldProjection, x:Float, z:Float):Float {
	final top = surfaceTopAt(projection, Std.int(x), Std.int(z));
	return (top < 0 ? 0.0 : top + 1.0) + WALK_EYE_HEIGHT;
}

/** Keep zoom within the same generous range as Fly movement. */
private function maximumOrbitDistance(projection:EditorWorldProjection):Float {
	var extent = projection.width;
	if (projection.height > extent)
		extent = projection.height;
	if (projection.depth > extent)
		extent = projection.depth;
	return extent + 128.0;
}

/** Return the camera target used by the native renderer and world picker. */
function cameraTarget(state:EditorCameraState):EditorWorldVector {
	return switch state {
		case OrbitingCamera(_, target, _): target;
		case WalkingCamera(pose) | FlyingCamera(pose): {
				x: pose.x + pose.lookX,
				y: pose.y + pose.lookY,
				z: pose.z + pose.lookZ
			};
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
		var hasPlacement = false;
		var placementX = x;
		var placementY = y;
		var placementZ = z;
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
				return {
					point: {x: x, y: y, z: z},
					placement: hasPlacement ? {x: placementX, y: placementY, z: placementZ} : null,
					distance: distance,
					solid: true
				};
			hasPlacement = true;
			placementX = x;
			placementY = y;
			placementZ = z;
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
		placement: {x: Std.int(floorX), y: layerY, z: Std.int(floorZ)},
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
