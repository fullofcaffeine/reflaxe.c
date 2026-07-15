package reflaxe.c.ir;

/** Raised only when a caller asks the validator to require a valid program. */
class HxcIRValidationError extends haxe.Exception {
	public final diagnostics:Array<HxcIRDiagnostic>;

	public function new(diagnostics:Array<HxcIRDiagnostic>) {
		this.diagnostics = diagnostics.copy();
		super(this.diagnostics.map(diagnostic -> diagnostic.render()).join("\n"));
	}
}
