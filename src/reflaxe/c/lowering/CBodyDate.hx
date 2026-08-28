package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Type;
import haxe.macro.TypeTools;

/**
	Recognizes the exact root `Date` type owned by the Haxe standard library.

	Date is nominal and stores its canonical timestamp in the target-owned Haxe
	class. The compiler keeps ordinary object identity and intervenes only for
	fresh factory allocation and hosted services. Central recognition prevents a
	user class named Date in another package from receiving those privileges.
**/
class CBodyDateRecognition {
	private function new() {}

	/** Return true only for the root standard-library Date declaration. */
	public static function isCoreDate(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.length == 0 && value.name == "Date";
	}

	/** Return true only when a type resolves to the root Date without parameters. */
	public static function isCoreDateType(type:Type):Bool
		return switch TypeTools.follow(type) {
			case TInst(reference, parameters): parameters.length == 0 && isCoreDate(reference);
			case _: false;
		};

	/** Recognize only the standard `haxe.Timer` declaration. */
	public static function isCoreTimer(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.length == 1 && value.pack[0] == "haxe" && value.name == "Timer";
	}

	/** Recognize only the C target's private hosted Date service owner. */
	public static function isDateHost(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return switch value.kind {
			case KAbstractImpl(abstractReference): final owner = abstractReference.get(); owner.pack.length == 2 && owner.pack[0] == "c" && owner.pack[1] == "internal" && owner.name == "DateHost";
			case _: value.pack.length == 2 && value.pack[0] == "c" && value.pack[1] == "internal" && value.name == "DateHost";
		};
	}
}
#else
class CBodyDateRecognition {
	private function new() {}
}
#end
