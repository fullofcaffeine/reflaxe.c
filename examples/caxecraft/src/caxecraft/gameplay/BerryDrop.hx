package caxecraft.gameplay;

import caxecraft.domain.Character;
import caxecraft.domain.Vitals.isDefeated;

/** Deterministic spawn and pickup rules for the first visible enemy drop. */
inline final PICKUP_DISTANCE_SQUARED:Float = 2.25;

inline function none():BerryDropState
	return make(0.0, 0.0, 0.0, 0, false);

/**
	Create one visible berry pickup from a committed defeated character.

	The actor controller event and selected content pack decide the item and
	quantity. This presentation helper only places that admitted quantity at the
	character body; it contains no Mossling identity or controller state.
**/
function fromDefeatedCharacter(character:Character, amount:Int):BerryDropState {
	if (!character.id.isValid() || !isDefeated(character.vitals) || amount <= 0)
		return none();
	return make(character.body.x, character.body.y + 0.24, character.body.z, amount, true);
}

function isInRange(state:BerryDropState, playerX:Float, playerY:Float, playerZ:Float):Bool {
	if (!state.active)
		return false;
	final dx = state.x - playerX;
	final dy = state.y - playerY;
	final dz = state.z - playerZ;
	return dx * dx + dy * dy + dz * dz <= PICKUP_DISTANCE_SQUARED;
}

/** Remove only the amount that actually entered the inventory. */
function collectAmount(state:BerryDropState, amount:Int):BerryDropState {
	if (!state.active || amount <= 0)
		return state;
	final remaining = state.amount - amount;
	if (remaining <= 0)
		return make(state.x, state.y, state.z, 0, false);
	return make(state.x, state.y, state.z, remaining, true);
}

private inline function make(x:Float, y:Float, z:Float, amount:Int, active:Bool):BerryDropState
	return {
		x: x,
		y: y,
		z: z,
		amount: amount,
		active: active
	};
