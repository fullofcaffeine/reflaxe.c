package caxecraft.pilot;

import caxecraft.pilot.GameInputFrame.GameInputFrames;
import caxecraft.pilot.PilotCheckpoint.PilotCheckpointKind;

/** Closed, allocation-free names for deterministic game scripts. */
enum abstract PilotScriptName(Int) {
	var LaunchSmoke = 0;
	var MoveJumpEdit = 1;
	var PauseRecapture = 2;
}

/** One closed semantic action selected for a scripted frame. */
enum abstract PilotAction(Int) {
	var Idle = 0;
	var Quit = 1;
	var Forward = 2;
	var ForwardTurn = 3;
	var ForwardJump = 4;
	var RightLook = 5;
	var Mine = 6;
	var Place = 7;
	var Pause = 8;
	var Capture = 9;
	var SelectNext = 10;
	var Interact = 11;
}

/**
 * Deterministic input scripts with fixed observation points and bounded quit.
 *
 * All operations are static over a small name value. A native pilot therefore
 * adds neither a heap object nor interface dispatch to the game loop.
 */
final class PilotScript {
	public static inline final ABSOLUTE_FRAME_LIMIT:Int = 120;

	public static function frameLimit(name:PilotScriptName):Int {
		if (name == LaunchSmoke)
			return 4;
		if (name == MoveJumpEdit)
			return 10;
		return 7;
	}

	public static function stableName(name:PilotScriptName):String {
		if (name == LaunchSmoke)
			return "launch-smoke";
		if (name == MoveJumpEdit)
			return "move-jump-edit";
		return "pause-recapture";
	}

	public static function actionAt(name:PilotScriptName, frameNumber:Int):PilotAction {
		if (frameNumber >= frameLimit(name) - 1)
			return Quit;
		if (name == LaunchSmoke)
			return Idle;
		if (name == MoveJumpEdit)
			return moveJumpAction(frameNumber);
		return pauseAction(frameNumber);
	}

	public static function sample(name:PilotScriptName, frameNumber:Int):GameInputFrame {
		final action = actionAt(name, frameNumber);
		return GameInputFrames.make(moveForward(action), moveRight(action), lookYaw(action), lookPitch(action), jumpPressed(action), minePressed(action),
			placePressed(action), interactPressed(action), pausePressed(action), capturePressed(action), quitPressed(action), hotbarSelection(action),
			hotbarCycle(action));
	}

	public static inline function moveForward(action:PilotAction):Float
		return action == Forward || action == ForwardTurn || action == ForwardJump ? 1.0 : 0.0;

	public static inline function moveRight(action:PilotAction):Float
		return action == RightLook ? 1.0 : 0.0;

	public static inline function lookYaw(action:PilotAction):Float
		return action == ForwardTurn ? -0.05 : 0.0;

	public static inline function lookPitch(action:PilotAction):Float
		return action == RightLook ? 0.04 : 0.0;

	public static inline function jumpPressed(action:PilotAction):Bool
		return action == ForwardJump;

	public static inline function minePressed(action:PilotAction):Bool
		return action == Mine;

	public static inline function placePressed(action:PilotAction):Bool
		return action == Place;

	public static inline function interactPressed(action:PilotAction):Bool
		return action == Interact;

	public static inline function pausePressed(action:PilotAction):Bool
		return action == Pause;

	public static inline function capturePressed(action:PilotAction):Bool
		return action == Capture;

	public static inline function quitPressed(action:PilotAction):Bool
		return action == Quit;

	public static inline function hotbarSelection(_action:PilotAction):Int
		return -1;

	public static inline function hotbarCycle(action:PilotAction):Int
		return action == SelectNext ? 1 : 0;

	/** Returns at most one evidence request for a completed frame. */
	public static function checkpoint(name:PilotScriptName, frameNumber:Int):Null<PilotCheckpoint> {
		return switch name {
			case LaunchSmoke:
				frameNumber == 1 ? new PilotCheckpoint("launch.frame-1", CaptureScreenshot) : null;
			case MoveJumpEdit:
				moveJumpCheckpoint(frameNumber);
			case PauseRecapture:
				pauseCheckpoint(frameNumber);
		};
	}

	public static inline function complete(name:PilotScriptName, frameNumber:Int):Bool
		return frameNumber >= frameLimit(name) - 1;

	static function moveJumpAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0: Forward;
			case 1: ForwardTurn;
			case 2: ForwardJump;
			case 3: RightLook;
			case 4: Mine;
			case 5: Place;
			case 6: SelectNext;
			case 7: Interact;
			case _: Idle;
		};
	}

	static function pauseAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 1: Pause;
			case 3: Capture;
			case _: Idle;
		};
	}

	static function moveJumpCheckpoint(frameNumber:Int):Null<PilotCheckpoint> {
		return switch frameNumber {
			case 7: new PilotCheckpoint("move-jump-edit.state", ObserveState);
			case 8: new PilotCheckpoint("move-jump-edit.frame", CaptureScreenshot);
			case _: null;
		};
	}

	static function pauseCheckpoint(frameNumber:Int):Null<PilotCheckpoint> {
		return switch frameNumber {
			case 2: new PilotCheckpoint("pause-recapture.paused", ObserveState);
			case 4: new PilotCheckpoint("pause-recapture.captured", ObserveState);
			case _: null;
		};
	}
}
