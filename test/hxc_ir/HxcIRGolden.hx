import haxe.Json;
import reflaxe.c.ir.HxcIR;
import reflaxe.c.ir.HxcIRDiagnostic;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.ir.HxcIRValidator;
import reflaxe.c.ir.HxcSourceSpan;

/** Builds deterministic semantic IR fixtures without invoking C emission. */
class HxcIRGolden {
	static inline final REPORT_PREFIX = "HXC_IR_REPORT=";
	static inline final PROFILE = "portable";
	static inline final MAIN_SOURCE = "test/hxc_ir/fixtures/SideEffects.hx";
	static inline final SUPPORT_SOURCE = "test/hxc_ir/fixtures/SideEffectSupport.hx";
	static inline final COVERAGE_SOURCE = "test/hxc_ir/fixtures/IRCoverage.hx";

	static function main():Void {
		final validator = new HxcIRValidator();
		final dumper = new HxcIRDumper();
		final semantic = semanticProgram(false);
		validator.requireValid(semantic, PROFILE);
		final semanticDump = dumper.dump(semantic);
		final reorderedDump = dumper.dump(semanticProgram(true));
		if (semanticDump != reorderedDump) {
			throw "HxcIR dump changed when unordered program collections were reversed";
		}

		final coverage = coverageProgram();
		validator.requireValid(coverage, PROFILE);
		validator.requireValid(nativeConstantAggregateProgram(), PROFILE);
		final coverageDump = dumper.dump(coverage);

		Sys.println(REPORT_PREFIX + Json.stringify({
			semantic: semanticDump,
			coverage: coverageDump,
			diagnostics: {
				missingTerminator: invalidDiagnostics(missingTerminatorProgram()),
				constantTypeMismatch: invalidDiagnostics(constantTypeMismatchProgram()),
				loadTypeMismatch: invalidDiagnostics(loadTypeMismatchProgram()),
				addressTypeMismatch: invalidDiagnostics(addressTypeMismatchProgram()),
				aggregateConstructionMismatch: invalidDiagnostics(aggregateConstructionMismatchProgram()),
				aggregateProjectionMismatch: invalidDiagnostics(aggregateProjectionMismatchProgram()),
				aggregateFieldPlaceMismatch: invalidDiagnostics(aggregateFieldPlaceMismatchProgram()),
				tagConstructionMismatch: invalidDiagnostics(tagConstructionMismatchProgram()),
				tagProjectionMismatch: invalidDiagnostics(tagProjectionMismatchProgram()),
				directPayloadRepresentation: invalidDiagnostics(directPayloadRepresentationProgram()),
				orphanVirtualSlot: invalidDiagnostics(orphanVirtualSlotProgram()),
				unknownVirtualImplementation: invalidDiagnostics(unknownVirtualImplementationProgram()),
				mismatchedVirtualTableBind: invalidDiagnostics(mismatchedVirtualTableBindProgram()),
				uncheckedVirtualCall: invalidDiagnostics(uncheckedVirtualCallProgram()),
				nonExhaustiveTagSwitch: invalidDiagnostics(nonExhaustiveTagSwitchProgram()),
				redundantDefaultTagSwitch: invalidDiagnostics(redundantDefaultTagSwitchProgram()),
				recursiveDirectLayout: invalidDiagnostics(recursiveDirectLayoutProgram()),
				uncheckedClassDereference: invalidDiagnostics(uncheckedClassDereferenceProgram()),
				unsafeClassUpcast: invalidDiagnostics(unsafeClassUpcastProgram()),
				mismatchedClassEquality: invalidDiagnostics(mismatchedClassEqualityProgram()),
				storeTypeMismatch: invalidDiagnostics(storeTypeMismatchProgram()),
				switchCaseTypeMismatch: invalidDiagnostics(switchCaseTypeMismatchProgram()),
				initializerTypeMismatch: invalidDiagnostics(initializerTypeMismatchProgram()),
				deferredInitializerMissingWrite: invalidDiagnostics(deferredInitializerMissingWriteProgram()),
				fixedArrayInitializerMismatch: invalidDiagnostics(fixedArrayInitializerMismatchProgram()),
				zeroFixedArrayOverBudget: invalidDiagnostics(zeroFixedArrayOverBudgetProgram()),
				zeroFixedArrayNonAutomatic: invalidDiagnostics(zeroFixedArrayNonAutomaticProgram()),
				invalidStaticBoundsProof: invalidDiagnostics(invalidStaticBoundsProofProgram()),
				uncheckedCollectionAccess: invalidDiagnostics(uncheckedCollectionAccessProgram()),
				nonDominatingBoundsProof: invalidDiagnostics(nonDominatingBoundsProofProgram()),
				unknownLoopBoundsGuard: invalidDiagnostics(unknownLoopBoundsGuardProgram()),
				voidReturnWithValue: invalidDiagnostics(voidReturnWithValueProgram()),
				valueReturnWithoutValue: invalidDiagnostics(valueReturnWithoutValueProgram()),
				returnTypeMismatch: invalidDiagnostics(returnTypeMismatchProgram()),
				useBeforeDefinition: invalidDiagnostics(useBeforeDefinitionProgram()),
				cleanupOrder: invalidDiagnostics(cleanupOrderProgram()),
				absoluteSource: invalidDiagnostics(absoluteSourceProgram()),
				primitiveRuntimeConversion: invalidDiagnostics(primitiveRuntimeConversionProgram()),
				invalidFloat32Narrow: invalidDiagnostics(invalidFloat32NarrowProgram()),
				invalidFloat32Widen: invalidDiagnostics(invalidFloat32WidenProgram()),
				nullableUnwrapWithoutFailure: invalidDiagnostics(nullableUnwrapWithoutFailureProgram()),
				stringByteLengthMismatch: invalidDiagnostics(stringByteLengthMismatchProgram()),
				cstringByteLengthMismatch: invalidDiagnostics(cstringByteLengthMismatchProgram()),
				cstringEmbeddedNul: invalidDiagnostics(cstringEmbeddedNulProgram()),
				ioFailurePolicy: invalidDiagnostics(ioFailurePolicyProgram()),
				defaultInitializationType: invalidDiagnostics(defaultInitializationTypeProgram()),
				statusConventionReturnType: invalidDiagnostics(statusConventionReturnTypeProgram()),
				statusCallWithoutFailure: invalidDiagnostics(statusCallWithoutFailureProgram()),
				throwWithoutStatus: invalidDiagnostics(throwWithoutStatusProgram()),
				unsupportedTypedNode: [
					HxcIRDiagnostic.unsupportedTypedAstNode(PROFILE, "TUnop(OpIncrement,Postfix)", "function app.Main.main expression", span(MAIN_SOURCE, 18))
						.render()]
			}
		}));
	}

	static function semanticProgram(reverse:Bool):HxcIRProgram {
		final bufferType:HxcIRTypeDeclaration = {
			id: "type.buffer",
			displayName: "app.Buffer",
			kind: IRTKAggregate([
				{
					name: "data",
					type: IRTPointer(IRTInt(32, true), false),
					mutable: true,
					source: span(MAIN_SOURCE, 2)
				},
				{
					name: "length",
					type: IRTInt(32, true),
					mutable: false,
					source: span(MAIN_SOURCE, 3)
				}
			]),
			source: span(MAIN_SOURCE, 1, 4)
		};
		final errorType:HxcIRTypeDeclaration = {
			id: "type.error",
			displayName: "app.Error",
			kind: IRTKTaggedUnion([
				{
					name: "Message",
					tagValue: 0,
					payload: [{name: "text", type: IRTDynamic, source: span(MAIN_SOURCE, 6)}],
					source: span(MAIN_SOURCE, 6)
				},
				{
					name: "Unavailable",
					tagValue: 1,
					payload: [],
					source: span(MAIN_SOURCE, 7)
				}
			]),
			source: span(MAIN_SOURCE, 5, 8)
		};
		final types = [bufferType, errorType];
		final instances:Array<HxcIRTypeInstance> = [
			{
				id: "instance.buffer",
				declarationId: "type.buffer",
				arguments: [],
				representation: IRRDirect,
				source: span(MAIN_SOURCE, 1, 4)
			},
			{
				id: "instance.error",
				declarationId: "type.error",
				arguments: [],
				representation: IRRTagged,
				source: span(MAIN_SOURCE, 5, 8)
			}
		];
		final mainFunctions = [sideEffectFunction()];
		final supportFunctions = [
			constantFunction("fn.nextIndex", "nextIndex", "0", 2),
			constantFunction("fn.produce", "produce", "7", 6)
		];
		final mainModule:HxcIRModule = {
			id: "app.SideEffects",
			types: types,
			typeInstances: instances,
			globals: [
				{
					id: "global.calls",
					type: IRTInt(32, true),
					mutable: true,
					initialization: IRGIConstant(IRCInt("0")),
					source: span(MAIN_SOURCE, 10)
				}
			],
			functions: mainFunctions,
			source: span(MAIN_SOURCE, 1, 40)
		};
		final supportModule:HxcIRModule = {
			id: "app.SideEffectSupport",
			types: [],
			typeInstances: [],
			globals: [],
			functions: supportFunctions,
			source: span(SUPPORT_SOURCE, 1, 10)
		};
		final modules = [mainModule, supportModule];
		if (reverse) {
			types.reverse();
			instances.reverse();
			mainFunctions.reverse();
			supportFunctions.reverse();
			mainModule.globals.reverse();
			mainModule.functions[0].locals.reverse();
			mainModule.functions[0].cleanupRegions.reverse();
			mainModule.functions[0].blocks.reverse();
			modules.reverse();
		}
		return {schemaVersion: HxcIRValidator.SCHEMA_VERSION, dispatch: emptyDispatch(), modules: modules};
	}

