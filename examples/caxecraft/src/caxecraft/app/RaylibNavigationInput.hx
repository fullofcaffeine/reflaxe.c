package caxecraft.app;

#if c
import caxecraft.input.NavigationInput.NavigationSample;
import raylib.GamepadAxis;
import raylib.GamepadButton;
import raylib.Raylib;

/**
 * Samples Raylib controller slot zero into Caxecraft's device-neutral UI shape.
 *
 * Raylib owns discovery and the current device state. This adapter chooses the
 * conventional south face button for confirm and east face button for cancel,
 * then returns ordinary Haxe scalars. Dead-zone and repeat behavior remain in
 * `NavigationRepeater`, so future keyboard, pilot, or split-screen adapters can
 * share the same interaction policy without calling Raylib.
 */
function samplePrimaryGamepad():NavigationSample {
	final gamepad = 0;
	if (!Raylib.IsGamepadAvailable(gamepad))
		return {
			connected: false,
			up: false,
			right: false,
			down: false,
			left: false,
			confirmPressed: false,
			cancelPressed: false,
			horizontal: 0.0,
			vertical: 0.0
		};
	return {
		connected: true,
		up: Raylib.IsGamepadButtonDown(gamepad, GamepadButton.DpadUp),
		right: Raylib.IsGamepadButtonDown(gamepad, GamepadButton.DpadRight),
		down: Raylib.IsGamepadButtonDown(gamepad, GamepadButton.DpadDown),
		left: Raylib.IsGamepadButtonDown(gamepad, GamepadButton.DpadLeft),
		confirmPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.FaceDown),
		cancelPressed: Raylib.IsGamepadButtonPressed(gamepad, GamepadButton.FaceRight),
		horizontal: Raylib.GetGamepadAxisMovement(gamepad, GamepadAxis.LeftX).toFloat(),
		vertical: Raylib.GetGamepadAxisMovement(gamepad, GamepadAxis.LeftY).toFloat()
	};
}
#end
