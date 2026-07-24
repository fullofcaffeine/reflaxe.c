package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Type.ClassType;
import haxe.macro.Type.Ref;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRecognition;
import reflaxe.c.lowering.CBodyBytes.CBodyBytesRecognition;
import reflaxe.c.lowering.CBodyDispatch.CBodyInstanceCallAccess;
import reflaxe.c.lowering.CBodyStringMap.CBodyStringMapRecognition;

/**
	Names the compiler-owned receiver families that bypass ordinary class dispatch.

	These values are source-language identities, not C representations. Reachability
	and body lowering share this closed classification so a standard-library call
	cannot be skipped by one phase and mistaken for a virtual Haxe method by the
	other.
**/
enum CBodyIntrinsicReceiverFamily {
	CBIRArray;
	CBIRStringMap;
	CBIRBytes;
	CBIRString;
	CBIROrdinaryClass;
}

/**
	Classifies one typed instance access before either reachability or lowering.

	Array, StringMap, Bytes, and String look like classes in Haxe's typed tree, but
	haxe.c gives each one a target-owned value model and a named lowering owner.
	An unrecognized method on one of those families still belongs to that owner,
	which can reject it precisely; it must not fall through to virtual dispatch.
**/
class CBodyIntrinsicReceiver {
	private function new() {}

	/** Return the one exact compiler-owned family for this typed receiver. */
	public static function classify(access:CBodyInstanceCallAccess):CBodyIntrinsicReceiverFamily {
		if (CBodyArrayRecognition.isCoreArray(access.owner))
			return CBIRArray;
		if (CBodyStringMapRecognition.isStringMap(access.owner))
			return CBIRStringMap;
		if (CBodyBytesRecognition.isCoreBytes(access.owner))
			return CBIRBytes;
		if (isCoreString(access.owner))
			return CBIRString;
		return CBIROrdinaryClass;
	}

	/** Recognize only Haxe's root `String`, never a user class with the same name. */
	public static function isCoreString(reference:Ref<ClassType>):Bool {
		final value = reference.get();
		return value.pack.length == 0 && value.name == "String";
	}
}
#else
class CBodyIntrinsicReceiver {
	private function new() {}
}
#end
