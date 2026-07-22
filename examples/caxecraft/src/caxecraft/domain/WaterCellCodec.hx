package caxecraft.domain;

/**
	The one owner of compact water bytes in the shared world buffer.

	Most code sees `WaterCellState`, never numbers. `WaterSimulation` uses the
	encoding functions while player physics, rendering, saves, and editor checks
	may use the read-only `stateAt` view without borrowing the mutable scheduler
	object. Malformed bytes remain explicit `InvalidStorage` values.
**/
private inline final AIR_CODE:Int = 0;

private inline final FIRST_SOLID_CODE:Int = 1;
private inline final LAST_SOLID_CODE:Int = 4;
private inline final SOURCE_CODE:Int = 16;
private inline final FLOWING_BASE_CODE:Int = 17;
private inline final FALLING_BASE_CODE:Int = 25;
inline final FLOW_LEVEL_COUNT:Int = 8;

/** Read one coordinate through the complete typed water/storage contract. */
function stateAt(cells:WorldCells, coord:BlockCoord):WaterCellState {
	if (!World.contains(coord))
		return Blocked;
	return decode(WorldStorage.readCode(cells, World.indexOf(coord)));
}

/** Decode every byte explicitly so malformed storage cannot masquerade as air. */
function decode(code:Int):WaterCellState {
	if (code == AIR_CODE)
		return Empty;
	if (code >= FIRST_SOLID_CODE && code <= LAST_SOLID_CODE)
		return Blocked;
	if (code == SOURCE_CODE)
		return Source;
	if (code >= FLOWING_BASE_CODE && code < FLOWING_BASE_CODE + FLOW_LEVEL_COUNT)
		return Flowing(level(code - FLOWING_BASE_CODE), false);
	if (code >= FALLING_BASE_CODE && code < FALLING_BASE_CODE + FLOW_LEVEL_COUNT)
		return Flowing(level(code - FALLING_BASE_CODE), true);
	return InvalidStorage(code);
}

/** Byte used when simulation removes unsupported water. */
inline function emptyCode():Int
	return AIR_CODE;

/** Byte used for one authored permanent source. */
inline function sourceCode():Int
	return SOURCE_CODE;

/** Recognize only the compact source and flowing ranges owned by this module. */
inline function isWaterCode(code:Int):Bool
	return code == SOURCE_CODE || (code >= FLOWING_BASE_CODE && code < FALLING_BASE_CODE + FLOW_LEVEL_COUNT);

/** Recognize the ordinary terrain codes without confusing them with water. */
inline function isSolidCode(code:Int):Bool
	return code >= FIRST_SOLID_CODE && code <= LAST_SOLID_CODE;

/** Admit exactly the byte meanings understood by the shared world buffer. */
inline function isValidCode(code:Int):Bool
	return code == AIR_CODE || isSolidCode(code) || isWaterCode(code);

/** Encode one already-validated flow level in the compact world byte. */
function encodeFlow(level:Int, falling:Bool):Int {
	return (falling ? FALLING_BASE_CODE : FLOWING_BASE_CODE) + level;
}

/** Convert a proven 0...7 representation into the closed public level type. */
private function level(value:Int):WaterLevel {
	return switch value {
		case 0: Full;
		case 1: One;
		case 2: Two;
		case 3: Three;
		case 4: Four;
		case 5: Five;
		case 6: Six;
		default: Thin;
	};
}
