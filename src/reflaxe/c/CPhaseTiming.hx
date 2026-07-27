package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.Json;
import haxe.io.Bytes;
import haxe.macro.Context;
#if eval
import eval.integers.UInt64;
import eval.luv.Resource;
import eval.luv.Result;
import eval.luv.Time;
import eval.vm.Gc;
#end

/**
	Owns opt-in, request-local evidence about where haxe.c spends its time.

	A span is one timed region. Spans form a strict parent/child stack, so each
	record can report both its complete duration and the duration left after
	subtracting nested work. This prevents a parent such as "semantic lowering"
	from being added to its children and counted twice. Normal builds create no
	profile state and preserve the older text records when profiling is enabled.
**/
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
	Closed names for detailed timers nested below a public compiler phase.

	These names appear in the opt-in detail report and Haxe's `--times` table.
	They split a broad phase into useful diagnostic parts without changing the
	stable `HXC_PHASE_TIMING` report consumed by existing tooling.
**/
enum abstract CDetailTimingId(String) to String {
	var CDTHxcIRFunctionPreparation = "HxcIR function preparation";
	var CDTHxcIRRepresentationPlanning = "HxcIR representation planning";
	var CDTHxcIRFunctionConstruction = "HxcIR function construction";
	var CDTHxcIRTypedBodyLowering = "HxcIR typed-body lowering";
	var CDTHxcIRFunctionFinalization = "HxcIR function finalization";
	var CDTHxcIRValueCoalescing = "HxcIR value coalescing";
	var CDTHxcIRValuePlanApplication = "HxcIR value-plan application";
	var CDTHxcIRProgramAssembly = "HxcIR program assembly";
	var CDTHxcIRManagedRootPlanning = "HxcIR managed-root planning";
	var CDTHxcIRNullCheckCoalescing = "HxcIR null-check coalescing";
	var CDTBodySetupAndValuePlanning = "body setup and value planning";
	var CDTBodyControlFlowPlanning = "body control-flow planning";
	var CDTBodyCASTEmission = "body CAST emission";
}

/** Bounded entity and I/O counts recorded only at semantic boundaries. */
enum abstract CProfileCounterId(String) to String {
	var CPCounterTypedModules = "typed.modules";
	var CPCounterTypedDeclarations = "typed.declarations";
	var CPCounterTypedExpressionRoots = "typed.expression-roots";
	var CPCounterHxcIRModules = "hxcir.modules";
	var CPCounterHxcIRTypes = "hxcir.types";
	var CPCounterHxcIRTypeInstances = "hxcir.type-instances";
	var CPCounterHxcIRGlobals = "hxcir.globals";
	var CPCounterHxcIRFunctions = "hxcir.functions";
	var CPCounterHxcIRBlocks = "hxcir.blocks";
	var CPCounterHxcIRInstructions = "hxcir.instructions";
	var CPCounterHxcIRManagedRoots = "hxcir.managed-roots";
	var CPCounterCASTFunctions = "cast.functions";
	var CPCounterRuntimeRequirements = "runtime.requirements";
	var CPCounterGeneratedFiles = "artifacts.files";
	var CPCounterGeneratedBytes = "artifacts.bytes";
	var CPCounterPriorOwnedFiles = "output.prior-owned-files";
	var CPCounterPlannedOutputFiles = "output.planned-files";
	var CPCounterSavedOutputFiles = "output.saved-files";
	var CPCounterPrefixChecks = "output.prefix-checks";
}

#if eval
@:noCompletion
typedef CProfileWallStamp = UInt64;
#else
@:noCompletion
typedef CProfileWallStamp = Float;
#end

private typedef CProfileSpanRecord = {
	final schemaVersion:Int;
	final recordKind:String;
	final requestId:String;
	final spanId:Int;
	final parentSpanId:Null<Int>;
	final category:String;
	final name:String;
	final status:String;
	final startOffsetMicroseconds:Float;
	final inclusiveWallMicroseconds:Float;
	final exclusiveWallMicroseconds:Float;
	final inclusiveCpuMicroseconds:Float;
	final exclusiveCpuMicroseconds:Float;
	final allocatedBytesDelta:Null<Float>;
	final residentBytesAtEnd:Null<Float>;
}

private typedef CProfileCounterRecord = {
	final schemaVersion:Int;
	final recordKind:String;
	final requestId:String;
	final name:String;
	final value:Float;
}

private typedef CProfileRequestRecord = {
	final schemaVersion:Int;
	final recordKind:String;
	final requestId:String;
	final status:String;
	final profile:String;
	final buildMode:String;
	final haxeVersion:String;
	final wallMicroseconds:Float;
	final cpuMicroseconds:Float;
	final allocatedBytesDelta:Null<Float>;
	final maximumObservedResidentBytes:Null<Float>;
	final spanCount:Int;
	final counterCount:Int;
}

