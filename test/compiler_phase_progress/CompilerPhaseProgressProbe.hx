package;

#if macro
import haxe.macro.Expr;
import reflaxe.c.CPhaseTiming;
import reflaxe.c.CPhaseTiming.CPhaseTimingId;

/**
	Exercises successful and failed compiler progress lifecycles without a target build.

	The real target calls the same request and phase methods. This focused macro
	keeps the diagnostic schema test fast while preserving exact nesting order.
**/
class CompilerPhaseProgressProbe {
	/** Emit one complete request followed by one request aborted inside a phase. */
	public static function install():Expr {
		CPhaseTiming.beginRequest();
		final capture = CPhaseTiming.start(CPTypedInputCapture);
		CPhaseTiming.stop(capture);
		final target = CPhaseTiming.start(CPTargetPipeline);
		CPhaseTiming.describeRequest("portable", "debug");
		final configuration = CPhaseTiming.start(CPConfigurationAndContracts);
		CPhaseTiming.stop(configuration);
		CPhaseTiming.stop(target);
		CPhaseTiming.finishRequest();

		CPhaseTiming.beginRequest();
		CPhaseTiming.start(CPTargetPipeline);
		CPhaseTiming.abortRequest("expected-failure");
		return macro null;
	}
}
#end
