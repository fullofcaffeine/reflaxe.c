package caxecraft.qa;

import caxecraft.pilot.PilotScript.PilotAction;
import caxecraft.pilot.PilotScript;
import caxecraft.pilot.AgentWorldObservation.renderAgentWorldObservation;
import caxecraft.pilot.RuntimePilotScript;
import caxecraft.pilot.RuntimePilotScript.RuntimePilotExpectationKind;
import caxecraft.pilot.RuntimePilotScript.RuntimePilotObservation;
import caxecraft.pilot.RuntimePilotScript.RuntimePilotReadResult;
import caxecraft.pilot.RuntimePilotScript.RuntimePilotRunResult;
import haxe.io.Bytes;

/**
 * Proves the generic parser and observer for reloadable content journeys.
 *
 * The fixture uses synthetic names, so this test cannot teach the runner about
 * a shipped campaign. Eval and generated C must accept the same valid bytes and
 * reject malformed bytes with the same source line.
 */
final class RuntimePilotScriptProbe {
	/** Native harness result. Zero means that all Haxe-owned checks passed. */
	@:expose("hxc_caxecraft_qa_RuntimePilotScriptProbe_observed")
	public static var observed:Int = -1;

	/** Run the complete focused contract under Eval or generated native C. */
	static function main():Void {
		observed = runChecks();
		#if eval
		Sys.println(observed);
		#end
	}

