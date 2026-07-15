package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.Position;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.ir.HxcIRDiagnostic;

/** A typed, source-positioned failure at the typed-AST/HxcIR boundary. */
class CBodyLoweringError extends haxe.Exception {
	public final diagnostic:HxcIRDiagnostic;
	public final position:Position;

	public function new(diagnostic:HxcIRDiagnostic, position:Position) {
		this.diagnostic = diagnostic;
		this.position = position;
		super(diagnostic.render());
	}

	public var diagnosticId(get, never):CDiagnosticId;

	function get_diagnosticId():CDiagnosticId
		return diagnostic.id;

	public var detail(get, never):String;

	function get_detail():String
		return diagnostic.message;
}
#else
class CBodyLoweringError {}
#end
