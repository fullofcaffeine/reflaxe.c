package c;

/** Requested C declaration linkage. */
enum abstract Linkage(String) to String {
	var External = "external";
	var Internal = "internal";
	var Static = "static";
	var Inline = "inline";
}
