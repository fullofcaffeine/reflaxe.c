package caxecraft.domain;

import caxecraft.domain.WorldRead.query as queryWorld;

/**
 * Resolves first-person and behind-player presentation from one gameplay aim.
 *
 * The committed character eye always owns mining, placing, talking, and combat
 * direction. Third-person changes only the presentation camera position. Its
 * boom samples a small camera volume against terrain and active authored solid
 * boxes, then shortens before the first obstruction.
 */
/** Player-selected presentation mode; it is not campaign content. */
enum PlayerCameraMode {
	FirstPerson;
	BehindPlayer;
}

/** One renderer-independent camera and its unchanged gameplay interaction ray. */
typedef PlayerCameraView = {
	final mode:PlayerCameraMode;
	final positionX:Float;
	final positionY:Float;
	final positionZ:Float;
	final targetX:Float;
	final targetY:Float;
	final targetZ:Float;
	final interactionOriginX:Float;
	final interactionOriginY:Float;
	final interactionOriginZ:Float;
	final interactionDirectionX:Float;
	final interactionDirectionY:Float;
	final interactionDirectionZ:Float;
	final boomDistance:Float;
	final avatarVisible:Bool;
}

/** Device-neutral camera and jump state sampled for one rendered frame. */
typedef PlayerLookFrameInput = {
	final mode:PlayerCameraMode;
	final lookX:Float;
	final lookY:Float;
	final lookZ:Float;
	final captured:Bool;
	final blockedByConversation:Bool;
	final togglePressed:Bool;
	final yawDelta:Float;
	final pitchDelta:Float;
	final jumpPressed:Bool;
	final jumpQueued:Bool;
}

/** Updated camera heading and queued jump after one input sample. */
typedef PlayerLookFrameOutcome = {
	final mode:PlayerCameraMode;
	final lookX:Float;
	final lookY:Float;
	final lookZ:Float;
	final jumpQueued:Bool;
}

inline final DESIRED_BOOM_DISTANCE:Float = 3.8;
inline final CAMERA_RADIUS:Float = 0.18;
inline final BOOM_STEP:Float = 0.1;
inline final EYE_HEIGHT:Float = 1.62;

/** Switch between the two admitted views without changing gameplay state. */
function togglePlayerCamera(mode:PlayerCameraMode):PlayerCameraMode
	return mode == FirstPerson ? BehindPlayer : FirstPerson;

/**
 * Apply one camera input sample without reading devices or mutating gameplay.
 *
 * The bounded yaw step keeps the inexpensive heading normalization stable, and
 * pitch remains inside the range used by world interaction and rendering.
 */
function advancePlayerLookFrame(input:PlayerLookFrameInput):PlayerLookFrameOutcome {
	var mode = input.mode;
	var lookX = input.lookX;
	var lookY = input.lookY;
	var lookZ = input.lookZ;
	var jumpQueued = input.jumpQueued;
	if (input.captured && !input.blockedByConversation) {
		if (input.togglePressed)
			mode = togglePlayerCamera(mode);
		var yawDelta = input.yawDelta;
		if (yawDelta > 0.25)
			yawDelta = 0.25;
		if (yawDelta < -0.25)
			yawDelta = -0.25;
		final candidateX = lookX + yawDelta * lookZ;
		final candidateZ = lookZ - yawDelta * lookX;
		final lengthSquared = candidateX * candidateX + candidateZ * candidateZ;
		final normalize = 1.5 - 0.5 * lengthSquared;
		lookX = candidateX * normalize;
		lookZ = candidateZ * normalize;
		lookY += input.pitchDelta;
		if (lookY > 0.90)
			lookY = 0.90;
		if (lookY < -0.90)
			lookY = -0.90;
		if (input.jumpPressed)
			jumpQueued = true;
	}
	return {
		mode: mode,
		lookX: lookX,
		lookY: lookY,
		lookZ: lookZ,
		jumpQueued: jumpQueued
	};
}

/**
 * Resolve one frame from committed state and current authored solid boxes.
 *
 * The returned interaction ray is identical in both modes. A zero-length look
 * falls back to the conventional negative-Z heading, so camera math cannot
 * create an invalid native vector.
 */
