package caxecraft.pilot;

import caxecraft.pilot.PilotScript.PilotScriptName;

/**
 * Defines the host-visible identity of every admitted native game pilot.
 *
 * Gameplay actions and checkpoints remain in `PilotScript` or a runtime
 * Piloscript file. This module adds only the metadata needed before launch:
 * the command name, telemetry code, frame bound, compile define, and review
 * image name. The Haxe exporter writes these records to `pilot-catalog.json`,
 * so host tooling can consume them without maintaining a second registry.
 */
/** Distinguishes a compiled engine probe from a reloadable content journey. */
enum abstract PilotExecutionKind(String) to String {
	/** The executable receives actions compiled from `PilotScript`. */
	var Compiled = "compiled";

	/** The executable loads actions and expectations from runtime content. */
	var RuntimeContent = "runtime-content";
}

/** One complete, immutable host contract derived from Haxe-owned pilot facts. */
typedef PilotMetadata = {
	/** Engine script that supplies actions or the runtime-content adapter. */
	final script:PilotScriptName;

	/** Stable command-line name accepted by the host runner. */
	final id:String;

	/** Closed integer identity written into native telemetry. */
	final scriptCode:Int;

	/** Exact compiled length or maximum runtime-content length. */
	final frameLimit:Int;

	/** Compile-time Haxe define that selects this pilot adapter. */
	final haxeDefine:String;

	/** Basename of the human-review framebuffer capture. */
	final screenshot:String;

	/** Whether actions and expectations are loaded after the executable starts. */
	final execution:PilotExecutionKind;
}

/**
 * Return the complete host catalog in stable command-line order.
 *
 * Standard compiled names, codes, and frame bounds come directly from
 * `PilotScript`. `secondary-locale` deliberately reuses the launch actions,
 * while `adventure-journey` selects the generic runtime-content adapter.
 */
function allPilotMetadata():Array<PilotMetadata> {
	return [
		compiled(PilotScriptName.LaunchSmoke, "caxecraft_pilot_launch_smoke", "caxecraft-smoke.png"),
		compiled(PilotScriptName.LaunchSmoke, "caxecraft_pilot_secondary_locale", "caxecraft-secondary-locale.png", "secondary-locale"),
		compiled(PilotScriptName.MoveJumpEdit, "caxecraft_pilot_move_jump_edit", "caxecraft-pilot-move.png"),
		compiled(PilotScriptName.PauseRecapture, "caxecraft_pilot_pause_recapture", "caxecraft-pilot-pause.png"),
		compiled(PilotScriptName.CombatDrop, "caxecraft_pilot_combat_drop", "caxecraft-pilot-combat.png"),
		compiled(PilotScriptName.RecoveryUse, "caxecraft_pilot_recovery_use", "caxecraft-pilot-recovery.png"),
		compiled(PilotScriptName.FullInventoryMining, "caxecraft_pilot_full_inventory_mining", "caxecraft-pilot-full-mining.png"),
		compiled(PilotScriptName.ResizeLayout, "caxecraft_pilot_resize_layout", "caxecraft-pilot-resize.png"),
		compiled(PilotScriptName.AquaticGear, "caxecraft_pilot_aquatic_gear", "caxecraft-pilot-aquatic-gear.png"),
		compiled(PilotScriptName.SmoothMotion, "caxecraft_pilot_smooth_motion", "caxecraft-pilot-smooth-motion.png"),
		compiled(PilotScriptName.EditorShell, "caxecraft_pilot_editor_shell", "caxecraft-pilot-editor.png"),
		compiled(PilotScriptName.CampaignTravel, "caxecraft_pilot_campaign_travel", "caxecraft-pilot-campaign-travel.png"),
		{
			script: PilotScriptName.RuntimeContentJourney,
			id: "adventure-journey",
			scriptCode: pilotScriptCode(PilotScriptName.RuntimeContentJourney),
			frameLimit: RuntimePilotScript.ABSOLUTE_FRAME_LIMIT,
			haxeDefine: "caxecraft_pilot_runtime",
			screenshot: "caxecraft-pilot-runtime-final.png",
			execution: RuntimeContent
		}
	];
}

/** Map a script to the stable integer carried by native telemetry. */
function pilotScriptCode(script:PilotScriptName):Int {
	return switch script {
		case LaunchSmoke: 0;
		case MoveJumpEdit: 1;
		case PauseRecapture: 2;
		case CombatDrop: 3;
		case RecoveryUse: 4;
		case FullInventoryMining: 6;
		case ResizeLayout: 7;
		case AquaticGear: 8;
		case SmoothMotion: 9;
		case EditorShell: 10;
		case CampaignTravel: 11;
		case RuntimeContentJourney: 12;
	};
}

/** Return the exact compiled length of one engine-regression script. */
function pilotFrameLimit(script:PilotScriptName):Int {
	return switch script {
		case LaunchSmoke | RecoveryUse | EditorShell: 4;
		case MoveJumpEdit: 14;
		case PauseRecapture | FullInventoryMining: 7;
		case CombatDrop: 40;
		case ResizeLayout: 6;
		case AquaticGear: 150;
		case SmoothMotion: 12;
		case CampaignTravel: 5;
		case RuntimeContentJourney: RuntimePilotScript.ABSOLUTE_FRAME_LIMIT;
	};
}

/** Return the stable engine-script name used in traces and reports. */
function pilotStableName(script:PilotScriptName):String {
	return switch script {
		case LaunchSmoke: "launch-smoke";
		case MoveJumpEdit: "move-jump-edit";
		case PauseRecapture: "pause-recapture";
		case CombatDrop: "combat-drop";
		case RecoveryUse: "recovery-use";
		case FullInventoryMining: "full-inventory-mining";
		case ResizeLayout: "resize-layout";
		case AquaticGear: "aquatic-gear";
		case SmoothMotion: "smooth-motion";
		case EditorShell: "editor-shell";
		case CampaignTravel: "campaign-travel";
		case RuntimeContentJourney: "runtime-content-journey";
	};
}

/** Build one compiled row while deriving every script-owned fact. */
private function compiled(script:PilotScriptName, haxeDefine:String, screenshot:String, ?alias:String):PilotMetadata {
	return {
		script: script,
		id: alias == null ? pilotStableName(script) : alias,
		scriptCode: pilotScriptCode(script),
		frameLimit: pilotFrameLimit(script),
		haxeDefine: haxeDefine,
		screenshot: screenshot,
		execution: Compiled
	};
}
