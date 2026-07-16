package reflaxe.c;

/** Presentation policy for root runtime requirements; packaging is unaffected. */
enum abstract CRuntimeDiagnostics(String) to String {
	var Off = "off";
	var Summary = "summary";
	var Warn = "warn";
}
