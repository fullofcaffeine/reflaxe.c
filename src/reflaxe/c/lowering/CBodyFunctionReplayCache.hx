package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.Position;
import reflaxe.c.CPhaseTiming;
import reflaxe.c.CPhaseTiming.CProfileCounterId;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.naming.CSymbolRequest;

/**
	Reuses one function's semantic result when its complete typed input is unchanged.

	A warm Haxe compiler process may type the same application repeatedly while a
	developer edits only one module. Rebuilding every unchanged function's HxcIR
	wastes work, but keeping compiler-owned `TypedExpr`, `Type`, or `Position`
	values between requests would make stale Haxe state reachable. This cache
	therefore retains only target-owned HxcIR, immutable name requests, runtime
	requirement facts, and the full canonical input text used to prove equality.

	The cache has a two-part key. The program revision covers shared layouts,
	callable signatures, dispatch, profile, and build mode; the function input
	covers one typed body and every stable source range inside it. Both must match
	before reuse. A request builds a replacement generation privately and
	`completeRequest` publishes it only after generated-output ownership succeeds.
**/
@:noCompletion
class CBodyFunctionReplayCache {
	/** Bump this whenever the retained payload or canonical key contract changes. */
	public static inline final SCHEMA_VERSION = 1;

	/** Disable replay while keeping ordinary function construction authoritative. */
	public static inline final DISABLE_DEFINE = "reflaxe_c_test_disable_body_function_replay_cache";

	/** Print one machine-readable cache record after a successful output transaction. */
	public static inline final REPORT_DEFINE = "reflaxe_c_body_function_replay_cache_report";

	/** Stable prefix that lets tooling separate the cache record from compiler output. */
	public static inline final REPORT_PREFIX = "HXC_BODY_FUNCTION_REPLAY_CACHE=";

	#if (eval && macro)
	@:persistent
	#end
	static var previousGeneration:Null<PersistedBodyFunctionGeneration> = null;

	static var activeRequest:Null<BodyFunctionReplayRequest> = null;

	/**
		Start a request-local candidate without changing prior successful evidence.

		The caller settles the shared program revision after representation and
		import discovery. Until then, no function lookup is legal.
	**/
	public static function beginRequest(enabled:Bool):Void {
		activeRequest = {
			enabled: enabled,
			programRevision: null,
			priorByFunction: [],
			candidateByFunction: [],
			hits: 0,
			misses: 0,
			missingFunctionMisses: 0,
			changedFunctionInputMisses: 0,
			programDecision: BRPDUnsettled
		};
	}

	/**
		Select the prior generation only when every shared semantic input matches.

		Changing a class layout or callee signature can change an otherwise
		untouched caller's HxcIR. Comparing the complete program revision before
		indexing prior functions makes that dependency fail closed: one shared
		change rebuilds the generation instead of mixing incompatible functions.
	**/
	public static function settleProgramRevision(revision:String):Void {
		final request = activeRequest;
		// Focused structural tools can call CBodyLowering without the Reflaxe
		// lifecycle. They remain authoritative cold builds rather than having to
		// manufacture process-persistent cache state.
		if (request == null)
			return;
		if (request.programRevision != null)
			throw new haxe.Exception("body-function replay cache received its program revision twice");
		if (revision == "")
			throw new haxe.Exception("body-function replay cache requires a non-empty program revision");
		request.programRevision = revision;
		final previous = previousGeneration;
		request.programDecision = if (!request.enabled) {
			BRPDDisabled;
		} else if (previous == null) {
			BRPDNoPriorGeneration;
		} else if (previous.schemaVersion != SCHEMA_VERSION) {
			BRPDSchemaChanged;
		} else if (previous.programRevision != revision) {
			BRPDProgramChanged;
		} else {
			BRPDMatched;
		};
		if (request.programDecision == BRPDMatched && previous != null)
			for (entry in previous.entries)
				request.priorByFunction.set(entry.functionId, entry);
	}

