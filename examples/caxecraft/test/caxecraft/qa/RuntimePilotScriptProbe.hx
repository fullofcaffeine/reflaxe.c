package caxecraft.qa;

import caxecraft.pilot.PilotScript.PilotAction;
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
		final source = Bytes.ofString("PILOSCRIPT 1\n" + "name synthetic-journey\n" + "frames 6\n" + "action 0 menu-next\n" + "action 1 menu-confirm\n"
			+ "checkpoint 1 capture title-selection\n" + "expect 1 screen campaign\n" + "expect 1 level synthetic-level\n"
			+ "expect 1 objective objective.synthetic\n" + "expect 1 generation 2\n" + "expect 1 publications 1\n" + "end\n");
		final script = switch RuntimePilotScript.read(source, "synthetic.piloscript") {
			case RuntimePilotReady(value): value;
			case RuntimePilotRejected(diagnostic):
				throw 'valid synthetic Piloscript was rejected at ${diagnostic.line}: ${diagnostic.message}';
		};
		require(script.stableName() == "synthetic-journey", "the stable script name changed");
		require(script.frameLimit() == 6, "the frame limit changed");
		require(script.actionAt(0) == PilotAction.MenuNext, "the first action changed");
		require(script.actionAt(1) == PilotAction.MenuConfirm, "the second action changed");
		require(script.actionAt(2) == PilotAction.Idle, "an omitted frame did not stay idle");
		require(script.actionAt(5) == PilotAction.Quit && script.actionAt(9) == PilotAction.Quit, "the bounded quit rule changed");
		final checkpoint = script.checkpointAt(1);
		require(checkpoint != null && checkpoint.label == "title-selection", "the capture checkpoint changed");
		require(script.expectationCountAt(1) == 5, "the expectation count changed");
		require(script.expectationAt(1, 0).kind == RuntimePilotExpectationKind.Screen, "the first expectation kind changed");

		final matching:RuntimePilotObservation = {
			screen: "campaign",
			mode: "adventure",
			level: "synthetic-level",
			objective: "objective.synthetic",
			generation: 2,
			publications: 1
		};
		switch script.observe(1, matching) {
			case RuntimePilotFrameAccepted:
			case RuntimePilotFrameRejected(_):
				throw "matching semantic state was rejected";
		}
		final wrong:RuntimePilotObservation = {
			screen: "campaign",
			mode: "adventure",
			level: "wrong-level",
			objective: "objective.synthetic",
			generation: 2,
			publications: 1
		};
		switch script.observe(1, wrong) {
			case RuntimePilotFrameRejected(diagnostic):
				require(diagnostic.line == 8, "the mismatch lost its expectation line");
				require(diagnostic.message.indexOf("synthetic-level") >= 0, "the mismatch lost its independent expectation");
			case RuntimePilotFrameAccepted:
				throw "wrong semantic state passed";
		}

		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\naction 1 unknown\nend\n", 4, "unknown action");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 151\nend\n", 3, "frame limit");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\ncheckpoint 1 capture ../escape\nend\n", 4, "capture label");
		expectRejected("PILOSCRIPT 1\nname bad\nframes 3\naction 0 idle\naction 0 menu-next\nend\n", 5, "duplicate action");
		return 0;
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
