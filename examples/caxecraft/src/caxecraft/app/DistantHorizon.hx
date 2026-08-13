package caxecraft.app;

import caxecraft.app.WaterSurfaceGeometry.isWater;
import caxecraft.app.WaterSurfaceGeometry.surfaceTop;
import caxecraft.domain.BlockKind;
import caxecraft.domain.WaterCellCodec.stateInView as waterStateAt;
import caxecraft.domain.World;
import caxecraft.domain.WorldRead.query;
import caxecraft.domain.WorldRead.surfaceY;
import caxecraft.domain.WorldView;
#if c
import caxecraft.app.TerrainAtlas.TerrainSheet;
import caxecraft.app.TerrainAtlas.VoxelFace;
import caxecraft.app.TerrainRenderer.emitTintedFace;
import raylib.Color;
import raylib.Rlgl;
import raylib.Texture2D;
#end

/**
	Derives distant scenery from the playable map's own boundary.

	CaxeMap remains the source of the visible material and water edge. This module
	extends only presentation beyond the finite simulation: the added hills and
	water cannot collide, flow, receive edits, or change gameplay. Three broad
	voxel layers hide the abrupt world cut while keeping the block-built style.
**/
enum HorizonEdge {
	North;
	South;
	East;
	West;
}

/** One diagonal region shared by two independently enabled horizon edges. */
enum HorizonCorner {
	NorthWest;
	NorthEast;
	SouthWest;
	SouthEast;
}

/** One authored edge sample used to continue land and water into the distance. */
typedef HorizonColumn = {
	/** Highest authored solid block, or `-1` for an empty boundary column. */
	final surfaceY:Int;

	/** Material at `surfaceY`, or `Air` when no solid block exists. */
	final material:BlockKind;

	/** Highest visible water surface, or `-1.0` when the column is dry. */
	final waterTop:Float;
}

/**
	Read one column from the selected physical edge.

	`authoredWidth` distinguishes the older 32-wide maps from the 64-wide world
	storage. The synthetic safety wall after a compact map is therefore never
	mistaken for authored scenery.
**/
function boundaryColumn(cells:WorldView, authoredWidth:Int, edge:HorizonEdge, offset:Int):HorizonColumn {
	final x = switch edge {
		case North | South: offset;
		case East: authoredWidth - 1;
		case West: 0;
	};
	final z = switch edge {
		case North: 0;
		case South: World.DEPTH - 1;
		case East | West: offset;
	};
	final solidY = surfaceY(cells, x, z);
	var highestWater = -1.0;
	var y = 0;
	while (y < World.HEIGHT) {
		final state = waterStateAt(cells, World.coord(x, y, z));
		if (isWater(state)) {
			final top = surfaceTop(state, y);
			if (top > highestWater)
				highestWater = top;
		}
		y++;
	}
	return {
		surfaceY: solidY,
		material: solidY < 0 ? BlockKind.Air : query(cells, World.coord(x, solidY, z)),
		waterTop: highestWater
	};
}

/**
	Return the top block for one increasingly distant scenery layer.

	The nearest layer preserves the authored silhouette exactly. Farther layers
	add bounded deterministic relief, so a flat map edge becomes a distant ridge
	rather than another flat wall. The result never exceeds the world's vertical
	envelope.
**/
function ridgeHeight(column:HorizonColumn, layer:Int, sample:Int):Int {
	var base = column.surfaceY;
	if (base < 0)
		base = 1;
	var extra = 0;
	if (layer == 1)
		extra = 2 + positiveNoise(sample, 17) % 4;
	else if (layer >= 2)
		extra = 6 + positiveNoise(sample, 43) % 7;
	final result = base + extra;
	return result < World.HEIGHT ? result : World.HEIGHT - 1;
}

/**
	Return whether both edges needed by one diagonal corner are enabled.

	A creator can omit either edge to keep that direction open. The renderer must
	not infer a corner from terrain alone because the environment block owns which
	directions receive distant scenery.
**/
function cornerEnabled(horizonMask:Int, corner:HorizonCorner):Bool {
	return switch corner {
		case NorthWest: horizonMask & 1 != 0 && horizonMask & 8 != 0;
		case NorthEast: horizonMask & 1 != 0 && horizonMask & 4 != 0;
		case SouthWest: horizonMask & 2 != 0 && horizonMask & 8 != 0;
		case SouthEast: horizonMask & 2 != 0 && horizonMask & 4 != 0;
	};
}