/** One open region in the request-local profile stack. */
@:noCompletion
class CProfileSpan {
	public final id:Int;
	public final parent:Null<CProfileSpan>;
	public final category:String;
	public final name:String;
	public final startedWall:CProfileWallStamp;
	public final startedCpu:Float;
	public final startedAllocatedBytes:Null<Float>;
	public final stopCompilerTimer:() -> Void;
	public var childWallMicroseconds:Float = 0.0;
	public var childCpuMicroseconds:Float = 0.0;
	public var stopped:Bool = false;

	public function new(id:Int, parent:Null<CProfileSpan>, category:String, name:String, stopCompilerTimer:() -> Void) {
		this.id = id;
		this.parent = parent;
		this.category = category;
		this.name = name;
		this.startedWall = CPhaseTiming.wallNow();
		this.startedCpu = Sys.cpuTime();
		this.startedAllocatedBytes = CPhaseTiming.allocatedBytes();
		this.stopCompilerTimer = stopCompilerTimer;
	}
}

/** Mutable profile state exists for one compiler request and nowhere else. */
@:noCompletion
private class CProfileRequestState {
	public final requestId:String;
	public final startedWall:CProfileWallStamp;
	public final startedCpu:Float;
	public final startedAllocatedBytes:Null<Float>;
	public final spans:Array<CProfileSpan> = [];
	public final counters:Map<String, Float> = [];
	public var nextSpanId:Int = 1;
	public var emittedSpanCount:Int = 0;
	public var profile:String = "unresolved";
	public var buildMode:String = "unresolved";
	public var haxeVersion:String = "unknown";
	public var maximumObservedResidentBytes:Null<Float>;

	public function new(requestId:String) {
		this.requestId = requestId;
		this.startedWall = CPhaseTiming.wallNow();
		this.startedCpu = Sys.cpuTime();
		this.startedAllocatedBytes = CPhaseTiming.allocatedBytes();
		this.maximumObservedResidentBytes = CPhaseTiming.residentBytes();
	}

	public function observeResidentBytes(value:Null<Float>):Void {
		if (value == null)
			return;
		if (maximumObservedResidentBytes == null || value > maximumObservedResidentBytes)
			maximumObservedResidentBytes = value;
	}
}

/** One request-local detailed clock backed by Haxe's compiler timer. */
@:noCompletion
class CDetailTimer {
	final id:CDetailTimingId;
	final span:CProfileSpan;

	public function new(id:CDetailTimingId) {
		this.id = id;
		this.span = CPhaseTiming.openSpan("detail", Std.string(id), Context.timer("hxc " + Std.string(id)));
	}

	public function stop():Void {
		final elapsedMicroseconds = CPhaseTiming.closeSpan(span, "ok");
		Sys.println(CPhaseTiming.DETAIL_REPORT_PREFIX + Std.string(id) + "\t" + CPhaseTiming.legacyDuration(elapsedMicroseconds));
	}
}

/**
	One request-local phase clock. `@:noCompletion` keeps this compiler-only
	utility out of user-facing autocomplete; it does not change visibility or
	runtime behavior.
**/
@:noCompletion
class CPhaseTimer {
	final id:CPhaseTimingId;
	final span:CProfileSpan;

	public function new(id:CPhaseTimingId) {
		this.id = id;
		this.span = CPhaseTiming.openSpan("phase", Std.string(id), Context.timer("hxc " + Std.string(id)));
	}

	public function stop():Void {
		final elapsedMicroseconds = CPhaseTiming.closeSpan(span, "ok");
		Sys.println(CPhaseTiming.REPORT_PREFIX + Std.string(id) + "\t" + CPhaseTiming.legacyDuration(elapsedMicroseconds));
	}
}

/**
	Opt-in nested timing and bounded counters for the real C compiler pipeline.

	Call `beginRequest` before the first typed-input span and `finishRequest`
	after output ownership. `CDiagnostic.fatal` calls `abortRequest` so an
	expected compiler failure closes every open span and reports which phase was
	active. The structured JSON Lines stream is diagnostic output, never a
	generated artifact or a cross-request cache.
**/
@:noCompletion
class CPhaseTiming {
	public static inline final DEFINE = "reflaxe_c_phase_timing";
	public static inline final REPORT_PREFIX = "HXC_PHASE_TIMING\t";
	public static inline final DETAIL_REPORT_PREFIX = "HXC_DETAIL_TIMING\t";
	public static inline final PROFILE_REPORT_PREFIX = "HXC_PROFILE\t";
	public static inline final PROFILE_SCHEMA_VERSION = 1;

	static var active:Null<CProfileRequestState> = null;

