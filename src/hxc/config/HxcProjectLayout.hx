package hxc.config;

/** Closed generated-C file layout selected before compiler-owned output is written. */
enum abstract HxcProjectLayout(String) to String {
	/** Private module headers/sources mirroring normalized Haxe ownership. */
	var Split = "split";

	/** One private header/source pair per normalized Haxe package. */
	var Package = "package";

	/** One ordinary implementation source plus its private umbrella header. */
	var Unity = "unity";
}
