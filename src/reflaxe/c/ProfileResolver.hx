package reflaxe.c;

#if macro
import haxe.macro.Context;
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
				Context.fatalError('HXC0003: invalid reflaxe.c profile `$value`; expected portable or metal.', Context.currentPos());
		};
		#else
		return Portable;
		#end
	}
}
