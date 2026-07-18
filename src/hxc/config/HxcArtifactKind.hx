package hxc.config;

/** Native artifact intent; the build adapter decides whether a tuple supports it. */
enum abstract HxcArtifactKind(String) to String {
	var Executable = "executable";
	var StaticLibrary = "static-library";
	var SharedLibrary = "shared-library";
}
