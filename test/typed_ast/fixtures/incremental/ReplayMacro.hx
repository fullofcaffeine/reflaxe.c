#if macro
import haxe.macro.Context;
import haxe.macro.Expr;
import sys.io.File;
#end

/**
	Reads one declared external input while Haxe types the incremental fixture.

	Macros can observe files that are not Haxe modules. `registerModuleDependency`
	tells Haxe's compilation server that changing this text file must invalidate
	the caller. The backend catalog then proves the rebuilt typed constant cannot
	reuse the old semantic function.
**/
class ReplayMacro {
	/**
		Return the integer stored in `replay-macro-value.txt` as typed Haxe syntax.

		The file is read only by the macro host; no file API reaches generated C.
		Malformed content stops typing rather than silently choosing a default.
	**/
	public static macro function value():ExprOf<Int> {
		final path = Context.resolvePath("replay-macro-value.txt");
		Context.registerModuleDependency(Context.getLocalModule(), path);
		final parsed = Std.parseInt(StringTools.trim(File.getContent(path)));
		if (parsed == null)
			Context.error("replay-macro-value.txt must contain one integer", Context.currentPos());
		return macro $v{parsed};
	}
}