/** Interpolate the same three ridge anchors used by a straight edge apron. */
function horizonApronHeight(column:HorizonColumn, sample:Int, distance:Int):Int {
	final near = ridgeHeight(column, 0, sample);
	final middle = ridgeHeight(column, 1, sample);
	final far = ridgeHeight(column, 2, sample);
	if (distance <= 8)
		return near;
	if (distance <= 26)
		return near + Std.int((middle - near) * (distance - 8) / 18);
	return middle + Std.int((far - middle) * (distance - 26) / 28);
}

/** Stable small integer variation; it is presentation and not world generation. */
private function positiveNoise(sample:Int, salt:Int):Int {
	return (sample * 1103515245 + salt * 12345) & 0x7fffffff;
}

#if c
/**
	Draws atlas-textured atmospheric voxel scenery around one finite authored map.

	Each visible column repeats the boundary material's normal one-block texture.
	This costs a bounded number of immediate quads, needs no additional asset, and
	keeps distant scenery visually related to the map without implying collision.
**/
final class DistantHorizonRenderer {
	var descriptorValue:Int;

	/** Construct the stateless renderer. */
	public function new() {
		descriptorValue = 0;
	}

	/** Select this frame's authored width and environment flags before drawing. */
	public inline function configure(authoredWidth:Int, horizonMask:Int):Void
		descriptorValue = authoredWidth | (horizonMask << 8);

	/** Submit only the edges enabled by the level's optional environment. */
	public function draw(cells:WorldView, baseTexture:Texture2D, baseReady:Bool, adventureTexture:Texture2D, adventureReady:Bool, waterCell:Int):Void {
		final authoredWidth = descriptorValue & 255;
		final horizonMask = descriptorValue >> 8;
		if (authoredWidth <= 0 || authoredWidth > World.WIDTH)
			return;
		final north = horizonMask & 1 != 0;
		final south = horizonMask & 2 != 0;
		final east = horizonMask & 4 != 0;
		final west = horizonMask & 8 != 0;
		drawSheet(cells, authoredWidth, north, south, east, west, baseTexture, baseReady, TerrainSheet.Base);
		drawSheet(cells, authoredWidth, north, south, east, west, adventureTexture, adventureReady, TerrainSheet.Adventure);
		if (horizonMask & 16 != 0)
			drawBoundaryWater(cells, authoredWidth, north, south, east, west, baseTexture, baseReady, waterCell);
	}

	/** Repeat the normal water texture where an authored stream meets an edge. */
	function drawBoundaryWater(cells:WorldView, authoredWidth:Int, north:Bool, south:Bool, east:Bool, west:Bool, texture:Texture2D, ready:Bool,
			presentationCell:Int):Void {
		if (!ready || presentationCell < 0)
			return;
		final column = presentationCell % TerrainAtlas.COLUMNS;
		final row = Std.int(presentationCell / TerrainAtlas.COLUMNS);
		final halfPixel = 0.5 / texture.width;
		final u0 = column / TerrainAtlas.COLUMNS + halfPixel;
		final u1 = (column + 1) / TerrainAtlas.COLUMNS - halfPixel;
		final v0 = row / TerrainAtlas.ROWS + halfPixel;
		final v1 = (row + 1) / TerrainAtlas.ROWS - halfPixel;
		Rlgl.BeginTransparentQuads(texture);
		if (north)
			drawEdgeWater(cells, authoredWidth, HorizonEdge.North, u0, u1, v0, v1);
		if (south)
			drawEdgeWater(cells, authoredWidth, HorizonEdge.South, u0, u1, v0, v1);
		if (east)
			drawEdgeWater(cells, authoredWidth, HorizonEdge.East, u0, u1, v0, v1);
		if (west)
			drawEdgeWater(cells, authoredWidth, HorizonEdge.West, u0, u1, v0, v1);
		Rlgl.EndTransparentQuads();
	}

