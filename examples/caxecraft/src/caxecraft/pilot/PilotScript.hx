package caxecraft.pilot;

import caxecraft.domain.Vitals.MAX_HEALTH;
import caxecraft.gameplay.Recovery.BERRY_HEALTH;
import caxecraft.gameplay.Inventory;
import caxecraft.gameplay.InventoryState;
import caxecraft.pilot.GameInputFrame.GameInputFrames;
import caxecraft.pilot.PilotCheckpoint.PilotCheckpointKind;
import caxecraft.pilot.PilotCatalog.pilotFrameLimit;
import caxecraft.pilot.PilotCatalog.pilotScriptCode;
import caxecraft.pilot.PilotCatalog.pilotStableName;

/** Closed, allocation-free names for deterministic game scripts. */
enum abstract PilotScriptName(Int) to Int {
	var LaunchSmoke = 0;
	var MoveJumpEdit = 1;
	var PauseRecapture = 2;
	var CombatDrop = 3;
	var RecoveryUse = 4;
	var FullInventoryGift = 5;
	var FullInventoryMining = 6;
	var ResizeLayout = 7;
	var AquaticGear = 8;
	var SmoothMotion = 9;
	var EditorShell = 10;
	var CampaignTravel = 11;
	var RuntimeContentJourney = 12;
}

/** One closed semantic action selected for a scripted frame. */
enum abstract PilotAction(Int) to Int {
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
	var ForwardLeft = 16;
	var LookDown = 17;
	var Travel = 18;
	var MenuNext = 19;
	var MenuConfirm = 20;
	var LookLeft = 21;
	var ForwardRise = 22;
	var Rise = 23;
	var Backward = 24;
	var Left = 25;
	var Right = 26;
	var TurnLeft = 27;
	var TurnRight = 28;
	var ForwardDescend = 29;
	var Descend = 30;
	var LookUp = 31;
	var Jump = 32;
	var BackwardJump = 33;
	var LeftJump = 34;
	var RightJump = 35;
	var LeftDescend = 36;
	var RightRise = 37;
}

/**
 * Deterministic input scripts with fixed observation points and bounded quit.
 *
 * All operations are static over a small name value. A native pilot therefore
 * adds neither a heap object nor interface dispatch to the game loop.
 */
final class PilotScript {
	public static inline final ABSOLUTE_FRAME_LIMIT:Int = 150;

	/** Stable numeric identity carried by native telemetry without a C string. */
	public static function scriptCode(name:PilotScriptName):Int
		return pilotScriptCode(name);

	/**
	 * Hash every scripted input and initial fixture fact for reproducible reports.
	 *
	 * The value is an identity for test input, not a security digest. Including
	 * the bounded action sequence, viewport requests, health, and complete
	 * inventory means a report cannot silently keep an old identity after one of
	 * those inputs changes.
	 */
	public static function inputHash(name:PilotScriptName):Int {
		var hash = mix(-2128831035, scriptCode(name));
		var frame = 0;
		while (frame < frameLimit(name)) {
			hash = mix(hash, actionCode(actionAt(name, frame)));
			hash = mix(hash, frameDurationMilliseconds(name, frame));
			hash = mix(hash, requestedWindowWidth(name, frame));
			hash = mix(hash, requestedWindowHeight(name, frame));
			frame++;
		}
		hash = mix(hash, initialHealth(name));
		final inventory = initialInventory(name);
		hash = mix(hash, inventory.selected);
		hash = mix(hash, inventory.grass);
		hash = mix(hash, inventory.dirt);
		hash = mix(hash, inventory.stone);
		hash = mix(hash, inventory.haxeforge);
		hash = mix(hash, inventory.sword);
		hash = mix(hash, inventory.berries);
		hash = mix(hash, inventory.bread);
		hash = mix(hash, inventory.lantern);
		return mix(hash, inventory.sand);
	}

	public static function frameLimit(name:PilotScriptName):Int
		return pilotFrameLimit(name);

