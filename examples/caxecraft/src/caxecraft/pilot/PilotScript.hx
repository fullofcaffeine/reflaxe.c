package caxecraft.pilot;

import caxecraft.gameplay.PlayerVitals;
import caxecraft.gameplay.Recovery;
import caxecraft.gameplay.Inventory;
import caxecraft.pilot.GameInputFrame.GameInputFrames;
import caxecraft.pilot.PilotCheckpoint.PilotCheckpointKind;

/** Closed, allocation-free names for deterministic game scripts. */
enum abstract PilotScriptName(Int) {
	var LaunchSmoke = 0;
	var MoveJumpEdit = 1;
	var PauseRecapture = 2;
	var CombatDrop = 3;
	var RecoveryUse = 4;
	var FullInventoryGift = 5;
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
	var SelectSword = 12;
	var Strike = 13;
	var SelectBerries = 14;
	var EatBerries = 15;
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
		if (name == PauseRecapture)
			return 7;
		if (name == CombatDrop)
			return 6;
		return 4;
	}

	public static function stableName(name:PilotScriptName):String {
		if (name == LaunchSmoke)
			return "launch-smoke";
		if (name == MoveJumpEdit)
			return "move-jump-edit";
		if (name == PauseRecapture)
			return "pause-recapture";
		if (name == CombatDrop)
			return "combat-drop";
		if (name == RecoveryUse)
			return "recovery-use";
		return "full-inventory-gift";
	}

	public static function actionAt(name:PilotScriptName, frameNumber:Int):PilotAction {
		if (frameNumber >= frameLimit(name) - 1)
			return Quit;
		if (name == LaunchSmoke)
			return Idle;
		if (name == MoveJumpEdit)
			return moveJumpAction(frameNumber);
		if (name == PauseRecapture)
			return pauseAction(frameNumber);
		if (name == CombatDrop)
			return combatAction(frameNumber);
		if (name == RecoveryUse)
			return recoveryAction(frameNumber);
		return fullInventoryAction(frameNumber);
	}

	public static function sample(name:PilotScriptName, frameNumber:Int):GameInputFrame {
		final action = actionAt(name, frameNumber);
		return GameInputFrames.make(moveForward(action), moveRight(action), lookYaw(action), lookPitch(action), jumpPressed(action), primaryPressed(action),
			secondaryPressed(action), interactPressed(action), pausePressed(action), capturePressed(action), quitPressed(action), hotbarSelection(action),
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

	public static inline function primaryPressed(action:PilotAction):Bool
		return action == Mine || action == Strike;

	public static inline function secondaryPressed(action:PilotAction):Bool
		return action == Place || action == EatBerries;

	public static inline function interactPressed(action:PilotAction):Bool
		return action == Interact;

	public static inline function pausePressed(action:PilotAction):Bool
		return action == Pause;

	public static inline function capturePressed(action:PilotAction):Bool
		return action == Capture;

	public static inline function quitPressed(action:PilotAction):Bool
		return action == Quit;

	public static inline function hotbarSelection(action:PilotAction):Int {
		if (action == SelectSword)
			return 4;
		return action == SelectBerries ? 5 : -1;
	}

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
			case CombatDrop:
				frameNumber == 4 ? new PilotCheckpoint("combat-drop.frame", CaptureScreenshot) : null;
			case RecoveryUse:
				frameNumber == 2 ? new PilotCheckpoint("recovery-use.frame", CaptureScreenshot) : null;
			case FullInventoryGift:
				frameNumber == 2 ? new PilotCheckpoint("full-inventory-gift.frame", CaptureScreenshot) : null;
		};
	}

	/** Initial fixture health; ordinary and release paths begin at full health. */
	public static inline function initialHealth(name:PilotScriptName):Int
		return name == RecoveryUse ? PlayerVitals.MAX_HEALTH - Recovery.BERRY_HEALTH : PlayerVitals.MAX_HEALTH;

	/** Whether the provider should begin with the berry stack at its exact cap. */
	public static inline function startsWithFullBerryStack(name:PilotScriptName):Bool
		return name == FullInventoryGift;

	public static inline function fullBerryStackCount():Int
		return Inventory.MAX_STACK;

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

	static function combatAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0: SelectSword;
			case 1 | 2 | 3: Strike;
			case _: Idle;
		};
	}

	static function recoveryAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0: SelectBerries;
			case 1: EatBerries;
			case _: Idle;
		};
	}

	static function fullInventoryAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0 | 1: Interact;
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