	/** Continue every wet edge cell to the same distant limit as its terrain. */
	function drawEdgeWater(cells:WorldView, authoredWidth:Int, edge:HorizonEdge, u0:Float, u1:Float, v0:Float, v1:Float):Void {
		final samples = switch edge {
			case North | South: authoredWidth;
			case East | West: World.DEPTH;
		};
		var offset = 0;
		while (offset < samples) {
			final column = boundaryColumn(cells, authoredWidth, edge, offset);
			if (column.waterTop >= 0.0) {
				var distance = 1;
				while (distance <= 54) {
					emitWaterTop(edge, authoredWidth, offset, distance, column.waterTop, u0, u1, v0, v1,
						distance < 14 ? CaxecraftPalette.horizonWaterNear() : CaxecraftPalette.horizonWaterFar());
					distance++;
				}
			}
			offset++;
		}
	}

	/** Submit one horizontal textured water tile outside the collision world. */
	function emitWaterTop(edge:HorizonEdge, authoredWidth:Int, offset:Int, distance:Int, waterTop:Float, u0:Float, u1:Float, v0:Float, v1:Float,
			tint:Color):Void {
		final x = switch edge {
			case North | South: offset;
			case East: authoredWidth - 1 + distance;
			case West: -distance;
		};
		final z = switch edge {
			case North: -distance;
			case South: World.DEPTH - 1 + distance;
			case East | West: offset;
		};
		Rlgl.Color(tint);
		Rlgl.Normal(0.0, 1.0, 0.0);
		waterVertex(u0, v0, x, waterTop, z);
		waterVertex(u0, v1, x, waterTop, z + 1);
		waterVertex(u1, v1, x + 1, waterTop, z + 1);
		waterVertex(u1, v0, x + 1, waterTop, z);
	}

	/** Repeat every selected material through one texture-bound rlgl region. */
	function drawSheet(cells:WorldView, authoredWidth:Int, north:Bool, south:Bool, east:Bool, west:Bool, texture:Texture2D, ready:Bool,
			sheet:TerrainSheet):Void {
		if (!ready)
			return;
		final halfPixel = 0.5 / texture.width;
		Rlgl.BeginQuads(texture);
		if (north)
			drawEdge(cells, authoredWidth, HorizonEdge.North, sheet, halfPixel);
		if (south)
			drawEdge(cells, authoredWidth, HorizonEdge.South, sheet, halfPixel);
		if (east)
			drawEdge(cells, authoredWidth, HorizonEdge.East, sheet, halfPixel);
		if (west)
			drawEdge(cells, authoredWidth, HorizonEdge.West, sheet, halfPixel);
		final horizonMask = (north ? 1 : 0) | (south ? 2 : 0) | (east ? 4 : 0) | (west ? 8 : 0);
		if (cornerEnabled(horizonMask, HorizonCorner.NorthWest))
			drawCorner(cells, authoredWidth, HorizonCorner.NorthWest, sheet, halfPixel);
		if (cornerEnabled(horizonMask, HorizonCorner.NorthEast))
			drawCorner(cells, authoredWidth, HorizonCorner.NorthEast, sheet, halfPixel);
		if (cornerEnabled(horizonMask, HorizonCorner.SouthWest))
			drawCorner(cells, authoredWidth, HorizonCorner.SouthWest, sheet, halfPixel);
		if (cornerEnabled(horizonMask, HorizonCorner.SouthEast))
			drawCorner(cells, authoredWidth, HorizonCorner.SouthEast, sheet, halfPixel);
		Rlgl.EndQuads();
	}

	/** Fill one diagonal gap with repeated terrain tiles from the shared corner. */
	function drawCorner(cells:WorldView, authoredWidth:Int, corner:HorizonCorner, sheet:TerrainSheet, halfPixel:Float):Void {
		final edge = switch corner {
			case NorthWest | NorthEast: HorizonEdge.North;
			case SouthWest | SouthEast: HorizonEdge.South;
		};
		final offset = switch corner {
			case NorthWest | SouthWest: 0;
			case NorthEast | SouthEast: authoredWidth - 1;
		};
		final column = boundaryColumn(cells, authoredWidth, edge, offset);
		if (column.surfaceY < 0 || TerrainAtlas.sheet(column.material) != sheet)
			return;
		emitCornerTop(column.material, authoredWidth, corner, column.surfaceY, halfPixel);
	}

