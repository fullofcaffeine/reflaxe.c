package reflaxe.c;

#if macro
import haxe.macro.Compiler;
import haxe.macro.Context;
#end

/** Detects the explicit C target without treating `-D c` as activation. */
class BuildDetection {
	#if macro
	public static function isCBuild():Bool {
		final output = Context.definedValue("c_output");
		if (output != null && output != "") {
			return true;
		}

		if (Context.definedValue("target.name") == "c") {
			return true;
		}

		#if (haxe >= version("5.0.0-preview.1"))
		final configuration = Compiler.getConfiguration();
		if (configuration != null) {
			switch (configuration.platform) {
				case CustomTarget("c"):
					return true;
				case _:
			}
		}
		#end

		return false;
	}
	#else
	public static function isCBuild():Bool {
		return false;
	}
	#end
}
