import haxe.Json;
import reflaxe.c.CDiagnostic;
import reflaxe.c.CDiagnostic.CDiagnosticId;
import reflaxe.c.CDiagnostic.CDiagnosticRelatedLocation;
import reflaxe.c.CDiagnostic.CDiagnosticSeverity;
import reflaxe.c.CDiagnostic.CDiagnosticSourceSpan;

class DiagnosticGolden {
	static inline final DEFINITIONS_PREFIX = "HXC_DIAGNOSTIC_DEFINITIONS=";
	static inline final EVENTS_PREFIX = "HXC_DIAGNOSTIC_EVENTS=";
	static inline final HUMAN_PREFIX = "HXC_DIAGNOSTIC_HUMAN=";

	static function main():Void {
		final source = new CDiagnosticSourceSpan("test/negative/Main.hx", 4, 3, 4, 12);
		final related = new CDiagnosticRelatedLocation("conflicting declaration", new CDiagnosticSourceSpan("test/negative/Support.hx", 8, 1, 8, 9));
		final unsupported = CDiagnostic.create(CDiagnosticId.UnsupportedExpression, "portable", "Unsupported typed node `TWhatever`.", source,
			CDiagnosticSeverity.Error, "function app.Main.main expression", ["The node was retained for inspection."], [related]);
		final internal = CDiagnostic.create(CDiagnosticId.InternalCompilerError, "metal", "Invalid HxcIR: block has no terminator.", source,
			CDiagnosticSeverity.Error, "module:app.Main.function:main.block:entry");
		final runtimeWarning = CDiagnostic.create(CDiagnosticId.RuntimeFeatureSelected, "portable", "Feature `string.utf8` was selected.", source,
			CDiagnosticSeverity.Warning, "runtime root");

		var rejectedSeverity = false;
		try {
			CDiagnostic.create(CDiagnosticId.UnsupportedExpression, "portable", "invalid severity", source, CDiagnosticSeverity.Warning);
		} catch (_:haxe.Exception) {
			rejectedSeverity = true;
		}
		if (!rejectedSeverity) {
			throw "unsupported-source diagnostics accepted warning severity";
		}

		var rejectedPath = false;
		try {
			new CDiagnosticSourceSpan("/host/private/Main.hx", 1, 1, 1, 2);
		} catch (_:haxe.Exception) {
			rejectedPath = true;
		}
		if (!rejectedPath) {
			throw "diagnostic record accepted an absolute source path";
		}

		var rejectedId = false;
		try {
			CDiagnostic.definition(cast("HXC" + "1234"));
		} catch (_:haxe.Exception) {
			rejectedId = true;
		}
		if (!rejectedId) {
			throw "diagnostic registry accepted an unregistered ID";
		}

		Sys.println(DEFINITIONS_PREFIX + Json.stringify(CDiagnostic.catalogSnapshot()));
		Sys.println(EVENTS_PREFIX + Json.stringify([unsupported.toJsonValue(), internal.toJsonValue(), runtimeWarning.toJsonValue()]));
		Sys.println(HUMAN_PREFIX + CDiagnostic.humanMessage(CDiagnosticId.UnsupportedExpression, "Unsupported typed node.", "portable"));
		Sys.println("diagnostics-haxe: OK");
	}
}
