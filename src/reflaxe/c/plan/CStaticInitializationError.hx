package reflaxe.c.plan;

#if (macro || reflaxe_runtime)
import haxe.macro.Expr.Position;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/** A source-positioned, user-authored initialization graph failure. */
class CStaticInitializationError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final detail:String;
	public final position:Position;

	public function new(detail:String, position:Position) {
		super(CDiagnostic.codeMessage(CDiagnosticId.StaticInitializationCycle, detail));
		this.diagnosticId = CDiagnosticId.StaticInitializationCycle;
		this.detail = detail;
		this.position = position;
	}
}
#else
class CStaticInitializationError extends haxe.Exception {}
#end
