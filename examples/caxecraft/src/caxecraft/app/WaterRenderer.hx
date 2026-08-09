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
import caxecraft.app.WaterRenderOrder.compareDepth;
import caxecraft.app.WaterRenderOrder.faceDepthSquared;
import raylib.Color;
import raylib.Rlgl;
import raylib.Texture2D;

/** Counts returned after the separate translucent water submission. */
typedef WaterRenderCounters = {
	final visible:Int;
	final drawCalls:Int;
}

/** One exposed face plus the camera-relative order needed before submission. */
private typedef WaterRenderFace = {
	final kind:Int;
	final x:Int;
	final y:Int;
	final z:Int;
	final corners:WaterSurfaceCorners;
	final depth:Float;
	final stableKey:Int;
}

/**
	Owns reusable transparent-face preparation across rendered frames.

	The renderer keeps one grow-on-demand face buffer because transparent ordering
	needs a complete visible set before submission. Reusing that buffer avoids
	per-frame storage growth; the authoritative fluid state remains in
	`GameSession`, and no render operation advances the simulation.
**/
final class WaterRenderer {
	/** Persistent derived face data; capacity grows only when a busier frame needs it. */
	final faces:Array<WaterRenderFace> = [];

	/** Construct an empty renderer before the application selects a level. */
	public function new() {}

	/**
		Draw the current fluid state without advancing its simulation.

		Solid terrain is submitted first by `TerrainRenderer`; this second coherent
		quad batch draws only exposed water faces with vertex alpha. Neighboring water
		faces are omitted, and cells that meet at one corner share an averaged visual
		height instead of exposing a translucent wall at each one-eighth simulation
		step. Exposed faces are sorted from far to near for the current camera, then
		drawn with depth testing enabled and depth writes disabled. Solid terrain can
		still hide water, while one translucent face cannot cut a hard hole through
		another. The exact discrete state remains in `GameSession`.
	**/
	public function draw(cells:WorldView, texture:Texture2D, textureReady:Bool, presentationCell:Int, eyeX:Float, eyeY:Float, eyeZ:Float):WaterRenderCounters {
		if (!textureReady || presentationCell < 0)
			return {visible: 0, drawCalls: 0};
		faces.resize(0);
		final column = presentationCell % TerrainAtlas.COLUMNS;
		final row = Std.int(presentationCell / TerrainAtlas.COLUMNS);
		final halfPixel = 0.5 / texture.width;
		final u0 = column / TerrainAtlas.COLUMNS + halfPixel;
		final u1 = (column + 1) / TerrainAtlas.COLUMNS - halfPixel;
		final v0 = row / TerrainAtlas.ROWS + halfPixel;
		final v1 = (row + 1) / TerrainAtlas.ROWS - halfPixel;
		var visible = 0;
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
								faces.push(makeFace(0, x, y, z, corners, eyeX, eyeY, eyeZ));
							}
							if (northOpen) {
								faces.push(makeFace(1, x, y, z, corners, eyeX, eyeY, eyeZ));
							}
							if (southOpen) {
								faces.push(makeFace(2, x, y, z, corners, eyeX, eyeY, eyeZ));
							}
							if (eastOpen) {
								faces.push(makeFace(3, x, y, z, corners, eyeX, eyeY, eyeZ));
							}
							if (westOpen) {
								faces.push(makeFace(4, x, y, z, corners, eyeX, eyeY, eyeZ));
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
		faces.sort(compareFaces);
		if (faces.length > 0) {
			Rlgl.BeginTransparentQuads(texture);
			for (face in faces)
				emitFace(face, u0, u1, v0, v1);
			Rlgl.EndTransparentQuads();
		}
		var drawCalls = 0;
		if (faces.length > 0)
			drawCalls = 1;
		return {visible: visible, drawCalls: drawCalls};
	}
}

