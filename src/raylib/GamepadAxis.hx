package raylib;

#if c
/**
 * Names the two sticks and two analog triggers exposed by raylib 6.0.
 *
 * The readable Haxe names carry the exact integer values from `raylib.h`.
 * `Raylib.GetGamepadAxisMovement` converts this value to C's 32-bit `int`
 * without allocating or adding a run-time wrapper.
 */
enum abstract GamepadAxis(Int) {
	var LeftX = 0;
	var LeftY = 1;
	var RightX = 2;
	var RightY = 3;
	var LeftTrigger = 4;
	var RightTrigger = 5;
}
#end
