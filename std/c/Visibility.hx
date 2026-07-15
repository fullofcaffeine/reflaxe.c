package c;

/** Native symbol visibility contract. Toolchain-specific support is validated later. */
enum abstract Visibility(String) to String {
	var Default = "default";
	var Hidden = "hidden";
}
