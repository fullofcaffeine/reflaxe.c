package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Context;

/** Closed names for the compiler phases exposed by the opt-in profiler. */
enum abstract CPhaseTimingId(String) to String {
	var CPTypedInputCapture = "typed input capture";
	var CPTargetPipeline = "target pipeline";
	var CPConfigurationAndContracts = "configuration and contracts";
	var CPWholeProgramAnalysis = "whole program analysis";
	var CPSemanticLowering = "semantic lowering";
	var CPHxcIRConstruction = "HxcIR construction";
	var CPHxcIRValidation = "HxcIR validation";
	var CPSemanticAnalysesAndNaming = "semantic analyses and naming";
	var CPCASTBodyConstruction = "CAST body construction";
	var CPOptionalReports = "optional reports";
	var CPRuntimePlanning = "runtime planning";
	var CPCASTProjectPlanning = "CAST project planning";
	var CPCPrinting = "C printing";
	var CPArtifactPlanning = "artifact planning";
	var CPOutputOwnership = "output ownership";
}

/**
	One request-local phase clock. `@:noCompletion` keeps this compiler-only
	utility out of user-facing autocomplete; it does not change visibility or
	runtime behavior.
**/
@:noCompletion
class CPhaseTimer {
	final id:CPhaseTimingId;
	final startedSeconds:Float;
	final stopCompilerTimer:() -> Void;
	var stopped:Bool;

	public function new(id:CPhaseTimingId) {
		this.id = id;
		this.startedSeconds = Sys.time();
		this.stopCompilerTimer = Context.timer("hxc " + Std.string(id));
		this.stopped = false;
	}

	public function stop():Void {
		if (stopped)
			throw 'compiler phase timer `${Std.string(id)}` was stopped twice';
		stopped = true;
		stopCompilerTimer();
		final elapsedMicroseconds = Math.round(Math.max(0.0, Sys.time() - startedSeconds) * 1000000.0);
		Sys.println(CPhaseTiming.REPORT_PREFIX + Std.string(id) + "\t" + Std.string(elapsedMicroseconds));
	}
}

/**
	Opt-in wall-clock timing for the real C compiler pipeline.

	Normal builds create no timers and print nothing. The Caxecraft profiler
	enables `reflaxe_c_phase_timing` together with Haxe's `--times`: the custom
	records retain accurate wall time for every named target phase, while Haxe's
	built-in report supplies parse/type/cache context around those phases.
**/
@:noCompletion
class CPhaseTiming {
	public static inline final DEFINE = "reflaxe_c_phase_timing";
	public static inline final REPORT_PREFIX = "HXC_PHASE_TIMING\t";

	public static function start(id:CPhaseTimingId):Null<CPhaseTimer> {
		return Context.defined(DEFINE) ? new CPhaseTimer(id) : null;
	}

	public static function stop(timer:Null<CPhaseTimer>):Void {
		if (timer != null)
			timer.stop();
	}
}
#else

/** Compiler phase timing exists only while the compiler macro is running. */
@:noCompletion
class CPhaseTiming {}
#end