	/**
		Return a fresh replay value or run the ordinary function builder.

		`identity.canonicalInput` is compared in full, so a hash collision cannot
		create a hit. Source positions remain request-local: persisted runtime
		requirements retain only their stable HxcIR range, then recover the current
		Haxe `Position` from `identity` for diagnostics.
	**/
	public static function resolve(functionId:String, identity:CBodyFunctionReplayIdentity, build:() -> CBodyFunctionReplayData):CBodyFunctionReplayResolution {
		final request = activeRequest;
		if (request == null)
			return new CBodyFunctionReplayResolution(build(), false);
		if (request.programRevision == null)
			throw new haxe.Exception("body-function replay lookup ran before the program revision was settled");
		if (functionId == "" || identity.canonicalInput == "")
			throw new haxe.Exception("body-function replay requires a function identity and canonical typed input");
		if (request.candidateByFunction.exists(functionId))
			throw new haxe.Exception('body-function replay received duplicate function `$functionId`');

		if (!request.enabled) {
			return new CBodyFunctionReplayResolution(build(), false);
		}

		final prior = request.priorByFunction.get(functionId);
		final reused = prior != null && prior.schemaVersion == SCHEMA_VERSION && prior.canonicalInput == identity.canonicalInput;
		final data = if (reused && prior != null) {
			request.hits++;
			restore(prior.data, identity);
		} else {
			request.misses++;
			if (request.programDecision == BRPDMatched) {
				if (prior == null)
					request.missingFunctionMisses++;
				else
					request.changedFunctionInputMisses++;
			}
			build();
		};
		request.candidateByFunction.set(functionId, {
			schemaVersion: SCHEMA_VERSION,
			functionId: functionId,
			canonicalInput: identity.canonicalInput,
			data: persist(data)
		});
		return new CBodyFunctionReplayResolution(data, reused);
	}

	/**
		Publish exactly one complete generation and return bounded accounting.

		Replacement drops deleted functions and bounds persistent process memory.
		Canonical ordering makes reports independent of Haxe map iteration.
	**/
	public static function completeRequest():CBodyFunctionReplayCacheStats {
		final request = requireRequest();
		final revision = request.programRevision;
		if (revision == null)
			throw new haxe.Exception("body-function replay cache completed before its program revision was settled");
		if (request.enabled) {
			final entries = [for (entry in request.candidateByFunction) entry];
			entries.sort((left, right) -> compareStrings(left.functionId, right.functionId));
			previousGeneration = {
				schemaVersion: SCHEMA_VERSION,
				programRevision: revision,
				entries: entries
			};
		}

		final retained = previousGeneration;
		var retainedInputCodeUnits = 0.0;
		final retainedFunctions = retained == null ? 0 : retained.entries.length;
		final retainedProgramRevisionCodeUnits = retained == null ? 0.0 : retained.programRevision.length;
		if (retained != null)
			for (entry in retained.entries)
				retainedInputCodeUnits += entry.canonicalInput.length;
		final stats:CBodyFunctionReplayCacheStats = {
			enabled: request.enabled,
			programRevisionMatched: request.programDecision == BRPDMatched,
			programDecision: request.programDecision,
			hits: request.hits,
			misses: request.misses,
			missingFunctionMisses: request.missingFunctionMisses,
			changedFunctionInputMisses: request.changedFunctionInputMisses,
			retainedFunctions: retainedFunctions,
			retainedProgramRevisionCodeUnits: retainedProgramRevisionCodeUnits,
			retainedInputCodeUnits: retainedInputCodeUnits
		};
		activeRequest = null;
		CPhaseTiming.setCounter(CPCounterHxcIRFunctionReplayCacheHits, stats.hits);
		CPhaseTiming.setCounter(CPCounterHxcIRFunctionReplayCacheMisses, stats.misses);
		CPhaseTiming.setCounter(CPCounterHxcIRFunctionReplayCacheMissingFunctionMisses, stats.missingFunctionMisses);
		CPhaseTiming.setCounter(CPCounterHxcIRFunctionReplayCacheChangedFunctionInputMisses, stats.changedFunctionInputMisses);
		CPhaseTiming.setCounter(CPCounterHxcIRFunctionReplayCacheProgramRevisionMatched, stats.programRevisionMatched ? 1 : 0);
		CPhaseTiming.setCounter(CPCounterHxcIRFunctionReplayCacheRetainedFunctions, stats.retainedFunctions);
		CPhaseTiming.setCounterFloat(CPCounterHxcIRFunctionReplayCacheRetainedProgramRevisionCodeUnits, stats.retainedProgramRevisionCodeUnits);
		CPhaseTiming.setCounterFloat(CPCounterHxcIRFunctionReplayCacheRetainedInputCodeUnits, stats.retainedInputCodeUnits);
		return stats;
	}

	/** Discard only unfinished request state; the prior successful generation survives. */
	public static function abortRequest():Void
		activeRequest = null;

	static function requireRequest():BodyFunctionReplayRequest {
		final request = activeRequest;
		if (request == null)
			throw new haxe.Exception("body-function replay cache has no active request");
		return request;
	}