	/** Start a fresh profile only when the opt-in define is present. */
	public static function beginRequest():Void {
		if (!Context.defined(DEFINE)) {
			active = null;
			return;
		}
		if (active != null)
			abortRequest("superseded");
		active = new CProfileRequestState("request-1");
	}

	/** Attach stable request configuration after normal profile resolution. */
	public static function describeRequest(profile:String, buildMode:String):Void {
		final state = active;
		if (state == null)
			return;
		state.profile = profile;
		state.buildMode = buildMode;
		final version = Context.definedValue("haxe");
		state.haxeVersion = version == null || version == "" ? "unknown" : version;
	}

	public static function start(id:CPhaseTimingId):Null<CPhaseTimer> {
		return active == null ? null : new CPhaseTimer(id);
	}

	public static function stop(timer:Null<CPhaseTimer>):Void {
		if (timer != null)
			timer.stop();
	}

	public static function startDetail(id:CDetailTimingId):Null<CDetailTimer> {
		return active == null ? null : new CDetailTimer(id);
	}

	public static function stopDetail(timer:Null<CDetailTimer>):Void {
		if (timer != null)
			timer.stop();
	}

	/** Set one closed counter; conflicting repeated values are an internal error. */
	public static function setCounter(id:CProfileCounterId, value:Int):Void {
		setCounterFloat(id, value);
	}

	/** Record a byte/count value that may exceed a 32-bit Haxe `Int`. */
	public static function setCounterFloat(id:CProfileCounterId, value:Float):Void {
		final state = active;
		if (state == null)
			return;
		if (!Math.isFinite(value) || value < 0.0)
			throw new haxe.Exception('compiler profile counter `${Std.string(id)}` must be finite and non-negative');
		final name = Std.string(id);
		final previous = state.counters.get(name);
		if (previous != null && previous != value)
			throw new haxe.Exception('compiler profile counter `$name` changed from $previous to $value in one request');
		state.counters.set(name, value);
	}

	/** Emit counters and the final request record after every successful span closed. */
	public static function finishRequest():Void {
		final state = active;
		if (state == null)
			return;
		if (state.spans.length != 0)
			throw new haxe.Exception('compiler profile request ended with ${state.spans.length} open span(s)');
		emitRequest(state, "ok");
		active = null;
	}

	/**
		Close every open span as failed before Haxe aborts compilation.

		This hook is called by the central diagnostic boundary, so expected
		negative fixtures retain useful profile evidence instead of losing the
		phase that rejected them.
	**/
	public static function abortRequest(status:String = "failed"):Void {
		final state = active;
		if (state == null)
			return;
		while (state.spans.length > 0) {
			final span = state.spans[state.spans.length - 1];
			closeSpan(span, status);
		}
		emitRequest(state, status);
		active = null;
	}

	/**
		Open one compiler-internal span beneath the currently active span.

		This is public only because Haxe module helper types own the lightweight
		timer handles. The enclosing class is hidden from completion and this
		method is not a target API.
	**/
	@:noCompletion
	public static function openSpan(category:String, name:String, stopCompilerTimer:() -> Void):CProfileSpan {
		final state = requireState();
		final parent = state.spans.length == 0 ? null : state.spans[state.spans.length - 1];
		final span = new CProfileSpan(state.nextSpanId++, parent, category, name, stopCompilerTimer);
		state.spans.push(span);
		return span;
	}

	/** Close the latest span, emit its structured record, and return wall time. */
	@:noCompletion
	public static function closeSpan(span:CProfileSpan, status:String):Float {
		final state = requireState();
		if (span.stopped)
			throw new haxe.Exception('compiler profile span `${span.name}` was stopped twice');
		if (state.spans.length == 0 || state.spans[state.spans.length - 1] != span)
			throw new haxe.Exception('compiler profile span `${span.name}` violated strict nesting');

		span.stopped = true;
		span.stopCompilerTimer();
		final endedWall = wallNow();
		final endedCpu = Sys.cpuTime();
		final inclusiveWall = elapsedWallMicroseconds(span.startedWall, endedWall);
		final inclusiveCpu = elapsedSecondsMicroseconds(span.startedCpu, endedCpu);
		final exclusiveWall = Math.max(0.0, inclusiveWall - span.childWallMicroseconds);
		final exclusiveCpu = Math.max(0.0, inclusiveCpu - span.childCpuMicroseconds);
		final endedAllocatedBytes = allocatedBytes();
		final allocatedDelta = optionalDifference(span.startedAllocatedBytes, endedAllocatedBytes);
		final resident = residentBytes();
		state.observeResidentBytes(resident);
		state.spans.pop();
		if (span.parent != null) {
			span.parent.childWallMicroseconds += inclusiveWall;
			span.parent.childCpuMicroseconds += inclusiveCpu;
		}
		state.emittedSpanCount++;
		final record:CProfileSpanRecord = {
			schemaVersion: PROFILE_SCHEMA_VERSION,
			recordKind: "span",
			requestId: state.requestId,
			spanId: span.id,
			parentSpanId: span.parent == null ? null : span.parent.id,
			category: span.category,
			name: span.name,
			status: status,
			startOffsetMicroseconds: elapsedWallMicroseconds(state.startedWall, span.startedWall),
			inclusiveWallMicroseconds: inclusiveWall,
			exclusiveWallMicroseconds: exclusiveWall,
			inclusiveCpuMicroseconds: inclusiveCpu,
			exclusiveCpuMicroseconds: exclusiveCpu,
			allocatedBytesDelta: allocatedDelta,
			residentBytesAtEnd: resident
		};
		emitSpan(record);
		return inclusiveWall;
	}

