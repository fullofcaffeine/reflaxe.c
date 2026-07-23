package caxecraft.gameplay;

/**
 * Fixed-step, renderer-independent state for one Mossling.
 *
 * The closed `MosslingMode` is stored directly. haxe.c emits it as a native C
 * enum field, preserving the readable Haxe model without runtime conversion.
 * `phaseTicks` is nonzero only while an attack is winding up or recovering.
 */
typedef MosslingState = {
	final x:Float;
	final y:Float;
	final z:Float;
	final homeX:Float;
	final homeZ:Float;
	final mode:MosslingMode;
	final phaseTicks:Int;
	final health:Int;
}
