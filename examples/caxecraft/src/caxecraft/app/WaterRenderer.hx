package caxecraft.app;

#if c
import caxecraft.domain.WaterCellCodec.stateInView as waterStateAt;
import caxecraft.domain.World;
import caxecraft.domain.WorldView;
import caxecraft.app.WaterSurfaceGeometry.isOpenTop;
import caxecraft.app.WaterSurfaceGeometry.isWater;
import caxecraft.app.WaterSurfaceGeometry.sideIsExposed;
import caxecraft.app.WaterSurfaceGeometry.surfaceCorners;
import caxecraft.app.WaterSurfaceGeometry.WaterSurfaceCorners;
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
	faces are omitted, and cells that meet at one corner share an averaged visual
	height instead of exposing a translucent wall at each one-eighth simulation
	step. The exact discrete state remains in `GameSession`; this is intentionally
	a small transparent pass rather than a per-block Raylib draw call or a
	renderer-owned fluid update.
**/
function draw(cells:WorldView, texture:Texture2D, textureReady:Bool, presentationCell:Int):WaterRenderCounters {
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
					final topOpen = isOpenTop(waterStateAt(cells, World.coord(x, y + 1, z)));
					final northOpen = sideIsExposed(waterStateAt(cells, World.coord(x, y, z - 1)));
					final southOpen = sideIsExposed(waterStateAt(cells, World.coord(x, y, z + 1)));
					final eastOpen = sideIsExposed(waterStateAt(cells, World.coord(x + 1, y, z)));
					final westOpen = sideIsExposed(waterStateAt(cells, World.coord(x - 1, y, z)));
					if (topOpen || northOpen || southOpen || eastOpen || westOpen) {
						// Corner sampling is the more detailed part of this pass. Hidden
						// interior water needs neither geometry nor those extra reads.
						final corners = surfaceCorners(cells, x, y, z);
						if (topOpen) {
							emitTop(x, z, corners, u0, u1, v0, v1);
							faces++;
						}
						if (northOpen) {
							emitNorth(x, y, corners.northWest, corners.northEast, z, u0, u1, v0, v1);
							faces++;
						}
						if (southOpen) {
							emitSouth(x, y, corners.southWest, corners.southEast, z, u0, u1, v0, v1);
							faces++;
						}
						if (eastOpen) {
							emitEast(x, y, corners.northEast, corners.southEast, z, u0, u1, v0, v1);
							faces++;
						}
						if (westOpen) {
							emitWest(x, y, corners.northWest, corners.southWest, z, u0, u1, v0, v1);
							faces++;
						}
						visible++;
					}
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

/** Select one calm translucent tint for the built-in fresh-water profile. */
private inline function tint():Color
	return Color.rgba(150, 226, 242, 178);

private function emitTop(x:Float, z:Float, corners:WaterSurfaceCorners, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(0.0, 1.0, 0.0);
	vertex(u0, v0, x, corners.northWest, z);
	vertex(u0, v1, x, corners.southWest, z + 1.0);
	vertex(u1, v1, x + 1.0, corners.southEast, z + 1.0);
	vertex(u1, v0, x + 1.0, corners.northEast, z);
}

private function emitNorth(x:Float, bottom:Float, topWest:Float, topEast:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(0.0, 0.0, -1.0);
	vertex(u0, v1, x, bottom, z);
	vertex(u0, v0, x, topWest, z);
	vertex(u1, v0, x + 1.0, topEast, z);
	vertex(u1, v1, x + 1.0, bottom, z);
}

private function emitSouth(x:Float, bottom:Float, topWest:Float, topEast:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(0.0, 0.0, 1.0);
	vertex(u0, v1, x, bottom, z + 1.0);
	vertex(u1, v1, x + 1.0, bottom, z + 1.0);
	vertex(u1, v0, x + 1.0, topEast, z + 1.0);
	vertex(u0, v0, x, topWest, z + 1.0);
}

private function emitEast(x:Float, bottom:Float, topNorth:Float, topSouth:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(1.0, 0.0, 0.0);
	vertex(u0, v1, x + 1.0, bottom, z);
	vertex(u0, v0, x + 1.0, topNorth, z);
	vertex(u1, v0, x + 1.0, topSouth, z + 1.0);
	vertex(u1, v1, x + 1.0, bottom, z + 1.0);
}

private function emitWest(x:Float, bottom:Float, topNorth:Float, topSouth:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(tint());
	Rlgl.Normal(-1.0, 0.0, 0.0);
	vertex(u0, v1, x, bottom, z);
	vertex(u1, v1, x, bottom, z + 1.0);
	vertex(u1, v0, x, topSouth, z + 1.0);
	vertex(u0, v0, x, topNorth, z);
}

/** Submit one atlas vertex through the reviewed binary32 Rlgl facade. */
private function vertex(u:Float, v:Float, x:Float, y:Float, z:Float):Void {
	Rlgl.TexCoord(u, v);
	Rlgl.Vertex(x, y, z);
}
#end