	static function persist(data:CBodyFunctionReplayData):PersistedBodyFunctionData {
		return {
			ir: cloneFunction(data.ir),
			localRequests: copyRequestMap(data.localRequests),
			spanLengthRequests: copyRequestMap(data.spanLengthRequests),
			temporaryRequests: copyRequestMap(data.temporaryRequests),
			tailArgumentRequests: copyRequestArrayMap(data.tailArgumentRequests),
			labelRequests: copyRequestMap(data.labelRequests),
			enumReasons: data.enumReasons.copy(),
			runtimeRequirements: data.runtimeRequirements.map(requirement -> {
				featureId: requirement.featureId,
				operationId: requirement.operationId,
				surface: requirement.surface,
				source: requirement.source,
				hadPosition: requirement.position != null,
				kind: requirement.kind
			})
		};
	}

	static function restore(data:PersistedBodyFunctionData, identity:CBodyFunctionReplayIdentity):CBodyFunctionReplayData {
		return {
			ir: cloneFunction(data.ir),
			localRequests: copyRequestMap(data.localRequests),
			spanLengthRequests: copyRequestMap(data.spanLengthRequests),
			temporaryRequests: copyRequestMap(data.temporaryRequests),
			tailArgumentRequests: copyRequestArrayMap(data.tailArgumentRequests),
			labelRequests: copyRequestMap(data.labelRequests),
			enumReasons: data.enumReasons.copy(),
			runtimeRequirements: data.runtimeRequirements.map(requirement -> {
				featureId: requirement.featureId,
				operationId: requirement.operationId,
				surface: requirement.surface,
				source: requirement.source,
				position: requirement.hadPosition ? identity.requirePosition(requirement.source) : null,
				kind: requirement.kind
			})
		};
	}

	/**
		Copy arrays later semantic passes may replace while keeping immutable nodes.

		Managed-root planning replaces root arrays and null-check coalescing replaces
		block instruction arrays. Giving every request fresh containers prevents
		those passes from mutating the persistent prior generation.
	**/
	static function cloneFunction(value:HxcIRFunction):HxcIRFunction {
		final managedRoots = value.managedRoots == null ? null : value.managedRoots.map(root -> {
			id: root.id,
			valueId: root.valueId,
			projections: root.projections.copy(),
			source: root.source
		});
		return {
			id: value.id,
			displayName: value.displayName,
			parameters: value.parameters.copy(),
			borrowedClassParameterIds: value.borrowedClassParameterIds.copy(),
			borrowedInterfaceParameterIds: value.borrowedInterfaceParameterIds == null ? null : value.borrowedInterfaceParameterIds.copy(),
			borrowedClassLocalIds: value.borrowedClassLocalIds.copy(),
			borrowedInterfaceLocalIds: value.borrowedInterfaceLocalIds == null ? null : value.borrowedInterfaceLocalIds.copy(),
			managedRoots: managedRoots,
			locals: value.locals.copy(),
			returnType: value.returnType,
			borrowedSpanReturn: value.borrowedSpanReturn,
			failureConvention: value.failureConvention,
			entryBlockId: value.entryBlockId,
			blocks: value.blocks.map(block -> {
				id: block.id,
				parameters: block.parameters.copy(),
				instructions: block.instructions.copy(),
				terminator: block.terminator,
				source: block.source
			}),
			cleanupRegions: value.cleanupRegions.map(region -> {
				id: region.id,
				parentId: region.parentId,
				actions: region.actions.copy(),
				source: region.source
			}),
			source: value.source
		};
	}

	static function copyRequestMap(source:Map<String, CSymbolRequest>):Map<String, CSymbolRequest> {
		final result:Map<String, CSymbolRequest> = [];
		for (key => value in source)
			result.set(key, value);
		return result;
	}

	static function copyRequestArrayMap(source:Map<String, Array<CSymbolRequest>>):Map<String, Array<CSymbolRequest>> {
		final result:Map<String, Array<CSymbolRequest>> = [];
		for (key => value in source)
			result.set(key, value.copy());
		return result;
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : left > right ? 1 : 0;
}

/**
	One request-local function key plus current diagnostic positions.

	The position map is deliberately absent from persistent entries. It only
	turns a replayed stable source range back into the current Haxe compiler's
	opaque `Position` value while this request is alive.
**/
@:noCompletion
class CBodyFunctionReplayIdentity {
	public final canonicalInput:String;

	final positionsBySource:Map<String, Position>;

	/** Keep exact reusable text beside current-request positions used only for diagnostics. */
	public function new(canonicalInput:String, positionsBySource:Map<String, Position>) {
		this.canonicalInput = canonicalInput;
		this.positionsBySource = positionsBySource;
	}

