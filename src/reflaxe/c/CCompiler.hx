package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.Json;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.emit.CProjectEmitter;
import reflaxe.c.emit.CProjectEmitter.CProjectCompilationStatus;
import reflaxe.c.emit.CProjectEmitter.CProjectEnvironment;
import reflaxe.c.emit.CProjectEmitter.CProjectRuntimeDiagnostics;
import reflaxe.c.emit.CProjectEmitter.CProjectRuntimePolicy;
import reflaxe.c.emit.CProjectEmitter.CProjectStandard;
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
import reflaxe.c.lowering.CBodyLowering.CBodyRuntimeRequirement;
import reflaxe.c.lowering.CBodyLoweringError;
import reflaxe.c.lowering.CStaticFunctionGraph;
import reflaxe.c.lowering.CStaticFunctionGraph.CStaticFunctionGraphCollector;
import reflaxe.c.ir.HxcIRValidationError;
import reflaxe.c.ir.HxcIRDumper;
import reflaxe.c.ir.HxcIR.HxcIRProgram;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CDeclarationPlanner;
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

/** Whole-program adapter into the primitive static-function executable slice. */
class CCompiler {
	public static inline final STATIC_INITIALIZATION_REPORT_DEFINE = "reflaxe_c_static_initialization_report";
	public static inline final STATIC_INITIALIZATION_REPORT_PREFIX = "HXC_STATIC_INITIALIZATION=";

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
			final configuration = resolveProjectConfiguration(context.profile);
			if (configuration.environment != CProjectEnvironment.Hosted) {
				CDiagnostic.fatal(CDiagnosticId.LoweringNotImplemented,
					'direct executable entry emission currently requires the hosted environment; `${configuration.environment}` remains fail-closed.',
					input.expression.pos, context.profile);
			}
			final entryFunctionId = CBodyLowering.functionId(input.declarationPath, input.fieldName);
			final staticInitialization = new CStaticInitializationPlanner().plan(program, entryFunctionId);
			context.setStaticInitialization(staticInitialization.snapshot);
			final graph = new CStaticFunctionGraphCollector().collect(input, program, staticInitialization.initializerInputs);
			final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", graph.entryFunctionId],
				CNSOrdinary("translation-unit"), CSVInternal, "main");
			final initializationRequest:Null<CSymbolRequest> = staticInitialization.executionFunctionIds.length == 0 ? null : new CSymbolRequest(CSKStaticInitializer,
				["compiler", "static-initialization", "hosted-executable", graph.entryFunctionId], CNSOrdinary("translation-unit"),
				CSVInternal);
			final headerGuardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
				CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
			context.symbols.register(entryRequest);
			if (initializationRequest != null) {
				context.symbols.register(initializationRequest);
			}
			context.symbols.register(headerGuardRequest);
			final lowered = new CBodyLowering(context).lower(graph.functions, graph.globals, staticInitialization.initializerInputs);
			if (Context.defined(STATIC_INITIALIZATION_REPORT_DEFINE)) {
				final inspection:StaticInitializationInspection = {
					schemaVersion: 1,
					plan: staticInitialization.snapshot,
					hxcir: new HxcIRDumper().dump(lowered.program)
				};
				Sys.println(STATIC_INITIALIZATION_REPORT_PREFIX + Json.stringify(inspection));
			}
			final helperIds = lowered.helpers.map(helper -> helper.helperId);
			final registry = RuntimeFeatureCatalog.registry();
			final runtimePlan = try {
				directRuntimePlan(configuration, helperIds, staticInitialization.snapshot, lowered.program, lowered.runtimeRequirements,
					lowered.aggregates.length, registry);
			} catch (error:RuntimeFeatureError) {
				CDiagnostic.fatal(error.diagnosticId, error.message, runtimeErrorPosition(error, lowered.runtimeRequirements, input.expression.pos),
					context.profile);
			};
			context.setRuntimePlan(runtimePlan);
			emitRuntimeDiagnostics(configuration.runtimeDiagnostics, runtimePlan, lowered.runtimeRequirements, input.expression.pos);
			final initializationName = initializationRequest == null ? null : context.symbols.identifierFor(initializationRequest);
			final runtimeAbiMajor = runtimePlan.features.length == 0 ? null : RuntimeAbiContract.MAJOR;
			final units = new CStaticFunctionProjectEmitter().emit(lowered, graph.entryFunctionId, context.symbols.identifierFor(entryRequest),
				context.symbols.identifierFor(headerGuardRequest), staticInitialization.executionFunctionIds, initializationName, runtimeAbiMajor);
			for (runtimeFile in new RuntimeFeaturePackager(registry).packageFiles(runtimePlan, new PackageRuntimeArtifactSource())) {
				units.push(runtimeFile);
			}
			return new CProjectEmitter().emit({
				schemaVersion: CProjectEmitter.SCHEMA_VERSION,
				projectName: input.declarationPath,
				compilationStatus: lowered.aggregates.length == 0 ? CProjectCompilationStatus.PrimitiveExecutable : CProjectCompilationStatus.DirectValueExecutable,
				profile: context.profile,
				environment: configuration.environment,
				cStandard: configuration.cStandard,
				runtimePolicy: configuration.runtimePolicy,
				runtimeDiagnostics: configuration.runtimeDiagnostics,
				runtimePolicyProvenance: configuration.runtimePolicyProvenance,
				runtimeDiagnosticsProvenance: configuration.runtimeDiagnosticsProvenance,
				units: units,
				buildFacts: lowered.buildFacts,
				primitiveHelperIds: helperIds,
				directAggregateCount: lowered.aggregates.length,
				stdlibModules: stdlibModules(lowered.runtimeRequirements),
				stdlibCapabilities: stdlibCapabilities(lowered.runtimeRequirements),
				staticInitialization: staticInitialization.snapshot,
				runtimePlan: runtimePlan,
				symbolTable: lowered.symbolTable
			});
		} catch (error:CStaticInitializationError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, error.position, context.profile);
		} catch (error:CBodyLoweringError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, error.position, context.profile);
		} catch (error:HxcIRValidationError) {
			final diagnostic = error.diagnostics[0];
			CDiagnostic.fatal(diagnostic.id, diagnostic.message, input.expression.pos, context.profile);
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
			runtimeRequirements:Array<CBodyRuntimeRequirement>, aggregateCount:Int,
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
		if (staticInitialization.executionOrder.length > 0) {
			directDecisions.push("compiler-planned-eager-static-initialization");
		}
		if (runtimeRequirements.length > 0) {
			directDecisions.push("direct-utf8-string-literals");
		}
		var proof = "reachable validated HxcIR contains only direct primitive storage, operations, functions, conversions, sequenced control flow, and calls";
		if (helperIds.length > 0) {
			proof = "reachable validated HxcIR contains only direct primitive storage, operations, request-local helpers, functions, conversions, sequenced control flow, and calls";
		}
		if (aggregateCount > 0) {
			proof += ", plus shape-deduplicated closed anonymous records with direct value storage and no runtime intent";
		}
		if (staticInitialization.executionOrder.length > 0) {
			proof += ", with eager static initialization planned and emitted entirely by the compiler";
		}
		final candidates:Array<RuntimeRequirementCandidate> = [];
		for (requirement in runtimeRequirements) {
			candidates.push(new RuntimeRequirementCandidate(RuntimeFeatureId.parse(requirement.featureId), requirement.operationId, "hosted-output",
				requirement.surface, requirement.source));
		}
		final analysis = new RuntimeRequirementAnalyzer().analyze(program, candidates);
		final noRuntimeEvidence = analysis.reasons.length == 0 ? new RuntimeNoRuntimeEvidence(RuntimeNoRuntimeScope.ReachableWholeProgram, proof,
			analysis.reachability, helperIds) : null;
		return new RuntimeFeaturePlanner(registry).plan(new RuntimePlanningRequest(RuntimePlanningPurpose.CompilerProgram, context.profile,
			configuration.environment, configuration.runtimePolicy, configuration.runtimePolicyProvenance, configuration.runtimeDiagnostics,
			configuration.runtimeDiagnosticsProvenance, analysis.reasons, [], directDecisions, noRuntimeEvidence));
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
				'hxrt selected ${plan.features.length} dependency-closed feature(s) for ${requirements.length} hosted output root(s): ${plan.features.join(", ")}.',
				summaryPosition, Std.string(plan.profile));
			return;
		}
		for (requirement in requirements) {
			CDiagnostic.warning(CDiagnosticId.RuntimeFeatureSelected,
				'Runtime feature `io` was selected for `${requirement.surface}`; transitive dependencies are recorded only in hxc.runtime-plan.json.',
				requirement.position, Std.string(plan.profile));
		}
	}

	static function stdlibModules(requirements:Array<CBodyRuntimeRequirement>):Array<String> {
		final modules:Array<String> = [];
		for (requirement in requirements) {
			final module = requirement.operationId == "trace-literal" ? "haxe.Log" : "Sys";
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

	static function resolveProjectConfiguration(profile:CProfile):ResolvedProjectConfiguration {
		final runtime = resolveRuntimePolicy(profile);
		final diagnostics = resolveRuntimeDiagnostics(profile);
		return {
			environment: resolveEnvironment(),
			cStandard: resolveCStandard(),
			runtimePolicy: runtime.value,
			runtimePolicyProvenance: runtime.provenance,
			runtimeDiagnostics: diagnostics.value,
			runtimeDiagnosticsProvenance: diagnostics.provenance
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
			sourcePath: target.sourcePath,
			fieldName: target.fieldName,
			sourceOrder: target.sourceOrder,
			fieldType: target.fieldType,
			expression: target.expression
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
				sourcePath: declaration.sourcePath,
				fieldName: field.name,
				sourceOrder: field.sourceOrder,
				fieldType: switch field.rawClassField {
					case null: throw "normalized static main field lost its typed ClassField";
					case raw: raw.type;
				},
				expression: field.expression
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
