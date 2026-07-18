package hxc.config;

/** Explicit C extension family. Strict ISO C remains the default. */
enum abstract HxcCExtensionPolicy(String) to String {
	var None = "none";
	var Gnu = "gnu";
	var Msvc = "msvc";
}
