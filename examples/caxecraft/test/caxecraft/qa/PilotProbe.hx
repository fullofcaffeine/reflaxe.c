package caxecraft.qa;

import caxecraft.pilot.GameInputFrame;
import caxecraft.pilot.GameInputFrame.GameInputFrames;
import caxecraft.pilot.GameInputSource;
import caxecraft.pilot.PilotCheckpoint.PilotCheckpointKind;
import caxecraft.pilot.PilotScript;
import caxecraft.pilot.PilotScript.PilotScriptName;

/** Focused acceptance proof for the target-neutral game-pilot contract. */
final class PilotProbe {
	static function main():Void {
		var sampledFrames = 0;
		var checkpoints = 0;

		sampledFrames += checkBounded(PilotScriptName.LaunchSmoke, 4);
		sampledFrames += checkBounded(PilotScriptName.MoveJumpEdit, 10);
		sampledFrames += checkBounded(PilotScriptName.PauseRecapture, 7);
		checkpoints += checkLaunch();
		checkpoints += checkMovement();
		checkpoints += checkPause();
		checkSharedInterface();

		Sys.println('caxecraft-pilot: 3 named scripts, $sampledFrames deterministic frames, $checkpoints checkpoints; bounded quit and shared input interface');
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
		require(PilotScript.sample(name, 3).moveRight == 1.0
			&& PilotScript.sample(name, 3).lookPitch == 0.04, "movement script lost strafe/look input");
		require(PilotScript.sample(name, 4).minePressed, "movement script lost mine input");
		require(PilotScript.sample(name, 5).placePressed, "movement script lost place input");
		require(PilotScript.sample(name, 6).hotbarCycle == 1, "movement script lost hotbar input");
		require(PilotScript.sample(name, 7).interactPressed, "movement script lost guide interaction input");
		final state = PilotScript.checkpoint(name, 7);
		final screenshot = PilotScript.checkpoint(name, 8);
		require(state != null && state.kind == ObserveState && state.label == "move-jump-edit.state", "movement state checkpoint changed");
		require(screenshot != null && screenshot.kind == CaptureScreenshot && screenshot.label == "move-jump-edit.frame",
			"movement screenshot checkpoint changed");
		return 2;
	}

	static function checkPause():Int {
		final name = PilotScriptName.PauseRecapture;
		require(PilotScript.sample(name, 1).pausePressed, "pause script lost its pause action");
		require(PilotScript.sample(name, 3).capturePressed
			&& !PilotScript.sample(name, 3).minePressed, "recapture became a mining action");
		final paused = PilotScript.checkpoint(name, 2);
		final captured = PilotScript.checkpoint(name, 4);
		require(paused != null && paused.kind == ObserveState && paused.label == "pause-recapture.paused", "paused checkpoint changed");
		require(captured != null
			&& captured.kind == ObserveState
			&& captured.label == "pause-recapture.captured", "recaptured checkpoint changed");
		return 2;
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
