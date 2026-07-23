package caxecraft.domain;

/**
	Immutable renderer-neutral snapshot of the committed session state.

	Presentation and telemetry receive values, not the mutable `GameSession` or
	its backing stores. The first slice contains only the local character and the
	deterministic clock. World, actor, heads-up-display, and event views will be
	added only with the focused consumers and lifetime rules that justify them.

	A record is clearer than a class here because the view has no identity or
	mutable lifetime: each call captures one point in simulation time. Haxe.c
	lowers it to a direct C struct value without allocation.
**/
typedef GameView = {
	/** False when no valid local character is bound to this session. */
	final valid:Bool;

	/** Committed character value; invalid storage sentinel when `valid` is false. */
	final localPlayer:Character;

	/** Number of fixed simulation steps committed before this snapshot. */
	final completedTicks:Int;
}
