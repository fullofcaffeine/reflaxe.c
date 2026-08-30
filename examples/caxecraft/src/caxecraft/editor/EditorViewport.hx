package caxecraft.editor;

import caxecraft.editor.EditorTypes.EditorCommand;
import caxecraft.editor.EditorTypes.EditorError;
import caxecraft.content.EditorObjectCatalog.EditorObjectRecipe;
import caxecraft.editor.EditorPlacement.checkpointTemplate;
import caxecraft.editor.EditorPlacement.objectRecipeTemplate;
import caxecraft.editor.EditorPlacement.EditorObjectTemplateResult;
import caxecraft.editor.EditorPlacement.EditorObjectTemplateContext;
import caxecraft.editor.EditorPlacement.triggerZoneCommand;
import caxecraft.editor.EditorWorldGrid.decode as decodeWorld;
import caxecraft.editor.EditorWorldViewport.EditorWorldProjection;
import caxecraft.scenario.ContentId;
import caxecraft.scenario.ScenarioGeometry.VoxelBounds;
import caxecraft.scenario.ScenarioGeometry.VoxelPoint;
import caxecraft.scenario.ScenarioGeometry.VoxelSize;
import caxecraft.scenario.ScenarioWorld;
import caxecraft.scenario.ScenarioWorld.BlockPaletteEntry;
import caxecraft.scenario.ScenarioObject;
import caxecraft.scenario.ScenarioId;

/**
	Projects editor terrain into a small, renderer-independent top-down view.

	The CAXEMAP draft remains the only world model. `project` reads one horizontal
	layer into a cached array after an edit; drawing code can then reuse that
	array for every steady frame. The same integer layout maps pointer pixels
	back to voxel coordinates, so Eval tests and the native Raylib screen agree
	on cell boundaries without either side imitating the other.
**/
/** The closed terrain and object tools exposed by the visual editor. */
enum EditorTool {
	SelectTool;
	PaintTool;
	EraseTool;
	FillTool;
	CheckpointTool;
	CatalogObjectTool;
	TriggerZoneTool;
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

	/** Non-air cells prepared once so Plan does not scan empty terrain per frame. */
	final paintedCells:Array<EditorViewportCell>;
}

/** One non-air Plan cell in canonical row-major order. */
typedef EditorViewportCell = {
	final x:Int;
	final z:Int;
	final paletteCode:Int;
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
	ToolSelectionReady(bounds:VoxelBounds);
	ToolCommandReady(command:EditorCommand);

	/** Several commands that must preview and commit as one edit. */
	ToolBatchReady(commands:Array<EditorCommand>, selectedObject:ScenarioId);

	ToolCommandRejected(error:EditorError);
}

/** Named draft facts used by tool gestures, kept explicit as the editor grows. */
typedef EditorToolContext = {
	final scenarioId:ScenarioId;
	final worldSize:VoxelSize;
	final paletteCode:Int;
	final selection:Null<VoxelBounds>;
	final objects:Array<ScenarioObject>;
	final ruleIds:Array<ScenarioId>;
	final dialogueIds:Array<ScenarioId>;
	final recipe:Null<EditorObjectRecipe>;
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
		cells: cells,
		paintedCells: collectPaintedCells(cells, world.size.width)
	};
}

/**
	Build one plan layer from cells that the 3D editor already decoded.

	The screen prepares both views after an accepted edit. Reusing the complete
	cell array avoids decoding every world chunk twice during that interaction.
	The function checks the array length and layer before it reads any cell, then
	returns the same compact layout as `project`.
**/
function projectFromCells(world:ScenarioWorld, worldCells:Array<Int>, layerY:Int):Null<EditorViewportProjection> {
	if (layerY < 0 || layerY >= world.size.height)
		return null;
	if (worldCells.length != world.size.width * world.size.height * world.size.depth)
		return null;
	final cells:Array<Int> = [];
	for (z in 0...world.size.depth)
		for (x in 0...world.size.width)
			cells.push(worldCells[(z * world.size.height + layerY) * world.size.width + x]);
	return {
		width: world.size.width,
		depth: world.size.depth,
		layerY: layerY,
		cells: cells,
		paintedCells: collectPaintedCells(cells, world.size.width)
	};
}

/**
	Build one horizontal layer from the complete cached 3D projection.

	The native layer controls call this function without serializing or decoding
	the CAXEMAP draft again. The returned cells remain a compact read-only copy,
	so changing the selected layer cannot mutate terrain or editor history.
**/
function projectFromWorld(world:EditorWorldProjection, layerY:Int):Null<EditorViewportProjection> {
	if (layerY < 0 || layerY >= world.height)
		return null;
	if (world.cells.length != world.width * world.height * world.depth)
		return null;
	final cells:Array<Int> = [];
	for (z in 0...world.depth)
		for (x in 0...world.width)
			cells.push(world.cells[(z * world.height + layerY) * world.width + x]);
	return {
		width: world.width,
		depth: world.depth,
		layerY: layerY,
		cells: cells,
		paintedCells: collectPaintedCells(cells, world.width)
	};
}

