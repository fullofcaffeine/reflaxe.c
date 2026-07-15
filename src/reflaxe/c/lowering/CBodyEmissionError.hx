package reflaxe.c.lowering;

import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/** An invariant failure after validated HxcIR reaches structural C lowering. */
class CBodyEmissionError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final detail:String;

	public function new(detail:String) {
		this.diagnosticId = CDiagnosticId.InternalCompilerError;
		this.detail = detail;
		super(CDiagnostic.codeMessage(diagnosticId, detail));
	}
}
