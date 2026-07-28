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
import reflaxe.c.CPhaseTiming.CProfileCounterId;
import reflaxe.c.CPhaseTiming.CPhaseTimer;
import reflaxe.c.CPhaseTiming.CPhaseTimingId;
import reflaxe.c.emit.GeneratedFile;
import reflaxe.c.emit.GeneratedFileDigestCache;
import reflaxe.c.emit.ProjectEmissionError;
import reflaxe.c.emit.ReflaxeOutputWriter;
import reflaxe.c.frontend.TypedAstNormalizer;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.frontend.NamedRecordSourceProvenance.NamedRecordSourceProvenanceError;

/** Reflaxe adapter. Semantic lowering remains in `CCompiler`. */
class CReflaxeCompiler extends GenericCompiler<Bool, Bool, Bool, Bool, Bool> {
	var pendingProgram:Null<TypedProgramInput> = null;
	var currentProgram:Null<TypedProgramInput> = null;
	var generatedFiles:Array<GeneratedFile> = [];
	var compilationContext:Null<CompilationContext> = null;
	var targetTimer:Null<CPhaseTimer> = null;
	var outputTimer:Null<CPhaseTimer> = null;

	public function new() {
		super();
	}

	override public function filterTypes(moduleTypes:Array<ModuleType>):Array<ModuleType> {
		// This runs before Reflaxe filters callbacks, so typedefs and externs are
		// retained even when no individual callback would compile them. Capture
		// expressions now as well: the framework may replace field expressions
		// during later callback preprocessing.
		CPhaseTiming.beginRequest();
		final capturedModules = moduleTypes.copy();
		if (Context.defined("reflaxe_c_test_reverse_typed_modules")) {
			capturedModules.reverse();
		}
		final captureTimer = CPhaseTiming.start(CPTypedInputCapture);
		try {
			pendingProgram = TypedAstNormalizer.normalize(capturedModules, getMainModule(), getMainExpr());
		} catch (error:NamedRecordSourceProvenanceError) {
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, error.detail, error.position);
		}
		final captured = pendingProgram;
		if (captured != null) {
			CPhaseTiming.setCounter(CPCounterTypedModules, captured.modules.length);
			CPhaseTiming.setCounter(CPCounterTypedDeclarations, captured.declarations.length);
			CPhaseTiming.setCounter(CPCounterTypedExpressionRoots, captured.expressionRoots.length);
		}
		CPhaseTiming.stop(captureTimer);
		return moduleTypes;
	}

	override public function onCompileStart():Void {
		// Every compilation gets fresh mutable state. Never cache a context across
		// compiler-server requests.
		targetTimer = CPhaseTiming.start(CPTargetPipeline);
		final profile = ProfileResolver.resolve();
		final buildMode = BuildModeResolver.resolve(profile);
		CPhaseTiming.describeRequest(Std.string(profile), Std.string(buildMode));
		GeneratedFileDigestCache.beginRequest(!Context.defined(GeneratedFileDigestCache.DISABLE_DEFINE));
		compilationContext = new CompilationContext(profile, buildMode);
		generatedFiles = [];
		currentProgram = pendingProgram;
		pendingProgram = null;
	}

	override public function onCompileEnd():Void {
		final context = requireContext();
		final program = currentProgram;
		currentProgram = null;
		if (program == null) {
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "typed-program capture was missing at compile end", Context.currentPos(), context.profile);
			return;
		}
		context.setTypedProgram(program);
		generatedFiles = new CCompiler(context).compileModules(program);
		CPhaseTiming.stop(targetTimer);
		targetTimer = null;
	}

	override public function generateFilesManually():Void {
		if (output == null) {
			GeneratedFileDigestCache.abortRequest();
			CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "Reflaxe output manager is not initialized", Context.currentPos());
			return;
		}

		outputTimer = CPhaseTiming.start(CPOutputOwnership);
		try {
			new ReflaxeOutputWriter().write(output, generatedFiles);
		} catch (error:ProjectEmissionError) {
			CPhaseTiming.stop(outputTimer);
			outputTimer = null;
			GeneratedFileDigestCache.abortRequest();
			CDiagnostic.fatal(error.diagnosticId, error.detail, Context.currentPos());
		}
	}

	/**
		Commit warm digest evidence only after Reflaxe finishes output ownership.

		`generateFilesManually` writes the planned artifacts, but Reflaxe still
		has to apply its stale-file policy and write ownership metadata afterward.
		`onOutputComplete` is the first hook after that whole
		transaction. A failure before this hook therefore leaves the previous
		successful cache generation untouched.
	**/
	override public function onOutputComplete():Void {
		CPhaseTiming.stop(outputTimer);
		outputTimer = null;
		GeneratedFileDigestCache.completeRequest(generatedFiles);
		CPhaseTiming.finishRequest();
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

	public function compileExpressionImpl(expr:TypedExpr, topLevel:Bool):Null<Bool> {
		return null;
	}

	function requireContext():CompilationContext {
		return switch compilationContext {
			case null:
				CDiagnostic.fatal(CDiagnosticId.InternalCompilerError, "missing per-build CompilationContext", Context.currentPos());
			case context:
				context;
		};
	}
}
#else
class CReflaxeCompiler {
	public function new() {}
}
#end
