package caxecraft.qa;

import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.GameInputFrame.GameInputFrames;
import caxecraft.pilot.GameInputSource;
import caxecraft.pilot.PilotCheckpoint.PilotCheckpointKind;
import caxecraft.pilot.PilotScript;
import caxecraft.pilot.PilotScript.PilotScriptName;
import caxecraft.gameplay.Inventory;

/** Focused acceptance proof for the target-neutral game-pilot contract. */
final class PilotProbe {
	static function main():Void {
		var sampledFrames = 0;
		var checkpoints = 0;

		sampledFrames += checkBounded(PilotScriptName.LaunchSmoke, 4);
		sampledFrames += checkBounded(PilotScriptName.MoveJumpEdit, 14);
		sampledFrames += checkBounded(PilotScriptName.PauseRecapture, 7);
		sampledFrames += checkBounded(PilotScriptName.CombatDrop, 40);
		sampledFrames += checkBounded(PilotScriptName.RecoveryUse, 4);
		sampledFrames += checkBounded(PilotScriptName.FullInventoryGift, 4);
		sampledFrames += checkBounded(PilotScriptName.FullInventoryMining, 7);
		sampledFrames += checkBounded(PilotScriptName.ResizeLayout, 6);
		sampledFrames += checkBounded(PilotScriptName.AquaticGear, 96);
		sampledFrames += checkBounded(PilotScriptName.SmoothMotion, 12);
		checkpoints += checkLaunch();
		checkpoints += checkMovement();
		checkpoints += checkPause();
		checkpoints += checkCombat();
		checkpoints += checkRecovery();
		checkpoints += checkFullInventory();
		checkpoints += checkFullInventoryMining();
		checkpoints += checkResize();
		checkpoints += checkAquaticGear();
		checkpoints += checkSmoothMotion();
		checkSharedInterface();

		Sys.println('caxecraft-pilot: 10 named scripts, $sampledFrames deterministic frames, $checkpoints checkpoints; bounded quit and shared input interface');
	}

	static function checkBounded(name:PilotScriptName, expectedLimit:Int):Int {
		final stableName = PilotScript.stableName(name);
		require(PilotScript.frameLimit(name) == expectedLimit, '$stableName changed its frame limit');
		require(PilotScript.frameLimit(name) <= PilotScript.ABSOLUTE_FRAME_LIMIT, '$stableName exceeded the absolute limit');
		var frame = 0;
		while (frame < expectedLimit + 3) {
			final first = PilotScript.sample(name, frame);
			final second = PilotScript.sample(name, frame);
			require(GameInputFrames.same(first, second), '$stableName changed while sampling frame $frame twice');
			if (frame < expectedLimit - 1)
				require(!first.quitPressed && !PilotScript.complete(name, frame), '$stableName quit before its declared limit');
			else
				require(first.quitPressed && PilotScript.complete(name, frame), '$stableName did not remain safely quit');
			frame++;
		}
		return expectedLimit;
	}

	static function checkLaunch():Int {
		final name = PilotScriptName.LaunchSmoke;
		require(PilotScript.stableName(name) == "launch-smoke", "launch script lost its stable name");
		final checkpoint = PilotScript.checkpoint(name, 1);
		require(checkpoint != null && checkpoint.label == "launch.frame-1", "launch screenshot checkpoint changed");
		require(checkpoint != null && checkpoint.kind == CaptureScreenshot, "launch checkpoint stopped requesting a screenshot");
		require(PilotScript.checkpoint(name, 0) == null
			&& PilotScript.checkpoint(name, 2) == null, "launch script added an unexpected checkpoint");
		return 1;
	}

