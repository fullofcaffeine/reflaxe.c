package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import reflaxe.c.CPhaseTiming;
import reflaxe.c.CPhaseTiming.CProfileCounterId;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlanningResult;

/**
	Reuses a validated structural control-flow plan for byte-identical HxcIR.

	Planning a large function's reducible branches and loops is deliberately
	careful, but a warm Haxe compiler request often supplies the exact same
	validated semantic function again. This cache keeps only the previous
	successful request's canonical HxcIR text and target-owned planning result.
	It never retains Haxe `Type`, `TypedExpr`, `Position`, compiler context,
	generated C, or a checkout path.

	A cache hit is safe because `CBodyControlFlowPlanner` already validated the
	plan against the same complete canonical function text before publication.
	The active request still builds and validates its current HxcIR program; a
	different source span, instruction, block, cleanup edge, or type changes the
	text and forces ordinary planning.
**/
@:noCompletion
class CBodyControlFlowPlanCache {
	/** Bump this when the cached plan contract or canonical key changes. */
	public static inline final SCHEMA_VERSION = 1;

	/** Disable reuse while retaining the authoritative planning path. */
	public static inline final DISABLE_DEFINE = "reflaxe_c_test_disable_control_flow_plan_cache";

	/** Opt-in machine-readable evidence for compiler-server regression tests. */
	public static inline final REPORT_DEFINE = "reflaxe_c_control_flow_plan_cache_report";

	/** Stable prefix separating the cache record from ordinary compiler output. */
	public static inline final REPORT_PREFIX = "HXC_CONTROL_FLOW_PLAN_CACHE=";

	#if (eval && macro)
	@:persistent
	#end
	static var previousGeneration:Array<PersistedControlFlowPlan> = [];

	static var activeRequest:Null<ControlFlowPlanCacheRequest> = null;

	/**
		Start a fresh request without changing the last successful generation.

		The lookup is rebuilt request-locally. A failed compilation can therefore
		discard its candidate while a later success still compares against the
		last complete output transaction.
	**/
	public static function beginRequest(enabled:Bool):Void {
		final priorByFunction:Map<String, PersistedControlFlowPlan> = [];
		if (enabled) {
			for (entry in previousGeneration)
				priorByFunction.set(entry.functionId, entry);
		}
		activeRequest = {
			enabled: enabled,
			priorByFunction: priorByFunction,
			candidateByFunction: [],
			hits: 0,
			misses: 0
		};
	}

	/** Whether this request needs canonical per-function text for lookup/publication. */
	public static function needsFunctionKeys():Bool {
		final request = activeRequest;
		return request != null && request.enabled;
	}

	/**
		Return prior validated evidence or build the current function's plan.

		`canonicalFunction` is exact text from `HxcIRDumper`, not a lossy hash.
		Comparing the complete text makes hash collisions irrelevant and includes
		every semantic child and stable source span owned by the function.
	**/
	public static function resolve(functionId:String, canonicalFunction:String, build:() -> CBodyControlFlowPlanningResult):CBodyControlFlowPlanResolution {
		final request = activeRequest;
		if (request == null || !request.enabled)
			return new CBodyControlFlowPlanResolution(build(), false);
		if (functionId == "" || canonicalFunction == "")
			throw new haxe.Exception("control-flow plan cache requires a function identity and canonical HxcIR text");
		if (request.candidateByFunction.exists(functionId))
			throw new haxe.Exception('control-flow plan cache received duplicate function `$functionId`');

		final prior = request.priorByFunction.get(functionId);
		final reused = prior != null && prior.schemaVersion == SCHEMA_VERSION && prior.canonicalFunction == canonicalFunction;
		final planning = if (reused) {
			request.hits++;
			prior.planning;
		} else {
			request.misses++;
			build();
		};
		request.candidateByFunction.set(functionId, {
			schemaVersion: SCHEMA_VERSION,
			functionId: functionId,
			canonicalFunction: canonicalFunction,
			planning: planning
		});
		return new CBodyControlFlowPlanResolution(planning, reused);
	}

	/**
		Publish the one complete candidate after generated-output ownership succeeds.

		Replacing instead of appending bounds process memory to one program
		generation. Canonical ordering keeps diagnostics and profile evidence
		independent of map iteration.
	**/
	public static function completeRequest():CBodyControlFlowPlanCacheStats {
		final request = activeRequest;
		if (request == null)
			throw new haxe.Exception("control-flow plan cache completed without an active request");
		if (request.enabled) {
			final replacement = [for (entry in request.candidateByFunction) entry];
			replacement.sort((left, right) -> compareStrings(left.functionId, right.functionId));
			previousGeneration = replacement;
		}
		var retainedKeyCodeUnits = 0.0;
		for (entry in previousGeneration)
			retainedKeyCodeUnits += entry.canonicalFunction.length;
		final stats:CBodyControlFlowPlanCacheStats = {
			enabled: request.enabled,
			hits: request.hits,
			misses: request.misses,
			retainedFunctions: previousGeneration.length,
			retainedKeyCodeUnits: retainedKeyCodeUnits
		};
		activeRequest = null;
		CPhaseTiming.setCounter(CPCounterControlFlowPlanCacheHits, stats.hits);
		CPhaseTiming.setCounter(CPCounterControlFlowPlanCacheMisses, stats.misses);
		CPhaseTiming.setCounter(CPCounterControlFlowPlanCacheRetainedFunctions, stats.retainedFunctions);
		CPhaseTiming.setCounterFloat(CPCounterControlFlowPlanCacheRetainedKeyCodeUnits, stats.retainedKeyCodeUnits);
		return stats;
	}

	/** Discard only the unfinished request; prior successful evidence remains. */
	public static function abortRequest():Void
		activeRequest = null;

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : left > right ? 1 : 0;
}

/** One cache lookup result plus whether planning work was skipped. */
@:noCompletion
class CBodyControlFlowPlanResolution {
	public final planning:CBodyControlFlowPlanningResult;
	public final reused:Bool;

	/** Keep lookup evidence immutable for the remainder of this request. */
	public function new(planning:CBodyControlFlowPlanningResult, reused:Bool) {
		this.planning = planning;
		this.reused = reused;
	}
}

/** Bounded counters emitted after one cache request completes. */
typedef CBodyControlFlowPlanCacheStats = {
	final enabled:Bool;
	final hits:Int;
	final misses:Int;
	final retainedFunctions:Int;
	final retainedKeyCodeUnits:Float;
}

private typedef PersistedControlFlowPlan = {
	final schemaVersion:Int;
	final functionId:String;
	final canonicalFunction:String;
	final planning:CBodyControlFlowPlanningResult;
}

private typedef ControlFlowPlanCacheRequest = {
	final enabled:Bool;
	final priorByFunction:Map<String, PersistedControlFlowPlan>;
	final candidateByFunction:Map<String, PersistedControlFlowPlan>;
	var hits:Int;
	var misses:Int;
}
#else
class CBodyControlFlowPlanCache {}
class CBodyControlFlowPlanResolution {}
#end