	static function sideEffectFunction():HxcIRFunction {
		final cleanup = fullCleanupPath();
		final entry:HxcIRBlock = {
			id: "entry",
			parameters: [],
			instructions: [
				instruction("i00.sequence", null, IRIOSequence("arr[nextIndex()] += produce()"), MAIN_SOURCE, 18),
				instruction("i01.next-index", result("value.index", IRTInt(32, true)), IRIOCall(call(IRCDDirect("fn.nextIndex"), [], IRTInt(32, true))),
					MAIN_SOURCE, 18),
				instruction("i02.slot-address", result("value.slot", IRTPointer(IRTInt(32, true), false)),
					IRIOAddress(IRPIndex(IRPLocal("local.array"), "value.index")), MAIN_SOURCE, 18),
				instruction("i03.current", result("value.current", IRTInt(32, true)), IRIOLoad(IRPDereference("value.slot")), MAIN_SOURCE, 18),
				instruction("i04.produce", result("value.rhs", IRTInt(32, true)), IRIOCall(call(IRCDDirect("fn.produce"), [], IRTInt(32, true),
					{
						kind: IRFException,
						target: IRFTBlock("catch"),
						arguments: [],
						cleanup: cleanup
					})),
					MAIN_SOURCE, 18),
				instruction("i05.add", result("value.updated", IRTInt(32, true)), IRIOBinary("haxe.i32.add", "value.current", "value.rhs", IRIStatic),
					MAIN_SOURCE, 18),
				instruction("i06.store", null, IRIOStore(IRPDereference("value.slot"), "value.updated"), MAIN_SOURCE, 18),
				instruction("i07.widen", result("value.widened", IRTFloat(64)), IRIOConvert("value.updated", IRCNumericExact, IRTFloat(64), IRIStatic, null),
					MAIN_SOURCE, 19)
			],
			terminator: terminator(IRTReturn("value.updated", cleanup), MAIN_SOURCE, 20),
			source: span(MAIN_SOURCE, 17, 21)
		};
		final catchBlock:HxcIRBlock = {
			id: "catch",
			parameters: [],
			instructions: [
				instruction("i08.fallback", result("value.fallback", IRTInt(32, true)), IRIOConstant(IRCInt("-1")), MAIN_SOURCE, 22)
			],
			terminator: terminator(IRTReturn("value.fallback", []), MAIN_SOURCE, 23),
			source: span(MAIN_SOURCE, 22, 24)
		};
		return {
			id: "fn.main",
			displayName: "app.SideEffects.main",
			parameters: [],
			locals: [
				local("local.array", IRTInstance("instance.buffer"), IRLSAutomatic, IRISInitialized, MAIN_SOURCE, 14),
				local("local.scratch", IRTInt(32, true), IRLSRegion("cleanup.inner"), IRISInitialized, MAIN_SOURCE, 15),
				local("local.token", IRTPointer(IRTVoid, true), IRLSRegion("cleanup.inner"), IRISInitialized, MAIN_SOURCE, 16)
			],
			returnType: IRTInt(32, true),
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [entry, catchBlock],
			cleanupRegions: [
				{
					id: "cleanup.outer",
					parentId: null,
					actions: [
						cleanupAction("destroy-array", IRCADestroy(IRPLocal("local.array"), IRISInitialized, IRISDestroyed), MAIN_SOURCE, 14)
					],
					source: span(MAIN_SOURCE, 13, 21)
				},
				{
					id: "cleanup.inner",
					parentId: "cleanup.outer",
					actions: [
						cleanupAction("destroy-scratch", IRCADestroy(IRPLocal("local.scratch"), IRISInitialized, IRISDestroyed), MAIN_SOURCE, 15),
						cleanupAction("release-token", IRCARelease(IRPLocal("local.token"), IRIProgramLocal("helper.release-token")), MAIN_SOURCE, 16)
					],
					source: span(MAIN_SOURCE, 15, 21)
				}
			],
			source: span(MAIN_SOURCE, 12, 24)
		};
	}

	static function fullCleanupPath():Array<HxcIRCleanupStep> {
		return [
			{regionId: "cleanup.inner", actionId: "release-token"},
			{regionId: "cleanup.inner", actionId: "destroy-scratch"},
			{regionId: "cleanup.outer", actionId: "destroy-array"}
		];
	}

