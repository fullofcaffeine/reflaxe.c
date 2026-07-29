package raylib;

#if c
/**
 * Gives raylib's platform-neutral gamepad positions readable Haxe names.
 *
 * Raylib describes face buttons by position rather than by an Xbox, PlayStation,
 * or Nintendo label, so one game action does not change meaning with the
 * controller brand. Each member keeps the exact integer value from
 * `raylib.h`; semantic facade calls pass it directly as C's 32-bit `int`.
 */
enum abstract GamepadButton(Int) {
	var Unknown = 0;
	var DpadUp = 1;
	var DpadRight = 2;
	var DpadDown = 3;
	var DpadLeft = 4;
	var FaceUp = 5;
	var FaceRight = 6;
	var FaceDown = 7;
	var FaceLeft = 8;
	var LeftBumper = 9;
	var LeftTrigger = 10;
	var RightBumper = 11;
	var RightTrigger = 12;
	var Select = 13;
	var Home = 14;
	var Start = 15;
	var LeftStick = 16;
	var RightStick = 17;
}
#end
