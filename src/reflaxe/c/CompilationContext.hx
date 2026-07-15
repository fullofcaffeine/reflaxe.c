package reflaxe.c;

#if (macro || reflaxe_runtime)
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.frontend.TypedProgramInput;
#end
import reflaxe.c.naming.CSymbolRegistry;

/** Mutable compiler state has one instance per Haxe compilation request. */
class CompilationContext {
	public final profile:CProfile;
	public final symbols:CSymbolRegistry;

	#if (macro || reflaxe_runtime)
	public var typedProgram(default, null):Null<TypedProgramInput> = null;
	#end

	public function new(profile:CProfile) {
		this.profile = profile;
		this.symbols = new CSymbolRegistry();
	}

	#if (macro || reflaxe_runtime)
	public function setTypedProgram(program:TypedProgramInput):Void {
		if (typedProgram != null) {
			throw CDiagnostic.codeMessage(CDiagnosticId.InternalCompilerError, "CompilationContext already owns a normalized typed program");
		}
		typedProgram = program;
	}
	#end
}
