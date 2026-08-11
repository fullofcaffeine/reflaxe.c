package caxecraft.app;

/**
	Chooses a world-space shape for one validated stateful object.

	Content owns the object's bounds and quarter-turn yaw. This module converts
	those facts into oriented dimensions before the Raylib adapter draws them.
	It contains no campaign names, texture handles, or renderer state.
**/
/** The two generic 3D forms admitted for current stateful world objects. */
enum StatefulObjectVisualKind {
	/** A compact control pedestal for switches, notes, glyphs, and relays. */
	MechanismVisual;

	/** A bounds-sized box for gates, walls, and other large structures. */
	StructureVisual;
}

/** Oriented dimensions and generic form for one stateful world object. */
typedef StatefulObjectVisual = {
	/** Generic 3D form selected from the declared bounds. */
	final kind:StatefulObjectVisualKind;

	/** World-space width after the authored quarter turn. */
	final widthMilli:Int;

	/** World-space height, which yaw does not change. */
	final heightMilli:Int;

	/** World-space depth after the authored quarter turn. */
	final depthMilli:Int;
}

/**
	Project declared bounds into the object's world-space orientation.

	Small stateful objects become grounded 3D controls. A larger object keeps
	its full box. Yaw 90 and 270 swap width and depth so visual and collision
	bounds agree. The scenario validator owns the closed yaw vocabulary.
**/
function statefulObjectVisual(widthMilli:Int, heightMilli:Int, depthMilli:Int, yawDegrees:Int):StatefulObjectVisual {
	final kind = widthMilli > 1200 || heightMilli > 1200 || depthMilli > 1200 ? StructureVisual : MechanismVisual;
	final quarterTurn = yawDegrees == 90 || yawDegrees == 270;
	return {
		kind: kind,
		widthMilli: quarterTurn ? depthMilli : widthMilli,
		heightMilli: heightMilli,
		depthMilli: quarterTurn ? widthMilli : depthMilli
	};
}
