package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.editor.EditorWorldGrid.decode as decodeWorld;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioWorld;

/**
	Projects editor terrain into a small, renderer-independent top-down view.

	The CAXEMAP draft remains the only world model. `project` reads one horizontal
	layer into a cached array after an edit; drawing code can then reuse that
	array for every steady frame. The same integer layout maps pointer pixels
	back to voxel coordinates, so Eval tests and the native Raylib screen agree
	on cell boundaries without either side imitating the other.
**/
/** The four block tools exposed by the first visual editor slice. */
enum EditorTool {
	SelectTool;
	PaintTool;
	EraseTool;
	FillTool;
}

/**
	One read-only horizontal layer prepared for repeated drawing.

	`cells` uses `z * width + x` order. Haxe arrays are mutable containers, so
	callers must treat this fresh array as read-only; only `EditorSession`
	commands may change the authored draft.
**/
typedef EditorViewportProjection = {
	final width:Int;
	final depth:Int;
	final layerY:Int;
	final cells:Array<Int>;
}

/**
	The centered square-cell grid inside one screen-space container.

	All edges use integer pixels and the right and bottom edges are excluded.
	This makes a pointer exactly on a grid line choose one deterministic cell.
**/
typedef EditorViewportLayout = {
	final left:Int;
	final top:Int;
	final width:Int;
	final height:Int;
	final cellSize:Int;
}

/** Either a closed editor command or the exact reason the tool cannot run. */
enum EditorToolCommandResult {
	ToolCommandReady(command:EditorCommand);
	ToolCommandRejected(error:EditorError);
}

/**
	Read one horizontal world layer for repeated presentation.

	The function rejects an out-of-range layer or malformed chunk coverage by
	returning `null`. It never repairs the map. Successful projection allocates
	one fresh compact layer, which the screen caches until the next accepted edit.
**/
function project(world:ScenarioWorld, layerY:Int):Null<EditorViewportProjection> {
	if (layerY < 0 || layerY >= world.size.height)
		return null;
	final worldCells = decodeWorld(world);
	if (worldCells == null)
		return null;
	final cells:Array<Int> = [];
	for (z in 0...world.size.depth)
		for (x in 0...world.size.width)
			cells.push(worldCells[(z * world.size.height + layerY) * world.size.width + x]);
	return {
		width: world.size.width,
		depth: world.size.depth,
		layerY: layerY,
		cells: cells
	};
}

/**
	Fit the largest centered square-cell grid inside a pixel rectangle.

	A container too small to give every voxel at least one pixel returns `null`.
	The caller draws and hit-tests with the returned values, so visual and input
	geometry cannot drift apart.
**/
function layout(containerLeft:Int, containerTop:Int, containerWidth:Int, containerHeight:Int, projection:EditorViewportProjection):Null<EditorViewportLayout> {
	if (containerWidth <= 0 || containerHeight <= 0 || projection.width <= 0 || projection.depth <= 0)
		return null;
	final widthCellSize = Std.int(containerWidth / projection.width);
	final depthCellSize = Std.int(containerHeight / projection.depth);
	final cellSize = widthCellSize < depthCellSize ? widthCellSize : depthCellSize;
	if (cellSize <= 0)
		return null;
	final width = cellSize * projection.width;
	final height = cellSize * projection.depth;
	return {
		left: containerLeft + Std.int((containerWidth - width) / 2),
		top: containerTop + Std.int((containerHeight - height) / 2),
		width: width,
		height: height,
		cellSize: cellSize
	};
}

/**
	Map one screen pixel to the voxel shown underneath it.

	The returned point always lies on the projection's selected Y layer. Pixels
	outside the centered grid return `null`, including the excluded right and
	bottom edges.
**/
function pointAt(projection:EditorViewportProjection, grid:EditorViewportLayout, screenX:Int, screenY:Int):Null<VoxelPoint> {
	if (screenX < grid.left || screenY < grid.top || screenX >= grid.left + grid.width || screenY >= grid.top + grid.height)
		return null;
	return {
		x: Std.int((screenX - grid.left) / grid.cellSize),
		y: projection.layerY,
		z: Std.int((screenY - grid.top) / grid.cellSize)
	};
}

/** Return one projected palette code, or `-1` for an invalid coordinate. */
function paletteCodeAt(projection:EditorViewportProjection, x:Int, z:Int):Int {
	if (x < 0 || z < 0 || x >= projection.width || z >= projection.depth)
		return -1;
	return projection.cells[z * projection.width + x];
}

/**
	Find the draft-local palette code for one semantic block type.

	Palette codes are compact numbers chosen by each map. They are not global
	block IDs, so an editor brush must resolve its block for the current draft.
	The function returns `-1` when the map does not admit that block.
**/
function paletteCodeForBlock(world:ScenarioWorld, blockType:ContentId):Int {
	final expected = blockType.text();
	for (entry in world.palette)
		if (entry.blockType.text() == expected)
			return entry.code;
	return -1;
}

/** Convert raygui's zero-based list selection into the closed editor tool type. */
function toolFromIndex(index:Int):Null<EditorTool> {
	return switch index {
		case 0: SelectTool;
		case 1: PaintTool;
		case 2: EraseTool;
		case 3: FillTool;
		case _: null;
	};
}

/**
	Translate one tool gesture into the existing `EditorSession` command language.

	Select chooses exactly one visible voxel. Paint and erase affect the pointed
	voxel. Fill reuses the current rectangular selection and therefore reports
	`NoSelection` until the author selects something. The UI never mutates a
	projection directly; every successful result still passes through
	`EditorSession.apply`, history, and validation.
**/
function commandFor(tool:EditorTool, point:VoxelPoint, paletteCode:Int, selection:Null<VoxelBounds>):EditorToolCommandResult {
	return switch tool {
		case SelectTool:
			ToolCommandReady(Select({
				origin: point,
				size: {width: 1, height: 1, depth: 1}
			}));
		case PaintTool:
			ToolCommandReady(PaintVoxel(point, paletteCode));
		case EraseTool:
			ToolCommandReady(EraseVoxel(point));
		case FillTool:
			if (selection == null) ToolCommandRejected(NoSelection); else ToolCommandReady(FillSelection(paletteCode));
	};
}
