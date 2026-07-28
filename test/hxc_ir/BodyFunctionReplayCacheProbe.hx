import haxe.macro.Expr;
import haxe.macro.Expr.Position;
import haxe.macro.Context;
import haxe.macro.TypedExprTools;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyRuntimeRequirement;
import reflaxe.c.lowering.CBodyFunctionReplayCache;
import reflaxe.c.lowering.CBodyFunctionReplayCache.CBodyFunctionReplayData;
import reflaxe.c.lowering.CBodyFunctionReplayCache.CBodyFunctionReplayIdentity;
import reflaxe.c.lowering.CBodyFunctionReplayCache.CBodyFunctionReplayProgramDecision;

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
		identical.data.ir.blocks.resize(0);
		CBodyFunctionReplayCache.completeRequest();

		CBodyFunctionReplayCache.beginRequest(true);
		CBodyFunctionReplayCache.settleProgramRevision("program-a");
		final afterMutation = CBodyFunctionReplayCache.resolve("function.demo", identity("input-a"), () -> build("unexpected"));
		require(afterMutation.reused
			&& afterMutation.data.ir.blocks.length == 1, "caller mutation escaped into the persistent generation");
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

		return macro null;
	}

	/** Prove variable allocation is ignored while a real expression edit is kept. */
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
		require(CBodyLowering.canonicalTypedExpressionText(typedFirst) == CBodyLowering.canonicalTypedExpressionText(typedSecond),
			"request-local variable allocation changed the canonical function input");
		final typedChanged = Context.typeExpr(macro {
			final value = 1;
			value + 3;
		});
		require(CBodyLowering.canonicalTypedExpressionText(typedFirst) != CBodyLowering.canonicalTypedExpressionText(typedChanged),
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
			borrowedClassLocalIds: [],
			borrowedInterfaceLocalIds: [],
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

	/** Add one diagnostic-bearing runtime requirement to exercise position recovery. */
	static function positionedData(id:String):CBodyFunctionReplayData {
		final result = data(id);
		result.runtimeRequirements.push(new CBodyRuntimeRequirement("gc", "probe", "position recovery probe", result.ir.source, Context.currentPos()));
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
