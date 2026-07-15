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
		final leftBytes = Bytes.ofString(left);
		final rightBytes = Bytes.ofString(right);
		final length = leftBytes.length < rightBytes.length ? leftBytes.length : rightBytes.length;
		for (index in 0...length) {
			final difference = leftBytes.get(index) - rightBytes.get(index);
			if (difference != 0) {
				return difference;
			}
		}
		return leftBytes.length - rightBytes.length;
	}
}
