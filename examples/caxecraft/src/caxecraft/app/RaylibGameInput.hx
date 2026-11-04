package caxecraft.app;

#if c
import caxecraft.input.ControlPrompts.ControlPromptDevice;
import caxecraft.input.GamepadInput.GamepadInputSnapshot;
import caxecraft.input.GamepadInput.gamepadInput;
import caxecraft.input.GamepadInput.mergeGameInput;
import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.GameInputFrame.GameInputFrames;
import raylib.GamepadAxis;
import raylib.GamepadButton;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;

/**
 * Converts real keyboard, mouse, and gamepad state into one shared input frame.
 *
 * Raylib remains the narrow platform boundary. The target-neutral gamepad
 * module owns semantic button mapping and focus safety, while this adapter only
 * samples named Raylib values and combines them with the existing keyboard and
 * mouse frame. The prompt device accompanies the frame for presentation only;
 * gameplay still receives `GameInputFrame` and cannot inspect hardware.
 */
typedef RaylibGameInputSample = {
	/** Complete device-neutral intent for the current rendered frame. */
	final frame:GameInputFrame;

	/** Available device whose instructions the menus and HUD should show. */
	final promptDevice:ControlPromptDevice;
}

/** Toggle developer metrics without adding that command to release input. */
#if caxecraft_devmode
function debugHudTogglePressed():Bool
	return Raylib.IsKeyPressed(KeyboardKey.F3);
#end

/** Toggle the player presentation camera while ordinary play owns the pointer. */
function cameraTogglePressed(captured:Bool):Bool
	return captured && Raylib.IsKeyPressed(KeyboardKey.F5);

/**
 * Sample every supported local device exactly once for this application frame.
 *
 * Focus is supplied by the application so a lost window produces no input
 * before the existing screen transition pauses play. A connected controller
 * selects controller help even on title, campaign, and pause screens.
 */
function sampleGameInput(captured:Bool, paused:Bool, focused:Bool, frameSeconds:Float):RaylibGameInputSample {
	final keyboard = keyboardAndMouseInput(captured, paused, focused);
	final gamepadAvailable = Raylib.IsGamepadAvailable(0);
	final controller = gamepadInput(primaryGamepadSnapshot(gamepadAvailable, captured, paused, focused, frameSeconds));
	return {
		frame: mergeGameInput(keyboard, controller),
		promptDevice: gamepadAvailable ? ControlPromptDevice.Gamepad : ControlPromptDevice.KeyboardMouse
	};
}

