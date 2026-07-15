package reflaxe.c.plan;

import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;

/** Stable, inspectable failure raised before any C file can be emitted. */
class CDeclarationPlanningError extends haxe.Exception {
	public static inline final DIAGNOSTIC_ID:CDiagnosticId = CDiagnosticId.InvalidTypedCContract;

	public final modulePaths:Array<String>;

	public function new(message:String, ?modulePaths:Array<String>) {
		this.modulePaths = modulePaths == null ? [] : modulePaths.copy();
		super(CDiagnostic.codeMessage(DIAGNOSTIC_ID, message));
	}
}