	/**
		Continue each authored edge as connected ground, then raise it into hills.

		Every distance receives a textured top tile. This terrain apron removes the
		blue gap that a few detached ridge walls left between the playable map and
		the horizon. Faces are presentation only and never enter collision storage.
	**/
	function drawEdge(cells:WorldView, authoredWidth:Int, edge:HorizonEdge, sheet:TerrainSheet, halfPixel:Float):Void {
		final samples = switch edge {
			case North | South: authoredWidth;
			case East | West: World.DEPTH;
		};
		var offset = 0;
		while (offset < samples) {
			final column = boundaryColumn(cells, authoredWidth, edge, offset);
			if (column.surfaceY >= 0 && TerrainAtlas.sheet(column.material) == sheet)
				drawTerrainApron(edge, authoredWidth, offset, column, halfPixel);
			if (offset > 0) {
				final previous = boundaryColumn(cells, authoredWidth, edge, offset - 1);
				drawLateralSeam(edge, authoredWidth, offset, previous, column, sheet, halfPixel);
			}
			offset++;
		}
	}

	/** Fill the vertical seam where neighboring terrain strips have different heights. */
	function drawLateralSeam(edge:HorizonEdge, authoredWidth:Int, offset:Int, previous:HorizonColumn, current:HorizonColumn, sheet:TerrainSheet,
			halfPixel:Float):Void {
		if (previous.surfaceY < 0 || current.surfaceY < 0)
			return;
		var startDistance = 1;
		while (startDistance <= 54) {
			final endDistance = apronSegmentEnd(startDistance);
			final previousHeight = horizonApronHeight(previous, offset - 1, endDistance);
			final currentHeight = horizonApronHeight(current, offset, endDistance);
			if (currentHeight > previousHeight && TerrainAtlas.sheet(current.material) == sheet)
				emitLateralStrip(current.material, edge, authoredWidth, offset, startDistance, endDistance, previousHeight + 1, currentHeight + 1, halfPixel,
					endDistance <= 12 ? nearTint() : (endDistance <= 34 ? middleTint() : farTint()), true);
			else if (previousHeight > currentHeight && TerrainAtlas.sheet(previous.material) == sheet)
				emitLateralStrip(previous.material, edge, authoredWidth, offset, startDistance, endDistance, currentHeight + 1, previousHeight + 1, halfPixel,
					endDistance <= 12 ? nearTint() : (endDistance <= 34 ? middleTint() : farTint()), false);
			startDistance = endDistance + 1;
		}
	}

	/** Draw one continuous 54-block strip from the authored edge to the far ridge. */
	function drawTerrainApron(edge:HorizonEdge, authoredWidth:Int, offset:Int, column:HorizonColumn, halfPixel:Float):Void {
		var previousHeight = column.surfaceY;
		var startDistance = 1;
		while (startDistance <= 54) {
			final endDistance = apronSegmentEnd(startDistance);
			final height = horizonApronHeight(column, offset, endDistance);
			final tint = endDistance <= 12 ? nearTint() : (endDistance <= 34 ? middleTint() : farTint());
			drawApronSegment(edge, authoredWidth, offset, height, previousHeight, startDistance, endDistance, tint, column.material, halfPixel);
			previousHeight = height;
			startDistance = endDistance + 1;
		}
	}

	/**
		Keep nearby blocks detailed and group progressively larger distant spans.

		The surface remains geometrically continuous through block 54. Far texture
		detail is stretched across a broader strip, which is cheaper for the software
		reviewer and visually appropriate after atmospheric tint has softened it.
	**/
	function apronSegmentEnd(startDistance:Int):Int {
		if (startDistance <= 4)
			return startDistance;
		if (startDistance <= 8)
			return startDistance + 1;
		if (startDistance <= 20)
			return startDistance + 3;
		if (startDistance <= 38)
			return startDistance + 7;
		return 54;
	}

