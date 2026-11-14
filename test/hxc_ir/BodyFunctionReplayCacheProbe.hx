import haxe.macro.Expr;
import haxe.macro.Expr.Position;
import haxe.macro.Context;
import haxe.macro.TypedExprTools;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.frontend.TypedFunctionSourceProvenance;
import reflaxe.c.lowering.CBodyFunctionReplayCache;
import reflaxe.c.lowering.CBodyFunctionReplayCache.CBodyFunctionReplayData;
import reflaxe.c.lowering.CBodyFunctionReplayCache.CBodyFunctionReplayIdentity;
import reflaxe.c.lowering.CBodyFunctionReplayCache.CBodyFunctionReplayProgramDecision;
import reflaxe.c.naming.CSymbolRequest;

/**
	Exercises semantic-function replay inside the persistent macro process.

	The production cache exists only while Haxe's compilation server keeps the
	target macro loaded. This focused probe models several compiler requests in
	one process so stale shared revisions, changed function inputs, failed output
	transactions, disabled reuse, and mutable result containers all have direct
	regression evidence without compiling Caxecraft.
**/
class BodyFunctionReplayCacheProbe {
	/**
		Require a full shared-program match and exact function input before reuse.

		The returned expression only satisfies Haxe's `--macro` protocol; every
		assertion runs while the test compiler loads this module.
	**/
	public static macro function run():Expr {
		var builds = 0;
		function build(id:String):CBodyFunctionReplayData {
			builds++;
			return data(id);
		}

		Context.onAfterInitMacros(testCanonicalTypedExpressions);

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-a");
		final first = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("function.demo"));
		require(!first.reused && builds == 1, "first request must build its function");
		final firstStats = CBodyFunctionReplayCache.completeRequest();
		require(firstStats.programDecision == BRPDNoPriorGeneration
			&& firstStats.hits == 0
			&& firstStats.misses == 1
			&& firstStats.missingFunctionMisses == 0
			&& firstStats.changedFunctionInputMisses == 0
			&& firstStats.retainedFunctions == 1,
			"first request accounting drifted");

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-a");
		final identical = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("unexpected"));
		require(identical.reused && builds == 1, "identical shared and function inputs did not replay");
		require(identical.data.ir.blocks.length == 1, "replayed function lost its block");
		final replayedBorrowParameters = identical.data.ir.mutableAggregateBorrowParameterIds;
		final replayedBorrowLocals = identical.data.ir.mutableAggregateBorrowLocalIds;
		if (replayedBorrowParameters == null || replayedBorrowLocals == null)
			throw new haxe.Exception("replayed function lost its mutable-record borrow ledgers");
		require(replayedBorrowParameters.length == 0, "replayed function changed its mutable-record parameter ledger");
		require(replayedBorrowLocals.length == 0, "replayed function changed its mutable-record local ledger");
		replayedBorrowParameters.push("caller.mutation");
		replayedBorrowLocals.push("caller.mutation");
		identical.data.ir.blocks.resize(0);
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-a");
		final afterMutation = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("unexpected"));
		require(afterMutation.reused
			&& afterMutation.data.ir.blocks.length == 1, "caller mutation escaped into the persistent generation");
		require(afterMutation.data.ir.mutableAggregateBorrowParameterIds != null
			&& afterMutation.data.ir.mutableAggregateBorrowParameterIds.length == 0
			&& afterMutation.data.ir.mutableAggregateBorrowLocalIds != null
			&& afterMutation.data.ir.mutableAggregateBorrowLocalIds.length == 0,
			"caller mutation escaped into the persistent mutable-record borrow ledgers");
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-a");
		final changedInput = CBodyFunctionReplayCache.resolve("function.demo", identity("input-b"), () -> build("function.demo"));
		require(!changedInput.reused && builds == 2, "changed typed input reused stale HxcIR");
		CBodyFunctionReplayCache.abortRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-a");
		final afterAbort = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("unexpected"));
		require(afterAbort.reused && builds == 2, "aborted request replaced the successful generation");
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-b");
		final changedProgram = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("function.demo"));
		require(!changedProgram.reused && builds == 3, "changed shared program reused an incompatible function");
		final changedProgramStats = CBodyFunctionReplayCache.completeRequest();
		require(changedProgramStats.programDecision == BRPDProgramChanged && !changedProgramStats.programRevisionMatched,
			"changed shared program reported a false revision match");

		CBodyFunctionReplayCache.beginRequest(false);
		CBodyFunctionReplayCache.settleProgramRevision("program-b");
		final disabled = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("function.demo"));
		require(!disabled.reused && builds == 4, "disabled replay skipped authoritative construction");
		final disabledStats = CBodyFunctionReplayCache.completeRequest();
		require(!disabledStats.enabled
			&& disabledStats.programDecision == BRPDDisabled
			&& disabledStats.hits == 0
			&& disabledStats.misses == 0
			&& disabledStats.retainedFunctions == 1,
			"disabled request changed retained evidence or reported a false decision");

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-b");
		final afterDisabled = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("unexpected"));
		require(afterDisabled.reused && builds == 4, "disabled request replaced the prior generation");
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-c");
		CBodyFunctionReplayCache.resolve("function.keep", identity("keep"), () -> build("function.keep"));
		CBodyFunctionReplayCache.resolve("function.remove", identity("remove"), () -> build("function.remove"));
		final twoFunctions = CBodyFunctionReplayCache.completeRequest();
		require(twoFunctions.retainedFunctions == 2, "two-function generation was not published completely");

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-c");
		final kept = CBodyFunctionReplayCache.resolve("function.keep", identity("keep"), () -> build("unexpected"));
		require(kept.reused, "surviving function did not replay");
		final replacement = CBodyFunctionReplayCache.completeRequest();
		require(replacement.retainedFunctions == 1, "replacement generation retained a deleted function");

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-c");
		final removed = CBodyFunctionReplayCache.resolve("function.remove", identity("remove"), () -> build("function.remove"));
		require(!removed.reused, "deleted function remained reachable from the replacement generation");
		final removedStats = CBodyFunctionReplayCache.completeRequest();
		require(removedStats.programDecision == BRPDMatched
			&& removedStats.missingFunctionMisses == 1
			&& removedStats.changedFunctionInputMisses == 0,
			"missing-function miss reason drifted");

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-position");
		CBodyFunctionReplayCache.resolve("function.positioned", positionedIdentity("positioned"), () -> {
			builds++;
			return positionedData("function.positioned");
		});
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-position");
		var rejectedIncompletePositionLedger = false;
		try {
			CBodyFunctionReplayCache.resolve("function.positioned", identity("positioned"), () -> build("unexpected"));
		} catch (error:haxe.Exception) {
			rejectedIncompletePositionLedger = error.message.indexOf("could not restore required source position") >= 0;
		}
		require(rejectedIncompletePositionLedger, "corrupt replay position evidence did not fail closed");
		CBodyFunctionReplayCache.abortRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-position");
		CBodyFunctionReplayCache.resolve("function.positioned", identity("positioned-input-changed"), () -> build("function.positioned"));
		final changedInputStats = CBodyFunctionReplayCache.completeRequest();
		require(changedInputStats.programDecision == BRPDMatched
			&& changedInputStats.missingFunctionMisses == 0
			&& changedInputStats.changedFunctionInputMisses == 1,
			"changed-input miss reason drifted");

		verifyBoundedReplacementSoak(build);
		verifySnapshotOwnership();
		return macro null;
	}

	/**
		Keep caller edits and current compiler positions out of retained generations.

		The cache transports typed containers; this fixture does not claim that its
		combined borrow, cleanup and exception metadata forms an executable program.
		Existing validator and native owners test those independent semantic rules.
	**/
	static function verifySnapshotOwnership():Void {
		var builds = 0;
		final firstPosition = snapshotPosition(10);
		final secondPosition = snapshotPosition(20);
		final thirdPosition = snapshotPosition(30);
		function build():CBodyFunctionReplayData {
			builds++;
			return snapshotData("function.snapshot", firstPosition);
		}
		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("snapshot-program");
		final seeded = CBodyFunctionReplayCache.resolve("function.snapshot", snapshotIdentity(firstPosition), build);
		require(!seeded.reused && builds == 1, "snapshot seed did not build exactly once");
		mutateSnapshot(seeded.data);
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("snapshot-program");
		final hit = CBodyFunctionReplayCache.resolve("function.snapshot", snapshotIdentity(secondPosition), build);
		require(hit.reused && builds == 1, "snapshot hit called the builder");
		requireSnapshot(hit.data, 20);
		mutateSnapshot(hit.data);
		CBodyFunctionReplayCache.completeRequest();
		// This result outlives publication. Its containers must remain caller-owned.
		hit.data.localRequests.set("late", new CSymbolRequest(CSKLocal, ["late"], CNSOrdinary("function.snapshot")));
		hit.data.ir.blocks.push(seeded.data.ir.blocks[0]);

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("snapshot-program");
		final third = CBodyFunctionReplayCache.resolve("function.snapshot", snapshotIdentity(thirdPosition), build);
		require(third.reused && builds == 1, "third generation did not reuse the original snapshot");
		requireSnapshot(third.data, 30);
		mutateSnapshot(third.data);
		CBodyFunctionReplayCache.abortRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("snapshot-program");
		var rejected = false;
		try {
			CBodyFunctionReplayCache.resolve("function.snapshot", identity("snapshot-input"), build);
		} catch (error:haxe.Exception) {
			rejected = error.message.indexOf("could not restore required source position") >= 0;
		}
		require(rejected && builds == 1, "missing current position did not reject the hit");
		// Retry in this request proves that the failed restore inserted no candidate.
		final recovered = CBodyFunctionReplayCache.resolve("function.snapshot", snapshotIdentity(thirdPosition), build);
		require(recovered.reused && builds == 1, "failed restore published a candidate or lost prior data");
		requireSnapshot(recovered.data, 30);
		final recoveredStats = CBodyFunctionReplayCache.completeRequest();
		require(recoveredStats.hits == 2 && recoveredStats.misses == 0 && recoveredStats.retainedFunctions == 1,
			"restoration failure changed existing hit accounting or publication");
	}

	/** A distinct compiler position lets each request prove that it used its own ledger. */
	public static function snapshotPosition(offset:Int):Position
		return Context.makePosition({file: "test/hxc_ir/BodyFunctionReplayCacheProbe.hx", min: offset, max: offset + 1});

	/** Bind the same exact function input to a new request's source-position ledger. */
	public static function snapshotIdentity(position:Position):CBodyFunctionReplayIdentity {
		final positions:Map<String, Position> = [];
		positions.set(snapshotSource().display(), position);
		return new CBodyFunctionReplayIdentity("snapshot-input", positions);
	}

	/** Stable source identity is shared; opaque compiler positions are supplied separately. */
	static function snapshotSource():HxcSourceSpan
		return new HxcSourceSpan("test/hxc_ir/BodyFunctionReplayCacheProbe.hx", 1, 1, 1, 2);

	/**
		One finite transport payload exercises every container copied by replay.

		The private allocation probe uses this ordinary public test fixture too.
		No cache internals or privileged access are exposed for measurement.
	**/
	public static function snapshotData(id:String, position:Position):CBodyFunctionReplayData {
		final source = snapshotSource();
		final request = new CSymbolRequest(CSKLocal, ["snapshot"], CNSOrdinary("function.snapshot"));
		final ir:HxcIRFunction = {
			id: id,
			displayName: id,
			parameters: [{id: "argument", type: IRTInt(32, true), source: source}],
			borrowedClassParameterIds: ["class.parameter"],
			borrowedInterfaceParameterIds: ["interface.parameter"],
			borrowedAggregateParameterIds: ["aggregate.parameter"],
			mutableAggregateBorrowParameterIds: ["mutable.parameter"],
			borrowedClassLocalIds: ["class.local"],
			borrowedInterfaceLocalIds: ["interface.local"],
			borrowedAggregateLocalIds: ["aggregate.local"],
			mutableAggregateBorrowLocalIds: ["mutable.local"],
			managedRoots: [
				{
					id: "root",
					valueId: "value",
					projections: [IRMRPDynamicPayload],
					source: source
				}
			],
			exceptionStrategy: IRESContainedRuntime,
			exceptionRegions: [
				{
					id: "exception",
					frameStorageId: "frame",
					payloadValueId: "payload",
					source: source
				}
			],
			exceptionCleanups: [
				{
					id: "exception.cleanup",
					storageId: "local",
					actionId: "cleanup.action",
					place: IRPLocal("local"),
					implementation: IRIStatic,
					source: source
				}
			],
			locals: [
				{
					id: "local",
					type: IRTInt(32, true),
					storage: IRLSAutomatic,
					initialState: IRISUninitialized,
					source: source
				}
			],
			returnType: IRTVoid,
			borrowedSpanReturn: null,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [{id: "block.argument", type: IRTBool, source: source}],
					instructions: [
						{
							id: "constant",
							result: {id: "value", type: IRTInt(32, true)},
							kind: IRIOConstant(IRCInt("7")),
							source: source
						}
					],
					terminator: {kind: IRTReturn(null, []), source: source},
					source: source
				}
			],
			cleanupRegions: [
				{
					id: "cleanup",
					parentId: null,
					actions: [
						{
							id: "cleanup.action",
							idempotence: IRCExactlyOnce,
							kind: IRCAFinally("entry"),
							source: source
						}
					],
					source: source
				}
			],
			source: source
		};
		return {
			ir: ir,
			localRequests: ["local" => request],
			spanLengthRequests: ["span" => request],
			temporaryRequests: ["temporary" => request],
			tailArgumentRequests: ["tail" => [request]],
			labelRequests: ["label" => request],
			enumReasons: [
				{
					instanceId: "enum.instance",
					source: source
				}
			],
			runtimeRequirements: [
				{
					featureId: "gc",
					operationId: "positioned",
					surface: "snapshot",
					source: source,
					position: position,
					kind: "runtime-operation"
				},
				{
					featureId: "gc",
					operationId: "unpositioned",
					surface: "snapshot",
					source: source,
					position: null,
					kind: "runtime-operation"
				}
			]
		};
	}

	/** Assert authored values after restore; the expectation does not inspect cache storage. */
	public static function requireSnapshot(value:CBodyFunctionReplayData, offset:Int):Void {
		final ir = value.ir;
		require(ir.parameters.length == 1 && ir.parameters[0].id == "argument" && ir.locals.length == 1 && ir.locals[0].id == "local",
			"snapshot lost parameters or locals");
		final ledgers = [
			ir.borrowedClassParameterIds,
			ir.borrowedInterfaceParameterIds,
			ir.borrowedAggregateParameterIds,
			ir.mutableAggregateBorrowParameterIds,
			ir.borrowedClassLocalIds,
			ir.borrowedInterfaceLocalIds,
			ir.borrowedAggregateLocalIds,
			ir.mutableAggregateBorrowLocalIds
		];
		final expected = [
			"class.parameter", "interface.parameter", "aggregate.parameter", "mutable.parameter",
			    "class.local",     "interface.local",     "aggregate.local",     "mutable.local"
		];
		for (index in 0...ledgers.length) {
			final ledger = ledgers[index];
			require(ledger != null && ledger.length == 1 && ledger[0] == expected[index], "snapshot borrow ledger changed");
		}
		final roots = ir.managedRoots;
		final regions = ir.exceptionRegions;
		final cleanups = ir.exceptionCleanups;
		require(roots != null
			&& roots.length == 1
			&& roots[0].id == "root"
			&& roots[0].projections.length == 1
			&& roots[0].projections[0] == IRMRPDynamicPayload,
			"snapshot managed root changed");
		require(regions != null && regions.length == 1 && regions[0].frameStorageId == "frame", "snapshot exception region changed");
		require(cleanups != null
			&& cleanups.length == 1
			&& cleanups[0].actionId == "cleanup.action", "snapshot exception cleanup changed");
		require(ir.blocks.length == 1
			&& ir.blocks[0].id == "entry"
			&& ir.blocks[0].parameters.length == 1
			&& ir.blocks[0].parameters[0].id == "block.argument"
			&& ir.blocks[0].instructions.length == 1
			&& ir.blocks[0].instructions[0].id == "constant",
			"snapshot block containers changed");
		require(ir.cleanupRegions.length == 1
			&& ir.cleanupRegions[0].actions.length == 1
			&& ir.cleanupRegions[0].actions[0].id == "cleanup.action",
			"snapshot cleanup actions changed");
		final maps = [
			value.localRequests,
			value.spanLengthRequests,
			value.temporaryRequests,
			value.labelRequests
		];
		final keys = ["local", "span", "temporary", "label"];
		for (index in 0...maps.length) {
			final map = maps[index];
			final request = map.get(keys[index]);
			require([for (key in map.keys()) key].length == 1 && request != null && request.qualifiedName.join(".") == "snapshot",
				"snapshot request map changed");
		}
		final tail = value.tailArgumentRequests.get("tail");
		require([for (key in value.tailArgumentRequests.keys()) key].length == 1
			&& tail != null
			&& tail.length == 1
			&& tail[0].qualifiedName.join(".") == "snapshot",
			"snapshot tail arguments changed");
		require(value.enumReasons.length == 1 && value.enumReasons[0].instanceId == "enum.instance", "snapshot enum reasons changed");
		require(value.runtimeRequirements.length == 2 && value.runtimeRequirements[1].position == null, "snapshot null position changed");
		final position = value.runtimeRequirements[0].position;
		require(position != null, "snapshot lost its required current position");
		if (position != null) {
			final current = Context.getPosInfos(position);
			require(current.min == offset && current.max == offset + 1, "snapshot retained an older compiler position");
		}
	}

	/** Mutate only containers which the public replay contract makes caller-owned. */
	static function mutateSnapshot(value:CBodyFunctionReplayData):Void {
		final ir = value.ir;
		ir.parameters.resize(0);
		ir.locals.resize(0);
		for (ledger in [
			ir.borrowedClassParameterIds,
			ir.borrowedInterfaceParameterIds,
			ir.borrowedAggregateParameterIds,
			ir.mutableAggregateBorrowParameterIds,
			ir.borrowedClassLocalIds,
			ir.borrowedInterfaceLocalIds,
			ir.borrowedAggregateLocalIds,
			ir.mutableAggregateBorrowLocalIds
		])
			if (ledger != null)
				ledger.resize(0);
		final roots = ir.managedRoots;
		if (roots != null) {
			roots[0].projections.resize(0);
			roots.resize(0);
		}
		final regions = ir.exceptionRegions;
		if (regions != null)
			regions.resize(0);
		final cleanups = ir.exceptionCleanups;
		if (cleanups != null)
			cleanups.resize(0);
		ir.blocks[0].parameters.resize(0);
		ir.blocks[0].instructions.resize(0);
		ir.cleanupRegions[0].actions.resize(0);
		ir.cleanupRegions.resize(0);
		for (map in [
			value.localRequests,
			value.spanLengthRequests,
			value.temporaryRequests,
			value.labelRequests
		])
			map.clear();
		final tail = value.tailArgumentRequests.get("tail");
		if (tail != null)
			tail.resize(0);
		value.tailArgumentRequests.clear();
		value.enumReasons.resize(0);
		value.runtimeRequirements.resize(0);
	}

	/**
		Replace one complete generation repeatedly without retaining old requests.

		Each cycle publishes two functions under a new program revision, then sends
		an unchanged request that must replay both. Exact retained text sizes stay
		constant across all 48 requests, proving the cache replaces one generation
		instead of appending history. This focused structural soak is intentionally
		cheap; the integrated server test separately owns real Haxe request order.
	**/
	static function verifyBoundedReplacementSoak(build:String->CBodyFunctionReplayData):Void {
		final inputA = "soak-input-a";
		final inputB = "soak-input-b";
		final expectedInputCodeUnits = inputA.length + inputB.length;
		for (cycle in 0...24) {
			final revision = 'soak-program-$cycle';
			CBodyFunctionReplayCache.beginRequest(true);
			CBodyFunctionReplayCache.settleProgramRevision(revision);
			CBodyFunctionReplayCache.resolve("function.soak.a", identity(inputA), () -> build("function.soak.a"));
			CBodyFunctionReplayCache.resolve("function.soak.b", identity(inputB), () -> build("function.soak.b"));
			final changed = CBodyFunctionReplayCache.completeRequest();
			require(changed.programDecision == BRPDProgramChanged
				&& changed.hits == 0
				&& changed.misses == 2
				&& changed.retainedFunctions == 2
				&& changed.retainedInputCodeUnits == expectedInputCodeUnits
				&& changed.retainedProgramRevisionCodeUnits == revision.length,
				'soak cycle $cycle did not replace the complete generation');

			CBodyFunctionReplayCache.beginRequest(true);
			CBodyFunctionReplayCache.settleProgramRevision(revision);
			CBodyFunctionReplayCache.resolve("function.soak.a", identity(inputA), () -> build("unexpected"));
			CBodyFunctionReplayCache.resolve("function.soak.b", identity(inputB), () -> build("unexpected"));
			final unchanged = CBodyFunctionReplayCache.completeRequest();
			require(unchanged.programDecision == BRPDMatched
				&& unchanged.hits == 2
				&& unchanged.misses == 0
				&& unchanged.retainedFunctions == 2
				&& unchanged.retainedInputCodeUnits == expectedInputCodeUnits
				&& unchanged.retainedProgramRevisionCodeUnits == revision.length,
				'soak cycle $cycle did not replay one bounded generation');
		}
	}

	/**
		Prove variable allocation is ignored while a real expression edit is kept.
		Call the canonicalization owner directly: the lowering facade delegates to
		it, but importing that facade also types the entire function builder.
	**/
	static function testCanonicalTypedExpressions():Void {
		final typedFirst = Context.typeExpr(macro {
			final value = 1;
			value + 2;
		});
		final typedSecond = Context.typeExpr(macro {
			final value = 1;
			value + 2;
		});
		require(TypedExprTools.toString(typedFirst, false) != TypedExprTools.toString(typedSecond, false),
			"probe did not receive distinct request-local variable IDs");
		require(TypedFunctionSourceProvenance.canonicalTypedExpressionText(typedFirst) == TypedFunctionSourceProvenance.canonicalTypedExpressionText(typedSecond),
			"request-local variable allocation changed the canonical function input");
		final typedChanged = Context.typeExpr(macro {
			final value = 1;
			value + 3;
		});
		require(TypedFunctionSourceProvenance.canonicalTypedExpressionText(typedFirst) != TypedFunctionSourceProvenance.canonicalTypedExpressionText(typedChanged),
			"semantic expression change disappeared from the canonical function input");
	}

	/** Build the smallest valid, target-owned semantic function payload. */
	static function data(id:String):CBodyFunctionReplayData {
		final source = new HxcSourceSpan("test/hxc_ir/BodyFunctionReplayCacheProbe.hx", 1, 1, 1, 2);
		final ir:HxcIRFunction = {
			id: id,
			displayName: id,
			parameters: [],
			borrowedClassParameterIds: [],
			borrowedInterfaceParameterIds: [],
			mutableAggregateBorrowParameterIds: [],
			borrowedClassLocalIds: [],
			borrowedInterfaceLocalIds: [],
			mutableAggregateBorrowLocalIds: [],
			managedRoots: [],
			locals: [],
			returnType: IRTVoid,
			borrowedSpanReturn: null,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [],
					terminator: {
						kind: IRTReturn(null, []),
						source: source
					},
					source: source
				}
			],
			cleanupRegions: [],
			source: source
		};
		return {
			ir: ir,
			localRequests: [],
			spanLengthRequests: [],
			temporaryRequests: [],
			tailArgumentRequests: [],
			labelRequests: [],
			enumReasons: [],
			runtimeRequirements: []
		};
	}

	/**
		Exercise position recovery with the replay API's runtime-requirement record.
		The lowering class adds a sorting key that this cache neither stores nor
		restores; constructing the exact record keeps this test focused on replay.
	**/
	static function positionedData(id:String):CBodyFunctionReplayData {
		final result = data(id);
		result.runtimeRequirements.push({
			featureId: "gc",
			operationId: "probe",
			surface: "position recovery probe",
			source: result.ir.source,
			position: Context.currentPos(),
			kind: "runtime-operation"
		});
		return result;
	}

	/** Construct one request-local exact input with no diagnostic positions. */
	static function identity(input:String):CBodyFunctionReplayIdentity
		return new CBodyFunctionReplayIdentity(input, []);

	/** Construct the current position ledger required by `positionedData`. */
	static function positionedIdentity(input:String):CBodyFunctionReplayIdentity {
		final source = new HxcSourceSpan("test/hxc_ir/BodyFunctionReplayCacheProbe.hx", 1, 1, 1, 2);
		final positions = new Map<String, Position>();
		positions.set(source.display(), Context.currentPos());
		return new CBodyFunctionReplayIdentity(input, positions);
	}

	/** Stop compilation at the first replay-contract violation. */
	static function require(condition:Bool, detail:String):Void {
		if (!condition)
			throw new haxe.Exception(detail);
	}
}
