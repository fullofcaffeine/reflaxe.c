package caxecraft.pilot;

import caxecraft.pilot.PilotScript.PilotScriptName;
import caxecraft.pilot.PilotCheckpoint.PilotScreenshot;

/** Resolve the optional screenshot owned by one completed compiled-pilot frame. */
function pilotScreenshotAt(name:PilotScriptName, frameNumber:Int):Null<PilotScreenshot> {
	final checkpoint = PilotScript.checkpoint(name, frameNumber);
	return checkpoint == null ? null : checkpoint.screenshot;
}

/**
 * Report whether a capture is the pilot's durable human-review frame.
 *
 * Intermediate editor captures diagnose individual workspaces. The environment
 * frame is the editor shell's review image; the later return frame only proves
 * that navigation reached its final state.
 */
function pilotScreenshotIsReview(screenshot:PilotScreenshot):Bool {
	return switch screenshot {
		case EditorTerrainPrompt | EditorTestPlay | EditorText | EditorAssets | EditorReturn: false;
		case _: true;
	};
}
