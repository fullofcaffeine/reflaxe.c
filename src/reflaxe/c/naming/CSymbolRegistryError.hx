package reflaxe.c.naming;

import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/** Stable, inspectable name-planning failure raised before C AST construction. */
class CSymbolRegistryError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final sourceSymbols:Array<String>;

	public function new(diagnosticId:CDiagnosticId, detail:String, ?sourceSymbols:Array<String>) {
		this.diagnosticId = diagnosticId;
		this.sourceSymbols = [];
		if (sourceSymbols != null) {
			for (source in sourceSymbols) {
				if (this.sourceSymbols.indexOf(source) == -1) {
					this.sourceSymbols.push(source);
				}
			}
		}
		this.sourceSymbols.sort(compareStrings);
		super(CDiagnostic.codeMessage(diagnosticId, detail));
	}

	static function compareStrings(left:String, right:String):Int
		return left < right ? -1 : (left > right ? 1 : 0);
}
