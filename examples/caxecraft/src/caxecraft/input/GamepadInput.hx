package caxecraft.input;

import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.GameInputFrame.GameInputFrames;

/**
 * Converts one platform adapter's controller snapshot into player intent.
 *
 * Raylib owns device discovery and physical button sampling. This module owns
 * the brand-neutral Caxecraft mapping, dead zones, time-scaled look motion,
 * focus safety, and combination with another input source. Returning the same
 * `GameInputFrame` used by deterministic pilots keeps gameplay and conversation
 * code independent from controller APIs and button numbering.
 */
typedef GamepadInputSnapshot = {
	/** True when the platform currently exposes controller slot zero. */
	final connected:Bool;

	/** False after the application window loses operating-system focus. */
	final focused:Bool;

	/** True while ordinary play owns relative world input. */
	final captured:Bool;

	/** True while the application intentionally stops simulation. */
	final paused:Bool;

	/** Bounded rendered-frame duration used to make stick look rate-independent. */
	final frameSeconds:Float;

	/** Raw left-stick horizontal position in the inclusive range -1...1. */
	final leftX:Float;

	/** Raw left-stick vertical position in the inclusive range -1...1. */
	final leftY:Float;

	/** Raw right-stick horizontal position in the inclusive range -1...1. */
	final rightX:Float;

	/** Raw right-stick vertical position in the inclusive range -1...1. */
	final rightY:Float;

	/** Bottom face-button press edge: jump and menu confirmation. */
	final bottomPressed:Bool;

	/** Bottom face-button hold: swim upward or deliberately skip dialogue. */
	final bottomHeld:Bool;

	/** Right face-button hold: swim downward. */
	final rightHeld:Bool;

	/** Left face-button press edge: interact or continue dialogue. */
	final leftPressed:Bool;

	/** Right trigger press edge: mine or attack. */
	final rightTriggerPressed:Bool;

	/** Left trigger press edge: place or use the selected item. */
	final leftTriggerPressed:Bool;

	/** Left bumper press edge: select the previous hotbar slot. */
	final leftBumperPressed:Bool;

	/** Right bumper press edge: select the next hotbar slot. */
	final rightBumperPressed:Bool;

	/** D-pad up press edge used by title and campaign selection. */
	final dpadUpPressed:Bool;

	/** D-pad down press edge used by title and campaign selection. */
	final dpadDownPressed:Bool;

	/** Start-button press edge: pause, resume, or leave a nested menu. */
	final startPressed:Bool;
}

/**
 * Map one raw controller snapshot to a complete device-neutral frame.
 *
 * A disconnected or unfocused controller produces no intent. Paused snapshots
 * retain only menu, resume, and pause actions; world movement, hotbar changes,
 * and combat cannot leak through the pause screen. Analog look is measured per
 * second, while movement remains a normalized direction.
 */
function gamepadInput(snapshot:GamepadInputSnapshot):GameInputFrame {
	if (!snapshot.connected || !snapshot.focused)
		return GameInputFrames.idle();

	final worldActive = snapshot.captured && !snapshot.paused;
	final moveRight = worldActive ? gamepadAxis(snapshot.leftX) : 0.0;
	final moveForward = worldActive ? -gamepadAxis(snapshot.leftY) : 0.0;
	final elapsed = boundedFrameSeconds(snapshot.frameSeconds);
	final lookYaw = worldActive ? -gamepadAxis(snapshot.rightX) * 2.2 * elapsed : 0.0;
	final lookPitch = worldActive ? -gamepadAxis(snapshot.rightY) * 2.2 * elapsed : 0.0;
	var hotbarCycle = 0;
	if (worldActive && snapshot.leftBumperPressed != snapshot.rightBumperPressed)
		hotbarCycle = snapshot.leftBumperPressed ? -1 : 1;

	return {
		moveForward: moveForward,
		moveRight: moveRight,
		lookYaw: lookYaw,
		lookPitch: lookPitch,
		jumpPressed: worldActive && snapshot.bottomPressed,
		riseHeld: worldActive && snapshot.bottomHeld,
		descendHeld: worldActive && snapshot.rightHeld,
		primaryPressed: worldActive && snapshot.rightTriggerPressed,
		secondaryPressed: worldActive && snapshot.leftTriggerPressed,
		interactPressed: worldActive && snapshot.leftPressed,
		travelPressed: false,
		menuNextPressed: snapshot.dpadUpPressed || snapshot.dpadDownPressed,
		menuConfirmPressed: snapshot.bottomPressed,
		pausePressed: snapshot.startPressed,
		capturePressed: snapshot.paused && snapshot.bottomPressed,
		quitPressed: false,
		hotbarSelection: -1,
		hotbarCycle: hotbarCycle
	};
}

/**
 * Combine two device-neutral sources without granting either one new actions.
 *
 * Keyboard and controller directions add and clamp, so matching inputs agree
 * and opposites cancel. Presses combine with logical OR. Direct numbered-slot
 * selection remains authoritative, while simultaneous opposite cycle requests
 * cancel. This function has no device state and allocates no runtime object.
 */
function mergeGameInput(primary:GameInputFrame, secondary:GameInputFrame):GameInputFrame {
	final hotbarSelection = primary.hotbarSelection >= 0 ? primary.hotbarSelection : secondary.hotbarSelection;
	final hotbarCycle = boundedDirection(primary.hotbarCycle + secondary.hotbarCycle);
	return {
		moveForward: boundedAxis(primary.moveForward + secondary.moveForward),
		moveRight: boundedAxis(primary.moveRight + secondary.moveRight),
		lookYaw: primary.lookYaw + secondary.lookYaw,
		lookPitch: primary.lookPitch + secondary.lookPitch,
		jumpPressed: primary.jumpPressed || secondary.jumpPressed,
		riseHeld: primary.riseHeld || secondary.riseHeld,
		descendHeld: primary.descendHeld || secondary.descendHeld,
		primaryPressed: primary.primaryPressed || secondary.primaryPressed,
		secondaryPressed: primary.secondaryPressed || secondary.secondaryPressed,
		interactPressed: primary.interactPressed || secondary.interactPressed,
		travelPressed: primary.travelPressed || secondary.travelPressed,
		menuNextPressed: primary.menuNextPressed || secondary.menuNextPressed,
		menuConfirmPressed: primary.menuConfirmPressed || secondary.menuConfirmPressed,
		pausePressed: primary.pausePressed || secondary.pausePressed,
		capturePressed: primary.capturePressed || secondary.capturePressed,
		quitPressed: primary.quitPressed || secondary.quitPressed,
		hotbarSelection: hotbarSelection,
		hotbarCycle: hotbarCycle
	};
}

/** Remove stick drift, then rescale the remaining travel back to -1...1. */
private function gamepadAxis(value:Float):Float {
	final bounded = boundedAxis(value);
	final magnitude = bounded < 0.0 ? -bounded : bounded;
	if (magnitude <= 0.20)
		return 0.0;
	final scaled = (magnitude - 0.20) / 0.80;
	return bounded < 0.0 ? -scaled : scaled;
}

/** Clamp platform or test input to the controller axis contract. */
private inline function boundedAxis(value:Float):Float
	return value < -1.0 ? -1.0 : value > 1.0 ? 1.0 : value;

/** Clamp one signed hotbar request to a single rendered-frame step. */
private inline function boundedDirection(value:Int):Int
	return value < -1 ? -1 : value > 1 ? 1 : value;

/** Match the application's existing stalled-frame safety bound. */
private inline function boundedFrameSeconds(value:Float):Float
	return value < 0.0 ? 0.0 : value > 0.25 ? 0.25 : value;