function resolvePlayerCamera(cells:WorldView, collisions:Array<DynamicCollisionBox>, mode:PlayerCameraMode, body:CharacterBody, lookX:Float, lookY:Float,
		lookZ:Float):PlayerCameraView {
	final lengthSquared = lookX * lookX + lookY * lookY + lookZ * lookZ;
	final inverseLength = lengthSquared <= 0.000001 ? 0.0 : 1.0 / Math.sqrt(lengthSquared);
	final directionX = inverseLength == 0.0 ? 0.0 : lookX * inverseLength;
	final directionY = inverseLength == 0.0 ? 0.0 : lookY * inverseLength;
	final directionZ = inverseLength == 0.0 ? -1.0 : lookZ * inverseLength;
	final eyeX = body.x;
	final eyeY = body.y + EYE_HEIGHT;
	final eyeZ = body.z;
	final distance = mode == FirstPerson ? 0.0 : clearBoomDistance(cells, collisions, eyeX, eyeY, eyeZ, -directionX, -directionY, -directionZ);
	return {
		mode: mode,
		positionX: eyeX - directionX * distance,
		positionY: eyeY - directionY * distance,
		positionZ: eyeZ - directionZ * distance,
		targetX: eyeX + directionX,
		targetY: eyeY + directionY,
		targetZ: eyeZ + directionZ,
		interactionOriginX: eyeX,
		interactionOriginY: eyeY,
		interactionOriginZ: eyeZ,
		interactionDirectionX: directionX,
		interactionDirectionY: directionY,
		interactionDirectionZ: directionZ,
		boomDistance: distance,
		avatarVisible: mode == BehindPlayer && distance > CAMERA_RADIUS * 2.0};
}

/** Return the furthest sampled distance whose camera volume stays clear. */
private function clearBoomDistance(cells:WorldView, collisions:Array<DynamicCollisionBox>, originX:Float, originY:Float, originZ:Float, directionX:Float,
		directionY:Float, directionZ:Float):Float {
	var clearDistance = 0.0;
	var candidate = BOOM_STEP;
	while (candidate <= DESIRED_BOOM_DISTANCE + 0.0001) {
		final x = originX + directionX * candidate;
		final y = originY + directionY * candidate;
		final z = originZ + directionZ * candidate;
		if (cameraVolumeBlocked(cells, collisions, x, y, z))
			return clearDistance;
		clearDistance = candidate;
		candidate += BOOM_STEP;
	}
	return DESIRED_BOOM_DISTANCE;
}

/** Test one small camera volume against terrain and authored AABBs. */
private function cameraVolumeBlocked(cells:WorldView, collisions:Array<DynamicCollisionBox>, x:Float, y:Float, z:Float):Bool {
	final minimumX = floorToInt(x - CAMERA_RADIUS);
	final maximumX = floorToInt(x + CAMERA_RADIUS);
	final minimumY = floorToInt(y - CAMERA_RADIUS);
	final maximumY = floorToInt(y + CAMERA_RADIUS);
	final minimumZ = floorToInt(z - CAMERA_RADIUS);
	final maximumZ = floorToInt(z + CAMERA_RADIUS);
	for (cellX in minimumX...maximumX + 1)
		for (cellY in minimumY...maximumY + 1)
			for (cellZ in minimumZ...maximumZ + 1) {
				final coordinate = World.coord(cellX, cellY, cellZ);
				if (!World.contains(coordinate) || World.isSolid(queryWorld(cells, coordinate)))
					return true;
			}
	for (box in collisions)
		if (x + CAMERA_RADIUS > box.minimumX && x - CAMERA_RADIUS < box.maximumX && y + CAMERA_RADIUS > box.minimumY && y - CAMERA_RADIUS < box.maximumY
			&& z + CAMERA_RADIUS > box.minimumZ && z - CAMERA_RADIUS < box.maximumZ)
			return true;
	return false;
}

/** Floor one finite world coordinate without target-specific math. */
private function floorToInt(value:Float):Int {
	final truncated = Std.int(value);
	return value < truncated ? truncated - 1 : truncated;
}
