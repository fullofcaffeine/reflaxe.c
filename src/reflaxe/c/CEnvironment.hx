package reflaxe.c;

/** Resolved target environment; capability adapters remain separate facts. */
enum abstract CEnvironment(String) to String {
	var Hosted = "hosted";
	var Freestanding = "freestanding";
	var Wasi = "wasi";
	var Emscripten = "emscripten";
}
