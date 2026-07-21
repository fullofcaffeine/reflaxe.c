package caxecraft.gameplay;

/**
 * Fixed-step, renderer-independent state for one Mossling.
 *
 * `Mossling.mode` is the typed API. The explicit carrier is temporary evidence
 * for the same haxe_c-49y nominal-record compiler gap as `GuideState`.
 */
typedef MosslingState = {
	final x:Float;
	final y:Float;
	final z:Float;
	final homeX:Float;
	final homeZ:Float;
	final modeCode:Int;
	final health:Int;
}
