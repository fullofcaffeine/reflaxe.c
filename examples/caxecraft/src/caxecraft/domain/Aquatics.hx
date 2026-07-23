package caxecraft.domain;

import caxecraft.domain.CharacterPhysics.FIXED_SECONDS;
import caxecraft.domain.CharacterPhysics.HEIGHT;
import caxecraft.domain.CharacterPhysics.input as landInput;
import caxecraft.domain.CharacterPhysics.resolveVelocity;
import caxecraft.domain.CharacterPhysics.step as stepOnLand;
import caxecraft.domain.WaterCellCodec.stateAt as waterStateAt;

/**
	Fixed-tick swimming, floating, breath, and waterline state.

	The smallest mental model is: observe how much of the body is in water,
	compute forces from that fraction, resolve the resulting velocity through the
	ordinary character collision box, then update breath from the final head sample.
	No renderer delta, camera position, item name, or frame traversal can change
	the result.
**/
private inline final DRY_ENTER:Float = 0.08;

private inline final DRY_EXIT:Float = 0.04;
private inline final FLOAT_ENTER:Float = 0.68;
private inline final FLOAT_EXIT:Float = 0.52;
private inline final SUBMERGED_EXIT:Float = 0.74;
private inline final FEET_SAMPLE:Float = 0.08;
private inline final BODY_SAMPLE:Float = 0.90;
private inline final HEAD_SAMPLE:Float = 1.62;
private inline final CAMERA_FADE_DEPTH:Float = 0.16;
private inline final LAND_GRAVITY:Float = -18.0;
private inline final MIN_WATER_SPEED:Float = 2.0;
private inline final WATER_SPEED_RANGE:Float = 2.0;
private inline final MIN_HORIZONTAL_ACCELERATION:Float = 4.0;
private inline final HORIZONTAL_ACCELERATION_RANGE:Float = 8.0;
private inline final MAX_VERTICAL_SPEED:Float = 4.5;
private inline final EPSILON:Float = 0.0001;

/**
	Create one bounded content-supplied capability profile.

	All numeric fields are clamped to conservative engine limits. Content loading
	should reject out-of-range authored values with a diagnostic; this second
	boundary keeps restored saves or generated adapters safe as well.
**/
function profile(maximumBreathTicks:Int, breathRecoveryPerTick:Int, horizontalControl:Float, ascentAcceleration:Float, descentAcceleration:Float,
		buoyancyAcceleration:Float, dragPerTick:Float, drowningIntervalTicks:Int, underwaterMining:Bool, coldProtection:Bool):AquaticProfile {
	return {
		maximumBreathTicks: clampInt(maximumBreathTicks, 1, 12000),
		breathRecoveryPerTick: clampInt(breathRecoveryPerTick, 1, 120),
		horizontalControl: clamp(horizontalControl, 0.0, 1.0),
		ascentAcceleration: clamp(ascentAcceleration, 0.0, 40.0),
		descentAcceleration: clamp(descentAcceleration, 0.0, 40.0),
		buoyancyAcceleration: clamp(buoyancyAcceleration, 0.0, 30.0),
		dragPerTick: clamp(dragPerTick, 0.0, 0.90),
		drowningIntervalTicks: clampInt(drowningIntervalTicks, 1, 1200),
		underwaterMining: underwaterMining,
		coldProtection: coldProtection
	};
}

/** Start or retry with full breath and no remembered water contact. */
function start(capability:AquaticProfile):AquaticState {
	return {
		medium: Dry,
		submersion: 0.0,
		headSubmerged: false,
		breathTicks: capability.maximumBreathTicks,
		drowningTicks: capability.drowningIntervalTicks
	};
}

/**
	Adopt a replacement capability without granting or deleting spent breath.

	Equipment changes the maximum tank, movement, and permissions immediately.
	The number of already consumed breath ticks stays the same, capped to the new
	maximum; this makes upgrades useful underwater without turning re-equipping
	into an unlimited refill. The current immersion label remains unchanged.
**/
function adoptProfile(state:AquaticState, previous:AquaticProfile, replacement:AquaticProfile):AquaticState {
	final previousBreath = clampInt(state.breathTicks, 0, previous.maximumBreathTicks);
	final spentBreath = previous.maximumBreathTicks - previousBreath;
	return {
		medium: state.medium,
		submersion: state.submersion,
		headSubmerged: state.headSubmerged,
		breathTicks: clampInt(replacement.maximumBreathTicks - spentBreath, 0, replacement.maximumBreathTicks),
		drowningTicks: clampInt(state.drowningTicks, 1, replacement.drowningIntervalTicks)
	};
}