	/**
		Return the current request's opaque diagnostic position or fail closed.

		A retained requirement that originally had a Haxe position must find the
		same stable range in the current exact-input ledger. Absence means the key
		or retained payload is internally inconsistent, so silently restoring
		`null` would turn cache corruption into weaker diagnostics.
	**/
	public function requirePosition(source:HxcSourceSpan):Position {
		final key = source.display();
		final position = positionsBySource.get(key);
		if (position == null)
			throw new haxe.Exception('body-function replay could not restore required source position `$key`');
		return position;
	}
}

/** Function-owned data safe to restore before whole-program HxcIR validation. */
typedef CBodyFunctionReplayData = {
	final ir:HxcIRFunction;
	final localRequests:Map<String, CSymbolRequest>;
	final spanLengthRequests:Map<String, CSymbolRequest>;
	final temporaryRequests:Map<String, CSymbolRequest>;
	final tailArgumentRequests:Map<String, Array<CSymbolRequest>>;
	final labelRequests:Map<String, CSymbolRequest>;
	final enumReasons:Array<CBodyFunctionReplayEnumReason>;
	final runtimeRequirements:Array<CBodyFunctionReplayRuntimeRequirement>;
}

/** One generic-enum specialization range first observed in this function. */
typedef CBodyFunctionReplayEnumReason = {
	final instanceId:String;
	final source:HxcSourceSpan;
}

/** A runtime dependency with an optional current-request diagnostic position. */
typedef CBodyFunctionReplayRuntimeRequirement = {
	final featureId:String;
	final operationId:String;
	final surface:String;
	final source:HxcSourceSpan;
	final position:Null<Position>;
	final kind:String;
}

/** One lookup result and whether ordinary typed-body construction was skipped. */
@:noCompletion
class CBodyFunctionReplayResolution {
	public final data:CBodyFunctionReplayData;
	public final reused:Bool;

	/** Pair a fresh request-owned payload with the decision that produced it. */
	public function new(data:CBodyFunctionReplayData, reused:Bool) {
		this.data = data;
		this.reused = reused;
	}
}

/** Bounded evidence emitted after one complete compiler request. */
typedef CBodyFunctionReplayCacheStats = {
	final enabled:Bool;
	final programRevisionMatched:Bool;
	final programDecision:CBodyFunctionReplayProgramDecision;
	final hits:Int;
	final misses:Int;
	final missingFunctionMisses:Int;
	final changedFunctionInputMisses:Int;
	final retainedFunctions:Int;
	final retainedProgramRevisionCodeUnits:Float;
	final retainedInputCodeUnits:Float;
}

private typedef PersistedBodyFunctionGeneration = {
	final schemaVersion:Int;
	final programRevision:String;
	final entries:Array<PersistedBodyFunctionEntry>;
}

private typedef PersistedBodyFunctionEntry = {
	final schemaVersion:Int;
	final functionId:String;
	final canonicalInput:String;
	final data:PersistedBodyFunctionData;
}

private typedef PersistedBodyFunctionData = {
	final ir:HxcIRFunction;
	final localRequests:Map<String, CSymbolRequest>;
	final spanLengthRequests:Map<String, CSymbolRequest>;
	final temporaryRequests:Map<String, CSymbolRequest>;
	final tailArgumentRequests:Map<String, Array<CSymbolRequest>>;
	final labelRequests:Map<String, CSymbolRequest>;
	final enumReasons:Array<CBodyFunctionReplayEnumReason>;
	final runtimeRequirements:Array<PersistedBodyFunctionRuntimeRequirement>;
}

private typedef PersistedBodyFunctionRuntimeRequirement = {
	final featureId:String;
	final operationId:String;
	final surface:String;
	final source:HxcSourceSpan;
	final hadPosition:Bool;
	final kind:String;
}

private typedef BodyFunctionReplayRequest = {
	final enabled:Bool;
	var programRevision:Null<String>;
	final priorByFunction:Map<String, PersistedBodyFunctionEntry>;
	final candidateByFunction:Map<String, PersistedBodyFunctionEntry>;
	var hits:Int;
	var misses:Int;
	var missingFunctionMisses:Int;
	var changedFunctionInputMisses:Int;
	var programDecision:CBodyFunctionReplayProgramDecision;
}

/**
	Why the complete shared-program half of the cache key did or did not match.

	Function-level missing/changed-input counters are meaningful only after
	`BRPDMatched`; every miss under another variant is already explained by this
	program-wide decision.
**/
@:noCompletion
enum abstract CBodyFunctionReplayProgramDecision(String) {
	var BRPDUnsettled = "unsettled";
	var BRPDDisabled = "disabled";
	var BRPDNoPriorGeneration = "no-prior-generation";
	var BRPDSchemaChanged = "schema-changed";
	var BRPDProgramChanged = "program-changed";
	var BRPDMatched = "matched";
}
#else
class CBodyFunctionReplayCache {}
class CBodyFunctionReplayIdentity {}
class CBodyFunctionReplayResolution {}
#end
