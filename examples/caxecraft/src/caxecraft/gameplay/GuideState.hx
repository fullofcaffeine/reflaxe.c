package caxecraft.gameplay;

/**
 * Small value snapshot for one friendly guide in the first sandbox.
 *
 * The closed `GuidePhase` stays nominal in Haxe and becomes a native enum field
 * in generated C. A save or network format should convert it to a versioned
 * external code at that boundary instead of weakening the in-memory model.
 */
typedef GuideState = {
	final x:Float;
	final y:Float;
	final z:Float;
	final phase:GuidePhase;
}
