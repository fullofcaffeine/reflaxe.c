package reflaxe.c;

#if macro
import haxe.io.Path;
import haxe.macro.Compiler;
import haxe.macro.Context;
import sys.FileSystem;
#end

/**
	Resolves the pinned Reflaxe framework before target initialization.

	Standard-library override precedence must be declared in the initial library
	HXML; this macro never attempts to repair it after typing has begun.
**/
class CompilerBootstrap {
	public static inline final READY_DEFINE = "reflaxe_c_bootstrap_ready";
	public static inline final COUNT_DEFINE = "reflaxe_c_bootstrap_count";

	#if macro
	static var bootstrapped = false;

	public static function Start():Void {
		if (!BuildDetection.isCBuild() || bootstrapped) {
			return;
		}
		if (!canResolveReflaxe()) {
			final root = findLibraryRoot();
			final vendoredSource = Path.normalize(Path.join([root, "vendor", "reflaxe", "src"]));
			if (FileSystem.exists(vendoredSource) && FileSystem.isDirectory(vendoredSource)) {
				Compiler.addClassPath(vendoredSource);
			}
		}

		if (!canResolveReflaxe()) {
			Context.fatalError("HXC0002: reflaxe.c could not resolve its pinned Reflaxe framework; run `npm ci` and use the checked-in scoped library configuration.",
				Context.currentPos());
		}

		bootstrapped = true;
		Compiler.define(COUNT_DEFINE, "1");
		Compiler.define(READY_DEFINE);
	}

	static function canResolveReflaxe():Bool {
		try {
			Context.resolvePath("reflaxe/ReflectCompiler.hx");
			return true;
		} catch (_:Dynamic) {
			return false;
		}
	}

	static function findLibraryRoot():String {
		final thisFile = Context.resolvePath("reflaxe/c/CompilerBootstrap.hx");
		final packageDirectory = Path.directory(thisFile);
		return Path.normalize(Path.join([packageDirectory, "..", "..", ".."]));
	}
	#else
	public static function Start():Void {}
	#end
}
