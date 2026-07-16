package reflaxe.c;

/** Optimization intent that may influence checked-lowering shape, never semantics. */
enum abstract CBuildMode(String) to String {
	var Debug = "debug";
	var Release = "release";
	var MinSizeRel = "minsizerel";
}
