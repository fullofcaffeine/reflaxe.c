import haxe.macro.Expr;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlan;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlanningResult;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowWorkReport;
import reflaxe.c.lowering.CBodyControlFlowPlanCache;

/**
	Exercises the warm control-flow cache inside Haxe's macro process.

	The production cache is process-persistent only while haxe.c runs as a macro,
	so an ordinary Eval unit test cannot observe its real storage class. This
	probe uses tiny target-owned plans to isolate key comparison, replacement,
	abort, and disabled behavior without invoking C emission.
**/
class ControlFlowPlanCacheProbe {
	/**
		Require exact schema, function, and text identity before reuse.

		The result is a compile-time expression only because Haxe's `--macro`
		hook requires one; all assertions run while the compiler loads the test.
	**/
	public static macro function run():Expr {
		var builds = 0;
		function build(label:String):CBodyControlFlowPlanningResult {
			builds++;
			return new CBodyControlFlowPlanningResult(CBodyControlFlowPlan.CCFLegacyIrreducible([label]), CBodyControlFlowWorkReport.zero());
		}

		CBodyControlFlowPlanCache.beginRequest(true);
		final first = CBodyControlFlowPlanCache.resolve("function.demo", "hxcir schema=23\nfunction demo\n", () -> build("first"));
		require(!first.reused && builds == 1, "first request must build one plan");
		final firstStats = CBodyControlFlowPlanCache.completeRequest();
		require(firstStats.hits == 0 && firstStats.misses == 1 && firstStats.retainedFunctions == 1, "first request accounting drifted");

		CBodyControlFlowPlanCache.beginRequest(true);
		final identical = CBodyControlFlowPlanCache.resolve("function.demo", "hxcir schema=23\nfunction demo\n", () -> build("unexpected"));
		require(identical.reused && builds == 1, "byte-identical function did not reuse its plan");
		CBodyControlFlowPlanCache.completeRequest();

		CBodyControlFlowPlanCache.beginRequest(true);
		final changedSchema = CBodyControlFlowPlanCache.resolve("function.demo", "hxcir schema=21\nfunction demo\n", () -> build("schema"));
		require(!changedSchema.reused && builds == 2, "changed HxcIR schema reused stale evidence");
		CBodyControlFlowPlanCache.completeRequest();

		CBodyControlFlowPlanCache.beginRequest(true);
		final changedFunction = CBodyControlFlowPlanCache.resolve("function.other", "hxcir schema=23\nfunction demo\n", () -> build("function"));
		require(!changedFunction.reused && builds == 3, "changed function identity reused stale evidence");
		CBodyControlFlowPlanCache.completeRequest();

		CBodyControlFlowPlanCache.beginRequest(true);
		final abortedText = CBodyControlFlowPlanCache.resolve("function.other", "hxcir schema=23\nfunction changed\n", () -> build("text"));
		require(!abortedText.reused && builds == 4, "changed function text reused stale evidence");
		CBodyControlFlowPlanCache.abortRequest();

		CBodyControlFlowPlanCache.beginRequest(true);
		final afterAbort = CBodyControlFlowPlanCache.resolve("function.other", "hxcir schema=23\nfunction demo\n", () -> build("unexpected"));
		require(afterAbort.reused && builds == 4, "aborted request replaced the prior successful generation");
		CBodyControlFlowPlanCache.completeRequest();

		CBodyControlFlowPlanCache.beginRequest(false);
		final disabled = CBodyControlFlowPlanCache.resolve("function.other", "hxcir schema=23\nfunction demo\n", () -> build("disabled"));
		require(!disabled.reused && builds == 5, "disabled cache skipped authoritative planning");
		final disabledStats = CBodyControlFlowPlanCache.completeRequest();
		require(!disabledStats.enabled && disabledStats.hits == 0 && disabledStats.misses == 0 && disabledStats.retainedFunctions == 1,
			"disabled request changed retained evidence or reported a false cache decision");

		return macro null;
	}

	/** Stop compilation at the first cache-contract violation. */
	static function require(condition:Bool, detail:String):Void {
		if (!condition)
			throw new haxe.Exception(detail);
	}
}
