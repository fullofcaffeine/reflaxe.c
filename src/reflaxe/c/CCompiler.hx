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
import reflaxe.c.lowering.CBodyLoweringError;
import reflaxe.c.lowering.CStaticFunctionGraph;
import reflaxe.c.lowering.CStaticFunctionGraph.CStaticFunctionGraphCollector;
import reflaxe.c.ir.HxcIRValidationError;
import reflaxe.c.naming.CSymbolRequest;
import reflaxe.c.plan.CDeclarationPlanner;

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

/** Whole-program adapter into the primitive static-function executable slice. */
class CCompiler {
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
					'primitive executable entry emission currently requires the hosted environment; `${configuration.environment}` remains fail-closed.',
					input.expression.pos, context.profile);
			}
			final graph = new CStaticFunctionGraphCollector().collect(input, program);
			final entryRequest = new CSymbolRequest(CSKStaticInitializer, ["compiler", "executable-entry-point", graph.entryFunctionId],
				CNSOrdinary("translation-unit"), CSVInternal, "main");
			final headerGuardRequest = new CSymbolRequest(CSKModule, ["compiler", "program-header", "guard"], CNSPreprocessor, CSVInternal,
				CDeclarationPlanner.headerGuardFor(CStaticFunctionProjectEmitter.HEADER_PATH));
			context.symbols.register(entryRequest);
			context.symbols.register(headerGuardRequest);
			final lowered = new CBodyLowering(context).lower(graph.functions, graph.globals);
			final units = new CStaticFunctionProjectEmitter().emit(lowered, graph.entryFunctionId, context.symbols.identifierFor(entryRequest),
				context.symbols.identifierFor(headerGuardRequest));
			return new CProjectEmitter().emit({
				schemaVersion: CProjectEmitter.SCHEMA_VERSION,
				projectName: input.declarationPath,
				compilationStatus: CProjectCompilationStatus.PrimitiveExecutable,
				profile: context.profile,
				environment: configuration.environment,
				cStandard: configuration.cStandard,
				runtimePolicy: configuration.runtimePolicy,
				runtimeDiagnostics: configuration.runtimeDiagnostics,
				runtimePolicyProvenance: configuration.runtimePolicyProvenance,
				runtimeDiagnosticsProvenance: configuration.runtimeDiagnosticsProvenance,
				units: units,
				buildFacts: [],
				symbolTable: lowered.symbolTable
			});
		} catch (error:CBodyLoweringError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, error.position, context.profile);
		} catch (error:HxcIRValidationError) {
			final diagnostic = error.diagnostics[0];
			CDiagnostic.fatal(diagnostic.id, diagnostic.message, input.expression.pos, context.profile);
		} catch (error:CBodyEmissionError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, input.expression.pos, context.profile);
		} catch (error:ProjectEmissionError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, input.expression.pos, context.profile);
		}
		return [];
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
