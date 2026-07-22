package caxecraft.app;

#if c
import caxecraft.domain.WaterCellCodec.stateAt as waterStateAt;
import caxecraft.domain.WaterCellState;
import caxecraft.domain.World;
import caxecraft.domain.WorldCells;
import raylib.Color;
import raylib.Rlgl;
import raylib.Texture2D;

/** Counts returned after the separate translucent water submission. */
typedef WaterRenderCounters = {
	final visible:Int;
	final drawCalls:Int;
}

/**
	Draw the current fluid state without advancing its simulation.

	Solid terrain is submitted first by `TerrainRenderer`; this second coherent
	quad batch draws only exposed water faces with vertex alpha. Neighboring water
	faces are omitted, and the top follows the exact eight-level simulation
	surface. This is intentionally a small first transparent pass rather than a
	per-block Raylib draw call or a renderer-owned fluid update.
**/
function draw(cells:WorldCells, texture:Texture2D, textureReady:Bool, presentationCell:Int):WaterRenderCounters {
	if (!textureReady || presentationCell < 0)
		return {visible: 0, drawCalls: 0};
	final column = presentationCell % TerrainAtlas.COLUMNS;
	final row = Std.int(presentationCell / TerrainAtlas.COLUMNS);
	final halfPixel = 0.5 / texture.width;
	final u0 = column / TerrainAtlas.COLUMNS + halfPixel;
	final u1 = (column + 1) / TerrainAtlas.COLUMNS - halfPixel;
	final v0 = row / TerrainAtlas.ROWS + halfPixel;
	final v1 = (row + 1) / TerrainAtlas.ROWS - halfPixel;
	var visible = 0;
	var faces = 0;
	Rlgl.BeginQuads(texture);
	var z = 0;
	while (z < World.DEPTH) {
		var y = 0;
		while (y < World.HEIGHT) {
			var x = 0;
			while (x < World.WIDTH) {
				final state = waterStateAt(cells, World.coord(x, y, z));
				if (isWater(state)) {
					final top = surfaceTop(state, y);
					var cellVisible = false;
					if (isOpenAir(waterStateAt(cells, World.coord(x, y + 1, z)))) {
						emitTop(x, top, z, u0, u1, v0, v1);
						faces++;
						cellVisible = true;
					}
					final northBottom = exposedSideBottom(waterStateAt(cells, World.coord(x, y, z - 1)), y, top);
					if (northBottom < top) {
						emitNorth(x, northBottom, top, z, u0, u1, v0, v1);
						faces++;
						cellVisible = true;
					}
					final southBottom = exposedSideBottom(waterStateAt(cells, World.coord(x, y, z + 1)), y, top);
					if (southBottom < top) {
						emitSouth(x, southBottom, top, z, u0, u1, v0, v1);
						faces++;
						cellVisible = true;
					}
					final eastBottom = exposedSideBottom(waterStateAt(cells, World.coord(x + 1, y, z)), y, top);
					if (eastBottom < top) {
						emitEast(x, eastBottom, top, z, u0, u1, v0, v1);
						faces++;
						cellVisible = true;
					}
					final westBottom = exposedSideBottom(waterStateAt(cells, World.coord(x - 1, y, z)), y, top);
					if (westBottom < top) {
						emitWest(x, westBottom, top, z, u0, u1, v0, v1);
						faces++;
						cellVisible = true;
					}
					if (cellVisible)
						visible++;
				}
				x++;
			}
			y++;
		}
		z++;
	}
	Rlgl.EndQuads();
	var drawCalls = 0;
	if (faces > 0)
		drawCalls = 1;
	return {visible: visible, drawCalls: drawCalls};
}

/** Draw a top only when the cell above is actual open air. */
private function isOpenAir(state:WaterCellState):Bool {
	return switch state {
		case Empty: true;
		case Source | Flowing(_, _) | Blocked | InvalidStorage(_): false;
	};
}

/**
	Find the visible bottom of one water side.

	Air exposes the complete side, lower neighboring water exposes only the step
	between surfaces, and opaque or malformed neighbors expose nothing. Returning
	`top` for the last case lets the caller use one clear `bottom < top` rule.
**/
private function exposedSideBottom(neighbor:WaterCellState, y:Int, top:Float):Float {
	return switch neighbor {
		case Empty: y;
		case Source: y + 1.0;
		case Flowing(_, _): surfaceTop(neighbor, y);
		case Blocked | InvalidStorage(_): top;
	};
}

/** Preserve the exact source/flow distinction while asking only about presence. */
private function isWater(state:WaterCellState):Bool {
	return switch state {
		case Source | Flowing(_, _): true;
		case Empty | Blocked | InvalidStorage(_): false;
	};
}

/** Convert the compact level into a continuous voxel-space top surface. */
private function surfaceTop(state:WaterCellState, y:Int):Float {
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

/** Select one calm translucent tint for the built-in fresh-water profile. */
private inline function tint():Color
	return Color.rgba(150, 226, 242, 178);

private function emitTop(x:Float, y:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(0.0, 1.0, 0.0);
	vertex(u0, v0, x, y, z);
	vertex(u0, v1, x, y, z + 1.0);
	vertex(u1, v1, x + 1.0, y, z + 1.0);
	vertex(u1, v0, x + 1.0, y, z);
}

private function emitNorth(x:Float, bottom:Float, top:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(0.0, 0.0, -1.0);
	vertex(u0, v1, x, bottom, z);
	vertex(u0, v0, x, top, z);
	vertex(u1, v0, x + 1.0, top, z);
	vertex(u1, v1, x + 1.0, bottom, z);
}

private function emitSouth(x:Float, bottom:Float, top:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(0.0, 0.0, 1.0);
	vertex(u0, v1, x, bottom, z + 1.0);
	vertex(u1, v1, x + 1.0, bottom, z + 1.0);
	vertex(u1, v0, x + 1.0, top, z + 1.0);
	vertex(u0, v0, x, top, z + 1.0);
}

private function emitEast(x:Float, bottom:Float, top:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(1.0, 0.0, 0.0);
	vertex(u0, v1, x + 1.0, bottom, z);
	vertex(u0, v0, x + 1.0, top, z);
	vertex(u1, v0, x + 1.0, top, z + 1.0);
	vertex(u1, v1, x + 1.0, bottom, z + 1.0);
}

private function emitWest(x:Float, bottom:Float, top:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(-1.0, 0.0, 0.0);
	vertex(u0, v1, x, bottom, z);
	vertex(u1, v1, x, bottom, z + 1.0);
	vertex(u1, v0, x, top, z + 1.0);
	vertex(u0, v0, x, top, z);
}

/** Submit one atlas vertex through the reviewed binary32 Rlgl facade. */
private function vertex(u:Float, v:Float, x:Float, y:Float, z:Float):Void {
	Rlgl.TexCoord(u, v);
	Rlgl.Vertex(x, y, z);
}
#end
