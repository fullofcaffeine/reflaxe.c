#if macro
import haxe.macro.Compiler;

/** Test-only stand-in for compiler-owned target and capability facts. */
class TargetPrelude {
	public static function define(name:String, ?value:String):Void {
		Compiler.define(name, value);
	}
}
#end
