package caxecraft.app;

#if c
import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.GameInputFrame.GameInputFrames;
import raylib.KeyboardKey;
import raylib.MouseButton;
import raylib.Raylib;

/** Converts real devices into the same allocation-free intent used by pilots. */
final class RaylibGameInput {
	static inline final LOOK_SENSITIVITY:Float = 0.0025;

	public static inline function sample(captured:Bool, paused:Bool):GameInputFrame {
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
			lookYaw = -mouse.x.toFloat() * LOOK_SENSITIVITY;
			lookPitch = -mouse.y.toFloat() * LOOK_SENSITIVITY;
		}

		final leftPressed = Raylib.IsMouseButtonPressed(MouseButton.Left);
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
		final wheel = Raylib.GetMouseWheelMove().toFloat();
		var hotbarCycle = 0;
		if (wheel > 0.0)
			hotbarCycle = -1;
		if (wheel < 0.0)
			hotbarCycle = 1;
		return GameInputFrames.make(forward, right, lookYaw, lookPitch,
			Raylib.IsKeyPressed(KeyboardKey.Space), captured && leftPressed, captured && Raylib.IsMouseButtonPressed(MouseButton.Right), captured
			&& Raylib.IsKeyPressed(KeyboardKey.E),
			Raylib.IsKeyPressed(KeyboardKey.Escape), paused && leftPressed, Raylib.IsKeyPressed(KeyboardKey.Q), hotbarSelection, hotbarCycle);
	}
}
#end
