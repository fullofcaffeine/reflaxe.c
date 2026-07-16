package reflaxe.c;

/** Runtime fallback policy, resolved only after compiler-owned representations. */
enum abstract CRuntimePolicy(String) to String {
	var Auto = "auto";
	var Minimal = "minimal";
	var None = "none";
}
