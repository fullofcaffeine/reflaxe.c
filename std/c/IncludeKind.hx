package c;

/** Delimiter policy for an explicit external include build fact. */
enum abstract IncludeKind(String) to String {
	var System = "system";
	var Local = "local";
}