	public static function stableName(name:PilotScriptName):String
		return pilotStableName(name);

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
		if (name == FullInventoryGift)
			return fullInventoryGiftAction(frameNumber);
		if (name == AquaticGear) {
			if (frameNumber < 88)
				return ForwardLeft;
			if (frameNumber < 94)
				return LookLeft;
			if (frameNumber == 94)
				return Mine;
			if (frameNumber < 130)
				return ForwardRise;
			return frameNumber < 146 ? Rise : Idle;
		}
		if (name == SmoothMotion)
			return frameNumber == 8 ? ForwardJump : Forward;
		if (name == EditorShell)
			return Idle;
		if (name == CampaignTravel)
			return frameNumber == 0 ? Travel : Idle;
		return fullInventoryMiningAction(frameNumber);
	}

	public static function sample(name:PilotScriptName, frameNumber:Int):GameInputFrame {
		final action = actionAt(name, frameNumber);
		return GameInputFrames.make(moveForward(action), moveRight(action), lookYaw(action), lookPitch(action), jumpPressed(action), primaryPressed(action),
			secondaryPressed(action), interactPressed(action), travelPressed(action), pausePressed(action), capturePressed(action), quitPressed(action),
			hotbarSelection(action), hotbarCycle(action), descendHeld(action), menuNextPressed(action), menuConfirmPressed(action), riseHeld(action));
	}

	public static inline function moveForward(action:PilotAction):Float
		return action == Backward
			|| action == BackwardJump ? -1.0 : action == Forward || action == ForwardTurn || action == ForwardJump || action == ForwardLeft
			|| action == ForwardRise || action == ForwardDescend ? 1.0 : 0.0;

	public static inline function moveRight(action:PilotAction):Float
		return action == RightLook
			|| action == Right
			|| action == RightJump
			|| action == RightRise ? 1.0 : action == ForwardLeft || action == Left || action == LeftJump || action == LeftDescend ? -1.0 : 0.0;

	public static inline function lookYaw(action:PilotAction):Float
		return action == LookLeft ? 0.25 : action == TurnLeft ? 0.05 : action == ForwardTurn || action == TurnRight ? -0.05 : 0.0;

	public static inline function lookPitch(action:PilotAction):Float
		return action == LookUp ? 0.25 : action == RightLook ? 0.04 : action == LookDown ? -0.25 : action == LookLeft ? -0.08 : 0.0;

	public static inline function jumpPressed(action:PilotAction):Bool
		return action == ForwardJump || action == BackwardJump || action == LeftJump || action == RightJump || action == Jump;

	/** Hold the same upward-swim intent supplied by interactive Space input. */
	public static inline function riseHeld(action:PilotAction):Bool
		return action == ForwardRise || action == RightRise || action == Rise;

	/** Hold the same downward-swim intent supplied by interactive Shift input. */
	public static inline function descendHeld(action:PilotAction):Bool
		return action == ForwardDescend || action == LeftDescend || action == Descend;

	public static inline function primaryPressed(action:PilotAction):Bool
		return action == Mine || action == Strike;

	public static inline function secondaryPressed(action:PilotAction):Bool
		return action == Place || action == EatBerries;

	public static inline function interactPressed(action:PilotAction):Bool
		return action == Interact;

	/** Request the campaign's sole authored way forward. */
	public static inline function travelPressed(action:PilotAction):Bool
		return action == Travel;

	/** Move to the neighboring game mode on the real title-menu path. */
	public static inline function menuNextPressed(action:PilotAction):Bool
		return action == MenuNext;

	/** Confirm the title menu's currently selected game mode. */
	public static inline function menuConfirmPressed(action:PilotAction):Bool
		return action == MenuConfirm;

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
				frameNumber == 38 ? new PilotCheckpoint("combat-drop.frame", CaptureScreenshot) : null;
			case RecoveryUse:
				frameNumber == 2 ? new PilotCheckpoint("recovery-use.frame", CaptureScreenshot) : null;
			case FullInventoryGift:
				frameNumber == 2 ? new PilotCheckpoint("full-inventory-gift.frame", CaptureScreenshot) : null;
			case FullInventoryMining:
				frameNumber == 5 ? new PilotCheckpoint("full-inventory-mining.frame", CaptureScreenshot) : null;
			case ResizeLayout:
				frameNumber == 3 ? new PilotCheckpoint("resize-layout.frame", CaptureScreenshot) : null;
			case AquaticGear:
				frameNumber == 146 ? new PilotCheckpoint("aquatic-gear.frame", CaptureScreenshot) : null;
			case SmoothMotion:
				frameNumber == 10 ? new PilotCheckpoint("smooth-motion.frame", CaptureScreenshot) : null;
			case EditorShell:
				frameNumber == 2 ? new PilotCheckpoint("editor-shell.frame", CaptureScreenshot) : null;
			case CampaignTravel:
				frameNumber == 3 ? new PilotCheckpoint("campaign-travel.frame", CaptureScreenshot) : null;
			case _: null;
		};
	}

	/**
	 * Deterministic outer-frame duration used only by native pilots.
	 *
	 * Ordinary pilots retain one exact 50 ms simulation step per frame. The
	 * smooth-motion script deliberately alternates common fast and slow display
	 * intervals; their accumulated time crosses fixed-tick boundaries with a
	 * remainder, giving the real renderer a position between committed states.
	 */
	public static function frameDurationMilliseconds(name:PilotScriptName, frameNumber:Int):Int {
		if (name != SmoothMotion)
			return 50;
		return switch frameNumber {
			case 0 | 1 | 3 | 5 | 7 | 9 | 11: 8;
			case 2 | 6 | 10: 17;
			case 4 | 8: 25;
			case _: 8;
		};
	}

	/**
	 * Requests a logical window width for a presentation test.
	 *
	 * Zero means "keep the current size". The application adapter performs the
	 * actual Raylib call, so this target-neutral script never imports a windowing
	 * API and release builds contain no scripted resize channel.
	 */
	public static inline function requestedWindowWidth(name:PilotScriptName, frameNumber:Int):Int
		return name == ResizeLayout && frameNumber == 1 ? 960 : 0;

	/** Logical height paired with `requestedWindowWidth`. */
	public static inline function requestedWindowHeight(name:PilotScriptName, frameNumber:Int):Int
		return name == ResizeLayout && frameNumber == 1 ? 540 : 0;

	/** Initial fixture health; ordinary and release paths begin at full health. */
	public static inline function initialHealth(name:PilotScriptName):Int
		return name == RecoveryUse ? MAX_HEALTH - BERRY_HEALTH : MAX_HEALTH;

	/**
	 * Initial state owned by a deterministic native pilot, never release play.
	 *
	 * Exact full stacks make rejection paths reproducible. The mining pilot fills
	 * all three collectable block stacks because terrain material is a world fact,
	 * not something an input script should duplicate or guess.
	 */
	public static function initialInventory(name:PilotScriptName):InventoryState {
		final starter = Inventory.starter();
		if (name == FullInventoryGift)
			return Inventory.make(starter.selected, starter.grass, starter.dirt, starter.stone, starter.haxeforge, starter.sword, Inventory.MAX_STACK,
				starter.bread, starter.lantern);
		if (name == FullInventoryMining)
			return Inventory.make(starter.selected, Inventory.MAX_STACK, Inventory.MAX_STACK, Inventory.MAX_STACK, starter.haxeforge, starter.sword,
				starter.berries, starter.bread, starter.lantern, Inventory.MAX_STACK);
		return starter;
	}

	public static inline function complete(name:PilotScriptName, frameNumber:Int):Bool
		return frameNumber >= frameLimit(name) - 1;

	/** Closed integer spelling used only by the versioned input hash. */
	static function actionCode(action:PilotAction):Int
		return action;

	/** FNV-style 32-bit mix shared by every input field. */
	static inline function mix(hash:Int, value:Int):Int
		return (hash ^ value) * 16777619;

	static function moveJumpAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0: Forward;
			case 1: ForwardTurn;
			case 2: ForwardJump;
			// Three bounded pitch steps put the ground inside the seven-block
			// interaction ray. The test therefore proves an edit happened instead
			// of merely sending mouse-button input while no block was selected.
			case 3 | 4 | 5: LookDown;
			case 6: Mine;
			case 7: SelectNext;
			case 8: Place;
			case 9: Interact;
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
			case 28: SelectSword;
			case 29 | 33 | 37: Strike;
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

	static function fullInventoryGiftAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0 | 1: Interact;
			case _: Idle;
		};
	}

	static function fullInventoryMiningAction(frameNumber:Int):PilotAction {
		return switch frameNumber {
			case 0: Forward;
			case 1: ForwardTurn;
			case 2: ForwardJump;
			case 3: RightLook;
			case 4: Mine;
			case _: Idle;
		};
	}

	static function moveJumpCheckpoint(frameNumber:Int):Null<PilotCheckpoint> {
		return switch frameNumber {
			case 10: new PilotCheckpoint("move-jump-edit.state", ObserveState);
			case 12: new PilotCheckpoint("move-jump-edit.frame", CaptureScreenshot);
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