/** Build input that can request independent upward and downward swimming. */
function input(moveX:Float, moveZ:Float, ascend:Bool, descend:Bool):CharacterIntent {
	return {
		moveX: moveX,
		moveZ: moveZ,
		jump: ascend,
		descend: descend
	};
}

/**
	Measure vertical water overlap and the documented feet/body/head samples.

	Water strength controls the surface height of a non-falling cell. A source or
	vertical fall fills its voxel. The continuous overlap is sampled at the
	character's center column; collision continues to use the complete horizontal
	body box.
**/
function observe(cells:WorldCells, body:CharacterBody):Immersion {
	final feetY = body.y;
	final headY = body.y + HEIGHT;
	var submergedHeight = 0.0;
	var cellY = floorToInt(feetY);
	final lastY = floorToInt(headY - EPSILON);
	while (cellY <= lastY) {
		final top = waterSurface(cells, body.x, cellY, body.z);
		if (top > cellY) {
			final overlapBottom = larger(feetY, cellY);
			final overlapTop = smaller(headY, top);
			if (overlapTop > overlapBottom)
				submergedHeight += overlapTop - overlapBottom;
		}
		cellY++;
	}
	final eyeY = body.y + HEAD_SAMPLE;
	final eyeSurface = waterSurface(cells, body.x, floorToInt(eyeY), body.z);
	final cameraDepth = eyeSurface - eyeY;
	return {
		submersion: clamp(submergedHeight / HEIGHT, 0.0, 1.0),
		feetWet: pointIsWet(cells, body.x, body.y + FEET_SAMPLE, body.z),
		bodyWet: pointIsWet(cells, body.x, body.y + BODY_SAMPLE, body.z),
		headWet: pointIsWet(cells, body.x, eyeY, body.z),
		cameraBlend: clamp((cameraDepth + CAMERA_FADE_DEPTH * 0.5) / CAMERA_FADE_DEPTH, 0.0, 1.0)
	};
}

/** Choose one stable label from continuous immersion and the prior label. */
function classifyMedium(previous:AquaticMedium, immersion:Immersion):AquaticMedium {
	return switch previous {
		case Dry:
			immersion.submersion >= DRY_ENTER ? Wading : Dry;
		case Wading:
			if (immersion.submersion <= DRY_EXIT) Dry; else if (immersion.headWet) Submerged; else if (immersion.submersion >= FLOAT_ENTER) Floating; else
				Wading;
		case Floating:
			if (immersion.headWet) Submerged; else if (immersion.submersion <= FLOAT_EXIT) Wading; else Floating;
		case Submerged: !immersion.headWet && immersion.submersion <= SUBMERGED_EXIT ? Floating : Submerged;
	};
}