/** Create one sortable face without changing the authoritative water state. */
private function makeFace(kind:Int, x:Int, y:Int, z:Int, corners:WaterSurfaceCorners, eyeX:Float, eyeY:Float, eyeZ:Float):WaterRenderFace {
	var centerX = x + 0.5;
	var centerY = y + 0.5;
	var centerZ = z + 0.5;
	switch kind {
		case 0:
			centerY = (corners.northWest + corners.northEast + corners.southEast + corners.southWest) * 0.25;
		case 1:
			centerY = (y + corners.northWest + y + corners.northEast) * 0.25;
			centerZ = z;
		case 2:
			centerY = (y + corners.southWest + y + corners.southEast) * 0.25;
			centerZ = z + 1.0;
		case 3:
			centerX = x + 1.0;
			centerY = (y + corners.northEast + y + corners.southEast) * 0.25;
		case 4:
			centerX = x;
			centerY = (y + corners.northWest + y + corners.southWest) * 0.25;
		case _:
	}
	return {
		kind: kind,
		x: x,
		y: y,
		z: z,
		corners: corners,
		depth: faceDepthSquared(eyeX, eyeY, eyeZ, centerX, centerY, centerZ),
		stableKey: (World.indexOf(World.coord(x, y, z)) * 5) + kind
	};
}

/** Compare complete records with the target-neutral scalar ordering rule. */
private function compareFaces(left:WaterRenderFace, right:WaterRenderFace):Int
	return compareDepth(left.depth, left.stableKey, right.depth, right.stableKey);

/** Submit one previously sorted face to the open transparent quad batch. */
private function emitFace(face:WaterRenderFace, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	switch face.kind {
		case 0:
			emitTop(face.x, face.z, face.corners, u0, u1, v0, v1);
		case 1:
			emitNorth(face.x, face.y, face.corners.northWest, face.corners.northEast, face.z, u0, u1, v0, v1);
		case 2:
			emitSouth(face.x, face.y, face.corners.southWest, face.corners.southEast, face.z, u0, u1, v0, v1);
		case 3:
			emitEast(face.x, face.y, face.corners.northEast, face.corners.southEast, face.z, u0, u1, v0, v1);
		case 4:
			emitWest(face.x, face.y, face.corners.northWest, face.corners.southWest, face.z, u0, u1, v0, v1);
		case _:
	}
}

/** Keep horizontal water readable without turning stacked layers opaque. */
private inline function topTint():Color
	return Color.rgba(175, 235, 255, 145);

/** Let a falling sheet reveal terrain and paths through its overlapping faces. */
private inline function sideTint():Color
	return Color.rgba(180, 235, 255, 120);

private function emitTop(x:Float, z:Float, corners:WaterSurfaceCorners, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(topTint());
	Rlgl.Normal(0.0, 1.0, 0.0);
	vertex(u0, v0, x, corners.northWest, z);
	vertex(u0, v1, x, corners.southWest, z + 1.0);
	vertex(u1, v1, x + 1.0, corners.southEast, z + 1.0);
	vertex(u1, v0, x + 1.0, corners.northEast, z);
}

private function emitNorth(x:Float, bottom:Float, topWest:Float, topEast:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(sideTint());
	Rlgl.Normal(0.0, 0.0, -1.0);
	vertex(u0, v1, x, bottom, z);
	vertex(u0, v0, x, topWest, z);
	vertex(u1, v0, x + 1.0, topEast, z);
	vertex(u1, v1, x + 1.0, bottom, z);
}

private function emitSouth(x:Float, bottom:Float, topWest:Float, topEast:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(sideTint());
	Rlgl.Normal(0.0, 0.0, 1.0);
	vertex(u0, v1, x, bottom, z + 1.0);
	vertex(u1, v1, x + 1.0, bottom, z + 1.0);
	vertex(u1, v0, x + 1.0, topEast, z + 1.0);
	vertex(u0, v0, x, topWest, z + 1.0);
}

private function emitEast(x:Float, bottom:Float, topNorth:Float, topSouth:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(sideTint());
	Rlgl.Normal(1.0, 0.0, 0.0);
	vertex(u0, v1, x + 1.0, bottom, z);
	vertex(u0, v0, x + 1.0, topNorth, z);
	vertex(u1, v0, x + 1.0, topSouth, z + 1.0);
	vertex(u1, v1, x + 1.0, bottom, z + 1.0);
}

private function emitWest(x:Float, bottom:Float, topNorth:Float, topSouth:Float, z:Float, u0:Float, u1:Float, v0:Float, v1:Float):Void {
	Rlgl.Color(sideTint());
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