	static function checkMovement():Int {
		final name = PilotScriptName.MoveJumpEdit;
		require(PilotScript.sample(name, 0).moveForward == 1.0, "movement script lost forward input");
		require(PilotScript.sample(name, 1).lookYaw == -0.05, "movement script lost look input");
		require(PilotScript.sample(name, 2).jumpPressed, "movement script lost jump input");
		require(PilotScript.sample(name, 3).lookPitch == -0.25
			&& PilotScript.sample(name, 4).lookPitch == -0.25
			&& PilotScript.sample(name, 5).lookPitch == -0.25,
			"movement script lost its bounded ground aim");
		require(PilotScript.sample(name, 6).primaryPressed, "movement script lost its primary world action");
		require(PilotScript.sample(name, 7).hotbarCycle == 1, "movement script lost hotbar input");
		require(PilotScript.sample(name, 8).secondaryPressed, "movement script lost its secondary world action");
		require(PilotScript.sample(name, 9).interactPressed, "movement script lost guide interaction input");
		final state = PilotScript.checkpoint(name, 10);
		final screenshot = PilotScript.checkpoint(name, 12);
		require(state != null && state.kind == ObserveState && state.label == "move-jump-edit.state", "movement state checkpoint changed");
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "move-jump-edit.frame",
			"movement screenshot checkpoint changed");
		return 2;
	}

	static function checkPause():Int {
		final name = PilotScriptName.PauseRecapture;
		require(PilotScript.sample(name, 1).pausePressed, "pause script lost its pause action");
		require(PilotScript.sample(name, 3).capturePressed
			&& !PilotScript.sample(name, 3).primaryPressed, "recapture became a world action");
		final paused = PilotScript.checkpoint(name, 2);
		final captured = PilotScript.checkpoint(name, 4);
		require(paused != null && paused.kind == ObserveState && paused.label == "pause-recapture.paused", "paused checkpoint changed");
		require(captured != null
			&& captured.kind == ObserveState
			&& captured.label == "pause-recapture.captured", "recaptured checkpoint changed");
		return 2;
	}

	static function checkCombat():Int {
		final name = PilotScriptName.CombatDrop;
		require(PilotScript.sample(name, 28).hotbarSelection == 4, "combat script lost semantic sword selection");
		require(PilotScript.sample(name, 29).primaryPressed
			&& PilotScript.sample(name, 33).primaryPressed
			&& PilotScript.sample(name, 37).primaryPressed,
			"combat script lost its three fixed-step strikes");
		require(!PilotScript.sample(name, 30).primaryPressed, "combat script stopped respecting the sword cooldown");
		final screenshot = PilotScript.checkpoint(name, 38);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "combat-drop.frame",
			"combat drop screenshot checkpoint changed");
		return 1;
	}

	static function checkRecovery():Int {
		final name = PilotScriptName.RecoveryUse;
		require(PilotScript.initialHealth(name) == 4, "recovery fixture no longer starts one heart below full");
		require(PilotScript.sample(name, 0).hotbarSelection == 5, "recovery script lost semantic berry selection");
		require(PilotScript.sample(name, 1).secondaryPressed && !PilotScript.sample(name, 1).primaryPressed,
			"recovery script lost its selected-item use action");
		final screenshot = PilotScript.checkpoint(name, 2);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "recovery-use.frame",
			"recovery screenshot checkpoint changed");
		return 1;
	}

	static function checkFullInventory():Int {
		final name = PilotScriptName.FullInventoryGift;
		require(PilotScript.initialInventory(name).berries == Inventory.MAX_STACK, "full-inventory fixture lost the exact berry cap");
		require(PilotScript.sample(name, 0).interactPressed && PilotScript.sample(name, 1).interactPressed,
			"full-inventory script lost Nia's two dialogue actions");
		final screenshot = PilotScript.checkpoint(name, 2);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "full-inventory-gift.frame",
			"full-inventory screenshot checkpoint changed");
		return 1;
	}

	static function checkFullInventoryMining():Int {
		final name = PilotScriptName.FullInventoryMining;
		final inventory = PilotScript.initialInventory(name);
		require(inventory.grass == Inventory.MAX_STACK && inventory.dirt == Inventory.MAX_STACK && inventory.stone == Inventory.MAX_STACK,
			"full-mining fixture lost its exact block-stack caps");
		require(PilotScript.sample(name, 0).moveForward == 1.0
			&& PilotScript.sample(name, 2).jumpPressed
			&& PilotScript.sample(name, 4).primaryPressed,
			"full-mining script lost its approach or mining action");
		final screenshot = PilotScript.checkpoint(name, 5);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "full-inventory-mining.frame",
			"full-mining screenshot checkpoint changed");
		return 1;
	}

	static function checkResize():Int {
		final name = PilotScriptName.ResizeLayout;
		require(PilotScript.stableName(name) == "resize-layout", "resize script lost its stable name");
		require(PilotScript.requestedWindowWidth(name, 0) == 0
			&& PilotScript.requestedWindowHeight(name, 0) == 0, "resize script changed the initial window");
		require(PilotScript.requestedWindowWidth(name, 1) == 960 && PilotScript.requestedWindowHeight(name, 1) == 540,
			"resize script lost its logical viewport request");
		require(PilotScript.requestedWindowWidth(name, 2) == 0 && PilotScript.requestedWindowHeight(name, 2) == 0,
			"resize script repeated its one-shot request");
		final screenshot = PilotScript.checkpoint(name, 3);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "resize-layout.frame",
			"resize screenshot checkpoint changed");
		return 1;
	}

	static function checkAquaticGear():Int {
		final name = PilotScriptName.AquaticGear;
		require(PilotScript.stableName(name) == "aquatic-gear", "aquatic gear script lost its stable name");
		require(PilotScript.sample(name, 0).moveForward == 1.0 && PilotScript.sample(name, 0).moveRight == -1.0,
			"aquatic gear script lost its authored-item approach");
		require(PilotScript.sample(name, 92).moveForward == 0.0 && PilotScript.sample(name, 92).moveRight == 0.0,
			"aquatic gear script did not stop for its evidence frame");
		final screenshot = PilotScript.checkpoint(name, 92);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "aquatic-gear.frame",
			"aquatic gear screenshot checkpoint changed");
		return 1;
	}

	static function checkSmoothMotion():Int {
		final name = PilotScriptName.SmoothMotion;
		require(PilotScript.stableName(name) == "smooth-motion", "smooth-motion script lost its stable name");
		require(PilotScript.sample(name, 0).moveForward == 1.0 && PilotScript.sample(name, 10).moveForward == 1.0,
			"smooth-motion script lost ordinary forward input");
		require(PilotScript.sample(name, 8).jumpPressed, "smooth-motion script lost its fixed-boundary jump input");
		require(PilotScript.frameDurationMilliseconds(name, 0) == 8
			&& PilotScript.frameDurationMilliseconds(name, 2) == 17
			&& PilotScript.frameDurationMilliseconds(name, 4) == 25
			&& PilotScript.frameDurationMilliseconds(PilotScriptName.MoveJumpEdit, 0) == 50,
			"smooth-motion cadence or ordinary one-tick cadence changed");
		final screenshot = PilotScript.checkpoint(name, 10);
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "smooth-motion.frame",
			"smooth-motion screenshot checkpoint changed");
		return 1;
	}

	static function checkSharedInterface():Void {
		final realLike:GameInputSource = new FixedInputSource();
		require(realLike.sample(12).moveRight == -1.0, "real-input-shaped adapter did not satisfy the shared interface");
		require(!PilotScript.sample(PilotScriptName.LaunchSmoke, 0).quitPressed, "pilot did not produce shared input values");
	}

	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}

/** Test double with the same shape a future Raylib input adapter will use. */
private final class FixedInputSource implements GameInputSource {
	public function new() {}

	public function sample(_frameNumber:Int):GameInputFrame {
		return GameInputFrames.move(0.0, -1.0, 0.0, 0.0, false);
	}
}
