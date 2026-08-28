package hxc.cli;

#if macro
import haxe.macro.Context;
import haxe.macro.Expr;
#end

/** Compile-time package facts used by both Eval and the future native CLI. */
class HxcBuildInfo {
	/**
		Read the haxelib-provided package version define when it exists.

		Direct source checkout runs deliberately report `development`; this avoids a
		second hand-maintained copy of the version from `haxelib.json`.
	**/
	public static macro function version():ExprOf<String> {
		final value = Context.definedValue("reflaxe.c");
		final normalized = value == null || StringTools.trim(value) == "" ? "development" : value;
		return macro $v{normalized};
	}
}
