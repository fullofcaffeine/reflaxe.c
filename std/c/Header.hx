package c;

/** Visibility of a compiler-owned generated header group. */
enum abstract Header(String) to String {
	var Public = "public";
	var Private = "private";
}
