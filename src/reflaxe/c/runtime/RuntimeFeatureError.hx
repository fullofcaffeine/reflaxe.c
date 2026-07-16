package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;

/** Typed failure shared by registry validation and policy planning. */
class RuntimeFeatureError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final featureIds:Array<String>;

	public function new(diagnosticId:CDiagnosticId, detail:String, ?featureIds:Array<String>) {
		super(detail);
		this.diagnosticId = diagnosticId;
		this.featureIds = featureIds == null ? [] : featureIds.copy();
	}
}