/** Keep the established keyboard and mouse grammar as one complete frame. */
private function keyboardAndMouseInput(captured:Bool, paused:Bool, focused:Bool):GameInputFrame {
	if (!focused)
		return GameInputFrames.idle();
	var forward = 0.0;
	var right = 0.0;
	if (Raylib.IsKeyDown(KeyboardKey.W))
		forward += 1.0;
	if (Raylib.IsKeyDown(KeyboardKey.S))
		forward -= 1.0;
	if (Raylib.IsKeyDown(KeyboardKey.D))
		right += 1.0;
	if (Raylib.IsKeyDown(KeyboardKey.A))
		right -= 1.0;

	var lookYaw = 0.0;
	var lookPitch = 0.0;
	if (captured) {
		final mouse = Raylib.GetMouseDelta();
		// Raylib reports positive X to the right. The game starts facing
		// negative Z, so a right turn has the opposite rotation sign.
		lookYaw = -mouse.x.toFloat() * 0.0025;
		lookPitch = -mouse.y.toFloat() * 0.0025;
	}

	final leftPressed = Raylib.IsMouseButtonPressed(MouseButton.Left);
	final primaryPressed = captured && leftPressed;
	final secondaryPressed = captured && Raylib.IsMouseButtonPressed(MouseButton.Right);
	final interactPressed = captured && Raylib.IsKeyPressed(KeyboardKey.E);
	#if caxecraft_devmode
	// Developer builds retain N as a quick map-preview shortcut. Release play
	// travels only when reloadable content raises an authored exit request.
	final travelPressed = captured && Raylib.IsKeyPressed(KeyboardKey.N);
	#else
	final travelPressed = false;
	#end
	var hotbarSelection = -1;
	if (Raylib.IsKeyPressed(KeyboardKey.One))
		hotbarSelection = 0;
	if (Raylib.IsKeyPressed(KeyboardKey.Two))
		hotbarSelection = 1;
	if (Raylib.IsKeyPressed(KeyboardKey.Three))
		hotbarSelection = 2;
	if (Raylib.IsKeyPressed(KeyboardKey.Four))
		hotbarSelection = 3;
	if (Raylib.IsKeyPressed(KeyboardKey.Five))
		hotbarSelection = 4;
	if (Raylib.IsKeyPressed(KeyboardKey.Six))
		hotbarSelection = 5;
	if (Raylib.IsKeyPressed(KeyboardKey.Seven))
		hotbarSelection = 6;
	if (Raylib.IsKeyPressed(KeyboardKey.Eight))
		hotbarSelection = 7;
	if (Raylib.IsKeyPressed(KeyboardKey.Nine))
		hotbarSelection = 8;
	final wheel = Raylib.GetMouseWheelMove().toFloat();
	var hotbarCycle = 0;
	if (wheel > 0.0)
		hotbarCycle = -1;
	if (wheel < 0.0)
		hotbarCycle = 1;
	return GameInputFrames.make(forward, right, lookYaw, lookPitch, Raylib.IsKeyPressed(KeyboardKey.Space), primaryPressed, secondaryPressed, interactPressed,
		travelPressed, Raylib.IsKeyPressed(KeyboardKey.Escape), paused && leftPressed, Raylib.IsKeyPressed(KeyboardKey.Q), hotbarSelection, hotbarCycle,
		Raylib.IsKeyDown(KeyboardKey.LeftShift), Raylib.IsKeyPressed(KeyboardKey.Up) || Raylib.IsKeyPressed(KeyboardKey.Down),
		Raylib.IsKeyPressed(KeyboardKey.Enter), Raylib.IsKeyDown(KeyboardKey.Space));
}

/** Copy named Raylib values into the target-neutral controller snapshot. */
private function primaryGamepadSnapshot(connected:Bool, captured:Bool, paused:Bool, focused:Bool, frameSeconds:Float):GamepadInputSnapshot {
	final gamepad = 0;
	if (!connected)
		return disconnectedGamepad(captured, paused, focused, frameSeconds);
	return {
		connected: true,
		focused: focused,
		captured: captured,
		paused: paused,
		frameSeconds: frameSeconds,
		leftX: Raylib.GetGamepadAxisMovement(gamepad, GamepadAxis.LeftX).toFloat(),
		leftY: Raylib.GetGamepadAxisMovement(gamepad, GamepadAxis.LeftY).toFloat(),
		rightX: Raylib.GetGamepadAxisMovement(gamepad, GamepadAxis.RightX).toFloat(),
		rightY: Raylib.GetGamepadAxisMovement(gamepad, GamepadAxis.RightY).toFloat(),
		bottomPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.FaceDown),
		bottomHeld: Raylib.IsGamepadButtonDown(gamepad, GamepadButton.FaceDown),
		rightHeld: Raylib.IsGamepadButtonDown(gamepad, GamepadButton.FaceRight),
		leftPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.FaceLeft),
		rightTriggerPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.RightTrigger),
		leftTriggerPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.LeftTrigger),
		leftBumperPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.LeftBumper),
		rightBumperPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.RightBumper),
		dpadUpPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.DpadUp),
		dpadDownPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.DpadDown),
		startPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.Start)
	};
}

/** Return a complete zeroed snapshot without calling unavailable axes. */
private function disconnectedGamepad(captured:Bool, paused:Bool, focused:Bool, frameSeconds:Float):GamepadInputSnapshot
	return {
		connected: false,
		focused: focused,
		captured: captured,
		paused: paused,
		frameSeconds: frameSeconds,
		leftX: 0.0,
		leftY: 0.0,
		rightX: 0.0,
		rightY: 0.0,
		bottomPressed: false,
		bottomHeld: false,
		rightHeld: false,
		leftPressed: false,
		rightTriggerPressed: false,
		leftTriggerPressed: false,
		leftBumperPressed: false,
		rightBumperPressed: false,
		dpadUpPressed: false,
		dpadDownPressed: false,
		startPressed: false
	};
#end
