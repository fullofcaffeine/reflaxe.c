package reflaxe.c;

/** Source-semantics profile; runtime selection remains an independent axis. */
enum abstract CProfile(String) to String {
	var Portable = "portable";
	var Metal = "metal";
}
