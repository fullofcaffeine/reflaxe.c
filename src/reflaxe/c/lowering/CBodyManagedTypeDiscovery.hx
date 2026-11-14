package reflaxe.c.lowering;

#if (macro || reflaxe_runtime)
import haxe.macro.Type;
import reflaxe.c.lowering.CBodyArray.CBodyArrayRecognition;
import reflaxe.c.lowering.CBodyDate.CBodyDateRecognition;
import reflaxe.c.lowering.CBodyTypedMap.CBodyTypedMapFamily;
import reflaxe.c.lowering.CBodyTypedMap.CBodyTypedMapRecognition;

/**
	Share one compiler declaration read among managed-type discovery questions.

	Haxe's Eval compiler rebuilds class metadata on each Ref.get(). The discovery
	walk asks whether one value is an Array, a typed map, or a Date. For a direct
	class with no type parameters, those answers need only one declaration read.
	This module bounds that sharing; it keeps no cache or compiler object alive.
**/
/** Immutable answers for one direct type; no source layout or lifetime is changed. */
typedef DirectManagedTypeFacts = {
	/** Whether the declaration is the root Array class. */
	final array:Bool;

	/** Concrete object/enum map policy, or null for other declarations. */
	final mapFamily:Null<CBodyTypedMapFamily>;

	/** Whether the declaration is the root Date class. */
	final date:Bool;
}

/**
	Read one parameterless Eval class once, or defer to the original recognizers.

	The first Ref.get() still runs, preserving class construction and its errors.
	Name-based recognition then uses the same declaration. A parameterless IMap
	has no key argument, so only concrete map identity can apply here. Other
	shapes return null without forcing a lazy type, resolving a monomorph, or
	following type arguments. Their original recognizers retain that work and
	its order relative to representation discovery. The record carries only the
	three answers; a retained ClassType or a request-wide memo is unnecessary.
**/
function directManagedTypeFacts(type:Type):Null<DirectManagedTypeFacts> {
	#if eval
	switch type {
		case TInst(reference, parameters) if (parameters.length == 0):
			final value = reference.get();
			return {
				array: CBodyArrayRecognition.isCoreArrayClass(value),
				mapFamily: CBodyTypedMapRecognition.familyForDeclaration(value),
				date: CBodyDateRecognition.isCoreDateClass(value)
			};
		case _:
	}
	#end
	return null;
}
#end
