package c;

/** C declaration layout requested for an ordinary typed Haxe declaration. */
enum abstract Layout(String) to String {
	var Struct = "struct";
	var Union = "union";
	var Opaque = "opaque";
	var Enum = "enum";
}