	/** Draw one connected top strip and the inward-facing riser of an uphill step. */
	function drawApronSegment(edge:HorizonEdge, authoredWidth:Int, offset:Int, topBlock:Int, previousHeight:Int, startDistance:Int, endDistance:Int,
			tint:Color, material:BlockKind, halfPixel:Float):Void {
		final riserX = switch edge {
			case North | South: offset;
			case East: authoredWidth - 1 + startDistance;
			case West: -startDistance;
		};
		final riserZ = switch edge {
			case North: -startDistance;
			case South: World.DEPTH - 1 + startDistance;
			case East | West: offset;
		};
		final x = switch edge {
			case North | South: offset;
			case East: authoredWidth - 1 + startDistance;
			case West: -endDistance;
		};
		final z = switch edge {
			case North: -endDistance;
			case South: World.DEPTH - 1 + startDistance;
			case East | West: offset;
		};
		final outwardFace = switch edge {
			case North: VoxelFace.South;
			case South: VoxelFace.North;
			case East: VoxelFace.West;
			case West: VoxelFace.East;
		};
		var y = previousHeight + 1;
		while (y <= topBlock) {
			emitTintedFace(material, outwardFace, riserX, y, riserZ, halfPixel, tint);
			y++;
		}
		emitTopStrip(material, edge, x, topBlock, z, endDistance - startDistance + 1, halfPixel, tint);
	}

	/** Submit one atlas-textured horizontal strip to the current terrain batch. */
	function emitTopStrip(material:BlockKind, edge:HorizonEdge, x:Int, y:Int, z:Int, length:Int, halfPixel:Float, tint:Color):Void {
		final tile = TerrainAtlas.tile(material, VoxelFace.Top);
		final u0 = TerrainAtlas.uMin(tile, halfPixel);
		final u1 = TerrainAtlas.uMax(tile, halfPixel);
		final v0 = TerrainAtlas.vMin(tile, halfPixel);
		final v1 = TerrainAtlas.vMax(tile, halfPixel);
		final width = switch edge {
			case North | South: 1;
			case East | West: length;
		};
		final depth = switch edge {
			case North | South: length;
			case East | West: 1;
		};
		Rlgl.Color(tint);
		Rlgl.Normal(0.0, 1.0, 0.0);
		waterVertex(u0, v0, x, y + 1, z);
		waterVertex(u0, v1, x, y + 1, z + depth);
		waterVertex(u1, v1, x + width, y + 1, z + depth);
		waterVertex(u1, v0, x + width, y + 1, z);
	}

	/**
		Submit small repeated patches outside both edges of an authored corner.

		One texture stretched across the full corner looks like a smooth coloured
		sheet. Two-block patches keep the voxel texture legible while adding only a
		bounded number of presentation quads.
	**/
	function emitCornerTop(material:BlockKind, authoredWidth:Int, corner:HorizonCorner, y:Int, halfPixel:Float):Void {
		final tile = TerrainAtlas.tile(material, VoxelFace.Top);
		final u0 = TerrainAtlas.uMin(tile, halfPixel);
		final u1 = TerrainAtlas.uMax(tile, halfPixel);
		final v0 = TerrainAtlas.vMin(tile, halfPixel);
		final v1 = TerrainAtlas.vMax(tile, halfPixel);
		final x0 = switch corner {
			case NorthWest | SouthWest: -54;
			case NorthEast | SouthEast: authoredWidth;
		};
		final x1 = switch corner {
			case NorthWest | SouthWest: 0;
			case NorthEast | SouthEast: authoredWidth + 54;
		};
		final z0 = switch corner {
			case NorthWest | NorthEast: -54;
			case SouthWest | SouthEast: World.DEPTH;
		};
		final z1 = switch corner {
			case NorthWest | NorthEast: 0;
			case SouthWest | SouthEast: World.DEPTH + 54;
		};
		var x = x0;
		while (x < x1) {
			var z = z0;
			while (z < z1) {
				final distanceX = x < 0 ? -x : x - authoredWidth + 2;
				final distanceZ = z < 0 ? -z : z - World.DEPTH + 2;
				final distance = distanceX > distanceZ ? distanceX : distanceZ;
				Rlgl.Color(distance <= 12 ? nearTint() : (distance <= 34 ? middleTint() : farTint()));
				Rlgl.Normal(0.0, 1.0, 0.0);
				waterVertex(u0, v0, x, y + 1, z);
				waterVertex(u0, v1, x, y + 1, z + 2);
				waterVertex(u1, v1, x + 2, y + 1, z + 2);
				waterVertex(u1, v0, x + 2, y + 1, z);
				z += 2;
			}
			x += 2;
		}
	}
}

