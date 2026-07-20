#if macro
import haxe.Json;
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.c.CProfile;
import reflaxe.c.CompilationContext;
import reflaxe.c.ast.CAST;
import reflaxe.c.ast.CASTPrinter;
import reflaxe.c.emit.CStaticFunctionProjectEmitter;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CBodyLowering.CBodyGlobalInput;
import reflaxe.c.lowering.CBodyControlFlow.CBodyBranchProof;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowCompletion;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowNode;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlan;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlanner;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowRegion;
import reflaxe.c.lowering.CBodyControlFlow.CBodyControlFlowPlanVerifier;
import reflaxe.c.lowering.CBodyControlFlow.CBodyGotoReason;
import reflaxe.c.lowering.CBodyEmitter;
import reflaxe.c.lowering.CBodyEmissionError;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueCoalescingPlanner;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueDisposition;
import reflaxe.c.lowering.CBodyValueCoalescing.CBodyValueMaterializationReason;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CDeclarationPlanner;

typedef EvaluationFunctionRecord = {
	final field:String;
	final cName:String;
	final blocks:Array<String>;
	final temporaryValues:Array<String>;
	final temporaries:Array<String>;
	final labels:Array<String>;
}

typedef EvaluationGlobalRecord = {
	final id:String;
	final cName:String;
	final mutable:Bool;
}

typedef EvaluationSourceRecord = {
	final path:String;
	final content:String;
}

typedef EvaluationControlFlowProof = {
	final summary:String;
	final emissionC:String;
}

typedef EvaluationOrderReport = {
	final schemaVersion:Int;
	final status:String;
	final profile:String;
	final hxcir:String;
	final header:String;
	final sources:Array<EvaluationSourceRecord>;
	final functions:Array<EvaluationFunctionRecord>;
	final globals:Array<EvaluationGlobalRecord>;
	final symbols:reflaxe.c.naming.CSymbolRegistry.CSymbolTableSnapshot;
	final runtimeFeatures:Array<String>;
	final temporaryElisionProof:String;
	final valueCoalescingProof:String;
	final controlFlowPlanProof:String;
	final controlFlowEmissionC:String;
}
#end

class EvaluationOrderProbe {
	#if macro
	public static inline final REPORT_PREFIX = "HXC_EVALUATION_ORDER=";

	public static function install():Void {
		Context.onAfterInitMacros(run);
	}

