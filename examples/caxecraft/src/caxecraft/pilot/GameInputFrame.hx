package caxecraft.pilot;

/**
 * Device-independent player intent sampled once for one rendered frame.
 *
 * This is an immutable value record rather than an allocated input object.
 * Real devices and deterministic pilots can therefore feed the same game path
 * without per-frame allocation or virtual dispatch in generated C.
 *
 * Movement uses camera-relative axes in the inclusive range -1...1. Look
 * values are already-bounded changes for this frame, not raw mouse pixels.
 * The Boolean actions, including the target-neutral `interactPressed` intent,
 * are one-frame presses rather than held device buttons.
 * A hotbar selection of `-1` means “keep the current slot”; the cycle value is
 * a signed number of slots, normally -1, 0, or 1.
 */
typedef GameInputFrame = {
	final moveForward:Float;
	final moveRight:Float;
	final lookYaw:Float;
	final lookPitch:Float;
	final jumpPressed:Bool;

	/** Primary world action: mine with a block selected, strike with a sword. */
	final primaryPressed:Bool;

	final placePressed:Bool;
	final interactPressed:Bool;
	final pausePressed:Bool;
	final capturePressed:Bool;
	final quitPressed:Bool;
	final hotbarSelection:Int;
	final hotbarCycle:Int;
}

/** Allocation-free constructors and comparisons for `GameInputFrame`. */
final class GameInputFrames {
	public static inline function make(moveForward:Float, moveRight:Float, lookYaw:Float, lookPitch:Float, jumpPressed:Bool, primaryPressed:Bool,
			placePressed:Bool, interactPressed:Bool, pausePressed:Bool, capturePressed:Bool, quitPressed:Bool, hotbarSelection:Int = -1,
			hotbarCycle:Int = 0):GameInputFrame
		return {
			moveForward: moveForward,
			moveRight: moveRight,
			lookYaw: lookYaw,
			lookPitch: lookPitch,
			jumpPressed: jumpPressed,
			primaryPressed: primaryPressed,
			placePressed: placePressed,
			interactPressed: interactPressed,
			pausePressed: pausePressed,
			capturePressed: capturePressed,
			quitPressed: quitPressed,
			hotbarSelection: hotbarSelection,
			hotbarCycle: hotbarCycle
		};

	public static inline function idle():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, false, false, false, false, false, false);

	public static inline function quit():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, false, false, false, false, false, true);

	public static inline function move(moveForward:Float, moveRight:Float, lookYaw:Float, lookPitch:Float, jumpPressed:Bool):GameInputFrame
		return make(moveForward, moveRight, lookYaw, lookPitch, jumpPressed, false, false, false, false, false, false);

	public static inline function mine():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, true, false, false, false, false, false);

	public static inline function place():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, false, true, false, false, false, false);

	public static inline function interact():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, false, false, true, false, false, false);

	public static inline function pause():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, false, false, false, true, false, false);

	public static inline function capture():GameInputFrame
		return make(0.0, 0.0, 0.0, 0.0, false, false, false, false, false, true, false);

	/** Exact equality is intentional: pilot scripts contain fixed literal input. */
	public static function same(left:GameInputFrame, right:GameInputFrame):Bool {
		return left.moveForward == right.moveForward
			&& left.moveRight == right.moveRight
			&& left.lookYaw == right.lookYaw
			&& left.lookPitch == right.lookPitch
			&& left.jumpPressed == right.jumpPressed
			&& left.primaryPressed == right.primaryPressed
			&& left.placePressed == right.placePressed
			&& left.interactPressed == right.interactPressed
			&& left.pausePressed == right.pausePressed
			&& left.capturePressed == right.capturePressed
			&& left.quitPressed == right.quitPressed
			&& left.hotbarSelection == right.hotbarSelection
			&& left.hotbarCycle == right.hotbarCycle;
	}
}