	/** Check parsing, bounds, generic observations, and located rejection. */
	static function runChecks():Int {
		checkAgentObservationEnvelope();
		final source = Bytes.ofString("PILOSCRIPT 1\n" + "name synthetic-journey\n" + "frames 8\n" + "action 0 menu-next\n" + "action 1 menu-confirm\n"
			+ "hold 2 4 forward\n" + "checkpoint 1 capture title-selection\n" + "expect 1 screen campaign\n" + "expect 1 level synthetic-level\n"
			+ "expect 1 objective objective.synthetic\n" + "expect 1 dialogue dialogue.synthetic\n" + "expect 1 journal journal.synthetic\n"
			+ "expect 1 generation 2\n" + "expect 1 publications 1\n" + "action 5 forward-descend\n" + "expect 5 medium submerged\n"
			+ "expect 5 equipment synthetic-gear\n" + "expect 5 lanterns 2\n" + "expect 5 sand 1\n" + "expect 5 position 4,3,2\n"
			+ "expect 5 object-state synthetic.gate=synthetic:open\n" + "action 6 toggle-camera\n" + "end\n");
		final script = switch RuntimePilotScript.read(source, "synthetic.piloscript") {
			case RuntimePilotReady(value): value;
			case RuntimePilotRejected(diagnostic):
				throw 'valid synthetic Piloscript was rejected at ${diagnostic.line}: ${diagnostic.message}';
		};
		require(script.stableName() == "synthetic-journey", "the stable script name changed");
		require(script.frameLimit() == 8, "the frame limit changed");
		require(script.actionAt(0) == PilotAction.MenuNext, "the first action changed");
		require(script.actionAt(1) == PilotAction.MenuConfirm, "the second action changed");
		require(script.actionAt(2) == PilotAction.Forward && script.actionAt(4) == PilotAction.Forward, "the held action range changed");
		require(script.actionAt(5) == PilotAction.ForwardDescend, "the downward-swim action changed");
		require(script.actionAt(6) == PilotAction.ToggleCamera
			&& PilotScript.cameraTogglePressed(script.actionAt(6)), "the camera-toggle action changed");
		require(script.actionAt(7) == PilotAction.Quit && script.actionAt(9) == PilotAction.Quit, "the bounded quit rule changed");
		final checkpoint = script.checkpointAt(1);
		require(checkpoint != null && checkpoint.label == "title-selection", "the capture checkpoint changed");
		require(script.expectationCountAt(1) == 7, "the expectation count changed");
		require(script.expectationAt(1, 0).kind == RuntimePilotExpectationKind.Screen, "the first expectation kind changed");

		final matching:RuntimePilotObservation = {
			screen: "campaign",
			mode: "adventure",
			level: "synthetic-level",
			objective: "objective.synthetic",
			dialogue: "dialogue.synthetic",
			journal: "journal.synthetic",
			generation: 2,
			publications: 1,
			cellX: 4,
			cellY: 3,
			cellZ: 2,
			aquaticMedium: "submerged",
			aquaticEquipment: "synthetic-gear",
			lanterns: 2,
			sand: 1,
			statefulObjectIds: ["synthetic.gate"],
			statefulObjectStates: ["synthetic:open"]
		};
		switch script.observe(1, matching) {
			case RuntimePilotFrameAccepted:
			case RuntimePilotFrameRejected(_):
				throw "matching semantic state was rejected";
		}
		switch script.observe(5, matching) {
			case RuntimePilotFrameAccepted:
			case RuntimePilotFrameRejected(_):
				throw "matching generic object state was rejected";
		}
		final wrong:RuntimePilotObservation = {
			screen: "campaign",
			mode: "adventure",
			level: "wrong-level",
			objective: "objective.synthetic",
			dialogue: "dialogue.synthetic",
			journal: "journal.synthetic",
			generation: 2,
			publications: 1,
			cellX: 4,
			cellY: 3,
			cellZ: 2,
			aquaticMedium: "submerged",
			aquaticEquipment: "synthetic-gear",
			lanterns: 2,
			sand: 1,
			statefulObjectIds: ["synthetic.gate"],
			statefulObjectStates: ["synthetic:sealed"]
		};
		switch script.observe(1, wrong) {
			case RuntimePilotFrameRejected(diagnostic):
				require(diagnostic.line == 9, "the mismatch lost its expectation line");
				require(diagnostic.message.indexOf("synthetic-level") >= 0, "the mismatch lost its independent expectation");
			case RuntimePilotFrameAccepted:
				throw "wrong semantic state passed";
		}

		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\naction 1 unknown\nend\n", 4, "unknown action");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 501\nend\n", 3, "frame limit");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\ncheckpoint 1 capture ../escape\nend\n", 4, "capture label");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\naction 0 idle\naction 0 menu-next\nend\n", 5, "duplicate action");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 4\nhold 1 3 forward\nend\n", 4, "held final frame");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 5\nhold 1 3 forward\naction 2 idle\nend\n", 5, "overlapping action");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\nexpect 1 object-state missing-pair\nend\n", 4, "object-state pair");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\ninspect-radius 7\nend\n", 4, "inspect radius");
		final explicitQuit = switch RuntimePilotScript.read(Bytes.ofString("PILOSCRIPT 1\nname live-session\nframes 2\ninspect-radius 5\naction 0 quit\nend\n"),
			"live-session.piloscript") {
			case RuntimePilotReady(value): value;
			case RuntimePilotRejected(diagnostic):
				throw 'explicit quit was rejected at ${diagnostic.line}: ${diagnostic.message}';
		};
		require(explicitQuit.actionAt(0) == PilotAction.Quit, "the explicit live-session quit action changed");
		require(explicitQuit.inspectionRadius() == 5, "the bounded live-session inspection radius changed");
		return 0;
	}

	/** Prove that checkpoint and terminal purpose survives the native JSON boundary. */
	static function checkAgentObservationEnvelope():Void {
		final rendered = renderAgentWorldObservation({
			sequence: 7,
			terminal: false,
			frame: 11,
			tick: 13,
			screen: "game",
			mode: "adventure",
			level: "synthetic-level",
			objective: "none",
			dialogue: "none",
			journal: "none",
			interaction: "none",
			aquaticMedium: "dry",
			aquaticEquipment: "none",
			position: {
				xMilli: 0,
				yMilli: 1000,
				zMilli: 2000,
				cellX: 0,
				cellY: 1,
				cellZ: 2
			},
			heading: {xMilli: 0, yMilli: 0, zMilli: 1000},
			vitals: {
				health: 10,
				safeTicks: 0,
				breathTicks: 20,
				maximumBreathTicks: 20
			},
			inventory: [],
			target: {
				hit: false,
				material: "air",
				cellX: 0,
				cellY: 0,
				cellZ: 0,
				distanceMilli: 0
			},
			nearby: [],
			terrainRadius: 0,
			terrain: [],
			events: [],
			screenshot: "caxecraft-pilot-runtime-final.png"
		});
		require(rendered.indexOf('"sequence":7,"terminal":false,"frame":11') >= 0, "agent checkpoint JSON lost its ordered nonterminal marker");
	}

	/** Require one malformed source to fail at the manually authored line. */
	static function expectRejected(source:String, line:Int, fragment:String):Void {
		switch RuntimePilotScript.read(Bytes.ofString(source), "malformed.piloscript") {
			case RuntimePilotReady(_):
				throw 'malformed Piloscript passed: $fragment';
			case RuntimePilotRejected(diagnostic):
				require(diagnostic.line == line, '$fragment reported line ${diagnostic.line} instead of $line');
		}
	}

	/** Stop at the first contract error with a useful Eval diagnostic. */
	static inline function require(condition:Bool, message:String):Void {
		if (!condition)
			throw message;
	}
}