	static function constantFunction(id:String, displayName:String, value:String, line:Int):HxcIRFunction {
		final valueId = '$id.result';
		return {
			id: id,
			displayName: 'app.SideEffectSupport.$displayName',
			parameters: [],
			locals: [],
			returnType: IRTInt(32, true),
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [
						instruction('$id.constant', result(valueId, IRTInt(32, true)), IRIOConstant(IRCInt(value)), SUPPORT_SOURCE, line)
					],
					terminator: terminator(IRTReturn(valueId, []), SUPPORT_SOURCE, line),
					source: span(SUPPORT_SOURCE, line)
				}
			],
			cleanupRegions: [],
			source: span(SUPPORT_SOURCE, line)
		};
	}

	static function coverageProgram():HxcIRProgram {
		final recordType:HxcIRTypeDeclaration = {
			id: "type.record",
			displayName: "coverage.Record",
			kind: IRTKAggregate([
				{
					name: "x",
					type: IRTInt(32, true),
					mutable: true,
					source: span(COVERAGE_SOURCE, 2)
				}
			]),
			source: span(COVERAGE_SOURCE, 1, 3)
		};
		final optionType:HxcIRTypeDeclaration = {
			id: "type.option",
			displayName: "coverage.Option",
			kind: IRTKTaggedUnion([
				{
					name: "Some",
					tagValue: 0,
					payload: [{name: "value", type: IRTInt(32, true), source: span(COVERAGE_SOURCE, 5)}],
					source: span(COVERAGE_SOURCE, 5)
				},
				{
					name: "None",
					tagValue: 1,
					payload: [],
					source: span(COVERAGE_SOURCE, 6)
				}
			]),
			source: span(COVERAGE_SOURCE, 4, 7)
		};
		final objectType:HxcIRTypeDeclaration = {
			id: "type.object",
			displayName: "coverage.Object",
			kind: IRTKClass({baseInstanceId: null, fields: [], header: IRCHVirtual("vtable.layout.coverage.Object")}),
			source: span(COVERAGE_SOURCE, 8)
		};
		final interfaceType:HxcIRTypeDeclaration = {
			id: "type.interface",
			displayName: "coverage.Interface",
			kind: IRTKReference,
			source: span(COVERAGE_SOURCE, 9)
		};
		final constructedType:HxcIRTypeDeclaration = {
			id: "type.constructed",
			displayName: "coverage.Constructed",
			kind: IRTKClass({baseInstanceId: null, fields: [], header: IRCHNone}),
			source: span(COVERAGE_SOURCE, 10)
		};
		final instances:Array<HxcIRTypeInstance> = [
			{
				id: "instance.record",
				declarationId: "type.record",
				arguments: [],
				representation: IRRDirect,
				source: span(COVERAGE_SOURCE, 1, 3)
			},
			{
				id: "instance.option",
				declarationId: "type.option",
				arguments: [],
				representation: IRRTagged,
				source: span(COVERAGE_SOURCE, 4, 7)
			},
			{
				id: "instance.object",
				declarationId: "type.object",
				arguments: [],
				representation: IRRDirect,
				source: span(COVERAGE_SOURCE, 8)
			},
			{
				id: "instance.interface",
				declarationId: "type.interface",
				arguments: [],
				representation: IRRDirect,
				source: span(COVERAGE_SOURCE, 9)
			},
			{
				id: "instance.constructed",
				declarationId: "type.constructed",
				arguments: [],
				representation: IRRDirect,
				source: span(COVERAGE_SOURCE, 10)
			}
		];
		return {
			schemaVersion: HxcIRValidator.SCHEMA_VERSION,
			dispatch: {
				layouts: [
					{
						id: "vtable.layout.coverage.Object",
						rootInstanceId: "instance.object",
						slotIds: ["slot.render"],
						source: span(COVERAGE_SOURCE, 8)
					}
				],
				slots: [
					{
						id: "slot.render",
						ownerInstanceId: "instance.object",
						parameterTypes: [IRTInt(32, true)],
						returnType: IRTInt(32, true),
						source: span(COVERAGE_SOURCE, 8)
					}
				],
				tables: [
					{
						id: "vtable.coverage.Object",
						layoutId: "vtable.layout.coverage.Object",
						classInstanceId: "instance.object",
						entries: [{slotId: "slot.render", implementationFunctionId: "fn.coverage.render"}],
						source: span(COVERAGE_SOURCE, 8)
					}
				]
			},
			modules: [
				{
					id: "coverage.IR",
					types: [recordType, optionType, objectType, interfaceType, constructedType],
					typeInstances: instances,
					globals: [],
					functions: [
						coverageTarget(),
						coverageVirtualTarget(),
						coverageThrowFunction(),
						coverageFunction(),
						coverageTagSwitchFunction()
					],
					source: span(COVERAGE_SOURCE, 1, 80)
				}
			]
		};
	}

	static function coverageTarget():HxcIRFunction {
		return voidFunction("fn.coverage.target", "coverage.IR.target", COVERAGE_SOURCE, 12);
	}

	static function coverageVirtualTarget():HxcIRFunction {
		return {
			id: "fn.coverage.render",
			displayName: "coverage.Object.render",
			parameters: [
				parameter("parameter.self", IRTPointer(IRTInstance("instance.object"), true), COVERAGE_SOURCE, 12),
				parameter("parameter.value", IRTInt(32, true), COVERAGE_SOURCE, 12)
			],
			locals: [],
			returnType: IRTInt(32, true),
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTReturn("parameter.value", []), COVERAGE_SOURCE, 12),
					source: span(COVERAGE_SOURCE, 12)
				}
			],
			cleanupRegions: [],
			source: span(COVERAGE_SOURCE, 12)
		};
	}

	static function coverageThrowFunction():HxcIRFunction {
		return {
			id: "fn.coverage.throw",
			displayName: "coverage.IR.throwValue",
			parameters: [parameter("value.thrown", IRTDynamic, COVERAGE_SOURCE, 14)],
			locals: [],
			returnType: IRTVoid,
			failureConvention: IRFCStatus(IRFException),
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTThrow("value.thrown", {
						kind: IRFException,
						target: IRFTPropagate,
						arguments: [],
						cleanup: []
					}), COVERAGE_SOURCE, 14),
					source: span(COVERAGE_SOURCE, 14)
				}
			],
			cleanupRegions: [],
			source: span(COVERAGE_SOURCE, 14)
		};
	}

	static function coverageFunction():HxcIRFunction {
		final allocationFailure:HxcIRFailureEdge = {
			kind: IRFAllocationFailure,
			target: IRFTPropagate,
			arguments: [],
			cleanup: []
		};
		final nativeFailure:HxcIRFailureEdge = {
			kind: IRFNativeStatus,
			target: IRFTPropagate,
			arguments: [],
			cleanup: []
		};
		final resultFailure:HxcIRFailureEdge = {
			kind: IRFResultError,
			target: IRFTBlock("result-error"),
			arguments: [],
			cleanup: []
		};
		return {
			id: "fn.coverage",
			displayName: "coverage.IR.coverage",
			parameters: [
				parameter("value.receiver", IRTPointer(IRTInstance("instance.object"), true), COVERAGE_SOURCE, 16),
				parameter("value.callable", IRTFunction([IRTInt(32, true)], IRTInt(32, true)), COVERAGE_SOURCE, 16),
				parameter("value.argument", IRTInt(32, true), COVERAGE_SOURCE, 16),
				parameter("value.float-input", IRTFloat(64), COVERAGE_SOURCE, 16),
				parameter("value.size", IRTAbiInteger(IRAKSize), COVERAGE_SOURCE, 16),
				parameter("value.nullable-reference", IRTNullable(IRTInstance("instance.object"), IRNPointer), COVERAGE_SOURCE, 16)
			],
			locals: [
				local("local.fixed", IRTFixedArray(IRTInt(32, true), 2, "coverage.Length2"), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17),
				local("local.fixed-zero", IRTFixedArray(IRTInt(8, false), 16, "coverage.Length16"), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17),
				local("local.span", IRTSpan(IRTInt(32, true), false), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17),
				local("local.record", IRTInstance("instance.record"), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17),
				local("local.constructed", IRTInstance("instance.constructed"), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17),
				local("local.virtual-object", IRTInstance("instance.object"), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17),
				local("local.owned", IRTPointer(IRTInstance("instance.object"), false), IRLSAutomatic, IRISUninitialized, COVERAGE_SOURCE, 17)
			],
			returnType: IRTVoid,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [
						instruction("c00.one", result("value.one", IRTInt(32, true)), IRIOConstant(IRCInt("1")), COVERAGE_SOURCE, 18),
						instruction("c00.string", result("value.string", IRTString), IRIOConstant(IRCString("line\x00é🙂", 11)), COVERAGE_SOURCE, 18),
						instruction("c00.cstring", result("value.cstring", IRTCString), IRIOConstant(IRCCStringLiteral("native-é", 9)), COVERAGE_SOURCE, 18),
						instruction("c00.native-constant", result("value.native-constant", IRTInt(32, true)),
							IRIOConstant(IRCNativeConstant("native.constant.coverage")), COVERAGE_SOURCE, 18),
						instruction("c00.output", null, IRIOCall(call(IRCDRuntime("io", "sys-println-literal"), ["value.string"], IRTVoid,
							{
								kind: IRFNativeStatus,
								target: IRFTAbort,
								arguments: [],
								cleanup: []
							})),
							COVERAGE_SOURCE, 18),
						instruction("c00.fixed", null,
							IRIOInitializeFixedArray(IRPLocal("local.fixed"), ["value.one", "value.one"], IRISUninitialized, IRISInitialized),
							COVERAGE_SOURCE, 18),
						instruction("c00.fixed-zero", null, IRIOZeroInitializeFixedArray(IRPLocal("local.fixed-zero"), IRISUninitialized, IRISInitialized),
							COVERAGE_SOURCE, 18),
						instruction("c00.span", null, IRIOInitializeSpan(IRPLocal("local.span"), IRPLocal("local.fixed"), IRISUninitialized, IRISInitialized),
							COVERAGE_SOURCE, 18),
						instruction("c00.bounds-static", null, IRIOBoundsCheck(IRPLocal("local.span"), "value.one", IRBPStaticProof(2, 1)), COVERAGE_SOURCE,
							18),
						instruction("c00.span-load", result("value.span-one", IRTInt(32, true)), IRIOLoad(IRPIndex(IRPLocal("local.span"), "value.one")),
							COVERAGE_SOURCE, 18),
						instruction("c00.default-initialize", null, IRIODefaultInitialize(IRPLocal("local.constructed"), IRISUninitialized, IRISInitialized),
							COVERAGE_SOURCE, 18),
						instruction("c00.virtual-initialize", null,
							IRIODefaultInitialize(IRPLocal("local.virtual-object"), IRISUninitialized, IRISInitialized), COVERAGE_SOURCE, 18),
						instruction("c00.virtual-bind", null, IRIOBindVirtualTable(IRPLocal("local.virtual-object"), "vtable.coverage.Object"),
							COVERAGE_SOURCE, 18),
						instruction("c00.bounds-checked", null,
							IRIOBoundsCheck(IRPLocal("local.span"), "value.argument", IRBPCheckedAbort("portable", "debug")), COVERAGE_SOURCE, 18),
						instruction("c01.convert", result("value.float", IRTFloat(64)),
							IRIOConvert("value.one", IRCNumericExact, IRTFloat(64), IRIStatic, null), COVERAGE_SOURCE, 19),
						instruction("c01.float32-narrow", result("value.float32", IRTFloat(32)),
							IRIOConvert("value.float-input", IRCNumericRoundBinary32, IRTFloat(32), IRIStatic, null), COVERAGE_SOURCE, 19),
						instruction("c01.float32-widen", result("value.float32-widened", IRTFloat(64)),
							IRIOConvert("value.float32", IRCNumericWidenBinary64, IRTFloat(64), IRIStatic, null), COVERAGE_SOURCE, 19),
						instruction("c01.saturating", result("value.saturated", IRTInt(32, true)),
							IRIOConvert("value.float-input", IRCNumericSaturating, IRTInt(32, true), IRIProgramLocal("hxc.f64.to.i32.saturating"), null),
							COVERAGE_SOURCE, 19),
						instruction("c01.checked", result("value.checked-i8", IRTInt(8, true)),
							IRIOConvert("value.argument", IRCNumericChecked, IRTInt(8, true), IRIProgramLocal("hxc.i32.to.i8.checked"), resultFailure),
							COVERAGE_SOURCE, 19),
						instruction("c01.nullable-inject", result("value.nullable-one", IRTNullable(IRTInt(32, true), IRNTagged)),
							IRIOConvert("value.one", IRCNullableInject, IRTNullable(IRTInt(32, true), IRNTagged), IRIStatic, null), COVERAGE_SOURCE, 19),
						instruction("c01.nullable-unwrap", result("value.unwrapped-one", IRTInt(32, true)),
							IRIOConvert("value.nullable-one", IRCNullableUnwrap, IRTInt(32, true), IRIStatic, resultFailure), COVERAGE_SOURCE, 19),
						instruction("c01.box", result("value.boxed", IRTDynamic), IRIOConvert("value.one", IRCBox, IRTDynamic, IRIRuntime("dynamic"), null),
							COVERAGE_SOURCE, 19),
						instruction("c01.unbox", result("value.unboxed", IRTInt(32, true)),
							IRIOConvert("value.boxed", IRCUnbox, IRTInt(32, true), IRIRuntime("dynamic"), null), COVERAGE_SOURCE, 19),
						instruction("c02.direct", null, IRIOCall(call(IRCDDirect("fn.coverage.target"), [], IRTVoid)), COVERAGE_SOURCE, 20),
						instruction("c02.result-edge", null, IRIOCall(call(IRCDDirect("fn.coverage.target"), [], IRTVoid, resultFailure)), COVERAGE_SOURCE, 20),
						instruction("c02.receiver-check", null, IRIONullCheck("value.receiver", IRNCPCheckedAbort("portable", "debug")), COVERAGE_SOURCE, 20),
						instruction("c03.virtual", result("value.virtual", IRTInt(32, true)),
							IRIOCall(call(IRCDVirtual("slot.render", "value.receiver"), ["value.argument"], IRTInt(32, true))), COVERAGE_SOURCE, 21),
						instruction("c04.interface", result("value.interface", IRTInt(32, true)),
							IRIOCall(call(IRCDInterface("instance.interface", "slot.measure", "value.receiver"), ["value.argument"], IRTInt(32, true))),
							COVERAGE_SOURCE, 22),
						instruction("c05.closure", result("value.closure", IRTInt(32, true)),
							IRIOCall(call(IRCDClosure("value.callable"), ["value.argument"], IRTInt(32, true))), COVERAGE_SOURCE, 23),
						instruction("c06.native", result("value.native", IRTInt(32, true)),
							IRIOCall(call(IRCDNative("native.status"), ["value.argument"], IRTInt(32, true), nativeFailure)), COVERAGE_SOURCE, 24),
						instruction("c07.runtime", null, IRIOCall(call(IRCDRuntime("exception", "checkpoint"), [], IRTVoid)), COVERAGE_SOURCE, 25),
						instruction("c08.intrinsic", result("value.intrinsic", IRTInt(32, true)),
							IRIOCall(call(IRCDIntrinsic("haxe.i32.clamp"), ["value.argument"], IRTInt(32, true))), COVERAGE_SOURCE, 26),
						instruction("c09.unary", result("value.negative", IRTInt(32, true)), IRIOUnary("haxe.i32.negate", "value.one", IRIStatic),
							COVERAGE_SOURCE, 27),
						instruction("c10.record", result("value.record", IRTInstance("instance.record")), IRIOConstructAggregate("instance.record",
							[
								{
									name: "x",
									valueId: "value.one"
								}
							]),
							COVERAGE_SOURCE, 28),
						instruction("c10.record-initialize", null,
							IRIOInitialize(IRPLocal("local.record"), "value.record", IRISUninitialized, IRISInitialized), COVERAGE_SOURCE, 28),
						instruction("c10.field-address", result("value.field-address", IRTPointer(IRTInt(32, true), false)),
							IRIOAddress(IRPField(IRPLocal("local.record"), "x")), COVERAGE_SOURCE, 28),
						instruction("c10.field-load", result("value.field-load", IRTInt(32, true)), IRIOLoad(IRPDereference("value.field-address")),
							COVERAGE_SOURCE, 28),
						instruction("c11.project", result("value.projected", IRTInt(32, true)), IRIOProject("value.record", "x"), COVERAGE_SOURCE, 29),
						instruction("c12.tag", result("value.option", IRTInstance("instance.option")),
							IRIOConstructTag("instance.option", "Some", ["value.one"]), COVERAGE_SOURCE, 30),
						instruction("c13.match", result("value.is-some", IRTBool), IRIOMatchTag("value.option", "Some"), COVERAGE_SOURCE, 31),
						instruction("c13.project-tag", result("value.some-payload", IRTInt(32, true)),
							IRIOProjectTag("value.option", "Some", 0, IRTCPCheckedAbort("portable", "debug")), COVERAGE_SOURCE, 31),
						instruction("c14.allocate", result("value.allocation", IRTPointer(IRTInstance("instance.object"), false)),
							IRIOAllocate(IRTInstance("instance.object"), IRAOwned, IRIProgramLocal("helper.allocate-object"), allocationFailure),
							COVERAGE_SOURCE, 32),
						instruction("c15.initialize", null, IRIOInitialize(IRPLocal("local.owned"), "value.allocation", IRISUninitialized, IRISInitialized),
							COVERAGE_SOURCE, 33),
						instruction("c16.retain", null, IRIORetain(IRPLocal("local.owned"), IRIRuntime("object")), COVERAGE_SOURCE, 34),
						instruction("c17.trace", null, IRIOTrace(IRPLocal("local.owned"), IRIRuntime("gc")), COVERAGE_SOURCE, 35),
						instruction("c18.deallocate", null, IRIODeallocate(IRPLocal("local.owned"), IRIProgramLocal("helper.free-object")), COVERAGE_SOURCE,
							36),
						instruction("c19.lifetime", null,
							IRIOLifetime(IRPLocal("local.owned"), IRISInitialized, IRISDestroyed, "explicit deallocation completed"), COVERAGE_SOURCE, 37)
					],
					terminator: terminator(IRTBranch("value.is-some", {
						targetBlockId: "success",
						arguments: ["value.projected"],
						cleanup: []
					}, {
						targetBlockId: "result-error",
						arguments: [],
						cleanup: []
					}), COVERAGE_SOURCE, 38),
					source: span(COVERAGE_SOURCE, 18, 39)
				},
				{
					id: "success",
					parameters: [parameter("value.forwarded", IRTInt(32, true), COVERAGE_SOURCE, 39)],
					instructions: [
						instruction("c20.forwarded", result("value.forwarded-copy", IRTInt(32, true)),
							IRIOUnary("haxe.i32.identity", "value.forwarded", IRIStatic), COVERAGE_SOURCE, 39)
					],
					terminator: terminator(IRTReturn(null, []), COVERAGE_SOURCE, 39),
					source: span(COVERAGE_SOURCE, 39)
				},
				{
					id: "result-error",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTReturn(null, []), COVERAGE_SOURCE, 40),
					source: span(COVERAGE_SOURCE, 40)
				}
			],
			cleanupRegions: [],
			source: span(COVERAGE_SOURCE, 15, 39)
		};
	}

	static function coverageTagSwitchFunction():HxcIRFunction {
		final someEdge:HxcIRBlockEdge = {targetBlockId: "some", arguments: [], cleanup: []};
		final noneEdge:HxcIRBlockEdge = {targetBlockId: "none", arguments: [], cleanup: []};
		return {
			id: "fn.coverage.tag-switch",
			displayName: "coverage.IR.tagSwitch",
			parameters: [
				parameter("value.option-switch", IRTInstance("instance.option"), COVERAGE_SOURCE, 42)
			],
			locals: [],
			returnType: IRTVoid,
			failureConvention: IRFCInfallible,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTTagSwitch("value.option-switch", [{tagName: "Some", edge: someEdge}, {tagName: "None", edge: noneEdge}], null),
						COVERAGE_SOURCE, 43),
					source: span(COVERAGE_SOURCE, 43)
				},
				{
					id: "some",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTReturn(null, []), COVERAGE_SOURCE, 44),
					source: span(COVERAGE_SOURCE, 44)
				},
				{
					id: "none",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTReturn(null, []), COVERAGE_SOURCE, 45),
					source: span(COVERAGE_SOURCE, 45)
				}
			],
			cleanupRegions: [],
			source: span(COVERAGE_SOURCE, 42, 45)
		};
	}

	static function missingTerminatorProgram():HxcIRProgram {
		return minimalProgram("invalid.MissingTerminator", [], null, [], [], "test/negative/MissingTerminator.hx");
	}

	static function useBeforeDefinitionProgram():HxcIRProgram {
		final file = "test/negative/UseBeforeDefinition.hx";
		return minimalProgram("invalid.UseBeforeDefinition", [
			instruction("bad.store", null, IRIOStore(IRPLocal("local.target"), "value.later"), file, 2),
			instruction("bad.constant", result("value.later", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 3)
		],
			terminator(IRTReturn(null, []), file, 4), [local("local.target", IRTInt(32, true), IRLSAutomatic, IRISInitialized, file, 1)], [], file);
	}

	static function constantTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/ConstantTypeMismatch.hx";
		return minimalProgram("invalid.ConstantTypeMismatch", [
			instruction("bad.constant", result("value.bad", IRTInt(32, true)), IRIOConstant(IRCBool(true)), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [], [], file);
	}

	static function nativeConstantAggregateProgram():HxcIRProgram {
		final file = "test/negative/NativeConstantAggregate.hx";
		return aggregateProgram(file, [
			instruction("bad.native-constant", result("value.bad", IRTInstance("instance.record")),
				IRIOConstant(IRCNativeConstant("native.constant.aggregate")), file, 2)
		], [], "invalid.NativeConstantAggregate");
	}

	static function loadTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/LoadTypeMismatch.hx";
		return minimalProgram("invalid.LoadTypeMismatch", [
			instruction("bad.load", result("value.bad", IRTBool), IRIOLoad(IRPLocal("local.value")), file, 2)
		],
			terminator(IRTReturn(null, []), file, 3), [local("local.value", IRTInt(32, true), IRLSAutomatic, IRISInitialized, file, 1)], [], file);
	}

	static function addressTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/AddressTypeMismatch.hx";
		return minimalProgram("invalid.AddressTypeMismatch", [
			instruction("bad.address", result("value.bad", IRTPointer(IRTBool, false)), IRIOAddress(IRPLocal("local.value")), file, 2)
		],
			terminator(IRTReturn(null, []), file, 3), [local("local.value", IRTInt(32, true), IRLSAutomatic, IRISInitialized, file, 1)], [], file);
	}

	static function aggregateConstructionMismatchProgram():HxcIRProgram {
		final file = "test/negative/AggregateConstructionMismatch.hx";
		return aggregateProgram(file, [
			instruction("bad.one", result("value.one", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2),
			instruction("bad.truth", result("value.truth", IRTBool), IRIOConstant(IRCBool(true)), file, 3),
			instruction("bad.construct", result("value.record", IRTBool), IRIOConstructAggregate("instance.record", [
				{
					name: "z",
					valueId: "value.one"
				},
				{name: "x", valueId: "value.truth"}
			]), file, 4)
		], [], "invalid.AggregateConstructionMismatch");
	}

	static function aggregateProjectionMismatchProgram():HxcIRProgram {
		final file = "test/negative/AggregateProjectionMismatch.hx";
		return aggregateProgram(file, [
			instruction("bad.one", result("value.one", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2),
			instruction("bad.truth", result("value.truth", IRTBool), IRIOConstant(IRCBool(true)), file, 3),
			instruction("bad.construct", result("value.record", IRTInstance("instance.record")), IRIOConstructAggregate("instance.record",
				[
					{
						name: "x",
						valueId: "value.one"
					},
					{name: "y", valueId: "value.truth"}
				]),
				file, 4),
			instruction("bad.project-type", result("value.projected", IRTBool), IRIOProject("value.record", "x"), file, 5),
			instruction("bad.project-name", result("value.missing", IRTInt(32, true)), IRIOProject("value.record", "z"), file, 6)
		], [], "invalid.AggregateProjectionMismatch");
	}

	static function aggregateFieldPlaceMismatchProgram():HxcIRProgram {
		final file = "test/negative/AggregateFieldPlaceMismatch.hx";
		return aggregateProgram(file, [
			instruction("bad.address", result("value.address", IRTPointer(IRTInt(32, true), false)), IRIOAddress(IRPField(IRPLocal("local.record"), "z")),
				file, 2)
		], [
			local("local.record", IRTInstance("instance.record"), IRLSAutomatic, IRISInitialized, file, 1)
		], "invalid.AggregateFieldPlaceMismatch");
	}

	static function tagConstructionMismatchProgram():HxcIRProgram {
		final file = "test/negative/TagConstructionMismatch.hx";
		return taggedUnionProgram(file, [
			instruction("bad.truth", result("value.truth", IRTBool), IRIOConstant(IRCBool(true)), file, 2),
			instruction("bad.construct", result("value.option", IRTBool), IRIOConstructTag("instance.option", "Some", ["value.truth"]), file, 3)
		], terminator(IRTReturn(null, []), file, 4), "invalid.TagConstructionMismatch");
	}

	static function tagProjectionMismatchProgram():HxcIRProgram {
		final file = "test/negative/TagProjectionMismatch.hx";
		return taggedUnionProgram(file, [
			instruction("bad.one", result("value.one", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2),
			instruction("bad.construct", result("value.option", IRTInstance("instance.option")), IRIOConstructTag("instance.option", "Some", ["value.one"]),
				file, 3),
			instruction("bad.project", result("value.payload", IRTBool), IRIOProjectTag("value.option", "Some", 0, IRTCPCheckedAbort("portable", "debug")),
				file, 4)
		], terminator(IRTReturn(null, []), file, 5), "invalid.TagProjectionMismatch");
	}

	static function nonExhaustiveTagSwitchProgram():HxcIRProgram {
		final file = "test/negative/NonExhaustiveTagSwitch.hx";
		final loopEdge:HxcIRBlockEdge = {targetBlockId: "entry", arguments: [], cleanup: []};
		return taggedUnionProgram(file, [
			instruction("bad.construct", result("value.option", IRTInstance("instance.option")), IRIOConstructTag("instance.option", "None", []), file, 2)
		], terminator(IRTTagSwitch("value.option", [
			{
				tagName: "Some",
				edge: loopEdge
			}
			], null), file, 3), "invalid.NonExhaustiveTagSwitch");
	}

	static function directPayloadRepresentationProgram():HxcIRProgram {
		final file = "test/negative/DirectPayloadRepresentation.hx";
		final program = taggedUnionProgram(file, [], terminator(IRTReturn(null, []), file, 3), "invalid.DirectPayloadRepresentation");
		program.modules[0].typeInstances[0] = {
			id: "instance.option",
			declarationId: "type.option",
			arguments: [],
			representation: IRRDirect,
			source: span(file, 1)
		};
		return program;
	}

	static function orphanVirtualSlotProgram():HxcIRProgram {
		final program = coverageProgram();
		program.dispatch.slots.push({
			id: "slot.zz-orphan",
			ownerInstanceId: "instance.object",
			parameterTypes: [],
			returnType: IRTVoid,
			source: span("test/negative/OrphanVirtualSlot.hx", 1)
		});
		return program;
	}

	static function unknownVirtualImplementationProgram():HxcIRProgram {
		final program = coverageProgram();
		final table = program.dispatch.tables[0];
		final entry = table.entries[0];
		table.entries[0] = {slotId: entry.slotId, implementationFunctionId: "fn.missing"};
		return program;
	}

	static function mismatchedVirtualTableBindProgram():HxcIRProgram {
		final program = coverageProgram();
		final instructions = coverageEntryInstructions(program);
		for (index in 0...instructions.length) {
			if (instructions[index].id == "c00.virtual-bind") {
				instructions[index] = instruction("c00.virtual-bind", null, IRIOBindVirtualTable(IRPLocal("local.record"), "vtable.coverage.Object"),
					"test/negative/MismatchedVirtualTableBind.hx", 1);
				return program;
			}
		}
		throw "coverage virtual-table bind instruction is missing";
	}

	static function uncheckedVirtualCallProgram():HxcIRProgram {
		final program = coverageProgram();
		final instructions = coverageEntryInstructions(program);
		for (index in 0...instructions.length) {
			if (instructions[index].id == "c02.receiver-check") {
				instructions.splice(index, 1);
				return program;
			}
		}
		throw "coverage virtual receiver check instruction is missing";
	}

	static function coverageEntryInstructions(program:HxcIRProgram):Array<HxcIRInstruction> {
		for (fn in program.modules[0].functions) {
			if (fn.id == "fn.coverage") {
				return fn.blocks[0].instructions;
			}
		}
		throw "coverage function is missing";
	}

	static function redundantDefaultTagSwitchProgram():HxcIRProgram {
		final file = "test/negative/RedundantDefaultTagSwitch.hx";
		final loopEdge:HxcIRBlockEdge = {targetBlockId: "entry", arguments: [], cleanup: []};
		return taggedUnionProgram(file, [
			instruction("bad.construct", result("value.option", IRTInstance("instance.option")), IRIOConstructTag("instance.option", "None", []), file, 2)
		], terminator(IRTTagSwitch("value.option", [
			{
				tagName: "Some",
				edge: loopEdge
			},
			{tagName: "None", edge: loopEdge}
			], loopEdge), file, 3), "invalid.RedundantDefaultTagSwitch");
	}

	static function recursiveDirectLayoutProgram():HxcIRProgram {
		final file = "test/negative/RecursiveDirectLayout.hx";
		final program = minimalProgram("invalid.RecursiveDirectLayout", [], terminator(IRTReturn(null, []), file, 4), [], [], file);
		program.modules[0].types.push({
			id: "type.recursive",
			displayName: "invalid.Recursive",
			kind: IRTKTaggedUnion([
				{
					name: "Loop",
					tagValue: 0,
					payload: [{name: "next", type: IRTInstance("instance.recursive"), source: span(file, 1)}],
					source: span(file, 1)
				},
				{
					name: "Stop",
					tagValue: 1,
					payload: [],
					source: span(file, 2)
				}
			]),
			source: span(file, 1, 2)
		});
		program.modules[0].typeInstances.push({
			id: "instance.recursive",
			declarationId: "type.recursive",
			arguments: [],
			representation: IRRTagged,
			source: span(file, 1, 2)
		});
		return program;
	}

	static function uncheckedClassDereferenceProgram():HxcIRProgram {
		final file = "test/negative/UncheckedClassDereference.hx";
		final program = classProgram(file, [
			instruction("bad.load", result("value.field", IRTInt(32, true)), IRIOLoad(IRPField(IRPDereference("value.object"), "value")), file, 3)
		], "invalid.UncheckedClassDereference");
		program.modules[0].functions[0].parameters.push(parameter("value.object", IRTPointer(IRTInstance("instance.class.root"), true), file, 2));
		return program;
	}

	static function unsafeClassUpcastProgram():HxcIRProgram {
		final file = "test/negative/UnsafeClassUpcast.hx";
		final program = classProgram(file, [
			instruction("bad.convert", result("value.leaf", IRTPointer(IRTInstance("instance.class.leaf"), true)),
				IRIOConvert("value.root", IRCRepresentation, IRTPointer(IRTInstance("instance.class.leaf"), true), IRIStatic, null), file, 3)
		], "invalid.UnsafeClassUpcast");
		program.modules[0].functions[0].parameters.push(parameter("value.root", IRTPointer(IRTInstance("instance.class.root"), true), file, 2));
		return program;
	}

	static function mismatchedClassEqualityProgram():HxcIRProgram {
		final file = "test/negative/MismatchedClassEquality.hx";
		final program = classProgram(file, [
			instruction("bad.equal", result("value.equal", IRTBool), IRIOBinary("haxe.class-reference.equal", "value.root", "value.leaf", IRIStatic), file, 3)
		], "invalid.MismatchedClassEquality");
		program.modules[0].functions[0].parameters.push(parameter("value.root", IRTPointer(IRTInstance("instance.class.root"), true), file, 2));
		program.modules[0].functions[0].parameters.push(parameter("value.leaf", IRTPointer(IRTInstance("instance.class.leaf"), true), file, 2));
		return program;
	}

	static function classProgram(file:String, instructions:Array<HxcIRInstruction>, moduleId:String):HxcIRProgram {
		final program = minimalProgram(moduleId, instructions, terminator(IRTReturn(null, []), file, 5), [], [], file);
		program.modules[0].types.push({
			id: "type.class.root",
			displayName: "invalid.Root",
			kind: IRTKClass({
				baseInstanceId: null,
				fields: [
					{
						name: "value",
						type: IRTInt(32, true),
						mutable: true,
						source: span(file, 1)
					}
				],
				header: IRCHNone
			}),
			source: span(file, 1)
		});
		program.modules[0].types.push({
			id: "type.class.leaf",
			displayName: "invalid.Leaf",
			kind: IRTKClass({baseInstanceId: "instance.class.root", fields: [], header: IRCHNone}),
			source: span(file, 1)
		});
		program.modules[0].typeInstances.push({
			id: "instance.class.root",
			declarationId: "type.class.root",
			arguments: [],
			representation: IRRDirect,
			source: span(file, 1)
		});
		program.modules[0].typeInstances.push({
			id: "instance.class.leaf",
			declarationId: "type.class.leaf",
			arguments: [],
			representation: IRRDirect,
			source: span(file, 1)
		});
		return program;
	}

	static function taggedUnionProgram(file:String, instructions:Array<HxcIRInstruction>, terminatorValue:HxcIRTerminator, moduleId:String):HxcIRProgram {
		final program = minimalProgram(moduleId, instructions, terminatorValue, [], [], file);
		program.modules[0].types.push({
			id: "type.option",
			displayName: "invalid.Option",
			kind: IRTKTaggedUnion([
				{
					name: "Some",
					tagValue: 0,
					payload: [{name: "value", type: IRTInt(32, true), source: span(file, 1)}],
					source: span(file, 1)
				},
				{
					name: "None",
					tagValue: 1,
					payload: [],
					source: span(file, 1)
				}
			]),
			source: span(file, 1)
		});
		program.modules[0].typeInstances.push({
			id: "instance.option",
			declarationId: "type.option",
			arguments: [],
			representation: IRRTagged,
			source: span(file, 1)
		});
		return program;
	}

	static function aggregateProgram(file:String, instructions:Array<HxcIRInstruction>, locals:Array<HxcIRLocal>,
			moduleId:String = "invalid.Aggregate"):HxcIRProgram {
		final program = minimalProgram(moduleId, instructions, terminator(IRTReturn(null, []), file, 8), locals, [], file);
		program.modules[0].types.push({
			id: "type.record",
			displayName: "invalid.Record",
			kind: IRTKAggregate([
				{
					name: "x",
					type: IRTInt(32, true),
					mutable: false,
					source: span(file, 1)
				},
				{
					name: "y",
					type: IRTBool,
					mutable: false,
					source: span(file, 1)
				}
			]),
			source: span(file, 1)
		});
		program.modules[0].typeInstances.push({
			id: "instance.record",
			declarationId: "type.record",
			arguments: [],
			representation: IRRDirect,
			source: span(file, 1)
		});
		return program;
	}

	static function storeTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/StoreTypeMismatch.hx";
		return minimalProgram("invalid.StoreTypeMismatch", [
			instruction("bad.constant", result("value.bad", IRTBool), IRIOConstant(IRCBool(true)), file, 2),
			instruction("bad.store", null, IRIOStore(IRPLocal("local.value"), "value.bad"), file, 3)
		],
			terminator(IRTReturn(null, []), file, 4), [local("local.value", IRTInt(32, true), IRLSAutomatic, IRISInitialized, file, 1)], [], file);
	}

	static function switchCaseTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/SwitchCaseTypeMismatch.hx";
		final loopEdge:HxcIRBlockEdge = {targetBlockId: "entry", arguments: [], cleanup: []};
		return minimalProgram("invalid.SwitchCaseTypeMismatch", [
			instruction("bad.subject", result("value.subject", IRTBool), IRIOConstant(IRCBool(true)), file, 2)
		], terminator(IRTSwitch("value.subject", [
			{
				value: IRCInt("1"),
				edge: loopEdge
			}
			], loopEdge), file, 3), [], [], file);
	}

	static function initializerTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/InitializerTypeMismatch.hx";
		return minimalProgram("invalid.InitializerTypeMismatch", [
			instruction("bad.constant", result("value.bad", IRTBool), IRIOConstant(IRCBool(true)), file, 2),
			instruction("bad.initialize", null, IRIOInitialize(IRPLocal("local.value"), "value.bad", IRISUninitialized, IRISInitialized), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [
			local("local.value", IRTInt(32, true), IRLSAutomatic, IRISUninitialized, file, 1)
		], [], file);
	}

	static function deferredInitializerMissingWriteProgram():HxcIRProgram {
		final file = "test/negative/DeferredInitializerMissingWrite.hx";
		final program = minimalProgram("invalid.DeferredInitializerMissingWrite", [], terminator(IRTReturn(null, []), file, 3), [], [], file);
		program.modules[0].globals.push({
			id: "global.value",
			type: IRTInt(32, true),
			mutable: true,
			initialization: IRGIDeferred("invalid.DeferredInitializerMissingWrite.main"),
			source: span(file, 1)
		});
		return program;
	}

	static function fixedArrayInitializerMismatchProgram():HxcIRProgram {
		final file = "test/negative/FixedArrayInitializerMismatch.hx";
		return minimalProgram("invalid.FixedArrayInitializerMismatch", [
			instruction("bad.value", result("value.one", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2),
			instruction("bad.initialize", null, IRIOInitializeFixedArray(IRPLocal("local.fixed"), ["value.one"], IRISUninitialized, IRISInitialized), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [
			local("local.fixed", IRTFixedArray(IRTInt(32, true), 2, "invalid.Length2"), IRLSAutomatic, IRISUninitialized, file, 1)
		], [], file);
	}

	static function zeroFixedArrayOverBudgetProgram():HxcIRProgram {
		final file = "test/negative/ZeroFixedArrayOverBudget.hx";
		return minimalProgram("invalid.ZeroFixedArrayOverBudget", [
			instruction("bad.zero-initialize", null, IRIOZeroInitializeFixedArray(IRPLocal("local.fixed"), IRISUninitialized, IRISInitialized), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [
			local("local.fixed", IRTFixedArray(IRTInt(8, false), 65537, "invalid.Length65537"), IRLSAutomatic, IRISUninitialized, file, 1)
		], [], file);
	}

	static function zeroFixedArrayNonAutomaticProgram():HxcIRProgram {
		final file = "test/negative/ZeroFixedArrayNonAutomatic.hx";
		return minimalProgram("invalid.ZeroFixedArrayNonAutomatic", [
			instruction("bad.zero-initialize", null, IRIOZeroInitializeFixedArray(IRPLocal("local.fixed"), IRISUninitialized, IRISInitialized), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [
			local("local.fixed", IRTFixedArray(IRTInt(8, false), 16, "invalid.Length16"), IRLSStatic, IRISUninitialized, file, 1)
		], [], file);
	}

	static function invalidStaticBoundsProofProgram():HxcIRProgram {
		final file = "test/negative/InvalidStaticBoundsProof.hx";
		return minimalProgram("invalid.InvalidStaticBoundsProof", [
			instruction("bad.index-claimed", result("value.index-claimed", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2),
			instruction("bad.bounds-claimed", null, IRIOBoundsCheck(IRPLocal("local.fixed"), "value.index-claimed", IRBPStaticProof(2, 0)), file, 3),
			instruction("bad.index-range", result("value.index-range", IRTInt(32, true)), IRIOConstant(IRCInt("2")), file, 4),
			instruction("bad.bounds-range", null, IRIOBoundsCheck(IRPLocal("local.fixed"), "value.index-range", IRBPStaticProof(2, 2)), file, 5)
		], terminator(IRTReturn(null, []), file, 4), [
			local("local.fixed", IRTFixedArray(IRTInt(32, true), 2, "invalid.Length2"), IRLSAutomatic, IRISInitialized, file, 1)
		], [], file);
	}

	static function uncheckedCollectionAccessProgram():HxcIRProgram {
		final file = "test/negative/UncheckedCollectionAccess.hx";
		return minimalProgram("invalid.UncheckedCollectionAccess", [
			instruction("bad.index", result("value.index", IRTInt(32, true)), IRIOConstant(IRCInt("0")), file, 2),
			instruction("bad.load", result("value.element", IRTInt(32, true)), IRIOLoad(IRPIndex(IRPLocal("local.fixed"), "value.index")), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [
			local("local.fixed", IRTFixedArray(IRTInt(32, true), 2, "invalid.Length2"), IRLSAutomatic, IRISInitialized, file, 1)
		], [], file);
	}

	/** A check on only one branch must not authorize the shared join block. */
	static function nonDominatingBoundsProofProgram():HxcIRProgram {
		final file = "test/negative/NonDominatingBoundsProof.hx";
		final program = minimalProgram("invalid.NonDominatingBoundsProof", [], null, [
			local("local.fixed", IRTFixedArray(IRTInt(32, true), 2, "invalid.Length2"), IRLSAutomatic, IRISInitialized, file, 1)
		], [], file);
		final fn = program.modules[0].functions[0];
		fn.parameters.push(parameter("value.index", IRTInt(32, true), file, 1));
		fn.parameters.push(parameter("value.condition", IRTBool, file, 1));
		final checkedEdge:HxcIRBlockEdge = {targetBlockId: "checked", arguments: [], cleanup: []};
		final uncheckedEdge:HxcIRBlockEdge = {targetBlockId: "unchecked", arguments: [], cleanup: []};
		final joinEdge:HxcIRBlockEdge = {targetBlockId: "join", arguments: [], cleanup: []};
		fn.blocks.resize(0);
		final replacementBlocks:Array<HxcIRBlock> = [
			{
				id: "entry",
				parameters: [],
				instructions: [],
				terminator: terminator(IRTBranch("value.condition", checkedEdge, uncheckedEdge), file, 2),
				source: span(file, 2)
			},
			{
				id: "checked",
				parameters: [],
				instructions: [
					instruction("checked.bounds", null, IRIOBoundsCheck(IRPLocal("local.fixed"), "value.index", IRBPCheckedAbort("portable", "debug")), file, 3)
				],
				terminator: terminator(IRTJump(joinEdge), file, 4),
				source: span(file, 3, 4)
			},
			{
				id: "unchecked",
				parameters: [],
				instructions: [],
				terminator: terminator(IRTJump(joinEdge), file, 5),
				source: span(file, 5)
			},
			{
				id: "join",
				parameters: [],
				instructions: [
					instruction("bad.load", result("value.element", IRTInt(32, true)), IRIOLoad(IRPIndex(IRPLocal("local.fixed"), "value.index")), file, 6)
				],
				terminator: terminator(IRTReturn(null, []), file, 7),
				source: span(file, 6, 7)
			}
		];
		for (block in replacementBlocks)
			fn.blocks.push(block);
		return program;
	}

	static function unknownLoopBoundsGuardProgram():HxcIRProgram {
		final file = "test/negative/UnknownLoopBoundsGuard.hx";
		return minimalProgram("invalid.UnknownLoopBoundsGuard", [
			instruction("bad.span", null, IRIOInitializeSpan(IRPLocal("local.span"), IRPLocal("local.fixed"), IRISUninitialized, IRISInitialized), file, 2),
			instruction("bad.index", result("value.index", IRTAbiInteger(IRAKSize)), IRIOLoad(IRPLocal("local.index")), file, 3),
			instruction("bad.bounds", null, IRIOBoundsCheck(IRPLocal("local.span"), "value.index", IRBPLoopGuarded("missing.guard", "local.index", 2)), file, 4)
		], terminator(IRTReturn(null, []), file, 4), [
			local("local.fixed", IRTFixedArray(IRTInt(32, true), 2, "invalid.Length2"), IRLSAutomatic, IRISInitialized, file, 1),
			local("local.span", IRTSpan(IRTInt(32, true), false), IRLSAutomatic, IRISUninitialized, file, 1),
			local("local.index", IRTAbiInteger(IRAKSize), IRLSAutomatic, IRISInitialized, file, 1)
		], [], file);
	}

	static function voidReturnWithValueProgram():HxcIRProgram {
		final file = "test/negative/VoidReturnWithValue.hx";
		return minimalProgram("invalid.VoidReturnWithValue", [
			instruction("bad.value", result("value.return", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2)
		], terminator(IRTReturn("value.return", []), file, 3), [], [], file);
	}

	static function valueReturnWithoutValueProgram():HxcIRProgram {
		final file = "test/negative/ValueReturnWithoutValue.hx";
		return minimalProgram("invalid.ValueReturnWithoutValue", [], terminator(IRTReturn(null, []), file, 2), [], [], file, IRTInt(32, true));
	}

	static function returnTypeMismatchProgram():HxcIRProgram {
		final file = "test/negative/ReturnTypeMismatch.hx";
		return minimalProgram("invalid.ReturnTypeMismatch", [
			instruction("bad.value", result("value.return", IRTBool), IRIOConstant(IRCBool(true)), file, 2)
		], terminator(IRTReturn("value.return", []), file, 3), [], [], file,
			IRTInt(32, true));
	}

	static function cleanupOrderProgram():HxcIRProgram {
		final file = "test/negative/CleanupOrder.hx";
		final locals = [
			local("local.first", IRTInt(32, true), IRLSRegion("cleanup.bad"), IRISInitialized, file, 1),
			local("local.second", IRTInt(32, true), IRLSRegion("cleanup.bad"), IRISInitialized, file, 2)
		];
		final regions:Array<HxcIRCleanupRegion> = [
			{
				id: "cleanup.bad",
				parentId: null,
				actions: [
					cleanupAction("first", IRCADestroy(IRPLocal("local.first"), IRISInitialized, IRISDestroyed), file, 3),
					cleanupAction("second", IRCADestroy(IRPLocal("local.second"), IRISInitialized, IRISDestroyed), file, 4)
				],
				source: span(file, 3, 5)
			}
		];
		return minimalProgram("invalid.CleanupOrder", [], terminator(IRTReturn(null, [
			{regionId: "cleanup.bad", actionId: "first"},
			{regionId: "cleanup.bad", actionId: "second"}
		]), file, 6), locals, regions, file);
	}

	static function absoluteSourceProgram():HxcIRProgram {
		final program = minimalProgram("invalid.AbsoluteSource", [], terminator(IRTReturn(null, []), "test/negative/AbsoluteSource.hx", 2), [], [],
			"test/negative/AbsoluteSource.hx");
		program.modules[0] = {
			id: program.modules[0].id,
			types: program.modules[0].types,
			typeInstances: program.modules[0].typeInstances,
			globals: program.modules[0].globals,
			functions: program.modules[0].functions,
			source: span("/tmp/private/AbsoluteSource.hx", 1)
		};
		return program;
	}

	static function primitiveRuntimeConversionProgram():HxcIRProgram {
		final file = "test/negative/PrimitiveRuntimeConversion.hx";
		return minimalProgram("invalid.PrimitiveRuntimeConversion", [
			instruction("bad.source", result("value.source", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2),
			instruction("bad.convert", result("value.target", IRTFloat(64)),
				IRIOConvert("value.source", IRCNumericExact, IRTFloat(64), IRIRuntime("primitive-conversion"), null), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [], [], file);
	}

	static function invalidFloat32NarrowProgram():HxcIRProgram {
		final file = "test/negative/InvalidFloat32Narrow.hx";
		return minimalProgram("invalid.InvalidFloat32Narrow", [
			instruction("bad.source", result("value.source", IRTFloat(64)), IRIOConstant(IRCFloat("1.0")), file, 2),
			instruction("bad.convert", result("value.target", IRTFloat(64)),
				IRIOConvert("value.source", IRCNumericRoundBinary32, IRTFloat(64), IRIStatic, null), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [], [], file);
	}

	static function invalidFloat32WidenProgram():HxcIRProgram {
		final file = "test/negative/InvalidFloat32Widen.hx";
		return minimalProgram("invalid.InvalidFloat32Widen", [
			instruction("bad.source", result("value.source", IRTFloat(32)), IRIOConstant(IRCFloat("1.0")), file, 2),
			instruction("bad.convert", result("value.target", IRTFloat(32)),
				IRIOConvert("value.source", IRCNumericWidenBinary64, IRTFloat(32), IRIStatic, null), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [], [], file);
	}

	static function nullableUnwrapWithoutFailureProgram():HxcIRProgram {
		final file = "test/negative/NullableUnwrapWithoutFailure.hx";
		return minimalProgram("invalid.NullableUnwrapWithoutFailure", [
			instruction("bad.nullable", result("value.nullable", IRTNullable(IRTInt(32, true), IRNTagged)), IRIOConstant(IRCNull), file, 2),
			instruction("bad.unwrap", result("value.unwrapped", IRTInt(32, true)),
				IRIOConvert("value.nullable", IRCNullableUnwrap, IRTInt(32, true), IRIStatic, null), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [], [], file);
	}

	static function stringByteLengthMismatchProgram():HxcIRProgram {
		final file = "test/negative/StringByteLengthMismatch.hx";
		return minimalProgram("invalid.StringByteLengthMismatch", [
			instruction("bad.string", result("value.string", IRTString), IRIOConstant(IRCString("é🙂", 3)), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [], [], file);
	}

	static function cstringByteLengthMismatchProgram():HxcIRProgram {
		final file = "test/negative/CStringByteLengthMismatch.hx";
		return minimalProgram("invalid.CStringByteLengthMismatch", [
			instruction("bad.cstring", result("value.cstring", IRTCString), IRIOConstant(IRCCStringLiteral("é🙂", 3)), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [], [], file);
	}

	static function cstringEmbeddedNulProgram():HxcIRProgram {
		final file = "test/negative/CStringEmbeddedNul.hx";
		return minimalProgram("invalid.CStringEmbeddedNul", [
			instruction("bad.cstring", result("value.cstring", IRTCString), IRIOConstant(IRCCStringLiteral("before\x00after", 12)), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [], [], file);
	}

	static function ioFailurePolicyProgram():HxcIRProgram {
		final file = "test/negative/IoFailurePolicy.hx";
		return minimalProgram("invalid.IoFailurePolicy", [
			instruction("bad.string", result("value.string", IRTString), IRIOConstant(IRCString("output", 6)), file, 2),
			instruction("bad.output", null, IRIOCall(call(IRCDRuntime("io", "sys-println-literal"), ["value.string"], IRTVoid)), file, 3)
		], terminator(IRTReturn(null, []), file, 4), [], [], file);
	}

	static function defaultInitializationTypeProgram():HxcIRProgram {
		final file = "test/negative/DefaultInitializationType.hx";
		return minimalProgram("invalid.DefaultInitializationType", [
			instruction("bad.default", null, IRIODefaultInitialize(IRPLocal("local.value"), IRISUninitialized, IRISInitialized), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [
			local("local.value", IRTInt(32, true), IRLSAutomatic, IRISUninitialized, file, 1)
		], [], file);
	}

	static function statusConventionReturnTypeProgram():HxcIRProgram {
		final file = "test/negative/StatusConventionReturnType.hx";
		final program = minimalProgram("invalid.StatusConventionReturnType", [
			instruction("bad.value", result("value.result", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2)
		],
			terminator(IRTReturn("value.result", []), file, 3), [], [], file, IRTInt(32, true), IRFCStatus(IRFException));
		return program;
	}

	static function statusCallWithoutFailureProgram():HxcIRProgram {
		final file = "test/negative/StatusCallWithoutFailure.hx";
		final program = minimalProgram("invalid.StatusCallWithoutFailure", [
			instruction("bad.call", null, IRIOCall(call(IRCDDirect("fn.status"), [], IRTVoid)), file, 2)
		], terminator(IRTReturn(null, []), file, 3), [], [], file);
		final target = voidFunction("fn.status", "invalid.StatusCallWithoutFailure.status", file, 4, IRFCStatus(IRFException));
		program.modules[0].functions.push(target);
		return program;
	}

	static function throwWithoutStatusProgram():HxcIRProgram {
		final file = "test/negative/ThrowWithoutStatus.hx";
		return minimalProgram("invalid.ThrowWithoutStatus", [
			instruction("bad.value", result("value.thrown", IRTInt(32, true)), IRIOConstant(IRCInt("1")), file, 2)
		], terminator(IRTThrow("value.thrown", {
			kind: IRFException,
			target: IRFTPropagate,
			arguments: [],
			cleanup: []
		}), file, 3), [], [], file);
	}

	static function minimalProgram(moduleId:String, instructions:Array<HxcIRInstruction>, terminatorValue:Null<HxcIRTerminator>, locals:Array<HxcIRLocal>,
			regions:Array<HxcIRCleanupRegion>, file:String, ?returnType:HxcIRTypeRef, ?failureConvention:HxcIRFunctionFailureConvention):HxcIRProgram {
		final functionReturnType = returnType == null ? IRTVoid : returnType;
		final functionFailureConvention = failureConvention == null ? IRFCInfallible : failureConvention;
		return {
			schemaVersion: HxcIRValidator.SCHEMA_VERSION,
			dispatch: emptyDispatch(),
			modules: [
				{
					id: moduleId,
					types: [],
					typeInstances: [],
					globals: [],
					functions: [
						{
							id: '$moduleId.main',
							displayName: '$moduleId.main',
							parameters: [],
							locals: locals,
							returnType: functionReturnType,
							failureConvention: functionFailureConvention,
							entryBlockId: "entry",
							blocks: [
								{
									id: "entry",
									parameters: [],
									instructions: instructions,
									terminator: terminatorValue,
									source: span(file, 1, 8)
								}
							],
							cleanupRegions: regions,
							source: span(file, 1, 8)
						}
					],
					source: span(file, 1, 8)
				}
			]
		};
	}

	static function emptyDispatch():HxcIRDispatchPlan
		return {layouts: [], slots: [], tables: []};

	static function invalidDiagnostics(program:HxcIRProgram):Array<String> {
		final diagnostics = new HxcIRValidator().validate(program, PROFILE);
		if (diagnostics.length == 0) {
			throw "invalid HxcIR fixture unexpectedly passed validation";
		}
		for (diagnostic in diagnostics) {
			final detail = diagnostic.compilerDetail();
			if (detail.indexOf('[ir=${diagnostic.irPath}; source=${diagnostic.source.display()}]') == -1) {
				throw "compiler-facing HxcIR diagnostic discarded its stable IR path or source";
			}
		}
		return diagnostics.map(diagnostic -> diagnostic.render());
	}

	static function voidFunction(id:String, displayName:String, file:String, line:Int, ?failureConvention:HxcIRFunctionFailureConvention):HxcIRFunction {
		return {
			id: id,
			displayName: displayName,
			parameters: [],
			locals: [],
			returnType: IRTVoid,
			failureConvention: failureConvention == null ? IRFCInfallible : failureConvention,
			entryBlockId: "entry",
			blocks: [
				{
					id: "entry",
					parameters: [],
					instructions: [],
					terminator: terminator(IRTReturn(null, []), file, line),
					source: span(file, line)
				}
			],
			cleanupRegions: [],
			source: span(file, line)
		};
	}

	static function call(dispatch:HxcIRCallDispatch, arguments:Array<String>, returnType:HxcIRTypeRef, ?failure:HxcIRFailureEdge):HxcIRCall {
		return {
			dispatch: dispatch,
			arguments: arguments,
			returnType: returnType,
			failure: failure
		};
	}

	static function instruction(id:String, resultValue:Null<HxcIRResult>, kind:HxcIRInstructionKind, file:String, line:Int):HxcIRInstruction {
		return {
			id: id,
			result: resultValue,
			kind: kind,
			source: span(file, line)
		};
	}

	static function result(id:String, type:HxcIRTypeRef):HxcIRResult
		return {id: id, type: type};

	static function parameter(id:String, type:HxcIRTypeRef, file:String, line:Int):HxcIRParameter
		return {id: id, type: type, source: span(file, line)};

	static function local(id:String, type:HxcIRTypeRef, storage:HxcIRLocalStorage, state:HxcIRInitializationState, file:String, line:Int):HxcIRLocal
		return {
			id: id,
			type: type,
			storage: storage,
			initialState: state,
			source: span(file, line)
		};

	static function cleanupAction(id:String, kind:HxcIRCleanupActionKind, file:String, line:Int):HxcIRCleanupAction
		return {
			id: id,
			idempotence: IRCExactlyOnce,
			kind: kind,
			source: span(file, line)
		};

	static function terminator(kind:HxcIRTerminatorKind, file:String, line:Int):HxcIRTerminator
		return {kind: kind, source: span(file, line)};

	static function span(file:String, startLine:Int, ?endLine:Int):HxcSourceSpan {
		final resolvedEnd = endLine == null ? startLine : endLine;
		return new HxcSourceSpan(file, startLine, 1, resolvedEnd, 2);
	}
}