	static function run():Void {
		final fixture = switch Context.getType("EvaluationFixture") {
			case TInst(reference, _): reference.get();
			case _: fatal("EvaluationFixture must remain a class", Context.currentPos());
		};
		final inputs:Array<CBodyFunctionInput> = [];
		final globals:Array<CBodyGlobalInput> = [];
		for (sourceOrder => field in fixture.statics.get()) {
			switch field.kind {
				case FMethod(_):
					final expression = switch field.expr() {
						case null: fatal('evaluation-order function `${field.name}` has no typed expression', field.pos);
						case value: value;
					};
					inputs.push({
						modulePath: fixture.module,
						declarationPath: fixture.pack.concat([fixture.name]).join("."),
						sourcePath: "test/evaluation_order/fixtures/EvaluationFixture.hx",
						fieldName: field.name,
						sourceOrder: sourceOrder,
						fieldType: field.type,
						expression: expression
					});
				case FVar(_, write):
					globals.push({
						modulePath: fixture.module,
						declarationPath: fixture.pack.concat([fixture.name]).join("."),
						sourcePath: "test/evaluation_order/fixtures/EvaluationFixture.hx",
						fieldName: field.name,
						sourceOrder: sourceOrder,
						fieldType: field.type,
						mutable: write != AccNever,
						position: field.pos,
						expression: field.expr()
					});
			}
		}
		if (Context.defined("evaluation_order_reverse_input")) {
			inputs.reverse();
			globals.reverse();
		}
		final profile = Context.definedValue("evaluation_order_profile") == "metal" ? CProfile.Metal : CProfile.Portable;
		final context = new CompilationContext(profile);
		final entryId = CBodyLowering.functionId("EvaluationFixture", "main");
		final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", entryId], CNSOrdinary("translation-unit"),
			CSVInternal, "main");
		final guardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
			CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
		context.symbols.register(entryRequest);
		context.symbols.register(guardRequest);
		final lowered = new CBodyLowering(context).lower(inputs, globals);
		final project = new CStaticFunctionProjectEmitter().plan(lowered, entryId, context.symbols.identifierFor(entryRequest),
			context.symbols.identifierFor(guardRequest));
		final functions:Array<EvaluationFunctionRecord> = [];
		for (fn in lowered.functions) {
			final temporaryValues = [for (valueId in fn.temporaryNames.keys()) valueId];
			temporaryValues.sort(compareStrings);
			final temporaries = [for (name in fn.temporaryNames) name.value];
			temporaries.sort(compareStrings);
			final labels = [for (name in fn.labelNames) name.value];
			labels.sort(compareStrings);
			functions.push({
				field: fn.fieldName,
				cName: fn.cName.value,
				blocks: fn.ir.blocks.map(block -> block.id),
				temporaryValues: temporaryValues,
				temporaries: temporaries,
				labels: labels
			});
		}
		functions.sort((left, right) -> compareStrings(left.field, right.field));
		final globals:Array<EvaluationGlobalRecord> = lowered.globals.map(global -> {
			id: global.ir.id,
			cName: global.cName.value,
			mutable: global.ir.mutable
		});
		globals.sort((left, right) -> compareStrings(left.id, right.id));
		final printer = new CASTPrinter();
		final controlFlowProof = proveControlFlowPlans();
		final sources:Array<EvaluationSourceRecord> = project.sources.map(source -> {
			path: source.path,
			content: printer.printTranslationUnit(source.unit)
		});
		final report:EvaluationOrderReport = {
			schemaVersion: 2,
			status: "typed-evaluation-order-runtime-free",
			profile: Std.string(profile),
			hxcir: new HxcIRDumper().dump(lowered.program),
			header: printer.printHeader(project.header),
			sources: sources,
			functions: functions,
			globals: globals,
			symbols: lowered.symbolTable,
			runtimeFeatures: [],
			temporaryElisionProof: "constants, parameters, proven-total pure conversions/operators, single-use compiler-owned aggregate values, and barrier-free single-use private-local loads may remain inline; calls and observable/cross-block/barrier loads stay materialized",
			valueCoalescingProof: proveValueCoalescing(),
			controlFlowPlanProof: controlFlowProof.summary,
			controlFlowEmissionC: controlFlowProof.emissionC
		};
		Sys.println(REPORT_PREFIX + Json.stringify(report));
	}

	static function proveValueCoalescing():String {
		final source = new HxcSourceSpan("test/evaluation_order/synthetic-value-coalescing.hx", 1, 1, 1, 2);
		final i32 = IRTInt(32, true);
		final recordType = IRTInstance("synthetic.Record");
		final pointerType = IRTPointer(i32, false);
		final localValue = syntheticLocal("local.value", i32, source);
		final localOther = syntheticLocal("local.other", i32, source);
		final localRecord = syntheticLocal("local.record", recordType, source);
		final localArray = syntheticLocal("local.items", IRTFixedArray(i32, 4, "synthetic.items.4"), source);
		final localPointer = syntheticLocal("local.pointer", pointerType, source);
		final candidate:HxcIRParameter = {id: "candidate", type: i32, source: source};
		final index:HxcIRParameter = {id: "index", type: i32, source: source};
		final pointer:HxcIRParameter = {id: "pointer", type: pointerType, source: source};
		final planner = new CBodyValueCoalescingPlanner();

		requireValueDisposition("single-use private local", planner.plan(syntheticValueFunction("synthetic.coalesce.local", i32, [], [localValue], [
			syntheticBlock("entry", IRTReturn("value.local", []), source, [
				valueInstruction("load-local", "value.local", i32, IRIOLoad(IRPLocal("local.value")), source)
			])
		], source)).disposition("value.local"), "inline-sequenced-load");
		requireValueDisposition("single-use private field", planner.plan(syntheticValueFunction("synthetic.coalesce.field", i32, [], [localRecord], [
			syntheticBlock("entry", IRTReturn("value.field", []), source, [
				valueInstruction("load-field", "value.field", i32, IRIOLoad(IRPField(IRPLocal("local.record"), "x")), source)
			])
		], source)).disposition("value.field"), "inline-sequenced-load");

		requireValueDisposition("intervening read", planner.plan(syntheticValueFunction("synthetic.coalesce.read-barrier", i32, [], [localValue, localOther], [
			syntheticBlock("entry", IRTReturn("value.first", []), source, [
				valueInstruction("load-first", "value.first", i32, IRIOLoad(IRPLocal("local.value")), source),
				valueInstruction("load-other", "value.other", i32, IRIOLoad(IRPLocal("local.other")), source)
			])
		], source)).disposition("value.first"), "materialize:effect-or-read:load-other");
		requireValueDisposition("intervening foreign call", planner.plan(syntheticValueFunction("synthetic.coalesce.call-barrier", i32, [], [localValue], [
			syntheticBlock("entry", IRTReturn("value.before-call", []), source, [
				valueInstruction("load-before-call", "value.before-call", i32, IRIOLoad(IRPLocal("local.value")), source),
				effectInstruction("foreign-call", IRIOCall({
					dispatch: IRCDNative("synthetic_observe"),
					arguments: [],
					returnType: IRTVoid,
					failure: null
				}), source)
			])
		],
			source)).disposition("value.before-call"), "materialize:effect-or-read:foreign-call");
		requireValueDisposition("intervening lifetime change",
			planner.plan(syntheticValueFunction("synthetic.coalesce.lifetime-barrier", i32, [], [localValue], [
				syntheticBlock("entry", IRTReturn("value.before-lifetime", []), source, [
					valueInstruction("load-before-lifetime", "value.before-lifetime", i32, IRIOLoad(IRPLocal("local.value")), source),
					effectInstruction("end-lifetime", IRIOLifetime(IRPLocal("local.value"), IRISInitialized, IRISMoved, "synthetic move"), source)
				])
			],
				source)).disposition("value.before-lifetime"), "materialize:failure-or-lifetime");
		requireValueDisposition("return-edge cleanup", planner.plan(syntheticValueFunction("synthetic.coalesce.return-cleanup", i32, [], [localValue], [
			syntheticBlock("entry", IRTReturn("value.before-cleanup", [
				{
					regionId: "cleanup.local",
					actionId: "destroy-local"
				}
			]), source, [
				valueInstruction("load-before-cleanup", "value.before-cleanup", i32, IRIOLoad(IRPLocal("local.value")), source)
			])
		], source, [
			{
				id: "cleanup.local",
				parentId: null,
				actions: [
					{
						id: "destroy-local",
						idempotence: IRCExactlyOnce,
						kind: IRCADestroy(IRPLocal("local.value"), IRISInitialized, IRISDestroyed),
						source: source
					}
				],
				source: source
			}
		])).disposition("value.before-cleanup"), "materialize:failure-or-lifetime");
		requireValueDisposition("intervening checked failure",
			planner.plan(syntheticValueFunction("synthetic.coalesce.failure-barrier", i32, [candidate], [localValue], [
				syntheticBlock("entry", IRTReturn("value.before-failure", []), source, [
					valueInstruction("load-before-failure", "value.before-failure", i32, IRIOLoad(IRPLocal("local.value")), source),
					valueInstruction("checked-convert", "value.checked", i32, IRIOConvert("candidate", IRCNumericChecked, i32, IRIStatic, {
						kind: IRFResultError,
						target: IRFTAbort,
						arguments: [],
						cleanup: []
					}), source)
				])
			],
				source)).disposition("value.before-failure"), "materialize:failure-or-lifetime");

		requireValueDisposition("global read", planner.plan(syntheticValueFunction("synthetic.coalesce.global", i32, [], [], [
			syntheticBlock("entry", IRTReturn("value.global", []), source, [
				valueInstruction("load-global", "value.global", i32, IRIOLoad(IRPGlobal("global.value")), source)
			])
		], source)).disposition("value.global"), "materialize:mutable-or-foreign-place");
		requireValueDisposition("foreign or volatile pointer read", planner.plan(syntheticValueFunction("synthetic.coalesce.pointer", i32, [pointer], [], [
			syntheticBlock("entry", IRTReturn("value.pointer", []), source, [
				valueInstruction("load-pointer", "value.pointer", i32, IRIOLoad(IRPDereference("pointer")), source)
			])
		], source)).disposition("value.pointer"), "materialize:mutable-or-foreign-place");
		requireValueDisposition("indexed read", planner.plan(syntheticValueFunction("synthetic.coalesce.index", i32, [index], [localArray], [
			syntheticBlock("entry", IRTReturn("value.indexed", []), source, [
				valueInstruction("load-indexed", "value.indexed", i32, IRIOLoad(IRPIndex(IRPLocal("local.items"), "index")), source)
			])
		], source)).disposition("value.indexed"), "materialize:mutable-or-foreign-place");

		requireValueDisposition("multiple reads of one value", planner.plan(syntheticValueFunction("synthetic.coalesce.multiple-use", i32, [], [localValue], [
			syntheticBlock("entry", IRTReturn("value.sum", []), source, [
				valueInstruction("load-twice", "value.twice", i32, IRIOLoad(IRPLocal("local.value")), source),
				valueInstruction("add-twice", "value.sum", i32, IRIOBinary("haxe.i32.add", "value.twice", "value.twice", IRIStatic), source)
			])
		], source)).disposition("value.twice"), "materialize:multiple-uses:2");
		requireValueDisposition("expression fanout", planner.plan(syntheticValueFunction("synthetic.coalesce.fanout", i32, [candidate], [localValue], [
			syntheticBlock("entry", IRTReturn("value.negative", []), source, [
				valueInstruction("load-for-fanout", "value.fanout-load", i32, IRIOLoad(IRPLocal("local.value")), source),
				valueInstruction("sum-for-fanout", "value.fanout-sum", i32, IRIOBinary("haxe.i32.add", "value.fanout-load", "candidate", IRIStatic), source),
				valueInstruction("negate-first", "value.negative", i32, IRIOUnary("haxe.i32.negate", "value.fanout-sum", IRIStatic), source),
				valueInstruction("negate-second", "value.negative-unused", i32, IRIOUnary("haxe.i32.negate", "value.fanout-sum", IRIStatic), source)
			])
		], source)).disposition("value.fanout-load"), "materialize:expression-fanout");

		requireValueDisposition("single-use compiler record",
			planner.plan(syntheticValueFunction("synthetic.coalesce.aggregate", recordType, [candidate], [], [
				syntheticBlock("entry", IRTReturn("value.record", []), source, [
					valueInstruction("construct-record", "value.record", recordType, IRIOConstructAggregate("synthetic.Record", [
						{
							name: "x",
							valueId: "candidate"
						}
					]), source)
				])
			], source)).disposition("value.record"), "inline-pure");
		requireValueDisposition("multiply-used compiler record",
			planner.plan(syntheticValueFunction("synthetic.coalesce.aggregate-multiple", i32, [candidate], [], [
				syntheticBlock("entry", IRTReturn("value.first-field", []), source, [
					valueInstruction("construct-shared-record", "value.shared-record", recordType, IRIOConstructAggregate("synthetic.Record",
						[
							{
								name: "x",
								valueId: "candidate"
							}
						]),
						source),
					valueInstruction("project-first", "value.first-field", i32, IRIOProject("value.shared-record", "x"), source),
					valueInstruction("project-second", "value.second-field", i32, IRIOProject("value.shared-record", "x"), source)
				])
			], source)).disposition("value.shared-record"), "materialize:multiple-uses:2");
		requireValueDisposition("cross-block compiler record",
			planner.plan(syntheticValueFunction("synthetic.coalesce.aggregate-cross-block", i32, [candidate], [], [
				syntheticBlock("entry", IRTJump(plainEdge("consumer")), source, [
					valueInstruction("construct-cross-record", "value.cross-record", recordType, IRIOConstructAggregate("synthetic.Record", [
						{
							name: "x",
							valueId: "candidate"
						}
					]), source)
				]),
				syntheticBlock("consumer", IRTReturn("value.cross-field", []), source, [
					valueInstruction("project-cross-record", "value.cross-field", i32, IRIOProject("value.cross-record", "x"), source)
				])
			], source)).disposition("value.cross-record"), "materialize:cross-block");
		requireValueDisposition("consumed call result", planner.plan(syntheticValueFunction("synthetic.coalesce.call-result", i32, [], [], [
			syntheticBlock("entry", IRTReturn("value.call", []), source, [
				valueInstruction("call-result", "value.call", i32, IRIOCall({
					dispatch: IRCDNative("synthetic_value"),
					arguments: [],
					returnType: i32,
					failure: null
				}), source)
			])
		], source)).disposition("value.call"), "materialize:unsupported-producer");
		// Reuse the same planner once more. This is a regression check that the
		// previous functions' maps cannot influence a fresh function.
		requireValueDisposition("planner reuse", planner.plan(syntheticValueFunction("synthetic.coalesce.reuse", i32, [], [localPointer], [
			syntheticBlock("entry", IRTReturn("value.reused-int", []), source, [
				valueInstruction("load-reused", "value.reused", pointerType, IRIOLoad(IRPLocal("local.pointer")), source),
				valueInstruction("pointer-to-int", "value.reused-int", i32, IRIOConvert("value.reused", IRCPointer, i32, IRIStatic, null), source)
			])
		], source)).disposition("value.reused"), "inline-sequenced-load");

		return "value-coalescing:single-use-private-local-and-field-and-pure-record-inline;"
			+ "read-call-lifetime-cleanup-failure-global-pointer-index-multiuse-fanout-cross-block-and-call-result-materialized;planner-reuse-isolated";
	}

	static function requireValueDisposition(label:String, actual:CBodyValueDisposition, expected:String):Void {
		final rendered = valueDispositionName(actual);
		if (rendered != expected)
			throw new haxe.Exception('value-coalescing proof `$label` expected `$expected` but received `$rendered`');
	}

	static function valueDispositionName(disposition:CBodyValueDisposition):String
		return switch disposition {
			case CBVDInlinePure: "inline-pure";
			case CBVDInlineSequencedLoad: "inline-sequenced-load";
			case CBVDMaterialize(reason):
				switch reason {
					case CBVMRMultipleUses(count): 'materialize:multiple-uses:$count';
					case CBVMRCrossBlock: "materialize:cross-block";
					case CBVMRMutableOrForeignPlace: "materialize:mutable-or-foreign-place";
					case CBVMREffectOrReadBarrier(instructionId): 'materialize:effect-or-read:$instructionId';
					case CBVMRExpressionFanout: "materialize:expression-fanout";
					case CBVMRFailureOrLifetimeBoundary: "materialize:failure-or-lifetime";
					case CBVMRUnsupportedProducer: "materialize:unsupported-producer";
				};
		};

	static function proveControlFlowPlans():EvaluationControlFlowProof {
		final source = new HxcSourceSpan("test/evaluation_order/synthetic-control-flow.hx", 1, 1, 1, 2);
		final condition:HxcIRParameter = {
			id: "condition",
			type: IRTBool,
			source: source
		};
		final diamond = syntheticFunction("synthetic.reducible-diamond", [condition], "entry", [
			syntheticBlock("entry", IRTBranch("condition", plainEdge("when-true"), plainEdge("when-false")), source),
			syntheticBlock("when-true", IRTJump(plainEdge("join")), source),
			syntheticBlock("when-false", IRTJump(plainEdge("join")), source),
			syntheticBlock("join", IRTReturn(null, []), source)
		], source);
		final planner = new CBodyControlFlowPlanner();
		final verifier = new CBodyControlFlowPlanVerifier();
		final diamondPlan = planner.plan(diamond);
		switch diamondPlan {
			case CCFStructured(_, _):
				verifier.requireValid(diamond, diamondPlan);
			case CCFLegacyIrreducible(_):
				throw new haxe.Exception("reducible diamond selected the irreducible fallback");
		}
		final normalJoinBranch = syntheticFunction("synthetic.normal-join-branch", [condition], "entry", [
			syntheticBlock("entry", IRTBranch("condition", plainEdge("nested"), plainEdge("common-tail")), source),
			syntheticBlock("nested", IRTBranch("condition", plainEdge("early-return"), plainEdge("common-tail")), source),
			syntheticBlock("early-return", IRTReturn(null, []), source),
			syntheticBlock("common-tail", IRTReturn(null, []), source)
		], source);
		requireStructured("branch with an abrupt arm and common normal tail", normalJoinBranch, planner.plan(normalJoinBranch), verifier);
		final sharedTargetBranch = syntheticFunction("synthetic.shared-target-branch", [condition], "entry", [
			syntheticBlock("entry", IRTBranch("condition", plainEdge("nested"), plainEdge("join")), source),
			syntheticBlock("nested", IRTBranch("condition", plainEdge("shared-arm"), plainEdge("shared-arm")), source),
			syntheticBlock("shared-arm", IRTJump(plainEdge("join")), source),
			syntheticBlock("join", IRTReturn(null, []), source)
		], source);
		requireStructured("nested branch with a shared target", sharedTargetBranch, planner.plan(sharedTargetBranch), verifier);
		final loopWithBreak = syntheticFunction("synthetic.loop-with-break", [condition], "loop-header", [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("loop-body"), plainEdge("exit")), source),
			syntheticBlock("loop-body", IRTBranch("condition", plainEdge("break-arm"), plainEdge("advance")), source),
			syntheticBlock("advance", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("break-arm", IRTJump(plainEdge("exit")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		final loopPlan = planner.plan(loopWithBreak);
		switch loopPlan {
			case CCFStructured(_, _):
				verifier.requireValid(loopWithBreak, loopPlan);
			case CCFLegacyIrreducible(_):
				throw new haxe.Exception("reducible loop with an early break selected the irreducible fallback");
		}
		final loopWithReturn = syntheticFunction("synthetic.loop-with-return", [condition], "loop-header", [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("loop-body"), plainEdge("exit")), source),
			syntheticBlock("loop-body", IRTBranch("condition", plainEdge("early-return"), plainEdge("advance")), source),
			syntheticBlock("advance", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("early-return", IRTReturn(null, []), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		final loopReturnPlan = planner.plan(loopWithReturn);
		switch loopReturnPlan {
			case CCFStructured(_, _):
				verifier.requireValid(loopWithReturn, loopReturnPlan);
			case CCFLegacyIrreducible(_):
				throw new haxe.Exception("reducible loop with an early return selected the irreducible fallback");
		}
		final directBreakNormalJoin = syntheticFunction("synthetic.direct-break-normal-join", [condition], "loop-header", [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("body-choice"), plainEdge("exit")), source),
			syntheticBlock("body-choice", IRTBranch("condition", plainEdge("exit"), plainEdge("normal-arm")), source),
			syntheticBlock("normal-arm", IRTBranch("condition", plainEdge("early-return"), plainEdge("advance")), source),
			syntheticBlock("advance", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("early-return", IRTReturn(null, []), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		requireStructured("direct loop break beside a normal join arm", directBreakNormalJoin, planner.plan(directBreakNormalJoin), verifier);
		final directContinueNormalJoin = syntheticFunction("synthetic.direct-continue-normal-join", [condition], "loop-header", [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("body-choice"), plainEdge("exit")), source),
			syntheticBlock("body-choice", IRTBranch("condition", plainEdge("loop-header"), plainEdge("normal-arm")), source),
			syntheticBlock("normal-arm", IRTBranch("condition", plainEdge("early-return"), plainEdge("advance")), source),
			syntheticBlock("advance", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("early-return", IRTReturn(null, []), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		requireStructured("direct loop continue beside a normal join arm", directContinueNormalJoin, planner.plan(directContinueNormalJoin), verifier);
		final convergingBreakBlocks = [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("first-choice"), plainEdge("exit")), source),
			syntheticBlock("first-choice", IRTBranch("condition", plainEdge("short-break"), plainEdge("second-choice")), source),
			syntheticBlock("second-choice", IRTBranch("condition", plainEdge("long-break"), plainEdge("advance")), source),
			syntheticBlock("advance", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("short-break", IRTJump(plainEdge("shared-break")), source),
			syntheticBlock("long-break", IRTJump(plainEdge("break-detour")), source),
			syntheticBlock("break-detour", IRTJump(plainEdge("shared-break")), source),
			syntheticBlock("shared-break", IRTJump(plainEdge("exit")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		];
		final convergingBreak = syntheticFunction("synthetic.converging-break-chains", [condition], "loop-header", convergingBreakBlocks, source);
		requireStructured("converging loop-break chains", convergingBreak, planner.plan(convergingBreak), verifier);
		final reorderedConvergingBreak = syntheticFunction("synthetic.converging-break-chains-reordered", [condition], "loop-header", [
			convergingBreakBlocks[0],
			convergingBreakBlocks[1],
			convergingBreakBlocks[2],
			convergingBreakBlocks[3],
			convergingBreakBlocks[5],
			convergingBreakBlocks[6],
			convergingBreakBlocks[4],
			convergingBreakBlocks[7],
			convergingBreakBlocks[8]
		], source);
		requireStructured("reordered converging loop-break chains", reorderedConvergingBreak, planner.plan(reorderedConvergingBreak), verifier);
		final invertedPreTest = syntheticFunction("synthetic.inverted-pre-test", [condition], "loop-header", [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("exit"), plainEdge("loop-body")), source),
			syntheticBlock("loop-body", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		requireInvertedLoop("pre-test", invertedPreTest, planner.plan(invertedPreTest), false, verifier);
		final invertedPostTest = syntheticFunction("synthetic.inverted-post-test", [condition], "loop-body", [
			syntheticBlock("loop-body", IRTJump(plainEdge("loop-decision")), source),
			syntheticBlock("loop-decision", IRTBranch("condition", plainEdge("exit"), plainEdge("loop-body")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		requireInvertedLoop("post-test", invertedPostTest, planner.plan(invertedPostTest), true, verifier);
		final selector:HxcIRParameter = {
			id: "selector",
			type: IRTInt(32, true),
			source: source
		};
		final normalJoinSwitch = syntheticFunction("synthetic.normal-join-switch", [selector], "entry", [
			syntheticBlock("entry", IRTSwitch("selector", [
				{value: IRCInt("1"), edge: plainEdge("early-return")},
				{value: IRCInt("2"), edge: plainEdge("first-arm")}
			],
				plainEdge("second-arm")),
				source),
			syntheticBlock("early-return", IRTReturn(null, []), source),
			syntheticBlock("first-arm", IRTJump(plainEdge("common-tail")), source),
			syntheticBlock("second-arm", IRTJump(plainEdge("common-tail")), source),
			syntheticBlock("common-tail", IRTReturn(null, []), source)
		], source);
		requireStructured("switch with an abrupt arm and common normal tail", normalJoinSwitch, planner.plan(normalJoinSwitch), verifier);
		final loopSwitchBreak = syntheticFunction("synthetic.loop-switch-break", [condition, selector], "loop-header", [
			syntheticBlock("loop-header", IRTBranch("condition", plainEdge("dispatch"), plainEdge("exit")), source),
			syntheticBlock("dispatch", IRTSwitch("selector", [
				{
					value: IRCInt("1"),
					edge: plainEdge("exit")
				}
			],
				plainEdge("advance")), source),
			syntheticBlock("advance", IRTJump(plainEdge("loop-header")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		final loopSwitchPlan = planner.plan(loopSwitchBreak);
		switch loopSwitchPlan {
			case CCFStructured(root, labeledTargets):
				verifier.requireValid(loopSwitchBreak, loopSwitchPlan);
				if (labeledTargets.join(",") != "exit" || countBoundedSwitchGotos(root) != 1)
					throw new haxe.Exception("loop switch break did not retain its single bounded structural escape");
			case CCFLegacyIrreducible(_):
				throw new haxe.Exception("reducible loop switch break selected the irreducible fallback");
		}

		final irreducible = syntheticFunction("synthetic.irreducible", [condition], "entry", [
			syntheticBlock("entry", IRTBranch("condition", plainEdge("left-entry"), plainEdge("right-entry")), source),
			syntheticBlock("left-entry", IRTJump(plainEdge("right-entry")), source),
			syntheticBlock("right-entry", IRTBranch("condition", plainEdge("left-entry"), plainEdge("exit")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		final irreduciblePlan = planner.plan(irreducible);
		switch irreduciblePlan {
			case CCFLegacyIrreducible(entries):
				if (entries.join(",") != "left-entry,right-entry")
					throw new haxe.Exception('irreducible entry proof drifted: ${entries.join(",")}');
				verifier.requireValid(irreducible, irreduciblePlan);
			case CCFStructured(_, _):
				throw new haxe.Exception("irreducible graph was incorrectly structuralized");
		}
		final nestedIrreducible = syntheticFunction("synthetic.nested-irreducible", [condition, selector], "entry", [
			syntheticBlock("entry", IRTSwitch("selector", [
				{
					value: IRCInt("1"),
					edge: plainEdge("left-entry")
				},
				{value: IRCInt("2"), edge: plainEdge("right-entry")}
			],
				plainEdge("exit")),
				source),
			syntheticBlock("left-entry", IRTJump(plainEdge("right-entry")), source),
			syntheticBlock("right-entry", IRTBranch("condition", plainEdge("left-entry"), plainEdge("entry")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		final nestedIrreduciblePlan = planner.plan(nestedIrreducible);
		switch nestedIrreduciblePlan {
			case CCFLegacyIrreducible(entries):
				if (entries.join(",") != "left-entry,right-entry")
					throw new haxe.Exception('nested irreducible entry proof drifted: ${entries.join(",")}');
				verifier.requireValid(nestedIrreducible, nestedIrreduciblePlan);
			case CCFStructured(_, _):
				throw new haxe.Exception("nested irreducible graph was incorrectly structuralized");
		}
		requirePlanFailure("overlapping-normal-join-prefixes",
			() -> planner.plan(syntheticFunction("synthetic.overlapping-normal-join-prefixes", [condition, selector], "entry", [
				syntheticBlock("entry", IRTSwitch("selector", [
					{
						value: IRCInt("1"),
						edge: plainEdge("first-arm")
					},
					{value: IRCInt("2"), edge: plainEdge("second-arm")}
				],
					plainEdge("default-arm")),
					source),
				syntheticBlock("first-arm", IRTJump(plainEdge("split")), source),
				syntheticBlock("split", IRTBranch("condition", plainEdge("first-return"), plainEdge("second-return")), source),
				syntheticBlock("second-arm", IRTJump(plainEdge("first-return")), source),
				syntheticBlock("default-arm", IRTJump(plainEdge("second-return")), source),
				syntheticBlock("first-return", IRTReturn(null, []), source),
				syntheticBlock("second-return", IRTReturn(null, []), source)
			], source)));

		requirePlanFailure("unreachable-block", () -> planner.plan(syntheticFunction("synthetic.unreachable", [], "entry", [
			syntheticBlock("entry", IRTReturn(null, []), source),
			syntheticBlock("orphan", IRTReturn(null, []), source)
		], source)));
		requirePlanFailure("edge-cleanup", () -> planner.plan(syntheticFunction("synthetic.edge-cleanup", [], "entry", [
			syntheticBlock("entry", IRTJump({
				targetBlockId: "exit",
				arguments: [],
				cleanup: [{regionId: "region", actionId: "action"}]
			}), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source)));
		requirePlanFailure("instruction-failure-block", () -> planner.plan(syntheticFunction("synthetic.instruction-failure-block", [], "entry", [
			syntheticBlock("entry", IRTJump(plainEdge("failure")), source, [
				{
					id: "failing-call",
					result: null,
					kind: IRIOCall({
						dispatch: IRCDNative("synthetic_status"),
						arguments: [],
						returnType: IRTVoid,
						failure: {
							kind: IRFNativeStatus,
							target: IRFTBlock("failure"),
							arguments: [],
							cleanup: []
						}
					}),
					source: source
				}
			]),
			syntheticBlock("failure", IRTReturn(null, []), source)
		], source)));
		requirePlanFailure("malformed-region", () -> verifier.requireValid(diamond, CCFStructured(new CBodyControlFlowRegion([], CFCClosed), [])));
		requirePlanFailure("swapped-branch-edges", () -> verifier.requireValid(diamond, CCFStructured(new CBodyControlFlowRegion([
			CFNIf("entry", "condition", new CBodyControlFlowRegion([CFNBlock("when-false")], CFCFallthrough),
				new CBodyControlFlowRegion([CFNBlock("when-true")], CFCFallthrough), CBPPostDominator("join")),
			CFNBlock("join")
		], CFCReturn("join")), [])));
		final sequence = syntheticFunction("synthetic.sequence", [], "entry", [
			syntheticBlock("entry", IRTJump(plainEdge("middle")), source),
			syntheticBlock("middle", IRTJump(plainEdge("exit")), source),
			syntheticBlock("exit", IRTReturn(null, []), source)
		], source);
		requirePlanFailure("reordered-sequence",
			() -> verifier.requireValid(sequence,
				CCFStructured(new CBodyControlFlowRegion([CFNBlock("middle"), CFNBlock("entry"), CFNBlock("exit")], CFCReturn("exit")), [])));

		return {
			summary: "typed-region-plan:reducible-diamond-normal-joins-loop-break-return-converging-escapes-inverted-pre-post-and-bounded-switch-escape-structured;maximal-and-nested-irreducible-fallback;overlapping-normal-join-malformed-unreachable-cleanup-and-instruction-failure-region-edge-mapping-and-sequence-order-rejected",
			emissionC: syntheticControlFlowEmission(loopSwitchBreak, nestedIrreducible)
		};
	}

	static function requireStructured(label:String, fn:HxcIRFunction, plan:CBodyControlFlowPlan, verifier:CBodyControlFlowPlanVerifier):Void {
		switch plan {
			case CCFStructured(_, _):
				verifier.requireValid(fn, plan);
			case CCFLegacyIrreducible(_):
				throw new haxe.Exception('reducible $label selected the irreducible fallback');
		}
	}

	static function syntheticControlFlowEmission(bounded:HxcIRFunction, irreducible:HxcIRFunction):String {
		final unit = new CTranslationUnit();
		unit.includes.push({path: "stdbool.h", kind: System});
		unit.includes.push({path: "stdint.h", kind: System});
		final emitter = new CBodyEmitter();
		final boundedName = new CIdentifier("hxc_bounded_control_flow");
		final irreducibleName = new CIdentifier("hxc_legacy_control_flow");
		addSyntheticControlFlowFunction(unit, emitter, bounded, boundedName, "hxc_bounded_label");
		addSyntheticControlFlowFunction(unit, emitter, irreducible, irreducibleName, "hxc_legacy_label");
		unit.declarations.push(DFunction({
			storage: [],
			functionSpecifiers: [],
			returnType: new CType(TNativeInt(IRInt, true)),
			declarator: DFunction(DName(new CIdentifier("main")), FPPrototype([], false)),
			body: SBlock([
				SExpr(ECall(EIdentifier(boundedName), [EBool(false), EInt(CIntegerLiteral.decimal("0"))])),
				SExpr(ECall(EIdentifier(boundedName), [EBool(true), EInt(CIntegerLiteral.decimal("1"))])),
				SExpr(ECall(EIdentifier(irreducibleName), [EBool(false), EInt(CIntegerLiteral.decimal("0"))])),
				SReturn(EInt(CIntegerLiteral.decimal("0")))
			]),
			attributes: []
		}));
		return new CASTPrinter().printTranslationUnit(unit);
	}

	static function addSyntheticControlFlowFunction(unit:CTranslationUnit, emitter:CBodyEmitter, fn:HxcIRFunction, cName:CIdentifier, labelPrefix:String):Void {
		final parameterNames:Map<String, CIdentifier> = [];
		for (parameter in fn.parameters)
			parameterNames.set(parameter.id, new CIdentifier(parameter.id));
		final labelNames:Map<String, CIdentifier> = [];
		for (index => block in fn.blocks)
			labelNames.set(block.id, new CIdentifier('${labelPrefix}_$index'));
		final localNames:Map<String, CIdentifier> = [];
		final temporaryNames:Map<String, CIdentifier> = [];
		final functionNames:Map<String, CIdentifier> = [];
		final globalNames:Map<String, CIdentifier> = [];
		final helperNames:Map<String, CIdentifier> = [];
		final tailArgumentNames:Map<String, Array<CIdentifier>> = [];
		unit.declarations.push(DFunction({
			storage: [SStatic],
			functionSpecifiers: [],
			returnType: emitter.cType(fn.returnType),
			declarator: DFunction(DName(cName), FPPrototype(emitter.parameters(fn, parameterNames), false)),
			body: emitter.emitBody(fn, parameterNames, localNames, temporaryNames, functionNames, globalNames, helperNames, false, tailArgumentNames,
				labelNames),
			attributes: []
		}));
	}

	static function requireInvertedLoop(label:String, fn:HxcIRFunction, plan:CBodyControlFlowPlan, postTest:Bool, verifier:CBodyControlFlowPlanVerifier):Void {
		switch plan {
			case CCFStructured(root, _):
				verifier.requireValid(fn, plan);
				if (countLoopPolarity(root, postTest, false) != 1)
					throw new haxe.Exception('inverted $label loop lost its continue-on-false polarity');
			case CCFLegacyIrreducible(_):
				throw new haxe.Exception('reducible inverted $label loop selected the irreducible fallback');
		}
	}

	static function countLoopPolarity(region:CBodyControlFlowRegion, postTest:Bool, continuesWhenTrue:Bool):Int {
		var count = 0;
		for (node in region.nodes) {
			switch node {
				case CFNBlock(_):
				case CFNIf(_, _, whenTrue, whenFalse, _):
					count += countLoopPolarity(whenTrue, postTest, continuesWhenTrue);
					count += countLoopPolarity(whenFalse, postTest, continuesWhenTrue);
				case CFNWhile(_, _, _, actualPolarity, condition, body, _):
					if (!postTest && actualPolarity == continuesWhenTrue)
						count++;
					count += countLoopPolarity(condition, postTest, continuesWhenTrue);
					count += countLoopPolarity(body, postTest, continuesWhenTrue);
				case CFNDoWhile(_, _, _, actualPolarity, body, condition, _):
					if (postTest && actualPolarity == continuesWhenTrue)
						count++;
					count += countLoopPolarity(body, postTest, continuesWhenTrue);
					count += countLoopPolarity(condition, postTest, continuesWhenTrue);
				case CFNSwitch(_, _, arms, _) | CFNTagSwitch(_, _, arms, _):
					for (arm in arms)
						count += countLoopPolarity(arm.body, postTest, continuesWhenTrue);
			}
		}
		return count;
	}

	static function countBoundedSwitchGotos(region:CBodyControlFlowRegion):Int {
		var count = switch region.completion {
			case CFCGoto(_, _, CBGRLoopBreakThroughSwitch): 1;
			case _: 0;
		};
		for (node in region.nodes) {
			switch node {
				case CFNBlock(_):
				case CFNIf(_, _, whenTrue, whenFalse, _):
					count += countBoundedSwitchGotos(whenTrue);
					count += countBoundedSwitchGotos(whenFalse);
				case CFNWhile(_, _, _, _, condition, body, _):
					count += countBoundedSwitchGotos(condition);
					count += countBoundedSwitchGotos(body);
				case CFNDoWhile(_, _, _, _, body, condition, _):
					count += countBoundedSwitchGotos(body);
					count += countBoundedSwitchGotos(condition);
				case CFNSwitch(_, _, arms, _) | CFNTagSwitch(_, _, arms, _):
					for (arm in arms)
						count += countBoundedSwitchGotos(arm.body);
			}
		}
		return count;
	}

	static function syntheticFunction(id:String, parameters:Array<HxcIRParameter>, entryBlockId:String, blocks:Array<HxcIRBlock>,
			source:HxcSourceSpan):HxcIRFunction
		return {
			id: id,
			displayName: id,
			parameters: parameters,
			locals: [],
			returnType: IRTVoid,
			failureConvention: IRFCInfallible,
			entryBlockId: entryBlockId,
			blocks: blocks,
			cleanupRegions: [],
			source: source
		};

	static function syntheticValueFunction(id:String, returnType:HxcIRTypeRef, parameters:Array<HxcIRParameter>, locals:Array<HxcIRLocal>,
			blocks:Array<HxcIRBlock>, source:HxcSourceSpan, ?cleanupRegions:Array<HxcIRCleanupRegion>):HxcIRFunction
		return {
			id: id,
			displayName: id,
			parameters: parameters,
			locals: locals,
			returnType: returnType,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: blocks,
			cleanupRegions: cleanupRegions == null ? [] : cleanupRegions,
			source: source
		};

	static function syntheticLocal(id:String, type:HxcIRTypeRef, source:HxcSourceSpan):HxcIRLocal
		return {
			id: id,
			type: type,
			storage: IRLSAutomatic,
			initialState: IRISInitialized,
			source: source
		};

	static function valueInstruction(id:String, valueId:String, type:HxcIRTypeRef, kind:HxcIRInstructionKind, source:HxcSourceSpan):HxcIRInstruction
		return {
			id: id,
			result: {id: valueId, type: type},
			kind: kind,
			source: source
		};

	static function effectInstruction(id:String, kind:HxcIRInstructionKind, source:HxcSourceSpan):HxcIRInstruction
		return {
			id: id,
			result: null,
			kind: kind,
			source: source
		};

	static function syntheticBlock(id:String, kind:HxcIRTerminatorKind, source:HxcSourceSpan, ?instructions:Array<HxcIRInstruction>):HxcIRBlock
		return {
			id: id,
			parameters: [],
			instructions: instructions == null ? [] : instructions,
			terminator: {kind: kind, source: source},
			source: source
		};

	static function plainEdge(targetBlockId:String):HxcIRBlockEdge
		return {
			targetBlockId: targetBlockId,
			arguments: [],
			cleanup: []
		};

	static function requirePlanFailure(label:String, action:Void->Void):Void {
		var rejected = false;
		try {
			action();
		} catch (_:CBodyEmissionError) {
			rejected = true;
		}
		if (!rejected)
			throw new haxe.Exception('control-flow planner accepted malformed `$label` input');
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);

	static function fatal<T>(message:String, position:haxe.macro.Expr.Position):T {
		Context.fatalError(message, position);
		throw new haxe.Exception("Context.fatalError returned after an evaluation-order fixture diagnostic");
	}
	#else
	public static function install():Void {}
	#end
}
