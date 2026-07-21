package caxecraft.gameplay;

/**
 * Small, serializable state for one friendly guide in the first sandbox.
 *
 * Callers use `GuideNpc.phase`; `phaseCode` is the narrow integer carrier
 * required until haxe_c-49y admits nominal values inside direct C records.
 */
typedef GuideState = {
	final x:Float;
	final y:Float;
	final z:Float;
	final phaseCode:Int;
}
