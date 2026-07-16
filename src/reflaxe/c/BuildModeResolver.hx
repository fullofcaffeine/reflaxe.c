package reflaxe.c;

#if macro
import haxe.macro.Context;
import reflaxe.c.CDiagnostic.CDiagnosticId;
#end

/** Resolves the build-mode seed without weakening source-level safety. */
class BuildModeResolver {
	public static function resolve(profile:CProfile):CBuildMode {
		#if macro
		return switch Context.definedValue("hxc_build") {
			case null | "" | "debug": Debug;
			case "release": Release;
			case "minsizerel": MinSizeRel;
			case value:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'invalid hxc_build `$value`; expected debug, release, or minsizerel.',
					Context.currentPos(), Std.string(profile));
		};
		#else
		return Debug;
		#end
	}
}
