package reflaxe.c.emit;

import haxe.io.Bytes;
import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/** Deterministic failure at the generated-project ownership boundary. */
class ProjectEmissionError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final detail:String;
	public final paths:Array<String>;

	public function new(detail:String, ?paths:Array<String>) {
		this.diagnosticId = CDiagnosticId.InternalCompilerError;
		this.detail = detail;
		this.paths = paths == null ? [] : paths.copy();
		this.paths.sort(compareUtf8);
		super(CDiagnostic.codeMessage(diagnosticId, detail));
	}

	static function compareUtf8(left:String, right:String):Int {
		return reflaxe.c.CUtf8Order.compare(left, right);
	}
}