/** Draw one stretched side face between adjacent horizon columns. */
private function emitLateralStrip(material:BlockKind, edge:HorizonEdge, authoredWidth:Int, offset:Int, startDistance:Int, endDistance:Int, lowY:Int,
		highY:Int, halfPixel:Float, tint:Color, currentIsTaller:Bool):Void {
	final face = switch edge {
		case North | South: currentIsTaller ? VoxelFace.West : VoxelFace.East;
		case East | West: currentIsTaller ? VoxelFace.North : VoxelFace.South;
	};
	final tile = TerrainAtlas.tile(material, face);
	final u0 = TerrainAtlas.uMin(tile, halfPixel);
	final u1 = TerrainAtlas.uMax(tile, halfPixel);
	final v0 = TerrainAtlas.vMin(tile, halfPixel);
	final v1 = TerrainAtlas.vMax(tile, halfPixel);
	final x0 = switch edge {
		case North | South: offset;
		case East: authoredWidth - 1 + startDistance;
		case West: -endDistance;
	};
	final x1 = switch edge {
		case North | South: offset;
		case East: authoredWidth + endDistance;
		case West:
			-startDistance + 1;
	};
	final z0 = switch edge {
		case North: -endDistance;
		case South: World.DEPTH - 1 + startDistance;
		case East | West: offset;
	};
	final z1 = switch edge {
		case North:
			-startDistance + 1;
		case South: World.DEPTH + endDistance;
		case East | West: offset;
	};
	Rlgl.Color(tint);
	switch face {
		case North:
			Rlgl.Normal(0.0, 0.0, -1.0);
			waterVertex(u0, v1, x0, lowY, z0);
			waterVertex(u0, v0, x0, highY, z0);
			waterVertex(u1, v0, x1, highY, z0);
			waterVertex(u1, v1, x1, lowY, z0);
		case South:
			Rlgl.Normal(0.0, 0.0, 1.0);
			waterVertex(u0, v1, x0, lowY, z0);
			waterVertex(u1, v1, x1, lowY, z0);
			waterVertex(u1, v0, x1, highY, z0);
			waterVertex(u0, v0, x0, highY, z0);
		case East:
			Rlgl.Normal(1.0, 0.0, 0.0);
			waterVertex(u0, v1, x0, lowY, z0);
			waterVertex(u0, v0, x0, highY, z0);
			waterVertex(u1, v0, x0, highY, z1);
			waterVertex(u1, v1, x0, lowY, z1);
		case West:
			Rlgl.Normal(-1.0, 0.0, 0.0);
			waterVertex(u0, v1, x0, lowY, z0);
			waterVertex(u1, v1, x0, lowY, z1);
			waterVertex(u1, v0, x0, highY, z1);
			waterVertex(u0, v0, x0, highY, z0);
		case Top | Bottom:
	}
}

/** Submit one textured distant-water vertex to the current rlgl batch. */
private inline function waterVertex(u:Float, v:Float, x:Float, y:Float, z:Float):Void {
	Rlgl.TexCoord(u, v);
	Rlgl.Vertex(x, y, z);
}

/** Keep the near atlas colors strong while separating them from playable land. */
private inline function nearTint():Color
	return Color.rgba(198, 218, 204);

/** Blend the middle atlas layer toward the existing atmospheric palette. */
private inline function middleTint():Color
	return Color.rgba(178, 204, 198);

/** Make the far texture subtle without erasing its voxel silhouette. */
private inline function farTint():Color
	return Color.rgba(163, 196, 194);
#end
