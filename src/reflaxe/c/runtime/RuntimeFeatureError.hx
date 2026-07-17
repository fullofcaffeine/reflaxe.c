package reflaxe.c.runtime;

import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.ir.HxcSourceSpan;
import reflaxe.c.runtime.RuntimeFeatureModel.RuntimePolicyBlockerRecord;

/** Typed failure shared by registry validation and policy planning. */
class RuntimeFeatureError extends haxe.Exception {
	public final diagnosticId:CDiagnosticId;
	public final featureIds:Array<String>;
	public final blockers:Array<RuntimePolicyBlockerRecord>;
	public final primarySource:Null<HxcSourceSpan>;

	public function new(diagnosticId:CDiagnosticId, detail:String, ?featureIds:Array<String>, ?blockers:Array<RuntimePolicyBlockerRecord>,
			?primarySource:HxcSourceSpan) {
		super(detail);
		this.diagnosticId = diagnosticId;
		this.featureIds = featureIds == null ? [] : featureIds.copy();
		this.blockers = blockers == null ? [] : blockers.copy();
		this.primarySource = primarySource;
	}
}
