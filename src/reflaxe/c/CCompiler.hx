package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.Json;
import reflaxe.c.ast.CAST.CIdentifier;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CPhaseTiming.CPhaseTimingId;
import reflaxe.c.emit.CProjectEmitter;
import reflaxe.c.emit.CProjectEmitter.CProjectCompilationStatus;
import reflaxe.c.emit.CProjectEmitter.CProjectEnvironment;
import reflaxe.c.emit.CProjectEmitter.CProjectRuntimeDiagnostics;
import reflaxe.c.emit.CProjectEmitter.CProjectRuntimePolicy;
import reflaxe.c.emit.CProjectEmitter.CProjectStandard;
import reflaxe.c.emit.CProjectLayout;
import reflaxe.c.emit.CProjectLayout.CProjectLayoutPlan;
import reflaxe.c.emit.CProjectLayout.CProjectLayoutPlanner;
import reflaxe.c.emit.CStaticFunctionProjectEmitter;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.ProjectEmissionError;
import reflaxe.c.frontend.TypedAstInventory;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.frontend.TypedProgramInput.TypedAstDeclaration;
import reflaxe.c.frontend.TypedProgramInput.TypedAstEntryPoint;
import reflaxe.c.lowering.CBodyEmissionError;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CBodyLowering.CBodyGlobalInput;
import reflaxe.c.lowering.CBodyLowering.CBodyInitializerInput;
import reflaxe.c.lowering.CBodyLowering.CBodyLoweringResult;
import reflaxe.c.lowering.CBodyLowering.CBodyRuntimeRequirement;
import reflaxe.c.lowering.CBodyConstructor.CBodyConstructorInput;
import reflaxe.c.lowering.CBodyLoweringError;
import reflaxe.c.lowering.CDispatchReport.CDispatchReportBuilder;
import reflaxe.c.lowering.CDispatchReport.CDispatchReportSnapshot;
import reflaxe.c.lowering.CGenericSpecializationReport.CGenericSpecializationReportBuilder;
import reflaxe.c.lowering.CStaticFunctionGraph;
import reflaxe.c.lowering.CStaticFunctionGraph.CStaticFunctionGraphCollector;
import reflaxe.c.macros.TypedCContractMacro;
import reflaxe.c.ir.HxcIRValidationError;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.ir.HxcIR.HxcIRProgram;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CStaticInitializationError;
import reflaxe.c.plan.CStaticInitializationPlanner;
import reflaxe.c.runtime.RuntimeAbiContract;
import reflaxe.c.runtime.RuntimeFeatureCatalog;
import reflaxe.c.runtime.RuntimeFeatureError;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeatureId;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeEvidence;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeNoRuntimeScope;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningPurpose;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePlanningRequest;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeRequirementCandidate;
import reflaxe.c.runtime.RuntimeFeaturePackager;
import reflaxe.c.runtime.RuntimeFeaturePackager.PackageRuntimeArtifactSource;
import reflaxe.c.runtime.RuntimeFeaturePlanner;
import reflaxe.c.runtime.RuntimeRequirementAnalyzer;

private typedef ResolvedRuntimePolicy = {
	final value:CProjectRuntimePolicy;
	final provenance:String;
}

private typedef ResolvedRuntimeDiagnostics = {
	final value:CProjectRuntimeDiagnostics;
	final provenance:String;
}

private typedef ResolvedProjectConfiguration = {
	final environment:CProjectEnvironment;
	final cStandard:CProjectStandard;
	final projectLayout:CProjectLayout;
	final runtimePolicy:CProjectRuntimePolicy;
	final runtimePolicyProvenance:String;
	final runtimeDiagnostics:CProjectRuntimeDiagnostics;
	final runtimeDiagnosticsProvenance:String;
}

private typedef StaticInitializationInspection = {
	final schemaVersion:Int;
	final plan:reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSnapshot;
	final hxcir:String;
}

private typedef ConstructorInspectionRecord = {
	final id:String;
	final haxePath:String;
	final instanceId:String;
	final elided:Bool;
	final canFail:Bool;
	final cName:Null<String>;
}

private typedef ConstructorLoweringInspection = {
	final schemaVersion:Int;
	final profile:String;
	final constructors:Array<ConstructorInspectionRecord>;
	final hxcir:String;
}

