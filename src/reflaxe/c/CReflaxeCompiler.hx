package reflaxe.c;

#if (macro || reflaxe_runtime)
import haxe.macro.Context;
import haxe.macro.Type;
import reflaxe.GenericCompiler;
import reflaxe.data.ClassFuncData;
import reflaxe.data.ClassVarData;
import reflaxe.data.EnumOptionData;
import reflaxe.output.DataAndFileInfo;
import reflaxe.output.StringOrBytes;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.ProjectEmissionError;
import reflaxe.c.emit.ReflaxeOutputWriter;
import reflaxe.c.frontend.TypedAstNormalizer;

/** Reflaxe adapter. Semantic lowering remains in `CCompiler`. */
class CReflaxeCompiler extends GenericCompiler<Bool, Bool, Dynamic, Dynamic, Dynamic> {
	var pendingModules:Array<ModuleType> = [];
	var currentModules:Array<ModuleType> = [];
	var generatedFiles:Array<GeneratedFile> = [];
	var compilationContext:Null<CompilationContext> = null;

	public function new() {
		super();
	}

	override public function filterTypes(moduleTypes:Array<ModuleType>):Array<ModuleType> {
		// This runs before Reflaxe filters callbacks, so typedefs and externs are
		// retained even when no individual callback would compile them.
		pendingModules = moduleTypes.copy();
		if (Context.defined("reflaxe_c_test_reverse_typed_modules")) {
			pendingModules.reverse();
		}
		return moduleTypes;
	}

	override public function onCompileStart():Void {
		// Every compilation gets fresh mutable state. Never cache a context across
		// compiler-server requests.
		compilationContext = new CompilationContext(ProfileResolver.resolve());
		generatedFiles = [];
		currentModules = pendingModules;
		pendingModules = [];
	}

	override public function onCompileEnd():Void {
		final context = requireContext();
		final modules = currentModules;
		currentModules = [];
		final program = TypedAstNormalizer.normalize(modules, getMainModule(), getMainExpr());
		context.setTypedProgram(program);
		generatedFiles = new CCompiler(context).compileModules(program);
	}

	override public function generateFilesManually():Void {
		if (output == null) {
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "Reflaxe output manager is not initialized", Context.currentPos());
			return;
		}

		try {
			new ReflaxeOutputWriter().write(output, generatedFiles);
		} catch (error:ProjectEmissionError) {
			CDiagnostic.fatal(error.diagnosticId, error.detail, Context.currentPos());
		}
	}

	public function generateOutputIterator():Iterator<DataAndFileInfo<StringOrBytes>> {
		final empty:Array<DataAndFileInfo<StringOrBytes>> = [];
		return empty.iterator();
	}

	public function compileClassImpl(classType:ClassType, varFields:Array<ClassVarData>, funcFields:Array<ClassFuncData>):Null<Bool> {
		// Selection is intentionally deferred to the whole-program compiler so it
		// can reason about layout, initialization order, runtime slices, and ABI.
		return null;
	}

	public function compileEnumImpl(enumType:EnumType, options:Array<EnumOptionData>):Null<Bool> {
		return null;
	}

	public function compileExpressionImpl(expr:TypedExpr, topLevel:Bool):Null<Dynamic> {
		return null;
	}

	function requireContext():CompilationContext {
		if (compilationContext == null) {
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "missing per-build CompilationContext", Context.currentPos());
		}
		return cast compilationContext;
	}
}
#else
class CReflaxeCompiler {
	public function new() {}
}
#end