/**
	Patch one accepted voxel into the cached Plan layer without scanning the layer.

	A voxel on another Y layer leaves this projection valid and returns `true`.
	Malformed coordinates or a painted-cell cache that disagrees with `cells`
	return `false`, so the screen can rebuild the layer from its complete world
	projection. The painted rows stay in canonical row-major order.
**/
function patchProjectedVoxel(projection:EditorViewportProjection, point:VoxelPoint, paletteCode:Int):Bool {
	if (point.x < 0 || point.z < 0 || point.x >= projection.width || point.z >= projection.depth)
		return false;
	if (projection.cells.length != projection.width * projection.depth)
		return false;
	if (point.y != projection.layerY)
		return true;

	final cellIndex = point.z * projection.width + point.x;
	final previousCode = projection.cells[cellIndex];
	var low = 0;
	var high = projection.paintedCells.length;
	while (low < high) {
		final middle = low + Std.int((high - low) / 2);
		final row = projection.paintedCells[middle];
		final rowIndex = row.z * projection.width + row.x;
		if (rowIndex < cellIndex)
			low = middle + 1;
		else
			high = middle;
	}
	final hasPaintedRow = low < projection.paintedCells.length
		&& projection.paintedCells[low].z * projection.width + projection.paintedCells[low].x == cellIndex;
	if ((previousCode != 0) != hasPaintedRow)
		return false;
	if (previousCode == paletteCode)
		return true;

	projection.cells[cellIndex] = paletteCode;
	if (paletteCode == 0) {
		projection.paintedCells.splice(low, 1);
	} else {
		final row:EditorViewportCell = {x: point.x, z: point.z, paletteCode: paletteCode};
		if (hasPaintedRow)
			projection.paintedCells[low] = row;
		else
			projection.paintedCells.insert(low, row);
	}
	return true;
}

/** Collect compact painted rows while preserving exact x/z display order. */
function collectPaintedCells(cells:Array<Int>, width:Int):Array<EditorViewportCell> {
	final painted:Array<EditorViewportCell> = [];
	for (index in 0...cells.length) {
		final paletteCode = cells[index];
		if (paletteCode != 0)
			painted.push({x: index % width, z: Std.int(index / width), paletteCode: paletteCode});
	}
	return painted;
}

/** Clamp one presentation-only layer to a finite world height. */
function clampLayer(layerY:Int, worldHeight:Int):Int {
	if (worldHeight <= 0 || layerY < 0)
		return 0;
	return layerY < worldHeight ? layerY : worldHeight - 1;
}

/**
	Decide when the property inspector takes space from the world canvas.

	Build keeps the world large until the creator opens details or the world
	list. Plan shows a selected item immediately because that view supports
	precise inspection. Both views always show an explicitly opened panel.
**/
function inspectorVisible(buildActive:Bool, hasSelection:Bool, detailsOpen:Bool, worldListOpen:Bool):Bool
	return detailsOpen || worldListOpen || (!buildActive && hasSelection);

/** True when one semantic voxel selection crosses the displayed layer. */
function boundsIntersectLayer(bounds:VoxelBounds, layerY:Int):Bool
	return layerY >= bounds.origin.y && layerY < bounds.origin.y + bounds.size.height;

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
function paletteCodeForBlock(palette:Array<BlockPaletteEntry>, blockType:ContentId):Int {
	final expected = blockType.text();
	for (entry in palette)
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
		case 4: CheckpointTool;
		case 5: CatalogObjectTool;
		case 6: TriggerZoneTool;
		case _: null;
	};
}

/**
	Translate one tool gesture into the existing `EditorSession` command language.

	Select returns workspace bounds instead of an authored command. Paint and
	erase affect the pointed voxel. Fill carries the current bounds explicitly.
	Object tools read existing IDs and create one reloadable record or one atomic
	template. The UI never mutates a projection directly.
**/
function commandFor(tool:EditorTool, point:VoxelPoint, context:EditorToolContext):EditorToolCommandResult {
	return switch tool {
		case SelectTool:
			ToolSelectionReady({
				origin: point,
				size: {width: 1, height: 1, depth: 1}
			});
		case PaintTool:
			ToolCommandReady(PaintVoxel(point, context.paletteCode));
		case EraseTool:
			ToolCommandReady(EraseVoxel(point));
		case FillTool:
			if (context.selection == null) ToolCommandRejected(NoSelection); else ToolCommandReady(FillBounds(context.selection, context.paletteCode));
		case CheckpointTool:
			final template = checkpointTemplate(point, context.objects, context.ruleIds);
			ToolBatchReady(template.commands, template.objectId);
		case CatalogObjectTool:
			if (context.recipe == null) ToolCommandRejected(MissingEditorObjectRecipe); else {
				final templateContext:EditorObjectTemplateContext = {
					scenarioId: context.scenarioId,
					worldSize: context.worldSize,
					objects: context.objects,
					dialogueIds: context.dialogueIds,
					ruleIds: context.ruleIds
				};
				switch objectRecipeTemplate(context.recipe, point, templateContext) {
					case ObjectTemplateRejected(error): ToolCommandRejected(error);
					case ObjectTemplateReady(template) if (template.commands.length == 1): ToolCommandReady(template.commands[0]);
					case ObjectTemplateReady(template): ToolBatchReady(template.commands, template.objectId);
				}
			}
		case TriggerZoneTool:
			ToolCommandReady(triggerZoneCommand(point, context.objects));
	};
}