private typedef VirtualDispatchInspection = {
	final schemaVersion:Int;
	final profile:String;
	final dispatch:CDispatchReportSnapshot;
	final hxcir:String;
}

/** Whole-program adapter into the primitive static-function executable slice. */
class CCompiler {
	public static inline final STATIC_INITIALIZATION_REPORT_DEFINE = "reflaxe_c_static_initialization_report";
	public static inline final STATIC_INITIALIZATION_REPORT_PREFIX = "HXC_STATIC_INITIALIZATION=";
	public static inline final CONSTRUCTOR_LOWERING_REPORT_DEFINE = "reflaxe_c_constructor_lowering_report";
	public static inline final CONSTRUCTOR_LOWERING_REPORT_PREFIX = "HXC_CONSTRUCTOR_LOWERING=";
	public static inline final VIRTUAL_DISPATCH_REPORT_DEFINE = "reflaxe_c_virtual_dispatch_report";
	public static inline final VIRTUAL_DISPATCH_REPORT_PREFIX = "HXC_VIRTUAL_DISPATCH=";

	final context:CompilationContext;

	public function new(context:CompilationContext) {
		this.context = context;
	}

	public function compileModules(program:TypedProgramInput):Array<GeneratedFile> {
		if (context.typedProgram != program) {
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "whole-program compiler received an input outside its per-build CompilationContext",
				compilationPosition(), context.profile);
		}
		if (Context.defined(TypedAstInventory.REPORT_DEFINE)) {
			Sys.println(TypedAstInventory.REPORT_PREFIX + Json.stringify(TypedAstInventory.snapshot(program)));
		}
		final input:CBodyFunctionInput = switch mainBodyInput(program) {
			case null:
				CDiagnostic.fatal(CDiagnosticId.LoweringNotImplemented,
					"reflaxe.c could not identify a typed static main body for the admitted body-lowering slice; no C was emitted.", compilationPosition(),
					context.profile);
			case value:
				value;
		};
		try {
			final configurationTimer = CPhaseTiming.start(CPConfigurationAndContracts);
			final configuration = resolveProjectConfiguration(context.profile);
			final typedCContract = TypedCContractMacro.collect(program.rawModules);
			if (configuration.environment != CProjectEnvironment.Hosted) {
				CDiagnostic.fatal(CDiagnosticId.LoweringNotImplemented,
					'direct executable entry emission currently requires the hosted environment; `${configuration.environment}` remains fail-closed.',
					input.expression.pos, context.profile);
			}
			CPhaseTiming.stop(configurationTimer);
			final analysisTimer = CPhaseTiming.start(CPWholeProgramAnalysis);
			final entryFunctionId = CBodyLowering.functionId(input.declarationPath, input.fieldName);
			final staticInitialization = new CStaticInitializationPlanner().plan(program, entryFunctionId);
			context.setStaticInitialization(staticInitialization.snapshot);
			final graph = new CStaticFunctionGraphCollector(context).collect(input, program, staticInitialization.initializerInputs);
			final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", graph.entryFunctionId],
				CNSOrdinary("translation-unit"), CSVInternal, "main");
			final initializationRequest:Null<CSymbolRequest> = staticInitialization.executionFunctionIds.length == 0 ? null : new CSymbolRequest(CSKStaticInitializer,
				["compiler", "static-initialization", "hosted-executable", graph.entryFunctionId], CNSOrdinary("translation-unit"),
				CSVInternal, null, [], [], null, input.readableDeclarationPath == null ? null : [input.readableDeclarationPath, "static-initialization"]);
			final layoutPlanner = new CProjectLayoutPlanner();
			final guardLayout = layoutPlanner.plan(configuration.projectLayout,
				projectModulePaths(program, graph.functions, graph.globals, graph.constructors, staticInitialization.initializerInputs));
			final headerGuardRequests:Map<String, CSymbolRequest> = [];
			for (headerPath in guardLayout.headerPaths) {
				final request = guardLayout.guardRequest(headerPath);
				headerGuardRequests.set(headerPath, request);
				context.symbols.register(request);
			}
			context.symbols.register(entryRequest);
			if (initializationRequest != null) {
				context.symbols.register(initializationRequest);
			}
			CPhaseTiming.stop(analysisTimer);
			final loweringTimer = CPhaseTiming.start(CPSemanticLowering);
			final lowered = new CBodyLowering(context).lower(graph.functions, graph.globals, staticInitialization.initializerInputs, graph.constructors,
				graph.dispatch, program, typedCContract);
			CPhaseTiming.stop(loweringTimer);
			final projectLayout = layoutPlanner.plan(configuration.projectLayout, loweredProjectModulePaths(lowered));
			final dispatchReport = new CDispatchReportBuilder().build(graph.dispatch, lowered.dispatch);
			final reportTimer = CPhaseTiming.start(CPOptionalReports);
			if (Context.defined(STATIC_INITIALIZATION_REPORT_DEFINE)) {
				final inspection:StaticInitializationInspection = {
					schemaVersion: 1,
					plan: staticInitialization.snapshot,
					hxcir: new HxcIRDumper().dump(lowered.program)
				};
				Sys.println(STATIC_INITIALIZATION_REPORT_PREFIX + Json.stringify(inspection));
			}
			if (Context.defined(CONSTRUCTOR_LOWERING_REPORT_DEFINE)) {
				final inspection:ConstructorLoweringInspection = {
					schemaVersion: 1,
					profile: Std.string(context.profile),
					constructors: lowered.constructors.map(value -> {
						id: value.id,
						haxePath: value.haxePath,
						instanceId: value.instanceId,
						elided: value.elided,
						canFail: value.canFail,
						cName: value.cName == null ? null : value.cName.value
					}),
					hxcir: new HxcIRDumper().dump(lowered.program)
				};
				Sys.println(CONSTRUCTOR_LOWERING_REPORT_PREFIX + Json.stringify(inspection));
			}
			if (Context.defined(VIRTUAL_DISPATCH_REPORT_DEFINE) && dispatchReport != null) {
				final inspection:VirtualDispatchInspection = {
					schemaVersion: 1,
					profile: Std.string(context.profile),
					dispatch: dispatchReport,
					hxcir: new HxcIRDumper().dump(lowered.program)
				};
				Sys.println(VIRTUAL_DISPATCH_REPORT_PREFIX + Json.stringify(inspection));
			}
			CPhaseTiming.stop(reportTimer);
			final runtimeTimer = CPhaseTiming.start(CPRuntimePlanning);
			final helperIds = lowered.helpers.map(helper -> helper.helperId);
			final directInstanceCallCount = dispatchReport == null ? 0 : dispatchReport.summary.directCalls;
			final indirectInstanceCallCount = dispatchReport == null ? 0 : dispatchReport.summary.indirectCalls;
			var virtualInstanceCallCount = 0;
			var interfaceInstanceCallCount = 0;
			if (dispatchReport != null)
				for (call in dispatchReport.calls)
					switch call.dispatch {
						case "virtual":
							virtualInstanceCallCount++;
						case "interface":
							interfaceInstanceCallCount++;
						case _:
					}
			final interfaceTypeCount = lowered.dispatch.layouts.filter(layout -> layout.prepared.isInterface()).length;
			final genericFunctionCount = graph.specializations.length;
			final genericTypeCount = lowered.enums.filter(value -> value.prepared.typeParameterNames.length > 0).length;
			final registry = RuntimeFeatureCatalog.registry();
			final runtimePlan = try {
				directRuntimePlan(configuration, helperIds, staticInitialization.snapshot, lowered.program, lowered.runtimeRequirements,
					lowered.aggregates.length, lowered.enums.length, lowered.classes.length, lowered.constructors.length, genericFunctionCount,
					genericTypeCount, virtualInstanceCallCount, interfaceInstanceCallCount,
					lowered.imports.functions.length + lowered.imports.constants.length, registry);
			} catch (error:RuntimeFeatureError) {
				CDiagnostic.fatal(error.diagnosticId, error.message, runtimeErrorPosition(error, lowered.runtimeRequirements, input.expression.pos),
					context.profile);
			};
			context.setRuntimePlan(runtimePlan);
			emitRuntimeDiagnostics(configuration.runtimeDiagnostics, runtimePlan, lowered.runtimeRequirements, input.expression.pos);
			CPhaseTiming.stop(runtimeTimer);
			final projectPlanTimer = CPhaseTiming.start(CPCASTProjectPlanning);
			final initializationName = initializationRequest == null ? null : context.symbols.identifierFor(initializationRequest);
			final runtimeAbiMajor = runtimePlan.features.length == 0 ? null : RuntimeAbiContract.MAJOR;
			final staticProjectEmitter = new CStaticFunctionProjectEmitter();
			final headerGuards:Map<String, CIdentifier> = [];
			for (headerPath in projectLayout.headerPaths) {
				final request = headerGuardRequests.get(headerPath);
				if (request == null)
					throw new ProjectEmissionError('project layout lost header guard request for `$headerPath`');
				headerGuards.set(headerPath, context.symbols.identifierFor(request));
			}
			final staticProject = staticProjectEmitter.planWithLayout(lowered, graph.entryFunctionId, context.symbols.identifierFor(entryRequest),
				projectLayout, headerGuards, staticInitialization.executionFunctionIds, initializationName, runtimeAbiMajor);
			CPhaseTiming.stop(projectPlanTimer);
			final printingTimer = CPhaseTiming.start(CPCPrinting);
			final units = staticProjectEmitter.emitPlan(staticProject);
			CPhaseTiming.stop(printingTimer);
			final artifactPlanTimer = CPhaseTiming.start(CPArtifactPlanning);
			for (runtimeFile in new RuntimeFeaturePackager(registry).packageFiles(runtimePlan, new PackageRuntimeArtifactSource())) {
				units.push(runtimeFile);
			}
			final specializationReport = new CGenericSpecializationReportBuilder(context).build(graph, lowered, staticProject.functionDefinitions, units,
				input.expression.pos, input.sourcePath);
			final generatedFiles = new CProjectEmitter().emit({
				schemaVersion: CProjectEmitter.SCHEMA_VERSION,
				projectName: input.readableDeclarationPath == null ? input.declarationPath : input.readableDeclarationPath,
				compilationStatus: lowered.aggregates.length == 0
				&& lowered.enums.length == 0
				&& lowered.classes.length == 0
				&& lowered.imports.functions.length == 0
				&& lowered.imports.constants.length == 0
				&& genericFunctionCount == 0 ? CProjectCompilationStatus.PrimitiveExecutable : CProjectCompilationStatus.DirectValueExecutable,
				profile: context.profile,
				environment: configuration.environment,
				cStandard: configuration.cStandard,
				projectLayout: configuration.projectLayout,
				runtimePolicy: configuration.runtimePolicy,
				runtimeDiagnostics: configuration.runtimeDiagnostics,
				runtimePolicyProvenance: configuration.runtimePolicyProvenance,
				runtimeDiagnosticsProvenance: configuration.runtimeDiagnosticsProvenance,
				units: units,
				buildFacts: lowered.buildFacts,
				primitiveHelperIds: helperIds,
				directAggregateCount: lowered.aggregates.length,
				directEnumCount: lowered.enums.length,
				directClassCount: lowered.classes.length,
				directInterfaceCount: interfaceTypeCount,
				directConstructorCount: lowered.constructors.length,
				directGenericFunctionCount: genericFunctionCount,
				directGenericTypeCount: genericTypeCount,
				directImportCount: lowered.imports.types.length + lowered.imports.functions.length + lowered.imports.constants.length,
				directImportTypeCount: lowered.imports.types.length,
				directInstanceCallCount: directInstanceCallCount,
				indirectInstanceCallCount: indirectInstanceCallCount,
				dispatchReport: dispatchReport,
				specializationReport: specializationReport,
				stdlibModules: stdlibModules(lowered.runtimeRequirements),
				stdlibCapabilities: stdlibCapabilities(lowered.runtimeRequirements),
				staticInitialization: staticInitialization.snapshot,
				runtimePlan: runtimePlan,
				symbolTable: lowered.symbolTable
			});
			CPhaseTiming.stop(artifactPlanTimer);
			return generatedFiles;
		} catch (error:CStaticInitializationError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, error.position, context.profile);
		} catch (error:CBodyLoweringError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, error.position, context.profile);
		} catch (error:HxcIRValidationError) {
			final diagnostic = error.diagnostics[0];
			// Keep the exact IR location even when the original macro Position is no
			// longer available. Without it, a whole-function internal-compiler-error
			// diagnostic leaves the author guessing which generated instruction violated
			// the compiler invariant.
			CDiagnostic.fatal(diagnostic.id, diagnostic.compilerDetail(), input.expression.pos, context.profile);
		} catch (error:CBodyEmissionError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, input.expression.pos, context.profile);
		} catch (error:RuntimeFeatureError) {
			CDiagnostic.fatal(error.diagnosticId, error.message, input.expression.pos, context.profile);
		} catch (error:ProjectEmissionError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, input.expression.pos, context.profile);
		}
		return [];
	}

	function directRuntimePlan(configuration:ResolvedProjectConfiguration, helperIds:Array<String>,
			staticInitialization:reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSnapshot, program:HxcIRProgram,
			runtimeRequirements:Array<CBodyRuntimeRequirement>, aggregateCount:Int, enumCount:Int, classCount:Int, constructorCount:Int,
			genericFunctionCount:Int, genericTypeCount:Int, virtualInstanceCallCount:Int, interfaceInstanceCallCount:Int, importOperationCount:Int,
			registry:reflaxe.c.runtime.RuntimeFeatureRegistry):RuntimeFeaturePlanSnapshot {
		final directDecisions = [
			"primitive-values",
			"ub-safe-primitive-operations",
			"primitive-static-storage",
			"static-functions",
			"direct-calls",
			"explicit-evaluation-order",
			"executable-entry-point"
		];
		if (helperIds.length > 0) {
			directDecisions.push("selected-program-local-helpers");
		}
		if (aggregateCount > 0) {
			directDecisions.push("closed-anonymous-value-records");
		}
		if (enumCount > 0) {
			directDecisions.push("bounded-haxe-enum-values");
		}
		if (classCount > 0) {
			directDecisions.push("concrete-class-reference-layouts");
		}
		if (constructorCount > 0) {
			directDecisions.push("bounded-stack-construction");
		}
		if (genericFunctionCount + genericTypeCount > 0) {
			directDecisions.push("closed-generic-specializations");
		}
		if (virtualInstanceCallCount > 0) {
			directDecisions.push("reachable-program-local-virtual-dispatch");
		}
		if (interfaceInstanceCallCount > 0)
			directDecisions.push("reachable-program-local-interface-dispatch");
		if (importOperationCount > 0) {
			directDecisions.push("typed-header-owned-c-imports");
		}
		if (staticInitialization.executionOrder.length > 0) {
			directDecisions.push("compiler-planned-eager-static-initialization");
		}
		if (hasRuntimeFeature(runtimeRequirements, "string-literal")
			|| hasRuntimeFeature(runtimeRequirements, "string-scalar")
			|| hasRuntimeFeature(runtimeRequirements, "io")
			|| hasRuntimeFeature(runtimeRequirements, "string-map")
			|| hasRuntimeFeature(runtimeRequirements, "bytes")) {
			directDecisions.push("direct-utf8-string-literals");
		}
		if (hasRuntimeFeature(runtimeRequirements, "array"))
			directDecisions.push("managed-haxe-arrays");
		if (hasRuntimeFeature(runtimeRequirements, "string-map"))
			directDecisions.push("managed-haxe-string-maps");
		if (hasRuntimeFeature(runtimeRequirements, "bytes"))
			directDecisions.push("managed-haxe-bytes");
		if (hasRuntimeFeature(runtimeRequirements, "string-scalar"))
			directDecisions.push("allocation-free-unicode-scalar-strings");
		if (hasRuntimeFeature(runtimeRequirements, "gc"))
			directDecisions.push("exact-traced-haxe-object-graph");
		var proof = "reachable validated HxcIR contains only direct primitive storage, operations, functions, conversions, sequenced control flow, and calls";
		if (helperIds.length > 0) {
			proof = "reachable validated HxcIR contains only direct primitive storage, operations, request-local helpers, functions, conversions, sequenced control flow, and calls";
		}
		if (aggregateCount > 0) {
			proof += ", plus shape-deduplicated closed anonymous records with direct value storage and no runtime intent";
		}
		if (enumCount > 0) {
			proof += ", plus specialized native/tagged Haxe enum values with checked payload projection and finite recursive pointer layouts";
		}
		if (classCount > 0) {
			proof += ", plus nullable concrete class references with base-prefix subobjects, explicit null checks, and private direct field storage";
		}
		if (constructorCount > 0) {
			proof += ", with compiler-proven stack construction, explicit partial/initialized cleanup order, and no escaping object lifetime";
		}
		if (genericFunctionCount + genericTypeCount > 0) {
			proof += ", with closed generic instances shared by collision-checked semantic keys and bounded code-size planning";
		}
		if (virtualInstanceCallCount > 0) {
			proof += ", with root-only program-local vtable pointers, reachable slots, concrete tables, and representation-checked override adapters selecting no runtime feature";
		}
		if (interfaceInstanceCallCount > 0)
			proof += ", with compact program-local interface values, reachable itables, and typed object-pointer adapters selecting no runtime feature";
		if (importOperationCount > 0) {
			proof += ", plus exact non-variadic header-owned C calls and nominal by-value imports with no wrapper allocation";
		}
		if (staticInitialization.executionOrder.length > 0) {
			proof += ", with eager static initialization planned and emitted entirely by the compiler";
		}
		if (hasRuntimeFeature(runtimeRequirements, "bytes"))
			proof += ", plus fixed-length shared haxe.io.Bytes storage with explicit ownership and checked binary operations";
		final candidates:Array<RuntimeRequirementCandidate> = [];
		for (requirement in runtimeRequirements) {
			candidates.push(new RuntimeRequirementCandidate(RuntimeFeatureId.parse(requirement.featureId), requirement.operationId, requirement.kind,
				requirement.surface, requirement.source));
		}
		final analysis = new RuntimeRequirementAnalyzer().analyze(program, candidates);
		final noRuntimeEvidence = analysis.reasons.length == 0 ? new RuntimeNoRuntimeEvidence(RuntimeNoRuntimeScope.ReachableWholeProgram, proof,
			analysis.reachability, helperIds) : null;
		return new RuntimeFeaturePlanner(registry).plan(new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, context.profile,
			configuration.environment, configuration.runtimePolicy, configuration.runtimePolicyProvenance, configuration.runtimeDiagnostics,
			configuration.runtimeDiagnosticsProvenance, analysis.reasons, [], directDecisions, noRuntimeEvidence));
	}

	static function hasRuntimeFeature(requirements:Array<CBodyRuntimeRequirement>, featureId:String):Bool {
		for (requirement in requirements)
			if (requirement.featureId == featureId)
				return true;
		return false;
	}

	static function runtimeErrorPosition(error:RuntimeFeatureError, requirements:Array<CBodyRuntimeRequirement>, fallback:Position):Position {
		final source = error.primarySource;
		if (source != null) {
			for (requirement in requirements) {
				if (requirement.source.file == source.file
					&& requirement.source.startLine == source.startLine
					&& requirement.source.startColumn == source.startColumn
					&& requirement.source.endLine == source.endLine
					&& requirement.source.endColumn == source.endColumn) {
					if (requirement.position != null)
						return requirement.position;
				}
			}
		}
		return fallback;
	}

	static function emitRuntimeDiagnostics(mode:CProjectRuntimeDiagnostics, plan:RuntimeFeaturePlanSnapshot, requirements:Array<CBodyRuntimeRequirement>,
			summaryPosition:Position):Void {
		if (requirements.length == 0 || mode == CProjectRuntimeDiagnostics.Off) {
			return;
		}
		if (mode == CProjectRuntimeDiagnostics.Summary) {
			CDiagnostic.info(CDiagnosticId.RuntimeFeatureSelected,
				'hxrt selected ${plan.features.length} dependency-closed feature(s) for ${requirements.length} typed runtime root(s): ${plan.features.join(", ")}.',
				summaryPosition, Std.string(plan.profile));
			return;
		}
		for (requirement in requirements) {
			CDiagnostic.warning(CDiagnosticId.RuntimeFeatureSelected,
				'Runtime feature `${requirement.featureId}` was selected for `${requirement.surface}`; transitive dependencies are recorded only in hxc.runtime-plan.json.',
				requirement.position == null ? summaryPosition : requirement.position, Std.string(plan.profile));
		}
	}

	static function stdlibModules(requirements:Array<CBodyRuntimeRequirement>):Array<String> {
		final modules:Array<String> = [];
		for (requirement in requirements) {
			final module = switch requirement.featureId {
				case "array": "Array";
				case "bytes": "haxe.io.Bytes";
				case "string-literal" | "string-scalar": "String";
				case "io" if (requirement.operationId == "trace-literal"): "haxe.Log";
				case "io": "Sys";
				case _: requirement.featureId;
			};
			if (modules.indexOf(module) == -1) {
				modules.push(module);
			}
		}
		modules.sort(CBodyLowering.compareUtf8);
		return modules;
	}

	static function stdlibCapabilities(requirements:Array<CBodyRuntimeRequirement>):Array<String> {
		final capabilities:Array<String> = [];
		for (requirement in requirements) {
			if (capabilities.indexOf(requirement.operationId) == -1) {
				capabilities.push(requirement.operationId);
			}
		}
		capabilities.sort(CBodyLowering.compareUtf8);
		return capabilities;
	}

	static function projectModulePaths(program:TypedProgramInput, functions:Array<CBodyFunctionInput>, globals:Array<CBodyGlobalInput>,
			constructors:Array<CBodyConstructorInput>, initializers:Array<CBodyInitializerInput>):Array<String> {
		final paths:Array<String> = [];
		// Header guards must be finalized with every normalized typed module that
		// can later own a reached value type. The exact emitted subset is selected
		// from validated HxcIR after lowering; unused candidate guards never become
		// files.
		for (module in program.modules)
			addProjectModulePath(paths, module.path);
		for (fn in functions)
			addProjectModulePath(paths, fn.modulePath);
		for (global in globals)
			addProjectModulePath(paths, global.modulePath);
		for (constructor in constructors)
			addProjectModulePath(paths, constructor.modulePath);
		for (initializer in initializers)
			addProjectModulePath(paths, initializer.modulePath);
		paths.sort(CBodyLowering.compareUtf8);
		return paths;
	}

	static function loweredProjectModulePaths(lowered:CBodyLoweringResult):Array<String> {
		final paths:Array<String> = [];
		for (module in lowered.program.modules)
			addProjectModulePath(paths, module.id);
		paths.sort(CBodyLowering.compareUtf8);
		return paths;
	}

	static function addProjectModulePath(paths:Array<String>, modulePath:String):Void {
		if (paths.indexOf(modulePath) == -1)
			paths.push(modulePath);
	}

	static function resolveProjectConfiguration(profile:CProfile):ResolvedProjectConfiguration {
		final runtime = resolveRuntimePolicy(profile);
		final diagnostics = resolveRuntimeDiagnostics(profile);
		return {
			environment: resolveEnvironment(),
			cStandard: resolveCStandard(),
			projectLayout: resolveProjectLayout(profile),
			runtimePolicy: runtime.value,
			runtimePolicyProvenance: runtime.provenance,
			runtimeDiagnostics: diagnostics.value,
			runtimeDiagnosticsProvenance: diagnostics.provenance
		};
	}

	static function resolveProjectLayout(profile:CProfile):CProjectLayout {
		return switch Context.definedValue("hxc_project_layout") {
			case null | "" | "split": CProjectLayout.Split;
			case "package": CProjectLayout.Package;
			case "unity": CProjectLayout.Unity;
			case invalid:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'invalid hxc_project_layout `$invalid`; expected split, package, or unity.',
					compilationPosition(), profile);
		};
	}

	static function resolveRuntimePolicy(profile:CProfile):ResolvedRuntimePolicy {
		final raw = Context.definedValue("hxc_runtime");
		return switch raw {
			case null | "":
				profile == CProfile.Portable ? {value: CProjectRuntimePolicy.Auto, provenance: "profile-preset:portable"} : {
					value: CProjectRuntimePolicy.Minimal,
					provenance: "profile-preset:metal"
				};
			case "auto": {value: CProjectRuntimePolicy.Auto, provenance: "direct-define:hxc_runtime"};
			case "minimal": {value: CProjectRuntimePolicy.Minimal, provenance: "direct-define:hxc_runtime"};
			case "none": {value: CProjectRuntimePolicy.None, provenance: "direct-define:hxc_runtime"};
			case invalid:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'invalid hxc_runtime `$invalid`; expected auto, minimal, or none.',
					compilationPosition(), profile);
		};
	}

	static function resolveRuntimeDiagnostics(profile:CProfile):ResolvedRuntimeDiagnostics {
		final raw = Context.definedValue("hxc_runtime_diagnostics");
		return switch raw {
			case null | "":
				profile == CProfile.Portable ? {value: CProjectRuntimeDiagnostics.Summary, provenance: "profile-preset:portable"} : {
					value: CProjectRuntimeDiagnostics.Warn,
					provenance: "profile-preset:metal"
				};
			case "off": {value: CProjectRuntimeDiagnostics.Off, provenance: "direct-define:hxc_runtime_diagnostics"};
			case "summary": {value: CProjectRuntimeDiagnostics.Summary, provenance: "direct-define:hxc_runtime_diagnostics"};
			case "warn": {value: CProjectRuntimeDiagnostics.Warn, provenance: "direct-define:hxc_runtime_diagnostics"};
			case invalid:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'invalid hxc_runtime_diagnostics `$invalid`; expected off, summary, or warn.',
					compilationPosition(), profile);
		};
	}

	static function resolveEnvironment():CProjectEnvironment {
		return switch Context.definedValue(TargetPlatform.ENVIRONMENT_DEFINE) {
			case "hosted": CProjectEnvironment.Hosted;
			case "freestanding": CProjectEnvironment.Freestanding;
			case "wasi": CProjectEnvironment.Wasi;
			case "emscripten": CProjectEnvironment.Emscripten;
			case value:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'C platform has unresolved environment `${value == null ? "null" : value}`.',
					compilationPosition(), "unresolved");
		};
	}

	static function resolveCStandard():CProjectStandard {
		return switch Context.definedValue("hxc_c_standard") {
			case null | "" | "c11": CProjectStandard.C11;
			case "c17": CProjectStandard.C17;
			case "c23": CProjectStandard.C23Experimental;
			case invalid:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'invalid hxc_c_standard `$invalid`; expected c11, c17, or c23.', compilationPosition());
		};
	}

	static function mainBodyInput(program:TypedProgramInput):Null<CBodyFunctionInput> {
		final entryPoint = program.entryPoint;
		if (entryPoint == null || entryPoint.declarationPath == null) {
			return null;
		}
		final declarationPath = entryPoint.declarationPath;
		for (declaration in program.declarations) {
			if (declaration.path != declarationPath) {
				continue;
			}
			final input = staticMainInput(declaration);
			if (input != null) {
				return input;
			}
		}
		return entryPointMainInput(entryPoint);
	}

	/**
		Haxe may retain the typed entry-point call while omitting an otherwise
		unreferenced main class from the module array. The call is only an
		ownership locator here; the reachable graph collector owns subsequent calls.
	**/
	static function entryPointMainInput(entryPoint:TypedAstEntryPoint):Null<CBodyFunctionInput> {
		final target = entryPoint.target;
		if (target == null || target.fieldName != "main") {
			return null;
		}
		return {
			modulePath: target.modulePath,
			declarationPath: target.declarationPath,
			readableDeclarationPath: target.readableDeclarationPath,
			sourcePath: target.sourcePath,
			fieldName: target.fieldName,
			sourceOrder: target.sourceOrder,
			fieldType: target.fieldType,
			expression: target.expression,
			typeParameters: [],
			specialization: null
		};
	}

	static function staticMainInput(declaration:TypedAstDeclaration):Null<CBodyFunctionInput> {
		for (field in declaration.fields) {
			if (field.role != "static" || field.name != "main" || field.expression == null) {
				continue;
			}
			return {
				modulePath: declaration.ownerModulePath,
				declarationPath: declaration.path,
				readableDeclarationPath: declaration.classKind == "module-fields" ? declaration.ownerModulePath : null,
				sourcePath: declaration.sourcePath,
				fieldName: field.name,
				sourceOrder: field.sourceOrder,
				fieldType: switch field.rawClassField {
					case null: throw "normalized static main field lost its typed ClassField";
					case raw: raw.type;
				},
				expression: field.expression,
				typeParameters: switch field.rawClassField {
					case null: throw "normalized static main field lost its typed ClassField";
					case raw: raw.params;
				},
				specialization: null
			};
		}
		return null;
	}

	static function compilationPosition():Position {
		final configuration = Compiler.getConfiguration();
		if (configuration != null && configuration.mainClass != null) {
			final mainClass = configuration.mainClass;
			final relativePath = mainClass.pack.concat([mainClass.name]).join("/") + ".hx";
			try {
				final file = Context.resolvePath(relativePath);
				return Context.makePosition({file: file, min: 0, max: 0});
			} catch (_:haxe.Exception) {}
		}
		return Context.currentPos();
	}
}
#else
class CCompiler {
	public function new(context:CompilationContext) {}
}
#end
