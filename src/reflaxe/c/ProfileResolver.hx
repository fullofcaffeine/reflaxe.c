package reflaxe.c;

#if macro
import haxe.macro.Context;
import reflaxe.c.CDiagnostic.CDiagnosticId;
#end

/** Resolves the early profile seed without selecting any runtime feature. */
class ProfileResolver {
	public static function resolve():CProfile {
		#if macro
		final raw = Context.definedValue("reflaxe_c_profile");
		return switch (raw) {
			case null | "" | "portable":
				Portable;
			case "metal":
				Metal;
			case value:
				CDiagnostic.fatal(CDiagnosticId.InvalidConfiguration, 'invalid reflaxe.c profile `$value`; expected portable or metal.', Context.currentPos(),
					"unresolved");
		};
		#else
		return Portable;
		#end
	}
}
