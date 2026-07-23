package caxecraft.gameplay;

/**
	Pure proximity rules for items authored in a CaxeMap.

	The map keeps positions as exact integer milliblocks. This helper converts at
	the mechanic boundary and checks a small three-dimensional radius. It knows
	nothing about the collector role, item IDs, equipment, campaigns, rendering,
	or input devices. A player, NPC, companion, or scripted actor may therefore
	use the same rule.
**/
private inline final PICKUP_RADIUS_SQUARED:Float = 2.25;

/** True when the collector is close enough for automatic collection. */
function isInRange(collectorX:Float, collectorY:Float, collectorZ:Float, itemXMilli:Int, itemYMilli:Int, itemZMilli:Int):Bool {
	final dx = collectorX - itemXMilli / 1000.0;
	final dy = collectorY + 0.9 - itemYMilli / 1000.0;
	final dz = collectorZ - itemZMilli / 1000.0;
	return dx * dx + dy * dy + dz * dz <= PICKUP_RADIUS_SQUARED;
}
