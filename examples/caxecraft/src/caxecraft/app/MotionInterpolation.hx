package caxecraft.app;

import caxecraft.domain.CharacterBody;

/**
	One read-only position used to draw a character between simulation ticks.

	Physics still owns the authoritative `CharacterBody`, including velocity and
	contact state. Presentation needs only position, so this smaller value cannot
	accidentally feed a smoothed velocity or grounded flag back into gameplay.
	The compiler lowers it to a direct C struct value without allocation.
**/
typedef RenderPosition = {
	final x:Float;
	final y:Float;
	final z:Float;
}

/**
	The last two committed positions needed for smooth presentation.

	A fixed-step game may render several frames between two simulation updates.
	Keeping both endpoints lets each render frame move gradually from `previous`
	to `current`, while collision, saves, and interactions continue to use the
	unchanged committed character. This history is a value rather than a mutable
	class because the application replaces it at explicit tick/reset boundaries.
**/
typedef MotionHistory = {
	final previous:RenderPosition;
	final current:RenderPosition;
}

/** Start presentation at one committed body, with no older motion to blend. */
function start(body:CharacterBody):MotionHistory {
	final position = fromBody(body);
	return {previous: position, current: position};
}

/**
	Advance the two-position window after one successful fixed simulation tick.

	Calling this once per committed tick also handles slow display frames that
	require several simulation updates: only the final adjacent pair remains.
**/
function advance(history:MotionHistory, body:CharacterBody):MotionHistory
	return {previous: history.current, current: fromBody(body)};

/**
	Discard old motion after a discontinuity such as spawn recovery or teleport.

	Both endpoints become the same committed position, so the next rendered frame
	cannot sweep visibly through walls or across a paused/loading transition.
**/
inline function reset(body:CharacterBody):MotionHistory
	return start(body);

/**
	Sample a visual position from the unconsumed fraction of the fixed clock.

	`remainderSeconds / fixedSeconds` is the blend fraction. It is clamped because
	a renderer must remain safe if timing input is early, late, or being reset.
	A non-positive fixed duration is invalid scheduling input; returning `current`
	is the safest presentation fallback and never changes simulation state.
**/
function sample(history:MotionHistory, remainderSeconds:Float, fixedSeconds:Float):RenderPosition {
	if (fixedSeconds <= 0.0)
		return history.current;
	var fraction = remainderSeconds / fixedSeconds;
	if (fraction < 0.0)
		fraction = 0.0;
	if (fraction > 1.0)
		fraction = 1.0;
	return {
		x: mix(history.previous.x, history.current.x, fraction),
		y: mix(history.previous.y, history.current.y, fraction),
		z: mix(history.previous.z, history.current.z, fraction)
	};
}

/** Copy only the presentation-owned position fields from committed gameplay. */
inline function fromBody(body:CharacterBody):RenderPosition
	return {x: body.x, y: body.y, z: body.z};

/** Linear interpolation written without target-specific math or allocation. */
inline function mix(start:Float, end:Float, fraction:Float):Float
	return start + (end - start) * fraction;
