package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Compiler;
import haxe.macro.Context;
import haxe.macro.Expr.Position;
import haxe.macro.Type;
import haxe.Json;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.frontend.TypedAstInventory;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.frontend.TypedProgramInput.TypedAstDeclaration;
import reflaxe.c.lowering.CBodyEmissionError;
import reflaxe.c.lowering.CBodyLowering;
import reflaxe.c.lowering.CBodyLowering.CBodyFunctionInput;
import reflaxe.c.lowering.CBodyLoweringError;
import reflaxe.c.ir.HxcIRValidationError;

/** Whole-program adapter into the first typed HxcIR/C body-lowering slice. */
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
			new CBodyLowering(context).lower([input]);
		} catch (error:CBodyLoweringError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, error.position, context.profile);
		} catch (error:HxcIRValidationError) {
			final diagnostic = error.diagnostics[0];
			CDiagnostic.fatal(diagnostic.id, diagnostic.message, input.expression.pos, context.profile);
		} catch (error:CBodyEmissionError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, input.expression.pos, context.profile);
		}
		CDiagnostic.fatal(CDiagnosticId.LoweringNotImplemented,
			"the typed main body lowered through validated HxcIR and structural C, but static-function, call, and executable entry-point emission are owned by E2.T03; no C was emitted.",
			input.expression.pos, context.profile);
		return [];
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
		return entryPointMainInput(entryPoint.expression);
	}

	/**
		Haxe may retain the typed entry-point call while omitting an otherwise
		unreferenced main class from the module array. The call is only an
		ownership locator here: E2.T03 still owns call and entry-point emission.
	**/
	static function entryPointMainInput(entryPoint:TypedExpr):Null<CBodyFunctionInput> {
		return switch entryPoint.expr {
			case TCall({expr: TField(_, FStatic(classReference, fieldReference))}, arguments) if (arguments.length == 0):
				final owner = classReference.get();
				final field = fieldReference.get();
				final expression = field.expr();
				if (field.name != "main" || expression == null) {
					null;
				} else {
					final staticFields = owner.statics.get();
					var sourceOrder = 0;
					for (index in 0...staticFields.length) {
						if (staticFields[index].name == field.name) {
							sourceOrder = index;
							break;
						}
					}
					{
						modulePath: owner.module,
						declarationPath: owner.pack.concat([owner.name]).join("."),
						sourcePath: owner.module.split(".").join("/") + ".hx",
						fieldName: field.name,
						sourceOrder: sourceOrder,
						expression: expression
					};
				}
			case _:
				null;
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