/** Advance movement and breath by exactly one 50 ms simulation tick. */
function step(cells:WorldCells, original:CharacterBody, aquatic:AquaticState, command:CharacterIntent, capability:AquaticProfile):AquaticStep {
	final before = observe(cells, original);
	final beforeMedium = classifyMedium(aquatic.medium, before);
	var moved = original;
	if (beforeMedium == Dry) {
		moved = stepOnLand(cells, original, landInput(command.moveX, command.moveZ, command.jump));
	} else {
		final wet = before.submersion;
		final speed = MIN_WATER_SPEED + WATER_SPEED_RANGE * capability.horizontalControl;
		final acceleration = MIN_HORIZONTAL_ACCELERATION + HORIZONTAL_ACCELERATION_RANGE * capability.horizontalControl;
		var velocityX = approach(original.velocityX, command.moveX * speed, acceleration * FIXED_SECONDS);
		var velocityZ = approach(original.velocityZ, command.moveZ * speed, acceleration * FIXED_SECONDS);
		final drag = 1.0 - capability.dragPerTick * wet;
		velocityX *= drag;
		velocityZ *= drag;
		var verticalAcceleration = LAND_GRAVITY * (1.0 - 0.85 * wet) + capability.buoyancyAcceleration * wet;
		if (command.jump)
			verticalAcceleration += capability.ascentAcceleration * wet;
		if (command.descend)
			verticalAcceleration -= capability.descentAcceleration * wet;
		var velocityY = (original.velocityY + verticalAcceleration * FIXED_SECONDS) * drag;
		velocityY = clamp(velocityY, -MAX_VERTICAL_SPEED, MAX_VERTICAL_SPEED);
		moved = resolveVelocity(cells, original, velocityX, velocityY, velocityZ);
	}

	final after = observe(cells, moved);
	final medium = classifyMedium(beforeMedium, after);
	final breath = advanceBreath(aquatic, after, capability);
	return {
		body: moved,
		immersion: after,
		aquatic: {
			medium: medium,
			submersion: after.submersion,
			headSubmerged: after.headWet,
			breathTicks: breath.remaining,
			drowningTicks: breath.drowningTicks
		},
		drowningDamage: breath.damage
	};
}

/** True when the current medium and resolved capability permit block mining. */
function canMine(state:AquaticState, capability:AquaticProfile):Bool {
	return state.medium != Submerged || capability.underwaterMining;
}

/** Expose the resolved cold-water protection bit without naming its source. */
inline function hasColdProtection(capability:AquaticProfile):Bool
	return capability.coldProtection;

private function advanceBreath(previous:AquaticState, immersion:Immersion, capability:AquaticProfile):BreathStep {
	var remaining = clampInt(previous.breathTicks, 0, capability.maximumBreathTicks);
	var drowningTicks = clampInt(previous.drowningTicks, 1, capability.drowningIntervalTicks);
	var damage = 0;
	if (immersion.headWet) {
		if (remaining > 0) {
			remaining--;
		} else {
			drowningTicks--;
			if (drowningTicks <= 0) {
				damage = 1;
				drowningTicks = capability.drowningIntervalTicks;
			}
		}
	} else {
		remaining = clampInt(remaining + capability.breathRecoveryPerTick, 0, capability.maximumBreathTicks);
		drowningTicks = capability.drowningIntervalTicks;
	}
	return {remaining: remaining, drowningTicks: drowningTicks, damage: damage};
}

private function waterSurface(cells:WorldCells, x:Float, y:Int, z:Float):Float {
	final coord = World.coord(floorToInt(x), y, floorToInt(z));
	return switch waterStateAt(cells, coord) {
		case Source: y + 1.0;
		case Flowing(level, falling):
			if (falling) y + 1.0; else {
				final distance:Int = level;
				y + (8.0 - distance) / 8.0;
			}
		case Empty | Blocked | InvalidStorage(_): y;
	};
}

private function pointIsWet(cells:WorldCells, x:Float, y:Float, z:Float):Bool {
	final cellY = floorToInt(y);
	return waterSurface(cells, x, cellY, z) > y + EPSILON;
}

private function approach(current:Float, target:Float, maximumChange:Float):Float {
	if (current < target)
		return smaller(current + maximumChange, target);
	if (current > target)
		return larger(current - maximumChange, target);
	return current;
}

private inline function smaller(left:Float, right:Float):Float
	return left < right ? left : right;

private inline function larger(left:Float, right:Float):Float
	return left > right ? left : right;

private function clamp(value:Float, minimum:Float, maximum:Float):Float {
	if (value < minimum)
		return minimum;
	if (value > maximum)
		return maximum;
	return value;
}

private function clampInt(value:Int, minimum:Int, maximum:Int):Int {
	if (value < minimum)
		return minimum;
	if (value > maximum)
		return maximum;
	return value;
}

private function floorToInt(value:Float):Int {
	final truncated = Std.int(value);
	return value < truncated ? truncated - 1 : truncated;
}

private typedef BreathStep = {
	final remaining:Int;
	final drowningTicks:Int;
	final damage:Int;
}
