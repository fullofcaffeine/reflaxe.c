package reflaxe.c;

#if (macro || reflaxe_runtime)
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.frontend.TypedProgramInput;
import reflaxe.c.plan.CStaticInitializationModel.CStaticInitializationSnapshot;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimeFeaturePlanSnapshot;
#end
import reflaxe.c.naming.CSymbolRegistry;

/** Mutable compiler state has one instance per Haxe compilation request. */
class CompilationContext {
	public final profile:CProfile;
	public final buildMode:CBuildMode;
	public final symbols:CSymbolRegistry;

	#if (macro || reflaxe_runtime)
	public var typedProgram(default, null):Null<TypedProgramInput> = null;
	public var staticInitialization(default, null):Null<CStaticInitializationSnapshot> = null;
	public var runtimePlan(default, null):Null<RuntimeFeaturePlanSnapshot> = null;
	#end

	public function new(profile:CProfile, buildMode:CBuildMode = Debug) {
		this.profile = profile;
		this.buildMode = buildMode;
		this.symbols = new CSymbolRegistry();
	}

	#if (macro || reflaxe_runtime)
	public function setTypedProgram(program:TypedProgramInput):Void {
		if (typedProgram != null) {
			throw CDiagnostic.codeMessage(CDiagnosticId.InternalCompilerError, "CompilationContext already owns a normalized typed program");
		}
		typedProgram = program;
	}

	public function setStaticInitialization(snapshot:CStaticInitializationSnapshot):Void {
		if (staticInitialization != null) {
			throw CDiagnostic.codeMessage(CDiagnosticId.InternalCompilerError, "CompilationContext already owns a static-initialization plan");
		}
		staticInitialization = snapshot;
	}

	public function setRuntimePlan(snapshot:RuntimeFeaturePlanSnapshot):Void {
		if (runtimePlan != null) {
			throw CDiagnostic.codeMessage(CDiagnosticId.InternalCompilerError, "CompilationContext already owns a runtime feature plan");
		}
		runtimePlan = snapshot;
	}
	#end
}