	static function emitRequest(state:CProfileRequestState, status:String):Void {
		final counterNames = [for (name in state.counters.keys()) name];
		counterNames.sort(compareUtf8);
		for (name in counterNames) {
			final value = state.counters.get(name);
			if (value == null)
				throw new haxe.Exception('compiler profile lost counter `$name`');
			final counter:CProfileCounterRecord = {
				schemaVersion: PROFILE_SCHEMA_VERSION,
				recordKind: "counter",
				requestId: state.requestId,
				name: name,
				value: value
			};
			emitCounter(counter);
		}
		final endedWall = wallNow();
		final endedCpu = Sys.cpuTime();
		final endedAllocatedBytes = allocatedBytes();
		final resident = residentBytes();
		state.observeResidentBytes(resident);
		final request:CProfileRequestRecord = {
			schemaVersion: PROFILE_SCHEMA_VERSION,
			recordKind: "request",
			requestId: state.requestId,
			status: status,
			profile: state.profile,
			buildMode: state.buildMode,
			haxeVersion: state.haxeVersion,
			wallMicroseconds: elapsedWallMicroseconds(state.startedWall, endedWall),
			cpuMicroseconds: elapsedSecondsMicroseconds(state.startedCpu, endedCpu),
			allocatedBytesDelta: optionalDifference(state.startedAllocatedBytes, endedAllocatedBytes),
			maximumObservedResidentBytes: state.maximumObservedResidentBytes,
			spanCount: state.emittedSpanCount,
			counterCount: counterNames.length
		};
		emitRequestRecord(request);
	}

	static function emitSpan(value:CProfileSpanRecord):Void
		Sys.println(PROFILE_REPORT_PREFIX + Json.stringify(value));

	static function emitCounter(value:CProfileCounterRecord):Void
		Sys.println(PROFILE_REPORT_PREFIX + Json.stringify(value));

	static function emitRequestRecord(value:CProfileRequestRecord):Void
		Sys.println(PROFILE_REPORT_PREFIX + Json.stringify(value));

	static function requireState():CProfileRequestState {
		final state = active;
		if (state == null)
			throw new haxe.Exception("compiler profile span was used outside an active request");
		return state;
	}

	/** Read the monotonic clock used by request and span helper records. */
	@:noCompletion
	public static function wallNow():CProfileWallStamp {
		#if eval
		return Time.hrTime();
		#else
		return Sys.time();
		#end
	}

	private static function elapsedWallMicroseconds(started:CProfileWallStamp, ended:CProfileWallStamp):Float {
		#if eval
		return Std.parseFloat((ended - started).toString()) / 1000.0;
		#else
		return Math.max(0.0, ended - started) * 1000000.0;
		#end
	}

	private static function elapsedSecondsMicroseconds(started:Float, ended:Float):Float
		return Math.max(0.0, ended - started) * 1000000.0;

	/** Read cumulative Eval allocation when the host exposes that counter. */
	@:noCompletion
	public static function allocatedBytes():Null<Float> {
		#if eval
		return Gc.allocated_bytes();
		#else
		return null;
		#end
	}

	/** Sample resident process memory when the Eval host exposes it. */
	@:noCompletion
	public static function residentBytes():Null<Float> {
		#if eval
		return switch Resource.residentSetMemory() {
			case Ok(value): Std.parseFloat(value.toString());
			case Error(_): null;
		};
		#else
		return null;
		#end
	}

	private static function optionalDifference(started:Null<Float>, ended:Null<Float>):Null<Float> {
		if (started == null || ended == null)
			return null;
		return Math.max(0.0, ended - started);
	}

	/** Preserve the bounded integer-microsecond format of the legacy report. */
	@:noCompletion
	public static function legacyDuration(value:Float):String
		return Std.string(Math.round(Math.min(value, 2147483647.0)));

	static function compareUtf8(left:String, right:String):Int {
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0)
				return difference;
		}
		return leftBytes.length - rightBytes.length;
	}
}
#else

/** Compiler phase timing exists only while the compiler macro is running. */
@:noCompletion
class CPhaseTiming {}
#end
