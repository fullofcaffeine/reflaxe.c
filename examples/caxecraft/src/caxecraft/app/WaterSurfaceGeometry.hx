package caxecraft.app;

import caxecraft.domain.WaterCellState;
import caxecraft.domain.World;
import caxecraft.domain.WorldView;
import caxecraft.domain.WaterCellCodec.stateInView as waterStateAt;

/**
	Turns discrete water levels into connected presentation geometry.

	The simulation deliberately stores one exact level per voxel. Drawing each
	voxel as a separate flat slab would expose a translucent wall at every small
	level change, so this module gives all cells that meet at one corner the same
	averaged height. The renderer can then join neighboring tops without changing
	the authoritative water bytes, collision, breath, or flow behavior.
**/
typedef WaterSurfaceCorners = {
	/** Height at the cell's north-west corner. */
	final northWest:Float;

	/** Height at the cell's north-east corner. */
	final northEast:Float;

	/** Height at the cell's south-east corner. */
	final southEast:Float;

	/** Height at the cell's south-west corner. */
	final southWest:Float;
}

/**
	Find four joined heights for one exposed water top.

	Each corner samples the same four voxel positions regardless of which
	neighboring top asks for it. Shared vertices therefore receive identical
	heights and cannot open a crack or require an internal vertical wall.
**/
function surfaceCorners(cells:WorldView, x:Int, y:Int, z:Int):WaterSurfaceCorners {
	return {
		northWest: cornerHeight(y, waterStateAt(cells, World.coord(x, y, z)), waterStateAt(cells, World.coord(x - 1, y, z)),
			waterStateAt(cells, World.coord(x, y, z - 1)), waterStateAt(cells, World.coord(x - 1, y, z - 1))),
		northEast: cornerHeight(y, waterStateAt(cells, World.coord(x, y, z)), waterStateAt(cells, World.coord(x + 1, y, z)),
			waterStateAt(cells, World.coord(x, y, z - 1)), waterStateAt(cells, World.coord(x + 1, y, z - 1))),
		southEast: cornerHeight(y, waterStateAt(cells, World.coord(x, y, z)), waterStateAt(cells, World.coord(x + 1, y, z)),
			waterStateAt(cells, World.coord(x, y, z + 1)), waterStateAt(cells, World.coord(x + 1, y, z + 1))),
		southWest: cornerHeight(y, waterStateAt(cells, World.coord(x, y, z)), waterStateAt(cells, World.coord(x - 1, y, z)),
			waterStateAt(cells, World.coord(x, y, z + 1)), waterStateAt(cells, World.coord(x - 1, y, z + 1)))
	};
}

/**
	Average the water surfaces that meet at one grid corner.

	Dry, solid, out-of-world, and malformed cells contribute no height. At least
	one caller-owned water state is expected in normal rendering; returning the
	voxel floor for an all-dry input keeps malformed callers bounded and visible.
**/
function cornerHeight(y:Int, first:WaterCellState, second:WaterCellState, third:WaterCellState, fourth:WaterCellState):Float {
	var sum = 0.0;
	var count = 0;
	final firstHeight = admittedSurface(first, y);
	if (firstHeight >= 0.0) {
		sum += firstHeight;
		count++;
	}
	final secondHeight = admittedSurface(second, y);
	if (secondHeight >= 0.0) {
		sum += secondHeight;
		count++;
	}
	final thirdHeight = admittedSurface(third, y);
	if (thirdHeight >= 0.0) {
		sum += thirdHeight;
		count++;
	}
	final fourthHeight = admittedSurface(fourth, y);
	if (fourthHeight >= 0.0) {
		sum += fourthHeight;
		count++;
	}
	return count == 0 ? y : sum / count;
}

/** Draw a top only when the cell above is actual open air. */
function isOpenTop(state:WaterCellState):Bool {
	return switch state {
		case Empty: true;
		case Source | Flowing(_, _) | Blocked | InvalidStorage(_): false;
	};
}

/**
	Draw an outer side only when the neighboring voxel is empty.

	Another water cell owns a joined top instead of an internal translucent wall.
	Solid and malformed neighbors remain closed rather than leaking presentation.
**/
function sideIsExposed(state:WaterCellState):Bool {
	return switch state {
		case Empty: true;
		case Source | Flowing(_, _) | Blocked | InvalidStorage(_): false;
	};
}

/** Preserve the exact source/flow distinction while asking only about presence. */
function isWater(state:WaterCellState):Bool {
	return switch state {
		case Source | Flowing(_, _): true;
		case Empty | Blocked | InvalidStorage(_): false;
	};
}

/** Convert one compact simulation level into its exact voxel-space height. */
function surfaceTop(state:WaterCellState, y:Int):Float {
	return switch state {
		case Source: y + 1.0;
		case Flowing(level, falling):
			if (falling) y + 1.0; else {
				final distance:Int = level;
				y + (8.0 - distance) / 8.0;
			}
		case Empty | Blocked | InvalidStorage(_): y;
	};
}

/** Return a water height, or `-1` when this state contributes no surface. */
private function admittedSurface(state:WaterCellState, y:Int):Float {
	return switch state {
		case Source | Flowing(_, _): surfaceTop(state, y);
		case Empty | Blocked | InvalidStorage(_): -1.0;
	};
}
